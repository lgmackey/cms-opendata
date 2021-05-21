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
The VirtualBox software itself and the virtual machine are both hard to work with sometimes if not closed properly. Always *power off the machine* instead of *saving the current state.* 
Upon reopening, VirtualBox will most likely notify you that there is a new version of the software available. DO NOT UPDATE unless there is a newer version of the software that has been tested and approved. That information can be found on the Open Data website. 

Sometimes when either the virtual machine or the VirtualBox software itself doesn't close properly, you will get a scary error that won't allow you to open it or it will look like the virtual machine is gone. To fix this on a Mac, open Finder. Navigate to the folder where you downloaded the VirtualBox software. The easiest way to find the file you need is to type **CMS** into the search bar and scroll down until you find the files named *CMS-OpenData-1.5.3.vbox* and *CMS-OpenData-1.5.3.vbox-prev*. (This is the version I've been using but depending on updates, your version number may be different.) Right click on one of the files and open with a text editing software (I use TextEditor), but DO NOT ACTUALLY EDIT THE CODE. The file with the "-prev" extension is the temporary file that VirtualBox makes when you open the software. Sometimes the file without the "-prev" extension doesn't close or save correctly, so you won't see any code when you open it with a text editor. To fix it, move the *CMS-OpenData-1.5.3.vbox* file to a different directory or folder, and rename the *CMS-OpenData-1.5.3.vbox-prev* file to *CMS-OpenData-1.5.3.vbox*. Once you verify that VirtualBox can open correctly again, it's safe to delete the empty file.

This problem can also occur with the VirtualBox software instead of just the virtual machine file. If neither of the two **.vbox** files are empty, navigate in Finder to your home directory. Use keyboard shortcut
```
cmd + shift + [.]
```
to show any hidden folders. Go into the **Library** folder (it might be greyed out, but still click it), and open the VirtualBox folder. Use the same method as before, but using the *VirtualBox.xml* and *VirtualBox.xml-prev* files instead. 

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

If you're using **vi** to open files from the command line, the command
``` 
:set number
```
was useful when a compiling error gave a number line that the error was found on. 

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
and you can check in Terminal that the file has been successfully copied to your HPC account by typing **ls**.

To move the file from the HPC to your local machine, open a new Terminal window. Type the command
```
scp username@uahpc.ua.edu:filename .
```
After typing in your password, you should see the same message indicating the file has been copied. 
