#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>
#include "ckeysstorage.h"
#include "cCmdInterp.hpp"

#define KEY_SIZE 4096

using namespace std;

int main(int argc, char* argv[])
{
	cCmdInterp cmdInterp("fifo");
	cmdInterp.cmdReadLoop();

	
	if (argv[1] == "--daemon")
	{
		
	}
	//remove("tmp");
    std::cout << KEY_SIZE << std::endl;
	cKeysStorage keyStorage = cKeysStorage();
    //for (int i = 0; i < 1; ++i)
       // keyStorage.generate(std::to_string(i));

	//keyStorage.sign("test.txt", 0);
	//keyStorage.GenerateRSAKey(KEY_SIZE);
	//keyStorage.RSASignFile("test.txt", "test.sig", 0);
	keyStorage.RSAVerifyFile("test.sig");

    return 0;
}

