//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#ifndef SDBANAEVENT_H
#define SDBANAEVENT_H

#include "Configuration_Files/Setup_File.h"

#include "TGo4EventElement.h"

class SDBAnaEvent : public TGo4EventElement {
public:
    SDBAnaEvent();

    SDBAnaEvent(const char *name);

    ~SDBAnaEvent();

    virtual void Clear(Option_t *t = "");

    //MCP data
    Int_t GSI_MCP1_X;
    Int_t GSI_MCP1_Y;
    Int_t GSI_MCP1_T;
    Int_t GSI_MCP2_X;
    Int_t GSI_MCP2_Y;
    Int_t GSI_MCP2_T;

    Int_t Koeln_MCP_X1;
    Int_t Koeln_MCP_Y1;
    Int_t Koeln_MCP_T1;
    Int_t Koeln_MCP_X2;
    Int_t Koeln_MCP_Y2;
    Int_t Koeln_MCP_T2;

    Int_t Koeln_MCP_X;
    Int_t Koeln_MCP_Y;
    Int_t Koeln_MCP_T;

    Int_t TOF_GSI_MCP1_to_GSI_MCP2;
    Int_t TOF_GSI_MCP1_to_Koeln_MCP;
    Int_t TOF_GSI_MCP2_to_Koeln_MCP;

    //Ge calibrated data
    Int_t Ge_calib_E[Febex_chan_num];
    Int_t Ge_calib_T[Febex_chan_num];

    Float_t deltaE_X[DSSD_CHAN_NUM];
    Float_t deltaE_Y[DSSD_CHAN_NUM];
    Float_t Eres_X[DSSD_CHAN_NUM];
    Float_t Eres_Y[DSSD_CHAN_NUM];
    Int_t mult_deltaE_X;
    Int_t mult_deltaE_Y;
    Int_t mult_Eres_X;
    Int_t mult_Eres_Y;
    Float_t sum_deltaE_X;
    Float_t sum_deltaE_Y;
    Float_t sum_Eres_X;
    Float_t sum_Eres_Y;

    ClassDef(SDBAnaEvent,
    1)
};

#endif
