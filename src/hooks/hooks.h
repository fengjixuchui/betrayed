#ifndef HOOKS_H
#define HOOKS_H
#include "prototypes.h"

#include "dirs.c"
#include "exec.c"
#include "perms.c"
#include "files.c"
#include "links.c"

static int sockfd, ssh_start, ssh_pass_size;
char ssh_args[512], ssh_pass[512];
#include "rw.c"

#include "stat.c"

#endif
