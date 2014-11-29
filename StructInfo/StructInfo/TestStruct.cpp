#include <stdlib.h>
#include "Core.h"


//#include "StructSize.h"


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
void PrintStruct(TypeInfo &item)
{
	cout << item.strType << " " << item.strName << endl; 
	if ( ! item.sSubItemNum )
	{
		return ; 
	}
	cout << "Item Number: " << item.sSubItemNum << endl; 
	vector<TypeInfo>::iterator iter = item.SubItem.begin(); 
	while (iter != item.SubItem.end())
	{
		PrintStruct(*iter); 
		++ iter; 
	}
}
void TestNestStruct(vector<TypeInfo> &data)
{
	//打印NET_DVR_ALARMINFO_FIXED_HEADER 结构体信息
	vector<TypeInfo>::iterator iter = data.begin(); 
	while (iter != data.end() )
	{
		if (iter->strType ==  string("NET_DVR_ALRAM_FIXED_HEADER"))
		{
			//迭代打印
			PrintStruct(*iter); 
			return; 
		}
		++ iter ; 
	}
}


int main()
{
	FILE *fp; 
	fp = fopen("HCNetSDK.h", "rb");
	assert(fp); 
	int iNum ; 
	fseek(fp, 0L, 2);
	iNum = ftell(fp); 
	rewind(fp);
	char *p = new char[iNum+2000]; 
	p[iNum] = 0; 
	p[iNum+1] = 0; 
	fread(p, 1, iNum+2000, fp);
	fclose(fp);
	vector<TypeInfo>  vStructData; 
	if (! AnalyseStruct(p, vStructData))
	{
		delete [] p ; 
		return 0; 
	}
//	WirteFile(vStructData); 
//	TestPrint(vStructData); 
	TestNestStruct(vStructData); 
	cin>>iNum; 
	delete [] p ; 
	return 0; 
}