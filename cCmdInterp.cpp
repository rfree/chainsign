#include "cCmdInterp.hpp"
#include <iostream>

cCmdInterp::cCmdInterp(std::string pFifoName, std::string pInstance)
:inst(pInstance)
{
	inputFIFO.open(pFifoName);
	inst = "inst20141120_1242"; // TODO
}

void cCmdInterp::cmdReadLoop()
{
	std::string line;
	keyStorage.GenerateRSAKey(4096, inst + "-key" + std::to_string(keyStorage.getCurrentKey()) + ".pub"); // generate 1st key
	while (1)
	{
		std::cout << "loop" << std::endl;
		inputFIFO.open("fifo");
		std::getline(inputFIFO, line);
		std::cout << "line" << line << std::endl;
		inputFIFO.close();
		if (line == "QUIT")
			break;
		else if(line == "SIGN-NEXTKEY")
		{
			std::string pubFileName =  inst + "-key" + std::to_string(keyStorage.getCurrentKey()) + ".pub";
			system(std::string("touch " + pubFileName).c_str());
			std::cout << "SIGN-NEXTKEY" << std::endl;
			std::getline(inputFIFO, line);
			keyStorage.RSASignFile(line, line + ".sig");
			keyStorage.GenerateRSAKey(4096, pubFileName);
			keyStorage.RemoveRSAKey();
		}
			
	}
	std::cout << "loop end" << std::endl;
}
