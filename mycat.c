#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc,char**argv)
{
    char data=0;
    int cnt=1;
    char flag=1;
    if(argc<2)
    {
        printf("正确格式:./a.out <filename> [选项参数]");
        return 0;
    }
    FILE *fp=fopen(argv[1],"rb");
    if(fp==NULL)
    {
        printf("%s打开文件失败\n",argv[1]);
        return 0;
    }
    while(1)
    {
        data = fgetc(fp);
        if(argv[2]!=NULL)
        {
            if(strstr(argv[2],"-n"))
            {
                if (flag)
                {
                    printf("%d ",cnt);
                    
                    flag = 0;
                    cnt++;
                }
                if (feof(fp))
                {
                    printf("以上是%s的内容\n", argv[1]);
                    break;
                }
                fputc(data, stdout);
                if (data == '\n')
                {
                    printf("%d ", cnt);
                    cnt++;
                }
            }
        }
        else
        {
            if (feof(fp))
            {
                printf("以上是%s的内容\n", argv[1]);
                break;
            }
            fputc(data, stdout);
        }
    }
}