/*
 * Tables.c
 */

#include "Tables.h"

// Prototypes
void 					TableDrawItem(void* pTbl, Int16 row, Int16 column, RectangleType* rectP);
			
/*
 * TableDrawItem
 */
void TableDrawItem( void* pTbl, Int16 row, Int16 column, RectangleType* rectP )
{
	Char					str[NAME_LENGTH] = "\0";
    UInt16					strHeight = 0;
    UInt16					strWidth = 0;

	WinPushDrawState();
	
	// Background
   	UIColorSetTableEntry( UIFieldBackground, &gBackColorRGB );
    // Foreground
    WinSetForeColor( gSep1ColorIdx );
	// Text
    WinSetTextColor( gTextColorIdx );
       
	if ( contacts[row + gTopRow] )
	{
		StrNCopy( str, contacts[row + gTopRow]->name, NAME_LENGTH - 1 );
		str[NAME_LENGTH - 1] = chrNull;
		
		FntSetFont( largeBoldFont );
		strHeight = FntLineHeight();
		strWidth = FntLineWidth( str, StrLen( str ) );
		
		WinDrawLine( rectP->topLeft.x,
						rectP->topLeft.y + rectP->extent.y - 1,
						rectP->topLeft.x + rectP->extent.x,
						rectP->topLeft.y + rectP->extent.y - 1);
						
		WinDrawTruncChars( str, StrLen( str ), 
							rectP->topLeft.x + 3,
							rectP->topLeft.y + ( rectP->extent.y - strHeight ) / 2,
							NAME_PIXELS_WIDTH );
	}
		
	WinPopDrawState();
	
	return;
	
} // TableDrawItem

/*
 * TableInit
 */
void TableInit( TableType* pTbl )
{ 
	UInt16				row = 0;
		
	if ( !pTbl ) return;
	
	gNumTableRows = TblGetNumberOfRows( pTbl );
	
	for ( row = 0; row < gNumTableRows; row++ )
	{		
		TblSetItemStyle( pTbl, row, 0, tallCustomTableItem );
		
		TblSetRowHeight( pTbl, row, TABLE_ROW_HEIGHT );
		TblSetRowUsable( pTbl, row, true );
		TblSetRowMasked( pTbl, row, false );
		TblSetRowSelectable( pTbl, row, true );
	} 
	 
	TblSetColumnUsable( pTbl, 0, true );
	TblSetColumnMasked( pTbl, 0, false );
	
	TblSetCustomDrawProcedure( pTbl, 0, TableDrawItem );
	
	TblHasScrollBar( pTbl, false ); 
	// TblMarkTableInvalid( pTbl );
	
	return;
						
} // TableInit

/*
 * Tables.c
 */