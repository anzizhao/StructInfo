#ifndef H_CORE
#define H_CORE

#include <vector>
#include <string>
#include <iostream>
using namespace std; 

#include <string.h>
#include <assert.h>


//���ַ��������ṹ��
/*
��Žṹ����Ϣ���ṹ������ͣ�int, double, short, �����ṹ�����֣������� ÿһ����Ϣ�ṹ����Ϣ��


*/
class TypeInfo
{
public:
	string strType;        //�������� "int", "double"
	string strName;        //�������� �ṹ�����û��
	short  sSubItemNum;    //�������� Ϊ0��ʾû������
	vector<TypeInfo> SubItem; //����
	TypeInfo(){ sSubItemNum = 0;  }
};


//�ṹ�������ú���
bool AnalyseStrtoStruct(char *src, vector<TypeInfo>& result); 
//Ƕ�׶��岻���� 
bool GetLine(  char *szSrc,  char *&pLTail );  //��ȡһ��
bool GetWord( char *szSrc,  char *pLTail,   char *&pWHead,  char *&pWTail);  //��ȡһ������
char* GetNoneNullChar( char *szSrc,   char *pLTail);  //��ȡ��һ���ǿյ��ַ�	
bool CopyWord( char *pSrcHead,  char* pSrcTail, string &dst); //���Ƶ���
bool strchrInMen( char *pS,  char* pT, char c);  //��һ���ڴ���  ����ĳ���ַ��Ƿ����
bool strstrInMen( char *pS,  char* pT,  char* str); //��һ���ڴ���  ����ĳ���ַ����Ƿ����

//���������ṹ��
bool AnalyseStruct(char *src, vector<TypeInfo>& result); 
char * AnalyseStruct(char *pFirst,  char *pEnd, TypeInfo & result);

//��ĸʶ�����
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