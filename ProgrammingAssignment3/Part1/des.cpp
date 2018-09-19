#include <cstdint>
#include "des.h"
#include "des_utils.h"
using namespace std;

//default constructor
DES::DES(uint64_t key)
{
    generateSubKeys(key);
}


//subkey function
void DES::generateSubKeys(uint64_t key)
{
    uint64_t firstPerm = 0;
    
    //perform first permutation from key schedule
    for(uint8_t i = 0; i < 56; i++)
    {
        firstPerm = firstPerm << 1;
        firstPerm = firstPerm | ((key >> (64 - PC1[i])) & 0x0000000000000001);
    }
    
    //split main key into two halves
    uint32_t k1 = (uint32_t) ((firstPerm >> 28) & 0x000000000fffffff);
    uint32_t k2 = (uint32_t) (firstPerm & 0x000000000fffffff);
    
    //calculate all 16 subkeys
    for(uint8_t i = 0; i < 16; i++)
    {
        //shifting
        for(uint8_t j = 0; j < shift[i]; j++)
        {
            k1 = ((k1 << 1) & 0x0fffffff) | ((k1 >> 27) & 0x00000001);
            k2 = ((k2 << 1) & 0x0fffffff) | ((k2 >> 27) & 0x00000001);
        }
        
        uint64_t secondPerm = (((uint64_t) k1 ) << 28) | (uint64_t) k2;
        
        //perform second permutation from key schedule
        subKey[i] = 0;
        for(uint8_t j = 0; j < 48; j++)
        {
            subKey[i] = subKey[i] << 1;
            subKey[i] = subKey[i] | ((secondPerm >> (56 - PC2[j])) & 0x0000000000000001);
        }
    }
}


    //PERMUTATION FUNCTIONS
    
//first permutation function
uint64_t DES::initialPerm(uint64_t block)
{
    uint64_t perm = 0;
    for(uint8_t i = 0; i < 64; i++)
    {
        perm = perm << 1;
        perm = perm | ((block >> (64 - IP[i])) & 0x0000000000000001);
    }
    return perm;
}

//last permutation function/inverse of initial permutation
uint64_t DES::finalPerm(uint64_t block)
{
    uint64_t perm = 0;
    for(uint8_t i = 0; i < 64; i++)
    {
        perm = perm << 1;
        perm = perm | ((block >> (64 - FP[i])) & 0x0000000000000001);
    }
    return perm; 
}


    //CENTRAL FUNCTION FUNCTIONS
    
//Feistel cipher
void DES::switchHalves(uint32_t &left, uint32_t &right, uint32_t F)
{
    uint32_t temp = right;
    right = left ^ F;
    left = temp;
}

//perform expansion on right half using key schedule
uint64_t DES::expand(uint32_t right)
{
    uint64_t e = 0;
    for(uint8_t i = 0; i < 48; i++)
    {
        e = e << 1;
        e = e | ((uint64_t) ((right >> (32 - E[i])) & 0x00000001));
    }
    
    return e;
}

//apply core round function using permutation box in key schedule
uint32_t DES::round(uint32_t s)
{
    uint32_t r = 0;
    for(uint8_t i = 0; i < 32; i++)
    {
        r = r << 1;
        r = r | ((s >> (32 - PBOX[i])) & 0x00000001);
    }
    
    return r;
}

//main function that handles substitution
uint32_t DES::function(uint32_t right, uint64_t key)
{
    //apply expansion box
    uint64_t s1 = expand(right);
    
    //XOR with subkey (Ri XOR Ki)
    s1 = s1 ^ key;
    
    //perform 8 rounds of substitution using substitution boxes
    uint32_t s2 = 0;
    for(uint8_t i = 0; i < 8; i++)
    {
        //calculate index for substitution box
        char row = (char) ((s1 & (0x0000840000000000 >> 6 * i)) >> (42 - 6 * i));
        row = (row >> 4) | (row & 0x01);
        
        char column = (char) ((s1 & (0x0000780000000000 >> 6 * i)) >> (43 - 6 * i));
        
        //apply substitution box
        s2 = s2 << 4;
        s2 = s2 | ((uint32_t) (stringMap[SBOXMAP[i]][16 * row + column] & 0x0f));
    }
    
    //apply round function
    uint32_t f = round(s2);
    
    return f;
}


    //CIPHER FUNCTIONS
    
//encryption function
uint64_t DES::encrypt(uint64_t block)
{
    //perform initial permutation
    block = initialPerm(block);
    
    //split block into two halves
    uint32_t left = (uint32_t) (block >> 32) & 0x00000000ffffffff;
    uint32_t right = (uint32_t) (block & 0x00000000ffffffff);
    
    //perform 16 rounds of DES
    for(uint8_t i = 0; i < 16; i++)
    {
        uint32_t F = function(right, subKey[i]);
        switchHalves(left, right, F);
    }
    
    //reassemble halves
    block = (((uint64_t) right) << 32) | (uint64_t) left;
    
    //perform final permutation
    return finalPerm(block);
}

//decryption function
uint64_t DES::decrypt(uint64_t block)
{
    //perform initial permutation
    block = initialPerm(block);
    
    //split block into two halves
    uint32_t left = (uint32_t) (block >> 32) & 0x00000000ffffffff;
    uint32_t right = (uint32_t) (block & 0x00000000ffffffff);
    
    //perform 16 rounds of DES
    for(uint8_t i = 0; i < 16; i++)
    {
        uint32_t F = function(right, subKey[15 - i]);
        switchHalves(left, right, F);
    }
    
    //reassemble halves
    block = (((uint64_t) right) << 32) | (uint64_t) left;
    
    //perform final permutation
    return finalPerm(block);
}
