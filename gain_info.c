#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define EXIT_FAILURE -1
int gain_info(const char *path) ;
int main(int argc, char **argv)
{
    if(argc<2)
    {
        printf("正确格式:./a.out <filename>");
        return EXIT_FAILURE;
    }
    gain_info(argv[1]);
}
int gain_info(const char *path)
{
    int fag=0;
    struct stat fileinfo;
    if(stat(path,&fileinfo)!=0)
    {
        printf("状态读取错误\n");
        return EXIT_FAILURE;
    }
     switch (fileinfo.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  printf("directory\n");               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symlink\n");                 break;
           case S_IFREG:  printf("regular file\n");            break;
           case S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
           }

    
}
