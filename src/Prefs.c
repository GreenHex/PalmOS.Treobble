/*
 * Prefs.c
 */

#include "Prefs.h"

/*
 * WriteUsPrefs
 */
void WriteUsPrefs( UnsavedPreferences_t* pUsPrefs )
{
	if ( !pUsPrefs ) return;
	
	PrefSetAppPreferences( appFileCreator, appPrefID, appPrefVersionNum, pUsPrefs, sizeof( UnsavedPreferences_t ), false );

	return;
	
} // WriteUsPrefs

/*
 * ReadUsPrefs
 */
void ReadUsPrefs( UnsavedPreferences_t* pUsPrefs )
{	
	UInt16 		usPrefSize = sizeof( UnsavedPreferences_t );
	UInt16		usPrefVersion = 0;
	
	if ( !pUsPrefs ) return;
	
	usPrefVersion = PrefGetAppPreferences( appFileCreator, appPrefID, pUsPrefs, &usPrefSize, false );
	
	if ( usPrefVersion != appPrefVersionNum )
	{
		// clear old prefs...
		PrefSetAppPreferences( appFileCreator, appPrefID, usPrefVersion, NULL, 0, false );
		
		InitializeUsPrefs( pUsPrefs );
		WriteUsPrefs( pUsPrefs );
	}
	
	return;
		
}// ReadUsPrefs

/*
 * InitializeUsPrefs()
 */
void InitializeUsPrefs( UnsavedPreferences_t* pUsPrefs )
{
	if ( !pUsPrefs ) return;

	pUsPrefs->topRow = 0;
	pUsPrefs->categoryIdx = dmAllCategories;
	pUsPrefs->selectedRow = 0;
	
	return;
	
} // InitializeUsPrefs()

/*
 * WriteAppPrefs
 */
void WriteAppPrefs( AppPreferences_t* pPrefs )
{
	if ( !pPrefs ) return;
	
	PrefSetAppPreferences( appFileCreator, appPrefID, appPrefVersionNum, pPrefs, sizeof( AppPreferences_t ), true );

	return;
	
} // WriteAppPrefs

/*
 * ReadAppPrefs
 */
void ReadAppPrefs( AppPreferences_t* pPrefs )
{	
	UInt16 		prefSize = sizeof( AppPreferences_t );
	UInt16		prefVersion = 0;
	
	if ( !pPrefs ) return;
	
	prefVersion = PrefGetAppPreferences( appFileCreator, appPrefID, pPrefs, &prefSize, true );
	
	if ( prefVersion != appPrefVersionNum )
	{
		// clear old prefs...
		PrefSetAppPreferences( appFileCreator, appPrefID, prefVersion, NULL, 0, true );
		
		InitializeAppPrefs( pPrefs );
		WriteAppPrefs( pPrefs );
	}
	
	return;
		
}// ReadAppPrefs

/*
 * InitializeAppPrefs()
 */
void InitializeAppPrefs(AppPreferences_t* pPrefs)
{
	if (!pPrefs) return;

	pPrefs->bEnabled = true;
	InitializeSettingsPrefs( &( pPrefs->Settings ) );
	InitializeColorPrefs( &( pPrefs->Colors) );
	
	return;
	
} // InitializeAppPrefs()

/*
 * InitializeSettingsPrefs(sounds_t* pSnd)
 */
void InitializeSettingsPrefs( Settings_t* pSettings )
{	
	if ( !pSettings ) return;
	
	pSettings->initialSpeed = INITIAL_SPEED;
	pSettings->thresholdStop = THRESHOLD_STOP;
	pSettings->thresholdSlow= THRESHOLD_SLOW;
	pSettings->thresholdMedium = THRESHOLD_MEDIUM;
	pSettings->thresholdFast = THRESHOLD_FAST;
	pSettings->reverseScroll = false;
	pSettings->popupDelay = POPUP_DELAY;
		
	return;
	
} // InitializeSettingsPrefs()

/*
 * InitializeColorPrefs()
 */
void InitializeColorPrefs(Colors_t* pColors)
{
	if (!pColors) return;
	
	pColors->background.r = 255 ; pColors->background.g = 255 ; pColors->background.b = 204 ; // white
	pColors->foreground.r = 68 ; pColors->foreground.g = 68 ; pColors->foreground.b = 68 ; // // grey
	pColors->color1.r = 0 ; pColors->color1.g = 51 ; pColors->color1.b = 204 ; // blue
	pColors->color2.r = 221 ; pColors->color2.g = 221 ; pColors->color2.b = 221 ; // grey
	pColors->color3.r = 200 ; pColors->color3.g = 200 ; pColors->color3.b = 200 ; // grey
	pColors->bSoundClicks = true;
	pColors->bSlowToAStop = true;
	
	return;
	
} // InitializeColorPrefs()


/*
 * Prefs.c
 */