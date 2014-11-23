#include <stdlib.h>
#include "Core.h"


#include "StructSize.h"


void TestPrint(vector<TypeInfo> &TypeData)
{
	vector<TypeInfo>::iterator iter = TypeData.begin(); 
	while (iter != TypeData.end())
	{
		cout << "Type:" << iter->strType << " SubItemNum: " << iter->sSubItemNum << endl; 
		iter ++; 	
	}
}

void WirteFile(vector<TypeInfo> &TypeData)
{
	FILE *fp; 
	fp = fopen("StructSize.h", "w");
	assert(fp);  
	fprintf(fp, "#include \"HCNetSDK.h\"\n"); 
	string csWStrutName; 
	string csWStrutSize;
	csWStrutName = "char *StructName[] = {";
	csWStrutSize =  "DWORD dwStructSize[] = {"; 
	string csTemp; 
	char szLan[1024]; 
	vector<TypeInfo>::iterator iter = TypeData.begin();
	unsigned int dwCount = 0; 
	while (iter != TypeData.end())
	{
		csTemp = iter->strType; 
		if ( dwCount % 10 == 9)
		{
			sprintf(szLan,"\"%s\",\\\n\t", iter->strType.c_str() ); 
			csTemp = szLan; 
		}
		else
		{
			csTemp += ", "; 
			sprintf(szLan,"\"%s\", ", iter->strType.c_str() ); 
			csTemp = szLan; 
		}
		csWStrutName += csTemp; 

		if ( dwCount % 10 == 9)
		{
			sprintf(szLan,"sizeof(%s),\n\t ", iter->strType.c_str() ); 
			csTemp = szLan; 
		}
		else
		{
			sprintf(szLan,"sizeof(%s), ", iter->strType.c_str() ); 
			csTemp = szLan; 
		}
		csWStrutSize += csTemp;
		dwCount++; 
		iter ++; 	
	}
	csWStrutName += " }; "; 
	csWStrutSize  += " }; "; 
	fprintf(fp,"%s \n ",  csWStrutName.c_str()); 
	fprintf(fp,"%s",  csWStrutSize.c_str()); 
	fclose(fp);
}



int main()
{
	FILE *fp; 
	fp = fopen("HCNetSDK.h", "r");
	assert(fp); 
	int iNum ; 
	fseek(fp, 0, 2);
	iNum = ftell(fp); 
	rewind(fp);
	char *p = new char[iNum+2]; 
	p[iNum] = 0; 
	p[iNum+1] = 0; 
	fread(p, 1, iNum, fp);
	fclose(fp);
	vector<TypeInfo>  vStructData; 
	if (! AnalyseStrtoStruct(p, vStructData))
	{
		return 0; 
	}
	//WirteFile(vStructData); 
	//TestPrint(vStructData); 
	string strFind("NET_DVR_CLOUDSTORAGE_COND"); 
	unsigned int uiLenStructSize = sizeof(dwStructSize)/sizeof(dwStructSize[0]); 
	unsigned int uiLenStructName = sizeof(StructName)/sizeof(StructName[0]); 
	assert(uiLenStructSize == uiLenStructName); 
	for (int i = 0; i < uiLenStructName; i++)
	{

		if (strcmp(strFind.c_str(),StructName[i]) == 0)
		{
			cout << StructName[i] << ":" << dwStructSize[i] << endl; 
			break; 
		}
	}
	cin>>iNum; 
	delete [] p ; 
	return 0; 
}