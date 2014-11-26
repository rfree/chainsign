#include "cCmdInterp.hpp"
#include <iostream>

#define KEY_SIZE 4096

cCmdInterp::cCmdInterp(std::string pFifoName, std::string pInstance)
:inst(pInstance)
{
	inputFIFO.open(pFifoName);
}

void cCmdInterp::cmdReadLoop()
{
	std::string line;
	keyStorage.GenerateRSAKey(KEY_SIZE, inst + "-key" + std::to_string(keyStorage.getCurrentKey()) + ".pub"); // generate 1st key
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
			std::cout << std::endl;
			std::string pubFileName = inst + "-key" + std::to_string(keyStorage.getCurrentKey()) + ".pub";
			system(std::string("touch " + pubFileName).c_str());
			std::cout << "pubFileName " << pubFileName << std::endl;
			inputFIFO.open("fifo");
			std::getline(inputFIFO, line);
			std::cout << "sign file " << line << std::endl;
			keyStorage.RSASignFile(line, inst + "-" + line + ".sig");
			std::cout << "generate new key" << std::endl;
			keyStorage.GenerateRSAKey(KEY_SIZE, pubFileName);
			std::cout << "rm old key" << std::endl;
			keyStorage.RemoveRSAKey();
			keyStorage.RSASignFile(pubFileName, pubFileName + ".sig");	// sign key
		}
		else if(line == "VERIFY-FILE")
		{
			std::cout << "VERIFY-FILE" << std::endl;
			std::cout << std::endl;
			inputFIFO.close();
			inputFIFO.open("fifo");
			std::getline(inputFIFO, line);
			std::string instance;
			auto it = line.begin();
			while (*it != '-')
			{
				instance.push_back(*it);
				it++;
			}
			std::cout << "line " << line << std::endl;
			std::cout << "instance " << instance << std::endl;
			keyStorage.RSAVerifyFile(line, instance);
			inputFIFO.close();
		}
			
	}
	std::cout << "loop end" << std::endl;
}


void cCmdInterp::verify(std::string firstKey) // verify keys
{
	//std::ifstream pubFile;
	std::string instance;
	std::string fileName = instance;
	bool good = true;
	int keyNumber = 2;
	while (firstKey.front() != '-')
	{
		instance += firstKey.front();
		firstKey.erase(firstKey.begin());
	}
	
	std::cout << "instance " << instance << std::endl;
	
	std::cout << "start loop" << std::endl; 
	unsigned int lastGoodKey = 0;
	while (good)
	{
		std::ifstream pubFile;
		fileName = instance + "-key" + std::to_string(keyNumber) + ".pub.sig";
		std::cout << "file name " << fileName << std::endl;
		pubFile.open(fileName);
		if(!pubFile.good()) // no file
		{
			std::cout << "No found " << fileName << std::endl;
			break;
		}
			
		good = keyStorage.RSAVerifyFile(fileName, instance);
		if (good)
			lastGoodKey = keyNumber;
		keyNumber++;
	}
	
	std::cout << "Last good key: " << lastGoodKey << std::endl;
 }
