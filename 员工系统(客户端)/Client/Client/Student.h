#pragma once
#include <vector>
#include "sokethd.h"
#include <list>
class CStudent
{
	typedef struct DATA
	{
		unsigned int num;
		char name[20];
		float salary;
	};
	std::vector<DATA> m_vect; 
	CSokethd m_sock;
	//typedef bool (*P_FUN)(DATA& data1,DATA& data2);
	static bool Sort_Num(DATA& data1,DATA& data2)
	{
		return data1.num>data2.num;
	}
	static bool Sort_Name(DATA& data1,DATA& data2)
	{
		return strcmp(data1.name,data2.name)<0;
	}
	static bool Sort_Salary(DATA& data1,DATA& data2)
	{
		return data1.salary>data2.salary;
	}
	void Load(void);
	void Save(void);
	bool OnReceive(CSokethd&);
public:
	CStudent(void);
	~CStudent(void);
	void Start(void);
	int Menu(void);
	bool Browse(void);
	bool Input(void);
	bool Delete(void);
	bool Modify(void);
	void Find_Num(void);
	bool Check(int num);
	bool Menu_Find(void);
	void Find_Name(void);
	void Find_Salary(void);
	bool SortMenu(void);
	void Browse_Sort(std::list<DATA>& cl);
};

