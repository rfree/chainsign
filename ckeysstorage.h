#ifndef CKEYSSTORAGE_H
#define CKEYSSTORAGE_H

#include <string>
#include <vector>
#include <map>

#include <crypto++/rsa.h>
#include <crypto++/osrng.h>

class cKeysStorage
{
public:
    cKeysStorage();
    //cryptopp
    void RSASignFile(const std::string& messageFilename, const std::string& signatureFilename);
    bool RSAVerifyFile(const std::string& fileName);
    void GenerateRSAKey(unsigned int keyLength, std::string fileName);
    unsigned int getCurrentKey() { return mCurrentKey; }
    void RemoveRSAKey();
private:   
    //cryptopp
    //std::vector <CryptoPP::RSA::PrivateKey> mPrvKeys;	// TODO map
    std::map <int, CryptoPP::RSA::PrivateKey> mPrvKeys;
    //CryptoPP::RSA::PublicKey mCurrentPublicKey;
    void savePubFile(unsigned int numberOfKey, const CryptoPP::RSA::PublicKey& pPubKey, std::string fileName);
    CryptoPP::RSA::PublicKey loadPubFile(unsigned int numberOfKey);
	
	unsigned int mCurrentKey;
};

#endif // CKEYSSTORAGE_H
