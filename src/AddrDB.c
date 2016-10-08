/*
 * AddrDB.c
 */
 
#include <Hs.h>
#include <HsAppLaunchCmd.h>
#include <DataMgr.h>

#include "AddrDB.h"

#define BitAtPosition(pos)                ((UInt32)1 << (pos))
#define GetBitMacro(bitfield, index)      ((bitfield) & BitAtPosition(index))

// Prototypes
static void 		StripExtraCharsFromLCStr( unsigned char* s );
static void 		PrvAddrDBUnpack(PrvAddrPackedDBRecord* src, AddrDBRecordPtr dest);
static Boolean 		CopyDetailsToArrayName( AddrDBRecordType* recordP, AddrDBRecordFlags* flagsP, Boolean bFirstNameFirst, Char** nameStrP );

/*
 * StripExtraCharsFromLCStr()
 */
static void StripExtraCharsFromLCStr( unsigned char* s )
{
	unsigned char c, *d = s;
	
	while ( ( c = *s++ ) )
		if ( TxtCharIsLower ( c ) )
			*d++ = c;
					
	*d = 0;

	return;	
	
} // StripExtraCharsFromLCStr()
 
/*
 * SearchStringInArray
 */
Boolean SearchStringInArray( Char* searchStr, UInt16* idx )
{
	Boolean			retVal = false;
	Char*			nameStrL = NULL; // searchStr should be lower-case
	
	*idx = 0;
	
	while ( contacts[*idx] )
	{
		nameStrL = MemPtrNew ( StrLen( contacts[*idx]->name ) + 1 );
		StrToLower( nameStrL, contacts[*idx]->name );
		
		StripExtraCharsFromLCStr( nameStrL );
		
		if ( StrStr( nameStrL, searchStr ) )
		{
			retVal = true;
			break;	
		}
		
		MemPtrFree( nameStrL );
		nameStrL = NULL;
		
		++ ( *idx );
	}
	
	return ( retVal );
	
} // SearchStringInArray()

/*
 * GetNumContactsRecords()
 */
UInt16	GetNumContactsRecords( UInt16 mode )
{
	UInt16				numRecords = 0;
	DmOpenRef			dbP = NULL;
	
	if ( ! AddrDBGetDatabase( &dbP, mode ) )
	{
		if ( dbP )
		{
			numRecords = DmNumRecordsInCategory( dbP, gCategoryIdx ); 
			
			DmCloseDatabase( dbP );
		}
	}
	
	return ( numRecords );
	
} // GetNumContactsRecords()

/*
 * PrvAddrDBUnpack()
 */
void PrvAddrDBUnpack( PrvAddrPackedDBRecord *src, AddrDBRecordPtr dest )
{
	Int16   	index;
	UInt32 		flags;
	char*		p;

	dest->options = src->options;
	flags = src->flags.allBits;
	p = &src->firstField;

	for ( index = firstAddressField; index < addressFieldsCount; index++ )
	{
		// If the flag is set point to the string else NULL
		if ( GetBitMacro(flags, index) != 0 )
		{
			dest->fields[index] = p;
			p += StrLen( p ) + 1;
		}
		else
		{
			dest->fields[index] = NULL;
		}
	}
	
	return;
	
} // PrvAddrDBUnpack()

/*
 * AddrDBGetRecord()
 * 
 * returns ACTUAL INDEX of the record in "recordIdxP" after all the manipulations...
 */
Err AddrDBGetRecord( DmOpenRef dbP, UInt16* recordIdxP, AddrDBRecordFlags* flagsP, AddrDBRecordPtr recordP, MemHandle* memHP )
{
	PrvAddrPackedDBRecord*		src;
	UInt16						idx = *recordIdxP;

	if ( !memHP ) return ( 1 );
	
	*memHP = NULL;

	*memHP = DmQueryNextInCategory( dbP, &idx, gCategoryIdx );
	
	if ( *memHP )
	{
		src = (PrvAddrPackedDBRecord *) MemHandleLock( *memHP );
		
		if (src == NULL)
			return ( dmErrIndexOutOfRange );
	
		PrvAddrDBUnpack( src, recordP );
	
		if ( flagsP )	
		{
			*flagsP = src->flags;
		}
		
		if ( recordIdxP )
		{
			*recordIdxP = idx;	
		}
	}
		
	return ( errNone );
	
} // AddrDBGetRecord()

/*
 * AddrDBGetDatabase
 */
Err AddrDBGetDatabase( DmOpenRef *dbPP, UInt16 mode )
{
	Err 					error = 0;
	DmOpenRef 				dbP;
	UInt16 					cardNo;
	LocalID 				dbID;
	DmSearchStateType 		stateInfo;

	*dbPP = NULL;
	
	error = DmGetNextDatabaseByTypeCreator( true, &stateInfo, addrDBType, addrDBCreatorID, true, &cardNo, &dbID );
	
	if ( error ) return ( dmErrCantFind );
	
	dbP = DmOpenDatabase ( cardNo, dbID, mode );
	
	if ( !dbP ) return ( DmGetLastErr() );

	*dbPP = dbP;
	
	return ( errNone );
	
} // AddrDBGetDatabase()

/*
 * InitializeContactsList()
 */
Err	InitializeContactsList( void )
{
	Err 					error = errNone;
	DmOpenRef				dbP = NULL;
	UInt16					recordIdx = 0;
	UInt16					i = 0;
	MemHandle				memH = NULL;
	AddrDBRecordType 		record;
	AddrDBRecordFlags 		flags;
	Char					firstChar = chrNull;
	FormType*				pProgressForm = FrmInitForm( PROGRESS_FORM );
	UInt16					progressUpdateIdx = 0;
	RectangleType			progressTxtRect = { { 55, 1 }, { 23, 11 } };
	RectangleType			progressRect = { { PROGRESS_BAR_X, PROGRESS_BAR_Y }, { PROGRESS_BAR_LENGTH, 2 } };
	Char					progressStr[maxStrIToALen] = "\0";
	UInt16					strHeight = 0;
	UInt16					strWidth = 0;
	RGBColorType			rgbRed = { 0, 204, 0, 51 };
	RGBColorType			rgbBlue = { 0, 0, 0, 204 };
	IndexedColorType 		colorIdxRed;
	IndexedColorType 		colorIdxBlue;
	
	if ( pProgressForm )
	{
		FrmSetActiveForm ( pProgressForm );
		FrmDrawForm ( pProgressForm );
		
		WinPushDrawState();
		
		colorIdxBlue = WinRGBToIndex( &rgbBlue );
		colorIdxRed = WinRGBToIndex( &rgbRed );
	
		WinSetForeColor( colorIdxBlue );
		
		WinSetTextColor( colorIdxRed );
		FntSetFont( stdFont );
			
		WinSetDrawMode( winOverlay ); 
	}
					
	if ( AddrDBGetDatabase( &dbP, dmModeReadOnly | dmModeShowSecret ) )
	{
		return ( DmGetLastErr() );
	}
	else if ( dbP )
	{
		gNumContactsRecords = DmNumRecordsInCategory( dbP, gCategoryIdx );
		
		gNumContactsRecords = ( gNumContactsRecords > MAX_NUM_CONTACTS ) ? MAX_NUM_CONTACTS : gNumContactsRecords;
		
		progressUpdateIdx = (UInt16) gNumContactsRecords / 10;
		
		recordIdx = 0;
		
		for ( i = 0 ; i < gNumContactsRecords ; i++ )
		{
			if ( !AddrDBGetRecord( dbP, &recordIdx, &flags, &record, &memH ) )
			{
				if ( memH )
				{
					contacts[i] = MemPtrNew( sizeof( Contacts_t ) );
					if ( contacts[i] )
					{
						MemSet( contacts[i], sizeof( Contacts_t ), 0 );
						
						contacts[i]->recordIdx = recordIdx;
						
						CopyDetailsToArrayName( &record, &flags, false, &( contacts[i]->name ) );
						
						// character location array...
						firstChar = contacts[i]->name[0];
						if ( TxtCharIsAlpha ( firstChar ) )
						{
							if ( TxtCharIsUpper( firstChar ) ) firstChar += 0x0020; // lower-case
							firstChar -= 0x0061; // offset to zero
							
							if ( gCharLocation[firstChar] == 0xFFFF )
							{
								gCharLocation[firstChar] = i;
							}
						}
					}
					
					MemHandleUnlock ( memH );
				}
			}
			else
			{
				error = DmGetLastErr();	
			}
			
			if ( ( pProgressForm ) && ( progressUpdateIdx ) && ( !( i % progressUpdateIdx ) ) )
			{
				WinEraseRectangle( &progressTxtRect, 2 );
				// WinDrawRectangle( &progressRect, 2 );
				
				
				StrIToA( progressStr, ( Int32 ) ( 100 * i ) / gNumContactsRecords );
				strHeight = FntLineHeight();
				strWidth = FntLineWidth( progressStr, StrLen( progressStr ) );
						
				WinPaintChars( progressStr, StrLen( progressStr ),
								progressTxtRect.topLeft.x + ( ( progressTxtRect.extent.x - strWidth ) ),
								progressTxtRect.topLeft.y + ( ( progressTxtRect.extent.y - strHeight ) / 2 ));
				WinPaintChars( PERCENTAGE_STR, StrLen( PERCENTAGE_STR ),
								progressTxtRect.topLeft.x + ( ( progressTxtRect.extent.x ) ),
								progressTxtRect.topLeft.y + ( ( progressTxtRect.extent.y - strHeight ) / 2 ));
								
				progressRect.extent.x = ( ( PROGRESS_BAR_LENGTH * i ) / gNumContactsRecords );
				WinDrawRectangle( &progressRect, 0 );		
			}
				
			++recordIdx; 
		}
		
		DmCloseDatabase( dbP );	
	}

	if ( pProgressForm )
	{
		WinPopDrawState();
		
		FrmEraseForm ( pProgressForm );
		FrmDeleteForm ( pProgressForm );
		pProgressForm = NULL;	
	}

	return ( error );
		
} // InitializeContactsList()

/*
 * DestroyContactsList()
 */
void DestroyContactsList( void )
{
	UInt16			i = 0;
	
	while ( contacts[i] )
	{
		if ( contacts[i]->name )
		{
			MemPtrFree( contacts[i]->name );
			contacts[i]->name = NULL;
		}
		
		MemPtrFree( contacts[i] );	
		contacts[i] = NULL;
		
		i++;
	}
		
	return;
	
} // DestroyContactsList()

/*
 * CopyDetailsToArrayName()
 */
static Boolean CopyDetailsToArrayName( AddrDBRecordType* recordP, AddrDBRecordFlags* flagsP, Boolean bFirstNameFirst, Char** nameStrP )
{
	Boolean			handled = false;
	Boolean			n1Bits = false;
	Boolean			n2Bits = false;
	Char*			n1Str = NULL; // holds either First Name or Last Name
	Char*			n2Str = NULL; // holds either First Name or Last Name
	Char*			tmpNameStr = NULL;
	Char*			tmpStr = NULL;
	UInt16			tmpStrLen = 0;			

	if ( ( ( flagsP->bits.name ) && ( StrLen ( recordP->fields[name] ) ) )
			|| ( ( flagsP->bits.firstName ) && ( StrLen ( recordP->fields[firstName] ) ) ) )
	{
		if ( bFirstNameFirst )
		{
			n1Bits = ( flagsP->bits.firstName );
			n2Bits = ( flagsP->bits.name );
			n1Str = recordP->fields[firstName];
			n2Str = recordP->fields[name];
		}
		else
		{
			n1Bits = ( flagsP->bits.name );
			n2Bits = ( flagsP->bits.firstName );
			n1Str = recordP->fields[name];
			n2Str = recordP->fields[firstName];
		}

		tmpStrLen = ( n1Bits ) ? StrLen ( n1Str ) : 0;
		tmpStrLen += ( n2Bits ) ? StrLen ( n2Str ) : 0;
		if ( ( n1Bits ) && ( n2Bits ) ) tmpStrLen += 4;
		
		if ( tmpStrLen )
		{
			tmpNameStr = MemPtrNew( tmpStrLen + 1 );
			tmpNameStr[0] = chrNull;
			if ( n1Bits )
			{
				StrCopy( tmpNameStr,  n1Str );
			}
			if ( ( n1Bits ) && ( n2Bits ) )
			{
				StrCat( tmpNameStr, ", ");	
			}
			if ( n2Bits )
			{
				
				StrCat( tmpNameStr, n2Str );	
			}
			
			tmpStr = tmpNameStr;
		}
	}
	else if ( ( flagsP->bits.company ) && ( StrLen( recordP->fields[company] ) ) )
	{
		tmpStr = recordP->fields[company];
	}
	else if ( ( flagsP->bits.phone1 ) && ( StrLen( recordP->fields[phone1] ) ) )
	{
		tmpStr = recordP->fields[phone1];
	}
	else if ( ( flagsP->bits.phone2 ) && ( StrLen( recordP->fields[phone2] ) ) )
	{
		tmpStr = recordP->fields[phone2];
	}
	else if ( ( flagsP->bits.phone3 ) && ( StrLen( recordP->fields[phone3] ) ) )
	{
		tmpStr = recordP->fields[phone3];
	}
	else if ( ( flagsP->bits.phone4 ) && ( StrLen( recordP->fields[phone4] ) ) )
	{
		tmpStr = recordP->fields[phone4];
	}
	else if ( ( flagsP->bits.phone5 ) && ( StrLen( recordP->fields[phone5] ) ) )
	{
		tmpStr = recordP->fields[phone5];
	}
	else
	{
		tmpStr = NO_NAME_STR;	
	}

	*nameStrP = MemPtrNew( StrLen( tmpStr ) + 1 );
	*nameStrP[0] = chrNull;
	StrCopy( *nameStrP, tmpStr );
	
	handled = ( StrLen( *nameStrP ) );
	
	if ( tmpNameStr ) MemPtrFree ( tmpNameStr );
		
	return ( handled );
	
} // CopyDetailsToArrayName()

/*
 * AddrDB.c
 */
