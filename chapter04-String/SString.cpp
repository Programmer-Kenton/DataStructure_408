//
// Created by Kenton on 2022/7/25.
//

#include <stdio.h>
#include <cstring>

/*定义模块*/
// 预定义最大串长为15
#define MAXLEN 15

typedef struct {
    // 为每个分量存储一个字符
    char ch[MAXLEN];
    // 串的实际长度
    int length;
}SString;


/*
 * 实际上，主函数中定义的 x 和 test 函数的参数 x 并不是同一个，在虚拟内存中，它们的地址是不同的。
 * 而 & 叫做引用符，它是 C++ 的内容（目前多数 C 语言编译器也能使用），它可以引用主函数中 x 的地址，这样就可以对主函数中的 x 进行修改。
 * 所以当 C 语言开发中，函数需要对原变量进行修改时，可以给该参数添加 &。这样函数中对参数的修改，就是对原变量的修改。
*/
// 函数声明
//函数声明
void InitStr(SString &S);//初始化
bool StrAssign(SString &T, char *str, int strLength);//赋值操作
void StrCopy(SString &T, SString S);//复制操作
bool StrEmpty(SString S);//判空
void Concat(SString &T, SString S1, SString S2);//串链操作
bool SubString(SString &Sub, SString S, int pos, int len);//求子串
int StrCompare(SString S, SString T);//比较操作，若S>T,则返回值>0;若S=T,则返回值=0；若S<T，则返回值<0;
int StrLength(SString S);//获取字符串长度
int Index(SString S, SString T);//定位操作,若主串S中存在与串T值相同的子串，则返回它在主串S中第一次出现的位置，否则函数值为0；
int Index_Simple(SString S, SString T);//简单模式匹配
int Index_Simple_CSKaoYan(SString S, SString T);//简单模式匹配——王道教材写法
int Index_KMP(SString S, SString T, int next[]);//KMP算法
void getNext(SString T, int *next);//求模式串T的next数组
int Index_KMP1(SString S, SString T);//KMP2
void Get_BetterNext(SString T, int betterNext[]);//优化next数组
void ClearStr(SString &S);//清空操作


// 初始化
void InitStr(SString &S){
    // 以字符串结束符号来作为初始状态
    S.ch[1] = '\0';
    S.length = 0;
}

// 赋值操作
bool StrAssign(SString &T,char *str,int strLength){
    if(str[0] == '\0'){
        // 传入空数组就失败 条件依据初始化操作和清空操作而定
        return false;
    }

    // 想一想为什么是i < strLength  因为i从0开始 数组T置空第一个元素位置
    for (int i = 0; i < strLength; ++i) {
        // 置空T的第一个元素位置
        T.ch[i+1] = str[i];
        T.length = strLength;
        return true;
    }
}

// 复制操作
void StrCopy(SString &T,SString S){
    // 想一想为什么这是i <= S.length
    for (int i = 1; i <= S.length; ++i) {
        T.ch[i] = S.ch[i];
    }
    T.length = S.length;
}

// 判空
bool StrEmpty(SString S){
    return S.length == 0;
}

// 串链操作
void Concat(SString &T,SString S1,SString S2){
    for (int i = 1; i <= S1.length; ++i) {
        T.ch[i] = S1.ch[i];
    }
    // 好好想一下循环的条件以及数组下标
    for (int j = S1.length + 1; j < S1.length + S2.length; ++j) {
        T.ch[j] = S2.ch[j - S1.length];
    }
    T.length = S1.length + S2.length;
}

// 求子串
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

// 比较操作 若S>T则返回值>0;若S=T则返回值0;若S<T则返回值<0
int StrCompare(SString S,SString T){
    for (int i = 1; i <= S.length && i <= T.length; ++i) {
        if(S.ch[i] != T.ch[i]){
            return S.ch[i] - T.ch[i];
        }
        // 扫描过的所有字符都相同 则长度大的串更大
        return S.length - T.length;
    }
}

// 获取字符串长度
int StrLength(SString S){
    return S.length;
}

// 定位操作 若主串S中存在与串值T相同的字串 则返回它在主串S中第一次出现的位置 否则函数值为0
int Index(SString S,SString T){
    int i = 1;
    int m;
    int n;
    n = StrLength(S), m = StrLength(T);
    SString sub;
    if(StrCompare(sub,T) != 0){
        i++;
        // 返回字串在主串中的位置
    } else return i;
}

// 简单模式匹配
int Index_Simple(SString S,SString T){
    // k记录当前主串指针
    int k = 1;
    // j记录模式串指针 i记录主串中模式串进行对比的字串的起始地址
    int i = k;
    int j = 1;
    while (i <= S.length && j <= T.length){
        if(S.ch[i] == T.ch[j]){
            // 继续比较后面的字符
            i++;
            j++;
        } else{
            // 检查下一个字串
            i = k;
            // 重置j的值
            j = 1;
        }
    }
    if(j > T.length){
        // 匹配成功
        return k;
    }else{
        return 0;   
    }
}

// 简单模式匹配——王道教材写法
int Index_Simple_CSKaoYan(SString S,SString T){
    // 记录当前主串指针
    int i = 1;
    // 记录模式串指针
    int j = 1;
    // S为主串 T为模式串
    while (i <= S.length && j <= T.length){
        if(S.ch[i] == T.ch[j]){
            // 继续比较后续字符
            ++i;
            ++j;
        } else{
            // TODO 检查下一个字串
            // 牢记主串的指针的移动公式
            i = i - j + 2;
            // 重置j的值
            j = 1;
        }
    }
    // j > T.length说明j指针已经移动在模式串后面的位置 意思是模式串前面的字符全都比较过且匹配的上
    // 如果j=T.length说明S.ch[i]和T.ch[j]还没有比较模式串最后一个字符 所以必须得是j > T.length
    if(j > T.length){
        // 匹配成功
        // TODO 从第i - T.length + 1开始的位置是主串和模式串匹配的起始位置
        return i - T.length;
    } else{
        return 0;
    }
}

// 求模式串T的next数组
void getNext(SString T,int *next){
    int i = 1,j = 0;
    next[1] = 0;
    while (i < T.length){
        if(j == 0 || T.ch[i] == T.ch[j]){
            ++i;
            ++j;
            // 如果pi=pj 则next[j+1]=next[j]+1
            next[j] = j;
        } else{
            // 否则令j=next[j]循环继续
            j = next[j];
        }
    }
}

// KMP 算法一
int Index_KMP(SString S,SString T){
    int i = 1,j = 1;
    int next[T.length + 1];
    getNext(T,next);
    while (i <= S.length && j <= T.length){
        if(j == 0 || S.ch[i] == T.ch[j]){
            // 继续比较后继字符
            ++i;
            ++j;
        } else{
            // 模式串右移动
            j = next[j];
        }
    }
    // 匹配成功
    if(j > T.length){
        return i - T.length;
    } else{
        return 0;
    }
}

// 优化next数组
void Get_BetterNext(SString T,int *betterNext){
    int i = 0,j = 0;
    int next[T.length + 1];
    // 先求出next数组
    getNext(T,next);
    // 令betterNext[1]=0
    betterNext[1] = 0;
    for(int j = 2;j <= T.length;++j){
        if(T.ch[next[j]] == T.ch[j]){
            // 仔细看 这里涉及三个数组
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
            // 继续比较后继字符
            ++i;
            ++j;
        } else{
            // 模式串向右移动
            j = next[j];
        }
    }
    // 匹配成功
    if(j > T.length){
        return i - T.length;
    } else{
        return 0;
    }
}

// 清空操作
void ClearStr(SString &S){
    S.length = 0;
    // 用到了cstring库中的memset函数
    memset(S.ch,'\0',MAXLEN);
}

// 销毁操作
/*
void DestoryString(SString &S){
 基于数组实现的字符串存储会自动销毁 无需单独销毁
}*/


/*测试模块*/
void printDs(SString S,char *StrName){
    printf("当前%s字符内容为:",StrName);
    for(int i = 1;i <= S.length;++i){
        if(S.ch[i] != '\0'){
            // 注意输出单个字符用的是%c 而%s是输出一个字符串
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
    printf("测试开始!\n");

    SString S,T;
    InitStr(S);
    InitStr(T);
    //使用这种初始化列表进行初始化，最后会数组会多一个结束符'\0'
    char str1[] = "kim";
    // char str1[] = {'k','i','m'};
    // 而这种不会，所以在选择初始化方式的时候尽量做到统一，否则你很有可能因为'\0'而匹配不到子串

    char str2[] = "kimYang";
    testBoolOperate(StrAssign(S, str1, 3), "赋值操作", "成功啦！", "失败啦！");
    printDs(S, "S");
    testBoolOperate(StrAssign(T, str2, 7), "赋值操作", "又成功啦！", "失败啦！");
    printDs(T, "T");

    SString S1;
    InitStr(S1);
    StrCopy(S1, S);
    printDs(S1, "S1");

    SString S2;
    InitStr(S2);
    Concat(S2, S, T);
    printDs(S2, "串链结束后S2");

    SString S3;
    InitStr(S3);
    testBoolOperate(SubString(S3, T, 2, 4), "取子串操作", "成功啦", "失败啦");
    printDs(S3, "当前取出的S3");

    if (0 == StrCompare(S, S1)) {
        printf("两字符串一样\n");
    } else {
        printf("两个字符串不一样！\n");
    }

    int n = Index(T, S3);
    if (0 == n) {
        printf("主串T中不含子串S3\n");
    } else {
        printf("主串T中含有S3，其下标为：%d\n", n);
    }

    int n1 = Index_Simple(T, S3);
    if (0 == n1) {
        printf("主串T中不含子串S3\n");
    } else {
        printf("主串T中含有S3，其下标为：%d\n", n1);
    }

    int n2 = Index_Simple_CSKaoYan(T, S3);
    if (0 == n2) {
        printf("主串T中不含子串S3\n");
    } else {
        printf("主串T中含有S3，其下标为：%d\n", n2);
    }

    int n3 = Index_KMP(T, S3);
    if (0 == n3) {
        printf("主串T中不含子串S3\n");
    } else {
        printf("主串T中含有S3，其下标为：%d\n", n3);
    }

    int betterNext[S3.length+1];
    Get_BetterNext(S3,betterNext);
    int n4=Index_KMP1(T,S3,betterNext);
    if (0 == n4) {
        printf("主串T中不含子串S3\n");
    } else {
        printf("主串T中含有S3，其下标为：%d\n", n4);
    }


    printf("测试结束!\n");
}

/**测试模块**/

int main() {
    testModule();
    return 0;
}