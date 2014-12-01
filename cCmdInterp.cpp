#include "cCmdInterp.hpp"
#include <iostream>
#include <list>
#include <sstream>
#include <boost/filesystem.hpp>

#define KEY_SIZE 4096

cCmdInterp::cCmdInterp(std::string pFifoName, std::string pInstance)
:inst(pInstance)
{
	inputFIFO.open(pFifoName);
}

void cCmdInterp::cmdReadLoop()
{
	std::string line;
	keyStorage.GenerateRSAKey(KEY_SIZE, mOutDir + inst + "-key" + std::to_string(keyStorage.getCurrentKey()) + ".pub"); // generate 1st key
	while (1)
	{
		//std::cout << "loop" << std::endl;
		inputFIFO.open("fifo");
		std::getline(inputFIFO, line);
		//std::cout << "line " << line << std::endl;
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
			keyStorage.RSASignFile(line, mOutDir + inst + "-" + line + ".sig");
			std::cout << "generate new key" << std::endl;
			keyStorage.GenerateRSAKey(KEY_SIZE, mOutDir + pubFileName);
			std::cout << "rm old key" << std::endl;
			keyStorage.RemoveRSAKey();
			keyStorage.RSASignFile(pubFileName, mOutDir + pubFileName + ".sig");	// sign key
		}
		else if(line == "VERIFY-FILE")
		{
			bool ok;
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
			std::string tmp;
			unsigned int key; // file was signed this key
			std::ifstream inFile(line);
			inFile >> tmp;
			inFile >> key;
			inFile.close();
			if (key < verify(std::string(instance + "-key1.pub.sig")))
			{
				std::cout << "                                               Keys OK" << std::endl;
				ok = true;
			}
			else
			{
				ok = false;
				std::cout << "                                               Keys verification error" << std::endl;
			}
			
			bool fileOK = keyStorage.RSAVerifyFile(line, instance);
			inputFIFO.close();
			
			std::cout << "keys " << ok << std::endl;
			std::cout << "file " << fileOK << std::endl;
			if (ok && fileOK)
				std::cout << "File OK" << std::endl;
		}
		else if(line == "SIGN-NEXTKEY-WAV-FILES")
		{
			std::cout << "SIGN-NEXTKEY-WAV-FILES" << std::endl;
			std::list<std::string> wavFiles;
			boost::filesystem::directory_iterator dirIterator(".");
			boost::filesystem::directory_iterator endIterator;
			//generate new key
			std::string pubFileName = inst + "-key" + std::to_string(keyStorage.getCurrentKey()) + ".pub";
			system(std::string("touch " + pubFileName).c_str());
			std::cout << "pubFileName " << pubFileName << std::endl;
			while (dirIterator != endIterator)
			{
				if (boost::filesystem::is_regular_file(dirIterator->status()))
				{
					std::stringstream ss;
					std::string fileName;
					ss << *dirIterator;
					ss >> fileName;
					fileName.erase(fileName.begin(), fileName.begin() + 3);
					fileName.pop_back();
					if (fileName.find("wav") == (fileName.size() - 3))
					{
						wavFiles.push_back(fileName);
						std::cout << fileName << std::endl;
						// sign
						keyStorage.RSASignFile(fileName, mOutDir + inst + "-" + fileName + ".sig");
					}
				}
				
				dirIterator++;
			}
			std::cout << "generate new key" << std::endl;
			keyStorage.GenerateRSAKey(KEY_SIZE, mOutDir + pubFileName);
			std::cout << "rm old key" << std::endl;
			keyStorage.RemoveRSAKey();
			keyStorage.RSASignFile(pubFileName, mOutDir + pubFileName + ".sig");	// sign key
		}
			
	}
	//std::cout << "loop end" << std::endl;
}


unsigned int cCmdInterp::verify(std::string firstKey) // verify keys
{
	//std::ifstream pubFile;
	system(std::string("cp " + firstKey + " " + mOutDir + firstKey).c_str());
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
	unsigned int lastGoodKey = 1;
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
		{
			lastGoodKey = keyNumber;
			//std::cout << "mv cmd " << "mv " + fileName + " " + mOutDir + fileName << std::endl;
			fileName.erase(fileName.end() - 4, fileName.end()); // rm ".sig"
			system(std::string("cp " + fileName + " " + mOutDir + fileName).c_str());
		}
		
		keyNumber++;
	}
	
	std::cout << "Last good key: " << lastGoodKey << std::endl;
	if (lastGoodKey > 1)
	{
		std::cout << "OK" << std::endl;
		return lastGoodKey;
	}
	else
		return -1;
}

void cCmdInterp::setOutDir(std::string outDir)
{
	mOutDir = outDir;
	system(std::string("mkdir " + mOutDir).c_str());
	if (outDir.back() != '/')
		mOutDir.push_back('/');
	//std::cout << "out dir: " << mOutDir << std::endl;
}

unsigned int cCmdInterp::verifyOneFile(std::string fileName) //fileName = sig file
{
	std::string instance;
	auto it = fileName.begin();
	while (*it != '-')
	{
		instance.push_back(*it);
		it++;
	}
	
	std::string firstPubKey;
	firstPubKey = instance + "-key1.pub";
	
	std::cout << "Start keys verification" << std::endl;
	unsigned int ret = verify(std::string(fileName));
	if (ret == -1)
		return 2;
	
	std::cout << "file name " << fileName << std::endl;
	ret = keyStorage.RSAVerifyFile(fileName, instance);
	//std::cout << ret << std::endl;
	if (ret == 0)
		return 3;
	
	std::cout << "OK" << std::endl;	
	return 0;
}
