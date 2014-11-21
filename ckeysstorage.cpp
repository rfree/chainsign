#include "ckeysstorage.h"

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <vector>

#include <crypto++/rsa.h>
#include <crypto++/osrng.h>
#include <crypto++/base64.h>
#include <crypto++/files.h>
#include <crypto++/hex.h>

using namespace CryptoPP;

cKeysStorage::cKeysStorage()
{
}


void cKeysStorage::GenerateRSAKey(unsigned int keyLength)
{
	using namespace CryptoPP;
	AutoSeededRandomPool rng;
	// Generate Parameters
	InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(rng, keyLength);
	
	// Create Keys
	CryptoPP::RSA::PrivateKey privateKey(params);
	CryptoPP::RSA::PublicKey publicKey(params);
	
	mPrvKeys.push_back(privateKey);
	savePubFile(mPrvKeys.size() - 1, publicKey); // 0, 1, 2 ...
	
	std::cout << "end of GenerateRSAKey" << std::endl;
	std::cin.get();
}

void cKeysStorage::RSAVerifyFile(const std::string& fileName) // load .sig file
{
	using namespace CryptoPP;
	std::string line;
	std::string clearTextFileName;
	int pubicKeyNumber;
	// read public key
	std::ifstream input(fileName);
	input >> line;
	//parse data
	input >> pubicKeyNumber;
	std::cout << line << " " << pubicKeyNumber << std::endl;
	for (int i = 0; i < 4; ++i)	// 3 lines
	{
		input >> line;
		std::cout << line;
		input >> line;
		std::cout << line << std::endl;
	}
	
	clearTextFileName = line;
	// load rsa data
	char byte;
	std::string signature;
	while (!input.eof())
	{
		input.read(&byte, 1);
		signature.push_back(byte);
	}
	
	signature.pop_back(); // rm 2 last chars
	signature.pop_back();
	
	std::cout << std::endl << "signature" << signature << std::endl;
		
	CryptoPP::RSA::PublicKey currentPubKey = loadPubFile(pubicKeyNumber);
	AutoSeededRandomPool rng;
	std::cout << "pub key validate " << currentPubKey.Validate(rng, 1);
	std::cout << std::endl << "start verify" << std::endl;
	RSASSA_PKCS1v15_SHA_Verifier verifier(currentPubKey);

	/*try
	{
		StringSource(signature.data(), signature.size(), true, 
		new SignatureVerificationFilter(verifier, NULL, SignatureVerificationFilter::THROW_EXCEPTION) );
		std::cout << "Signature OK" << std::endl;
	}
	catch(SignatureVerificationFilter::SignatureVerificationFailed &err)
	{
		std::cout << "verify error " << err.what() << std::endl;
	}*/
	std::cout << "end of RsaVerifyFile" << std::endl;
	std::cin.get();
}

void cKeysStorage::savePubFile(unsigned int numberOfKey, const CryptoPP::RSA::PublicKey& pPubKey)
{
	std::ofstream mOutFile;
    std::string mOutName(std::to_string(numberOfKey));
    mOutName += ".pub";
    mOutFile.open(mOutName);
    //save header
    mOutFile << "version 1" << std::endl;
    mOutFile << "crypto rsa" << std::endl;
    mOutFile << "size 4096" << std::endl;
    mOutFile << "END" << std::endl;
    
    //generate pub key in txt file
    Base64Encoder pubkeysink(new FileSink("tmp"));
	pPubKey.DEREncode(pubkeysink);
	pubkeysink.MessageEnd();
	
	//append from tmp to pub file
	std::ifstream tmpFile("tmp");
	char s;
	while (!tmpFile.eof())
	{
		tmpFile >> std::noskipws >> s;
		mOutFile << s;
	}
	
    //mOutFile.write(reinterpret_cast<const char*>(&pPubKey), sizeof(pPubKey));
    mOutFile.close();
    
    /*std::cout << "savePubFile" << std::endl;
    std::cout << "saved data" << std::endl;
    std::cout.write(reinterpret_cast<const char*>(&pPubKey), sizeof(pPubKey));
    std::cout << std::endl << "end of saved data" << std::endl;
    
    std::cout << "size of pub key: " << sizeof(pPubKey) << std::endl;*/
    
    std::cout << "end of savePubFile" << std::endl;
	std::cin.get();
}

CryptoPP::RSA::PublicKey cKeysStorage::loadPubFile(unsigned int numberOfKey)
{
	std::string fileName(std::to_string(numberOfKey));
	fileName += ".pub";
	std::cout << "Public key file: " << fileName << std::endl;
	std::string line;
	std::ifstream input(fileName);
	for (int i = 0; i < 3; i++)
	{
		input >> line;
		std::cout << line << " ";
		input >> line;
		std::cout << line << std::endl;
	}
	input >> line; // END
	
	// load rsa data
	char byte;
	
	//from .pub to tmp
	std::ofstream tmpFile("tmp", std::ios::trunc);
	while (!input.eof())
	{
		input >> std::noskipws >> byte;
		tmpFile << byte;
	}
	
	tmpFile.close();
	
	//Read public key
	CryptoPP::ByteQueue bytes;
	FileSource file("tmp", true, new Base64Decoder);
	file.TransferTo(bytes);
	bytes.MessageEnd();
	RSA::PublicKey pubKey;
	pubKey.Load(bytes);
	
	std::cout << "end of loadPubFile" << std::endl;
	
	std::cin.get();
	return pubKey;
}

//https://gist.github.com/TimSC/5251670
void cKeysStorage::RSASignFile(const std::string& messageFilename, const std::string& signatureFilename, unsigned int numberOfKey)
{
	AutoSeededRandomPool rng;
	std::ifstream mInputFile(messageFilename, std::ios::binary | std::ios::in);
	// get length of file:
    mInputFile.seekg (0, mInputFile.end);
    int mLength = mInputFile.tellg();
    mInputFile.seekg (0, mInputFile.beg);
    std::cout << "input file size: " << mLength << std::endl;
    
    std::shared_ptr<char> mBuffer (new char[mLength], [](char* p){delete []p;});
    mInputFile.read(mBuffer.get(), mLength);
    mInputFile.close();
    
    CryptoPP::RSA::PrivateKey privateKey = mPrvKeys.at(numberOfKey);
	RSASSA_PKCS1v15_SHA_Signer privkey(privateKey);
	SecByteBlock sbbSignature(privkey.SignatureLength());
	privkey.SignMessage(
		rng,
		(byte const*) mBuffer.get(),
		mLength,
		sbbSignature);
 
	std::ofstream output(signatureFilename);
	output << "id-nr " << numberOfKey << std::endl;
	output << "key-ver 1" << std::endl;
	output << "key-crypto rsa" << std::endl;
	output << "key-size 4096" << std::endl;
	output << "cleartext-file "<< messageFilename << std::endl;
	output << "END" << std::endl;
	
	//Save result
	FileSink sinksig("tmp");
	sinksig.Put((byte const*)sbbSignature, sbbSignature.size());
	
	std::ifstream tmpFile("tmp");
	char s;
	while (!tmpFile.eof())
	{
		tmpFile >> std::noskipws >> s;
		output << s;
	}
	
	output.close();
	
	std::cout << "end of RSASignFile" << std::endl;
	std::cin.get();
}


/*
.pub format:
id-instance inst20141120_1242
id-nr 1
key-ver 1
key-crypto rsa
key-size 4096
END
<after this line "END" we paste here the RSA data: of the public key>
   .sig format:
id-instance inst20141120_1242
id-nr 1
key-ver 1
key-crypto rsa
key-size 4096
cleartext-file foo1.log
END
<after this line "END" we paste here the RSA data: of signature>
*/
