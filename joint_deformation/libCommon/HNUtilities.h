#pragma once
#include <string.h>
#include <istream>
#include <sstream>
#include <iostream>

class HNUtilities
{
public:
	HNUtilities(void);
	~HNUtilities(void);
	template<class T>
	bool convertString(T &t, const std::string& s, std::ios_base& (*f)(std::ios_base&));
};

