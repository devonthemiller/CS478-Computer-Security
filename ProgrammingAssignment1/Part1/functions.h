#include <string>
#include <vector>
using namespace std;

class cipher
{
    private:
        string ciphertext;
        string frequency;
        vector<int> freqNum;
        string key;
        string plaintext;
        
    public:
        cipher();
        
        void readCiphertext(const string & file);
        
        int compare(const string & file);
        
        void calculateFreq();
        
        void generateKey();
        
        void permuteKey();
        
        void decipher();
        
        void setKey(string k);
        
        string getKey();
        
        string getFrequency();
        
        string getCiphertext();
        
        string getPlaintext();
        
        vector<int> getFreqNum();
};
