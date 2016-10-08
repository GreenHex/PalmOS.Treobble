/*
 * Bitmaps.c
 */

#include "Bitmaps.h"

/*
 * getBitmap
 */
BitmapType* GetBitmap(DmResID resID, Coord* widthP, Coord* heightP, MemHandle* bmpHP)
{
	BitmapType* 		pBmp = NULL;
	
	*bmpHP = DmGetResource(bitmapRsc, resID); 
	pBmp = MemHandleLock(*bmpHP);

	BmpGetDimensions(pBmp, widthP, heightP, NULL);
		
	return (pBmp);
	
} // getBitmap

/*
 * releaseBitmap
 */
void ReleaseBitmap(MemHandle* bmpHP)
{
	if (!bmpHP) return;
	
	MemHandleUnlock(*bmpHP);
	DmReleaseResource(*bmpHP);
	
	return;
	
} // releaseBitmmap

/*
 * Bitmaps.c
 */