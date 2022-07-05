//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#include "SDBAnaEvent.h"
#define ZERO_ARRAY(x) memset(x, 0, sizeof(x))
SDBAnaEvent::SDBAnaEvent() :TGo4EventElement("SDBAnaEvent") {}
SDBAnaEvent::SDBAnaEvent(const char* name) :TGo4EventElement(name){ }

SDBAnaEvent::~SDBAnaEvent(){ }

void SDBAnaEvent::Clear(Option_t * t) {
  mult_deltaE_X = 0;
  mult_deltaE_Y = 0;
  mult_Eres_X = 0;
  mult_Eres_Y = 0;

  sum_deltaE_X = 0;
  sum_deltaE_Y = 0;
  sum_Eres_X = 0;
  sum_Eres_Y = 0;

  for(int i=0;i<DSSD_CHAN_NUM;i++){
    deltaE_X[i] = sqrt(-1);
    deltaE_Y[i] = sqrt(-1);
    Eres_X[i] = sqrt(-1);
    Eres_Y[i] = sqrt(-1);
  }
}
