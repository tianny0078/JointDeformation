#include "HNUtilities.h"


HNUtilities::HNUtilities(void)
{
}


HNUtilities::~HNUtilities(void)
{
}

template <class T>
bool HNUtilities::convertString(T &t, const std::string& s, std::ios_base& (*f)(std::ios_base&)){
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}