#ifndef CCMDINTERP_HPP
#define CCMDINTERP_HPP

#include <fstream>
#include <string>
#include "ckeysstorage.h"

class cCmdInterp
{
public:
	cCmdInterp(std::string pFifoName, std::string pInstance);
	void cmdReadLoop();
private:
	std::ifstream inputFIFO;
	cKeysStorage keyStorage;
	std::string inst;
};

#endif
