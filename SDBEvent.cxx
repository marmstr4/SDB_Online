//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------
#include "SDBEvent.h"
#define ZERO_ARRAY(x) memset(x, 0, sizeof(x))
SDBEvent::SDBEvent() :TGo4EventElement("SDBEvent") {}
SDBEvent::SDBEvent(const char* name) :TGo4EventElement(name){ }

SDBEvent::~SDBEvent(){ }


void SDBEvent::Clear(Option_t * t) {

  fbPlasDetNum_Fast = 0;
  fbPlasDetNum_Slow = 0;
  ZERO_ARRAY(fbPlas_FastChan);
  ZERO_ARRAY(fbPlas_SlowChan);
  ZERO_ARRAY(fbPlast_Fast_Lead_N);
  ZERO_ARRAY(fbPlast_Slow_Lead_N);
  ZERO_ARRAY(fbPlast_Fast_Trail_N);
  ZERO_ARRAY(fbPlast_Slow_Trail_N);
  ZERO_ARRAY(fbPlast_Fast_Lead);
  ZERO_ARRAY(fbPlast_Slow_Lead);
  ZERO_ARRAY(fbPlast_Fast_Trail);
  ZERO_ARRAY(fbPlast_Slow_Trail);

  ///bPlastic Tamex
  ZERO_ARRAY(fbPlasChan);
  fbPlasDetNum = -1;
  ZERO_ARRAY(fbPlas_PMT_Lead_N);
  ZERO_ARRAY(fbPlas_PMT_Trail_N);
  ZERO_ARRAY(fbPlas_Lead_PMT);
  ZERO_ARRAY(fbPlas_Trail_PMT);

  ZERO_ARRAY(VME_ADC);
  //ZERO_ARRAY(VME_ADC1);
  //ZERO_ARRAY(VME_ADC2);
  ZERO_ARRAY(VME_TDC);
  ZERO_ARRAY(VME_QDC);

  //Febex raw data
  ZERO_ARRAY(Febex_raw_E);
  ZERO_ARRAY(Febex_raw_T);
}
