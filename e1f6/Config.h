#include <iostream>
#include <map>
#include <fstream>
#include "TObjArray.h"

class Config
{
public:
    Config(const char* filename);
    ~Config();
    TObjArray* GetProperties(const char* label);
    Float_t GetFloat(const char* label);
    vector< vector<float> > GetSectorParms(const char* label);
protected:
    ifstream propfile;
    multimap<std::string,std::string> propsmap;
};