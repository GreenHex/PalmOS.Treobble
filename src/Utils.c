/*
 * Utils.c
 */

#include "Utils.h"

/*
 * Alert
 */
void Alert( Char* str, UInt32* numP )
{
  	Char	numStr[32] = "\0";

	if (numP)
	{
	  	StrPrintF( numStr, "0x%lx", *numP );
	}
  	FrmCustomAlert( ErrOKAlert, str, "\n\n", numStr );
  	
  	return;

} // Alert

/*
 * vibrate()
 */
void vibrate(void)
{
 	UInt16 		count = 1;
	UInt16		state = kIndicatorForceOn;
	
	HsIndicatorState(count, kIndicatorTypeVibrator, &state);
	SysTaskDelay(5);
	state = kIndicatorForceOff;
	HsIndicatorState(count, kIndicatorTypeVibrator, &state);
	
	return;	

} // vibrate


/*
 * LEDState()
 * set = true SETS the state
 * set = false GETS the state
 */
void LEDState( Boolean set, UInt16* countP, UInt16* stateP )
{
	if ( !set )
	{
		*countP = kIndicatorCountGetState;
		*stateP = kIndicatorStateNull;
	}
	
	HsIndicatorState( *countP, kIndicatorTypeLed, stateP );
	
	return;
		
} // LEDState()

/*
 * FlashLED()
 */
void FlashLED( Boolean blink, UInt8 BlinkType )
{
	UInt16 		count = kIndicatorCountForever;
	UInt16		onState[4] = { kIndicatorAlertAlert , kIndicatorForceRedGreen , kIndicatorForceRed , kIndicatorForceGreen } ;
	UInt16		offState[4] = { kIndicatorAlertNone , kIndicatorForceNone , kIndicatorForceNone , kIndicatorForceNone } ;
	UInt16		state = (blink) ? onState[BlinkType] : offState[BlinkType];
		
	HsIndicatorState(count, kIndicatorTypeLed, &state);
	
	return;
	
} // FlashLED()

/*
 * OpenKeyLib()
 */
Boolean OpenKeyLib( UInt16* pmKeyLibRefNumP )
{
	Boolean pmKeyLibLoaded = false;
	
	if ( SysLibFind( kPmKeyLibName, pmKeyLibRefNumP ) )
	{
		pmKeyLibLoaded = ( ( ( SysLibLoad( kPmKeyLibType, kPmKeyLibCreator, pmKeyLibRefNumP ) ) == errNone )
								&& ( *pmKeyLibRefNumP != sysInvalidRefNum ) );
	}
	
	if ( *pmKeyLibRefNumP )
	{
		PmKeyLibOpen( *pmKeyLibRefNumP );
	}
	
	return ( pmKeyLibLoaded );
	
} // OpenKeyLib(

/*
 * CloseKeyLib()
 */
void CloseKeyLib(UInt16 pmKeyLibRefNum, Boolean pmKeyLibLoaded)
{
	if ( pmKeyLibRefNum )
	{
		PmKeyLibClose( pmKeyLibRefNum );
		
		if ( pmKeyLibLoaded )
		{
			SysLibRemove( pmKeyLibRefNum );	
		}	
	}
	
	return;
	
} // CloseKeyLib()

/*
 * CommonInitGauge
 */
void CommonInitGauge( FormPtr pForm )
{
	UInt32 			hsStatusVersion;
	UInt16 			libRefNum;
	Err				error = errNone;
	
	if ( FtrGet( hsFtrCreator, hsFtrIDStatusGadgetRev, &hsStatusVersion ) == 0 )
	{
		HsStatusSetGadgetType( pForm, BATTERY_STATUS_GADGET, hsStatusGadgetBattery );
		
		HsStatusSetGadgetType( pForm, PHONE_STATUS_GADGET, hsStatusGadgetSignal );
		
		error = SysLibFind( kPmSysGadgetLibName, &libRefNum );
			if ( error ) SysLibLoad( kPmSysGadgetLibType, kPmSysGadgetLibCreator, &libRefNum );
		
		PmSysGadgetLibOpen( libRefNum );
		PmSysGadgetStatusGadgetTypeSet( libRefNum, pForm, BT_STATUS_GADGET, pmSysGadgetStatusGadgetBt );
		PmSysGadgetLibClose( libRefNum );
	}
	
	return;
	
} // CommonInitGauge

/*
 * GetPhoneVolume()
 * typedef UInt16  ToneVolume;
 */
ToneVolume GetPhoneVolume(Boolean* pSoundOnVibrate, Boolean* pSoundOffVibrate)
{
	ToneVolume			volume = toneVolumeOff;
	Err					error = errNone;
	UInt16				tonesLibRefNum = 0;
	Boolean				tonesLibLoaded = false;
	SoundPreference		phnSndPref;
	
	if ( SysLibFind( tonesLibName, &tonesLibRefNum ) )
	{
		tonesLibLoaded = ( (SysLibLoad(tonesLibType, tonesLibCreator, &tonesLibRefNum) == errNone)
				&& ( tonesLibRefNum != sysInvalidRefNum ) );
	}
	
	error = TonesLibOpen( tonesLibRefNum );
					
	if (!error)
	{
		TonesLibGetSoundPrefs( tonesLibRefNum, soundPrefTypePhone, &phnSndPref );
		volume = phnSndPref.soundOnVolume;
	
		if ( pSoundOnVibrate )
		{
			*pSoundOnVibrate = phnSndPref.soundOnVibrate;
		}
		
		if ( pSoundOffVibrate )
		{
			*pSoundOffVibrate = phnSndPref.soundOffVibrate;
		}
		
		TonesLibClose( tonesLibRefNum );
	}
	
	if ( tonesLibLoaded )
	{
		SysLibRemove( tonesLibRefNum );	
	}
	
	return ( volume );
	
} // GetPhoneVolume() 

/*
 * ShowAboutForm()
 */
void ShowAboutForm( FormType* pOldForm )
{
	FormType*				pAboutForm = FrmInitForm( ABOUT_FORM );
	
	MemHandle				strH = NULL;
	Char*					strP = NULL;
	FormActiveStateType 	curFrmState;
	
	FrmSaveActiveState ( &curFrmState );
	
	strH = DmGet1Resource( 'tver', 1000 );
	
	if (strH)
	{
		strP = MemHandleLock( strH );
		
		if (strP)
		{
			Char				verStr[20];
		 
			StrCopy( verStr, "v" );
			StrCat( verStr, strP );
			StrCat( verStr, " [Freeware]" );
			verStr[StrLen((Char *) FrmGetLabel( pAboutForm, ABOUT_VERSION_LABEL ) )] = chrNull;
			
			FrmCopyLabel( pAboutForm, ABOUT_VERSION_LABEL, verStr );
			
			MemHandleUnlock( strH );
		}
	   	
	   	DmReleaseResource( strH );
	   	strH = NULL;
	}
	
	FrmDoDialog( pAboutForm );
	FrmDeleteForm( pAboutForm );
	pAboutForm = NULL;
	
	FrmRestoreActiveState ( &curFrmState );
	
	if ( pOldForm )
	{
		FrmSetActiveForm( pOldForm );
		// FrmDrawForm(pOldForm);
	}
	
	return;
	
} // ShowAboutForm()

/*
 * RomVersionCompatible
 */
Err RomVersionCompatible( UInt32 requiredVersion, UInt16 launchFlags )
{	
	UInt32 	rom = 0;
	UInt32 	hal = 0;
	UInt32	company = 0;
	UInt32	device = 0;
	UInt32	hsFtrVersion;

	if( FtrGet( hsFtrCreator, hsFtrIDVersion, &hsFtrVersion ) != 0 )
		return( 1 );

    FtrGet( sysFtrCreator, sysFtrNumOEMHALID, &hal );
    FtrGet( sysFtrCreator, sysFtrNumOEMCompanyID, &company );
    FtrGet( sysFtrCreator, sysFtrNumOEMDeviceID, &device );
	FtrGet( sysFtrCreator, sysFtrNumROMVersion, &rom );
		
	if (rom >= requiredVersion
		&&	(company == kPalmCompanyIDPalm
			|| company == kPalmCompanyIDOldPalm
			|| kPalmCompanyIDHandspring)

		&&	(device == kPalmOneDeviceIDTreo650
			|| device == kPalmOneDeviceIDTreo650Sim

			|| device == kPalmOneDeviceIDTreo600
			|| device == kPalmOneDeviceIDTreo600Sim

			|| device == 'D053' // Treo 680
			|| device == 'H104'

			|| device == 'D052' // Treo 700p
			|| device == 'H103'

			|| device == 'D060' // Treo 755p
			|| device == 'H104'
			
			|| device == 'D061' // Centro ???
			|| device == 'D062' // Centro
			)
			
		&&	(hal == hsHALIDHandspringOs5Rev2 // Treo 650
			|| hal == hsHALIDHandspringOs5Rev2Sim

			|| hal == hsHALIDHandspringOs5Rev1 // Treo 600
			|| hal == hsHALIDHandspringOs5Rev1Sim

			|| hal == hsHALIDHandspringOs5Rev4 // Treo 680, 755p
			|| hal == hsHALIDHandspringOs5Rev4Sim

			|| hal == hsHALIDHandspringOs5Rev3 // Treo 700p
			|| hal == hsHALIDHandspringOs5Rev3Sim
			
			|| hal == 'P052' // Centro ???
			|| hal == 'P053' // Centro
		))
	{
		return 0;
	}
	
	if ( ( launchFlags & LAUNCH_FLAGS ) == LAUNCH_FLAGS )
	{
		// Pilot 1.0 will continuously relaunch this app unless we switch to 
		// another safe one.
		if ( rom < sysMakeROMVersion(2, 0, 0, sysROMStageRelease, 0 ) )
			AppLaunchWithCommand (sysFileCDefaultApp, 
					sysAppLaunchCmdNormalLaunch, NULL);
	}
	
	return ( sysErrRomIncompatible );
	
} // RomVersionCompatible

/***********************************************************************
 *
 * FUNCTION:    DateCompare
 *
 * DESCRIPTION: This routine compares two dates.
 *
 * PARAMETERS:  d1 - a date 
 *              d2 - a date 
 *
 * RETURNED:    if d1 > d2  returns a positive int
 *              if d1 < d2  returns a negative int
 *              if d1 = d2  returns zero
 *
 * NOTE: This routine treats the DateType structure like an unsigned int,
 *       it depends on the fact the the members of the structure are ordered
 *       year, month, day form high bit to low low bit.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/12/95		Initial Revision
 *
 ***********************************************************************/
Int16 DateCompare (DateType d1, DateType d2)
{
	UInt16 int1, int2;
	
	int1 = DateToInt(d1);
	int2 = DateToInt(d2);
	
	if (int1 > int2)
		return (1);
	else if (int1 < int2)
		return (-1);
	return 0;
}

/***********************************************************************
 *
 * FUNCTION:    TimeCompare
 *
 * DESCRIPTION: This routine compares two times.  "No time" is represented
 *              by minus one, and is considered less than all times.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    if t1 > t2  returns a positive int
 *              if t1 < t2  returns a negative int
 *              if t1 = t2  returns zero
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/12/95		Initial Revision
 *
 ***********************************************************************/
Int16 TimeCompare (TimeType t1, TimeType t2)
{
	Int16 int1, int2;
	
	int1 = TimeToInt(t1);
	int2 = TimeToInt(t2);

	if (int1 > int2)
		return (1);
	else if (int1 < int2)
		return (-1);
	return 0;
}
 
/*
 * Utils.c
 */