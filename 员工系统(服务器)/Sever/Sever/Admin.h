#pragma once
#include <list>
#include "Sokethd.h"
class CAdmin
{
	typedef struct DATA
	{
		char name[20];
		char pass[20];
		int  prio;
	};
	std::list<DATA> m_list;
public:
	CAdmin(void);
	~CAdmin(void);
	void Load(void);
	void Save(void);
	bool OnLogin(CSokethd& socka);
};

