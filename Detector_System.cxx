//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#ifndef DETECTOR_SYSTEM_H
#define DETECTOR_SYSTEM_H 

#include "Raw_Event.h"


#include <string>

#include "TGo4UserException.h"
#include "TGo4Picture.h"
#include "TGo4MbsEvent.h"
#include "TGo4EventProcessor.h"


class Detector_System{

public:

    virtual ~Detector_System(){};

    virtual void Process_MBS(int*) = 0;
    
    virtual void Process_MBS(TGo4MbsSubEvent*) = 0;

    virtual void get_Event_data(Raw_Event*) = 0;
    
    virtual int* get_pdata() = 0;
    
    virtual bool calibration_done() = 0;


};

#endif
