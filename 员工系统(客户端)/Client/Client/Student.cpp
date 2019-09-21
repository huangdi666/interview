#include "Student.h"
#include <iostream>
#include <conio.h>
#include "define.h"
using namespace std;
CStudent::CStudent(void)
{
}


CStudent::~CStudent(void)
{
}


void CStudent::Start(void)
{
	if(!m_sock.Creat())
	{
		cout<<GetLastError()<<endl; 
		return ;
	}
	if(!m_sock.Connect("192.168.8.136",PORT))
	{
		cout<<"链接服务器失败："<<GetLastError()<<endl;
		return;
	}
	while(Menu()); 
}


void CStudent::Load(void)
{
}


void CStudent::Save(void)
{
}

int CStudent::Menu(void)
{
	system("cls");
	puts("\n\t\t 1、浏览所有信息");
	puts("\n\t\t 2、添加信息");
	puts("\n\t\t 3、删除信息");
	puts("\n\t\t 4、修改信息");
	puts("\n\t\t 5、查找信息");
	puts("\n\t\t 6、颜色设置");
	puts("\n\t\t 0、退出");
	puts("\t\t请选择：");
	int i = 0;
	scanf_s("%d",&i);
	switch(i)
	{
	case 1:
		//Browse();
		while(SortMenu());
		system("pause");
		break;
	case 2:
		while(Input());
		break;
	case 3:
		while(Delete());
		break;
	case 4:
		while(Modify());
		break;	
	case 5:
		while(Menu_Find());
		break;	
	case 6:
		//ColorSet();
		break;	
	}

	return i;
}


bool CStudent::Browse(void)
{
	int nType = REQ_BROW;
	m_sock.Send(&nType,sizeof(nType));//按照协议进行
	int i = 0,nSize = 0;
	m_sock.Receive(&nSize,sizeof(nSize));
	if(!nSize)
	{
		cout<<"无数据"<<endl;
		return true;
	}
	cout<<"工号\t姓名\t工资"<<endl;
	DATA data;
	while(i<nSize)
	{
		m_sock.Receive(&data,sizeof(data));
		cout<<data.num<<'\t'<<data.name<<'\t'<<data.salary<<endl;
		i++;
	}
	return true;
}


bool CStudent::Input(void)
{
	DATA data;
	cout<<"请输入工号\t";
	cin>>data.num;
	if(Check(data.num))
		cout<<"该号码已存在"<<endl;
	else
	{
		cout<<"请输入姓名\t工资"<<endl;
		cin>>data.name>>data.salary;
		int nType = REQ_ADD;
		m_sock.Send(&nType,sizeof(nType));
		m_sock.Send(&data,sizeof(data));
	}
	Browse();
	cout<<"是否继续添加数据？[y/n]"<<endl;
	char c = _getch();
	putchar(c);
	putchar('\n');
	return c == 'y'||c== 'Y';
}


bool CStudent::Delete(void)
{
	int num;
	cout<<"请输入要删除的工号："<<endl;
	cin>>num;
	int nType = REQ_DEL;
	m_sock.Send(&nType,sizeof(nType));
	m_sock.Send(&num,sizeof(num));
	Browse();
	cout<<"是否继续继续删除？[y/n]"<<endl;
	char c = _getch();
	putchar(c);
	putchar('\n');
	return c == 'y'||c== 'Y';
}


bool CStudent::Modify(void)
{
	return false;
}


void CStudent::Find_Num(void)
{
	int ret,num,nType = REQ_FIND_NUM;
	DATA data;
	cout<<"请输入要查找的工号"<<endl;
	cin>>num;
	m_sock.Send(&nType,sizeof(nType));
	m_sock.Send(&num,sizeof(num));
	m_sock.Receive(&ret,sizeof(ret));
	if(ret)
	{
		m_sock.Receive(&data,sizeof(data));
		cout<<"工号"<<'\t'<<"姓名"<<'\t'<<"工资"<<endl;
		cout<<data.num<<'\t'<<data.name<<'\t'<<data.salary<<endl;
	}
	else
	{
		cout<<"没找到该工号"<<endl;
	}
}


bool CStudent::Check(int num)
{
	int ret,nType = REQ_CHECK;
	m_sock.Send(&nType,sizeof(nType));
	m_sock.Send(&num,sizeof(num));
	m_sock.Receive(&ret,sizeof(ret));
	return ret==1;
}


bool CStudent::Menu_Find(void)
{
	system("cls");
	puts("\n\t\t 1、按工号查找");
	puts("\n\t\t 2、按姓名查找");
	puts("\n\t\t 3、按工资段查找");
	puts("\n\t\t 0、退出");
	puts("\t\t请选择：");
	int i = 0;
	scanf_s("%d",&i);
	switch(i)
	{
	case 1:
		Find_Num();
		system("pause");
		break;
	case 2:
		Find_Name();
		system("pause");
		break;
	case 3:
		Find_Salary();
		system("pause");
		break;
	case 0:
		//ColorSet();
		break;	
	}

	return i;
}


void CStudent::Find_Name(void)
{
	int ret,nType = REQ_FIND_NAME;
	DATA data;
	cout<<"请输入要查找的姓名"<<endl;
	char name[20];
	cin>>name;
	m_sock.Send(&nType,sizeof(nType));
	m_sock.Send(name,sizeof(name));
	while(true)
	{
		m_sock.Receive(&ret,sizeof(ret));
		if(!ret)
			break;
		m_sock.Receive(&data,sizeof(data));
		cout<<"工号"<<'\t'<<"姓名"<<'\t'<<"工资"<<endl;
		cout<<data.num<<'\t'<<data.name<<'\t'<<data.salary<<endl;
	}
}


void CStudent::Find_Salary(void)
{	
	int ret,nType = REQ_FIND_SALARY;
	DATA data;
	cout<<"请输入要查找到的工资区间"<<endl;
	float salary1,salary2;
	cin>>salary1>>salary2;
	m_sock.Send(&nType,sizeof(nType));
	m_sock.Send(&salary1,sizeof(salary1));
	m_sock.Send(&salary2,sizeof(salary2));
	while(true)
	{
		m_sock.Receive(&ret,sizeof(ret));
		if(!ret)
			break;
		m_sock.Receive(&data,sizeof(data));
		cout<<"工号"<<'\t'<<"姓名"<<'\t'<<"工资"<<endl;
		cout<<data.num<<'\t'<<data.name<<'\t'<<data.salary<<endl;
	}
}


bool CStudent::SortMenu(void)
{
	system("cls");
	puts("\n\t\t 1、按工号排序");
	puts("\n\t\t 2、按姓名排序");
	puts("\n\t\t 3、按工资排序");
	puts("\n\t\t 0、退出");
	puts("\t\t请选择：");
	int i = 0;
	scanf_s("%d",&i);
	if(i>=1&&i<=3)
	{
		list<DATA> cl;
		Browse_Sort(cl);
		bool (*p[])(DATA& data1,DATA& data2)={Sort_Num,Sort_Name,Sort_Salary};
		cl.sort(p[i-1]);
		list<DATA>::iterator it = cl.begin();
		DATA data;
		while(it!=cl.end())
		{
			data = *it++;
			cout<<"工号"<<'\t'<<"姓名"<<'\t'<<"工资"<<endl;
			cout<<data.num<<'\t'<<data.name<<'\t'<<data.salary<<endl;;
		}
	}
	else
	{
		cout<<"无数据"<<endl;
	}

	return false;
}


void CStudent::Browse_Sort(list<DATA>& cl)
{
	int nType = REQ_BROW;
	m_sock.Send(&nType,sizeof(nType));//按照协议进行
	int i = 0,nSize = 0;
	m_sock.Receive(&nSize,sizeof(nSize));
	if(!nSize)
	{
		cout<<"无数据"<<endl;
		return;
	}
	DATA data;
	while(i<nSize)
	{
		m_sock.Receive(&data,sizeof(data));
		cl.push_back(data);
		i++;
	}
	return;

}
