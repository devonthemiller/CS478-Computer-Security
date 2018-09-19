#include "des.h"
using namespace std;

//DES_CBC class instantiates a DES_CBC object
//which contains a DES object. This modularity
//allows for easier separation between higher-level
//CBC computations and DES computations.
class DES_CBC
{
    private:
        DES des;
        uint64_t IV;
        uint64_t endBlock;
        
    public:
        DES_CBC(uint64_t key, uint64_t IV);
        
        uint64_t CBCencrypt(uint64_t block);
        uint64_t CBCdecrypt(uint64_t block);
        
        void resetCBC();
};
