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
git clone git://github.com/cms-opendata-validation/2011-jet-inclusivecrosssection-ntupleproduction-optimized.git
scram b
cd 2011-jet-inclusivecrosssection-ntupleproduction-optimized/AnalysisFW/python/
```

