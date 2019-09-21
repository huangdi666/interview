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
		cout<<"�����ļ�ʧ�ܣ�"<<GetLastError()<<endl;
		return;	
	}
	list<DATA>::const_iterator it=m_list.begin();
	while(it!=m_list.end())
	{
		fwrite(&(*it),1,sizeof(DATA),fp);//ע�������&(*it)����ֱ��д��it
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
		//cout<<"��ȡ�ļ�ʧ�ܣ�"<<GetLastError()<<endl;
		return;	
	}
	DATA data;
	while(fread(&data,1,sizeof(DATA),fp) == sizeof(DATA))
		m_list.push_back(data);
	if(!m_list.size())
	{//�����ȡ��������Ϊ�գ���Ĭ��һ���˻�
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
		if(0 == stricmp(d.name,data.name))//�����ִ�Сд�Ƚ�
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
