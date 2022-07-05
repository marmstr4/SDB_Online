//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#include "Raw_Event.h"
#include <iostream>

using namespace std;

//---------------------------------------------------------------

Raw_Event::Raw_Event() {
  Event_Type = -1;
}

//---------------------------------------------------------------

Raw_Event::~Raw_Event() {}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------bPlast twinpeaks--------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Raw_Event::set_DATA_PLASTIC_TWINPEAKS(int * it_bPlastTwinPeaks, double ** Edge_Coarse_bPlastTwinPeaks, double ** Edge_fine_bPlastTwinPeaks, UInt ** ch_ed_bPlastTwinPeaks, double * Coarse_Trigger_bPlastTwinPeaks, double * Fine_Trigger_bPlastTwinPeaks, int amount_hit_bPlastTwinPeaks, int ** Lead_Arr_bPlastTwinPeaks){

  this -> amount_hit_bPlastTwinPeaks = amount_hit_bPlastTwinPeaks;
  //reset lead and trail hits
  for (int i = 0; i < bPLASTIC_TAMEX_MODULES; i++) {
    for (int j = 0; j < 32; j++) {
      leading_hits_ch_bPlastTwinPeaks[i][j] = 0;
      trailing_hits_ch_bPlastTwinPeaks[i][j] = 0;
      leading_array_bPlastTwinPeaks[i][j] = 0;
      //coarse_T_edge_lead_bPlastTwinPeaks[i][j] = 0;
      //fine_T_edge_lead_bPlastTwinPeaks[i][j] = 0;
    }
  }

  //loop over all tamex modules
  for (int i = 0; i < amount_hit_bPlastTwinPeaks; i++) {
    iterator_bPlastTwinPeaks[i] = it_bPlastTwinPeaks[i];
    trigger_coarse_bPlastTwinPeaks[i] = Coarse_Trigger_bPlastTwinPeaks[i];
    trigger_fine_bPlastTwinPeaks[i] = Fine_Trigger_bPlastTwinPeaks[i];
    fired_tamex_bPlastTwinPeaks[i] = (iterator_bPlastTwinPeaks[i] > 0);
    leading_hits_bPlastTwinPeaks[i] = 0;
    trailing_hits_bPlastTwinPeaks[i] = 0;

    for (int j = 0; j < iterator_bPlastTwinPeaks[i]; ++j) {
      ch_ID_bPlastTwinPeaks[i][j] = ch_ed_bPlastTwinPeaks[i][j];
      leading_array_bPlastTwinPeaks[i][j] = Lead_Arr_bPlastTwinPeaks[i][j];
//       cout<<"ch_ID_bPlastTwinPeaks[i][j] " <<ch_ID_bPlastTwinPeaks[i][j] << " i " << i << " j " << j << " j % 2 " <<j % 2 << endl;

      if (ch_ID_bPlastTwinPeaks[i][j] < 33 && j % 2 == 0) {
        coarse_T_edge_lead_bPlastTwinPeaks[i][j] = (double) Edge_Coarse_bPlastTwinPeaks[i][j];
        fine_T_edge_lead_bPlastTwinPeaks[i][j] = (double) Edge_fine_bPlastTwinPeaks[i][j];
        phys_channel_bPlastTwinPeaks[i][j] = (ch_ID_bPlastTwinPeaks[i][j]);
        //cout <<"LEAD RAW Ch " << ch_ID_bPlastTwinPeaks[i][j] <<" phys_channel_bPlastTwinPeaks[i][j] " << phys_channel_bPlastTwinPeaks[i][j]<<" coarse_T_edge_lead_bPlastTwinPeaks[i][j] " <<coarse_T_edge_lead_bPlastTwinPeaks[i][j] <<" fine_T_edge_lead_bPlastTwinPeaks[i][j] " <<fine_T_edge_lead_bPlastTwinPeaks[i][j] <<" i " << i << " j " << j <<   endl;
        leading_hits_bPlastTwinPeaks[i]++;
        leading_hits_ch_bPlastTwinPeaks[i][phys_channel_bPlastTwinPeaks[i][j]]++;
        //cout <<"RAW Lead" << "coarse_T_edge_lead_bPlastTwinPeaks[i][j] " << coarse_T_edge_lead_bPlastTwinPeaks[i][j] << " fine_T_edge_lead_bPlastTwinPeaks[i][j] " <<fine_T_edge_lead_bPlastTwinPeaks[i][j] <<" phys_channel_bPlastTwinPeaks[i][j] " <<phys_channel_bPlastTwinPeaks[i][j]<< " i " << i << " j " << j << endl;
      }
      if (ch_ID_bPlastTwinPeaks[i][j] > 33 && j % 2 == 1 && ch_ID_bPlastTwinPeaks[i][j] < 66) {
        coarse_T_edge_trail_bPlastTwinPeaks[i][j] = (double) Edge_Coarse_bPlastTwinPeaks[i][j];
        fine_T_edge_trail_bPlastTwinPeaks[i][j] = (double) Edge_fine_bPlastTwinPeaks[i][j];
        trailing_hits_bPlastTwinPeaks[i]++;
        phys_channel_bPlastTwinPeaks[i][j] = (ch_ID_bPlastTwinPeaks[i][j]) - 33;
        if (phys_channel_bPlastTwinPeaks[i][j] < 100) {
          trailing_hits_ch_bPlastTwinPeaks[i][phys_channel_bPlastTwinPeaks[i][j]]++;
        }
     }
    }
  }

  Event_Type = 2;
}

//------------------------ bPlast Twinpeaks TAMEX ------------------------------------//

int Raw_Event::get_bPlast_TWINPEAKS_tamex_hits() {
  return amount_hit_bPlastTwinPeaks;}

int Raw_Event::get_bPlast_TWINPEAKS_am_Fired(int i) {
  return iterator_bPlastTwinPeaks[i];}

double Raw_Event::get_bPlast_TWINPEAKS_trigger_T(int i) {
  return (trigger_coarse_bPlastTwinPeaks[i] - trigger_fine_bPlastTwinPeaks[i]) * 5000;}

int Raw_Event::get_bPlast_TWINPEAKS_CH_ID(int i, int j) {
  return ch_ID_bPlastTwinPeaks[i][j];}

///here
double Raw_Event::get_bPlast_TWINPEAKS_lead_T(int i, int j) {
  // cout << " i " << i << " j " << j << " coarse_T_edge_lead_bPlastTwinPeaks " << coarse_T_edge_lead_bPlastTwinPeaks[i][j] << "fine_T_edge_lead_bPlastTwinPeaks " <<fine_T_edge_lead_bPlastTwinPeaks[i][j] <<endl;
  return (coarse_T_edge_lead_bPlastTwinPeaks[i][j] - fine_T_edge_lead_bPlastTwinPeaks[i][j]);}

double Raw_Event::get_bPlast_TWINPEAKS_coarse_lead(int i, int j) {
  //  cout <<"RAW getter coarse_T_edge_lead_bPlastTwinPeaks[i][0]" <<coarse_T_edge_lead_bPlastTwinPeaks[i][0]<<" i " <<i << " j " << j <<endl; 
  return coarse_T_edge_lead_bPlastTwinPeaks[i][j];}

double Raw_Event::get_bPlast_TWINPEAKS_fine_lead(int i, int j){
  return fine_T_edge_lead_bPlastTwinPeaks[i][j];}
double Raw_Event::get_bPlast_TWINPEAKS_coarse_trail(int i, int j){
  return coarse_T_edge_trail_bPlastTwinPeaks[i][j];}
double Raw_Event::get_bPlast_TWINPEAKS_fine_trail(int i, int j){
  return fine_T_edge_trail_bPlastTwinPeaks[i][j];}

///here
double Raw_Event::get_bPlast_TWINPEAKS_trail_T(int i, int j){
  //cout << " i " << i << " j " << j << " coarse_T_edge_trail_bPlastTwinPeaks " << coarse_T_edge_trail_bPlastTwinPeaks[i][j] << " fine_T_edge_trail_bPlastTwinPeaks " <<fine_T_edge_trail_bPlastTwinPeaks[i][j] <<endl;
  return (coarse_T_edge_trail_bPlastTwinPeaks[i][j] - fine_T_edge_trail_bPlastTwinPeaks[i][j]);}

double Raw_Event::get_bPlast_TWINPEAKS_Lead_Lead(int i, int j){
  double T_lead_bPlastTwinPeaks1 = (coarse_T_edge_lead_bPlastTwinPeaks[i][j] - fine_T_edge_lead_bPlastTwinPeaks[i][j]);
  double T_lead_bPlastTwinPeaks2 = (coarse_T_edge_lead_bPlastTwinPeaks[i][j + 2] - fine_T_edge_lead_bPlastTwinPeaks[i][j + 2]);
  return T_lead_bPlastTwinPeaks1 - T_lead_bPlastTwinPeaks2;}

double Raw_Event::get_bPlast_TWINPEAKS_TOT(int i, int j){
  double T_lead_bPlastTwinPeaks = (coarse_T_edge_lead_bPlastTwinPeaks[i][j] - fine_T_edge_lead_bPlastTwinPeaks[i][j]);
  double T_trail_bPlastTwinPeaks = (coarse_T_edge_trail_bPlastTwinPeaks[i][j + 1] - fine_T_edge_trail_bPlastTwinPeaks[i][j + 1]);
  return T_trail_bPlastTwinPeaks - T_lead_bPlastTwinPeaks;}

int Raw_Event::get_bPlast_TWINPEAKS_trail_hits(int i){
  return trailing_hits_bPlastTwinPeaks[i];}
int Raw_Event::get_bPlast_TWINPEAKS_lead_hits(int i){
  return leading_hits_bPlastTwinPeaks[i];}
int Raw_Event::get_bPlast_TWINPEAKS_physical_channel(int i, int j){
  return phys_channel_bPlastTwinPeaks[i][j];}
int Raw_Event::get_bPlast_TWINPEAKS_physical_lead_hits(int i, int j){
  return leading_hits_ch_bPlastTwinPeaks[i][j];}
int Raw_Event::get_bPlast_TWINPEAKS_physical_trail_hits(int i, int j){
  return trailing_hits_ch_bPlastTwinPeaks[i][j];}
int Raw_Event::get_Event_type(){
  return Event_Type;}
