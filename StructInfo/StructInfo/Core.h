#ifndef H_CORE
#define H_CORE

#include <vector>
#include <string>
#include <iostream>
using namespace std; 

#include <string.h>
#include <assert.h>


//从字符串分析结构体
/*
存放结构体信息（结构体的类型（int, double, short, ），结构体名字，项数， 每一项信息结构体信息）


*/
class TypeInfo
{
public:
	string strType;        //类型名， "int", "double"
	string strName;        //对象名， 结构体可以没有
	short  sSubItemNum;    //子项数量 为0表示没有子项
	vector<TypeInfo> SubItem; //子项
	TypeInfo(){ sSubItemNum = 0;  }
};


//结构分析调用函数
bool AnalyseStrtoStruct(char *src, vector<TypeInfo>& result); 
//嵌套定义不考虑 
bool GetLine(  char *szSrc,  char *&pLTail );  //获取一行
bool GetWord( char *szSrc,  char *pLTail,   char *&pWHead,  char *&pWTail);  //获取一个单词
char* GetNoneNullChar( char *szSrc,   char *pLTail);  //获取第一个非空的字符	
bool CopyWord( char *pSrcHead,  char* pSrcTail, string &dst); //复制单词
bool strchrInMen( char *pS,  char* pT, char c);  //在一段内存内  查找某个字符是否存在
bool strstrInMen( char *pS,  char* pT,  char* str); //在一段内存内  查找某个字符串是否存在

//分析单个结构体
bool AnalyseStruct(char *src, vector<TypeInfo>& result); 
char * AnalyseStruct(char *pFirst,  char *pEnd, TypeInfo & result);

//字母识别相关
inline bool IsBigAlpha(char c)
{
	if ( (c >= 'A' && c <= 'Z'))
	{
		return true;  
	}
	return false; 
}
inline bool IsSmallAlpha(char c)
{
	if ( (c >= 'a' && c <= 'z'))
	{
		return true;  
	}
	return false; 
}

inline bool IsStrHasSmallAlpha(char *str, unsigned int uiLen)
{
//	unsigned int uiLen = strlen(str);
	for (unsigned int i = 0; i < uiLen; i++)
	{
		if (IsSmallAlpha(str[i]))
		{
			return true; 
		}
	}	
	return false; 
}

inline bool IsStrAllBigAlpha(char *str, unsigned int uiLen)
{
//	unsigned int uiLen = strlen(str);
	for (unsigned int i = 0; i < uiLen; i++)
	{
		if (! IsBigAlpha(str[i]))
		{
			return false; 
		}
	}
	
	return true; 
}


inline bool IsAlpha(char c)
{
	if ( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
	{
		return true;  
	}
	return false; 
}

inline bool IsAlnum(char c)
{
	if ( IsAlpha(c))
	{
		return true; 
	}
	if ( c >= '0'&& c <= '9')
	{
		return true;
	}
	return false; 
}




#endif 