// -*- C++ -*-
//
// Package:    ZJetAnalyzer
// Class:      ZJetAnalyzer
// 
/**\class ZJetAnalyzer ZJetAnalyzer.cc zJetAnalysis/ZJetAnalyzer/src/ZJetAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  
//         Created:  Tue Sep 22 19:10:02 GMT 2020
// $Id$
//
//


// system include files
#include <memory>
#include "TTree.h"
#include "TLorentzVector.h"

// user include files
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"

#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

//class declaration
using namespace edm;
using namespace reco;


class ZJetAnalyzer : public edm::EDAnalyzer {
   public:
      explicit ZJetAnalyzer(const edm::ParameterSet&);
      virtual void beginJob() ;
      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void endJob() ;
      virtual ~ZJetAnalyzer();
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

    
      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

      // ----------member data ---------------------------
//output tree
	edm::Service<TFileService>  fs;
    	TTree                       *mTree;


//Configurable parameters
	edm::InputTag   mPFak5JetsName;
	edm::InputTag	muonTag;
	edm::InputTag	vertexTag;

//Tree Jet Variables (for now just max values)
	Float_t maxPt;
	Float_t maxEta;
	Float_t maxPhi;

//Event Identification
	UInt_t run;
	UInt_t lumi;
	ULong64_t event;  

//Jet correction labels
	std::string mJetCorr_ak5;

//Z boson decay variables
	Float_t muonPt1, muonEta1, muonPhi1, muonCharge1, dxyVertexVal1, dzVertexVal1;
	bool tracker1, global1, tight1, dxyVertex1, dzVertex1;
	Float_t muonPt2, muonEta2, muonPhi2, muonCharge2, dxyVertexVal2, dzVertexVal2; 
	bool tracker2, global2, tight2, dxyVertex2, dzVertex2;
	Float_t zMassCand, zMass;
	reco::LeafCandidate::LorentzVector qStar;
	reco::LeafCandidate::LorentzVector bestZCand;
    	Float_t qStarMass;
};


ZJetAnalyzer::ZJetAnalyzer(const edm::ParameterSet& iConfig) {
   //now do what ever initialization is needed

	mPFak5JetsName = iConfig.getParameter<edm::InputTag> ("pfak5jets");
	mJetCorr_ak5   = iConfig.getParameter<std::string>   ("jetCorr_ak5");
	muonTag = iConfig.getParameter<edm::InputTag> ("muons");
	vertexTag = iConfig.getParameter<edm::InputTag> ("primaryVertices");
}



void ZJetAnalyzer::beginJob() {
        mTree = fs->make< TTree >("ZJetTree", "ZJetTree");

        mTree->Branch("Jet", &maxPt, "maxPt/F:maxEta:maxPhi");
        mTree->Branch("EventInfo", &run, "run/i:lumi/i:event/l");
	mTree->Branch("Muon1Info", &muonPt1, "muonPt1/F:muonEta1/F:muonPhi1/F:muonCharge1/F:dxyVertexVal1/F:dzVertexVal1/F:tracker1/b:global1/b:tight1/b:dxyVertex1/b:dzVertex1/b");
	mTree->Branch("Muon2Info", &muonPt2, "muonPt2/F:muonEta2/F:muonPhi2/F:muonCharge2/F:dxyVertexVal2/F:dzVertexVal2/F:tracker2/b:global2/b:tight2/b:dxyVertex2/b:dzVertex2/b"); 
	mTree->Branch("ZMass", &zMass, "zMass/F");
	mTree->Branch("ZJet", &qStarMass, "qStarMass/F");

}

void ZJetAnalyzer::endJob() {
}

void ZJetAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&) {
}


void ZJetAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
   using namespace edm;

	//three separate branches
	run = iEvent.id().run();
	lumi = iEvent.id().luminosityBlock();
	event = iEvent.id().event();
	
	edm::Handle<reco::PFJetCollection> ak5_handle;
	iEvent.getByLabel(mPFak5JetsName, ak5_handle);
	const JetCorrector* corrector_ak5 = JetCorrector::getJetCorrector(mJetCorr_ak5, iSetup);

	//Jet energy correction factor
	double jec = -1.0;

   	auto ptTemp = 0.0, etaTemp = 0.0, phiTemp = 0.0;  
	reco::LeafCandidate::LorentzVector bestJet; //the one with highest pt

   	for (auto i = ak5_handle->begin(); i != ak5_handle->end(); i++) {
		jec = corrector_ak5->correction(*i, iEvent, iSetup); 
     		auto correctedValue = (i->pt() * jec);

		if (correctedValue > ptTemp) {
			ptTemp = correctedValue;
			etaTemp = i->eta();
			phiTemp = i->phi();
			bestJet = i->p4();
   		}
	}

   	maxPt = ptTemp;
   	maxEta = etaTemp;
   	maxPhi = phiTemp; 	


//Start of muon decay analysis

	edm::Handle<reco::MuonCollection> muonHandle;
        iEvent.getByLabel(muonTag, muonHandle);
	edm::Handle<reco::VertexCollection> vertexHandle;
	iEvent.getByLabel(vertexTag, vertexHandle);

	reco::Vertex primaryVertex = vertexHandle->front(); //may be a better way of getting the primary vertex

	UInt_t size = muonHandle->size();
	if (size < 2) return;

	//insert lines like these for troubleshooting
		//UInt_t vSize = vertexHandle->size();
		//std::cout << "Vertex Handle Size = " << vSize << std::endl;	

		//std::cout << "MuonHandle size = " << size << std::endl;

	//initialize all parameters
	muonPt1 = 0.0, muonEta1 = 0.0, muonPhi1 = 0.0, muonCharge1 = 0.0, dxyVertexVal1 = 0.0, dzVertexVal1 = 0.0;
	muonPt2 = 0.0, muonEta2 = 0.0, muonPhi2 = 0.0, muonCharge2 = 0.0, dxyVertexVal2 = 0.0, dzVertexVal2 = 0.0;
	zMassCand = 0.0, zMass = 0.0;
	Float_t tempDiff = 1000000.0, currentDiff = 0.0;
	tracker1 = false, global1 = false, tight1 = false, dxyVertex1 = false, dzVertex1 = false;
	tracker2 = false, global2 = false, tight2 = false, dxyVertex2 = false, dzVertex2 = false;	

	for (auto i = muonHandle->begin(); i != muonHandle->end(); i++) {
		for (auto j = i; j  != muonHandle->end(); j++) {
			if (j == i) continue;  //if j = i, skip this j value and loop again

			if (i->charge() + j->charge() == 0) {
			if (i->globalTrack().isNull()) continue;
			if (j->globalTrack().isNull()) continue;
			//getting muon parameters
				muonPt1 = i->pt();
				muonEta1 = i->eta();
				muonPhi1 = i->phi();
				muonCharge1 = i->charge();
				dxyVertexVal1 = fabs(i->muonBestTrack()->dxy(primaryVertex.position()));
				dzVertexVal1 = fabs(i->muonBestTrack()->dz(primaryVertex.position()));
				if (i->isTrackerMuon()) tracker1 = true;
				if (i->isGlobalMuon()) global1 = true;
				if(i->globalTrack()->normalizedChi2() < 10 && i->globalTrack()->hitPattern().numberOfValidMuonHits() > 0) tight1 = true;
				if(fabs(i->muonBestTrack()->dxy(primaryVertex.position())) < 0.2) dxyVertex1 = true;
				if(fabs(i->muonBestTrack()->dz(primaryVertex.position())) < 0.5) dzVertex1 = true;	
				muonPt2 = j->pt();
				muonEta2 = j->eta();
				muonPhi2 = j->phi();
				muonCharge2 = j->charge();
				dxyVertexVal2 = fabs(j->muonBestTrack()->dxy(primaryVertex.position()));
                                dzVertexVal2 = fabs(j->muonBestTrack()->dz(primaryVertex.position()));
				if (j->isTrackerMuon()) tracker2 = true;
                                if (j->isGlobalMuon()) global2 = true;
                                if(j->globalTrack()->normalizedChi2() < 10 && j->globalTrack()->hitPattern().numberOfValidMuonHits() > 0) tight2 = true;
                                if(fabs(j->muonBestTrack()->dxy(primaryVertex.position())) < 0.2) dxyVertex2 = true;
                                if(fabs(j->muonBestTrack()->dz(primaryVertex.position())) < 0.5) dzVertex2 = true;

			//if multiple pairs, compare all
				zMassCand = (i->p4() + j->p4()).M();
				currentDiff = std::abs(91.2 - zMassCand);
				if (currentDiff < tempDiff) {
					tempDiff = currentDiff;
					bestZCand = i->p4() + j->p4(); //best candidate is automatically the first one
					zMass = zMassCand;
				}

			//compare to actual Z boson mass (91.2 GeV)
				if (zMass <= 71.2 || zMass >= 111.2) {
					zMass = 0.0;
				} 

			}//end of charge if statement
	    }//end of j loop
	}//end of i loop 

	//combine bestZ with jet to make q*
		qStar = bestZCand + bestJet;
		qStarMass = qStar.M();

//fill muon branches
	if (zMass != 0.0) mTree->Fill();

	//if (zMass == 0.0) std::cout << "No Zs in this event." << std::endl;



#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}

void ZJetAnalyzer::endRun(edm::Run const&, edm::EventSetup const&) {
}


ZJetAnalyzer::~ZJetAnalyzer() {
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}

void ZJetAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

void ZJetAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}


void ZJetAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(ZJetAnalyzer);
