
/***********************************************************
* 版权所有 (C)2020, hyz
*
* 文件名称： Decode.h
* 文件标识：无
* 内容摘要：公共头文件，存放存储数据结构体
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
#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
typedef struct FILE_HEAD {
    unsigned char flag[3];
    int _charKind;		//字符种类---------------------------
    unsigned char lastValidBit;		//最后一个字节的有效位数
    unsigned char unused[8];			//待用空间
} FILE_HEAD;								//这个结构体总共占用16个字节的空间

typedef struct CHAR_FREQ {
    unsigned char _char;		//字符,考虑到文件中有汉字，所以定义成unsigned char
    int freq;								//字符出现的频度
} CHAR_FREQ;

typedef struct HF_TABLE {
    CHAR_FREQ _charFreq;
    int left;
    int right;
    bool visited;
    char *code;
} HF_TAB;


//得到index位的值，若index位为0，则GET_BYTE值为假，否则为真
#define GET_BYTE(vbyte, index) (((vbyte) & (1 << ((index) ^ 7))) != 0)
//index位置1
#define SET_BYTE(vbyte, index) ((vbyte) |= (1 << ((index) ^ 7)))
//index位置0
#define CLR_BYTE(vbyte, index) ((vbyte) &= (~(1 << ((index) ^ 7))))


#define NOT_INIT		-1



#endif // COMMON_H_INCLUDED
