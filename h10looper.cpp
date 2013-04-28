#include "h10looper.h"

h10looper::h10looper() : fChain() { };
h10looper::h10looper(Data *d, TTree *tree, int cachesize /* = -1 */)  : fChain(0)
{
	fHandlerChain = new HandlerChain();
	data = d;
	_cachesize = cachesize;
	fTreeNumber = -1;
	eventnum = 0;
	fRegExp_run = new TRegexp("[0-9][0-9][0-9][0-9][0-9]");
	fRegExp_Anum = new TRegexp("[Aa][0-9][0-9]");
	if (tree == 0) {
		TChain * chain = new TChain("h10","");
		chain->Add("/data1/e1f/root/main/root_*.root");
		tree = chain;
	}
	Init(tree);
}
h10looper::~h10looper()
{
	delete fHandlerChain;
	if (fChain)	delete fChain->GetCurrentFile();
	delete fRegExp_run;
	delete fRegExp_Anum;
	//delete data;
}
void h10looper::Loop(Long64_t ntoproc /* = -1 */, Bool_t fastcount /* =kTRUE */, TEntryList *elist /* = 0 */)
{
	if (fChain == 0) return;
	if (ntoproc == -1) {
		if (fastcount) ntoproc = fChain->GetEntriesFast();
		else ntoproc = elist ? elist->GetN() : fChain->GetEntries();
	}
	std::cout << "ntoproc = " << ntoproc << std::endl;
	_ntoprocess = ntoproc;
	//Long64_t nbytes = 0, nb = 0;
	_swmain.Start();
	_swgroup.Start();
	for (Long64_t jentry_el=0; jentry_el<ntoproc; jentry_el++) {
		jentry = elist ? elist->GetEntry(jentry_el) : jentry_el;
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		PrintProgress(jentry);
		//data->CheapPop(ientry);
		GetEntry(jentry);
		fHandlerChain->Process(data);
	}
	fHandlerChain->Finalize(data);
	Float_t ttime = _swmain.RealTime();
	Float_t percentProcessed = (Float_t)eventnum/_ntoprocess*100;
	printf("Total: (%.2f) %lld/%.2f = %i events/sec\n",percentProcessed,eventnum,ttime,(Int_t)(eventnum/ttime));
}
Int_t h10looper::GetEntry(Long64_t entry)
{
// Read contents of entry.
	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}
Long64_t h10looper::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
	if (!fChain) return -5;
	Long64_t centry = fChain->LoadTree(entry);
	if (centry < 0) return centry;
	if (fChain->GetTreeNumber() != fCurrent) {
		fCurrent = fChain->GetTreeNumber();
		Notify();
	}
	return centry;
}
void h10looper::Init(TTree *tree)
{
	// The Init() function is called when the selector needs to initialize
	// a new tree or chain. Typically here the branch addresses and branch
	// pointers of the tree will be set.
	// It is normally not necessary to make changes to the generated
	// code, but the routine can be extended by the user if needed.
	// Init() will be called many times when running on PROOF
	// (once per file to be processed).
	// Set branch addresses and branch pointers
	if (!tree) return;
	fChain = tree;
	fCurrent = -1;
	fChain->SetMakeClass(1);
	data->Bind(fChain,_cachesize);
	Notify();
}
Bool_t h10looper::Notify()
{
	// The Notify() function is called when a new file is opened. This
	// can be either for a new TTree in a TChain or when when a new TTree
	// is started when using PROOF. It is normally not necessary to make changes
	// to the generated code, but the routine can be extended by the
	// user if needed. The return value is currently not used.
	int tn = fChain->GetTreeNumber();
	if (tn != fTreeNumber) {
		TString fullfn = ((TChain*)fChain)->GetFile()->GetName();
		TObjArray *tokens = fullfn.Tokenize("/");
		TObjString *tok = (TObjString*)tokens->At(tokens->GetLast());
		TString fn = tok->GetString();
		data->run = ((TString)fn(*fRegExp_run)).Atoi();
		data->file_anum = ((TString)((TString)fn(*fRegExp_Anum))(1,2)).Atoi();
		data->filename = fn.Data();
		delete tokens;
	}
	return kTRUE;
}
void h10looper::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
	if (!fChain) return;
	fChain->Show(entry);
}
Int_t h10looper::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
	return 1;
}
