#include "Config.h"

#include <vector>

#include "TObjString.h"
#include "TString.h"

using namespace std;

Config::Config(const char* filename)
{
    cout << "Loading configuration from " << filename << endl;
    propfile.open(filename);
    string commentchar = "#";
    string line, key, value;
    if (propfile.is_open())
    {
        while (propfile.good())
        {
            propfile >> key;
            if (key.compare(0, 1, commentchar) == 0) {
                //cout << "ignoring comment line" << endl;
                propfile.ignore(1024, '\n');
            } else {
                propfile >> value;
                propsmap.insert(pair<string,string>(key,value));
                cout << key << ":" << value << endl;
            }
        }
    }
    propfile.close();
}

Config::~Config()
{
}

TObjArray* Config::GetProperties(const char* label)
{
    pair <multimap<string,string>::iterator,multimap<string,string>::iterator> its;
    its = propsmap.equal_range((string)label);
    TObjArray *propvals = new TObjArray();
    for (multimap<string,string>::iterator it = its.first; it != its.second; ++it)
    {
        TString tmp(it->second.data());
        propvals->Add(tmp.Tokenize(","));
    }
    propvals->Compress();
    return propvals;
}

TString Config::GetString(const char* label)
{
    TString retval;
    TObjArray *t = GetProperties(label);
    if (t->GetEntries() == 1)
    {
        TObjArray *t2 = (TObjArray*)t->At(0);
        if (t2->GetEntries() == 1)
        {
            TObjString *tos = (TObjString*) t2->At(0);
            retval = tos->GetString();
        }
    }
    delete t;
    return retval;
}

Float_t Config::GetFloat(const char* label)
{
    Float_t retval = 0.0;
    TObjArray *t = GetProperties(label);
    if (t->GetEntries() == 1)
    {
        TObjArray *t2 = (TObjArray*)t->At(0);
        if (t2->GetEntries() == 1)
        {
            TObjString *tos = (TObjString*) t2->At(0);
            TString valstr = tos->GetString();
            retval = tos->GetString().Atof();
        }
    }
    delete t;
    return retval;
}


vector< vector<float> > Config::GetSectorParms(const char* label)
{
    vector< vector<float> > ret(6);
    pair <multimap<string,string>::iterator,multimap<string,string>::iterator> its;
    its = propsmap.equal_range((string)label);
    for (multimap<string,string>::iterator it = its.first; it != its.second; ++it)
    {
        TString tmp(it->second.data());
        TObjArray *toks = tmp.Tokenize(",");
        Int_t sector = ((TObjString*)toks->At(0))->GetString().Atoi();
        vector<float> parms;
        //iterate over tokens 1..N pushing back onto parms
        toks->RemoveAt(0);
        toks->Compress();
        TIter next(toks);
        while (TObjString *parmstr = (TObjString*)next())
        {
            parms.push_back(parmstr->GetString().Atof());
        }
        ret[sector-1] = parms;
    }
    return ret;
}
