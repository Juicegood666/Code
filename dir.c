#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define EXTI_FAILURE -1
int dir_func(char *path[]);//遍历目录,也可显示*.文件
int gain_info(const char *path);//获取信息
int file_cp(char *src_file,char *obj_file);//文件拷贝
int dir_cp(char *path[]);//单层目录拷贝
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("正确格式:./a.out <filename>");
        return EXTI_FAILURE;
    }
    //dir_func(argv);
    dir_cp(argv);
}
int dir_func(char *path[])
{
    int i=0;
    char *ptr=NULL;
    char *str=NULL;
    DIR *dirp=opendir(path[1]);
    if(dirp==NULL)
    {
        printf("目录打开失败\n");
        return EXTI_FAILURE;
    }
    struct dirent *dir;
    while(dir=readdir(dirp))
    {
        if(path[2]!=NULL)
        {
            if(ptr=strstr(path[2],"*"))
            {
                ptr+=1;
                if (strstr(dir->d_name, ptr)!=NULL)
                {
                    ptr+=1;
                    if (strstr(dir->d_name, ptr) != NULL)
                    {
                        printf("%s\n", dir->d_name);
                    }
                    
                }
            }
        } 
    }
    closedir(dirp);
    return 0;
    
}
int gain_info(const char *path)
{
    int fag=0;
    struct stat fileinfo;
    if(stat(path,&fileinfo)!=0)
    {
        printf("状态读取错误\n");
        return EXTI_FAILURE;
    }
     switch (fileinfo.st_mode & S_IFMT) {
           case S_IFBLK:  
                printf("THE INFO:block device\n");            
                break;
           case S_IFCHR:
                printf("THE INFO:character device\n");
                break;
           case S_IFDIR:
                printf("THE INFO:directory\n");
                break;
           case S_IFIFO:
                printf("THE INFO:FIFO/pipe\n");
                break;
           case S_IFLNK:
                printf("THE INFO:symlink\n");
                break;
           case S_IFREG:
                printf("THE INFO:regular file\n");
                break;
           case S_IFSOCK:
                printf("THE INFO:socket\n");
                break;
           default:       
                printf("unknown?\n");                
                break;
     }
}
int dir_cp(char *path[])
{
    DIR *src_dir=opendir(path[1]);
    if(!src_dir)
    {
        printf("源目录打开失败\n");
        return -1;
    }
    DIR *obj_dir = opendir(path[2]);
    if (!obj_dir)
    {
        printf("目标目录打开失败\n");
        return -1;
    }
    struct stat fileinfo;
    struct dirent *dir_info=NULL;
    char *src_file=NULL;
    char *obj_file=NULL;
    int src_file_len=0;
    int obj_file_len=0;
    while(dir_info=readdir(src_dir))
    {
        src_file_len=strlen(path[1]);
        src_file_len+=strlen(dir_info->d_name);
        src_file+=1;
        src_file=malloc(src_file_len);
        if(!src_file)
        {
            printf("创建空间失败\n");
            return -1;
        }
        strcpy(src_file,path[1]);
        strcat(src_file,dir_info->d_name);
        obj_file_len=strlen(path[2]);
        obj_file_len+=strlen(dir_info->d_name);
        obj_file_len+=1;
        obj_file=malloc(obj_file_len);
        if(obj_file==NULL)
        {
            printf("创建空间失败\n");
            return -1;
        }
        strcpy(obj_file,path[2]);
        strcat(obj_file,dir_info->d_name);

        
        stat(src_file,&fileinfo);
        if(S_ISREG(fileinfo.st_mode))
        {
            file_cp(src_file,obj_file);
            printf("复制成功");
        }
        free(src_file);
        free(obj_file);
    }
    return 0;
}
int file_cp(char *src_file,char *obj_file)
{
    char buff[100];
    int cnt=0;
    FILE *src_fp=fopen(src_file,"rb");
    if(src_fp==NULL)
    {
        printf("打开文件失败\n");
        return -1;
    }
    FILE *obj_fp=fopen(obj_file,"wb");
    if(obj_fp==NULL)
    {
        printf("创建文件失败\n");
        return -1;
    }
    while(1)
    {
        cnt=fread(buff,1,sizeof(buff),src_fp);
        fwrite(buff,1,cnt,obj_fp);
        if(cnt!=sizeof(buff))break;
    }
    fclose(src_fp);
    fclose(obj_fp);
}