#ifndef CONFIG_H_
#define CONFIG_H_
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include "TObjArray.h"
#include "TString.h"

using namespace std;

class Config
{
    public:
        Config(const char* filename);
        ~Config();
        TObjArray* GetProperties(const char* label);
        TString GetString(const char* label);
        Float_t GetFloat(const char* label);
        vector< vector<float> > GetSectorParms(const char* label);
    protected:
        ifstream propfile;
        multimap<std::string,std::string> propsmap;
};
#endif //CONFIG_H_