/*
 * CountGadget.c
 */

#include "CountGadget.h"

/*
 * InitializeCountGadget()
 */
void InitializeCountGadget(FormType* pForm, UInt16* rowCountP, UInt16* scrollPosP )
{
	if ( ( !pForm ) || ( !rowCountP ) || ( !rowCountP ) ) return;
	
	FrmSetActiveForm( pForm );
 			
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CONTACTS_COUNT_GADGET ), (FormGadgetHandlerType *) CountGadgetHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, VOLUME_GADGET ), (FormGadgetHandlerType *) CountGadgetHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, SCROLL_POSITION_GADGET ), (FormGadgetHandlerType *) CountGadgetHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, SELECTED_CONTACT_INDICATOR ), (FormGadgetHandlerType *) CountGadgetHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, SEARCH_STRING_GADGET ), (FormGadgetHandlerType *) CountGadgetHandler );
	
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CONTACTS_COUNT_GADGET ), rowCountP );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, VOLUME_GADGET ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, SCROLL_POSITION_GADGET ), scrollPosP );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, SELECTED_CONTACT_INDICATOR ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, SEARCH_STRING_GADGET ), NULL );
	
	return;
	
} // InitializeCountGadget()

/* 
 * CountGadgetHandler()
 */
Boolean CountGadgetHandler(FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP)
{
	Boolean 				handled = false;
	FormType*				pForm = FrmGetActiveForm();
	RectangleType			rect = gadgetP->rect;
	RGBColorType			rgbWhite = { 0, 255, 255, 255 };
	// RGBColorType			rgbPaleYellow = { 0, 255, 255, 204 };
	RGBColorType			rgbGrey = { 0, 85, 85, 85 };
	RGBColorType			rgbLightGrey = /* { 0, 187, 187, 187 }; */ { 0, 221, 221, 221 };
	RGBColorType			rgbRed = { 0, 204, 0, 51 };
	RGBColorType			rgbDarkRed = { 0, 153, 0, 0 };
	RGBColorType			rgbOrange = { 0, 255, 153, 102 };
	RGBColorType			rgbYellow = { 0, 255, 204, 51 };
	RGBColorType			rgbBlue = { 0, 0, 0, 204 };
	RGBColorType			rgbGreen = { 0, 0, 151, 51 };
	IndexedColorType		colorIdxWhite;
	// IndexedColorType		colorIdxPaleYellow;
	IndexedColorType		colorIdxGrey;
	IndexedColorType		colorIdxLightGrey;
	IndexedColorType 		colorIdxRed;
	IndexedColorType 		colorIdxDarkRed;
	IndexedColorType 		colorIdxOrange;
	IndexedColorType 		colorIdxYellow;
	IndexedColorType 		colorIdxBlue;
	IndexedColorType 		colorIdxGreen;
	Char					numStr[maxStrIToALen] = "\0";
	Char					str[5] = "\0";
	UInt16					strHeight = 0;
	UInt16					strWidth = 0;
	static UInt16			oldRowCount = -1;
	static ToneVolume		oldVolume = toneVolumeOff;
	static UInt16			oldMuteSwitchPosition = -1;
	static UInt16			oldTopRow = -1;
	static UInt16			oldSelectedContact = -1;
	
	switch (cmd )
	{
		case formGadgetDeleteCmd:
			handled = true;
			break;
			
		case formGadgetEraseCmd:
			handled = true;
			break;		
		
		case formGadgetHandleEventCmd:
		case formGadgetDrawCmd:
	
			FrmSetActiveForm( pForm );

			WinPushDrawState();
			
			WinSetDrawMode( winOverlay );
	
			colorIdxWhite = WinRGBToIndex( & rgbWhite );
			// colorIdxPaleYellow = WinRGBToIndex( &rgbPaleYellow );
			colorIdxGrey = WinRGBToIndex( &rgbGrey );
			colorIdxLightGrey = WinRGBToIndex( &rgbLightGrey );
			colorIdxRed = WinRGBToIndex( &rgbRed );
			colorIdxDarkRed = WinRGBToIndex( &rgbDarkRed );
			colorIdxOrange = WinRGBToIndex( &rgbOrange );
			colorIdxYellow = WinRGBToIndex( &rgbYellow );
			colorIdxBlue = WinRGBToIndex( &rgbBlue );
			colorIdxGreen = WinRGBToIndex( &rgbGreen );
			
			gadgetP->attr.visible = true;
							
			switch ( gadgetP->id )
			{
				case CONTACTS_COUNT_GADGET:
					{
						UInt16*					rowCountP = (UInt16 *) gadgetP->data;
					
						if ( oldRowCount == *rowCountP ) break;
						
						oldRowCount = *rowCountP;
										
						WinSetBackColor( colorIdxLightGrey );
						WinSetForeColor( colorIdxLightGrey );
						WinSetTextColor( colorIdxGrey );
										
						// Patch
						// WinEraseRectangle( &rect, 0 );
						WinDrawRectangle( &rect, 2 );
						
						// Text
						StrIToA( numStr, (Int32) *rowCountP );
						StrNCopy( str, numStr, 4 );
						str[4] = chrNull;
						
						FntSetFont( stdFont );
						strHeight = FntLineHeight();
						strWidth = FntLineWidth( str, StrLen( str ) );
						
						WinPaintChars( str, StrLen( str ), rect.topLeft.x + ( ( rect.extent.x - strWidth ) / 2 ), rect.topLeft.y );				
						
						// Outline
						// WinDrawRectangleFrame(rectangleFrame, &rect);
					}
					break; 
					
				case VOLUME_GADGET:
					{
						UInt16					muteSwitchPosition = hsAttrRingSwitchMute;
						ToneVolume	 			volume = GetPhoneVolume( NULL, NULL );						
						MemHandle 				BmpH = 0;
						BitmapType* 			BmpP = 0;
						Coord					BmpSizeX = 0;
						Coord					BmpSizeY = 0;
						
						if ( HsAttrGet( hsAttr68KRingSwitch, 0, (UInt32 *) &muteSwitchPosition ) == errNone )
						{
							if ( muteSwitchPosition == hsAttrRingSwitchMute )
							{
								WinSetTextColor( colorIdxRed );
								StrCopy( str, "X" );
								str[2] = chrNull;
							}
							else
							{
								if ( volume < toneVolume2 )
								{
									WinSetTextColor( colorIdxRed );
								}
								else
								{
									WinSetTextColor( colorIdxGreen );
								} 
								
								StrIToA( numStr, (Int32) volume );
								StrNCopy( str, numStr, 4 );
								str[2] = chrNull;
							}
						}
						
						if ( ( oldVolume == volume ) && ( oldMuteSwitchPosition == muteSwitchPosition ) ) break;
						
						oldVolume = volume;
						oldMuteSwitchPosition = muteSwitchPosition;
						
						// Outer...
						// WinSetForeColor( colorIdxLightGrey );
						// WinDrawRectangle( &rect, 2 );
						
						BmpP = GetBitmap( VOLUME_BMP, &BmpSizeX, &BmpSizeY, &BmpH );
						WinDrawBitmap( BmpP, rect.topLeft.x, rect.topLeft.y );
						ReleaseBitmap( &BmpH );	
						BmpH = NULL;
						BmpP = NULL;
						
						WinSetBackColor( colorIdxWhite );
						
						FntSetFont( stdFont );
						strHeight = FntLineHeight();
						strWidth = FntLineWidth( str, StrLen( str ) );

						WinPaintChars( str, StrLen( str ), rect.topLeft.x + ( rect.extent.x - ( strWidth + 1 ) ), rect.topLeft.y );	
						
					}
					break;
					
				case SCROLL_POSITION_GADGET:
					{
						UInt16					topRow = *(UInt16 *) gadgetP->data;
						RectangleType			rectBox;
									
						// if ( ( oldTopRow == topRow ) && ( !gEventDataP->speed ) ) { break; }
						
						oldTopRow = topRow; 
												
						rectBox.topLeft.x = rect.topLeft.x;
						if ( gNumContactsRecords > gNumTableRows )
						{
							rectBox.topLeft.y = rect.topLeft.y + ( ( ( SCROLL_POSITION_GADGET_HEIGHT - 1 ) * topRow ) / ( gNumContactsRecords - gNumTableRows ) );
						}
						else
						{
							rectBox.topLeft.y = rect.topLeft.y + ( SCROLL_POSITION_GADGET_HEIGHT / 2 );
						}
						
						WinEraseLine( rect.topLeft.x + 1, rect.topLeft.y, 
										rect.topLeft.x + 1, rect.topLeft.y + rect.extent.y);
						
						WinSetForeColor( colorIdxLightGrey );
						WinDrawLine( rect.topLeft.x, rect.topLeft.y, 
										rect.topLeft.x, rect.topLeft.y + rect.extent.y);
						
						rectBox.extent.x = rectBox.extent.y = 2;						
						
						switch ( gEventDataP->speed )
						{
							case high:
								WinSetForeColor( colorIdxDarkRed );
								break;
							
							case medium:
								WinSetForeColor( colorIdxOrange );
								break;
								
							case low:
								WinSetForeColor( colorIdxYellow );
								break;
								
							default:
								WinSetForeColor( colorIdxGreen );
								break;
							
						}								
						
						if ( ( !topRow ) 
								|| ( ( topRow + gNumTableRows ) == gNumContactsRecords ) )
						{ 
							WinSetForeColor( colorIdxGreen );
						}
						
						WinDrawRectangle( &rectBox, 0 );	
					}
					break;
					
				case SELECTED_CONTACT_INDICATOR:
					{
						RectangleType			selContactRect = { { rect.topLeft.x, 
																		( rect.topLeft.y + 9 ) + ( TABLE_ROW_HEIGHT * gSelectedContact ) },
																		{ 2, 3 } };
						
						WinEraseRectangle( &rect, 0 );
						
						// if ( oldSelectedContact == gSelectedContact ) break;
						
						oldSelectedContact = gSelectedContact;
						
						if ( !contacts[gTopRow] ) break;

						WinSetForeColor( colorIdxRed );	
						WinDrawRectangle( &selContactRect, 0 );				
					}
					break;
					
				case SEARCH_STRING_GADGET:
					{
						UInt16			searchStrLen = StrLen( gSearchStr );
						Char			searchStrUCChar[2] = "\0";
						// RGBColorType 	formBackColorRGB;
						
						// UIColorGetTableEntryRGB ( UIFormFill, formBackColorRGB );
						
						// WinSetBackColor( colorIdxWhite );
						WinSetForeColor( colorIdxLightGrey );

						WinEraseRectangle( &rect, 0 );
						// WinEraseRectangleFrame( rectangleFrame, &rect );
						
						if ( !searchStrLen )
						{
							gadgetP->attr.visible = false;
							FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_CATEGORY_POPUP ) );
							CtlSetUsable( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, SEARCH_STRING_GADGET ) ), false );
							break;
						}
						else if ( searchStrLen == 1 )
						{
							searchStrUCChar[0] = gSearchStr[0] - 0x0020;
							searchStrUCChar[1] = chrNull;
							WinSetTextColor( colorIdxRed );
						}
						else /* > 1 */
						{
							WinSetTextColor( colorIdxBlue );	
						}
						
						WinDrawRectangle( &rect, 2 );
						
						FntSetFont( stdFont );
						strHeight = FntLineHeight();
						strWidth = FntLineWidth( gSearchStr, searchStrLen );
	
						WinPaintChars( ( searchStrLen == 1 )? searchStrUCChar : gSearchStr, 
											searchStrLen, rect.topLeft.x + ( ( rect.extent.x - strWidth ) / 2 ), rect.topLeft.y );
						
						// Outline
						// WinSetForeColor( colorIdxLightGrey );
						// WinDrawRectangleFrame( rectangleFrame, &rect );
						CtlSetUsable( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, SEARCH_STRING_GADGET ) ), true );
					}
								
					break;
					
				default:
					break;
			}		
			
			WinPopDrawState();
			
			handled = true; 
			
			
			break;
			
		default:
			break;
		
	}
	
	return ( handled );
	
} // CountGadgetHandler()

/*
 * CountGadget.c
 */