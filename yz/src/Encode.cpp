
/***********************************************************
* 版权所有 (C)2020, hyz
*
* 文件名称： Decode.h
* 文件标识：无
* 内容摘要：压缩类实现文件
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
* 功能描述：初始化数据
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/
void Encode::initData() {
    memset(srcFilePath,0, sizeof(srcFilePath));
    memset(desFilePath,0, sizeof(desFilePath));
    memset(hufIndex,0, sizeof(hufIndex));
    _charFreq = NULL;				//统计字符及频度的数组
    _charKind = 0;								// 字符种类
    hfTable = NULL;			//哈夫曼表
    code = NULL;									//存储字符的哈夫曼编码
}
/*********************************************************
* 功能描述：文件路径判空
* 输入参数：文件路径
* 输出参数：

* 返回值： bool
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
* 功能描述：设置路径
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/

void Encode::setDesFilePath(char *path) {
    strcpy(desFilePath, path);
}
/*********************************************************
* 功能描述：获取字符频度
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/

void Encode::getCharFreq() {
    int freq[256] = {0};

    int index;
    _charFreq = NULL;
    FILE *fpIn;
    int ch;

    fpIn = fopen(srcFilePath, "rb");

    /*统计所有字符的频度*/
    ch = fgetc(fpIn);
    while (!feof(fpIn)) {
        freq[ch]++;
        ch = fgetc(fpIn);
    }
    fclose(fpIn);

    /*统计所有字符的种类*/
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
* 功能描述：初始化哈夫曼表
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/
void Encode::initHuffmanTab() {

    hfTable = (HF_TABLE *) calloc(sizeof(HF_TABLE), 2 * _charKind - 1);
    //hfTable申请了 2 * _charKind - 1大小的空间，在这只用了 _charKind个，还剩_charKind - 1个,用来合并节点
    for (int i = 0; i < _charKind; i++) {
        hufIndex[_charFreq[i]._char] = i;    //把哈夫曼表中的字符和其对应的下标形成键值对,存到hufIndex中
        hfTable[i]._charFreq = _charFreq[i];
        hfTable[i].left = hfTable[i].right = -1;
        hfTable[i].visited = false;//记录是否访问
        hfTable[i].code = (char *) calloc(sizeof(char), _charKind);//开辟空间
    }

}
/*********************************************************
* 功能描述：获取字符最小频度索引
* 输入参数：
* 输出参数：

* 返回值： int
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
* 功能描述：创建哈夫曼树
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/
void Encode::creatHuffmanTree() {
    int left;
    int right;

    //使用_charKind-1的空间创建_char Kind个父节点
    for (int i = 0; i < _charKind - 1; i++) {
        left = getMinFreq(_charKind + i);//先访问一遍，设为visit再访问一遍
        right = getMinFreq(_charKind + i);
        hfTable[_charKind + i]._charFreq._char = '#';//父节点默认字符
        hfTable[_charKind + i]._charFreq.freq = hfTable[left]._charFreq.freq + hfTable[right]._charFreq.freq;//把孩子节点的freq加起来
        hfTable[_charKind + i].left = left;
        hfTable[_charKind + i].right = right;
        hfTable[_charKind + i].visited = false;
        //cout<<hfTable[_charKind + i]._charFreq.freq<<" "<<hfTable[_charKind + i]._charFreq._char<<endl;
    }

}

//创建哈夫曼编码
/*********************************************************
* 功能描述：创建哈夫曼编码
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/
void Encode::makeHuffmanCode(int root, int index) {
    if (hfTable[root].left != -1 && hfTable[root].right != -1) {
        code[index] = '1';
        makeHuffmanCode(hfTable[root].left, index + 1);
        code[index] = '0';
        makeHuffmanCode(hfTable[root].right, index + 1);
    } else {
        code[index] = 0;//'\0'的ASCII就是0
        strcpy(hfTable[root].code, code);
    }


}
/*********************************************************
* 功能描述：获取末尾有效位
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/
int Encode::getlastValidBit(){
    int sum = 0;
    for(int i = 0; i < _charKind; i++) {
        sum += strlen(hfTable[i].code) * hfTable[i]._charFreq.freq;
        //如果不执行这一步，当数据长度超过int的表示范围，就会出错
        sum &= 0xFF; //0xFF化为二进制位1111 1111，这样做sum始终是最后一个字节,8位
    }
    //举例：若最后生成7位二进制，划分为一个字节，那么，这一个字节只有7位为有效位，其余都是垃圾位。
    //我们只需要取出这个字节的那7个有效位，所以sum和8取余即可
    //sum = sum % 8 <=> sum = sum & 0x7
    //返回最后一个字节的有效位数
    sum &= 0x7;

    return sum == 0 ? 8 : sum;
}
void Encode::dispHuffmanTab() {
    printf("%-4s %-4s %-4s %-6s %-6s %-6s %s\n", "下标", "字符", "频度", "左孩子", "右孩子", "visited", "code");
    for (int i = 0; i < 2 * _charKind - 1; i++) {
        printf("%-5d %-4c %-5d %-6d %-7d %-4d %s\n",
               i,
               hfTable[i]._charFreq._char,
               hfTable[i]._charFreq.freq,
               hfTable[i].left,
               hfTable[i].right,
               hfTable[i].visited,
               (hfTable[i].code ? hfTable[i].code : "无"));
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
* 功能描述：写入压缩文件
* 输入参数：
* 输出参数：

* 返回值： void
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

    //给文件头部写入元数据
    fwrite(&fileHead, sizeof(FILE_HEAD), 1, fpOut);
    //给元数据后写字符种类和频度，解压缩时需要用这些生成一模一样新的哈夫曼树
    fwrite(_charFreq, sizeof(CHAR_FREQ), _charKind, fpOut);

    ch = fgetc(fpIn);
    while(!feof(fpIn)) {
        hufCode = hfTable[hufIndex[ch]].code;
        //把每个字符的哈夫曼编码一个一个过。
        //如果是字符'0'，就转换为二进制的0
        //如果是字符'1'，就转换为二进制的1
        for(int i = 0; hufCode[i]; i++) {
            if('0' == hufCode[i]) {
                //val为一个字节
                //从第1位依次赋值，若大于八位（一个字节）了，就写入文件中
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
    //如果最后一次不满一个字节，依然需要写到文件中，注意：写入的最后一个字节可能会存在垃圾位
    if(bitIndex) {
        fwrite(&val, sizeof(unsigned char), 1, fpOut);
    }

    fclose(fpIn);
    fclose(fpOut);
}
/*********************************************************
* 功能描述：主方法，包含压缩的执行过程
* 输入参数：
* 输出参数：

* 返回值： void
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
    //如果只有一种字符，拒绝压缩
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
