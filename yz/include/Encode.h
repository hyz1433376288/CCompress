
/***********************************************************
* ��Ȩ���� (C)2020, hyz
*
* �ļ����ƣ� Decode.h
* �ļ���ʶ����
* ����ժҪ��ѹ����ͷ�ļ�
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
#ifndef ENCODE_H
#define ENCODE_H


#pragma pack(push)
#pragma pack(1)		//�ڴ�����Ϊ1���ֽڶ���ģʽ

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "COMMON.h"



class Encode {
public:
    Encode();//���캯��
    virtual ~Encode();//��������
    void initData();//���ݶ����ʼ��
    bool isFileExist(char *path);//·���п�
    void setSrcFilePath(char *path);//����Դ�ļ�·��
    void setDesFilePath(char *path);//����Ŀ���ļ�·��
    void getCharFreq();//��ȡ�ַ�Ƶ��
    void initHuffmanTab();//��ʼ����������
    void creatHuffmanTree();////������������
    void makeHuffmanCode(int ,int );//�������������
    void dispHuffmanTab();//�����������
    void huffmanEncoding();//ѹ���ļ�
    void Main(char *src,char *des);//��������ִ��

//˽�з���
private:
    int getMinFreq(int );//��ȡ��Сfreq���ַ�
    int getlastValidBit();//��ȡ�ļ���ĩβ��Чλ
//���ݶ���
private:
    char srcFilePath[256] = {0};//
    char desFilePath[256] = {0};
    CHAR_FREQ *_charFreq = NULL;//ͳ���ַ���Ƶ�ȵ�����
    int _charKind = 0;			// �ַ�����
    HF_TABLE *hfTable = NULL;	//��������
    char *code = NULL;			//�洢�ַ��Ĺ���������
    int hufIndex[256] = {0};	//�±�Ϊ�ַ���ASCII�룬��ֵΪ���ַ��ڹ��������е��±꣬�γɼ�ֵ��

};


#endif // ENCODE_H
