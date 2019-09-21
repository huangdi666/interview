#pragma once
#include <vector>
#include "sokethd.h"
#include <iostream>
#include "admin.h"

class CStudent
{
	typedef struct DATA
	{
		int num;
		char name[20];
		float salary;
	};
	static void theProc(void* p)
	{
		CSokethd* psocka =(CSokethd*)p;
		while(m_hd.OnReceive(*psocka));
		delete psocka;
	}
	CAdmin m_admin;
	std::vector<DATA> m_vect; 
	CSokethd m_sock;


	void Load(void);
	void Save(void);
	bool OnReceive(CSokethd&);
	bool OnBrow(CSokethd& socka);
	bool OnAdd(CSokethd&);
	bool OnDelete(CSokethd& socka);
	bool Check(CSokethd& socka);
public:
	static CStudent m_hd;
	CStudent(void);
	~CStudent(void);
	void Start(void);
	bool Find_Num(CSokethd& socka);
	bool Find_Name(CSokethd& socka);
	bool Find_Salary(CSokethd& socka);
};

