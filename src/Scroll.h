/*
 * Scroll.h
 */

#ifndef __SCROLL_H__
#define __SCROLL_H__

#include <Hs.h>
#include <Chars.h>

#include "Global.h"
#include "Prefs.h"
#include "AddrDB.h"
#include "Dial.h"
#include "AppResources.h"



typedef struct { 
   eventsEnum 				eType; 
   Boolean 					penDown; 
   UInt8 					tapCount; 
   Int16 					screenX; 
   Int16 					screenY; 
   union { 
      struct _GenericEventType generic; // sets size of union 
      struct { 
         UInt16				tableID;
         UInt16 			updateCode;
      } tblData; 
   } data; 
} TableUpdateEvent_t;

TableUpdateEvent_t			tblUpdateEvt;

/*
firstUserEvent = 0x6000,
lastUserEvent  = 0x7FFF
*/  
#define tblUpdate			0x6789

typedef struct {
	Settings_t				Settings;
	Boolean					bSoundClicks;
	Boolean					bSlowToAStop;
	UInt32					ticksLastNilEvt;
	UInt32					ticksTimePD;
	UInt32					ticksTimePU;
	UInt32					stopDelay;
	PointType				ptPD;
	PointType				ptPM;
	Direction_e				direction;
	Speed_e					speed;
	EventType				tblUpdateEvt;
	EventType				gadgetUpdateEvt;
} EventData_t;

extern UInt16				gNumTableRows;
extern Int16				gTopRow;
extern UInt16				gNumContactsRecords;
extern UInt16				gSelectedRow;
extern Boolean				gRowInverted;
extern RectangleType		gRowRect;
extern UInt16				gSelectedContact;
extern UInt16				gCharLocation[(UInt16) 26];
extern Char					gSearchStr[SEARCH_STR_MAX_LEN];

EventData_t*				gEventDataP;

// Prototypes
extern Boolean 				ProcessScrollEvent( FormType* pForm, EventType* pEvent );
extern void					InitEventData( void );
extern Direction_e 			GetSpeed( void );

#endif /* __SCROLL_H__ */

/*
 * Scroll.h
 */