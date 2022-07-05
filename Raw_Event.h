//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#ifndef RAW_EVENT_H
#define RAW_EVENT_H

#include <math.h>
#include <float.h>
#include <vector>

#include "Configuration_Files/Setup_File.h"

//#include "Rtypes.h"

typedef unsigned long ULong;
typedef unsigned int UInt;
typedef unsigned long long ULong64_t;
typedef float Float_t;
typedef int Int_t;
typedef unsigned int UInt_t;


class Raw_Event{

private:


    bool fired_tamex[5];
    bool VME_Event;
    
    //bPlastic TAMEX
    int         amount_hit_tamex_bPlas;
    int         tamex_id_bPlas[100];
    int         iterator_bPlas[bPLASTIC_TAMEX_MODULES];
    double      trigger_coarse_bPlas[48];
    double      trigger_fine_bPlas[48];
    int         leading_array_bPlas[bPLASTIC_TAMEX_MODULES][48];
    int         leading_hits_bPlas[bPLASTIC_TAMEX_MODULES];
    int         trailing_hits_bPlas[bPLASTIC_TAMEX_MODULES];
    int         phys_channel_bPlas[bPLASTIC_TAMEX_MODULES][48];
    int         leading_hits_ch_bPlas[bPLASTIC_TAMEX_MODULES][48];
    int         trailing_hits_ch_bPlas[bPLASTIC_TAMEX_MODULES][48];
    int         ch_ID_bPlas[bPLASTIC_TAMEX_MODULES][100];
    double      coarse_T_edge_lead_bPlas[bPLASTIC_TAMEX_MODULES][48];
    double      coarse_T_edge_trail_bPlas[bPLASTIC_TAMEX_MODULES][48];
    double      fine_T_edge_lead_bPlas[bPLASTIC_TAMEX_MODULES][48];
    double      fine_T_edge_trail_bPlas[bPLASTIC_TAMEX_MODULES][48];
    double      lead_T_bPlas[bPLASTIC_TAMEX_MODULES][48];
    double      trail_T_bPlas[bPLASTIC_TAMEX_MODULES][48];
    double      ToT[bPLASTIC_TAMEX_MODULES][48];
    bool        fired_tamex_bPlas[bPLASTIC_TAMEX_MODULES];
    
    //bPlast Twinpeaks TAMEX
    int     amount_hit_bPlastTwinPeaks;
    int     iterator_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES];
    double  trigger_coarse_bPlastTwinPeaks[100];
    double  trigger_fine_bPlastTwinPeaks[100];
    int     leading_array_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
    int     leading_hits_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES];
    int     trailing_hits_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES];
    int     phys_channel_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
    int     leading_hits_ch_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
    int     trailing_hits_ch_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
    UInt_t  ch_ID_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
    double  coarse_T_edge_lead_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
    double  coarse_T_edge_trail_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
    double  fine_T_edge_lead_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
    double  fine_T_edge_trail_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES][100];
    bool    fired_tamex_bPlastTwinPeaks[bPLASTIC_TAMEX_MODULES];


public:
	//Raw_Event(bool);
    Raw_Event();
	~Raw_Event();

    
    void set_DATA_PLASTIC_TWINPEAKS(int*,double**,double**,UInt**,double*,double*,int,int**);
	int get_Event_type();
int Event_Type;
	bool PLASTIC_CheckVME();

    
      //temporary bPlast TAMEX getters
    int     get_bPlast_TWINPEAKS_am_Fired(int);
    double  get_bPlast_TWINPEAKS_trigger_T(int);
    int     get_bPlast_TWINPEAKS_CH_ID(int,int);
    double  get_bPlast_TWINPEAKS_lead_T(int,int);
    double  get_bPlast_TWINPEAKS_trail_T(int,int);
    int     get_bPlast_TWINPEAKS_trail_hits(int);
    int     get_bPlast_TWINPEAKS_lead_hits(int);
    int     get_bPlast_TWINPEAKS_physical_channel(int,int);
    int     get_bPlast_TWINPEAKS_physical_lead_hits(int,int);
    int     get_bPlast_TWINPEAKS_physical_trail_hits(int,int);
    double  get_bPlast_TWINPEAKS_coarse_lead(int,int);
    double  get_bPlast_TWINPEAKS_fine_lead(int,int);
    double  get_bPlast_TWINPEAKS_coarse_trail(int,int);
    double  get_bPlast_TWINPEAKS_fine_trail(int,int);
    double  get_bPlast_TWINPEAKS_TOT(int,int);
    double  get_bPlast_TWINPEAKS_TOT_added(int,int);
    int     get_bPlast_TWINPEAKS_tamex_hits();
    double  get_bPlast_TWINPEAKS_Lead_Lead(int,int);

    int     get_scaler_iterator();
    double  get_scaler_data(int);
    


};


#endif
