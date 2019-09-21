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
		cout<<"���ӷ�����ʧ�ܣ�"<<GetLastError()<<endl;
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
	puts("\n\t\t 1�����������Ϣ");
	puts("\n\t\t 2�������Ϣ");
	puts("\n\t\t 3��ɾ����Ϣ");
	puts("\n\t\t 4���޸���Ϣ");
	puts("\n\t\t 5��������Ϣ");
	puts("\n\t\t 6����ɫ����");
	puts("\n\t\t 0���˳�");
	puts("\t\t��ѡ��");
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
	m_sock.Send(&nType,sizeof(nType));//����Э�����
	int i = 0,nSize = 0;
	m_sock.Receive(&nSize,sizeof(nSize));
	if(!nSize)
	{
		cout<<"������"<<endl;
		return true;
	}
	cout<<"����\t����\t����"<<endl;
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
	cout<<"�����빤��\t";
	cin>>data.num;
	if(Check(data.num))
		cout<<"�ú����Ѵ���"<<endl;
	else
	{
		cout<<"����������\t����"<<endl;
		cin>>data.name>>data.salary;
		int nType = REQ_ADD;
		m_sock.Send(&nType,sizeof(nType));
		m_sock.Send(&data,sizeof(data));
	}
	Browse();
	cout<<"�Ƿ����������ݣ�[y/n]"<<endl;
	char c = _getch();
	putchar(c);
	putchar('\n');
	return c == 'y'||c== 'Y';
}


bool CStudent::Delete(void)
{
	int num;
	cout<<"������Ҫɾ���Ĺ��ţ�"<<endl;
	cin>>num;
	int nType = REQ_DEL;
	m_sock.Send(&nType,sizeof(nType));
	m_sock.Send(&num,sizeof(num));
	Browse();
	cout<<"�Ƿ��������ɾ����[y/n]"<<endl;
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
	cout<<"������Ҫ���ҵĹ���"<<endl;
	cin>>num;
	m_sock.Send(&nType,sizeof(nType));
	m_sock.Send(&num,sizeof(num));
	m_sock.Receive(&ret,sizeof(ret));
	if(ret)
	{
		m_sock.Receive(&data,sizeof(data));
		cout<<"����"<<'\t'<<"����"<<'\t'<<"����"<<endl;
		cout<<data.num<<'\t'<<data.name<<'\t'<<data.salary<<endl;
	}
	else
	{
		cout<<"û�ҵ��ù���"<<endl;
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
	puts("\n\t\t 1�������Ų���");
	puts("\n\t\t 2������������");
	puts("\n\t\t 3�������ʶβ���");
	puts("\n\t\t 0���˳�");
	puts("\t\t��ѡ��");
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
	cout<<"������Ҫ���ҵ�����"<<endl;
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
		cout<<"����"<<'\t'<<"����"<<'\t'<<"����"<<endl;
		cout<<data.num<<'\t'<<data.name<<'\t'<<data.salary<<endl;
	}
}


void CStudent::Find_Salary(void)
{	
	int ret,nType = REQ_FIND_SALARY;
	DATA data;
	cout<<"������Ҫ���ҵ��Ĺ�������"<<endl;
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
		cout<<"����"<<'\t'<<"����"<<'\t'<<"����"<<endl;
		cout<<data.num<<'\t'<<data.name<<'\t'<<data.salary<<endl;
	}
}


bool CStudent::SortMenu(void)
{
	system("cls");
	puts("\n\t\t 1������������");
	puts("\n\t\t 2������������");
	puts("\n\t\t 3������������");
	puts("\n\t\t 0���˳�");
	puts("\t\t��ѡ��");
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
			cout<<"����"<<'\t'<<"����"<<'\t'<<"����"<<endl;
			cout<<data.num<<'\t'<<data.name<<'\t'<<data.salary<<endl;;
		}
	}
	else
	{
		cout<<"������"<<endl;
	}

	return false;
}


void CStudent::Browse_Sort(list<DATA>& cl)
{
	int nType = REQ_BROW;
	m_sock.Send(&nType,sizeof(nType));//����Э�����
	int i = 0,nSize = 0;
	m_sock.Receive(&nSize,sizeof(nSize));
	if(!nSize)
	{
		cout<<"������"<<endl;
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
