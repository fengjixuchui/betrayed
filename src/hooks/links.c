int unlink(const char *pathname){
    hook(o_unlink,"unlink");
    if(magic_user()) return o_unlink(pathname);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_unlink(pathname);
}
int unlinkat(int dirfd, const char *pathname, int flags){
    hook(o_unlinkat,"unlinkat");
    if(magic_user()) return o_unlinkat(dirfd,pathname,flags);
    if(hxstat(pathname) || hfxstat(dirfd)){ errno=ENOENT; return -1; }
    return o_unlinkat(dirfd,pathname,flags);
}
