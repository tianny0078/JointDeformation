#pragma once
#include<vector>
#include<algorithm>
template<class T>
class HNEventGenerator
{
protected:
	std::vector<T *> _pEventHandlers;
public:
	void addEventHandler(T *eventHandler) {
		_pEventHandlers.push_back(eventHandler);
	}

	void removeEventHandler(T *eventHandler) {
		_pEventHandlers.erase(std::find(_pEventHandlers.begin(), _pEventHandlers.end(), eventHandler), _pEventHandlers.end());
	}

	HNEventGenerator(void) {}
	virtual bool init() = 0;
	virtual bool startGenerating() = 0;
	virtual bool stopGenerating() = 0;
	~HNEventGenerator(void){}
};

