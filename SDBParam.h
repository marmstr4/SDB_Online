//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#define __ARRAYSIZE__ 100
#define __MEVX__ 5
#define __MEVY__ 10

#include "TGo4Parameter.h"

class SDBParam : public TGo4Parameter {
   public:
      SDBParam();
      SDBParam(const char* name);
      virtual ~SDBParam();
      virtual Int_t  PrintParameter(Text_t * n, Int_t);
      virtual Bool_t UpdateFrom(TGo4Parameter *);

      // samples for all supported data types:
      // comments appear in the generic GUI editor
      // dont forget to change the UpdateFrom and PrintParameter
      // functions when changing members
      Bool_t   fill;    // control filling
      Int_t    shift;   // control
      Bool_t   show_pu; // control
      Float_t  frP1;    // control frP1
      Float_t  frP2;    // control frP2
      Int_t    fiNum;   // control fiNum
      Bool_t   fbEnab;  // control
      Double_t fdTest;  // control
      Short_t  fsTest;  // control
      Char_t   fcTest;  // control
      Long_t   flTest;  // control
      TString  fxInputfile;
      Int_t    fiDataArray[__ARRAYSIZE__];
      Double_t fdMeV[__MEVX__][__MEVY__];
      UInt_t   fuNum;
      Double_t fdTestarr[__MEVX__];
      UShort_t fuShort;
      UChar_t  fuChar;
      ULong_t  fuLong;

   ClassDef(SDBParam,1)
};

//----------------------------END OF GO4 SOURCE FILE ---------------------
