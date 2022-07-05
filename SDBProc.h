//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#ifndef TUNPACKPROCESSOR_H
#define TUNPACKPROCESSOR_H

#include "Detector_System.cxx"
#include "PLASTIC_TWINPEAKS_Detector_System.h"
#include "Raw_Event.h"
#include "TGo4MbsEvent.h"
#include "Detector_System.cxx"
#include "CalibParameter.h"
#include "Configuration_Files/Setup_File.h"

#include "TGo4EventProcessor.h"

class SDBParam;

class SDBProc : public TGo4EventProcessor {
   public:
      SDBProc() ;
      SDBProc(const char* name);
      virtual ~SDBProc() ;
      virtual void load_bPlasticTamex_Allocationfile();
      Bool_t BuildEvent(TGo4EventElement *target);
      
      void set_DATA_PLASTIC_TWINPEAKS(int * , double ** , double ** , UInt ** , double * , double * , int, int ** );

      void load_PrcID_File();
      bool Used_Systems[10];
      void get_used_systems();
      int TAMEX_bPlast_Det[bPLASTIC_TAMEX_MODULES][bPLASTIC_TAMEX_CHANNELS];
      int TAMEX_bPlast_Chan[bPLASTIC_TAMEX_MODULES][bPLASTIC_TAMEX_CHANNELS];

      Int_t SubType;

      int totalEvents;
      int implantEvents;
      int decayEvents;
      int pulserEvents;
      int nonsenseEvents;
      void PrintDespecParameters();
      void print_MBS(int * , int);
     
      int count;
      int iterator;

      int event_number;
      int fTrigger;
      int fevent_number;
      
      //bPlast Twinpeaks TAMEX
      int iterator_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES];
      double trigger_coarse_bPlastTwinPeaks[100];
      double trigger_fine_bPlastTwinPeaks[100];
      int leading_array_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
      int leading_hits_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES];
      int trailing_hits_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES];
      int phys_channel_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
      int leading_hits_ch_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
      int trailing_hits_ch_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
      Int_t ch_ID_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
      double coarse_T_edge_lead_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
      double coarse_T_edge_trail_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
      double fine_T_edge_lead_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
      double fine_T_edge_trail_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
      bool fired_tamex_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES];
      double get_bPlast_TWINPEAKS_trail_T(int, int);

      protected: 
      
      TH1 ** ** TRAIL_TRAIL;
      TH1 ** ** LEAD_LEAD;
      TH1 ** ** TOT_TOT;
      TH1 ** * TOT_Single;

    private:
	  
	  Detector_System ** Detector_Systems;
	  Raw_Event * RAW;
	  
	  TH1I          *h_fFebex_E[Febex_chan_num];
	  TH1I          *h_fFebex_T[Febex_chan_num];
	  TH1F          *h_fVME_TDC[VME_TDC_CHAN_NUM];
	  TH1F          *h_fVME_ADC[VME_NADC][VME_ADC_CHAN_NUM];
	  TH1F		*h_fVME_QDC[VME_QDC_CHAN_NUM];
	  TH1F          *h_fLead_Fast[bPLASTIC_CHAN_PER_DET];
	  TH1F          *h_fLead_Slow[bPLASTIC_CHAN_PER_DET];
	  TH1F          *h_fToT_Fast[bPLASTIC_CHAN_PER_DET];
	  TH1F          *h_fToT_Slow[bPLASTIC_CHAN_PER_DET];

	  
	  TGo4WinCond   *fconHis1;
	  TGo4WinCond   *fconHis2;
	  TGo4PolyCond  *fPolyCon;
	  TGo4CondArray *fConArr;
	  TGo4MbsEvent  *fInput;
	  SDBParam      *fParam;
	  TGo4Picture   *fPicture;
	  TGo4Picture   *fcondSet;
	  Float_t        fCrate1[8];
	  Float_t        fCrate2[8];

   ClassDef(SDBProc,1)
};
#endif //TUNPACKPROCESSOR_H


