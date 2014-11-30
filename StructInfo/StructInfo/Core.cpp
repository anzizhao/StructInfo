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
	if (strlen(szSrc) == 1)
	{
		//ֻ��һ��Ϊ�ļ�ĩβ
		return false; 
	}
	char *pTemp = szSrc; 
	if (!(pTemp = ( char *)strchr((char *)pTemp, '\n')) )
	{
		//�ļ�ĩβ   Ӧ�÷����� �������һ��
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
	//�жϵ�һ���ʷ�struct �� union�����ش���
	if (strTemp != "struct" && strTemp != "union" )
	{
		return NULL; 
	}
	char *pTemp = pFirst; 
	char *pLineTail = NULL; 
	//Ѱ��{ 
	if (!(pTemp = strchr(pTemp, '{')) )
	{
		return NULL; 
	}
	TypeInfo struTypeInfo = vStructData;
	//��ʼһ��һ�д���
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
		//��һ���ַ�Ϊ��ĸ��_�� ������һ������
		else if (IsAlpha(cFirstChar) || cFirstChar == '_')
		{
			//��ȡ����
			assert(GetWord(pFirstChar, pLineTail, pWHead, pWTail)); 
			TypeInfo typeTemp; 
			typeTemp.sSubItemNum = 0; 
			assert(CopyWord(pWHead, pWTail,typeTemp.strType));
			//�����union ����struct     
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
			else if(strstrInMen(pWTail+1, pLineTail, "/*"))
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

//������һ������
string LastWord(char *pFirst,  char *pFindPos)
{
	if (pFindPos < pFirst)
	{
		return string(); 
	}
	//������һ�����ʵĽ�β 
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
	//������һ�����ʵĿ�ʼ
	while ((IsAlnum(*pTemp) || *pTemp == '_'))
	{
		pTemp --; 
	}
	char *pWordHead =  pTemp+1; 
	string strWord; 
	CopyWord(pWordHead, pWordTail, strWord); 
	return strWord; 
}