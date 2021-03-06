# CMS Z-Jet Analysis

This project is a CMSSW module that runs an analysis for events containing jets and muon decays. The source code, written in C++, will choose jets based on the highest transverse momentum (pt) value and reconstruct Z-bosons from a dimuon decay. Muon selections have been added in order to choose the best muons for analysis, and a new parameter is reconstructed after combining the best Z candidate with the jet. 

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

First, create a directory
```
mkdir zJetAnalysis
cd zJetAnalysis
```
Then, make the Analyzer
```
mkedanalyzer ZJetAnalyzer
cd ZJetAnalyzer
```
Now there should be the following seven directories in the ED Analyzer template
```
BuildFile.xml   doc   interface   python    src   test    zjetanalyzer_cfg.py
```

## Using the Analyzer

A C++ source code file will be automatically created in the `src` directory, so use `cd src` to access it. `zjetanalyzer_cfg.py` is the configuration file that will be used to run the source code. 

Copy the `zJet_cfg.py` file from this page to the `zjetanalyzer_cfg.py` file in your virtual machine. 

*Note: it might be easier to rename the file in the VM so that all the file names are consistent and easier to follow along with.*

Move into the `src` directory, and copy the `ZJetAnalyzer.cc` file into the .cc file that was automatically created. 

With `/home/cms-opendata/workingArea/CMSSW_5_3_32/src/zJetAnalysis/ZJetAnalyzer` as the current folder, run the following commands to download the files needed
```
wget http://opendata.cern.ch/record/10048/files/CMS_MonteCarlo2012_Summer12_DR53X_ZJetToMuMu_Pt-80to120_TuneEE3C_8TeV_herwigpp_AODSIM_PU_S10_START53_V19-v1_20000_file_index.txt

ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_V27
```

To understand the file names, record numbers, global tags and how to find that information for other files to run on, use http://opendata.cern.ch/search?page=1&size=200&q=&subtype=Simulated&experiment=CMS&year=2012&type=Dataset&subcategory=Drell-Yan&subcategory=ElectroWeak&category=Standard%20Model%20Physics to see the list of simulated datasets on the Open Data website. I narrowed these down to the eight datasets that involved simulating a Z-jet decay (ZJetToMuMu_) using different pt values. Here is the link to the dataset this code uses: http://opendata.cern.ch/record/10048

If you decide to run on Data instead of Monte Carlo simulations, you will also have to run 
```
wget http://opendata.cern.ch/record/1001/files/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt
```
to get a JSON file for good runs. Another change you will have to make when running on Data is to uncomment lines 32-36 (listed below) in the `zJet_cfg.py` file.
```
#import FWCore.PythonUtilities.LumiList as LumiList 
#goodJSON = './Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt' 
#myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',') 
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange() 
#process.source.lumisToProcess.extend(myLumis)
```

Once you have these files downloaded, you should compile the source code file. To do this, move into the `src` directory, and type
```
scram b
```
If you see a message that starts with `>> Building edm plugin `, the .cc file was successfully compiled without errors. You can move back up a directory by typing
```
cd ..
```
and run the configuration file by typing the command
```
cmsRun zJet_cfg.py
```
## Useful Commands for VirtualBox on Mac
The first time you open a virtual machine on VirtualBox, the viewing window will be very small and you will not be able to change the size by just dragging the window bigger. You can change this by clicking **Settings > Display** in VirtualBox, and changing the scale factor to 200%. 

The Copy and Paste keyboard commands in VirtualBox are different than they are for regular Mac use. You can use 
```
shift + cntrl + c
```
for copy. To paste, use 
```
shift + cntrl + v
```
You can also use 
``` 
cntrl + [-]
```
to zoom out of the CMS Shell Terminal window, and 
```
shift + cntrl + [+]
```
to zoom in.

If you're using `vi` to open files from the command line, the command
``` 
:set number
```
was useful when a compiling error gave a number line that the error was found on. 

## Troubleshooting
The VirtualBox software itself and the virtual machine are both hard to work with sometimes if not closed properly. Always *power off the machine* instead of *saving the current state.* 
Upon reopening, VirtualBox will most likely notify you that there is a new version of the software available. DO NOT UPDATE unless there is a newer version of the software that has been tested and approved. That information can be found on the Open Data website. 

Sometimes when either the virtual machine or the VirtualBox software itself doesn't close properly, you will get a scary error that won't allow you to open it or it will look like the virtual machine is gone. To fix this on a Mac, open Finder. Navigate to the folder where you downloaded the VirtualBox software. The easiest way to find the file you need is to type **CMS** into the search bar and scroll down until you find the files named *CMS-OpenData-1.5.3.vbox* and *CMS-OpenData-1.5.3.vbox-prev*. (This is the version I've been using but depending on updates, your version number may be different.) Right click on one of the files and open with a text editing software (I use TextEditor), but DO NOT ACTUALLY EDIT THE CODE. The file with the "-prev" extension is the temporary file that VirtualBox makes when you open the software. Sometimes the file without the "-prev" extension doesn't close or save correctly, so you won't see any code when you open it with a text editor. To fix it, move the *CMS-OpenData-1.5.3.vbox* file to a different directory or folder, and rename the *CMS-OpenData-1.5.3.vbox-prev* file to *CMS-OpenData-1.5.3.vbox*. Once you verify that VirtualBox can open correctly again, it's safe to delete the empty file.

This problem can also occur with the VirtualBox software instead of just the virtual machine file. If neither of the two **.vbox** files are empty, navigate in Finder to your home directory. Use keyboard shortcut
```
cmd + shift + [.]
```
to show any hidden folders. Go into the **Library** folder (it might be greyed out, but still click it), and open the VirtualBox folder. Use the same method as before, but using the *VirtualBox.xml* and *VirtualBox.xml-prev* files instead. 


## Copying Files to the HPC
You can save files to your personal computer from the virtual machine using UA's High Performance Computer if you get access to it. To do so, connect to the VPN, open a new Terminal (on Mac), and type
```
ssh username@uahpc.ua.edu
```
Then type in your myBama password. 
Once you've logged in to the HPC, go back to VirtualBox and navigate to the directory with the file you want to move. Type the following
```
scp filename username@uahpc.ua.edu:
```
Once you type your password, you should see a message that looks like:
```
zJet_cfg.py                                        100% 5250     5.1KB/s   00:00
```
and you can check in Terminal that the file has been successfully copied to your HPC account by typing `ls`.

To move the file from the HPC to your local machine, open a new Terminal window. Type the command
```
scp username@uahpc.ua.edu:filename .
```
After typing in your password, you should see the same message indicating the file has been copied. 

## Resources
The CMS Open Data website has several useful examples to gain familiarity with the VirtualBox software and the CMS environment. 
Other GitHub repositories using similar analyses can be found here: http://opendata.cern.ch/search?page=1&size=20&type=Software&subtype=Analysis&experiment=CMS&year=2011&year=2011-2012

Two useful ones that I used to build this code were:
1. http://opendata.cern.ch/record/5104
2. http://opendata.cern.ch/record/234


