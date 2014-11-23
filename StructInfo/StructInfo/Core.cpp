#include "Core.h"

bool AnalyseStrtoStruct(char *szStructStr, vector<TypeInfo>& vStructData)
{
	char *pTemp = szStructStr; 
	while((pTemp = strstr(pTemp, "struct")) != NULL)
	{
		//Ѱ��{ ѹ��ջ
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
					//����"*/"ȥ��  ����
					pTemp = strstr(pTemp, "*/");
					assert(pTemp); 				
					pTemp = strchr(pTemp+2, '\n'); 
					assert(pTemp); 
					pTemp ++; 
					continue; 
				}
				//���� ����ȥ�� ������
			}
			else if (isalnum(cFirstChar) || cFirstChar == '_')
			{
				//��ȡ����

				assert(GetWord(pFirstChar, pLineTail, pWHead, pWTail)); 
				TypeInfo typeTemp; 
				typeTemp.sSubItemNum = 0; 
				assert(CopyWord(pWHead, pWTail,typeTemp.strType));
				//�����union ����struct   Ƕ�ײ�����  
				//ֻ�������֣�Ѱ��{}���� 
				if (typeTemp.strType == "union" || typeTemp.strType == "struct")
				{
					char *p = pTemp; 
					if ((p = strchr(p, '{')) && (p = strchr(p, '}')))
					{
						GetLine(p,  pLineTail);
						//��ȡ����
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
				


				//��ȡ����
				pWHead =  GetNoneNullChar(pWTail+1, pLineTail); 
				assert(GetWord(pWHead, pLineTail, pWHead, pWTail)); 
				assert(CopyWord(pWHead, pWTail,typeTemp.strName)); 			 
				struTypeInfo.SubItem.push_back(typeTemp); 
				struTypeInfo.sSubItemNum ++; 

				//����}�Ƿ����
				if (strchrInMen( pWTail+1,  pLineTail, '}') )

				{

				}
				else if ( strstrInMen(pWTail+1, pLineTail, "/*"))
				{
					//'/*'ע�����к� Ѱ��*/����
					//����"*/"ȥ��  ����
					pTemp = strstr(pTemp, "*/");
					assert(pTemp); 				
					pTemp = strchr(pTemp+2, '\n'); 
					assert(pTemp); 
					pTemp ++; 
					continue; 
				}
				else if (strchrInMen(pWTail+1, pLineTail, ';'))
				{
					//��ȷ��

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


//��ȡһ�� ����ҵ� pLTail������β  
bool GetLine( char *szSrc,  char *&pLTail )
{
	char *pTemp = szSrc; 
	if (!(pTemp = ( char *)strchr((char *)pTemp, '\n')) )
	{
		return false; 
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

//��ȡһ������
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
//��ȡ��һ���ǿյ��ַ�
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

//��ֵ����
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
//��һ���ڴ���  ����ĳ���ַ��Ƿ����
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

//��һ���ڴ���  ����ĳ���ַ����Ƿ����
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