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
A C++ source code file will be automatically created in the **src** directory, so use **cd src** to access it. The configuration file that will be used to 

## Troubleshooting
The VirtualBox software itself and the virtual machine are both hard to work with sometimes if not closed properly. Always *power down* the machine instead of *saving the current state.* Upon reopening, VirtualBox will most likely notify you that there is a new version of the software available. DO NOT UPDATE unless there is a newer version of the software that has been tested and approved. That information can be found on the Open Data website. 

## Copying Files to the HPC
You can save files to your personal computer from the Virtual Machine using UA's High Performance Computer if you get access to it. To do so, connect to the VPN, open a new Terminal (on Mac), and type
```
ssh username@uahpc.ua.edu
```
Then type in your myBama password. 
Once you've logged in on your local machine, go back to virtualBox and navigate to the directory with the file you want to move. Type the following
```
scp filename username@uahpc.ua.edu
```
Once you type your password, you should see a message that looks like: 
**zJet_cfg.py                                        100% 5250     5.1KB/s   00:00**
and you can check in Terminal that the file has been successfully copied by typing **ls**.
