//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#include "SDBAnalysis.h"

#include <stdlib.h>
#include "Riostream.h"

#include "TH1.h"
#include "TFile.h"
#include "TSystem.h"

#include "TGo4AnalysisStep.h"
#include "TGo4StepFactory.h"
#include "TGo4Version.h"
#include "Go4EventServer.h"


//***********************************************************
SDBAnalysis::SDBAnalysis() :
   TGo4Analysis(){}

  SDBAnalysis::SDBAnalysis(int argc, char** argv) :
  TGo4Analysis(argc, argv), fMbsEvent(0){

  //Create step 1: Raw data handling
  TGo4StepFactory* factory1 = new TGo4StepFactory("Unpack-factory");
  factory1->DefEventProcessor("SDBProc", "SDBProc");                                
  factory1->DefOutputEvent("SDBEvent", "SDBEvent");                                     
  TGo4AnalysisStep* step1 = new TGo4AnalysisStep("Unpack",factory1,0,0,0);
  step1->SetSourceEnabled(kTRUE);
  step1->SetStoreEnabled(kTRUE);  // enable output
  step1->SetProcessEnabled(kTRUE);
  step1->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step1);

  // Create step 2: Calibration, mapping and Main Monitoring histograms
  
  TGo4StepFactory* factory2 = new TGo4StepFactory("Analysis-factory");
  factory2->DefInputEvent("SDBEvent", "SDBEvent");                                   
  factory2->DefEventProcessor("SDBAnaProc", "SDBAnaProc"); 
  factory2->DefOutputEvent("SDBAnaEvent", "SDBAnaEvent"); 
  TGo4AnalysisStep* step2 = new TGo4AnalysisStep("Analysis",factory2,0,0,0);
  step2->SetSourceEnabled(kTRUE);
  step2->SetStoreEnabled(kTRUE);  
  step2->SetProcessEnabled(kTRUE);
  step2->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step2);
}
//***********************************************************
SDBAnalysis::~SDBAnalysis()
{
  cout << "**** SDBAnalysis: Delete" << endl;
}
