//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#include "CalibParameter.h"
#include <iostream>

using namespace std;
 
CalibParameter::CalibParameter()
: TGo4Parameter("AnaOnlineCalPar")
{ Int_t i;
    
     for(i=0; i<48; i++){  
       DetIDPlas_TAMEX = 0; 
       Abplas_TAMEX[i] = 0;
       Bbplas_TAMEX[i] = 0;
     } 
}

  //----------------------------------------------
CalibParameter::CalibParameter(const Text_t *name)
: TGo4Parameter(name)
{
  Int_t i;
  ifstream    file;

    //-----------------------------------------------------------------------------------------------------//
   file.open("Configuration_Files/Plastic_TAMEX_Energy_Calibration.txt");
  if (file.fail()) {
    cout << "ERROR:  Could not open file: Plastic_TAMEX_Energy_Calibration.txt ! (params set to 1 and 0)\n";
    for (i = 0; i < 48; i++){
       DetIDPlas_TAMEX = i; 
       Abplas_TAMEX[i] = 1;
       Bbplas_TAMEX[i] = 0;
    }
  }
  
   else {
    cout << "CalibParameter - reading calibration from: Plastic_TAMEX_Energy_Calibration.txt\n";
    for (i = 0; i < 48; i++){
      if(IsData(file)) file >> DetIDPlas_TAMEX  >> Abplas_TAMEX[i] >> Bbplas_TAMEX[i] ;
       if (file.fail()) cout << "ERROR reading Plastic_TAMEX_Energy_Calibration.txt\n ";
    }
  }
  file.close();
}
//------------------------------------------------------------------------------//

CalibParameter::~CalibParameter()
{}
//------------------------------------------------------------------------------

Int_t CalibParameter::PrintParameter(Text_t *buf, Int_t)
{
    return 1;
}

//------------------------------------------------------------------------------

Bool_t CalibParameter::UpdateFrom(TGo4Parameter *pp)
{
  if(pp->InheritsFrom("CalibParameter"))
  {
    Int_t i;
    CalibParameter *from = (CalibParameter *) pp;
    
      for (i = 0; i < 48; i++){
       DetIDPlas_TAMEX = from->DetIDPlas_TAMEX; 
       Abplas_TAMEX[i] = from->Abplas_TAMEX[i];
       Bbplas_TAMEX[i] = from->Bbplas_TAMEX[i];
       
      }    
      cout << "CalibParameter - Parameter : " << GetName() << " UPDATED\n";
  }
     
      else {
      cout << "ERROR: CalibParameter - Wrong parameter object: " << pp->ClassName() << endl;}
 
        return kTRUE;
         
}
    int CalibParameter::IsData(ifstream &f) {
        char dum;
        char dumstr[300];
        int retval = 0;

        /* 'operator >>' does not read End-of-line, therefore check if read 
            character is not EOL (10) */
        do {
            dum = f.get();
            if (dum == '#')    // comment line => read whole line and throw it away
            f.getline(dumstr,300);
        }
        while ((dum == '#') || ((int)dum == 10)); 

        f.unget();   // go one character back
        retval = 1;
        return retval;
    }

ClassImp(CalibParameter)
