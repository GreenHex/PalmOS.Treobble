 /*
 * AppMain.c
 */
   
// #define ERROR_CHECK_FULL
#include <Hs.h>
#include <palmOneCreators.h>
#include <Category.h>

#include "Sections.h"
#include "Global.h"
#include "Utils.h"
#include "Prefs.h"
#include "Settings.h"
#include "Tables.h"
#include "Scroll.h"
#include "Colours.h"
#include "DrawPE.h"
#include "CountGadget.h"
#include "AppResources.h"

// Ooops... Globals!
UInt16						gNumContactsRecords;
Int16						gTopRow;
UInt16						gSelectedRow;
Boolean						gRowInverted;
RectangleType				gRowRect;
UInt16						gCharLocation[26];
UInt16						gSelectedContact;
RGBColorType				gBackColorRGB;
IndexedColorType			gBackColorIdx;
IndexedColorType			gTextColorIdx;
IndexedColorType			gSep1ColorIdx;
// IndexedColorType			gSep2ColorIdx;
UInt16						gCategoryIdx;
Char						gCategoryName[dmCategoryLength];
Char						gSearchStr[SEARCH_STR_MAX_LEN];
IndexedColorType			gOldFieldColorIdx;
RGBColorType				gOldFieldColorRGB;

extern EventData_t*			gEventDataP;

// Prototypes
// code 2 // EXTRA_SECTION_ONE
static Err 					AppStart(void) 													EXTRA_SECTION_ONE;
static void 				AppStop(void) 													EXTRA_SECTION_ONE;
static void 				InitializeMainForm(FormType* pForm, AppPreferences_t* pPrefs) 	EXTRA_SECTION_ONE;
static Boolean 				MainFormHandleEvent(EventType* pEvent) 							EXTRA_SECTION_ONE;
// code 1
static Boolean 				AppHandleEvent(EventType* pEvent);
static void 				AppEventLoop(void);
UInt32 						PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags);

/*
 * AppStart
 */
static Err AppStart( void )
{
	AppPreferences_t			prefs;
	UnsavedPreferences_t 		usPrefs;
	
	ReadAppPrefs( &prefs );
	ReadUsPrefs( &usPrefs );
	
	gBackColorRGB = prefs.Colors.background;
	gBackColorIdx = WinRGBToIndex( &( prefs.Colors.background ) );
	gTextColorIdx = WinRGBToIndex( &( prefs.Colors.foreground ) );
	gSep1ColorIdx = WinRGBToIndex( &( prefs.Colors.color1 ) );
	// gSep2ColorIdx = WinRGBToIndex( &( prefs.Colors.color2 ) );
	
	gTopRow = usPrefs.topRow;
	gSelectedRow = usPrefs.selectedRow;
	gRowInverted = false;
	gSelectedContact = gSelectedRow;
	gCategoryIdx = usPrefs.categoryIdx;
	gCategoryName[0] = chrNull;
	gEventDataP = NULL;
	gOldFieldColorIdx = UIColorGetTableEntryIndex( UIFieldBackground );
	WinIndexToRGB( gOldFieldColorIdx, &gOldFieldColorRGB );
	
	MemSet( gCharLocation, sizeof( UInt16 ) * 26, 0xFF );
	MemSet( gSearchStr, SEARCH_STR_MAX_LEN, 0 );
	
	if ( InitializeContactsList( ) )
	{
		FrmCustomAlert( ErrOKAlert, ADDR_DB_NOT_FOUND_ERROR, "", "" );
	}
	
	FrmGotoForm( MAIN_FORM );

	return ( errNone );
	
} // AppStart

/*
 * AppStop
 */
static void AppStop( void )
{
	UnsavedPreferences_t 		usPrefs;
	
	usPrefs.topRow = gTopRow;
	usPrefs.categoryIdx = gCategoryIdx;
	usPrefs.selectedRow = gSelectedRow;
	WriteUsPrefs( &usPrefs );
	
	FrmCloseAllForms();	
	
	DestroyContactsList();
	
} // AppStop

/*
 * InitializeMainForm
 */
static void InitializeMainForm( FormType* pForm, AppPreferences_t* pPrefs )
{
	TableType*				pTbl = FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_TABLE ) );
	DmOpenRef				dbP = NULL;
	UInt16					lastCategory = dmAllCategories;
	
	FrmSetActiveForm( pForm );
	
	TableInit( pTbl );	
	
	InitializeCountGadget( pForm, &gNumContactsRecords, &gTopRow );
	CommonInitGauge( pForm );
	
	AddrDBGetDatabase( &dbP, dmModeReadOnly | dmModeShowSecret );

	lastCategory = CategoryFind ( dbP, "" );
	
	if ( gCategoryIdx >= dmRecNumCategories )
	{
		gCategoryIdx = dmAllCategories;
	}
	
	CategoryGetName( dbP, gCategoryIdx, gCategoryName );
	
	DmCloseDatabase( dbP );

	CategorySetTriggerLabel( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_CATEGORY_POPUP ) ), gCategoryName );
				
	FrmDrawForm( pForm );

	FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_CATEGORY_POPUP ) );

	return;
	
} // InitializeMainForm

/*
 * MainFormHandleEvent
 */
static Boolean MainFormHandleEvent( EventType* pEvent )
{
	Boolean 				handled = false;
	FormType* 				pForm = FrmGetActiveForm();
	AppPreferences_t		prefs;		
	EventType 				newEvent;
	Err						error = errNone;
	DmSearchStateType 		stateInfo;
   	UInt16 					cardNo = 0;
   	LocalID					dbID = 0;
	UInt32					result = 0;	
	static UInt16			oldCategoryIdx = dmAllCategories;
	EventType				tableUpdateEvt;
	EventType				gadgetUpdateEvt;
	static Boolean			bMenuDisplayed = false; // UNUSED	
	
	FrmSetActiveForm( pForm );
	
	
	switch ( pEvent->eType )
	{				
		case tblUpdate: // custom event
		
			TblDrawTable( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_TABLE ) ) );
			
			WinPushDrawState();
				
			if ( contacts[gTopRow] )
			{
				WinSetForeColor( gSep1ColorIdx );
				
				WinDrawLine( 4, 4, 155, 4 );
			}
			else
			{
				WinEraseLine( 4, 4, 155, 4 );
			}
			
			WinPopDrawState();
			
			handled = true;
			break;
							
		case tblSelectEvent:
		
			handled = false;
			break;
			
		case tblEnterEvent:
		
			gSelectedRow = pEvent->data.tblEnter.row;
			
			if ( gRowInverted )
			{
				gRowInverted = false;					  
				WinInvertRectangle ( &gRowRect, 0 );
			}
			
			if ( contacts[gTopRow + gSelectedRow] )
			{
				TblGetItemBounds( pEvent->data.tblEnter.pTable, 
									 pEvent->data.tblEnter.row,
									  pEvent->data.tblEnter.column,
									  &gRowRect );
				
				if ( !gRowInverted )
				{
					gRowInverted = true;					  
					WinInvertRectangle ( &gRowRect, 0 );
				}
			}
			handled = true; // no longer receive the pen-up event!
			break;
			
		case tblExitEvent:

			handled = false;
			break;
			
		case frmOpenEvent:
			
			ReadAppPrefs( &prefs ); // what a waste

			oldCategoryIdx = gCategoryIdx;
	
			InitializeMainForm( pForm, &prefs );
			
			tableUpdateEvt.eType = tblUpdate;
			EvtAddEventToQueue( &tableUpdateEvt );
			
			handled = true;
			break;
			
		case ctlSelectEvent:
		
			if ( pEvent->data.ctlSelect.controlID == MAIN_CATEGORY_POPUP )
			{
				DmOpenRef				dbP = NULL;
				
				AddrDBGetDatabase( &dbP, dmModeReadOnly | dmModeShowSecret );

				CategorySelect ( dbP, pForm, MAIN_CATEGORY_POPUP, MAIN_CATEGORY_LIST, true, &gCategoryIdx,
											gCategoryName, 0, categoryHideEditCategory );
				
				DmCloseDatabase( dbP );

				if ( oldCategoryIdx != gCategoryIdx )
				{
					oldCategoryIdx = gCategoryIdx;
					
					DestroyContactsList();
					MemSet( gCharLocation, sizeof( UInt16 ) * 26, 0xFF );
					
					InitializeContactsList( );
										
					FrmSetActiveForm( pForm );
					
					gTopRow = 0;
					gSelectedContact = 0;
					gSelectedRow = 0;
					
					tableUpdateEvt.eType = tblUpdate;
					EvtAddEventToQueue( &tableUpdateEvt );

					gadgetUpdateEvt.eType = frmGadgetMiscEvent;
					
					gadgetUpdateEvt.data.gadgetMisc.gadgetID = CONTACTS_COUNT_GADGET;
					gadgetUpdateEvt.data.gadgetMisc.dataP = &gTopRow;
					EvtAddEventToQueue( &gadgetUpdateEvt );
					
					gadgetUpdateEvt.data.gadgetMisc.gadgetID = SELECTED_CONTACT_INDICATOR;
					gadgetUpdateEvt.data.gadgetMisc.dataP = NULL;
					EvtAddEventToQueue( &gadgetUpdateEvt );
					
					gadgetUpdateEvt.data.gadgetMisc.gadgetID = SCROLL_POSITION_GADGET;
					gadgetUpdateEvt.data.gadgetMisc.dataP = &gNumContactsRecords;
					EvtAddEventToQueue( &gadgetUpdateEvt );
				}
				
				MemSet( gSearchStr, SEARCH_STR_MAX_LEN, 0 );
					
				gadgetUpdateEvt.data.gadgetMisc.gadgetID = SEARCH_STRING_GADGET;
				EvtAddEventToQueue( &gadgetUpdateEvt );
			}
			
			handled = true;
			break;
			
		case menuEvent: 
								 
			switch ( pEvent->data.menu.itemID )
			{	
				case MAIN_MENU_CONTACTS:

					error = DmGetNextDatabaseByTypeCreator(true, &stateInfo, sysFileTApplication, 
																kPalmOneCreatorIDContacts, true, &cardNo, &dbID);	
					if ( !error )
					{
						SysUIAppSwitch ( cardNo, dbID, sysAppLaunchCmdNormalLaunch, NULL );															
					}
				
					handled = true;
					break;
				
				case MAIN_MENU_PHONE:
				
					error = DmGetNextDatabaseByTypeCreator(true, &stateInfo, sysFileTApplication, 
																hsFileCPhone, true, &cardNo, &dbID);	
					if ( !error )
					{
						SysUIAppSwitch ( cardNo, dbID, phoneAppLaunchCmdViewPhoneHome, NULL );															
					}
					
					handled = true;
					break;
					
				case MAIN_MENU_EXIT:
					
					newEvent.eType = appStopEvent;
									
					EvtAddEventToQueue( &newEvent );			
				
					handled = true;
					break;
						
				case MAIN_MENU_SETTINGS:
					
					ReadAppPrefs( &prefs );
					
					if ( ShowSettingsForm( &( prefs.Settings ) ) )
					{
						WriteAppPrefs( &prefs );
					}
					
					InitEventData();
					
					FrmSetActiveForm( pForm );
					
					handled = true;
					break;
					
				case MAIN_MENU_PREFERENCES:
					
					ReadAppPrefs( &prefs );
					
					if ( ShowColorSelForm( &( prefs.Colors ) ) )
					{
						WriteAppPrefs( &prefs );
						
						gBackColorRGB = prefs.Colors.background;
						gBackColorIdx = WinRGBToIndex( &( prefs.Colors.background ) );
						gTextColorIdx = WinRGBToIndex( &( prefs.Colors.foreground ) );
						gSep1ColorIdx = WinRGBToIndex( &( prefs.Colors.color1 ) );
						// gSep2ColorIdx = WinRGBToIndex( &( prefs.Colors.color2 ) );
					}
					
					InitEventData();
					
					FrmSetActiveForm( pForm );
					
					handled = true;
					break;
					
				case MAIN_MENU_ABOUT:
					
					ShowAboutForm( pForm );
					
					handled = true;
					break;
			
				default:
					break;
			}
			
			break;
		
		case menuOpenEvent:
			
			bMenuDisplayed = true;
			
			break;
	
		default:
		
			handled = ProcessScrollEvent ( pForm, pEvent );
			
			break;
	}
	
	return ( handled );

} // MainFormHandleEvent

/*
 * AppHandleEvent
 */
static Boolean AppHandleEvent( EventType* pEvent )
{
	UInt16 		formID;
	FormType* 	pForm;
	Boolean		handled = false;
 
	if ( pEvent->eType == frmLoadEvent )
	{
		formID = pEvent->data.frmLoad.formID;
		
		pForm = FrmInitForm( formID );
		FrmSetActiveForm( pForm );

		if ( formID == MAIN_FORM )
			FrmSetEventHandler( pForm, MainFormHandleEvent );
			
		handled = true;
	}
	
	return ( handled );

} // AppHandleEvent

/*
 * AppEventLoop
 */
static void AppEventLoop( void )
{
	Err					error = errNone;
	EventType			event;

	gEventDataP = MemPtrNew( sizeof( EventData_t ) );	
	
	MemSet( gEventDataP, sizeof( EventData_t ), 0 );
	
	InitEventData();

	do
	{	
		EvtGetEvent( &event, SysTicksPerSecond()
								/ ( gEventDataP->Settings.initialSpeed 
										* ( ( gEventDataP->speed ) ? gEventDataP->speed : 1 ) ) );
		
		if ( SysHandleEvent( &event ) )
			continue;
			
		if ( MenuHandleEvent( 0, &event, &error ) )
			continue;
		
		if ( AppHandleEvent( &event ) )
			continue;
/*						
		if ( ProcessScrollEvent( &event ) )
			continue;
*/			
		FrmDispatchEvent( &event );
	}
	while ( event.eType != appStopEvent );
	
	
	if ( gEventDataP )
		MemPtrFree ( gEventDataP );
	
	return;
	
} // AppEventLoop

/*
 * PilotMain()
 */
UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags )
{
	Err 							error = errNone;
			
	switch ( cmd )
	{
		case sysAppLaunchCmdNormalLaunch:
		
			if ( error = RomVersionCompatible( MIN_VERSION, launchFlags ) )
			{
				FrmCustomAlert( ErrOKAlert, ALERT_INCOMPATIBLE_ERROR, "", "" );

				break;
			}

			if ( GetNumContactsRecords( dmModeReadOnly | dmModeShowSecret ) > MAX_NUM_CONTACTS )
			{
				FrmCustomAlert( InfoOKAlert, ADDR_DB_TOO_MANY_RECORDS, "", "" );
				break;	
			}
			
			if ( ( error = AppStart() ) == 0 )
			{
				AppEventLoop();
				AppStop();
			}
			break;
			
		default:
			break;
	}

	return ( error );

} // PilotMain

/*
 * AppMain.c
 */

