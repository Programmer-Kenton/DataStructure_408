//
// Created by Kenton on 2022/7/25.
//

#include <stdio.h>
#include <cstring>

/*����ģ��*/
// Ԥ������󴮳�Ϊ15
#define MAXLEN 15

typedef struct {
    // Ϊÿ�������洢һ���ַ�
    char ch[MAXLEN];
    // ����ʵ�ʳ���
    int length;
}SString;


/*
 * ʵ���ϣ��������ж���� x �� test �����Ĳ��� x ������ͬһ�����������ڴ��У����ǵĵ�ַ�ǲ�ͬ�ġ�
 * �� & �������÷������� C++ �����ݣ�Ŀǰ���� C ���Ա�����Ҳ��ʹ�ã��������������������� x �ĵ�ַ�������Ϳ��Զ��������е� x �����޸ġ�
 * ���Ե� C ���Կ����У�������Ҫ��ԭ���������޸�ʱ�����Ը��ò������ &�����������жԲ������޸ģ����Ƕ�ԭ�������޸ġ�
*/
// ��������
//��������
void InitStr(SString &S);//��ʼ��
bool StrAssign(SString &T, char *str, int strLength);//��ֵ����
void StrCopy(SString &T, SString S);//���Ʋ���
bool StrEmpty(SString S);//�п�
void Concat(SString &T, SString S1, SString S2);//��������
bool SubString(SString &Sub, SString S, int pos, int len);//���Ӵ�
int StrCompare(SString S, SString T);//�Ƚϲ�������S>T,�򷵻�ֵ>0;��S=T,�򷵻�ֵ=0����S<T���򷵻�ֵ<0;
int StrLength(SString S);//��ȡ�ַ�������
int Index(SString S, SString T);//��λ����,������S�д����봮Tֵ��ͬ���Ӵ����򷵻���������S�е�һ�γ��ֵ�λ�ã�������ֵΪ0��
int Index_Simple(SString S, SString T);//��ģʽƥ��
int Index_Simple_CSKaoYan(SString S, SString T);//��ģʽƥ�䡪�������̲�д��
int Index_KMP(SString S, SString T, int next[]);//KMP�㷨
void getNext(SString T, int *next);//��ģʽ��T��next����
int Index_KMP1(SString S, SString T);//KMP2
void Get_BetterNext(SString T, int betterNext[]);//�Ż�next����
void ClearStr(SString &S);//��ղ���


// ��ʼ��
void InitStr(SString &S){
    // ���ַ���������������Ϊ��ʼ״̬
    S.ch[1] = '\0';
    S.length = 0;
}

// ��ֵ����
bool StrAssign(SString &T,char *str,int strLength){
    if(str[0] == '\0'){
        // ����������ʧ�� �������ݳ�ʼ����������ղ�������
        return false;
    }

    // ��һ��Ϊʲô��i < strLength  ��Ϊi��0��ʼ ����T�ÿյ�һ��Ԫ��λ��
    for (int i = 0; i < strLength; ++i) {
        // �ÿ�T�ĵ�һ��Ԫ��λ��
        T.ch[i+1] = str[i];
        T.length = strLength;
        return true;
    }
}

// ���Ʋ���
void StrCopy(SString &T,SString S){
    // ��һ��Ϊʲô����i <= S.length
    for (int i = 1; i <= S.length; ++i) {
        T.ch[i] = S.ch[i];
    }
    T.length = S.length;
}

// �п�
bool StrEmpty(SString S){
    return S.length == 0;
}

// ��������
void Concat(SString &T,SString S1,SString S2){
    for (int i = 1; i <= S1.length; ++i) {
        T.ch[i] = S1.ch[i];
    }
    // �ú���һ��ѭ���������Լ������±�
    for (int j = S1.length + 1; j < S1.length + S2.length; ++j) {
        T.ch[j] = S2.ch[j - S1.length];
    }
    T.length = S1.length + S2.length;
}

// ���Ӵ�
bool SubString(SString &Sub,SString S,int pos,int len){
    if(pos + len - 1 > S.length){
        return false;
    }
    for (int i = pos; i < pos + len; ++i) {
        Sub.ch[i - pos + 1] = S.ch[i];
    }
    Sub.length = len;
    return true;
}

// �Ƚϲ��� ��S>T�򷵻�ֵ>0;��S=T�򷵻�ֵ0;��S<T�򷵻�ֵ<0
int StrCompare(SString S,SString T){
    for (int i = 1; i <= S.length && i <= T.length; ++i) {
        if(S.ch[i] != T.ch[i]){
            return S.ch[i] - T.ch[i];
        }
        // ɨ����������ַ�����ͬ �򳤶ȴ�Ĵ�����
        return S.length - T.length;
    }
}

// ��ȡ�ַ�������
int StrLength(SString S){
    return S.length;
}

// ��λ���� ������S�д����봮ֵT��ͬ���ִ� �򷵻���������S�е�һ�γ��ֵ�λ�� ������ֵΪ0
int Index(SString S,SString T){
    int i = 1;
    int m;
    int n;
    n = StrLength(S), m = StrLength(T);
    SString sub;
    if(StrCompare(sub,T) != 0){
        i++;
        // �����ִ��������е�λ��
    } else return i;
}

// ��ģʽƥ��
int Index_Simple(SString S,SString T){
    // k��¼��ǰ����ָ��
    int k = 1;
    // j��¼ģʽ��ָ�� i��¼������ģʽ�����жԱȵ��ִ�����ʼ��ַ
    int i = k;
    int j = 1;
    while (i <= S.length && j <= T.length){
        if(S.ch[i] == T.ch[j]){
            // �����ȽϺ�����ַ�
            i++;
            j++;
        } else{
            // �����һ���ִ�
            i = k;
            // ����j��ֵ
            j = 1;
        }
    }
    if(j > T.length){
        // ƥ��ɹ�
        return k;
    }else{
        return 0;   
    }
}

// ��ģʽƥ�䡪�������̲�д��
int Index_Simple_CSKaoYan(SString S,SString T){
    // ��¼��ǰ����ָ��
    int i = 1;
    // ��¼ģʽ��ָ��
    int j = 1;
    // SΪ���� TΪģʽ��
    while (i <= S.length && j <= T.length){
        if(S.ch[i] == T.ch[j]){
            // �����ȽϺ����ַ�
            ++i;
            ++j;
        } else{
            // TODO �����һ���ִ�
            // �μ�������ָ����ƶ���ʽ
            i = i - j + 2;
            // ����j��ֵ
            j = 1;
        }
    }
    // j > T.length˵��jָ���Ѿ��ƶ���ģʽ�������λ�� ��˼��ģʽ��ǰ����ַ�ȫ���ȽϹ���ƥ�����
    // ���j=T.length˵��S.ch[i]��T.ch[j]��û�бȽ�ģʽ�����һ���ַ� ���Ա������j > T.length
    if(j > T.length){
        // ƥ��ɹ�
        // TODO �ӵ�i - T.length + 1��ʼ��λ����������ģʽ��ƥ�����ʼλ��
        return i - T.length;
    } else{
        return 0;
    }
}

// ��ģʽ��T��next����
void getNext(SString T,int *next){
    int i = 1,j = 0;
    next[1] = 0;
    while (i < T.length){
        if(j == 0 || T.ch[i] == T.ch[j]){
            ++i;
            ++j;
            // ���pi=pj ��next[j+1]=next[j]+1
            next[j] = j;
        } else{
            // ������j=next[j]ѭ������
            j = next[j];
        }
    }
}

// KMP �㷨һ
int Index_KMP(SString S,SString T){
    int i = 1,j = 1;
    int next[T.length + 1];
    getNext(T,next);
    while (i <= S.length && j <= T.length){
        if(j == 0 || S.ch[i] == T.ch[j]){
            // �����ȽϺ���ַ�
            ++i;
            ++j;
        } else{
            // ģʽ�����ƶ�
            j = next[j];
        }
    }
    // ƥ��ɹ�
    if(j > T.length){
        return i - T.length;
    } else{
        return 0;
    }
}

// �Ż�next����
void Get_BetterNext(SString T,int *betterNext){
    int i = 0,j = 0;
    int next[T.length + 1];
    // �����next����
    getNext(T,next);
    // ��betterNext[1]=0
    betterNext[1] = 0;
    for(int j = 2;j <= T.length;++j){
        if(T.ch[next[j]] == T.ch[j]){
            // ��ϸ�� �����漰��������
            betterNext[j] = betterNext[next[j]];
        } else{
            betterNext[j] = next[j];
        }
    }
}

// KMP
int Index_KMP1(SString S,SString T,int next[]){
    int i = 1,j =1;
    while (i <= S.length && j <= T.length){
        if(j == 0 || S.ch[i] == T.ch[j]){
            // �����ȽϺ���ַ�
            ++i;
            ++j;
        } else{
            // ģʽ�������ƶ�
            j = next[j];
        }
    }
    // ƥ��ɹ�
    if(j > T.length){
        return i - T.length;
    } else{
        return 0;
    }
}

// ��ղ���
void ClearStr(SString &S){
    S.length = 0;
    // �õ���cstring���е�memset����
    memset(S.ch,'\0',MAXLEN);
}

// ���ٲ���
/*
void DestoryString(SString &S){
 ��������ʵ�ֵ��ַ����洢���Զ����� ���赥������
}*/


/*����ģ��*/
void printDs(SString S,char *StrName){
    printf("��ǰ%s�ַ�����Ϊ:",StrName);
    for(int i = 1;i <= S.length;++i){
        if(S.ch[i] != '\0'){
            // ע����������ַ��õ���%c ��%s�����һ���ַ���
            printf("%c",S.ch[i]);
        }
    }
    printf("\n");
}

void testBoolOperate(bool result,char *message,char *success,char *fail){
    if(result){
        printf("%s%s\n",message,success);
    } else{
        printf("%s%s\n",message,fail);
    }
}

void testModule(){
    printf("���Կ�ʼ!\n");

    SString S,T;
    InitStr(S);
    InitStr(T);
    //ʹ�����ֳ�ʼ���б���г�ʼ��������������һ��������'\0'
    char str1[] = "kim";
    // char str1[] = {'k','i','m'};
    // �����ֲ��ᣬ������ѡ���ʼ����ʽ��ʱ��������ͳһ����������п�����Ϊ'\0'��ƥ�䲻���Ӵ�

    char str2[] = "kimYang";
    testBoolOperate(StrAssign(S, str1, 3), "��ֵ����", "�ɹ�����", "ʧ������");
    printDs(S, "S");
    testBoolOperate(StrAssign(T, str2, 7), "��ֵ����", "�ֳɹ�����", "ʧ������");
    printDs(T, "T");

    SString S1;
    InitStr(S1);
    StrCopy(S1, S);
    printDs(S1, "S1");

    SString S2;
    InitStr(S2);
    Concat(S2, S, T);
    printDs(S2, "����������S2");

    SString S3;
    InitStr(S3);
    testBoolOperate(SubString(S3, T, 2, 4), "ȡ�Ӵ�����", "�ɹ���", "ʧ����");
    printDs(S3, "��ǰȡ����S3");

    if (0 == StrCompare(S, S1)) {
        printf("���ַ���һ��\n");
    } else {
        printf("�����ַ�����һ����\n");
    }

    int n = Index(T, S3);
    if (0 == n) {
        printf("����T�в����Ӵ�S3\n");
    } else {
        printf("����T�к���S3�����±�Ϊ��%d\n", n);
    }

    int n1 = Index_Simple(T, S3);
    if (0 == n1) {
        printf("����T�в����Ӵ�S3\n");
    } else {
        printf("����T�к���S3�����±�Ϊ��%d\n", n1);
    }

    int n2 = Index_Simple_CSKaoYan(T, S3);
    if (0 == n2) {
        printf("����T�в����Ӵ�S3\n");
    } else {
        printf("����T�к���S3�����±�Ϊ��%d\n", n2);
    }

    int n3 = Index_KMP(T, S3);
    if (0 == n3) {
        printf("����T�в����Ӵ�S3\n");
    } else {
        printf("����T�к���S3�����±�Ϊ��%d\n", n3);
    }

    int betterNext[S3.length+1];
    Get_BetterNext(S3,betterNext);
    int n4=Index_KMP1(T,S3,betterNext);
    if (0 == n4) {
        printf("����T�в����Ӵ�S3\n");
    } else {
        printf("����T�к���S3�����±�Ϊ��%d\n", n4);
    }


    printf("���Խ���!\n");
}

/**����ģ��**/

int main() {
    testModule();
    return 0;
}