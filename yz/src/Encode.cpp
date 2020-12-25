
/***********************************************************
* ��Ȩ���� (C)2020, hyz
*
* �ļ����ƣ� Decode.h
* �ļ���ʶ����
* ����ժҪ��ѹ����ʵ���ļ�
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
#include "Encode.h"
#include "COMMON.h"
#include <strings.h>
#include <iostream>

using namespace std;

Encode::Encode() {
    //construction
    initData();
}

Encode::~Encode() {
    //deconstruction
}
/*********************************************************
* ������������ʼ������
* ���������
* ���������

* ����ֵ�� void
************************************************************/
void Encode::initData() {
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
* ���������

* ����ֵ�� bool
************************************************************/
bool Encode::isFileExist(char *path) {
    FILE *fp;

    fp = fopen(path, "rb");
    if (NULL == fp) {
        return false;
    }

    fclose(fp);
    return true;
}

void Encode::setSrcFilePath(char *path) {
    strcpy(srcFilePath, path);
}
/*********************************************************
* ��������������·��
* ���������
* ���������

* ����ֵ�� void
************************************************************/

void Encode::setDesFilePath(char *path) {
    strcpy(desFilePath, path);
}
/*********************************************************
* ������������ȡ�ַ�Ƶ��
* ���������
* ���������

* ����ֵ�� void
************************************************************/

void Encode::getCharFreq() {
    int freq[256] = {0};

    int index;
    _charFreq = NULL;
    FILE *fpIn;
    int ch;

    fpIn = fopen(srcFilePath, "rb");

    /*ͳ�������ַ���Ƶ��*/
    ch = fgetc(fpIn);
    while (!feof(fpIn)) {
        freq[ch]++;
        ch = fgetc(fpIn);
    }
    fclose(fpIn);

    /*ͳ�������ַ�������*/
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            _charKind++;
        }
    }

    _charFreq = (CHAR_FREQ *) calloc(sizeof(CHAR_FREQ), _charKind);
    for (int i = index = 0; i < 256; i++) {
        if (freq[i]) {
            _charFreq[index]._char = i;
            _charFreq[index].freq = freq[i];
            index++;
        }
    }

}
/*********************************************************
* ������������ʼ����������
* ���������
* ���������

* ����ֵ�� void
************************************************************/
void Encode::initHuffmanTab() {

    hfTable = (HF_TABLE *) calloc(sizeof(HF_TABLE), 2 * _charKind - 1);
    //hfTable������ 2 * _charKind - 1��С�Ŀռ䣬����ֻ���� _charKind������ʣ_charKind - 1��,�����ϲ��ڵ�
    for (int i = 0; i < _charKind; i++) {
        hufIndex[_charFreq[i]._char] = i;    //�ѹ��������е��ַ������Ӧ���±��γɼ�ֵ��,�浽hufIndex��
        hfTable[i]._charFreq = _charFreq[i];
        hfTable[i].left = hfTable[i].right = -1;
        hfTable[i].visited = false;//��¼�Ƿ����
        hfTable[i].code = (char *) calloc(sizeof(char), _charKind);//���ٿռ�
    }

}
/*********************************************************
* ������������ȡ�ַ���СƵ������
* ���������
* ���������

* ����ֵ�� int
************************************************************/
int Encode::getMinFreq(int count) {
    int index;
    int minIndex = NOT_INIT;

    for (index = 0; index < count; index++) {
        if (false == hfTable[index].visited) {
            if (NOT_INIT == minIndex || hfTable[index]._charFreq.freq < hfTable[minIndex]._charFreq.freq) {
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
void Encode::creatHuffmanTree() {
    int left;
    int right;

    //ʹ��_charKind-1�Ŀռ䴴��_char Kind�����ڵ�
    for (int i = 0; i < _charKind - 1; i++) {
        left = getMinFreq(_charKind + i);//�ȷ���һ�飬��Ϊvisit�ٷ���һ��
        right = getMinFreq(_charKind + i);
        hfTable[_charKind + i]._charFreq._char = '#';//���ڵ�Ĭ���ַ�
        hfTable[_charKind + i]._charFreq.freq = hfTable[left]._charFreq.freq + hfTable[right]._charFreq.freq;//�Ѻ��ӽڵ��freq������
        hfTable[_charKind + i].left = left;
        hfTable[_charKind + i].right = right;
        hfTable[_charKind + i].visited = false;
        //cout<<hfTable[_charKind + i]._charFreq.freq<<" "<<hfTable[_charKind + i]._charFreq._char<<endl;
    }

}

//��������������
/*********************************************************
* ������������������������
* ���������
* ���������

* ����ֵ�� void
************************************************************/
void Encode::makeHuffmanCode(int root, int index) {
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
* ������������ȡĩβ��Чλ
* ���������
* ���������

* ����ֵ�� void
************************************************************/
int Encode::getlastValidBit(){
    int sum = 0;
    for(int i = 0; i < _charKind; i++) {
        sum += strlen(hfTable[i].code) * hfTable[i]._charFreq.freq;
        //�����ִ����һ���������ݳ��ȳ���int�ı�ʾ��Χ���ͻ����
        sum &= 0xFF; //0xFF��Ϊ������λ1111 1111��������sumʼ�������һ���ֽ�,8λ
    }
    //���������������7λ�����ƣ�����Ϊһ���ֽڣ���ô����һ���ֽ�ֻ��7λΪ��Чλ�����඼������λ��
    //����ֻ��Ҫȡ������ֽڵ���7����Чλ������sum��8ȡ�༴��
    //sum = sum % 8 <=> sum = sum & 0x7
    //�������һ���ֽڵ���Чλ��
    sum &= 0x7;

    return sum == 0 ? 8 : sum;
}
void Encode::dispHuffmanTab() {
    printf("%-4s %-4s %-4s %-6s %-6s %-6s %s\n", "�±�", "�ַ�", "Ƶ��", "����", "�Һ���", "visited", "code");
    for (int i = 0; i < 2 * _charKind - 1; i++) {
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


void makeHuffmanCode(HF_TABLE *hfTable, int root, int index, char *code) {
    if(hfTable[root].left != -1 && hfTable[root].right != -1) {
        code[index] = '1';
        makeHuffmanCode(hfTable, hfTable[root].left, index + 1, code);
        code[index] = '0';
        makeHuffmanCode(hfTable, hfTable[root].right, index + 1, code);
    } else {
        code[index] = 0;
        strcpy(hfTable[root].code, code);
    }

}
/*********************************************************
* ����������д��ѹ���ļ�
* ���������
* ���������

* ����ֵ�� void
************************************************************/
void Encode::huffmanEncoding() {
    FILE *fpIn;
    FILE *fpOut;
    int ch;
    unsigned char val;
    int bitIndex = 0;

    char *hufCode = NULL;
    FILE_HEAD fileHead = {'h', 'y', 'z'};

    fpIn = fopen(srcFilePath, "rb");
    fpOut = fopen(desFilePath, "wb");

    //fileHead._charKind = (unsigned char) _charKind;------------
    fileHead._charKind = _charKind;
    fileHead.lastValidBit = getlastValidBit();

    //���ļ�ͷ��д��Ԫ����
    fwrite(&fileHead, sizeof(FILE_HEAD), 1, fpOut);
    //��Ԫ���ݺ�д�ַ������Ƶ�ȣ���ѹ��ʱ��Ҫ����Щ����һģһ���µĹ�������
    fwrite(_charFreq, sizeof(CHAR_FREQ), _charKind, fpOut);

    ch = fgetc(fpIn);
    while(!feof(fpIn)) {
        hufCode = hfTable[hufIndex[ch]].code;
        //��ÿ���ַ��Ĺ���������һ��һ������
        //������ַ�'0'����ת��Ϊ�����Ƶ�0
        //������ַ�'1'����ת��Ϊ�����Ƶ�1
        for(int i = 0; hufCode[i]; i++) {
            if('0' == hufCode[i]) {
                //valΪһ���ֽ�
                //�ӵ�1λ���θ�ֵ�������ڰ�λ��һ���ֽڣ��ˣ���д���ļ���
                CLR_BYTE(val, bitIndex);
            } else {
                SET_BYTE(val, bitIndex);
            }
            bitIndex++;
            if(bitIndex >= 8) {
                bitIndex = 0;
                fwrite(&val, sizeof(unsigned char), 1, fpOut);
            }
        }
        ch = fgetc(fpIn);
    }
    //������һ�β���һ���ֽڣ���Ȼ��Ҫд���ļ��У�ע�⣺д������һ���ֽڿ��ܻ��������λ
    if(bitIndex) {
        fwrite(&val, sizeof(unsigned char), 1, fpOut);
    }

    fclose(fpIn);
    fclose(fpOut);
}
/*********************************************************
* ����������������������ѹ����ִ�й���
* ���������
* ���������

* ����ֵ�� void
************************************************************/

void Encode::Main(char *src, char *des) {
    if (!isFileExist(src)) {
        cout << "Fail because of src path error" << endl;
        return;
    }
    cout << "10%Doing initData" << endl;
    initData();
    setSrcFilePath(src);
    setDesFilePath(des);
    cout << "23% Doing getCharFreq" << endl;
    getCharFreq();
    //���ֻ��һ���ַ����ܾ�ѹ��
    if(_charKind == 1){
        cout << "Refuse for character frequence is 1" << endl;
        return;
    }
    initHuffmanTab();
    cout << "46% Doing creatHuffmanTree" << endl;
    creatHuffmanTree();
    code = (char *) calloc(sizeof(char), _charKind);
    cout << "78% Doing makeHuffmanCode" << endl;
    makeHuffmanCode(2 * _charKind - 2, 0);
//    dispHuffmanTab();
    cout << "90% Doing huffmanEncoding" << endl;
    huffmanEncoding();
    cout<<"100% export to "<<desFilePath<<endl;

}
