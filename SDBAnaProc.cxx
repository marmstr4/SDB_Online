//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#include "SDBAnaProc.h"
#include "stdint.h"

#include "Riostream.h"

#include "TH1.h"
#include "TH2.h"
#include "TCutG.h"
#include "snprintf.h"

#include "TGo4MbsEvent.h"
#include "TGo4WinCond.h"
#include "TGo4PolyCond.h"
#include "TGo4CondArray.h"
#include "TGo4Picture.h"


//***********************************************************
SDBAnaProc::SDBAnaProc() : TGo4EventProcessor("Proc") {
    std::cout << "**** SDBAnaProc: Create instance " << std::endl;

}

//***********************************************************
SDBAnaProc::~SDBAnaProc() {
    std::cout << "**** SDBAnaProc: Delete instance " << std::endl;
}

//***********************************************************
// this one is used in standard factory
SDBAnaProc::SDBAnaProc(const char *name) : TGo4EventProcessor(name) {
    std::cout << "**** SDBAnaProc: Create instance " << name << std::endl;
    Load_TDCfile();
    Load_QDCfile();
    Load_ADCfile();
    Load_Ge_calib_file();
    Make_MCP_Histos();
    Make_DSSD_Histos();
    Make_Ge_Histos();

}

void SDBAnaProc::Load_TDCfile() {
    const char *format = "%d %d %c";
    ifstream data("Configuration_Files/MCP_TDC_config.txt");
    if (data.fail()) {
        cerr << "Could not find TDC_config file!" << endl;
        exit(0);
    }

    for (int i = 0; i < NMCP; i++) {
        MCP_X_TDCchn[i] = -1;
        MCP_Y_TDCchn[i] = -1;
        MCP_T_TDCchn[i] = -1;
    }
    int mcp, chn;
    char xyt;
    string line;
    while (data.good()) {
        getline(data, line, '\n');
        if (line[0] == '#')
            continue;
        sscanf(line.c_str(), format, &chn, &mcp, &xyt);
        //cout << mcp << " " << xyt << " " << chn << endl;
        if (xyt == 'X' || xyt == 'x')
            MCP_X_TDCchn[mcp] = chn;
        if (xyt == 'Y' || xyt == 'y')
            MCP_Y_TDCchn[mcp] = chn;
        if (xyt == 'T' || xyt == 't')
            MCP_T_TDCchn[mcp] = chn;
    }

    // for DSSDs
    format = "%d %d %c %d";
    ifstream data2("Configuration_Files/DSSD_TDC_config.txt");
    if (data2.fail()) {
        cerr << "Could not find DSSD_TDC_config file!" << endl;
        exit(0);
    }
    for (int i = 0; i < NDSSD; i++) {
        for (int k = 0; k < 2; k++) {
            for (int j = 0; j < DSSD_CHAN_NUM; j++) {
                DSSD_TDCchn[i][k][j] = -1;
            }
        }
    }
    int det, strip;
    char xy;
    while (data2.good()) {
        getline(data2, line, '\n');
        if (line[0] == '#')
            continue;
        sscanf(line.c_str(), format, &chn, &det, &xy, &strip);
        int ixy = -1;
        if (xy == 'X' || xy == 'x')
            ixy = 0;
        else if (xy == 'Y' || xy == 'y')
            ixy = 1;
        if (ixy < 0) {
            cout << "problem reading line:" << endl;
            cout << line << endl;
            return;
        }

        DSSD_TDCchn[det][ixy][strip] = chn;
    }


    return;
}

//load QDC mappings
void SDBAnaProc::Load_QDCfile() {
    const char *format = "%d %d";
    ifstream data("Configuration_Files/QDC_config.txt");
    if (data.fail()) {
        cerr << "Could not find DDC_config file!" << endl;
        exit(0);
    }
    for (int i = 0; i < NMCP; i++)
        MCP_QDCchn[i] = -1;
    int mcp, chn;
    char xyt;
    string line;
    while (data.good()) {
        getline(data, line, '\n');
        if (line[0] == '#')
            continue;
        sscanf(line.c_str(), format, &chn, &mcp);
        //cout << mcp << " " << chn << endl;
        MCP_QDCchn[mcp] = chn;
    }


}

//load ADC mappings
void SDBAnaProc::Load_ADCfile() {

    const char *format = "%d %d %d %c %d %f %f %f";
    ifstream data("Configuration_Files/ADC_config.txt");
    if (data.fail()) {
        cerr << "Could not find DSSD_config file!" << endl;
        exit(0);
    }

    for (int i = 0; i < NDSSD; i++) {
        for (int k = 0; k < 2; k++) {
            for (int j = 0; j < DSSD_CHAN_NUM; j++) {
                DSSD_ADCmod[i][k][j] = -1;
                DSSD_ADCchn[i][k][j] = -1;
                DSSD_gain[i][k][j] = 1;
                DSSD_offs[i][k][j] = 0;
                DSSD_thresh[i][k][j] = 0;
            }
        }
    }

    int ADC, chn, det, strip;
    char xy;
    float gain, offset, thresh;
    string line;
    while (data.good()) {
        getline(data, line, '\n');
        if (line[0] == '#')
            continue;
        sscanf(line.c_str(), format, &ADC, &chn, &det, &xy, &strip, &gain, &offset, &thresh);
        int ixy = -1;
        if (xy == 'X' || xy == 'x')
            ixy = 0;
        else if (xy == 'Y' || xy == 'y')
            ixy = 1;
        if (ixy < 0) {
            cout << "problem reading line:" << endl;
            cout << line << endl;
            return;
        }

        DSSD_ADCmod[det][ixy][strip] = ADC;
        DSSD_ADCchn[det][ixy][strip] = chn;
        DSSD_gain[det][ixy][strip] = gain;
        DSSD_offs[det][ixy][strip] = offset;
        DSSD_thresh[det][ixy][strip] = thresh;

    }

}

//load germanium calibration coefficients
void SDBAnaProc::Load_Ge_calib_file() {

    double coeff0 = 0, coeff1 = 0;
    int chan = 0;
    ifstream data("Configuration_Files/Ge_calib_file.txt");

    if (data.fail()) {
        cerr << "Could not find DDC_config file!" << endl;
        exit(0);
    }
    while (!data.eof()) {
        data >> coeff0;
        data >> coeff1;
        data >> chan;
        Ge_E_calib_coeff_0[chan] = coeff0;
        Ge_E_calib_coeff_1[chan] = coeff1;
    }

}



Bool_t
SDBAnaProc::BuildEvent(TGo4EventElement *output) {  // called by framework. We dont fill any output event here at all
    SDBAnaEvent *tgt = (SDBAnaEvent *) output;
    if (tgt == nullptr)
        return kFALSE;

    tgt->SetValid(kTRUE);  // events always accepted
    SDBEvent *src = (SDBEvent *) GetInputEvent();

    // process the DSSDs here
    ProcessDSSD(src, tgt);

    // process the MCP's here
    ProcessMCP(src, tgt);
    
    // process DEGAS here
    ProcessGe(src, tgt);

    // Fill the histograms
    if (HISTOS) {
        Fill_DSSD_Histos(tgt);
        Fill_MCP_Histos(tgt);
        Fill_Ge_Histos(tgt);
    }

    return kTRUE;
}

Bool_t SDBAnaProc::ProcessDSSD(SDBEvent *input, SDBAnaEvent *output) {
    //cout << __PRETTY_FUNCTION__ << endl;

    //cout << input->VME_ADC1[0] << endl;
    float es;
    int mult[NDSSD][2];
    float sum[NDSSD][2];

    for (int d = 0; d < NDSSD; d++) {
        for (int x = 0; x < 2; x++) {
            mult[d][x] = 0;
            sum[d][x] = 0;
            for (int s = 0; s < DSSD_CHAN_NUM; s++) {
                es = input->VME_ADC[d][DSSD_ADCchn[d][x][s]];
                es = (es - DSSD_offs[d][x][s]) * DSSD_gain[d][x][s];
                if (es > DSSD_thresh[d][x][s]) {
                    mult[d][x] += 1;
                    sum[d][x] += es;
                    if (d == 0 && x == 0)
                        output->deltaE_X[s] = es;
                    if (d == 0 && x == 1)
                        output->deltaE_Y[s] = es;
                    if (d == 1 && x == 0)
                        output->Eres_X[s] = es;
                    if (d == 1 && x == 1)
                        output->Eres_Y[s] = es;
                }// threshold
            }//channels
        }// x/y
    }//dssds

    output->mult_deltaE_X = mult[0][0];
    output->mult_deltaE_Y = mult[0][1];
    output->mult_Eres_X = mult[1][0];
    output->mult_Eres_Y = mult[1][1];

    output->sum_deltaE_X = sum[0][0];
    output->sum_deltaE_Y = sum[0][1];
    output->sum_Eres_X = sum[1][0];
    output->sum_Eres_Y = sum[1][1];

    return kTRUE;
}

void SDBAnaProc::Make_DSSD_Histos() {
  
    h_deltaE_Xstrips = new TH2F("h_deltaE_Xstrips", "h_deltaE_Xstrips", DSSD_CHAN_NUM, 0, DSSD_CHAN_NUM, 1000, 0, 1000);
    AddHistogram(h_deltaE_Xstrips, "DSSD_Ana");
    h_deltaE_Ystrips = new TH2F("h_deltaE_Ystrips", "h_deltaE_Ystrips", DSSD_CHAN_NUM, 0, DSSD_CHAN_NUM, 1000, 0, 1000);
    AddHistogram(h_deltaE_Ystrips, "DSSD_Ana");
    h_Eres_Xstrips = new TH2F("h_Eres_Xstrips", "h_Eres_Xstrips", DSSD_CHAN_NUM, 0, DSSD_CHAN_NUM, 1000, 0, 5000);
    AddHistogram(h_Eres_Xstrips, "DSSD_Ana");
    h_Eres_Ystrips = new TH2F("h_Eres_Ystrips", "h_Eres_Ystrips", DSSD_CHAN_NUM, 0, DSSD_CHAN_NUM, 1000, 0, 5000);
    AddHistogram(h_Eres_Ystrips, "DSSD_Ana");
    
}

void SDBAnaProc::Fill_DSSD_Histos(SDBAnaEvent *output) {
    for (int s = 0; s < DSSD_CHAN_NUM; s++) {
        if (!isnan(output->deltaE_X[s]))
            h_deltaE_Xstrips->Fill(s, output->deltaE_X[s]);
        if (!isnan(output->deltaE_Y[s]))
            h_deltaE_Ystrips->Fill(s, output->deltaE_Y[s]);
        if (!isnan(output->Eres_X[s]))
            h_Eres_Xstrips->Fill(s, output->Eres_X[s]);
        if (!isnan(output->Eres_Y[s]))
            h_Eres_Ystrips->Fill(s, output->Eres_Y[s]);
    }
}

//----------------------------END OF GO4 SOURCE FILE ---------------------
Bool_t SDBAnaProc::ProcessMCP(SDBEvent *input, SDBAnaEvent *output) {

    //Extract MCP x,y,t
    output->GSI_MCP1_X = input->VME_TDC[MCP_X_TDCchn[0]];
    output->GSI_MCP1_Y = input->VME_TDC[MCP_Y_TDCchn[0]];
    output->GSI_MCP1_T = input->VME_TDC[MCP_Y_TDCchn[0]];
    output->GSI_MCP2_X = input->VME_TDC[MCP_X_TDCchn[1]];
    output->GSI_MCP2_Y = input->VME_TDC[MCP_Y_TDCchn[1]];
    output->GSI_MCP2_T = input->VME_TDC[MCP_T_TDCchn[1]];

    output->Koeln_MCP_X1 = input->VME_TDC[MCP_X_TDCchn[2]];
    output->Koeln_MCP_Y1 = input->VME_TDC[MCP_Y_TDCchn[2]];
    output->Koeln_MCP_T1 = input->VME_TDC[MCP_Y_TDCchn[2]];
    output->Koeln_MCP_X2 = input->VME_TDC[MCP_X_TDCchn[3]];
    output->Koeln_MCP_Y2 = input->VME_TDC[MCP_Y_TDCchn[3]];
    output->Koeln_MCP_T2 = input->VME_TDC[MCP_T_TDCchn[3]];

    output->Koeln_MCP_X = output->Koeln_MCP_X2 - output->Koeln_MCP_X1;
    output->Koeln_MCP_Y = output->Koeln_MCP_Y2 - output->Koeln_MCP_Y1;
    output->Koeln_MCP_T = output->Koeln_MCP_T2;


    if(output->GSI_MCP2_T>500 && output->GSI_MCP1_T > 500 ) {
      output->TOF_GSI_MCP1_to_GSI_MCP2 = output->GSI_MCP2_T - output->GSI_MCP1_T;
    output->TOF_GSI_MCP1_to_Koeln_MCP = output->GSI_MCP1_T - output->Koeln_MCP_T;
    output->TOF_GSI_MCP2_to_Koeln_MCP = output->GSI_MCP2_T - output->Koeln_MCP_T;

   }

    return kTRUE;
}

void SDBAnaProc::Fill_MCP_Histos(SDBAnaEvent *output) {
    //Fill Histos
    h_GSI_MCP1_X->Fill(output->GSI_MCP1_X);
    h_GSI_MCP1_Y->Fill(output->GSI_MCP1_Y);
    h_GSI_MCP1_T->Fill(output->GSI_MCP1_T);
    h_GSI_MCP2_X->Fill(output->GSI_MCP2_X);
    h_GSI_MCP2_Y->Fill(output->GSI_MCP2_Y);
    h_GSI_MCP2_T->Fill(output->GSI_MCP2_T);

    h_Koeln_MCP_X1->Fill(output->Koeln_MCP_X1);
    h_Koeln_MCP_Y1->Fill(output->Koeln_MCP_Y1);
    h_Koeln_MCP_T1->Fill(output->Koeln_MCP_T1);
    h_Koeln_MCP_X2->Fill(output->Koeln_MCP_X2);
    h_Koeln_MCP_Y2->Fill(output->Koeln_MCP_Y2);
    h_Koeln_MCP_T2->Fill(output->Koeln_MCP_T2);

    h_Koeln_MCP_X->Fill(output->Koeln_MCP_X);
    h_Koeln_MCP_Y->Fill(output->Koeln_MCP_Y);
    h_Koeln_MCP_T->Fill(output->Koeln_MCP_T);

    // 2D correlation histograms for all parameter combinations

    h_GSI_MCP1_X_Y->Fill(output->GSI_MCP1_X, output->GSI_MCP1_Y);
    h_GSI_MCP1_X_T->Fill(output->GSI_MCP1_X, output->GSI_MCP1_T);
    h_GSI_MCP1_Y_T->Fill(output->GSI_MCP1_Y, output->GSI_MCP1_T);

    h_GSI_MCP2_X_Y->Fill(output->GSI_MCP2_X, output->GSI_MCP2_Y);
    h_GSI_MCP2_X_T->Fill(output->GSI_MCP2_X, output->GSI_MCP2_T);
    h_GSI_MCP2_Y_T->Fill(output->GSI_MCP2_Y, output->GSI_MCP2_T);

    h_GSI_MCP_X_correlation->Fill(output->GSI_MCP1_X, output->GSI_MCP2_X);
    h_GSI_MCP_Y_correlation->Fill(output->GSI_MCP1_Y, output->GSI_MCP2_Y);
    h_GSI_MCP_T_correlation->Fill(output->GSI_MCP1_T, output->GSI_MCP2_T);

    h_Koeln_MCP_X1_Y1->Fill(output->Koeln_MCP_X1, output->Koeln_MCP_Y1);
    h_Koeln_MCP_X1_T1->Fill(output->Koeln_MCP_X1, output->Koeln_MCP_T1);
    h_Koeln_MCP_Y1_T1->Fill(output->Koeln_MCP_Y1, output->Koeln_MCP_T1);

    h_Koeln_MCP_X2_Y2->Fill(output->Koeln_MCP_X2, output->Koeln_MCP_Y2);
    h_Koeln_MCP_X2_T2->Fill(output->Koeln_MCP_X2, output->Koeln_MCP_T2);
    h_Koeln_MCP_Y2_T2->Fill(output->Koeln_MCP_Y2, output->Koeln_MCP_T2);

    h_Koeln_MCP_X1_X2->Fill(output->Koeln_MCP_X1, output->Koeln_MCP_X2);
    h_Koeln_MCP_Y1_Y2->Fill(output->Koeln_MCP_Y1, output->Koeln_MCP_Y2);
    h_Koeln_MCP_T1_T2->Fill(output->Koeln_MCP_T1, output->Koeln_MCP_T2);

    h_Koeln_MCP_X_Y->Fill(output->Koeln_MCP_X, output->Koeln_MCP_Y);
    h_Koeln_MCP_X_T->Fill(output->Koeln_MCP_X, output->Koeln_MCP_T);
    h_Koeln_MCP_Y_T->Fill(output->Koeln_MCP_Y, output->Koeln_MCP_T);

    // TOF: from GSI MCP_1 to MCP_2, from GSI MCP 1 or 2 to Koeln MCP

    h_TOF_GSI_MCP1_to_GSI_MCP2->Fill(output->TOF_GSI_MCP1_to_GSI_MCP2);
    h_TOF_GSI_MCP1_to_Koeln_MCP->Fill(output->TOF_GSI_MCP1_to_Koeln_MCP);
    h_TOF_GSI_MCP2_to_Koeln_MCP->Fill(output->TOF_GSI_MCP2_to_Koeln_MCP);
    
    //dE-TOF correlations
    
    for (int s = 0; s < DSSD_CHAN_NUM; s++) {
      
      if (!isnan(output->deltaE_X[s]))
            h_dE_TOF_all->Fill(output->deltaE_X[s],output->TOF_GSI_MCP1_to_GSI_MCP2);
	    h_dE_TOF_X[s]->Fill(output->deltaE_X[s],output->TOF_GSI_MCP1_to_GSI_MCP2);
      
      if (!isnan(output->deltaE_Y[s]))
            h_dE_TOF_all->Fill(output->deltaE_Y[s],output->TOF_GSI_MCP1_to_GSI_MCP2);
	    h_dE_TOF_Y[s]->Fill(output->deltaE_Y[s],output->TOF_GSI_MCP1_to_GSI_MCP2);
	
    }

}

void SDBAnaProc::Make_MCP_Histos() {

    h_GSI_MCP1_X = new TH1F("h_GSI_MCP1_X", "h_GSI_MCP1_X", 1000, -50000, 50000);
    AddHistogram(h_GSI_MCP1_X, "GSI_MCP1_Ana");
    h_GSI_MCP1_Y = new TH1F("h_GSI_MCP1_Y", "h_GSI_MCP1_Y", 1000, -50000, 50000);
    AddHistogram(h_GSI_MCP1_Y, "GSI_MCP1_Ana");
    h_GSI_MCP1_T = new TH1F("h_GSI_MCP1_T", "h_GSI_MCP1_T", 1000, -50000, 50000);
    AddHistogram(h_GSI_MCP1_T, "GSI_MCP1_Ana");
    h_GSI_MCP2_X = new TH1F("h_GSI_MCP2_X", "h_GSI_MCP2_X", 1000, -50000, 50000);
    AddHistogram(h_GSI_MCP2_X, "GSI_MCP2_Ana");
    h_GSI_MCP2_Y = new TH1F("h_GSI_MCP2_Y", "h_GSI_MCP2_Y", 1000, -50000, 50000);
    AddHistogram(h_GSI_MCP2_Y, "GSI_MCP2_Ana");
    h_GSI_MCP2_T = new TH1F("h_GSI_MCP2_T", "h_GSI_MCP2_T", 1000, -50000, 50000);
    AddHistogram(h_GSI_MCP2_T, "GSI_MCP2_Ana");

    h_Koeln_MCP_X1 = new TH1F("h_Koeln_MCP_X1", "h_Koeln_MCP_X1", 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_X1, "Koeln_MCP_Ana");
    h_Koeln_MCP_Y1 = new TH1F("h_Koeln_MCP_Y1", "h_Koeln_MCP_Y1", 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_Y1, "Koeln_MCP_Ana");
    h_Koeln_MCP_T1 = new TH1F("h_Koeln_MCP_T1", "h_Koeln_MCP_T1", 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_T1, "Koeln_MCP_Ana");
    h_Koeln_MCP_X2 = new TH1F("h_Koeln_MCP_X2", "h_Koeln_MCP_X2", 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_X2, "Koeln_MCP_Ana");
    h_Koeln_MCP_Y2 = new TH1F("h_Koeln_MCP_Y2", "h_Koeln_MCP_Y2", 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_Y2, "Koeln_MCP_Ana");
    h_Koeln_MCP_T2 = new TH1F("h_Koeln_MCP_T2", "h_Koeln_MCP_T2", 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_T2, "Koeln_MCP_Ana");
    h_Koeln_MCP_X = new TH1F("h_Koeln_MCP_X", "h_Koeln_MCP_X", 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_X, "Koeln_MCP_Ana");
    h_Koeln_MCP_Y = new TH1F("h_Koeln_MCP_Y", "h_Koeln_MCP_Y", 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_Y, "Koeln_MCP_Ana");
    h_Koeln_MCP_T = new TH1F("h_Koeln_MCP_T", "h_Koeln_MCP_T", 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_T, "Koeln_MCP_Ana");

    h_GSI_MCP1_X_Y = new TH2F("h_GSI_MCP1_X_Y", "h_GSI_MCP1_X_Y", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_GSI_MCP1_X_Y, "GSI_MCP1_Ana");
    h_GSI_MCP1_X_T = new TH2F("h_GSI_MCP1_X_T", "h_GSI_MCP1_X_T", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_GSI_MCP1_X_T, "GSI_MCP1_Ana");
    h_GSI_MCP1_Y_T = new TH2F("h_GSI_MCP1_Y_T", "h_GSI_MCP1_Y_T", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_GSI_MCP1_Y_T, "GSI_MCP1_Ana");

    h_GSI_MCP2_X_Y = new TH2F("h_GSI_MCP2_X_Y", "h_GSI_MCP2_X_Y", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_GSI_MCP2_X_Y, "GSI_MCP2_Ana");
    h_GSI_MCP2_X_T = new TH2F("h_GSI_MCP2_X_T", "h_GSI_MCP2_X_T", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_GSI_MCP2_X_T, "GSI_MCP2_Ana");
    h_GSI_MCP2_Y_T = new TH2F("h_GSI_MCP2_Y_T", "h_GSI_MCP2_Y_T", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_GSI_MCP2_Y_T, "GSI_MCP2_Ana");

    h_GSI_MCP_X_correlation = new TH2F("h_GSI_MCP_X_correlation", "h_GSI_MCP_X_correlation", 1000, -50000, 50000, 1000,
                                       -50000, 50000);
    AddHistogram(h_GSI_MCP_X_correlation, "GSI_MCP_Ana");
    h_GSI_MCP_Y_correlation = new TH2F("h_GSI_MCP_Y_correlation", "h_GSI_MCP_Y_correlation", 1000, -50000, 50000, 1000,
                                       -50000, 50000);
    AddHistogram(h_GSI_MCP_Y_correlation, "GSI_MCP_Ana");
    h_GSI_MCP_T_correlation = new TH2F("h_GSI_MCP_T_correlation", "h_GSI_MCP_T_correlation", 1000, -50000, 50000, 1000,
                                       -50000, 50000);
    AddHistogram(h_GSI_MCP_T_correlation, "GSI_MCP_Ana");

    h_Koeln_MCP_X1_Y1 = new TH2F("h_Koeln_MCP_X1_Y1", "h_Koeln_MCP_X1_Y1", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_X1_Y1, "Koeln_MCP_Ana");
    h_Koeln_MCP_X1_T1 = new TH2F("h_Koeln_MCP_X1_T1", "h_Koeln_MCP_X1_T1", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_X1_T1, "Koeln_MCP_Ana");
    h_Koeln_MCP_Y1_T1 = new TH2F("h_Koeln_MCP_Y1_T1", "h_Koeln_MCP_Y1_T1", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_Y1_T1, "Koeln_MCP_Ana");

    h_Koeln_MCP_X2_Y2 = new TH2F("h_Koeln_MCP_X2_Y2", "h_Koeln_MCP_X2_Y2", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_X2_Y2, "Koeln_MCP_Ana");
    h_Koeln_MCP_X2_T2 = new TH2F("h_Koeln_MCP_X2_T2", "h_Koeln_MCP_X2_T2", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_X2_T2, "Koeln_MCP_Ana");
    h_Koeln_MCP_Y2_T2 = new TH2F("h_Koeln_MCP_Y2_T2", "h_Koeln_MCP_Y2_T2", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_Y2_T2, "Koeln_MCP_Ana");

    h_Koeln_MCP_X1_X2 = new TH2F("h_Koeln_MCP_X1_X2", "h_Koeln_MCP_X1_X2", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_X1_X2, "Koeln_MCP_Ana");
    h_Koeln_MCP_Y1_Y2 = new TH2F("h_Koeln_MCP_Y1_Y2", "h_Koeln_MCP_Y1_Y2", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_Y1_Y2, "Koeln_MCP_Ana");
    h_Koeln_MCP_T1_T2 = new TH2F("h_Koeln_MCP_T1_T2", "h_Koeln_MCP_T1_T2", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_T1_T2, "Koeln_MCP_Ana");

    h_Koeln_MCP_X_Y = new TH2F("h_Koeln_MCP_X_Y", "h_Koeln_MCP_X_Y", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_X_Y, "Koeln_MCP_Ana");
    h_Koeln_MCP_X_T = new TH2F("h_Koeln_MCP_X_T", "h_Koeln_MCP_X_T", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_X_T, "Koeln_MCP_Ana");
    h_Koeln_MCP_Y_T = new TH2F("h_Koeln_MCP_Y_T", "h_Koeln_MCP_Y_T", 1000, -50000, 50000, 1000, -50000, 50000);
    AddHistogram(h_Koeln_MCP_Y_T, "Koeln_MCP_Ana");

    h_TOF_GSI_MCP1_to_GSI_MCP2 = new TH1F("h_TOF_GSI_MCP1_to_GSI_MCP2", "h_TOF_GSI_MCP1_to_GSI_MCP2", 100000, -50000, 50000);
    AddHistogram(h_TOF_GSI_MCP1_to_GSI_MCP2, "TOF_Ana");
    h_TOF_GSI_MCP1_to_Koeln_MCP = new TH1F("h_TOF_GSI_MCP1_to_Koeln_MCP", "h_TOF_GSI_MCP1_to_Koeln_MCP", 100000, -50000, 50000);
    AddHistogram(h_TOF_GSI_MCP1_to_Koeln_MCP, "TOF_Ana");
    h_TOF_GSI_MCP2_to_Koeln_MCP = new TH1F("h_TOF_GSI_MCP2_to_Koeln_MCP", "h_TOF_GSI_MCP2_to_Koeln_MCP", 100000, -50000, 50000);
    AddHistogram(h_TOF_GSI_MCP2_to_Koeln_MCP, "TOF_Ana");
    
    h_dE_TOF_all = new TH2F("h_dE_TOF_all", "h_dE_TOF_all", 4000, -2000, 2000,4000, -2000, 2000);
   
    for (int i = 0; i < DSSD_CHAN_NUM; i++) {
        snprintf(chis, 15, "h_dE_TOF_X%02d", i + 1);
        snprintf(chead, 63, "h_dE_TOF_X%2d", i + 1);
        h_dE_TOF_X[i] = new TH2F(chis, chead, 4000,-2000, 2000, 4000, -2000, 2000);
        AddHistogram(h_dE_TOF_X[i], "correl");
    }
    
    for (int i = 0; i < DSSD_CHAN_NUM; i++) {
        snprintf(chis, 15, "h_dE_TOF_Y%02d", i + 1);
        snprintf(chead, 63, "h_dE_TOF_Y%2d", i + 1);
        h_dE_TOF_Y[i] = new TH2F(chis, chead, 4000, -2000, 2000, 4000, -2000, 2000);
        AddHistogram(h_dE_TOF_Y[i], "correl");
    }
    
    

}

Bool_t SDBAnaProc::ProcessGe(SDBEvent *input, SDBAnaEvent *output) {

    for (int i = 0; i < Febex_chan_num; i++) {
        output->Ge_calib_E[i] = input->Febex_raw_E[i] * Ge_E_calib_coeff_1[i] + Ge_E_calib_coeff_0[i];
    }

    return kTRUE;
}

void SDBAnaProc::Make_Ge_Histos() {

    //Calibrated germanium energy histograms
    for (int i = 0; i < Febex_chan_num; i++) {
        snprintf(chis, 15, "Germanium_E%02d", i + 1);
        snprintf(chead, 63, "Germanium_E%2d", i + 1);
        h_fFebex_E[i] = new TH1F(chis, chead, 4000, 0, 4000);
        AddHistogram(h_fFebex_E[i], "Germanium_calib");
    }

}

void SDBAnaProc::Fill_Ge_Histos(SDBAnaEvent *output) {

    for (int i = 0; i < Febex_chan_num; i++) {
        h_fFebex_E[i]->Fill(output->Ge_calib_E[i]);
    }
}

