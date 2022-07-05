///Should be set for each experiment depending upon configuration
#ifndef DESPEC_SETUP_FILE
#define DESPEC_SETUP_FILE

//bplast parameters
#define BPLAS_ProcID_TAMEX 80
#define bPLASTIC_TWINPEAKS 1
#define bPLASTIC_TAMEX_MODULES 9
#define bPLASTIC_TAMEX_HITS 10
#define bPLASTIC_TAMEX_CHANNELS 16
#define RAW_DATA_LENGTH 4000
#define bPLASTIC_CHAN_PER_DET 64

//enable bplast yes/no 1/0
#define bPlast 1

///bPlast Additional coincidence 
#define bPLASTIC_OR_UP_DOWN 3
#define bPLASTIC_UP_COIN 8
#define bPLASTIC_DOWN_COIN 8

//#define RAW_DATA_LENGTH 400
#define E_DATA_LENGTH 512

//VME parameters
#define VME_ProcID 20
#define VME_TDC_CHAN_NUM 32
#define VME_NADC 2
#define VME_ADC_CHAN_NUM 32
#define VME_QDC_CHAN_NUM 32
#define NMCP 4
#define NDSSD 2
#define DSSD_CHAN_NUM 16

//VME data masks
#define  OV_MASK 0x00001000  // overflow bit
#define  UN_MASK 0x00002000  // underthreshold bit
#define   V_MASK 0x00004000  // data valid bit
#define FCH_MASK 0x00003F00  // number of fired channels mask
#define  CH_MASK 0x003F0000  // channel mask
#define  VME_ADC_QDC_DA_MASK 0x00000FFF  // data mask for V785, V792, V775
#define  TY_MASK 0x07000000  // type mask
#define GEO_MASK 0xF8000000  // geo address mask
#define CH_MASK2 0x03E00000  // channel mask for V1290
#define VME_TDC_DA_MASK 0x001FFFFF  // data mask for V1290
#define bad_evt_hex 3134196653 //Bad event hex equal to 0xbad00bad

//Febex parameters
#define FEBEX_ProcID 60
#define Febex_chan_num 16

//Fill histos in analysis step yes/no 1/0
#define HISTOS 1

#endif /* DESPEC_SETUP_FILE */
