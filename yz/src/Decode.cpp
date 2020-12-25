
/***********************************************************
* ��Ȩ���� (C)2020, hyz
*
* �ļ����ƣ� Decode.h
* �ļ���ʶ����
* ����ժҪ����ѹ��ʵ���ļ�
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

#include "Decode.h"

#include <string.h>
#include <iostream>
using namespace std;

Decode::Decode() {
    initData();
    //construction
}
Decode::~Decode() {
    //deconstruction
}
/*********************************************************
* ������������ʼ������
* ���������
* ���������

* ����ֵ�� void
************************************************************/

void Decode::initData() {
    memset(srcFilePath,0, sizeof(srcFilePath));
    memset(desFilePath,0, sizeof(desFilePath));
    memset(hufIndex,0, sizeof(hufIndex));
    _charFreq = NULL;				//ͳ���ַ���Ƶ�ȵ�����
    _charKind = 0;								// �ַ�����
    hfTable = NULL;			//��������
    code = NULL;									//�洢�ַ��Ĺ���������
}
/*********************************************************
* �����������ļ�·���п�
* ����������ļ�·��
* ���������bool

* ����ֵ�� bool
************************************************************/
bool Decode::isFileExist(char *path) {
    FILE *fp;

    fp = fopen(path, "rb");
    if (NULL == fp) {
        return false;
    }

    fclose(fp);
    return true;
}
/*********************************************************
* ���������������ļ���·��
* ���������
* ���������

* ����ֵ�� void
************************************************************/
void Decode::setSrcFilePath(char *path) {
    strcpy(srcFilePath, path);
}
/*********************************************************
* ���������������ļ���·��
* ���������
* ���������

* ����ֵ�� void
************************************************************/
void Decode::setDesFilePath(char *path) {
    strcpy(desFilePath, path);
}
/*********************************************************
* ������������ȡ�ļ�ͷԪ����
* �����������
* �����������

* ����ֵ�� bool
************************************************************/
bool Decode::readFileHead(){
    FILE *fp;
    fp = fopen(srcFilePath, "rb");
    //��ȡѹ�����ļ���ͷ��Ԫ���ݣ�16���ֽ�
    fread(&fileHead, sizeof(FILE_HEAD), 1, fp);
    fclose(fp);
    if(!(fileHead.flag[0] == 'h' && fileHead.flag[1] == 'y' && fileHead.flag[2] == 'z')) {
        printf("format is not hyz\n");
        return false;
    }
    return true;
}
/*********************************************************
* ������������ȡ�ַ�Ƶ��
* ���������
* ���������

* ����ֵ�� void
************************************************************/
void Decode::getCharFreq() {

    FILE *fpIn;

    _charKind = fileHead._charKind;
    _charFreq = (CHAR_FREQ *) calloc(sizeof(CHAR_FREQ), _charKind);
    fpIn = fopen(srcFilePath, "rb");
    //�Թ�ǰ16���ֽڵ�Ԫ����
    fseek(fpIn, 16, SEEK_SET);
    fread(_charFreq, sizeof(CHAR_FREQ), _charKind, fpIn);
    fclose(fpIn);

}
/*********************************************************
* ������������ʼ����������
* ���������
* ���������

* ����ֵ�� void
************************************************************/
void Decode::initHuffmanTab() {


    hfTable = (HF_TABLE *) calloc(sizeof(HF_TABLE), 2 * _charKind - 1);
    //hfTable������ 2 * _charKind - 1��С�Ŀռ䣬����ֻ���� _charKind������ʣ_charKind - 1��
    for(int i = 0; i < _charKind; i++) {
        hufIndex[_charFreq[i]._char] = i;	//�ѹ��������е��ַ������Ӧ���±��γɼ�ֵ��,�浽hufIndex��
        hfTable[i]._charFreq = _charFreq[i];
        hfTable[i].left = hfTable[i].right = -1;
        hfTable[i].visited = false;
        hfTable[i].code = (char *) calloc(sizeof(char), _charKind);
    }


}
/*********************************************************
* ������������ȡƵ����С���ַ�����
* ���������
* ���������

* ����ֵ�� int ����
************************************************************/
int Decode::getMinFreq(int count) {
    int index;
    int minIndex = NOT_INIT;

    for(index = 0; index < count; index++) {
        if(false == hfTable[index].visited) {
            if(NOT_INIT == minIndex || hfTable[index]._charFreq.freq < hfTable[minIndex]._charFreq.freq) {
                minIndex = index;
            }
        }
    }
    hfTable[minIndex].visited = true;

    return minIndex;
}
/*********************************************************
* ����������������������
* ���������
* ���������

* ����ֵ�� void
************************************************************/

void Decode::creatHuffmanTree() {
    int left;
    int right;

    //hfTableʹ��ʣ�µ� _charKind - 1���ռ�
    for (int i = 0; i < _charKind - 1; i++) {
        left = getMinFreq(_charKind + i);
        right = getMinFreq(_charKind + i);
        hfTable[_charKind + i]._charFreq._char = '#';
        hfTable[_charKind + i]._charFreq.freq = hfTable[left]._charFreq.freq + hfTable[right]._charFreq.freq;
        hfTable[_charKind + i].left = left;
        hfTable[_charKind + i].right = right;
        hfTable[_charKind + i].visited = false;
        //cout<<hfTable[_charKind + i]._charFreq.freq<<" "<<hfTable[_charKind + i]._charFreq._char<<endl;
    }

}
/*********************************************************
* ������������������������
* ���������
* ���������

* ����ֵ�� void
************************************************************/
void Decode::makeHuffmanCode(int root, int index) {
    if (hfTable[root].left != -1 && hfTable[root].right != -1) {
        code[index] = '1';
        makeHuffmanCode(hfTable[root].left, index + 1);
        code[index] = '0';
        makeHuffmanCode(hfTable[root].right, index + 1);
    } else {
        code[index] = 0;//'\0'��ASCII����0
        strcpy(hfTable[root].code, code);
    }


}
/*********************************************************
* ���������������������
* ���������
* ���������

* ����ֵ�� void
************************************************************/

void Decode::dispHuffmanTab() {
    printf("%-4s %-4s %-4s %-6s %-6s %-6s %s\n", "�±�", "�ַ�", "Ƶ��", "����", "�Һ���", "visited", "code");
    for (int i = 0; i < 2*_charKind-1; i++) {
        printf("%-5d %-4c %-5d %-6d %-7d %-4d %s\n",
               i,
               hfTable[i]._charFreq._char,
               hfTable[i]._charFreq.freq,
               hfTable[i].left,
               hfTable[i].right,
               hfTable[i].visited,
               (hfTable[i].code ? hfTable[i].code : "��"));
    }
}
/*********************************************************
* ����������д���ѹ�ļ�
* ���������
* ���������

* ����ֵ�� void
************************************************************/
void Decode::huffmanDecoding() {
    int root = 2 * _charKind - 2;
    FILE *fpIn;
    FILE *fpOut;
    bool    finished = false;
    unsigned char val;
    unsigned char outValue;
    int index = 0;
    long fileSize;
    long curLocation;

    fpIn = fopen(srcFilePath, "rb");
    fpOut = fopen(desFilePath, "wb");
    fseek(fpIn, 0L, SEEK_END);
    fileSize = ftell(fpIn);	//�ļ��ܳ���fileSize
    fseek(fpIn, 16 + 5 * fileHead._charKind, SEEK_SET);	//�Թ�ǰ��16���ֽڵ�Ԫ���ݣ�5�ֽڵ��ַ������Ƶ��
   // fseek(fpIn,  + 5 * fileHead._charKind, SEEK_SET);
    curLocation = ftell(fpIn);

    //�Ӹ�������'1'���������ߣ�'0'���������ߣ�������Ҷ�ӽ�㣬���Ҷ�ӽ���±��Ӧ���ַ����ٻص�����������
    fread(&val, sizeof(unsigned char), 1, fpIn);
    while(!finished) {
        if(hfTable[root].left == -1 && hfTable[root].right == -1) {
            outValue = hfTable[root]._charFreq._char;
            fwrite(&outValue, sizeof(unsigned char), 1, fpOut);
            if(curLocation >= fileSize && index >= fileHead.lastValidBit) {
                break;
            }
            root = 2 * _charKind - 2;
        }
        //ȡ����һ���ֽڴӵ�һλ��ʼ����'1'���������ߣ�'0'����������
        //������һ���ֽڣ�8λ������Ҫ��ȡ��һ���ֽ�
        if(GET_BYTE(val, index)) {
            root = hfTable[root].left;
        } else {
            root = hfTable[root].right;
        }
        if(++index >= 8) {
            index = 0;
            fread(&val, sizeof(unsigned char), 1, fpIn);
            curLocation = ftell(fpIn);
        }
    }

    fclose(fpIn);
    fclose(fpOut);

}
/*********************************************************
* ������������������������ѹ��ִ�й���
* ���������
* ���������

* ����ֵ�� void
************************************************************/
void Decode::Main(char *src, char *des) {
    if (!isFileExist(src)) {
        cout << "Fail because of src path error" << endl;
        return;
    }
    cout << "20% Doing initData" << endl;
    initData();
    setSrcFilePath(src);
    setDesFilePath(des);
    cout << "43% Doing readFileHead" << endl;
    if(!readFileHead()){
        cout << "Fail because of wrong fileHead" << endl;
        return;
    }
    cout << "50% Doing getCharFreq" << endl;
    getCharFreq();
    initHuffmanTab();
    cout << "68% Doing creatHuffmanTree" << endl;
    creatHuffmanTree();
    code = (char *) calloc(sizeof(char), _charKind);
    cout << "81 %Doing makeHuffmanCode" << endl;
    makeHuffmanCode(2 * _charKind - 2, 0);
//    dispHuffmanTab();
    cout << "90% Doing huffmanDecoding" << endl;
    huffmanDecoding();
    cout<<"100% export to "<<desFilePath<<endl;
}

