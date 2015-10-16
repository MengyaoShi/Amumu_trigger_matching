
### ========
### Skeleton
### ========

## ---
## Start with pre-defined skeleton process
## ---
from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.Utilities.FileUtils as FileUtils
mylist=FileUtils.loadListFromFile('/afs/cern.ch/user/m/mshi/amumu_trigger_matching/CMSSW_7_4_1_patch4/src/sourceFiles.txt')
## switch to uncheduled mode
process.options.allowUnscheduled = cms.untracked.bool(True)
#process.Tracer = cms.Service("Tracer")


#process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
#process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")
##process.load("PhysicsTools.PatAlgos.slimming.slimming_cff")
process.source.fileNames = cms.untracked.vstring(*mylist)
process.maxEvents.input     = 1000 # reduce number of events for testing.
process.options.wantSummary = True # to suppress the long output at the end of the job


### ========
### Plug-ins
### ========

## ---
## PAT trigger matching
## --
process.muonTriggerMatchHLTMuons = cms.EDProducer(
  # matching in DeltaR, sorting by best DeltaR
  "PATTriggerMatcherDRLessByR"
  # matcher input collections
, src     = cms.InputTag( 'slimmedMuons' )
, matched = cms.InputTag( 'patTrigger' )
  # selections of trigger objects
, matchedCuts = cms.string( 'type( "TriggerMuon" ) && path( "HLT_Mu16_TkMu0_dEta18_Onia_v1", 1, 0 )' ) # input does not yet have the 'saveTags' parameter in HLT
  # selection of matches
, maxDPtRel   = cms.double( 0.5 ) # no effect here
, maxDeltaR   = cms.double( 0.5 )
, maxDeltaEta = cms.double( 0.2 ) # no effect here
  # definition of matcher output
, resolveAmbiguities    = cms.bool( True )
, resolveByMatchQuality = cms.bool( True )
)
### ============
### Python tools
### ============

## --
## Switch to selected PAT objects in the main work flow
## --
##from PhysicsTools.PatAlgos.tools.coreTools import removeCleaning
##removeCleaning( process ) ## this function is not available anymore in 70X (TJ)
# to save a bit of disk space
process.out.outputCommands += [ 'drop *_*_*_*'
                               ,'keep *_patTrigger*_*_*'
                               ,'keep *_slimmedMuons*_*_*'
                              ]
process.out.fileName = 'edmPatTrigger.root'
## --
## Switch on PAT trigger
## --
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process ) # This is optional and can be omitted.
switchOnTriggerMatching( process, triggerMatchers = [ 'muonTriggerMatchHLTMuons' ])
# Switch to selected PAT objects in the trigger matching
#removeCleaningFromTriggerMatching( process ) ## this function is not available anymore in 70X (TJ)
