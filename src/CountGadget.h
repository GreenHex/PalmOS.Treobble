/*
 * CountGadget.h
 */

#ifndef __COUNTGADGETS_H__
#define __COUNTGADGETS_H__

#include <Hs.h>
#include <HsPhone.h>
#include <HsCreators.h>
#include <palmOneResources.h>
#include <PalmTypes.h>
#include <Window.h>
#include <Form.h>
#include <UIColor.h>
#include <StringMgr.h>
#include <HsExtCommon.h>

#include "Sections.h"
#include "Utils.h"
#include "Bitmaps.h"
#include "AddrDB.h"
#include "Scroll.h"
#include "AppResources.h"

#define SCROLL_POSITION_GADGET_HEIGHT		140

extern Int16								gTopRow;
extern UInt16								gNumContactsRecords;
extern UInt16								gNumTableRows;
extern UInt16								gSelectedContact;
extern Char									gSearchStr[SEARCH_STR_MAX_LEN];
extern EventData_t*							gEventDataP;

// Prototypes
extern void 					InitializeCountGadget( FormType* pForm, UInt16* rowCountP, UInt16* scrollPosP );
extern Boolean 					CountGadgetHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP );

#endif /* __COUNTGADGETS_H__ */
 
/*
 * CountGadget.h
 */