/*
 * KeyGuard.h
 */
 
#ifndef __KEYGUARD_H__
#define __KEYGUARD_H__

#include <Hs.h>
#include <HsPhone.h>
#include <HsNav.h>
#include <HsExt.h>
#include <palmOneResources.h>
#include <Form.h>
#include <Field.h>
#include <StringMgr.h>

#include "Sections.h"
#include "Prefs.h"
#include "Utils.h"
#include "FormUtils.h"

#define SETTINGS_FORM						3000
#define SETTINGS_DONE_BUTTON				3001
#define SETTINGS_CANCEL_BUTTON				3002
#define SETTINGS_INITIAL_SPEED_FIELD		3010
#define SETTINGS_THRESHOLD_STOP_FIELD		3020
#define SETTINGS_THRESHOLD_SLOW_FIELD		3030
#define SETTINGS_THRESHOLD_MEDIUM_FIELD		3040
#define SETTINGS_THRESHOLD_FAST_FIELD		3050
#define SETTINGS_REVERSE_SCROLL_CHECKBOX	3060
#define SETTINGS_POPUP_DELAY_FIELD			3070
			
// Prototypes
extern Boolean 								ShowSettingsForm(Settings_t* pSettings);
extern RGBColorType							gOldFieldColorRGB;

#endif /* __KEYGUARD_H__ */

/*
 * KeyGuard.h
 */