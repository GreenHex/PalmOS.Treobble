/*
 * Dial.h
 */

#ifndef __DIAL_H__
#define __DIAL_H__

#include <Hs.h>
#include <HsPhone.h>
#include <HsNav.h>
#include <HsExt.h>
#include <PalmTypes.h>
#include <palmOneResources.h>
#include <Form.h>
#include <Control.h>
#include <PalmOSGlue.h>
#include <CharLatin.h>

#include "AddrDB.h"

#define DIAL_FORM							5000
#define DIAL_CANCEL_BUTTON					5002
#define DIAL_NAME_LABEL						5010
#define DIAL_CONTACTS_GOTO_BUTTON			5020
#define DIAL_PHONE_ROOT						5100 // NOT A BUTTON!!!
#define DIAL_PHONE1_BUTTON					5101
#define DIAL_PHONE2_BUTTON					5102
#define DIAL_PHONE3_BUTTON					5103
#define DIAL_PHONE4_BUTTON					5104
#define DIAL_PHONE5_BUTTON					5105
#define DIAL_PHONE_PUSH_BTN					5201
#define DIAL_SMS_PUSH_BTN					5202
#define DIAL_EMAIL_PUSH_BTN					5203

#define PHN_BMP_WORK						4600
#define PHN_BMP_HOME						4601
#define PHN_BMP_FAX							4602
#define PHN_BMP_OTHER						4603
#define PHN_BMP_EMAIL			 			4604
#define PHN_BMP_MAIN						4605
#define PHN_BMP_PAGER						4606
#define PHN_BMP_MOBILE						4607

/*
#define DIAL_SEP_LEN				120
#define DIAL_SEP_LOC_X				4
#define DIAL_SEP1_LOC_Y				16
#define DIAL_SEP2_LOC_Y				35
#define DIAL_SEP3_LOC_Y				54
#define DIAL_SEP4_LOC_Y				73
#define DIAL_SEP5_LOC_Y				92
#define DIAL_SEP6_LOC_Y				111
*/

#define PHN_LOC_X							6
#define PHN1_LOC_Y							24
#define PHN2_LOC_Y							PHN1_LOC_Y + 19
#define PHN3_LOC_Y							PHN2_LOC_Y + 19
#define PHN4_LOC_Y							PHN3_LOC_Y + 19
#define PHN5_LOC_Y							PHN4_LOC_Y + 19

/*
#define PHN_TYPE_MAIN				" m"
#define PHN_TYPE_MOBILE				" m"
#define PHN_TYPE_WORK				" w"
#define PHN_TYPE_HOME				" h"
#define PHN_TYPE_PAGER				" p"
#define PHN_TYPE_FAX				" f"
#define PHN_TYPE_EMAIL				" e"
*/

#define DIAL_DISPLAY_NAME_STR_LEN			23
#define DIAL_DISPLAY_NUMBER_STR_LEN			18
#define DIALER_KILL_SECONDS					16

typedef enum {
	phone							= 0,
	messaging						= 1,
	email							= 2,
	addressbook						= 3
} DialType_e;

// extern IndexedColorType				sep2ColorIdx;

// Prototypes
extern Boolean 						ShowDialingForm( Contacts_t* pContact );

#endif /* __DIAL_H__ */
 
/*
 * Dial.h
 */