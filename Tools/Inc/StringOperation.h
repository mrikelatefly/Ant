#ifndef __STRINGOPERATION_H
#define __STRINGOPERATION_H

/***************************************************************************************************
时间：2018 5 1
作者:latefly
功能：如下

这里要实现的函数有
StringReverse 字符串反转顺序
SplitString
IntString
StringInt
FloatString
StringFloat
DoubleString
StringDouble
StringEqual
StringCopy
HasSymbol 判断是否含有某一个字符串
StringTrim
StringLength
StringBuffEmpty

edit version 2018 7 23
edit version 2018 9 19
字符加入长度，修改缓冲区的大小
edit version 2018 9 27
加入字符判别标识
edit version 2018 10 2
加入缓冲区清除 能够获取使用了多少的位置
edit version 2018 10 4
修改字符串转换到int型时为其他类转换做的准备

******************************************************************************************************/

/*******************************************错误定义**************************************************/
#define TypeError 0x01







/****************************************************************************************************/
//基础的字符判定定义
#define IsCharacter(x) ((x>='a'&&x<='z')||(x>='A'&&x<='Z'))
#define IsNum(x)       (x>='0'&&x<='9')
#define IsSign(x)     ((IsNum(x))||(IsCharacter(x))||(x=='_'))
#define IsDefine(x)     (x=='#')
//#define IsBoundStart(x)      (*(x)=='/'&&(*(x++)=='*'||*(x++)=='/'))
//#define IsBoundEnd(x)          (*(x)=='*'&&*(x++)=='/')
#define IsOperator(x)   (x=='+'||x=='-'||x=='*'||x=='/')
//用于保存结果




void StringBuffEmpty(void);
char GetStringBuffUsedNum(void);
int StringLength(char *s);
char StringTrim(char sysmbol,char *s);
char StringCopy(char *s1,char *s2);
char StringReverse(char *s);
char SplitString(char split,char *s);
char StringEqual(char *s1,char *s2);
char HasSymbol(char symbol,char *s);
int IntString(char *s);
char StringInt(int num);
float FloatString(char *s);
char StringFloat(float number);
double DoubleString(char *s);
char StringDouble(double number);














#endif // __STRINGOPERATION_H


















