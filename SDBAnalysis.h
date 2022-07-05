//-------------------------------------------------------------------
//                SDB ONLINE ANALYSIS Go4 Code
//                      16-June-2022
//               M.Armstrong, M.Armstrong@gsi.de
//-------------------------------------------------------------------

#ifndef SDB_ANALYSIS_H
#define SDB_ANALYSIS_H

#include "TGo4Analysis.h"
class TGo4MbsEvent;
class SDBAnalysis : public TGo4Analysis  {
public:
  SDBAnalysis();
  SDBAnalysis(int argc, char** argv);
  virtual ~SDBAnalysis() ;

private:
  TGo4MbsEvent       *fMbsEvent;

  ClassDef(SDBAnalysis,1)
};

#endif //SDBANALYSIS_H                                                                                                       
