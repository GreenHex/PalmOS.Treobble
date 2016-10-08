/*
 * DrawPE.c
 */

#include "DrawPE.h"

/*
 * DrawPenInfo()
 */
void DrawPenInfo( EventType* pEvent, Coord y )
{
	Char				strScrX[32] = "\0";
	Char				strScrY[32] = "\0";
	Char				strTapCount[32] = "\0";
	Char				str[100] = "\0";
	RectangleType		rect = { { 0 , y } , { 160 , 12 } };
	UInt16				strHeight = 0;
	UInt16				strWidth = 0;
	
	StrIToA( strScrX, pEvent->screenX );
	StrIToA( strScrY, pEvent->screenY );
	StrIToA( strTapCount, pEvent->tapCount );
	
	if ( pEvent->eType == penUpEvent )
	{
		StrCopy( str, "PU: x: " );
	}
	else if ( pEvent->eType == penDownEvent )
	{
		StrCopy( str, "PD: x: " );
	}
	else if ( pEvent->eType == penMoveEvent )
	{
		StrCopy( str, "PM: x: " );
	}
	
	StrCat( str, strScrX );
	StrCat( str, " y: " );
	StrCat( str, strScrY );
	StrCat( str, " c: " );
	StrCat( str, strTapCount );
	
	strHeight = FntLineHeight();
	strWidth = FntLineWidth( str, StrLen( str ) );
	
	WinEraseRectangle( &rect, 0 );
	WinDrawChars( str,
					StrLen(str),
					rect.topLeft.x + ( rect.extent.x - strWidth ) / 2,
					y + 1 );
					
	return;
	
} // DrawPenInfo()

/*
 * DrawPE.c
 */