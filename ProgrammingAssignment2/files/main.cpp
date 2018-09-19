#include "file.h"
#include <iostream>
#include <random>
#include <string>
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
    string temp;
    uint64_t key;
    uint64_t IV;
    bool setK = false;
    bool setIV = false;
    bool setIn = false;
    bool setOut = false;
    
    for(int i = 0; i < argc; i++)
    {
        temp = argv[i];
        if(temp == "-K")
        {
            temp = argv[i + 1];
            key = strtoull(temp.c_str(), nullptr, 16);
            setK = true;
        }
        else if(temp == "-IV")
        {
            temp = argv[i + 1];
            IV = strtoull(temp.c_str(), nullptr, 16);
            setIV = true;
        }
        else if(temp == "-i")
        {
            temp = argv[i + 1];
            inputFile = temp;
            setIn = true;
        }
        else if(temp == "-o")
        {
            temp = argv[i + 1];
            outputFile = temp;
            setOut = true;
        }
    }
    
    if(!setK)
    {
        key = random_64bit();
    }
    if(!setIV)
    {
        IV = random_64bit();
    }
    if(!setIn)
    {
        cout << "Error: No input file given." << endl;
        return 0;
    }
    if(!setOut)
    {
        cout << "Error: No output file given." << endl;
        return 0;
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
        