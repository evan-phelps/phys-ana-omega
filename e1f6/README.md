# README

## Notes

* Having evolved from command-line scripted classes, design is a mess -- too much interdependency between H10, data handlers, and utility functions!
* need to implement a good particle-level handler concept -- currently requires callback to CalcLVs when particle properties are changed or particles are filtered.

## H10

H10 is the main processing class that binds h10-formatted trees to variables and controls the event loop.  It has a HandlerChain that manages for each event the conditional execution of a sequence of DataHandlers.

## HandlerChain

Maintains the queue of data handlers.

## DataHandler

Data handlers can serve as tree (or binned data) builders, plot builders (monitor), transformers, filters, or any combination of these.  The following table denotes the current status of each handler in terms of function.

| Handler             | trees | plots | transform | filter | notes |
|:--------------------|:-----:|:-----:|:---------:|:------:|:------|
| DH_SC_Hists_PrePid  |       |   X   |           |        |       |
| DH_EC_Hists_PreEid  |       |   X   |           |        |       |
| DH_CC_Hists         |       |   X   |           |        |       |
| DH_RunQuality       |   X   |   X   |           |   X    | Q skim |
| DH_Eid              |       |   X   |     X     |   X    | first particle must be electron to pass |
| DH_Efid             |       |       |           |   X    | currently skims events with electrons in fiducial volume |
| DH_Hfid             |       |       |     X     |        | hack: id=0 if outside of fiducial volume |
| DH_SC_BadPdls       |       |       |     X     |        | hack: id=0 if hits bad paddle |
| DH_Efid_Hists       |       |   X   |     X     |        | change to "PreEfid"? |
| DH_Hists_Monitor    |       |   X   |           |        |       |
| DH_EC_Hists         |       |   X   |           |        |       |
| DH_CloneH10         |   X   |   X   |           |        |       |
| DH_H6Maker          |   X   |       |           |        |       |

The two handlers that currently alter the particle id would need to modify H10's lorentz vectors to have a logical effect -- **This is just bad design and needs to be changed!**  Current work-around is to expose access to H10::CalcLVs() for data handlers to call, which recalculates all lorentz vectors, invariants, and rotations.

### DH_EC_Hists_PreEid

Produces histograms for first particle of each event with overly strong electron isolation cuts independently applied to amplify the character of electron EC signatures.  Resulting histograms are used by ../worksheets/eid.ipynb to produce cut parameters that feed into DH_Eid.

### DH_RunQuality

Should perhaps be called DH_RunQuality_and_ChargeSkimmer.  It not only builds a luminosity-block-level tree of event counts and accumulated Faraday-cup charge, but it also picks out events that have a negative first detected particle plus either (a) 2 positive, 0 negative, (b) 1 positive, 1 negative, or (c) 2 positive, 1 negative particles (any number of neutrals are allowed).

For each run, block entries are accumulated.  Each entry includes block-level values.

        file_name
        run_num
        file_num
        lumblock_num
        evt_num_first
        evt_num_last
        fc_charge_ltcorr
        livetime
        ntrigs
        ne
        np
        npip
        npim
        nevts_Neg1st
        nevts_Neg1st_2Pos_exc
        nevts_Neg1st_1Pos1Neg_exc
        nevts_Neg1st_2Pos1Neg_exc

Two electron-sector-specific histograms are filled:

* hq2_V_w
* hq2_V_w_elast_exc

And one histogram of fully exclusive events:

* hmmppippim_V_mmp

In the same event loop, an "h10" skimmed event tree is populated for events that have

1. gpart < 8
1. negative particle first
1. exactly 2 positive OR 1 positive, 1 negative OR 2 positive, 1 negative

### DH_Eid

Side effects:

* etot = etot > ec_ei+ec_eo ? etot : ec_ei+ec_eo
* filters -- cuts events that do not have a good electron as first particle

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

1. Determine electron identification parameters
    + jget selection of cooked data files for selected experiment
        - /mss/clas/e1f/production/pass2/v1/data/
        - /mss/clas/e1-6a/production/pass2/v1/data/
    + convert to hbook and root
        <pre><code>for fn in $(ls run*.1); do nt10maker -t1 -o$fn.hbook $fn && h2root $fn.hbook; done</code></pre>
    + in root/CInt, create histograms for parameter extraction
        <pre><code>.L Config.cpp+
        .L DataHandler.h+
        .L HandlerChain.cpp+
        .L H10.C+
        .L DH_EC_Hists_PreEid.h+
        TFile *fout = new TFile("EC_Hists_e16.root","create")
        TChain *c = new TChain("h10")
        c->Add("*.root")
        H10 *h10proc = new H10(c, "e16")
        h10proc->Add(new DH_EC_Hists_PreEid("ecpreeid", fout))
        h10proc->Loop()</code></pre>
    + copy root file to local computer
    + use ../worksheets/eid.ipynb to generate figures and get parameters
    + copy parameters into appropriate input file:
        - input.e16.exp.parms
        - input.e1f.exp.parms
1. Create data skim
    + see worksheet "run_list" to produce xml submission files
        - CDATA execution line reads
            <pre><code>bash -l -c "python ~/omega/src/processH10s.py -b -o out.root -w ~/omega/src -c ~/omega/src/input.e16.exp.parms"</code></pre>
        - sequential folder/handler names in pipeline:
            <pre>mon_raw, echists_raw, cchists_raw, runquality, cchists_qskim, echists_qskim, mon_qskim, eid, echists_eskim, cchists_eskim, mon_eskim, h10_eskim, scpid_eskim</pre>
            <pre>DH_Hists_Monitor, DH_EC_Hists_PreEid, DH_CC_Hists, DH_RunQuality, DH_CC_Hists, DH_EC_Hists,
        DH_Hists_Monitor, DH_Eid, DH_EC_Hists, DH_CC_Hists, DH_Hists_Monitor, DH_CloneH10, DH_SC_Hists_PrePid</pre>
    + perform several data handling tasks on this process:
        - run quality
        - electron identification
        - accumulate histograms for momentum corrections
        - clone h10 for skimmed events
        <pre><code>.L Config.cpp+
        .L DataHandler.h+
        .L HandlerChain.cpp+
        .L H10.C+
        .L DH_EC_Hists_PreEid.h+
        .L DH_RunQuality.cpp+
        .L DH_EC_Hists.h+
        .L DH_CloneH10.h+
        .L DH_Hists_Monitor.h+
        .L DH_Eid.h+
        .L DH_SC_Hists_PrePid.h+
        .L DH_W_Skim.h+
        .L DH_MMp_Skim.h+
        TFile *fout = new TFile("test.root","recreate")
        TChain *c = new TChain("h10clone/h10")
        c->Add("/data/e1f/skim/3812?.root")
        H10 *h10proc = new H10(c, "input.e1f.exp.parms")
        h10proc->Add(new DH_Hists_Monitor("mon_raw", fout))
        h10proc->Add(new DH_EC_Hists("echists_qskim", fout))
        h10proc->Add(new DH_RunQuality("runquality", fout))
        h10proc->Add(new DH_EC_Hists_PreEid("echists_raw", fout))
        h10proc->Add(new DH_EC_Hists("echists_qskim", fout))
        h10proc->Add(new DH_Hists_Monitor("mon_qskim", fout))
        h10proc->Add(new DH_Eid("eid", fout))
        h10proc->Add(new DH_EC_Hists("echists_eskim", fout))
        h10proc->Add(new DH_Hists_Monitor("mon_eskim", fout))
        h10proc->Add(new DH_SC_Hists_PrePid("scpid", fout))
        h10proc->Add(new DH_W_Skim("w_skim", fout))
        h10proc->Add(new DH_MMp_Skim("mmp_skim", fout))
        h10proc->Add(new DH_CloneH10("h10clone", fout))
        h10proc->Loop(-1,kFALSE)</code></pre>
1. Determine or use existing electron momentum corrections
1. Determine electron fiducial cut parameters
1. Determine hadron identification parameters
1. Determine hadron fiducial cut parameters
