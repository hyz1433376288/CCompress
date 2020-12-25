
/***********************************************************
* 版权所有 (C)2020, hyz
*
* 文件名称： main.cpp
* 文件标识：无
* 内容摘要：工程入口
* 其它说明：无
* 当前版本： V1.0
* 作   者：侯言志
* 完成日期： 20201224
*
* 修改记录1：
* 修改日期： 20201224
* 版本号： V1.0
* 修改人： 侯言志
* 修改内容：创建
**********************************************************/
#include <iostream>
#include "Encode.h"
#include "Decode.h"
using namespace std;

void file_size(char *srcFilePath,char *desFilePath,int flag){
    FILE *fp = fopen(srcFilePath,"r");

    fseek(fp,0L,SEEK_END);
    double srcSize = (double)ftell(fp);
    fclose(fp);

    fp = fopen(desFilePath,"r");

    fseek(fp,0L,SEEK_END);
    double desSize = (double)ftell(fp);
    fclose(fp);
    printf("before:%.2fK\n",srcSize/1024);
    printf("after:%.2fK\n",desSize/1024);
    if(flag == 1){
        printf("rate:%.2f%%\n",(1-desSize/srcSize)*100);
    }
}
int main(int argc, char **argv) {
    if(argc != 4){
        cout<<"yz -e /srcFilePath /desFilePath"<<endl;
        cout<<"yz -d /srcFilePath /desFilePath"<<endl;
        return 0;
    }
    for(int i=0;i<argc;i++){
        printf("%s\n",argv[i]);
    }
    char src[200];
    char des[200];
    strcpy(src,argv[2]);
    strcpy(des,argv[3]);
    printf("%s %s\n",src,des);

    Encode ec;
    Decode dc;

    //如果是压缩
    if(!strcmp(argv[1],"-e")){
//        ec.Main("D:\\Qt\\myQt\\demo1\\CCompress_copycore\\lz.bmp","D:\\Qt\\myQt\\demo1\\CCompress_copycore\\lz.bmp.yz");
        ec.Main(src,des);
        file_size(src,des,1);
    }
    //如果是解压缩
    else if(!strcmp(argv[1],"-d")){
//        dc.Main("D:\\Qt\\myQt\\demo1\\CCompress_copycore\\lz.bmp.yz","D:\\Qt\\myQt\\demo1\\CCompress_copycore\\lzDecode.bmp");
        dc.Main(src,des);
        file_size(src,des,0);
    }
    else{
        cout<<"yz -e /srcFilePath /desFilePath"<<endl;
        cout<<"yz -d /srcFilePath /desFilePath"<<endl;
        return 0;
    }
    cout<<"-------------OVER---------------"<<endl;
    return 0;
}
