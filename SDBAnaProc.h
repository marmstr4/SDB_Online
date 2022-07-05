//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#ifndef TANAPROCESSOR_H
#define TANAPROCESSOR_H

#include "Detector_System.cxx"
#include "PLASTIC_TWINPEAKS_Detector_System.h"
#include "TGo4MbsEvent.h"
#include "CalibParameter.h"
#include "Configuration_Files/Setup_File.h"

#include "TGo4EventProcessor.h"
#include "SDBEvent.h"
#include "SDBAnaEvent.h"

class SDBAnaParam;

class SDBAnaProc : public TGo4EventProcessor {
public:
    SDBAnaProc();

    SDBAnaProc(const char *name);

    virtual ~SDBAnaProc();

    Bool_t BuildEvent(TGo4EventElement *output);

private:
    void Load_ADCfile();

    void Load_TDCfile();

    void Load_QDCfile();

    void Load_Ge_calib_file();

    void Make_DSSD_Histos();

    void Make_MCP_Histos();

    void Make_Ge_Histos();

    Bool_t ProcessDSSD(SDBEvent *input, SDBAnaEvent *output);

    Bool_t ProcessMCP(SDBEvent *input, SDBAnaEvent *output);

    Bool_t ProcessGe(SDBEvent *input, SDBAnaEvent *output);

    void Fill_DSSD_Histos(SDBAnaEvent *output);

    void Fill_MCP_Histos(SDBAnaEvent *output);

    void Fill_Ge_Histos(SDBAnaEvent *output);

    SDBEvent *fInput;

    Int_t DSSD_ADCmod[NDSSD][2][DSSD_CHAN_NUM];
    Int_t DSSD_ADCchn[NDSSD][2][DSSD_CHAN_NUM];
    Float_t DSSD_gain[NDSSD][2][DSSD_CHAN_NUM];
    Float_t DSSD_offs[NDSSD][2][DSSD_CHAN_NUM];
    Float_t DSSD_thresh[NDSSD][2][DSSD_CHAN_NUM];

    Int_t DSSD_TDCchn[NDSSD][2][DSSD_CHAN_NUM];

    Int_t MCP_X_TDCchn[NMCP];
    Int_t MCP_Y_TDCchn[NMCP];
    Int_t MCP_T_TDCchn[NMCP];

    Int_t MCP_QDCchn[NMCP];

    TH1F *h_GSI_MCP1_X;
    TH1F *h_GSI_MCP1_Y;
    TH1F *h_GSI_MCP1_T;
    TH1F *h_GSI_MCP2_X;
    TH1F *h_GSI_MCP2_Y;
    TH1F *h_GSI_MCP2_T;

    TH1F *h_Koeln_MCP_X1;
    TH1F *h_Koeln_MCP_Y1;
    TH1F *h_Koeln_MCP_T1;
    TH1F *h_Koeln_MCP_X2;
    TH1F *h_Koeln_MCP_Y2;
    TH1F *h_Koeln_MCP_T2;

    TH1F *h_Koeln_MCP_X;
    TH1F *h_Koeln_MCP_Y;
    TH1F *h_Koeln_MCP_T;

    TH2F *h_GSI_MCP1_X_Y;
    TH2F *h_GSI_MCP1_X_T;
    TH2F *h_GSI_MCP1_Y_T;

    TH2F *h_GSI_MCP2_X_Y;
    TH2F *h_GSI_MCP2_X_T;
    TH2F *h_GSI_MCP2_Y_T;

    TH2F *h_GSI_MCP_X_correlation;
    TH2F *h_GSI_MCP_Y_correlation;
    TH2F *h_GSI_MCP_T_correlation;

    TH2F *h_Koeln_MCP_X1_Y1;
    TH2F *h_Koeln_MCP_X1_T1;
    TH2F *h_Koeln_MCP_Y1_T1;

    TH2F *h_Koeln_MCP_X2_Y2;
    TH2F *h_Koeln_MCP_X2_T2;
    TH2F *h_Koeln_MCP_Y2_T2;

    TH2F *h_Koeln_MCP_X1_X2;
    TH2F *h_Koeln_MCP_Y1_Y2;
    TH2F *h_Koeln_MCP_T1_T2;

    TH2F *h_Koeln_MCP_X_Y;
    TH2F *h_Koeln_MCP_X_T;
    TH2F *h_Koeln_MCP_Y_T;

    TH1F *h_TOF_GSI_MCP1_to_GSI_MCP2;
    TH1F *h_TOF_GSI_MCP1_to_Koeln_MCP;
    TH1F *h_TOF_GSI_MCP2_to_Koeln_MCP;

    TH2F *h_deltaE_Xstrips;
    TH2F *h_deltaE_Ystrips;
    TH2F *h_Eres_Xstrips;
    TH2F *h_Eres_Ystrips;
    
    TH2F  *h_dE_TOF_all;
    TH2F  *h_dE_TOF_X[DSSD_CHAN_NUM];
    TH2F  *h_dE_TOF_Y[DSSD_CHAN_NUM];

    char chis[40];
    char chead[40];

    TH1F *h_fFebex_E[Febex_chan_num];

    Double_t Ge_E_calib_coeff_0[Febex_chan_num];
    Double_t Ge_E_calib_coeff_1[Febex_chan_num];

    ClassDef(SDBAnaProc,
    1);

};

#endif
