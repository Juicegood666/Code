#include <stdio.h>
int main(int argc,char **argv)
{
    long num=0;
    if(argc!=2)
    {
        printf("正确打开方式:./a.out <filename>");
        return 0;
    }
    FILE *fp=fopen(argv[1],"rb");
    if(fp==NULL)
    {
        printf("打开文件失败\n");
        return 0;
    }
    fseek(fp,0,2);
    num=ftell(fp);
    printf("%d\n",num);
    fclose(fp);
    
}
