#include "Config.h"

#include "TObjString.h"
#include "TString.h"

using namespace std;

Config::Config(const char* filename)
{
    propfile.open(filename);
    string line, key, value;
    if (propfile.is_open()) {
      while (propfile >> key && propfile >> value) {
        propsmap.insert(pair<string,string>(key,value));
        cout << key << ":" << value << endl;
      }
    }
    propfile.close();
}

Config::~Config()
{
}

TObjArray* Config::GetProperties(const char* label, bool sectorset)
{
    pair <multimap<string,string>::iterator,multimap<string,string>::iterator> its;
    its = propsmap.equal_range((string)label);
    TObjArray *propvals = new TObjArray();
    for (multimap<string,string>::iterator it = its.first; it != its.second; ++it) {
        TString tmp(it->second.data());
        propvals->Add(tmp.Tokenize(","));
    }
    return propvals;
}

Float_t Config::GetFloat(const char* label)
{
  Float_t retval = 0.0;
  TObjArray *t = GetProperties(label);
  if (t->GetEntries() == 1) {
    TObjArray *t2 = (TObjArray*)t->At(0);
    if (t2->GetEntries() == 1) {
      TObjString *tos = (TObjString*) t2->At(0);
      TString valstr = tos->GetString();
      retval = tos->GetString().Atof();
    }
  }
  delete t;
  return retval;
}