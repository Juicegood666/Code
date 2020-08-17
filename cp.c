#include <stdio.h>
int main(int argc,char **argv)
{
    unsigned char buff[100];
    int cnt=0;
    if(argc<3)
    {
        printf("正确格式:./a.out <filename1> <filename2>");
        return 0;
    }
    FILE *fp1=fopen(argv[1],"rb");
    if(fp1==NULL)
    {
        printf("%s打开文件失败\n",argv[1]);
        return 0;
    }
    FILE *fp2=fopen(argv[2],"wb");
    if(fp2==NULL)
    {
        printf("%s创建文件失败",argv[2]);
        return 0;
    }
    int num=0;
    while(1)
    {
        cnt=fread(buff,1,sizeof(buff),fp1);
        num=cnt;
        printf("%d\n",num);
        fwrite(buff,1,cnt,fp2);
        if(cnt!=sizeof(buff))
        {
            break;
        }
    }
    printf("%s->%s拷贝成功\n",argv[1],argv[2]);
    fclose(fp1);
    fclose(fp2);
    return 0;   
}
