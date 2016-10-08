/*
 * Scroll.c
 */
 
#include "Scroll.h"

// Prototypes
static Boolean 			DoSearch( EventType* pEvent );

/*
 * ProcessScroll()
 */
Boolean ProcessScrollEvent( FormType* pForm, EventType* pEvent )
{
	Boolean 				handled = false;
	UInt32					timeTicks = TimGetTicks();
	static Int16			oldTopRow = 0;
	static UInt16			oldSelectedContact = 0;
	static UInt16			oldSearchStrLen = 0;
	
	switch ( pEvent->eType )
	{
		case keyDownEvent:
			
			if ( DoSearch( pEvent ) ) break;
			
			if ( ( pEvent->data.keyDown.keyCode == chrVerticalTabulation /* vchrRockerUp */ ) 
						|| ( pEvent->data.keyDown.keyCode == vchrRockerUp ) 
						/* || ( pEvent->data.keyDown.chr == hsChrVolumeDown ) */ ) // UP
			{
				--gTopRow;
			}
			else if ( ( pEvent->data.keyDown.keyCode == chrFormFeed ) 
						|| ( pEvent->data.keyDown.keyCode == vchrRockerDown )
						/* || ( pEvent->data.keyDown.chr == hsChrVolumeUp ) */ ) // DOWN
			{
				++gTopRow;
			}
			else if ( pEvent->data.keyDown.keyCode == chrBackspace )
			{
				if ( pEvent->data.keyDown.modifiers & shiftKeyMask )
				{
					gTopRow = gNumContactsRecords - gNumTableRows; // handled below...
				}
				else
				{
					gTopRow = 0;
				}
			}
			else if ( pEvent->data.keyDown.keyCode == chrSpace )
			{
				++gTopRow;
			}
			else if ( ( pEvent->data.keyDown.keyCode == chrLineFeed )
						|| ( pEvent->data.keyDown.chr == vchrRockerCenter )
						|| ( pEvent->data.keyDown.chr == hsChrSide ) )
			{
				ShowDialingForm( contacts[gTopRow + gSelectedContact] );
			}
			
			
			
			if ( ( pEvent->data.keyDown.chr == chrDigitZero ) 
						|| ( pEvent->data.keyDown.chr == chrFullStop ) )
			{
					gSelectedContact = 0;
			}
			else if ( pEvent->data.keyDown.chr == vchrRockerLeft )
			{
				if ( gSelectedContact > 0 )
				{
					--gSelectedContact;
				}
				else
				{
					--gTopRow;
				}
			}
			else if ( ( pEvent->data.keyDown.chr == vchrRockerRight )
						&& ( contacts[gTopRow + gSelectedContact + 1] ) )
			{
				if ( gSelectedContact < 6 )
				{
					++gSelectedContact;
				}
				else
				{
					++gTopRow;
				}
			}
			
			// validation...
			if ( ( gTopRow < 0 ) || ( gNumTableRows >= gNumContactsRecords ) )
			{
				gTopRow = 0;
			}
			else if ( ( gTopRow + gNumTableRows ) > gNumContactsRecords )
			{
				gTopRow = ( gNumContactsRecords - gNumTableRows );
			}
			
			// gadget updates...
			if ( oldTopRow != gTopRow )
			{
				oldTopRow = gTopRow;
				EvtAddEventToQueue( &( gEventDataP->tblUpdateEvt ) );
				
				gEventDataP->gadgetUpdateEvt.data.gadgetMisc.gadgetID = SCROLL_POSITION_GADGET;
				EvtAddEventToQueue( &( gEventDataP->gadgetUpdateEvt ) );
			}
			
			if ( oldSelectedContact != gSelectedContact )
			{
				gSelectedRow = gSelectedContact;
				oldSelectedContact = gSelectedContact;
				gEventDataP->gadgetUpdateEvt.data.gadgetMisc.gadgetID = SELECTED_CONTACT_INDICATOR;
				EvtAddEventToQueue( &( gEventDataP->gadgetUpdateEvt ) );
			}
			
			if ( pEvent->data.keyDown.chr == hsChrSide )
			{
				gEventDataP->gadgetUpdateEvt.data.gadgetMisc.gadgetID = VOLUME_GADGET;
				EvtAddEventToQueue( &( gEventDataP->gadgetUpdateEvt ) );
			}
			
			if ( oldSearchStrLen != StrLen( gSearchStr ) )
			{
				oldSearchStrLen = StrLen( gSearchStr ); 
				gEventDataP->gadgetUpdateEvt.data.gadgetMisc.gadgetID = SEARCH_STRING_GADGET;
				EvtAddEventToQueue( &( gEventDataP->gadgetUpdateEvt ) );
			}

			break;
			
		case penDownEvent:
			
			gEventDataP->ptPD.x = pEvent->screenX;
			gEventDataP->ptPD.y = pEvent->screenY;
			gEventDataP->ticksTimePD = timeTicks;
			gEventDataP->direction = halt;
			gEventDataP->speed = stop;
			gEventDataP->stopDelay = 0;
			gEventDataP->ticksTimePU = 0;
			
			MemSet( gSearchStr, SEARCH_STR_MAX_LEN, 0 );
			
			break;
		
		case penUpEvent:
		
			if ( gRowInverted )				
			{
				WinInvertRectangle ( &gRowRect, 0 );
				gRowInverted = false;
				
				if ( ( TimGetTicks() - gEventDataP->ticksTimePD )
						> ( gEventDataP->Settings.popupDelay ) )
				{
					gSelectedContact = gSelectedRow; // <--
					
					if ( gEventDataP->bSoundClicks )
					{
						SndPlaySystemSound( sndClick );
					}
					
					ShowDialingForm( contacts[gTopRow + gSelectedRow] );
					
					if ( gEventDataP->bSoundClicks )
					{
						SndPlaySystemSound( sndClick );
					}
				}
			}
			
			gEventDataP->stopDelay = 0; // preset...
			
			if ( gEventDataP->bSlowToAStop )
			{
				if ( gEventDataP->speed == high )
				{
					gEventDataP->stopDelay = ( ( SysTicksPerSecond() * 3 ) / 4 );	
				}
				else if ( gEventDataP->speed == medium )
				{
					gEventDataP->stopDelay = ( ( SysTicksPerSecond() * 7 ) / 12 );
				}
				else if ( gEventDataP->speed == low )
				{
					gEventDataP->stopDelay = ( SysTicksPerSecond() / 3 );
				}
				
				// InitEventData( );
				gEventDataP->ticksTimePU = ( gEventDataP->stopDelay ) ? timeTicks : 0;
			}
			else
			{
				gEventDataP->speed = stop;
				gEventDataP->direction = halt;
			}
			
			break;
				
		case penMoveEvent:
			
			if ( gNumTableRows >= gNumContactsRecords ) break;
			
			/* if ( ( timeTicks - gEventDataP->ticksLastNilEvt )
						< ( gEventDataP->Settings.initialSpeed ) ) break;
			*/
			gEventDataP->ptPM.x = pEvent->screenX;
			gEventDataP->ptPM.y = pEvent->screenY;
				
			GetSpeed();
			
			if ( ( gRowInverted ) && ( gEventDataP->speed != stop ) )
			{
				WinInvertRectangle ( &gRowRect, 0 );
				gRowInverted = false;
			}
			
			// FALL-THROUGH, NO "BREAK" HERE!!!
		case nilEvent:
			
			if ( ( timeTicks - gEventDataP->ticksLastNilEvt ) 
						< ( /* 5 8 10 */ 10 - ( ( gEventDataP->speed ) ? gEventDataP->speed : 1 ) ) )
			{	
				gEventDataP->gadgetUpdateEvt.data.gadgetMisc.gadgetID = SCROLL_POSITION_GADGET;
				EvtAddEventToQueue( &( gEventDataP->gadgetUpdateEvt ) );
			
				break;	
			}
			
			if ( ( gEventDataP->bSlowToAStop ) && ( gEventDataP->ticksTimePU ) )
			{
				gEventDataP->speed = medium;
				
				if ( ( ( gEventDataP->ticksTimePU + gEventDataP->stopDelay ) - timeTicks ) 
							< ( SysTicksPerSecond() / 3 ) )
				{
					gEventDataP->speed = low;
				}
				else // if ( ( ( gEventDataP->ticksTimePU + gEventDataP->stopDelay ) - timeTicks ) < ( SysTicksPerSecond() * 3 ) / 4 )
				{
					gEventDataP->speed = medium;
				}

				if ( ( timeTicks - gEventDataP->ticksTimePU ) > gEventDataP->stopDelay )
				{
					InitEventData();
					gEventDataP->speed = stop;
					gEventDataP->direction = halt;
					gEventDataP->ticksTimePU = 0;
					gEventDataP->stopDelay = 0;
					
					if ( gEventDataP->bSoundClicks )
					{
						SndPlaySystemSound( sndClick );
					}
				}
			}

			gEventDataP->ticksLastNilEvt = timeTicks;
			
			if ( gEventDataP->direction == up ) 
			{
				gTopRow -= ( ( gEventDataP->Settings.reverseScroll ) ? -1 : 1 );
			}
			else if ( gEventDataP->direction == down ) 
			{
				gTopRow += ( ( gEventDataP->Settings.reverseScroll ) ? -1 : 1 );
			}
			
			// validation...
			if ( ( gTopRow < 0 ) || ( gNumTableRows >= gNumContactsRecords ) )
			{
				gTopRow = 0;
			}
			else if ( ( gTopRow + gNumTableRows ) > gNumContactsRecords )
			{
				gTopRow = ( gNumContactsRecords - gNumTableRows );
			}
			
			// gadget updates...
			if ( oldTopRow != gTopRow )
			{
				oldTopRow = gTopRow;
				EvtAddEventToQueue( &( gEventDataP->tblUpdateEvt ) );
				
				gEventDataP->gadgetUpdateEvt.data.gadgetMisc.gadgetID = SCROLL_POSITION_GADGET;
				EvtAddEventToQueue( &( gEventDataP->gadgetUpdateEvt ) );
				
				EvtResetAutoOffTimer();
			}
			
			if ( oldSelectedContact != gSelectedContact )
			{
				oldSelectedContact = gSelectedContact;
				gEventDataP->gadgetUpdateEvt.data.gadgetMisc.gadgetID = SELECTED_CONTACT_INDICATOR;
				EvtAddEventToQueue( &( gEventDataP->gadgetUpdateEvt ) );
			}
			
			gEventDataP->gadgetUpdateEvt.data.gadgetMisc.gadgetID = VOLUME_GADGET;
			EvtAddEventToQueue( &( gEventDataP->gadgetUpdateEvt ) );
			
			if ( oldSearchStrLen != StrLen( gSearchStr ) )
			{
				oldSearchStrLen = StrLen( gSearchStr ); 
				gEventDataP->gadgetUpdateEvt.data.gadgetMisc.gadgetID = SEARCH_STRING_GADGET;
				EvtAddEventToQueue( &( gEventDataP->gadgetUpdateEvt ) );
			}
					
			handled = false;
						
		default:
			
			break;	
	}
	
	return ( handled );
	
} // ProcessScrollEvent()

/* 
 * InitEventData()
 */
void InitEventData( void )
{
	AppPreferences_t	prefs;
	
	if ( !gEventDataP ) return;
	
	// MemSet( gEventDataP, sizeof( EventData_t ), 0 );
	
	gEventDataP->ticksTimePD = -1; // invalid
//	gEventDataP->ticksLastNilEvt = 0;
	
	gEventDataP->tblUpdateEvt.eType = tblUpdate; // custom event
	
	gEventDataP->gadgetUpdateEvt.eType = frmGadgetMiscEvent;
	gEventDataP->gadgetUpdateEvt.data.gadgetMisc.gadgetID = SCROLL_POSITION_GADGET;
	// gEventDataP->gadgetUpdateEvt.data.gadgetMisc.dataP = &gTopRow;
	
	ReadAppPrefs( &prefs );
	gEventDataP->Settings = prefs.Settings;
	gEventDataP->bSoundClicks = prefs.Colors.bSoundClicks;
	gEventDataP->bSlowToAStop = prefs.Colors.bSlowToAStop;
		
	return;
	
} // InitEventData

/*
 * GetSpeed()
 * returns direction!
 */
Direction_e GetSpeed( void )
{
	UInt16			vector = ( gEventDataP->ptPM.y > gEventDataP->ptPD.y ) 
									? ( gEventDataP->ptPM.y - gEventDataP->ptPD.y ) 
									: ( gEventDataP->ptPD.y - gEventDataP->ptPM.y );
	
	gEventDataP->direction = halt;
	gEventDataP->speed = stop;
	
	if ( gEventDataP->ptPM.y > gEventDataP->ptPD.y )
	{
		gEventDataP->direction = down;
	}
	else if ( gEventDataP->ptPM.y < gEventDataP->ptPD.y )
	{
		gEventDataP->direction = up;
	}
	
	if ( vector < gEventDataP->Settings.thresholdStop )
	{
		gEventDataP->direction = gEventDataP->speed = stop;
	}
	else if ( vector < gEventDataP->Settings.thresholdSlow )
	{
		gEventDataP->speed = low;
	}	
	else if ( vector < gEventDataP->Settings.thresholdMedium )
	{
		gEventDataP->speed = medium;
	}
	else // if ( vector < gEventDataP->Settings.thresholdFast )
	{
		gEventDataP->speed = high;
	}

	return ( gEventDataP->direction );
	
} // GetSpeed()

/*
 * DoSearch()
 * This uses globals all over
 */
static Boolean DoSearch( EventType* pEvent )
{
	Boolean				retVal = false;
	Boolean				found = false;
	
	if ( ( pEvent->data.keyDown.keyCode == chrBackspace )
					&& ( StrLen( gSearchStr ) ) )
	{
		if ( pEvent->data.keyDown.modifiers & shiftKeyMask ) 
		{
			MemSet( gSearchStr, SEARCH_STR_MAX_LEN, 0 );			
		}
		else
		{
			gSearchStr[StrLen( gSearchStr ) - 1] = chrNull;			
		}
		retVal = true;
	}
	else if ( StrLen( gSearchStr ) > SEARCH_STR_MAX_LEN - 1 )
	{
		if ( gEventDataP->bSoundClicks )
		{				
			SndPlaySystemSound( sndError );
		}
		retVal = true;
	}		
	else if ( ( pEvent->data.keyDown.keyCode > chrSmall_A - 1 /* 0x0060 */ ) 
					&& ( pEvent->data.keyDown.keyCode <  chrSmall_Z + 1 /* 0x007B */ ) )
	{
		gSearchStr[StrLen( gSearchStr )] = pEvent->data.keyDown.keyCode; // This is LOWERCASE, not "chr"
		gSearchStr[StrLen( gSearchStr ) + 1] = chrNull;
		retVal = true;
	}
	else if ( ( pEvent->data.keyDown.keyCode < chrSmall_A )
					|| ( pEvent->data.keyDown.keyCode > chrSmall_Z ) )
	{
		if ( pEvent->data.keyDown.keyCode == chrSpace )
		{
			retVal = ( StrLen( gSearchStr ) );
		}

		MemSet( gSearchStr, SEARCH_STR_MAX_LEN, 0 );		
	}
	
	if ( ( retVal ) && ( StrLen( gSearchStr ) ) )
	{
		Boolean			found = false;
		UInt16			idx = 0;
			
		if ( StrLen( gSearchStr ) > 1 )
		{
			found = SearchStringInArray( gSearchStr, &idx );
			
			if ( found )
			{
				gTopRow = idx;		
			}
			else
			{
				gSearchStr[StrLen( gSearchStr ) - 1] = chrNull;
			}
		}
		else
		{
			if ( gCharLocation[gSearchStr[0] - 0x0061] != 0xFFFF )
			{
				found = true;
				gTopRow = gCharLocation[gSearchStr[0] - 0x0061];
			}
		}
	
		if ( found )
		{
			gSelectedContact = 0;
			
			if ( gNumContactsRecords <= gNumTableRows )
			{
				gSelectedContact = idx;	
			}
			else if ( ( gTopRow + gNumTableRows ) > gNumContactsRecords )
			{
				gSelectedContact = ( gTopRow + gNumTableRows ) - gNumContactsRecords;
				
				gSelectedContact = ( gSelectedContact < gNumTableRows ) ? gSelectedContact : gNumTableRows - 1;
			}
			if ( gEventDataP->bSoundClicks )
			{		
				SndPlaySystemSound( sndClick );
			}
		}
		else
		{	
			if ( gEventDataP->bSoundClicks )
			{
				SndPlaySystemSound( sndError );
			}
		}
	}
	
 	return ( retVal );
 	
} // DoSearch()

/*
 * Scroll.c
 */