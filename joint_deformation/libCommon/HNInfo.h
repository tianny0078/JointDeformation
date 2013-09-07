#pragma once
#define HOSTNAME_SIZE 100
#include<cstring>
#include "HNLogger.h"

class HNInfo
{
public:
	char name[HOSTNAME_SIZE];


	HNInfo(void) { }
	HNInfo(HNInfo &info) { set(info);}
	HNInfo(char *name) { 
		set(name);
	}

	virtual void setName(char *name) { strcpy(this->name, name);}
	virtual void set(char *name) { setName(name);}
	virtual void set(HNInfo &info) { set(info.name);}
	~HNInfo(void) { }
};

