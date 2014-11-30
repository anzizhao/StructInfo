#include "Core.h"

bool AnalyseStrtoStruct(char *szStructStr, vector<TypeInfo>& vStructData)
{
	char *pTemp = szStructStr; 
	while((pTemp = strstr(pTemp, "struct")) != NULL)
	{
		//寻找{ 压入栈
		if (!(pTemp = strchr(pTemp, '{')) )
		{
			break; 
		}
		char *pLineTail = NULL; 
		TypeInfo struTypeInfo; 
		struTypeInfo.sSubItemNum = 0; 
		char* pWHead, *pWTail; 
		while( GetLine(pTemp,  pLineTail) )
		{
			char *pFirstChar =  GetNoneNullChar(pTemp, pLineTail); 
			if (!pFirstChar)
			{
				pTemp = pLineTail + 1; 
				continue; 
			}
			char cFirstChar = *pFirstChar; 
			//assert(cFirstChar); 
			if (cFirstChar == '/')
			{
				char cSecondChar = *(pFirstChar+1);
				if (cSecondChar == '*')
				{
					//查找"*/"去掉  整行
					pTemp = strstr(pTemp, "*/");
					assert(pTemp); 				
					pTemp = strchr(pTemp+2, '\n'); 
					assert(pTemp); 
					pTemp ++; 
					continue; 
				}
				//其他 整行去掉 不处理
			}
			else if (isalnum(cFirstChar) || cFirstChar == '_')
			{
				//获取类型

				assert(GetWord(pFirstChar, pLineTail, pWHead, pWTail)); 
				TypeInfo typeTemp; 
				typeTemp.sSubItemNum = 0; 
				assert(CopyWord(pWHead, pWTail,typeTemp.strType));
				//如果是union 或者struct   嵌套不考虑  
				//只负责名字，寻找{}名称 
				if (typeTemp.strType == "union" || typeTemp.strType == "struct")
				{
					char *p = pTemp; 
					if ((p = strchr(p, '{')) && (p = strchr(p, '}')))
					{
						GetLine(p,  pLineTail);
						//获取对象
						pWHead =  GetNoneNullChar(p+1, pLineTail); 
						assert(GetWord(pWHead, pLineTail, pWHead, pWTail)); 
						assert(CopyWord(pWHead, pWTail,typeTemp.strName)); 	

						struTypeInfo.SubItem.push_back(typeTemp); 
						struTypeInfo.sSubItemNum ++; 
						pTemp =   pLineTail + 1;
						continue; 
					}
					else
						assert(false); 
				}
				


				//获取对象
				pWHead =  GetNoneNullChar(pWTail+1, pLineTail); 
				assert(GetWord(pWHead, pLineTail, pWHead, pWTail)); 
				assert(CopyWord(pWHead, pWTail,typeTemp.strName)); 			 
				struTypeInfo.SubItem.push_back(typeTemp); 
				struTypeInfo.sSubItemNum ++; 

				//查找}是否存在
				if (strchrInMen( pWTail+1,  pLineTail, '}') )

				{

				}
				else if ( strstrInMen(pWTail+1, pLineTail, "/*"))
				{
					//'/*'注释在行后， 寻找*/跳过
					//查找"*/"去掉  整行
					pTemp = strstr(pTemp, "*/");
					assert(pTemp); 				
					pTemp = strchr(pTemp+2, '\n'); 
					assert(pTemp); 
					pTemp ++; 
					continue; 
				}
				else if (strchrInMen(pWTail+1, pLineTail, ';'))
				{
					//正确的

				}
			}
			else if (cFirstChar == '}')
			{
				assert(GetWord(pFirstChar, pLineTail, pWHead, pWTail)); 
				assert(CopyWord(pWHead, pWTail, struTypeInfo.strType)); 
				if ( !IsStrHasSmallAlpha(pWHead, pWTail-pWHead+1) )
				{
					vStructData.push_back(struTypeInfo);
				}
				break; 
			}
			else
			{
				//assert(false); 
			}
			pTemp = pLineTail + 1; 
		}
	}
	return true; 
}


//获取一行 如果找到 pLTail返回行尾  
bool GetLine( char *szSrc,  char *&pLTail )
{
	if (strlen(szSrc) == 1)
	{
		//只有一个为文件末尾
		return false; 
	}
	char *pTemp = szSrc; 
	if (!(pTemp = ( char *)strchr((char *)pTemp, '\n')) )
	{
		//文件末尾   应该返回真 处理最后一行
		pLTail = szSrc + strlen(szSrc); 
		return true; 
	}
	pLTail = pTemp; 
	try
	{
		if (*(pTemp+1) == '\t')
		{
			pLTail = pTemp + 1; 
		}
	}catch(...)
	{
		return true;
	}

	return true; 
}

//获取一个单词
bool GetWord( char *szSrc,  char *pLTail, char *&pWHead,  char *&pWTail)
{
	char *pTemp = szSrc; 
	while(pTemp <= pLTail && *pTemp != '_' && !IsAlpha(*pTemp))
	{
		pTemp ++; 
	}
	pWHead = pTemp; 
	while (pTemp <= pLTail && (IsAlnum(*pTemp) || *pTemp == '_'))
	{
		pTemp ++; 
	}
	if (pTemp > pLTail)
	{
		return false; 
	}

	pWTail = pTemp - 1; 
	return true; 
}
//获取第一个非空的字符
char* GetNoneNullChar( char *szSrc,  char *pLTail )
{
	char *pTemp = szSrc; 
	while ( pTemp <= pLTail && (*pTemp == ' '|| *pTemp < 31))
	{		
		pTemp++; 
	}
	if (pTemp <= pLTail)
	{
		return (char *)pTemp;
	}
	return NULL; 
}

//赋值单词
bool CopyWord( char *pSrcHead,  char* pSrcTail, string &dst)
{
	if (!pSrcHead || !pSrcTail )
	{
		return false; 
	}
	char szTemp[128] = {0}; 
	memcpy(szTemp, pSrcHead, pSrcTail-pSrcHead+1); 
	dst = szTemp; 
	return true; 

}
//在一段内存内  查找某个字符是否存在
bool strchrInMen( char *pS,  char* pT,  char c)
{
	if (pS > pT)
	{
		return false; 
	}
	char *pTemp = pS; 
	while (pTemp<=pT)
	{
		if ( *pTemp++ == c)
		{
			return true; 
		}
	}
	return false; 
}

//在一段内存内  查找某个字符串是否存在
bool strstrInMen( char *pS,  char* pT,  char* str)
{
	if (pS > pT)
	{
		return false; 
	}
	try
	{
		char *p ; 
		p = strstr(pS, str);
		if (!p || p>pT)
		{
			return false; 
		}
		return true; 
	}
	catch (...)
	{
		return false; 
	}
}

bool AnalyseStruct(char *pMemStart, vector<TypeInfo>& szStructStr)
{
	char *pTemp = pMemStart;
	char *pReturn = NULL; 
	unsigned int dwStrLen = strlen(pMemStart); 
	while((pTemp = strstr(pTemp, "struct")) != NULL)
	{
		TypeInfo typeTemp; 
		string strLastWord = LastWord(pMemStart, pTemp); 	
		if (strLastWord == "typedef")
		{
			typeTemp.SetTypedefFlag(); 
		}
		pReturn = AnalyseStruct(pTemp, pMemStart+dwStrLen, typeTemp ); 
		if ( ! pReturn)
		{
			break; 
		}
		szStructStr.push_back(typeTemp); 
		pTemp = pReturn ; 
	}
	return true; 
}



char * AnalyseStruct(char *pFirst,  char *pEnd, TypeInfo& vStructData)
{
	char* pWHead, *pWTail; 
	GetWord(pFirst, pEnd, pWHead, pWTail); 
	string strTemp; 
	CopyWord(pWHead, pWTail,strTemp); 
	//判断第一单词非struct 或 union，返回错误
	if (strTemp != "struct" && strTemp != "union" )
	{
		return NULL; 
	}
	char *pTemp = pFirst; 
	char *pLineTail = NULL; 
	//寻找{ 
	if (!(pTemp = strchr(pTemp, '{')) )
	{
		return NULL; 
	}
	TypeInfo struTypeInfo = vStructData;
	//开始一行一行处理
	while( pTemp < pEnd && GetLine(pTemp,  pLineTail)   )
	{
		char *pFirstChar =  GetNoneNullChar(pTemp, pLineTail); 
		if (!pFirstChar)
		{
			pTemp = pLineTail + 1; 
			continue; 
		}	
		char cFirstChar = *pFirstChar; 
		if (cFirstChar == '/')
		{
			char cSecondChar = *(pFirstChar+1);
			if (cSecondChar == '*')
			{
				//查找"*/"去掉  整行
				pTemp = strstr(pTemp, "*/");
				assert(pTemp); 				
				pTemp = strchr(pTemp+2, '\n'); 
				assert(pTemp); 
				pTemp ++; 
				continue; 
			}
		//其他 整行去掉 不处理
		}
		//第一个字符为字母或_， 可能是一个类型
		else if (IsAlpha(cFirstChar) || cFirstChar == '_')
		{
			//获取类型
			assert(GetWord(pFirstChar, pLineTail, pWHead, pWTail)); 
			TypeInfo typeTemp; 
			typeTemp.sSubItemNum = 0; 
			assert(CopyWord(pWHead, pWTail,typeTemp.strType));
			//如果是union 或者struct     
			if (typeTemp.strType == "union" || typeTemp.strType == "struct")
			{
				char * pStructEnd = NULL; 
				if( (pStructEnd = AnalyseStruct(pWHead, pEnd, typeTemp)) )
				{
					struTypeInfo.SubItem.push_back(typeTemp); 
					struTypeInfo.sSubItemNum ++; 
					pTemp = pStructEnd; 
					continue; 
				}
				else 
					assert(false); 
			}			
			//获取对象
			pWHead =  GetNoneNullChar(pWTail+1, pLineTail); 
			assert(GetWord(pWHead, pLineTail, pWHead, pWTail)); 
			assert(CopyWord(pWHead, pWTail,typeTemp.strName)); 		
		
			struTypeInfo.SubItem.push_back(typeTemp); 
			struTypeInfo.sSubItemNum ++; 
			//查找}是否存在
			if (strchrInMen( pWTail+1,  pLineTail, '}') )
			{

			}
			else if(strstrInMen(pWTail+1, pLineTail, "/*"))
			{
				//'/*'注释在行后， 寻找*/跳过
				//查找"*/"去掉  整行
				pTemp = strstr(pTemp, "*/");
				assert(pTemp); 				
				pTemp = strchr(pTemp+2, '\n'); 
				assert(pTemp); 
				pTemp ++; 
				continue; 
			}
		}
		else if (cFirstChar == '}')
		{
			assert(GetWord(pFirstChar, pLineTail, pWHead, pWTail));
			assert(CopyWord(pWHead, pWTail, struTypeInfo.GetTypeOrName())); 
			//if ( !IsStrHasSmallAlpha(pWHead, pWTail-pWHead+1) )
			//{
			vStructData = struTypeInfo;
			/*}*/
			return pLineTail; 
		}
		pTemp = pLineTail + 1; 
	}
	return NULL; 
}

//查找上一个单词
string LastWord(char *pFirst,  char *pFindPos)
{
	if (pFindPos < pFirst)
	{
		return string(); 
	}
	//查找上一个单词的结尾 
	char *pTemp = pFindPos-1; 
	while (pTemp >=  pFirst && !(IsAlnum(*pTemp) || *pTemp == '_'))
	{
		pTemp --; 
	}
	if (pTemp <  pFirst)
	{
		return string(); 
	}
	char *pWordTail = pTemp; 
	//查找上一个单词的开始
	while ((IsAlnum(*pTemp) || *pTemp == '_'))
	{
		pTemp --; 
	}
	char *pWordHead =  pTemp+1; 
	string strWord; 
	CopyWord(pWordHead, pWordTail, strWord); 
	return strWord; 
}