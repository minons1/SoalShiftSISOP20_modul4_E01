#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdlib.h>


static const char *dirpath = "/home/salim/Documents";
static const char *key='8,GwP_ND|jO9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI\'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[Lm';

char swap(char x,int move){
    int i;
    for(i=0;i<strlen(key);i++){
        if(x==key[i]){
            return key[i+move];
        }
    }
}

void encv_1(char *encrypt){
    if(!strcmp(encrypt,".") || !strcmp(encrypt,"..")) return;
    int i;
    for(i=0;i<strlen(encrypt);i++){
        encrypt[i]=swap(encrypt[i],10);
    }
}

void decv_1(char *decrypt){
    if(!strcmp(encrypt,".") || !strcmp(encrypt,"..")) return;
    int i=0;
    if(decrypt[i]=='/'){
        i++;
    }
    for(;i<strlen(encrypt);i++){
        decrypt[i]=swap(decrypt[i],-10);
    }
}

static  int  xmp_getattr(const char *path, struct stat *stbuf){
    int res;
    res = lstat(path, stbuf);
    if (res == -1){
        return -errno;
    }
    return 0;
}

  
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi){
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;
    dp = opendir(path);
    if (dp == NULL){
        return -errno;
    }    
    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0)){
            break;
        }
    }

    closedir(dp);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi){
    int fd;
    int res;
    (void) fi;
    fd = open(path, O_RDONLY);
    if (fd == -1){
        return -errno;
    }
    res = pread(fd, buf, size, offset);
    if (res == -1){
        res = -errno;
    }
    close(fd);
    return res;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
};

int  main(int  argc, char *argv[]){
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}