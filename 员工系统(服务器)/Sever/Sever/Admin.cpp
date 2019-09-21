#include "Admin.h"
#include <WinSock2.h>
#include <iostream>
using namespace std;

CAdmin::CAdmin(void)
{
}


CAdmin::~CAdmin(void)
{
}



void CAdmin::Save(void)
{
	FILE* fp = NULL;
	fopen_s(&fp,"admin.hd","w");
	if(!fp)
	{
		cout<<"保存文件失败："<<GetLastError()<<endl;
		return;	
	}
	list<DATA>::const_iterator it=m_list.begin();
	while(it!=m_list.end())
	{
		fwrite(&(*it),1,sizeof(DATA),fp);//注意这里的&(*it)不能直接写成it
		it++;
	}
	fclose(fp);
}

void CAdmin::Load(void)
{
	FILE* fp = NULL;
	fopen_s(&fp,"admin.hd","r");
	if(!fp)
	{
		//cout<<"读取文件失败："<<GetLastError()<<endl;
		return;	
	}
	DATA data;
	while(fread(&data,1,sizeof(DATA),fp) == sizeof(DATA))
		m_list.push_back(data);
	if(!m_list.size())
	{//如果读取到的数据为空，则默认一个账户
		DATA data1 = {"admin","admin",0};
		m_list.push_back(data1);
	}
	fclose(fp);
}


bool CAdmin::OnLogin(CSokethd& socka)
{
	DATA data;
	if(socka.Receive(&data,sizeof(data))<=0)
		return false;
	list<DATA>::const_iterator it = m_list.begin();
	int ret=-1;
	while(it!=m_list.end())
	{
		const DATA& d =*it++;
		if(0 == stricmp(d.name,data.name))//不区分大小写比较
		{
			if(0 == strcmp(d.pass,data.pass))
			{
				ret = d.prio;
			}
			break;
		}
	}
	socka.Send(&ret,sizeof(ret));  
	return false;
}
