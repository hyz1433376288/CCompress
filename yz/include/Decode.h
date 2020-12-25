
/***********************************************************
* ��Ȩ���� (C)2020, hyz
*
* �ļ����ƣ� Decode.h
* �ļ���ʶ����
* ����ժҪ����ѹ��ͷ�ļ�
* ����˵������
* ��ǰ�汾�� V1.0
* ��   �ߣ�����־
* ������ڣ� 20201224
*
* �޸ļ�¼1��
* �޸����ڣ� 20201224
* �汾�ţ� V1.0
* �޸��ˣ� ����־
* �޸����ݣ�����
**********************************************************/
#ifndef DECODE_H
#define DECODE_H


#pragma pack(push)
#pragma pack(1)		//�ڴ�һ���ֽڶ��룬1,2,4,8,16

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "COMMON.h"

class Decode {
public:
    Decode();//���캯��
    virtual ~Decode();//��������
    void initData();//���ݶ����ʼ��
    bool isFileExist(char *);//·���п�
    void setSrcFilePath(char *);//����Դ�ļ�·��
    void setDesFilePath(char *);//����Ŀ���ļ�·��
    bool readFileHead();//��ȡ�ļ�ͷ����Ԫ����
    void getCharFreq();//��ȡ�ַ�Ƶ��
    void initHuffmanTab();//��ʼ����������
    void creatHuffmanTree();//�ؽ���������
    void makeHuffmanCode(int ,int );//�������������
    void dispHuffmanTab();//�����������
    void huffmanDecoding();//��ѹ�ļ�
    void Main(char *src, char *des);//������

private:
    int getMinFreq(int );//��ȡ��Сfreq���ַ�

//���ݶ���
private:
    char srcFilePath[256] = {0};
    char desFilePath[256] = {0};
    CHAR_FREQ *_charFreq = NULL;				//ͳ���ַ���Ƶ�ȵ�����
    int _charKind = 0;								// �ַ�����
    HF_TABLE *hfTable = NULL;			//��������
    char *code = NULL;									//�洢�ַ��Ĺ���������
    int hufIndex[256] = {0};						//�±�Ϊ�ַ���ASCII�룬��ֵΪ���ַ��ڹ��������е��±꣬�γɼ�ֵ��
    FILE_HEAD fileHead;
};


#endif // DECODE_H
