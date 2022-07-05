//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#ifndef CALIBPAR_H
#define CALIBPAR_H

#include "Go4StatusBase/TGo4Parameter.h"
#include <iomanip>
#include "Configuration_Files/Setup_File.h"

class CalibParameter: public TGo4Parameter {

  public: CalibParameter();
  CalibParameter(const Text_t * name);
  virtual~CalibParameter();
  Int_t PrintParameter(Text_t * buf, Int_t);
  Bool_t UpdateFrom(TGo4Parameter * );
  int IsData(std::ifstream & f);

  Double_t Abplas_TAMEX[48],
  Bbplas_TAMEX[48];
  Int_t DetIDPlas_TAMEX;

  ClassDef(CalibParameter, 5)
};

#endif
