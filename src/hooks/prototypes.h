int __libc_start_main(int *(main) (int, char * *, char * *), int argc, char * * ubp_av, void (*init) (void), void (*fini) (void), void (*rtld_fini) (void), void (* stack_end));
typeof(__libc_start_main) *o_libc_start_main;

/*
      __ _ _                                                                     
     / _(_) |                                  _                                 
    | |_ _| | ___    ___  _ __   ___ _ __    _| |_    __ _  ___ ___ ___  ___ ___ 
    |  _| | |/ _ \  / _ \| '_ \ / _ \ '_ \  |_   _|  / _` |/ __/ __/ _ \/ __/ __|
    | | | | |  __/ | (_) | |_) |  __/ | | |   |_|   | (_| | (_| (_|  __/\__ \__ \
    |_| |_|_|\___|  \___/| .__/ \___|_| |_|          \__,_|\___\___\___||___/___/
                         | |                                                     
                         |_|                                                     
 */
int open(const char *pathname, int flags, mode_t mode);     /* open() & open64() */
typeof(open) *o_open;
int open64(const char *pathname, int flags, mode_t mode);
typeof(open64) *o_open64;
FILE *fopen(const char *pathname, const char *mode);        /* fopen() & fopen64() */
typeof(fopen) *o_fopen;
FILE *fopen64(const char *pathname, const char *mode);
typeof(fopen64) *o_fopen64;
int access(const char *pathname, int amode);                /* access() */
typeof(access) *o_access;
int creat(const char *pathname, mode_t mode);               /* & creat() */
typeof(creat) *o_creat;

int execve(const char *filename, char *const argv[], char *const envp[]);
typeof(execve) *o_execve;
int execvp(const char *filename, char *const argv[]);
typeof(execvp) *o_execvp;

int mkdir(const char *pathname, mode_t mode);    /* mkdir() */
typeof(mkdir) *o_mkdir;
int rmdir(const char *pathname);                 /* rmdir() */
typeof(rmdir) *o_rmdir;

struct dirent *readdir(DIR *dirp);               /* readdir() */
typeof(readdir) *o_readdir;
struct dirent64 *readdir64(DIR *dirp);           /* & readdir64() */
typeof(readdir64) *o_readdir64;

DIR *opendir(const char *pathname);              /* opendir() */
typeof(opendir) *o_opendir;
DIR *opendir64(const char *pathname);            /* & opendir64() */
typeof(opendir64) *o_opendir64;

DIR *fdopendir(int fd);                          /* fdopendir() */
typeof(fdopendir) *o_fdopendir;
int chdir(const char *pathname);                 /* chdir() */
typeof(chdir) *o_chdir;
int fchdir(int fd);                              /* & fchdir() */
typeof(fchdir) *o_fchdir;
int chroot(const char *path);                    /* chroot() */
typeof(chroot) *o_chroot;

ssize_t read(int fd, void *buf, size_t nbyte);
typeof(read) *o_read;
ssize_t write(int fd, const void *buf, size_t nbyte);
typeof(write) *o_write;

int stat(const char *pathname, struct stat *buf);
typeof(stat) *o_stat;
int stat64(const char *pathname, struct stat64 *buf);
typeof(stat64) *o_stat64;

int fstat(int fd, struct stat *buf);
typeof(fstat) *o_fstat;
int fstat64(int fd, struct stat64 *buf);
typeof(fstat64) *o_fstat64;

int lstat(const char *pathname, struct stat *buf);
typeof(lstat) *o_lstat;
int lstat64(const char *pathname, struct stat64 *buf);
typeof(lstat64) *o_lstat64;

int __xstat(int version, const char *pathname, struct stat *buf);
typeof(__xstat) *o_xstat;
int __xstat64(int version, const char *pathname, struct stat64 *buf);
typeof(__xstat64) *o_xstat64;
int __fxstat(int version, int fd, struct stat *buf);
typeof(__fxstat) *o_fxstat;
int __fxstat64(int version, int fd, struct stat64 *buf);
typeof(__fxstat64) *o_fxstat64;
int __lxstat(int version, const char *pathname, struct stat *buf);
typeof(__lxstat) *o_lxstat;
int __lxstat64(int version, const char *pathname, struct stat64 *buf);
typeof(__lxstat64) *o_lxstat64;

int unlink(const char *pathname);
typeof(unlink) *o_unlink;
int unlinkat(int dirfd, const char *pathname, int flags);
typeof(unlinkat) *o_unlinkat;
int chown(const char *pathname, uid_t owner, gid_t group);
typeof(chown) *o_chown;
int chmod(const char *pathname, mode_t mode);
typeof(chmod) *o_chmod;
int fchmod(int fd, mode_t mode);
typeof(fchmod) *o_fchmod;
int fchmodat(int dirfd, const char *pathname, mode_t mode, int flags);
typeof(fchmodat) *o_fchmodat;