/*
 * Tables.h
 */

#ifndef __TABLES_H__
#define __TABLES_H__

#include <Hs.h>
#include <HsNav.h>
#include <HsExt.h>
#include <palmOneResources.h>
#include <Form.h>
#include <Field.h>
#include <Table.h>
#include <StringMgr.h>
#include <HsAppLaunchCmd.h>
#include <UIColor.h>
#include <HsExtCommon.h>

#include "Sections.h"
#include "AddrDB.h"
#include "DrawPE.h"

#define NAME_PIXELS_WIDTH			140
#define TABLE_ROW_HEIGHT			20

UInt16								gNumTableRows;
extern Int16						gTopRow;
extern RGBColorType					gBackColorRGB;
extern IndexedColorType				gBackColorIdx;
extern IndexedColorType				gTextColorIdx;
extern IndexedColorType				gSep1ColorIdx;
// extern IndexedColorType				gSep2ColorIdx;

// Prototypes
extern void 						TableInit( TableType* pTbl ) 		EXTRA_SECTION_ONE;

#endif /* __TABLES_H__*/
 
/*
 * Tables.h
 */