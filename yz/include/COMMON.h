
/***********************************************************
* ��Ȩ���� (C)2020, hyz
*
* �ļ����ƣ� Decode.h
* �ļ���ʶ����
* ����ժҪ������ͷ�ļ�����Ŵ洢���ݽṹ��
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
#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
typedef struct FILE_HEAD {
    unsigned char flag[3];
    int _charKind;		//�ַ�����---------------------------
    unsigned char lastValidBit;		//���һ���ֽڵ���Чλ��
    unsigned char unused[8];			//���ÿռ�
} FILE_HEAD;								//����ṹ���ܹ�ռ��16���ֽڵĿռ�

typedef struct CHAR_FREQ {
    unsigned char _char;		//�ַ�,���ǵ��ļ����к��֣����Զ����unsigned char
    int freq;								//�ַ����ֵ�Ƶ��
} CHAR_FREQ;

typedef struct HF_TABLE {
    CHAR_FREQ _charFreq;
    int left;
    int right;
    bool visited;
    char *code;
} HF_TAB;


//�õ�indexλ��ֵ����indexλΪ0����GET_BYTEֵΪ�٣�����Ϊ��
#define GET_BYTE(vbyte, index) (((vbyte) & (1 << ((index) ^ 7))) != 0)
//indexλ��1
#define SET_BYTE(vbyte, index) ((vbyte) |= (1 << ((index) ^ 7)))
//indexλ��0
#define CLR_BYTE(vbyte, index) ((vbyte) &= (~(1 << ((index) ^ 7))))


#define NOT_INIT		-1



#endif // COMMON_H_INCLUDED
