#include "ckeysstorage.h"

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include <crypto++/rsa.h>
#include <crypto++/osrng.h>
#include <crypto++/base64.h>
#include <crypto++/files.h>
#include <crypto++/hex.h>


cKeysStorage::cKeysStorage()
{
}

// http://www.codepool.biz/tech-frontier/how-to-use-openssl-generate-rsa-keys-cc.html
bool cKeysStorage::generate(std::string pFileName)
{
    int             ret = 0;
    RSA             *rsa = NULL;
    BIGNUM          *bne = NULL;
    BIO             *bp_public = NULL, *bp_private = NULL;

    int             bits = 4096;
    unsigned long   e = RSA_F4;

    // 1. generate rsa key
    bne = BN_new();
    ret = BN_set_word(bne, e);

    rsa = RSA_new();
    ret = RSA_generate_key_ex(rsa, bits, bne, NULL);

    // 2. save public key
    std::string mFileName = pFileName;
    mFileName += "_pub.pem";
    bp_public = BIO_new_file(mFileName.c_str(), "w+");
    ret = PEM_write_bio_RSAPublicKey(bp_public, rsa);

    // 3. save private key
    mFileName = pFileName;
    mFileName += "_prv.pem";
    bp_private = BIO_new_file(mFileName.c_str(), "w+");
    ret = PEM_write_bio_RSAPrivateKey(bp_private, rsa, NULL, NULL, 0, NULL, NULL);

    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
    RSA_free(rsa);
    BN_free(bne);

    return (ret == 1);
}

bool cKeysStorage::sign(std::string pFileName, unsigned int pNumberOfKey)
{
 /*   std::ifstream mInputFile;
    std::stringstream ss;
    ss << pNumberOfKey;
    ss << "_prv.pem";
    RSA *rsa = NULL;//createRSAWithFilename(ss.str().c_str(), true); // XXX
    if (rsa == NULL)
    {
		std::cout << "rsa create error" << std::endl;
        return false;
	}

    mInputFile.open(pFileName, std::ios::in | std::ios::binary);
    if (!mInputFile.is_open())
    {
		std::cout << "file open error" << std::endl;
        return false;
	}

    // get length of file:
    mInputFile.seekg (0, mInputFile.end);
    int mLength = mInputFile.tellg();
    mInputFile.seekg (0, mInputFile.beg);

    std::shared_ptr<void> mBuffer (new char[mLength]);
    std::shared_ptr<void> mOutBuffer (new char[mLength]);
    mInputFile.read(std::static_pointer_cast<char>(mBuffer).get(), mLength);
    mInputFile.close();

    RSA_private_encrypt(mLength,
                        std::static_pointer_cast<unsigned char>(mBuffer).get(),
                        std::static_pointer_cast<unsigned char>(mOutBuffer).get(),
                        rsa,
                        RSA_NO_PADDING);
    RSA_free(rsa);
    std::ofstream mOutFile;
    std::string mOutName(pFileName);
    mOutName += ".pub";
    mOutFile.open(mOutName);
    //save header
    mOutFile << "version 1" << std::endl;
    mOutFile << "crypto rsa" << std::endl;
    mOutFile << "size 4096" << std::endl;
    mOutFile << "END" << std::endl;
    mOutFile.close();
    //save data
    mOutFile.open(mOutName, std::ios::out | std::ios::app | std::ios::binary);
    mOutFile.write(std::static_pointer_cast<char>(mOutBuffer).get(), mLength);
    mOutFile.close();*/
    return true;
}

bool cKeysStorage::verify(std::string pFileName, unsigned int pNumberOfKey)
{

    return true;
}


RSA* cKeysStorage::createRSAWithFilename(const char * filename, bool pub)
{
	std::cout << "open key " << filename << std::endl;
    FILE * fp = fopen(filename,"rb");

    if(fp == NULL)
    {
		std::cout << "open key error" << std::endl;
        return NULL;
    }
    RSA *rsa= RSA_new();
    if (rsa != NULL)
		std::cout << "rsa address " << rsa << std::endl;

    if(pub)
    {
		std::cout << "rsa open pub key" << std::endl;
        rsa = PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
    }
    else
    {
		std::cout << "rsa open prv key" << std::endl;
        rsa = PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
    }

    return rsa;
}







/*void cKeysStorage::Sign()
{
	using namespace CryptoPP;
   std::string strContents = "A message to be signed";
   //FileSource("tobesigned.dat", true, new StringSink(strContents));
   
   AutoSeededRandomPool rng;
   
   //Read private key
   CryptoPP::ByteQueue bytes;
   FileSource file("privkey.txt", true, new Base64Decoder);
   file.TransferTo(bytes);
   bytes.MessageEnd();
   RSA::PrivateKey privateKey;
   privateKey.Load(bytes);
   
   //Sign message
   RSASSA_PKCS1v15_SHA_Signer privkey(privateKey);
   SecByteBlock sbbSignature(privkey.SignatureLength());
   privkey.SignMessage(
       rng,
       (byte const*) strContents.data(),
       strContents.size(),
       sbbSignature);
   
   //Save result
   FileSink sink("signed.dat");
   sink.Put((byte const*) strContents.data(), strContents.size());
   FileSink sinksig("sig.dat");
   sinksig.Put(sbbSignature, sbbSignature.size());
}*/

void cKeysStorage::GenerateRSAKey(unsigned int keyLength, const char *pubFilename)
{
	using namespace CryptoPP;
	/*char seed[1024];
	for (int i = 0; i < 1024; i++)
		seed[i] = rand() % 256; // TODO better random generator
	RandomPool randPool;	
	randPool.IncorporateEntropy((byte *)seed, strlen(seed));
	
	RSAES_OAEP_SHA_Decryptor priv(randPool, keyLength);
	//HexEncoder privFile(new FileSink(privFilename));
	//priv.DEREncode(privFile);
	//privFile.MessageEnd();

	RSAES_OAEP_SHA_Encryptor pub(priv);
	HexEncoder pubFile(new FileSink(pubFilename));
	pub.DEREncode(pubFile);
	pubFile.MessageEnd();*/
	
	// Generate Parameters
	AutoSeededRandomPool rng;
	InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(rng, 4096);
	
	// Create Keys
	CryptoPP::RSA::PrivateKey privateKey(params);
	CryptoPP::RSA::PublicKey publicKey(params);
}


void cKeysStorage::RSASignFile(const char *privFilename, const char *messageFilename, const char *signatureFilename)
{
	/*using namespace CryptoPP;
	FileSource privFile(privFilename, true, new HexDecoder);
	RSASS<PKCS1v15, SHA>::Signer priv(privFile);
	FileSource f(messageFilename, true, new SignerFilter(GlobalRNG(), priv, new HexEncoder(new FileSink(signatureFilename))));*/
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
