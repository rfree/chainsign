#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>
#include "ckeysstorage.h"
#include "cCmdInterp.hpp"

#define KEY_SIZE 4096

using namespace std;

// usage
// chainsign --daemon name_of_instance out_dir
// chainsign --verify-chain 1st_pub_key --move out_dir
int main(int argc, char* argv[])
{
	std::cout << "start main" << std::endl;
	
	if (std::string(argv[1]) == "--daemon")
	{
		cCmdInterp cmdInterp("fifo", argv[2]);
		cmdInterp.setOutDir(std::string(argv[3]));
		std::cout << "start loop" << std::endl;
		cmdInterp.cmdReadLoop();
	}
	
	else if (std::string(argv[1]) == "--verify-chain" )
	{
		cCmdInterp cmdInterp;
		cmdInterp.setOutDir(std::string(argv[4]));
		cmdInterp.verify(std::string(argv[2]));
	}
	
    std::cout << KEY_SIZE << std::endl;
	/*cKeysStorage keyStorage = cKeysStorage();
	keyStorage.RSAVerifyFile("test.txt.sig", "my_instance");*/
    //for (int i = 0; i < 1; ++i)
       // keyStorage.generate(std::to_string(i));

	//keyStorage.sign("test.txt", 0);
	//keyStorage.GenerateRSAKey(KEY_SIZE);
	//keyStorage.RSASignFile("test.txt", "test.sig", 0);

    return 0;
}

