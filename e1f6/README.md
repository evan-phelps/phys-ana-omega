# README

## H10

H10 is the main processing class that binds h10-formatted trees to variables and controls the event loop.  It has a HandlerChain that manages for each event the conditional execution of a sequence of DataHandlers.

## HandlerChain

## DataHandler

### DH_EC_Hists_PreEid

Produces histograms for first particle of each event with overly strong electron isolation cuts independently applied to amplify the character of electron EC signatures.  Resulting histograms are used by ../worksheets/eid.ipynb to produce cut parameters that feed into DH_Eid.

### DH_RunQuality

Should perhaps be called DH_RunQuality_and_ChargeSkimmer.  It not only builds a luminosity-block-level tree of event counts and accumulated Faraday-cup charge, but it also picks out events that have a negative first detected particle plus either (a) 2 positive, 0 negative, (b) 1 positive, 1 negative, or (c) 2 positive, 1 negative particles (any number of neutrals are allowed).

### DH_Eid

Side effect:  etot = etot > ec_ei+ec_eo ? etot : ec_ei+ec_eo

Performs identification of strong electron candidates according to the following criteria:

* first particle of event
* negative particle (already accounted if DH_RunQuality executed prior)
* same-sector DC, SC, CC, EC hits
* DC track points toward SC hit and be matched in time (stat>0, geometrical matching and viable timing from SC)
* EC energy/p ~ 1 (SF/p cuts)
* EC inner < minimum ionizing threshold

The last two items depend on parameters from output parameters of ../worksheets/eid.ipynb, assumed to be copied into input.*.exp.parms, chosen by experiment.

### DH_CloneH10

Clones the h10 tree for events surviving cuts imposed by previous data handlers.

### DH_Hists_Monitor

Produces common histograms that are interesting to look at between various event selection schemes.

## Procedures

* Determine electron identification parameters
** jget selection of cooked data files for selected experiment
*** /mss/clas/e1f/production/pass2/v1/data/
*** /mss/clas/e1-6a/production/pass2/v1/data/
** convert to hbook and root
    for fn in $(ls run*.1); do nt10maker -t1 -o$fn.hbook $fn && h2root $fn.hbook; done
** in root/CInt, create histograms for parameter extraction
    .L Config.cpp+
    .L DataHandler.h+
    .L HandlerChain.cpp+
    .L H10.C+
    .L DH_EC_Hists_PreEid.h+
    TFile *fout = new TFile("EC_Hists_e16.root","create")
    TChain *c = new TChain("h10")
    c->Add("*.root")
    H10 *h10proc = new H10(c, "e16")
    h10proc->Add(new DH_EC_Hists_PreEid("ecpreeid", fout))
    h10proc->Loop()
** copy root file to local computer
** use ../worksheets/eid.ipynb to generate figures and get parameters
** copy parameters into appropriate input file:
*** input.e16.exp.parms
*** input.e1f.exp.parms
* Create data skim
** perform several data handling tasks on this process:
*** run quality
*** electron identification
*** accumulate histograms for momentum corrections
*** clone h10 for skimmed events
