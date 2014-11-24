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
	//std::getline(inputFIFO, line);
	while (1)
	{
		std::cout << "loop" << std::endl;
		inputFIFO.open("fifo");
		std::getline(inputFIFO, line);
		std::cout << "line " << line << std::endl;
		inputFIFO.close();
		if (line == "QUIT")
			break;
		else if(line == "SIGN-NEXTKEY")
		{
			std::cout << "SIGN-NEXTKEY" << std::endl;
			std::cout << "" << std::endl;
			std::string pubFileName =  inst + "-key" + std::to_string(keyStorage.getCurrentKey()) + ".pub";
			system(std::string("touch " + pubFileName).c_str());
			inputFIFO.close();
			inputFIFO.open("fifo");
			std::getline(inputFIFO, line);
			std::cout << "sign file " << line << std::endl;
			keyStorage.RSASignFile(line, line + ".sig");
			keyStorage.RSASignFile(pubFileName, pubFileName + ".sig");	// sign key
			std::cout << "generate new key" << std::endl;
			keyStorage.GenerateRSAKey(4096, pubFileName);
			std::cout << "rm old key" << std::endl;
			//keyStorage.RemoveRSAKey();
		}
			
	}
	std::cout << "loop end" << std::endl;
}
