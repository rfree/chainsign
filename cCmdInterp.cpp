#include "cCmdInterp.hpp"
#include <iostream>

cCmdInterp::cCmdInterp(std::string pFifoName)
{
	inputFIFO.open(pFifoName);
}

void cCmdInterp::cmdReadLoop()
{
	std::string line;
	while (1)
	{
		inputFIFO.open("fifo");
		std::getline(inputFIFO, line);
		std::cout << line << std::endl;
		inputFIFO.close();
		if (line == "QUIT")
			break;
		else if(line == "SIGN-NEXTKEY")
		{
			std::getline(inputFIFO, line);
			std::cout << "path " << line << std::endl;
		}
			
	}
}
