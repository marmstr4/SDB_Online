//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#ifndef SDBEVENT_H
#define SDBEVENT_H

#include "Riostream.h"
#include "Configuration_Files/Setup_File.h"

#include "TGo4EventElement.h"

class SDBEvent: public TGo4EventElement {
 public:
  SDBEvent();
  SDBEvent(const char * name);
  ~SDBEvent();

  virtual void Clear(Option_t * t = "");
      
  Int_t fbPlasDetNum_Fast;
  Int_t fbPlasDetNum_Slow;
  Int_t fbPlas_FastChan[bPLASTIC_TAMEX_MODULES + 1];
  Int_t fbPlas_SlowChan[bPLASTIC_TAMEX_MODULES + 1];
  Int_t fbPlast_Fast_Lead_N[bPLASTIC_TAMEX_MODULES + 1][bPLASTIC_CHAN_PER_DET];
  Int_t fbPlast_Slow_Lead_N[bPLASTIC_TAMEX_MODULES + 1][bPLASTIC_CHAN_PER_DET];
  Int_t fbPlast_Fast_Trail_N[bPLASTIC_TAMEX_MODULES + 1][bPLASTIC_CHAN_PER_DET];
  Int_t fbPlast_Slow_Trail_N[bPLASTIC_TAMEX_MODULES + 1][bPLASTIC_CHAN_PER_DET];
  Double_t fbPlast_Fast_Lead[bPLASTIC_TAMEX_MODULES + 1][bPLASTIC_CHAN_PER_DET][bPLASTIC_TAMEX_HITS];
  Double_t fbPlast_Slow_Lead[bPLASTIC_TAMEX_MODULES + 1][bPLASTIC_CHAN_PER_DET][bPLASTIC_TAMEX_HITS];
  Double_t fbPlast_Fast_Trail[bPLASTIC_TAMEX_MODULES + 1][bPLASTIC_CHAN_PER_DET][bPLASTIC_TAMEX_HITS];
  Double_t fbPlast_Slow_Trail[bPLASTIC_TAMEX_MODULES + 1][bPLASTIC_CHAN_PER_DET][bPLASTIC_TAMEX_HITS];
 
  
  
  //bPlas TAMEX 
  Int_t fbPlasDetNum;
  Int_t fTrigger;
  Int_t fevent_number;
  Int_t fbPlasChan[bPLASTIC_TAMEX_MODULES + 1];
  Int_t fbPlas_PMT_Lead_N[bPLASTIC_TAMEX_MODULES + 1][bPLASTIC_CHAN_PER_DET];
  Int_t fbPlas_PMT_Trail_N[bPLASTIC_TAMEX_MODULES + 1][bPLASTIC_CHAN_PER_DET];
  Double_t fbPlas_Lead_PMT[bPLASTIC_TAMEX_MODULES + 1][bPLASTIC_CHAN_PER_DET][bPLASTIC_TAMEX_HITS];
  Double_t fbPlas_Trail_PMT[bPLASTIC_TAMEX_MODULES + 1][bPLASTIC_CHAN_PER_DET][bPLASTIC_TAMEX_HITS];

  // VME raw data
  Int_t VME_ADC[VME_NADC][VME_ADC_CHAN_NUM];
  Int_t VME_TDC[VME_TDC_CHAN_NUM];
  Int_t VME_QDC[VME_QDC_CHAN_NUM];

    //Febex raw data
  Int_t Febex_raw_E[Febex_chan_num];
  Int_t Febex_raw_T[Febex_chan_num];
  
  ClassDef(SDBEvent, 1)
};
#endif 
