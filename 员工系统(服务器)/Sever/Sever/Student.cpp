#include "Student.h"
#include <iostream>
#include "define.h"
#include <process.h>
using namespace std;
CStudent CStudent::m_hd;

CStudent::CStudent(void)
{
}


CStudent::~CStudent(void)
{
}


void CStudent::Start(void)
{
	Load();
	if(!m_sock.Creat(PORT))
	{
		cout<<GetLastError()<<endl; 
		return ;
	}
	m_sock.Listen();
	CSokethd* psocka = new CSokethd;
	UINT nPort;
	char sIP[20];
	while(m_sock.Accept(*psocka,sIP,&nPort))
	{
		cout<<sIP<<"-"<<nPort<<"登录："<<endl;
		_beginthread(&theProc,0,psocka);
		psocka = new CSokethd;
	}
	delete psocka;
}

void CStudent::Load(void)
{
	FILE* fp = NULL;
	fopen_s(&fp,"haha.hd","r");
	if(!fp)
	{
		//cout<<"读取文件失败："<<GetLastError()<<endl;
		return;	
	}
	DATA data;
	while(fread(&data,1,sizeof(DATA),fp) == sizeof(DATA))
		m_vect.push_back(data);
	fclose(fp);
}


void CStudent::Save(void)
{
	FILE* fp = NULL;
	fopen_s(&fp,"haha.hd","w");
	if(!fp)
	{
		cout<<"保存文件失败："<<GetLastError()<<endl;
		return;	
	}
	int i = 0,nSize = m_vect.size();
	while(i<nSize)
	{
		fwrite(&m_vect[i],1,sizeof(DATA),fp);
		i++;
	}
	fclose(fp);
}

//协议解析函数
bool CStudent::OnReceive(CSokethd& socka)
{
	int nType = 0;
	if(socka.Receive(&nType,sizeof(nType))<=0)
		return false;
	switch(nType)
	{
	case REQ_BROW:
		return OnBrow(socka);
	case REQ_ADD:
		return OnAdd(socka);
	case REQ_DEL:
		return OnDelete(socka);
		break;
	case REQ_MOD:
		break;
	case REQ_FIND_NUM:
		return Find_Num(socka);
		break;
	case REQ_FIND_NAME:
		return Find_Name(socka);
		break;
	case REQ_FIND_SALARY:
		return Find_Salary(socka);
		break;
	case REQ_CHECK:
		return Check(socka);
	case ADM_LOGIN:
		return m_admin.OnLogin(socka);
	case ADM_ADD:
		return true;
	case ADM_DEL:
		return true;
		
	}
	return false;
}

					  
bool CStudent::OnBrow(CSokethd& socka)
{
	int nSize = m_vect.size(),i = 0;
	socka.Send(&nSize,sizeof(nSize));
	while(i<nSize)
	{
		DATA& data = m_vect[i++];
		socka.Send(&data,sizeof(DATA));
	}
	return true;
}


bool CStudent::OnAdd(CSokethd& socka)
{
	DATA data;
	if(socka.Receive(&data,sizeof(data))<=0)
		return false;//表示客户端断开了
	m_vect.push_back(data);
	Save();
	return true ;
}


bool CStudent::OnDelete(CSokethd& socka)
{
	int num,i = 0,nSize = m_vect.size();
	if(socka.Receive(&num,sizeof(num))<=0)
		return false;
	while(i<nSize)
	{
		if(num == m_vect[i].num)
		{
			m_vect.erase(m_vect.begin()+i);
			break;
		}
		i++;
	}
	return true;
}


bool CStudent::Check(CSokethd& socka)
{
	int num,i = 0,nSize = m_vect.size();
	if(socka.Receive(&num,sizeof(num))<=0)
		return false;
	int ret = 0;
	while(i<nSize)
	{
		if(num == m_vect[i].num)
		{
			ret = 1;
			socka.Send(&ret,sizeof(ret));
			return true;
		}
		i++;
	}
	socka.Send(&ret,sizeof(ret));
	return true;
}


bool CStudent::Find_Num(CSokethd& socka)
{
	int num,i = 0,nSize = m_vect.size();
	if(socka.Receive(&num,sizeof(num))<=0)
		return false;
	int ret = 0;
	while(i<nSize)
	{
		if(num == m_vect[i].num)
		{
			ret = 1;
			socka.Send(&ret,sizeof(ret));
			socka.Send(&m_vect[i],sizeof(DATA));
			return true;
		}
		i++;
	}
	socka.Send(&ret,sizeof(ret));
	return true;
}


bool CStudent::Find_Name(CSokethd& socka)
{
	char name[20],i = 0,nSize = m_vect.size();
	if(socka.Receive(name,sizeof(name))<=0)
		return false;
	int ret = 0;
	while(i<nSize)
	{
		if(!strcmp(m_vect[i].name,name))
		{
			ret = 1;
			socka.Send(&ret,sizeof(ret));
			socka.Send(&m_vect[i],sizeof(DATA));
			//return true;
		}
		i++;
	}
	ret = 0;
	socka.Send(&ret,sizeof(ret));
	return true;
}


bool CStudent::Find_Salary(CSokethd& socka)
{
	float salary1,salary2;
	int i = 0,nSize = m_vect.size();
	if(socka.Receive(&salary1,sizeof(salary1))<=0)
		return false;
	if(socka.Receive(&salary2,sizeof(salary2))<=0)
		return false;
	int ret = 0;
	while(i<nSize)
	{
		if(m_vect[i].salary>=salary1 &&m_vect[i].salary<=salary2 )
		{
			ret = 1;
			socka.Send(&ret,sizeof(ret));
			socka.Send(&m_vect[i],sizeof(DATA));
			//return true;
		}
		i++;
	}
	ret = 0;
	socka.Send(&ret,sizeof(ret));
	return true;
}
