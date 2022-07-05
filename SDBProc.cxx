//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#include "SDBProc.h"
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

#include "SDBParam.h"
#include "SDBEvent.h"
#include "QDC.h"
#include "TAMEX.h"

//***********************************************************
SDBProc::SDBProc() : TGo4EventProcessor("Proc") {
    std::cout << "**** SDBProc: Create instance " << std::endl;
    RAW = new Raw_Event();
    load_bPlasticTamex_Allocationfile();
}

//***********************************************************
SDBProc::~SDBProc() {
    std::cout << "**** SDBProc: Delete instance " << std::endl;
}

//***********************************************************
SDBProc::SDBProc(const char *name) : TGo4EventProcessor(name) {
    std::cout << "**** SDBProc: Create instance " << name << std::endl;

    Text_t chis[16];
    Text_t chead[64];
    Int_t i;

    //Creation of parameters 
        fParam = new SDBParam("Par1");
        AddParameter(fParam);
        std::cout << "**** SDBProc: Created parameter" << std::endl;
    
    //FEBEX histograms
	
	//Energy
        for (i = 0; i < Febex_chan_num; i++) {
            snprintf(chis, 15, "Febex_E%02d", i + 1);
            snprintf(chead, 63, "Febex_E %2d", i + 1);
            h_fFebex_E[i] = new TH1I(chis, chead, 65536, 0, 65535);
            AddHistogram(h_fFebex_E[i], "Febex");
        }
	//Time
        for (i = 0; i < Febex_chan_num; i++) {
            snprintf(chis, 15, "Febex_T%02d", i + 1);
            snprintf(chead, 63, "Febex_T%2d", i + 1);
            h_fFebex_T[i] = new TH1I(chis, chead, 8192, -4096, 4097);
            AddHistogram(h_fFebex_T[i], "Febex");
        }
        
    //VME histograms
    
	//CAEN V1190 TDC
	for(i =0;i<VME_TDC_CHAN_NUM;i++)  {
	  snprintf(chis,15,"VME_TDC%02d",i+1);  
	  snprintf(chead,63,"VME_TDC %2d",i+1);
 	  h_fVME_TDC[i] = new TH1F (chis,chead,4096,1,524288*4);
	  AddHistogram(h_fVME_TDC[i],"VME");
	}
	
	//CAEN_ADC_V785
	for(int a =0;a<VME_NADC;a++){
	  for(i=0;i<VME_ADC_CHAN_NUM;i++){
	    snprintf(chis,15,"VME_ADC%d_%02d",a+1,i+1);  
	    snprintf(chead,63,"VME ADC%d %2d",a+1,i+1);
	    h_fVME_ADC[a][i] = new TH1F (chis,chead,5000,0,5000);
	    AddHistogram(h_fVME_ADC[a][i],"VME");
	  }
	}
	
	//QDC
	for(i =0;i<VME_QDC_CHAN_NUM;i++)   {
	  snprintf(chis,15,"VME_QDC%02d",i+1);  
	  snprintf(chead,63,"VME_QDC %2d",i+1);
	  h_fVME_QDC[i] = new TH1F (chis,chead,1000,0,1000);
	  AddHistogram(h_fVME_QDC[i],"VME");
	}
	
    //BPLAST histograms and calibration
	Detector_Systems = new Detector_System * [7];
	Detector_Systems[2] = !Used_Systems[2] ? nullptr : new PLASTIC_TWINPEAKS_Detector_System();
	
	RAW = new Raw_Event();
	load_bPlasticTamex_Allocationfile();
	
	for(i =0;i<64;i++)   {
	  snprintf(chis,70,"ToT_fast%02d",i+1);  
	  snprintf(chead,70,"ToT fast%2d",i+1);
	  h_fToT_Fast[i] = new TH1F (chis,chead,1000,-500,500);
	  AddHistogram(h_fToT_Fast[i],"Bplast");
	}
	
	for(i =0;i<64;i++)   {
	  snprintf(chis,70,"ToT_slow%02d",i+1);  
	  snprintf(chead,70,"Tot slow%2d",i+1);
	  h_fToT_Slow[i] = new TH1F (chis,chead,1000,-500,500);
	  AddHistogram(h_fToT_Slow[i],"Bplast");
	}
	
	
	for(i =0;i<64;i++)   {
	  snprintf(chis,70,"Lead_Fast%02d",i+1);  
	  snprintf(chead,70,"Lead_Fast%2d",i+1);
	  h_fLead_Fast[i] = new TH1F (chis,chead,1000,-500,500);
	  AddHistogram(h_fLead_Fast[i],"Bplast");
	}
	
	for(i =0;i<64;i++)   {
	  snprintf(chis,70,"Lead_Slow%02d",i+1);  
	  snprintf(chead,70,"Lead_Slow%2d",i+1);
	  h_fLead_Slow[i] = new TH1F (chis,chead,1000,-500,500);
	  AddHistogram(h_fLead_Slow[i],"Bplast");
	}
	
}


Bool_t SDBProc::BuildEvent(TGo4EventElement *dest) {  

    TGo4MbsSubEvent *psubevt;
    int i;
    Int_t lwords;
    
    unsigned long  caen_header, geo,type;                 
    long           d_cnt;                
    unsigned long  data, chan;
    
    UInt_t l_l;
    Int_t *pl_se_dat;
    Int_t *pl_tmp;

    UInt_t l_dat_len;
    UInt_t l_dat_len_byte;

    UInt_t l_dat;
    UInt_t l_trig_type;
    UInt_t l_sfp_id;
    UInt_t l_feb_id;
    UInt_t l_cha_id;
    Short_t proc_id;
    
    UInt_t l_cha_head;
    UInt_t l_cha_size;
    
    Int_t l_fpga_energy;   
    UInt_t header1;
    UInt_t header2;
    UInt_t dummy;
    UInt_t ts_h;
    UInt_t ts_l;
    UInt_t time[16];
    UInt_t time_fract[16];    
    UInt_t e_data[16];        
    UInt_t pattern;
    UInt_t pileup;

    Int_t sign_bit = 0; 

    //Get Event
    fInput = (TGo4MbsEvent *) GetInputEvent();
    if (fInput == 0) {
        std::cout << "AnlProc: no input event !" << std::endl;
        return kFALSE;
    }
    
    SDBEvent * out_evt = (SDBEvent * ) dest;

    fInput->ResetIterator();

    //Get Next Subevent
    while((psubevt = fInput->NextSubEvent()) != 0) // loop over subevents
      {   

      //Get Datafield, length and process id
      pl_se_dat = psubevt->GetDataField();
      l_dat_len = psubevt->GetDlen();
      l_dat_len_byte = (l_dat_len - 2) * 2;
      lwords= psubevt->GetIntLen();
      proc_id = psubevt->GetProcid();
    
      //Store datafield in temperorary variable for iteration
      pl_tmp = pl_se_dat;

      if (((*pl_tmp) == bad_evt_hex) || lwords < 3) {
	  printf("ERROR>> found bad event (0xbad00bad or 3134196653 ) \n");
	  goto bad_event;
      }

      //Process Febex events
      if (proc_id==FEBEX_ProcID) {

	//Iterate through datafield
	while ((pl_tmp - pl_se_dat) < (l_dat_len_byte / 4)) {
	    l_dat = *pl_tmp++;   

	    //Find 4byte padding words
	    if ((l_dat & 0xfff00000) == 0xadd00000) {
		//Skip forward over padding words to header
		l_dat = (l_dat & 0xff00) >> 8;
		pl_tmp += l_dat - 1;
	    } 
	    
	    //Find Datafield Header
	    else if ((l_dat & 0xff) == 0x34) {
		//Bitshift from header to find febex ID
		l_cha_head = l_dat;
		l_feb_id = (l_cha_head & 0xff0000) >> 16;

		//Find Summary channel
		if ((l_cha_head & 0xff000000) == 0xff000000) {
		    //number of channels in event
		    l_cha_size = *pl_tmp++;    // from gosip

		    //Find 4 header words below summary channel 
		    header1 = *pl_tmp++;
		    header2 = *pl_tmp++;
		    pattern = *pl_tmp++;  //pattern type id to veto pileup events
		    dummy = *pl_tmp++;
		  
		    pileup = (pattern >> 16) & 0x0000FFFF;
		    pileup = pileup & pattern;           

		    //Zero data arrays
		    for (i = 0; i < 16; i++) {
			e_data[i] = 0;
			time[i] = 0;
			time_fract[i] = 0;    
		    }

		    //Iterate other all 16 channels
		    for (i = 0; i < 16; i++)  {
		        //process data with zero pileup suppression
			if (pattern & 1 << i) {
			    //Set timestamp 
			    ts_h = *pl_tmp++;
			    ts_l = *pl_tmp++;
			    dummy = *pl_tmp++;                         

			    //extract energy using data masks
			    l_fpga_energy = dummy & 0x00FFFFFF;
			    sign_bit = (dummy & 0x00800000) >> 23;                   
			    if (sign_bit) {
				l_fpga_energy = l_fpga_energy | 0xFF000000;
			    }
   

			    //Set times 
			    time[i] = ts_l;

			    //Check that energy not negative
			    if (l_fpga_energy < 0) {
				fflush(stdout);
				e_data[i] = 0;
			    } 
			    
			    if (l_fpga_energy > 0) {
				//Get energy from fpga signal
				e_data[i] = l_fpga_energy >> fParam->shift;
			    }
			
			    //Fill energy histograms for each channel
			    h_fFebex_E[l_feb_id * 16 + i]->Fill(e_data[i]);    
			    h_fFebex_E[l_feb_id * 16 + i]->Fill(time[i]);
			    dummy = *pl_tmp++;
			}
		    }   

		}
		
		
	    } else {
		
	    }

	} 
      
      }

      
      //Need to roll MCP1, MCP2, DSSSD1, DSSSD2 into one procID loop
      //VME_ProcID 20
      if (proc_id==VME_ProcID ){
	
	  //Determine subevent type
	  type = psubevt->GetType();
	  
	  //Check event has data
	  if (type==10) {
	   
	    for(int a=0;a<VME_NADC;a++) {
	      for(i=0;i<VME_ADC_CHAN_NUM;i++) {
		long ADC = psubevt->Data(i+1+34*a);
		ADC = ADC & VME_ADC_QDC_DA_MASK;
		//Fill ADC Histogram
		h_fVME_ADC[a][i]->Fill(ADC);
		out_evt->VME_ADC[a][i] = ADC;		
	      }
	    }
	    
	    for(i=0;i<VME_QDC_CHAN_NUM;i++) {
	      long QDC = psubevt->Data(i+69);
	      QDC = QDC & VME_ADC_QDC_DA_MASK;
	      //Fill TDC Histogram
	      h_fVME_QDC[i]->Fill(QDC); 
	      out_evt->VME_QDC[i] = QDC;
	    }
	    
	    for(i=0;i<VME_TDC_CHAN_NUM;i++) {
	      long TDC = psubevt->Data(i+103);
	      TDC = TDC & VME_TDC_DA_MASK;
	      //Fill TDC Histogram
	      h_fVME_TDC[i]->Fill(TDC); 
	      out_evt->VME_TDC[i] = TDC;
	    }   	  
	    
	 }  
      }
	  
      
      
      
      if (proc_id==BPLAS_ProcID_TAMEX) {
	
	Detector_Systems[2] -> Process_MBS(psubevt);
	Detector_Systems[2] -> Process_MBS(pl_tmp);

	//get mbs stream data from unpacker (pointer copy solution)
	pl_tmp = Detector_Systems[2] -> get_pdata();

	//get data from subevent and send to RAW
	Detector_Systems[2] -> get_Event_data(RAW);

	
	int bPlasfired[bPLASTIC_TAMEX_MODULES];
	int Phys_Channel_Lead_Fast_bPlast[3][256];
	int Phys_Channel_Trail_Fast_bPlast[3][256];
	int Phys_Channel_Lead_Slow_bPlast[3][256];
	int Phys_Channel_Trail_Slow_bPlast[3][256];
	int bPlasdetnum_fast = -1;
	int bPlasdetnum_slow = -1;
	int N1_fast = 0;
	int N1_slow = 0;
	
	///Loop over tamex ID's
	for (int i = 0; i < RAW -> get_bPlast_TWINPEAKS_tamex_hits(); i++) { 
         if (RAW -> get_bPlast_TWINPEAKS_am_Fired(i) < 256) {

          bPlasfired[i] = RAW -> get_bPlast_TWINPEAKS_am_Fired(i);
	  
	  //Loop over hits per board
          for (int j = 0; j < bPlasfired[i]; j++) { 
// 	    
	    if((TAMEX_bPlast_Det[i][((RAW -> get_bPlast_TWINPEAKS_physical_channel(i, j) + 1) / 2) - 1])>0){
	      
              //NOW DEFINE Fast amplified odd channels and Slow amplified even channels  
	      if (RAW -> get_bPlast_TWINPEAKS_CH_ID(i, j) > 0 && RAW -> get_bPlast_TWINPEAKS_CH_ID(i, j) < 66) {

	      //Lead hits
              if (j % 2 == 0) { 
//                 
                ///Fast lead channels odd
                if (RAW -> get_bPlast_TWINPEAKS_CH_ID(i, j) % 2 == 1) {
                  if (RAW -> get_bPlast_TWINPEAKS_lead_T(i, j) > 0) {
                    Phys_Channel_Lead_Fast_bPlast[i][j] = TAMEX_bPlast_Chan[i][((RAW -> get_bPlast_TWINPEAKS_physical_channel(i, j) + 1) / 2) - 1];
                    bPlasdetnum_fast = TAMEX_bPlast_Det[i][((RAW -> get_bPlast_TWINPEAKS_physical_channel(i, j) + 1) / 2) - 1];
                    out_evt -> fbPlasDetNum_Fast = bPlasdetnum_fast;
      
		    int chan_bPlast_fast_lead = Phys_Channel_Lead_Fast_bPlast[i][j];
		    out_evt -> fbPlas_FastChan[bPlasdetnum_fast] = chan_bPlast_fast_lead;
		    
		    
		    if (chan_bPlast_fast_lead > -1 && chan_bPlast_fast_lead < bPLASTIC_CHAN_PER_DET) {
                      N1_fast = out_evt -> fbPlast_Fast_Lead_N[bPlasdetnum_fast][chan_bPlast_fast_lead]++;
                      out_evt -> fbPlast_Fast_Lead[bPlasdetnum_fast][chan_bPlast_fast_lead][N1_fast] = RAW -> get_bPlast_TWINPEAKS_lead_T(i, j);
		    }
                  }
                }
                
                ///Slow lead channels, even 
                if (RAW -> get_bPlast_TWINPEAKS_CH_ID(i, j) % 2 == 0) {
                  Phys_Channel_Lead_Slow_bPlast[i][j] = TAMEX_bPlast_Chan[i][(RAW -> get_bPlast_TWINPEAKS_physical_channel(i, j) / 2) - 1];
		  
                  int chan_bPlast_slow_lead = Phys_Channel_Lead_Slow_bPlast[i][j];
                  bPlasdetnum_slow = TAMEX_bPlast_Det[i][((RAW -> get_bPlast_TWINPEAKS_physical_channel(i, j) + 1) / 2) - 1];
		  //std::cout<<i<<" "<<j<<" Lead Slow"<<std::endl;

		  out_evt -> fbPlasDetNum_Slow = bPlasdetnum_slow;
                  out_evt -> fbPlas_SlowChan[bPlasdetnum_slow] = chan_bPlast_slow_lead;
		  
                  if (chan_bPlast_slow_lead > -1 && chan_bPlast_slow_lead < bPLASTIC_CHAN_PER_DET) {
                    N1_slow = out_evt -> fbPlast_Slow_Lead_N[bPlasdetnum_fast][chan_bPlast_slow_lead]++;
                    out_evt -> fbPlast_Slow_Lead[bPlasdetnum_fast][chan_bPlast_slow_lead][N1_slow] = RAW -> get_bPlast_TWINPEAKS_lead_T(i, j);
		  }
                }
              }

              //Tail hits
              if (j % 2 == 1) { 
		
                ///Fast trail channels even
                if (RAW -> get_bPlast_TWINPEAKS_CH_ID(i, j) % 2 == 0 && (RAW -> get_bPlast_TWINPEAKS_physical_channel(i, j) + 1) / 2 < 256) {

                  Phys_Channel_Trail_Fast_bPlast[i][j] = TAMEX_bPlast_Chan[i][(RAW -> get_bPlast_TWINPEAKS_physical_channel(i, j) / 2)];
                  int chan_bPlast_fast_trail = Phys_Channel_Trail_Fast_bPlast[i][j];
                  bPlasdetnum_fast = TAMEX_bPlast_Det[i][((RAW -> get_bPlast_TWINPEAKS_physical_channel(i, j) + 1) / 2) - 1];
		  //std::cout<<i<<" "<<j<<" Trail Fast"<<std::endl;


                  if (chan_bPlast_fast_trail > -1 && chan_bPlast_fast_trail < bPLASTIC_CHAN_PER_DET) {
                    N1_fast = out_evt -> fbPlast_Fast_Trail_N[bPlasdetnum_fast][chan_bPlast_fast_trail]++;
                    out_evt -> fbPlast_Fast_Trail[bPlasdetnum_fast][chan_bPlast_fast_trail][N1_fast] = RAW -> get_bPlast_TWINPEAKS_trail_T(i, j);
                  }
                }
                ///Slow trail channels even
                if (RAW -> get_bPlast_TWINPEAKS_CH_ID(i, j) % 2 == 1 && RAW -> get_bPlast_TWINPEAKS_physical_channel(i, j) < 256) {

                  Phys_Channel_Trail_Slow_bPlast[i][j] = TAMEX_bPlast_Chan[i][(RAW -> get_bPlast_TWINPEAKS_physical_channel(i, j) / 2) - 1];
                  bPlasdetnum_slow = TAMEX_bPlast_Det[i][((RAW -> get_bPlast_TWINPEAKS_physical_channel(i, j) + 1) / 2) - 1];
                  int chan_bPlast_slow_trail = Phys_Channel_Trail_Slow_bPlast[i][j];
		  //std::cout<<i<<" "<<j<<" Trail Slow"<<std::endl;
		  
		  
                  if (chan_bPlast_slow_trail > -1 && chan_bPlast_slow_trail < bPLASTIC_CHAN_PER_DET) {
                    N1_slow = out_evt -> fbPlast_Slow_Trail_N[bPlasdetnum_slow][chan_bPlast_slow_trail]++;
                    out_evt -> fbPlast_Slow_Trail[bPlasdetnum_slow][chan_bPlast_slow_trail][N1_slow] = RAW -> get_bPlast_TWINPEAKS_trail_T(i, j);
                  }
                }
              }
            }
          }
	 }
        }
        
        ///Read out lead and trail timings from bplast by looping over channel number k and hit number l in bplast (detector 1)
	for (int k=0; k<64; k++){
	  for (int l=0; l<10; l++){
	    if (out_evt->fbPlast_Fast_Trail[1][k][l] - out_evt->fbPlast_Fast_Lead[1][k][l]!=0) {
	      
	      h_fLead_Slow[k]->Fill(out_evt->fbPlast_Slow_Lead[1][k][l]);
	      h_fLead_Fast[k]->Fill(out_evt->fbPlast_Fast_Lead[1][k][l]);
	      
	      h_fToT_Fast[k]->Fill(out_evt->fbPlast_Fast_Trail[1][k][l] - out_evt->fbPlast_Fast_Lead[1][k][l]);	  
	      h_fToT_Slow[k]->Fill(out_evt->fbPlast_Slow_Trail[1][k][l] - out_evt->fbPlast_Slow_Lead[1][k][l]);
	      
	    }
	  }
	}
	
	
      }	
    }  
  }
    
bad_event:
return kTRUE;

}

//load bplast channel mappings from file
void SDBProc::load_bPlasticTamex_Allocationfile() {

  const char * format = "%d %d %d %d";
  ifstream data("Configuration_Files/bPlast_TAMEX_allocation.txt");
  if (data.fail()) {
    cerr << "Could not find bPlast_TAMEX_allocation file!" << endl;
    exit(0);
  }

  for (int i = 0; i < bPLASTIC_TAMEX_MODULES; i++) {
    for (int j = 0; j < bPLASTIC_TAMEX_CHANNELS; j++) {
      TAMEX_bPlast_Chan[i][j] = 0;
      TAMEX_bPlast_Det[i][j] = 0;
    }
  }
  
  int bPlastTamID = 0;
  int bPlastTamCh = 0;
  int bPlast_det = 0;
  int bPlast_ch = 0;
  string line;
  while (data.good()) {

    getline(data, line, '\n');
    if (line[0] == '#') continue;
    sscanf(line.c_str(), format, & bPlastTamID, & bPlastTamCh, & bPlast_det, & bPlast_ch);

    TAMEX_bPlast_Det[bPlastTamID][bPlastTamCh] = bPlast_det;
    TAMEX_bPlast_Chan[bPlastTamID][bPlastTamCh] = bPlast_ch;
  }
}

