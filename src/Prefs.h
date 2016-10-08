/*
 * Prefs.h
 */
 
#ifndef __PREFS_H__
#define __PREFS_H__

#include <Hs.h>
#include <HsNav.h>
#include <PalmOS.h>
#include <palmOneResources.h>
#include <PalmTypes.h>
#include <Window.h>

#include "Global.h"

#define ROW_INVALID					-1

#define INITIAL_SPEED				10
#define THRESHOLD_STOP				8
#define THRESHOLD_SLOW				40
#define THRESHOLD_MEDIUM			80
#define THRESHOLD_FAST				120
#define POPUP_DELAY					30

// Unsaved
typedef struct {
	UInt16					topRow;
	UInt16					categoryIdx;
	UInt16					selectedRow;
} UnsavedPreferences_t;

// Saved

typedef struct {
	RGBColorType 			background;
	RGBColorType 			foreground;
	RGBColorType 			color1;
	RGBColorType 			color2;
	RGBColorType			color3;
	Boolean					bSoundClicks;
	Boolean					bSlowToAStop;
} Colors_t;

typedef struct {
	UInt16					initialSpeed;
	Int16					thresholdStop;
	Int16					thresholdSlow;
	Int16					thresholdMedium;
	Int16					thresholdFast;
	Boolean					reverseScroll;
	Int16					popupDelay;
} Settings_t;

typedef struct {
	Boolean					bEnabled;
	Settings_t				Settings;
	Colors_t				Colors;
} AppPreferences_t;

// Prototypes
extern void 				WriteUsPrefs( UnsavedPreferences_t* pUsPrefs );
extern void 				ReadUsPrefs( UnsavedPreferences_t* pUsPrefs );
extern void 				InitializeUsPrefs( UnsavedPreferences_t* pUsPrefs );
extern void 				WriteAppPrefs( AppPreferences_t* pPrefs );
extern void 				ReadAppPrefs( AppPreferences_t* pPrefs );
extern void 				InitializeAppPrefs( AppPreferences_t* pPrefs );
extern void 				InitializeSettingsPrefs( Settings_t* pSettings );
extern void 				InitializeColorPrefs(Colors_t* pColors);

#endif /* __PREFS_H__ */
 
/*
 * Prefs.h
 */