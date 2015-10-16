#include <map>
#include <string>

#include "TH1D.h"
#include "TH2D.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"

class PatTriggerAnalyzer : public edm::EDAnalyzer {

 public:
  explicit PatTriggerAnalyzer( const edm::ParameterSet & iConfig );
  ~PatTriggerAnalyzer();
 private:
  virtual void beginJob();
  virtual void analyze( const edm::Event & iEvent, const edm::EventSetup & iSetup );
  virtual void endJob();
  edm::InputTag trigger_;
  edm::EDGetTokenT< pat::TriggerEvent > triggerEventToken_;
  edm::EDGetTokenT< pat::MuonCollection > muonsToken_;
  std::string   muonMatch_;
  std::map< std::string, TH1D* > histos1D_;
  std::map< std::string, TH2D* > histos2D_;
};

#include "TMath.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"


using namespace pat;


PatTriggerAnalyzer::PatTriggerAnalyzer( const edm::ParameterSet & iConfig ) :
  trigger_( iConfig.getParameter< edm::InputTag >( "trigger" ) ),
  triggerEventToken_( consumes< TriggerEvent >( iConfig.getParameter< edm::InputTag >( "triggerEvent" ) ) ),
  muonsToken_( consumes< MuonCollection >( iConfig.getParameter< edm::InputTag >( "muons" ) ) ),
  muonMatch_( iConfig.getParameter< std::string >( "muonMatch" ) ),
  histos1D_(), histos2D_()
{
}
PatTriggerAnalyzer::~PatTriggerAnalyzer()
{
}

void PatTriggerAnalyzer::beginJob()
{
  edm::Service< TFileService > fileService;
  histos2D_[ "ptTrigCand" ] = fileService->make< TH2D >( "ptTrigCand", "Object vs. candidate p_{T} (GeV)", 60, 0., 300., 60, 0., 300. );
  histos2D_[ "ptTrigCand" ]->SetXTitle( "candidate p_{T} (GeV)" );
  histos2D_[ "ptTrigCand" ]->SetYTitle( "object p_{T} (GeV)" );

  histos2D_[ "etaTrigCand" ] = fileService->make< TH2D >( "etaTrigCand", "Object vs. candidate #eta", 50, -2.5, 2.5, 50, -2.5, 2.5 );
  histos2D_[ "etaTrigCand" ]->SetXTitle( "candidate #eta" );
  histos2D_[ "etaTrigCand" ]->SetYTitle( "object #eta" );

  histos2D_[ "phiTrigCand" ] = fileService->make< TH2D >( "phiTrigCand", "Object vs. candidate #phi", 60, -TMath::Pi(), TMath::Pi(), 60, -TMath::Pi(), TMath::Pi() );
  histos2D_[ "phiTrigCand" ]->SetXTitle( "candidate #phi" );
  histos2D_[ "phiTrigCand" ]->SetYTitle( "object #phi" );
  histos1D_[ "test" ]=fileService->make< TH1D >("test", "this is to see how many muons are in a event", 10, 0, 10);
}
void PatTriggerAnalyzer::analyze( const edm::Event & iEvent, const edm::EventSetup & iSetup )
{
  edm::Handle< TriggerEvent > triggerEvent;
  iEvent.getByToken( triggerEventToken_, triggerEvent );
  edm::Handle< MuonCollection > muons;
  iEvent.getByToken( muonsToken_, muons );
  const helper::TriggerMatchHelper matchHelper;
  size_t test=0; 
  for( size_t iMuon = 0; iMuon < muons->size(); ++iMuon ) {
    const TriggerObjectRef trigRef( matchHelper.triggerMatchObject( muons, iMuon, muonMatch_, iEvent, *triggerEvent ) );
    if ( trigRef.isAvailable() && trigRef.isNonnull()&& (muons->size())!=0 ) { // check references (necessary!)
      test++;
      histos2D_[ "ptTrigCand" ]->Fill( muons->at( iMuon ).pt(), trigRef->pt() );
      histos2D_[ "etaTrigCand" ]->Fill( muons->at( iMuon ).eta(), trigRef->eta() );
      histos2D_[ "phiTrigCand" ]->Fill( muons->at( iMuon ).phi(), trigRef->phi() );
    }
  }
  histos1D_[ "test" ]->Fill(test);
}

void PatTriggerAnalyzer::endJob()
{
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE( PatTriggerAnalyzer );
