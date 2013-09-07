#pragma once
#define PATH_SIZE 100
#include<cstring>
#include "HNInfo.h"
#include "HNLogger.h"

class HNFileInfo:public HNInfo
{
public:
	char path[PATH_SIZE];
	bool isWritable, repeat;
	int startAfter, endBefore, repeatFrom;

	HNFileInfo(void) { }
	HNFileInfo(HNInfo &info) { set(info);}

	HNFileInfo(char *path, char *name, bool isWritable = false, bool repeat = true, int startAfter = 0, int endBefore = 0, int repeatFrom = 0) {
		set(path, name, isWritable, repeat, startAfter, endBefore, repeatFrom);
	}

	virtual void setName(char *name) { strcpy_s(this->name, name);}
	void set(char *name) { setName(name);}
	void set(char *path, char *name, bool isWritable = false, bool repeat = true, int startAfter = 0, int endBefore = 0, int repeatFrom = 0) {
		setPath(path, name);
		set(name);
		this->isWritable = isWritable;
		this->repeat = repeat;
		this->startAfter = startAfter;
		this->endBefore = endBefore;
		this->repeatFrom = repeatFrom;
	}
	void setPath(char* path, char *name){ sprintf(this->path, "%s%s", path, name);}
	void set(HNInfo &info) {set(info.name);}
	char* getFilePath() { return path; }
	~HNFileInfo(void) HNLOGGER_ENTERED }
};

