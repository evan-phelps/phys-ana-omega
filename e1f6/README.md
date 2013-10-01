# README

## H10

H10 is the main processing class that binds h10-formatted trees to variables and controls the event loop.  It has a HandlerChain that manages for each event the conditional execution of a sequence of DataHandlers.

## HandlerChain

## DataHandler

### DH_RunQuality

Should perhaps be called DH_RunQuality_and_ChargeSkimmer.  It not only builds a luminosity-block-level tree of event counts and accumulated Faraday-cup charge, but it also picks out events that have a negative first detected particle plus either (a) 2 positive, 0 negative, (b) 1 positive, 1 negative, or (c) 2 positive, 1 negative particles (any number of neutrals are allowed).


### DH_EC_Hists_PreEid

Produces histograms for first particle of each event with overly strong electron isolation cuts independently applied to amplify the character of electron EC signatures.  Resulting histograms are used by ../worksheets/eid.ipynb to produce cut parameters that feed into DH_Eid.

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
