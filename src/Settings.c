/*
 * KeyGuard.c
 */
 
#include "Settings.h"

// Prototypes
// Code 2 // EXTRA_SECTION_ONE
static void 					InitializeSettingsForm( FormType* pForm, Settings_t* pSettings ) 	EXTRA_SECTION_ONE;
static void 					CopySettingsPrefs( FormType* pForm, Settings_t* pSettings ) 		EXTRA_SECTION_ONE;

/*
 * ShowSettingsForm()
 */
Boolean ShowSettingsForm( Settings_t* pSettings )
{
	Boolean 		retVal = false;
	FormType*		pForm = NULL;
	
	pForm = FrmInitForm( SETTINGS_FORM );
	
	FrmSetActiveForm( pForm );
	
	UIColorSetTableEntry( UIFieldBackground, &gOldFieldColorRGB );
	
	InitializeSettingsForm( pForm, pSettings );
	
	retVal = ( FrmDoDialog( pForm ) == SETTINGS_DONE_BUTTON );
	
	if ( retVal )
	{
		CopySettingsPrefs( pForm, pSettings );
	}
	
	FrmEraseForm(pForm);
	FrmDeleteForm(pForm);
	
	pForm = NULL;
	
	return ( retVal );
	
} // ShowSettingsForm()

/*
 * InitializeSettingsForm()
 */
static void InitializeSettingsForm( FormType* pForm, Settings_t* pSettings )
{
	ControlType*		pCtl = NULL;
	
	if ( ( !pForm ) || ( !pSettings ) ) return;

	SetFieldTextFromInt( pForm, SETTINGS_INITIAL_SPEED_FIELD, pSettings->initialSpeed );	
	SetFieldTextFromInt( pForm, SETTINGS_THRESHOLD_STOP_FIELD, pSettings->thresholdStop );	
	SetFieldTextFromInt( pForm, SETTINGS_THRESHOLD_SLOW_FIELD, pSettings->thresholdSlow );	
	SetFieldTextFromInt( pForm, SETTINGS_THRESHOLD_MEDIUM_FIELD, pSettings->thresholdMedium );	
	SetFieldTextFromInt( pForm, SETTINGS_THRESHOLD_FAST_FIELD, pSettings->thresholdFast );
	SetFieldTextFromInt( pForm, SETTINGS_POPUP_DELAY_FIELD, pSettings->popupDelay );	
	
	pCtl = (ControlType *) FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, SETTINGS_REVERSE_SCROLL_CHECKBOX ) );
	CtlSetValue( pCtl, pSettings->reverseScroll );
	
	return;
	
} // InitializeSettingsForm()

/*
 * CopySettingsPrefs()
 */
static void CopySettingsPrefs( FormType* pForm, Settings_t* pSettings )
{
	ControlType*		pCtl = NULL;
	
	if ( ( !pForm ) || ( !pSettings ) ) return;

	pSettings->initialSpeed = GetNumFromFieldText( pForm, SETTINGS_INITIAL_SPEED_FIELD );
	if ( !( pSettings->initialSpeed ) ) pSettings->initialSpeed = INITIAL_SPEED;
		
	pSettings->thresholdStop = GetNumFromFieldText( pForm, SETTINGS_THRESHOLD_STOP_FIELD );	
	if ( !( pSettings->thresholdStop ) ) pSettings->thresholdStop = THRESHOLD_STOP;
	
	pSettings->thresholdSlow = GetNumFromFieldText( pForm, SETTINGS_THRESHOLD_SLOW_FIELD );	
	if ( !( pSettings->thresholdSlow ) ) pSettings->thresholdSlow = THRESHOLD_SLOW;
	
	pSettings->thresholdMedium = GetNumFromFieldText( pForm, SETTINGS_THRESHOLD_MEDIUM_FIELD );	
	if ( !( pSettings->thresholdMedium ) ) pSettings->thresholdMedium = THRESHOLD_MEDIUM;
	
	pSettings->thresholdFast = GetNumFromFieldText( pForm, SETTINGS_THRESHOLD_FAST_FIELD );	
	if ( !( pSettings->thresholdFast ) ) pSettings->thresholdFast = THRESHOLD_FAST;
	
	pSettings->popupDelay = GetNumFromFieldText( pForm, SETTINGS_POPUP_DELAY_FIELD );
	if ( !( pSettings->popupDelay ) ) pSettings->popupDelay = POPUP_DELAY;

	pCtl = (ControlType *) FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, SETTINGS_REVERSE_SCROLL_CHECKBOX ) );
	pSettings->reverseScroll = ( CtlGetValue( pCtl ) == 1 );
	
	return;	

} // CopySettingsPrefs()

/*
 * KeyGuard.c
 */