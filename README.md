# CMS Jet-Z boson analysis

This project is a CMSSW module ...... (edit desctiption)

The instructions assume that you will work on a VM properly contextualized for CMS, available from http://opendata.cern.ch/VM/CMS.

## Creating the Working Area 
```
mkdir WorkingArea
cd ./WorkingArea
cmsrel CMSSW_5_3_32
cd ./CMSSW_5_3_32/src
cmsenv
```

## Creating an EDAnalyzer Template
```
#first you must create a directory
mkdir zJetAnalysis

#then make the analyzer
mkedanalyzer ZJetAnalyzer

#move to the analyzer 
cd ZJetAnalyzer
```
