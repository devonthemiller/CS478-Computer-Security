#include "file.h"
#include <iostream>
#include <random>
using namespace std;

//function for calculating a random number
//that is exactly 64 bits long
uint64_t random_64bit()
{
    random_device rd;
    
    mt19937_64 generator(rd());
    
    uniform_int_distribution<long long int> dist( llround(pow(2,61)), llround(pow(2,62)) );
    
    return dist(generator);
}

//main function
int main(int argc, char **argv)
{
    string choice = argv[1];
    string inputFile, outputFile;
    uint64_t key;
    uint64_t IV;
    
    //in the case that key and iv values are given
    if(argc > 4)
    {
        inputFile = argv[4];
        outputFile = argv[5];
        key = strtoull(argv[2], nullptr, 16);
        IV = strtoull(argv[3], nullptr, 16);
    }
    //in the case that no key or iv values are given
    else
    {
        inputFile = argv[2];
        outputFile = argv[3];
        key = random_64bit();
        IV = random_64bit();
    }
    
    //outputs key and iv for the user
    cout << "Key: ";
    printf("%lx",key);
    cout << endl;
    cout << "IV: ";
    printf("%lx",IV);
    cout << endl;
    
    //creates an object for reading and writing bytes
    file f(key, IV);
    
    //for encryption
    if(choice == "-e")
        return f.encrypt(inputFile, outputFile);
    //for decryption
    if(choice == "-d")
        return f.decrypt(inputFile, outputFile);
    
    return 0;
}
        