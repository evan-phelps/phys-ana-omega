#ifndef DATA_H
#define DATA_H
#include "TTree.h"

class Data
{
public:
	bool is_sim;
	int run;
	int file_anum;
	std::string filename;
	Data()
	{
		is_sim = false;
		run = -1;
		file_anum = -1;
		filename = "";
	}
	virtual ~Data()
	{
	}
	virtual bool CheapPop(Long64_t ientry) const = 0;
	virtual bool BigPop(Long64_t ientry) const = 0;
	virtual bool Bind(TTree *fChain, int cachesize = -1) = 0;
};

#endif // DATA_H
