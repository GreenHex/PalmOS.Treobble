/*
 * Utils.h
 */
 
#ifndef __UTILS_H__
#define __UTILS_H__

#include <Hs.h>
#include <HsPhone.h>
#include <HsNav.h>
#include <HsExt.h>
#include <PalmTypes.h>
#include <palmOneResources.h>
#include <TonesLibTypes.h>
#include <TonesLib.h>
#include <PmSysGadgetLib.h>
#include <PmSysGadgetLibCommon.h>
#include <PmKeyLibCommon.h>
#include <PmKeyLib.h>

#include "Global.h"
#include "AppResources.h"

#define MIN_VERSION  					sysMakeROMVersion( 5, 0, 0, sysROMStageRelease, 0 )
#define LAUNCH_FLAGS 					( sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp )

// About form
#define ABOUT_FORM						2000
#define ABOUT_DONE_BUTTON				2001
#define ABOUT_VERSION_LABEL				2010

// Prototypes
extern void 				Alert( Char* str, UInt32* numP );
extern void					vibrate(void);
extern void 				LEDState( Boolean set, UInt16* countP, UInt16* stateP );
extern void 				FlashLED( Boolean blink, UInt8 BlinkType );
extern ToneVolume 			GetPhoneVolume( Boolean* pSoundOnVibrate, Boolean* pSoundOffVibrate );
extern Boolean 				OpenKeyLib( UInt16* pmKeyLibRefNumP );
extern void 				CloseKeyLib( UInt16 pmKeyLibRefNum, Boolean pmKeyLibLoaded );
extern void 				CommonInitGauge( FormPtr pForm );
extern ToneVolume 			GetPhoneVolume( Boolean* pSoundOnVibrate, Boolean* pSoundOffVibrate );
extern void 				ShowAboutForm( FormType* pOldForm );
extern Err 					RomVersionCompatible( UInt32 requiredVersion, UInt16 launchFlags );
extern Int16 				DateCompare( DateType d1, DateType d2 );
extern Int16 				TimeCompare( TimeType t1, TimeType t2 );

#endif /* __UTILS_H__ */
 
/*
 * Utils.h
 */