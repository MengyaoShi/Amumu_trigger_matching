# Amumu_trigger_matching
This directory contains necessary things for trigger matching of gluon fusion to pseudoscalar higgs in NMSSM model, decays to two muon. Using Mengyao's CERN eos space files, listed in .txt file, combine with Physics Analysis Toolkit, you are goood to go!

1)Here are steps getting necessary package:

ssh -X your_lxplus_name@lxplus6.cern.ch 

cd scratch0/

mkdir amumu_trigger_matching

cd amumu_trigger_matching

cmsrel CMSSW_7_4_1_patch4

cd CMSSW_7_4_1_patch4/src 

cmsenv

git cms-addpkg PhysicsTools/PatAlgos

git cms-merge-topic -u CMS-PAT-Tutorial:CMSSW_7_1_0_patTutorial

scram b

2) replace PhysicsTools/PatExamples/test/producePatTrigger_cfg.py with this new file producePatTrigger_cfg.py

replace PhysicsTools/PatExamples/test/analyzePatTrigger_cfg.py with file of same name in this folder

replace PhysicsTools/PatExamples/plugins/PatTriggerAnalyzer.cc with file of same name in this folder

put sourceFiles.txt file in CMSSW_7_4_1_patch4/src folder

cd CMSSW_7_4_1_patch4/src

scram b

3)

cmsRun PhysicsTools/PatExamples/test/producePatTrigger_cfg.py

you will get a file with name edmPatTrigger.root

then cmsRun PhysicsTools/PatExamples/test/analyzePatTrigger_cfg.py

you can get analyzePatTrigger.root

then you root

TBrowser i 

To have a look at plots.


