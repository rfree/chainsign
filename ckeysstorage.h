#ifndef CKEYSSTORAGE_H
#define CKEYSSTORAGE_H

#include <string>
#include <vector>

#include <crypto++/rsa.h>
#include <crypto++/osrng.h>

class cKeysStorage
{
public:
    cKeysStorage();
    //cryptopp
    void RSASignFile(const std::string& messageFilename, const std::string& signatureFilename, unsigned int numberOfKey);
    void RSAVerifyFile(const std::string& fileName);
    void GenerateRSAKey(unsigned int keyLength);
private:
    
    //cryptopp
    std::vector <CryptoPP::RSA::PrivateKey> mPrvKeys;	// TODO map
    //CryptoPP::RSA::PublicKey mCurrentPublicKey;
    void savePubFile(unsigned int numberOfKey, const CryptoPP::RSA::PublicKey& pPubKey);
    CryptoPP::RSA::PublicKey loadPubFile(unsigned int numberOfKey);
	
	//CryptoPP::AutoSeededRandomPool rng;
    
};

#endif // CKEYSSTORAGE_H
