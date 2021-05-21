
# Forked from SMPJ Analysis Framework
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/SMPJAnalysisFW
# https://github.com/cms-smpj/SMPJ/tree/v1.0
# (further optimized to improve performance)


## Import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.Utilities.FileUtils as FileUtils

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# True : when running in OpenData virtual machine
# False: when runing in lxplus 
runOnVM = True

# Index of data files
#maybe make a loop that runs over multiple files
#or store the files in an array and loop over each file in the array ? 
#files2011data = FileUtils.loadListFromFile('CMS_Run2011A_Jet_AOD_12Oct2013-v1_20000_file_index.txt')
files2011data = FileUtils.loadListFromFile('CMS_MonteCarlo2012_Summer12_DR53X_ZJetToMuMu_Pt-80to120_TuneEE3C_8TeV_herwigpp_AODSIM_PU_S10_START53_V19-v1_20000_file_index.txt')
process.source.fileNames = cms.untracked.vstring(*files2011data)

# Read condition data from local sqlite database
if runOnVM:
    process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/START53_V27.db')
#CHANGE GLOBAL TAG HERE

# Read good luminosity sections from local JSON file
#import FWCore.PythonUtilities.LumiList as LumiList 
#goodJSON = './Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt' 
#myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',') 
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange() 
#process.source.lumisToProcess.extend(myLumis)


# Global tag for 2011A data
#process.GlobalTag.globaltag = 'FT_53_LV5_AN1::All'
process.GlobalTag.globaltag = 'START53_V27::All'
#CHANGE GLOBAL TAG HERE

# Select good vertices
process.goodOfflinePrimaryVertices = cms.EDFilter(
    "VertexSelector",
    filter = cms.bool(False),
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
    )

# Tracking failure filter
from RecoMET.METFilters.trackingFailureFilter_cfi import trackingFailureFilter
process.trackingFailureFilter = trackingFailureFilter.clone()
process.trackingFailureFilter.VertexSource = cms.InputTag('goodOfflinePrimaryVertices')

# Load jet correction services for all jet algoritms
process.load("JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff")

################### EDAnalyzer ##############################
process.ak5ak7 = cms.EDAnalyzer('OpenDataTreeProducerOptimized',
    ## jet collections ###########################
    pfak7jets       = cms.InputTag('ak7PFJets'),
    pfak5jets       = cms.InputTag('ak5PFJets'),
    ## MET collection ####
    pfmet           = cms.InputTag('pfMET5'),
    ## set the conditions for good Vtx counting ##
    offlineVertices = cms.InputTag('goodOfflinePrimaryVertices'),
    goodVtxNdof     = cms.double(4), 
    goodVtxZ        = cms.double(24),
    ## rho #######################################
    srcPFRho        = cms.InputTag('kt6PFJets','rho'),
    ## preselection cuts #########################
    maxY            = cms.double(5.0), 
    minPFPt         = cms.double(15),
    minNPFJets      = cms.int32(1),
    minJJMass       = cms.double(-1),
    isMCarlo        = cms.untracked.bool(False),
    ## trigger ###################################
    printTriggerMenu = cms.untracked.bool(True),
    processName     = cms.string('HLT'),
    triggerNames    = cms.vstring(
                                    'HLT_Jet30', 'HLT_Jet60', 'HLT_Jet80', 'HLT_Jet110', 
                                    'HLT_Jet150', 'HLT_Jet190','HLT_Jet240','HLT_Jet370',
                                ),
    triggerResults  = cms.InputTag("TriggerResults","","HLT"),
    triggerEvent    = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    ## jet energy correction labels ##############
    jetCorr_ak5      = cms.string('ak5PFL1FastL2L3Residual'),
    jetCorr_ak7      = cms.string('ak7PFL1FastL2L3Residual'),
)

# HLT filter
process.hltFilter = cms.EDFilter('HLTHighLevel',
    TriggerResultsTag  = cms.InputTag('TriggerResults','','HLT'),
    HLTPaths           = cms.vstring('HLT_Jet*', 'HLT_DiJetAve*'),
    eventSetupPathsKey = cms.string(''),
    andOr              = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
    throw              = cms.bool(False)
)

process.zJetAnalyzer = cms.EDAnalyzer('ZJetAnalyzer',
	pfak5jets       = cms.InputTag('ak5PFJets'),
	jetCorr_ak5     = cms.string('ak5PFL1FastL2L3Residual'),
	muons 		= cms.InputTag('muons'),
	primaryVertices	= cms.InputTag('offlinePrimaryVertices') 
)

# Run everything
process.p = cms.Path(
    process.goodOfflinePrimaryVertices*
    process.hltFilter *
    process.trackingFailureFilter *
    process.zJetAnalyzer
  
)

# Approximate processing time on VM (Intel Core i5-5300U 2.3GHz laptop):
# 50000 events per 1 hour (both for DATA and MC)

# Change number of events here:
process.maxEvents.input = 96000

process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Change Output file name here
process.TFileService = cms.Service("TFileService", fileName = cms.string('zJet_80to120.root'))


# To suppress long output at the end of the job
#process.options.wantSummary = False   

del process.outpath
