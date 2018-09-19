#include "file.h"

//default constructor
file::file(uint64_t key, uint64_t IV) : des(key, IV)
{
}

//function for encrypting data given by inputFile
int file::encrypt(string inputFile, string outputFile)
{
    return operation(inputFile, outputFile, false);
}

//function for decrypting data given by inputFile
int file::decrypt(string inputFile, string outputFile)
{
    return operation(inputFile, outputFile, true);
}

//main function that reads, encrypts/decrypts, and then writes data
int file::operation(string inputFile, string outputFile, bool mode)
{
    ifstream readFile;
    ofstream writeFile;
    uint64_t buff;
    
    readFile.open(inputFile, ios::binary | ios::in | ios::ate);
    writeFile.open(outputFile, ios::binary | ios::out);
    
    uint64_t size = readFile.tellg();
    readFile.seekg(0, ios::beg);
    
    uint64_t block = size / 8;
    if(mode) block --;
    
    for(uint64_t i = 0; i < block; i++)
    {
        readFile.read((char*) &buff, 8);
        
        if(mode)
            buff = des.CBCdecrypt(buff); //decrypt
        else
            buff = des.CBCencrypt(buff); //encrypt
        
        writeFile.write((char*) &buff, 8);
    }
    
    //if encrypting then add necessary padding to message
    if(mode == false)
    {
        uint8_t pad = 8 - (size % 8);
        
        if(pad == 0)
            pad = 8;
        
        buff = (uint64_t) 0;
        if(pad != 8)
            readFile.read((char*) &buff, 8 - pad);
        
        uint8_t shift = pad * 8;
        buff = buff << shift;
        buff = buff | ((uint64_t) 0x0000000000000001 << (shift - 1));
        
        buff = des.CBCencrypt(buff);
        writeFile.write((char*) &buff, 8);
    }
    
    //if decrypting then ignore padding and decrypt final line
    else
    {
        readFile.read((char*) &buff, 8);
        buff = des.CBCdecrypt(buff);
        
        uint8_t pad = 0;
        
        while(!(buff & 0x00000000000000ff))
        {
            buff = buff >> 8;
            pad++;
        }
        
        buff = buff >> 8;
        pad++;
        
        if(pad != 8)
            writeFile.write((char*) &buff, 8 - pad);
    }
    
    readFile.close();
    writeFile.close();
    return 0;
}
