#include "hashing.h"
#include "object.h"

#include <algorithm> //For std::reverse
#include <iomanip>
#include <iostream>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <string>

std::string convertRestToHexa(int val) {

    std::string matchValue[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};
    if (val >= 16 || val < 0) {
        std::cerr << "Error: The format of the value passed as an argument is invalid in function convertRestToHexa" << std::endl;
        exit(0);
    }
    else {
        return matchValue[val];
    }
}

std::string convertToHexa(int valToConvert, int length) {

    std::string valInHexa = "";
    for(int i = 0; i < length; i++) {
        int rest = valToConvert % 16;
        valInHexa += convertRestToHexa(rest);
        valToConvert /= 16;
    }
    std::reverse(valInHexa.begin(), valInHexa.end());
    return valInHexa;

}

std::string hashString(std::string contentToHash) {
    
    //Méthode obsolète

    /*unsigned char hashedContent[SHA256_DIGEST_LENGTH];

    SHA256_CTX context;
    SHA256_Init(&context);
    
    SHA256_Update(&context, contentToHash.c_str(), contentToHash.length());
    SHA256_Final(hashedContent, &context);

    std::string readableHash = "";
    for(size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        readableHash += convertToHexa(int(hashedContent[i]), 2);
    }
    return readableHash; */

    std::vector<unsigned char> hashedContent(EVP_MAX_MD_SIZE);
    unsigned int hash_len = 0;
    

    EVP_MD_CTX* context = EVP_MD_CTX_new();

    if(context == nullptr) {
        std::cerr << "Error : Failed to create message digest context in hashString" << std::endl;
        return "";
    }
    if(EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1) {
        std::cerr << "Error : Failed to initialize digest context in hashString" << std::endl;
        EVP_MD_CTX_free(context);
        return "";
    }
    if(EVP_DigestUpdate(context, contentToHash.c_str(), contentToHash.length()) != 1) {
        std::cerr << "Error : Failed to update digest in hashString in hashString" << std::endl;
        EVP_MD_CTX_free(context);
        return "";
    }
    if(EVP_DigestFinal_ex(context, hashedContent.data(), &hash_len) != 1) {
        std::cerr << "Error : Failed to finalize digest in hashString" << std::endl;
        EVP_MD_CTX_free(context);
        return "";
    }
    EVP_MD_CTX_free(context);

    hashedContent.resize(hash_len);
    std::string readableHash = "";
    for(size_t i = 0; i < hash_len; i++) {
        readableHash += convertToHexa((int)hashedContent[i], 2);
    }
    return readableHash;
}