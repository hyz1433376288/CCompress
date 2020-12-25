
/***********************************************************
* 版权所有 (C)2020, hyz
*
* 文件名称： Decode.h
* 文件标识：无
* 内容摘要：解压类头文件
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
#ifndef DECODE_H
#define DECODE_H


#pragma pack(push)
#pragma pack(1)		//内存一个字节对齐，1,2,4,8,16

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "COMMON.h"

class Decode {
public:
    Decode();//构造函数
    virtual ~Decode();//析构函数
    void initData();//数据对象初始化
    bool isFileExist(char *);//路径判空
    void setSrcFilePath(char *);//设置源文件路径
    void setDesFilePath(char *);//设置目标文件路径
    bool readFileHead();//读取文件头部的元数据
    void getCharFreq();//获取字符频度
    void initHuffmanTab();//初始化哈夫曼表
    void creatHuffmanTree();//重建哈夫曼树
    void makeHuffmanCode(int ,int );//构造哈夫曼编码
    void dispHuffmanTab();//输出哈夫曼表
    void huffmanDecoding();//解压文件
    void Main(char *src, char *des);//主方法

private:
    int getMinFreq(int );//获取最小freq的字符

//数据对象
private:
    char srcFilePath[256] = {0};
    char desFilePath[256] = {0};
    CHAR_FREQ *_charFreq = NULL;				//统计字符及频度的数组
    int _charKind = 0;								// 字符种类
    HF_TABLE *hfTable = NULL;			//哈夫曼表
    char *code = NULL;									//存储字符的哈夫曼编码
    int hufIndex[256] = {0};						//下标为字符的ASCII码，其值为该字符在哈夫曼表中的下标，形成键值对
    FILE_HEAD fileHead;
};


#endif // DECODE_H
