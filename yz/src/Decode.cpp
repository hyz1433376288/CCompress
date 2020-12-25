
/***********************************************************
* 版权所有 (C)2020, hyz
*
* 文件名称： Decode.h
* 文件标识：无
* 内容摘要：解压类实现文件
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
* 功能描述：初始化数据
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/

void Decode::initData() {
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
* 输出参数：bool

* 返回值： bool
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
* 功能描述：设置文件；路径
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/
void Decode::setSrcFilePath(char *path) {
    strcpy(srcFilePath, path);
}
/*********************************************************
* 功能描述：设置文件；路径
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/
void Decode::setDesFilePath(char *path) {
    strcpy(desFilePath, path);
}
/*********************************************************
* 功能描述：读取文件头元数据
* 输入参数：无
* 输出参数：无

* 返回值： bool
************************************************************/
bool Decode::readFileHead(){
    FILE *fp;
    fp = fopen(srcFilePath, "rb");
    //读取压缩的文件的头部元数据，16个字节
    fread(&fileHead, sizeof(FILE_HEAD), 1, fp);
    fclose(fp);
    if(!(fileHead.flag[0] == 'h' && fileHead.flag[1] == 'y' && fileHead.flag[2] == 'z')) {
        printf("format is not hyz\n");
        return false;
    }
    return true;
}
/*********************************************************
* 功能描述：获取字符频度
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/
void Decode::getCharFreq() {

    FILE *fpIn;

    _charKind = fileHead._charKind;
    _charFreq = (CHAR_FREQ *) calloc(sizeof(CHAR_FREQ), _charKind);
    fpIn = fopen(srcFilePath, "rb");
    //略过前16个字节的元数据
    fseek(fpIn, 16, SEEK_SET);
    fread(_charFreq, sizeof(CHAR_FREQ), _charKind, fpIn);
    fclose(fpIn);

}
/*********************************************************
* 功能描述：初始化哈夫曼表
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/
void Decode::initHuffmanTab() {


    hfTable = (HF_TABLE *) calloc(sizeof(HF_TABLE), 2 * _charKind - 1);
    //hfTable申请了 2 * _charKind - 1大小的空间，在这只用了 _charKind个，还剩_charKind - 1个
    for(int i = 0; i < _charKind; i++) {
        hufIndex[_charFreq[i]._char] = i;	//把哈夫曼表中的字符和其对应的下标形成键值对,存到hufIndex中
        hfTable[i]._charFreq = _charFreq[i];
        hfTable[i].left = hfTable[i].right = -1;
        hfTable[i].visited = false;
        hfTable[i].code = (char *) calloc(sizeof(char), _charKind);
    }


}
/*********************************************************
* 功能描述：获取频度最小的字符索引
* 输入参数：
* 输出参数：

* 返回值： int 索引
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
* 功能描述：创建哈夫曼树
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/

void Decode::creatHuffmanTree() {
    int left;
    int right;

    //hfTable使用剩下的 _charKind - 1个空间
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
* 功能描述：创建哈夫曼编码
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/
void Decode::makeHuffmanCode(int root, int index) {
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
* 功能描述：输出哈夫曼表
* 输入参数：
* 输出参数：

* 返回值： void
************************************************************/

void Decode::dispHuffmanTab() {
    printf("%-4s %-4s %-4s %-6s %-6s %-6s %s\n", "下标", "字符", "频度", "左孩子", "右孩子", "visited", "code");
    for (int i = 0; i < 2*_charKind-1; i++) {
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
/*********************************************************
* 功能描述：写入解压文件
* 输入参数：
* 输出参数：

* 返回值： void
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
    fileSize = ftell(fpIn);	//文件总长度fileSize
    fseek(fpIn, 16 + 5 * fileHead._charKind, SEEK_SET);	//略过前面16个字节的元数据，5字节的字符种类和频度
   // fseek(fpIn,  + 5 * fileHead._charKind, SEEK_SET);
    curLocation = ftell(fpIn);

    //从根出发，'1'向左子树走，'0'向右子树走，若到达叶子结点，输出叶子结点下标对应的字符。再回到根结点继续。
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
        //取出的一个字节从第一位开始看，'1'向左子树走，'0'向右子树走
        //若超过一个字节，8位，则需要读取下一个字节
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
* 功能描述：主方法，包含解压的执行过程
* 输入参数：
* 输出参数：

* 返回值： void
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

