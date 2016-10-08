/*
 * Bitmap.c
 */
 
#ifndef __BITMAPS_H__
#define __BITMAPS_H__

#include <Hs.h>
#include <HsPhone.h>
#include <HsNav.h>
#include <HsExt.h>
#include <palmOneResources.h>
#include <PalmTypes.h>

#include "AppResources.h"

// Prototypes
extern BitmapType* 				GetBitmap(DmResID resID, Coord* widthP, Coord* heightP, MemHandle* bmpHP);
extern void 					ReleaseBitmap(MemHandle* bmpHP);

#endif /* __BITMAPS_H__ */

/*
 * Bitmap.c
 */