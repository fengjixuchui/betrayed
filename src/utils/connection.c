int read_line(int sock, char *buffer){
    size_t length = 0;
    int result;
    while(1){
        char data;
        result = recv(sock, &data, 1, 0);
        if((result <= 0) || (data == EOF)) exit(1);
        buffer[length] = data;
        length++;

        if(length >= 2 && buffer[length - 2] == '\r' && buffer[length - 1] == '\n'){
            buffer[length - 2] = '\0';
            return length;
        }
    }
}

char *get_prefix(char *line){
    char clone[512], *prefix = (char *)malloc(512);
    strncpy(clone, line, strlen(line) + 1);
    if(line[0] == ':'){
        char *splitted = strtok(clone," ");
        if(splitted != NULL) strncpy(prefix, splitted + 1, strlen(splitted) + 1);
        else prefix[0] = '\0';
    }else prefix[0] = '\0';
    return prefix;
}

char *get_username(char *line){
    char clone[512], *username = (char *)malloc(512);
    strncpy(clone, line, strlen(line) + 1);

    if(strchr(clone, '!') != NULL){
        char *splitted = strtok(clone, "!");
        if(splitted != NULL) strncpy(username, splitted + 1, strlen(splitted) + 1);
        else username[0] = '\0';
    }else username[0] = '\0';
    return username;
}

char *get_command(char line[]){
    char clone[512], *command = (char *)malloc(512), *splitted;
    strncpy(clone, line, strlen(line)+1);
    splitted = strtok(clone, " ");

    if(splitted != NULL){
        if(splitted[0] == ':') splitted = strtok(NULL, " ");
        if(splitted != NULL) strncpy(command, splitted, strlen(splitted) + 1);
        else command[0] = '\0';
    }else command[0] = '\0';
    return command;
}

char *get_last_argument(char line[]){
    char clone[512], *argument = (char *)malloc(512), *splitted;
    strncpy(clone, line, strlen(line) + 1);
    splitted = strstr(clone, " :");
    if(splitted != NULL) strncpy(argument, splitted + 2, strlen(splitted) + 1);
    else argument[0] = '\0';
    return argument;
}

char *get_argument(char line[], int argno){
    char clone[512], *argument = (char *)malloc(512), *splitted;
    int current_arg = 0;
    strncpy(clone, line, strlen(line) + 1);
    splitted = strtok(clone, " ");
    while(splitted != NULL){
        if(splitted[0] != ':') current_arg++;

        if(current_arg == argno){
            strncpy(argument, splitted, strlen(splitted) + 1);
            return argument;
        }

        splitted = strtok(NULL, " ");
    }
    if(current_arg != argno) argument[0] = '\0';
    return argument;
}

void set_nick(int sock, char nick[]){
    char nick_packet[512];
    sprintf(nick_packet, "NICK %s\r\n", nick);
    send(sock, nick_packet, strlen(nick_packet), 0);
}

void send_user_packet(int sock, char nick[]){
    char user_packet[512];
    sprintf(user_packet, "USER %s 0 * %s\r\n", nick, nick);
    send(sock, user_packet, strlen(user_packet), 0);
}

void join_channel(int sock, char channel[]){
    char join_packet[512];
#ifndef CHPASS
    sprintf(join_packet, "JOIN %s\r\n", channel);
#else
    sprintf(join_packet, "JOIN %s %s\r\n", channel, CHPASS);
#endif
    send(sock, join_packet, strlen(join_packet), 0);
}

void send_pong(int sock, char argument[]){
    char pong_packet[512];
    sprintf(pong_packet, "PONG :%s\r\n", argument);
    send(sock, pong_packet, strlen(pong_packet), 0);
}

void send_message(int sock, char to[], char message[]){
    char message_packet[512];
    sprintf(message_packet, "PRIVMSG %s :%s\r\n", to, message);
    send(sock, message_packet, strlen(message_packet), 0);
}

int setup_connection(void){
    struct addrinfo hints, *res;
    int s;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int rc = getaddrinfo(HOST, PORT, &hints, &res);
    if(rc < 0)
        exit(0);

    s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(s < 0)
        goto fail;

    if(connect(s, res->ai_addr, res->ai_addrlen) < 0)
        goto fail;
    freeaddrinfo(res);

    set_nick(s, NICK);
    send_user_packet(s, NICK);
    return s;
fail:
    freeaddrinfo(res);
    exit(0);
}
