# CMS Z-Jet Analysis

This project is a CMSSW module ...... (edit desctiption)

The instructions assume that you will work on a VM properly contextualized for CMS, available from http://opendata.cern.ch/VM/CMS.

## Creating the Working Area 
```
mkdir WorkingArea
cd WorkingArea
cmsrel CMSSW_5_3_32
cd CMSSW_5_3_32/src
cmsenv
```

## Creating an EDAnalyzer Template

##### First, create a directory
```
mkdir zJetAnalysis
cd zJetAnalysis
```
##### Then, make the Analyzer
```
mkedanalyzer ZJetAnalyzer
cd ZJetAnalyzer
```
##### Now there should be the following seven directories in the ED Analyzer template
```
BuildFile.xml   doc   interface   python    src   test    zjetanalyzer_cfg.py
```

##### Troubleshooting
The VirtualBox software itself and the virtual machine are both hard to work with sometimes if not closed properly. Always *power down* the machine instead of *saving the current state.* Upon reopening, VirtualBox will most likely notify you that there is a new version of the software available. DO NOT UPDATE unless there is a newer version of the software that has been tested and approved. That information can be found on the Open Data website. 

