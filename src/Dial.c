/*
 * Dial.c
 */

#include "Dial.h"

// Prototypes
void 						KillDialerAlarmCallBack( UInt16 almProcCmd, SysAlarmTriggeredParamType* almTriggerParam );
Boolean 					DialFormHandleEvent( EventType* pEvent );
static void 				InitializeDialingForm( FormType* pForm, Contacts_t* pContact, AddrDBRecordType* recordP, AddrDBRecordFlags* flagsP );
static void 				SetPhoneNumber( FormType* pForm, UInt16 ctlID, Boolean set, Char* numberStr, AddressPhoneLabels label, Coord labelLocY );
static Err 					DialOut( DialType_e dialType, Char* number, Char* name, Char* message, UInt16 recordIdx );
static Err 					DialPhoneLaunch( Char* number, Char* name );
static Err 					LaunchAddressbook( UInt16 recordIdx );

/* 
 * KillDialerAlarmCallBack()
 */
void KillDialerAlarmCallBack( UInt16 almProcCmd, SysAlarmTriggeredParamType* almTriggerParam )
{
	FormType* 		pForm = (FormType *) almTriggerParam->ref;
	EventType		newEvent;
	
	if ( !pForm )
		return;
	
	FrmSetActiveForm( pForm );
		
	newEvent.eType = frmCloseEvent;
	newEvent.data.frmClose.formID = FrmGetActiveFormID();
	
	EvtAddEventToQueue( &newEvent );
	
	SndPlaySystemSound( sndClick );
		
	EvtWakeup();
	
	return;
	
} // KillDialerAlarmCallBack()

/*
 * DialFormHandleEvent()
 */
Boolean DialFormHandleEvent( EventType* pEvent )
{
	Boolean				handled = false;
	FormType*			pForm = FrmGetActiveForm();
	
	FrmSetActiveForm( pForm );
	
	switch ( pEvent->eType )
	{
		case penDownEvent:
		
			AlmSetProcAlarm( KillDialerAlarmCallBack, (UInt32) pForm, TimGetSeconds() + DIALER_KILL_SECONDS );	
			
			break;
				
		case keyDownEvent:
		
			if ( ( pEvent->data.keyDown.keyCode == chrBackspace )
					|| ( pEvent->data.keyDown.keyCode == chrSpace ) )			
			{
				EventType			newEvent;
				
				newEvent.eType = frmCloseEvent;
				newEvent.data.frmClose.formID = FrmGetActiveFormID();
				
				EvtAddEventToQueue( &newEvent );	
				
				handled = true;
			}
			else if ( pEvent->data.keyDown.keyCode == chrLineFeed )
			{
				EvtEnqueueKey( vchrRockerCenter, vchrRockerCenter, commandKeyMask );
				handled = true;	
			}
			else if ( pEvent->data.keyDown.chr == hsChrSide )
			{
				EvtEnqueueKey( vchrRockerCenter, vchrRockerCenter, commandKeyMask );
				handled = true;	
			}
			else if ( pEvent->data.keyDown.chr == hsChrVolumeUp )
			{
				EvtEnqueueKey( chrVerticalTabulation, chrVerticalTabulation, commandKeyMask ); // 0x000B UP
				handled = true;;
			}
			else if ( pEvent->data.keyDown.chr == hsChrVolumeDown )
			{
				EvtEnqueueKey( chrFormFeed, chrFormFeed, commandKeyMask );  // 0x000C DOWN
				handled = true;	
			}
			else if ( ( pEvent->data.keyDown.keyCode == vchrRockerUp )
						|| ( pEvent->data.keyDown.keyCode == vchrRockerDown )
						|| ( pEvent->data.keyDown.keyCode == vchrRockerRight )
						|| ( pEvent->data.keyDown.keyCode == vchrRockerLeft ) 
						|| ( pEvent->data.keyDown.keyCode == chrVerticalTabulation )
						|| ( pEvent->data.keyDown.keyCode == chrFormFeed ) )
			{
				AlmSetProcAlarm( KillDialerAlarmCallBack, (UInt32) pForm, TimGetSeconds() + DIALER_KILL_SECONDS );	
			}
			else if ( ( pEvent->data.keyDown.chr == vchrHardPower )
						|| ( pEvent->data.keyDown.chr == vchrHard4 ) )
			{
				EventType 			newEvent;
				
				newEvent.eType = frmCloseEvent;
				newEvent.data.frmClose.formID = FrmGetActiveFormID();	
				
				EvtAddEventToQueue( &newEvent );
			}
			break;
				
		default:
			break;		
		
	}

	return ( handled );
	
} // DialFormHandleEvent()

/*
 * ShowDialingForm()
 */
Boolean ShowDialingForm( Contacts_t* pContact )
{
	Boolean 					retVal = false;
	Err							error = errNone;
	UInt16						numberBtnID = 0;
	FormType*					pOldForm = FrmGetActiveForm();
	FormType*					pForm = FrmInitForm( DIAL_FORM );
	DialType_e					dialType = phone;
	Char*						name = NULL;
	Char*						tmpPhoneNumber = NULL;
	Char*						phoneNumber = NULL;
	UInt16						recordIdx = 0;
	DmOpenRef					dbP = NULL;
	MemHandle					memH = NULL;
	AddrDBRecordFlags 			flags;
	AddrDBRecordType 			record;
	AddressPhoneLabels			labelType;
	
	if ( !pContact ) return ( retVal );
	
	if ( AddrDBGetDatabase( &dbP, dmModeReadOnly | dmModeShowSecret ) )
	{
		return ( DmGetLastErr() == errNone );
	}
	else if ( dbP )
	{
		if ( !AddrDBGetRecord( dbP, &(pContact->recordIdx), &flags, &record, &memH ) )
		{
			if ( memH )
			{					
				FrmSetActiveForm( pForm );
				
				FrmSetEventHandler(pForm, DialFormHandleEvent);
				
				InitializeDialingForm( pForm, pContact, &record, &flags );
				
				AlmSetProcAlarm( KillDialerAlarmCallBack, (UInt32) pForm, TimGetSeconds() + DIALER_KILL_SECONDS );
				
				numberBtnID = FrmDoDialog( pForm );
				
				AlmSetProcAlarm( KillDialerAlarmCallBack, 0, 0 );
				
				retVal = ( numberBtnID != DIAL_CANCEL_BUTTON );
				
				if ( retVal )
				{
					if ( numberBtnID == DIAL_CONTACTS_GOTO_BUTTON )
					{
						dialType = addressbook;
					}
					else if ( CtlGetValue( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, DIAL_SMS_PUSH_BTN ) ) ) == 1 )
					{
						dialType = messaging;	
					}
					else if ( CtlGetValue( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, DIAL_EMAIL_PUSH_BTN ) ) ) == 1 )
					{
						dialType = email;
					}
					// else dialType = phone
			
					switch ( numberBtnID )
					{
						case DIAL_PHONE1_BUTTON:
							tmpPhoneNumber = (Char *)( record.fields[phone1] );
							labelType =	(AddressPhoneLabels) record.options.phones.phone1;
							break;
						case DIAL_PHONE2_BUTTON:
							tmpPhoneNumber = (Char *)( record.fields[phone2] );
							labelType =	(AddressPhoneLabels) record.options.phones.phone2;	
							break;
						case DIAL_PHONE3_BUTTON:
							tmpPhoneNumber = (Char *)( record.fields[phone3] );
							labelType =	(AddressPhoneLabels) record.options.phones.phone3;		
							break;
						case DIAL_PHONE4_BUTTON:
							tmpPhoneNumber = (Char *)( record.fields[phone4] );
							labelType =	(AddressPhoneLabels) record.options.phones.phone4;
							break;
						case DIAL_PHONE5_BUTTON:
							tmpPhoneNumber = (Char *)( record.fields[phone5] );
							labelType =	(AddressPhoneLabels) record.options.phones.phone5;
							break;	
						default:
							break;	
					}
			
					if ( tmpPhoneNumber )
					{
						phoneNumber = MemPtrNew( StrLen( tmpPhoneNumber ) + 1 );
						phoneNumber[0] = chrNull;
						StrCopy( phoneNumber, tmpPhoneNumber );	
				
						name = MemPtrNew ( StrLen( pContact->name ) + 1 );
						name[0] = chrNull;
						StrCopy( name, pContact->name );
					}
					else // goto
					{		
						recordIdx = pContact->recordIdx;
					}
				}
				else
				{
					vibrate();	
				}
				
				FrmEraseForm(pForm);
				FrmDeleteForm(pForm);
				
				pForm = NULL;
			
				MemHandleUnlock ( memH );	
			}
			
		}
				
		DmCloseDatabase ( dbP );	
	}
	
	if ( pOldForm ) FrmSetActiveForm( pOldForm );

	if ( retVal )
	{
		if ( ( dialType != messaging ) 
				&& ( ( labelType == emailLabel ) || ( ( phoneNumber ) && ( StrChr( phoneNumber, chrCommercialAt ) ) ) ) )
		{
			dialType = email;
		}
		
		error = DialOut( dialType, phoneNumber, name, NULL, recordIdx );

		if ( phoneNumber ) MemPtrFree ( phoneNumber );
		if ( name ) MemPtrFree ( name );
		
		if ( error != errNone )
		{
			Alert( DIAL_OUT_ERROR, &error );	
		}
	}
	
	return ( retVal );
	
} // ShowDialingForm()

/*
 * InitializeDialingForm
 */
static void InitializeDialingForm( FormType* pForm, Contacts_t* pContact, 
										AddrDBRecordType* recordP, AddrDBRecordFlags* flagsP )
{
	ControlType*				pCtl = NULL;
	Char						str[DIAL_DISPLAY_NAME_STR_LEN] = "\0";
	
	
	if ( ( !pForm) || ( !pContact ) || ( !recordP ) || ( !flagsP ) ) return;
	
	// Phone
	SetPhoneNumber( pForm, DIAL_PHONE1_BUTTON, (Boolean)(flagsP->bits.phone1), (Char *)(recordP->fields[phone1]), (AddressPhoneLabels) recordP->options.phones.phone1, PHN1_LOC_Y );
	SetPhoneNumber( pForm, DIAL_PHONE2_BUTTON, (Boolean)(flagsP->bits.phone2), (Char *)(recordP->fields[phone2]), (AddressPhoneLabels) recordP->options.phones.phone2, PHN2_LOC_Y );
	SetPhoneNumber( pForm, DIAL_PHONE3_BUTTON, (Boolean)(flagsP->bits.phone3), (Char *)(recordP->fields[phone3]), (AddressPhoneLabels) recordP->options.phones.phone3, PHN3_LOC_Y );
	SetPhoneNumber( pForm, DIAL_PHONE4_BUTTON, (Boolean)(flagsP->bits.phone4), (Char *)(recordP->fields[phone4]), (AddressPhoneLabels) recordP->options.phones.phone4, PHN4_LOC_Y );
	SetPhoneNumber( pForm, DIAL_PHONE5_BUTTON, (Boolean)(flagsP->bits.phone5), (Char *)(recordP->fields[phone5]), (AddressPhoneLabels) recordP->options.phones.phone5, PHN5_LOC_Y );
	
	// Name
	StrNCopy( str, pContact->name, DIAL_DISPLAY_NAME_STR_LEN - 1 );
	str[DIAL_DISPLAY_NAME_STR_LEN - 1] = chrNull;
	
	if ( StrLen( pContact->name ) > DIAL_DISPLAY_NAME_STR_LEN - 1 )
	{
		str[DIAL_DISPLAY_NAME_STR_LEN - 2] = chrHorizontalEllipsis; // 0x0085; // ellipses
	}
	
	FrmCopyLabel( pForm, DIAL_NAME_LABEL, str );
	
	// Dial Selector
	CtlSetValue ( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, DIAL_PHONE_PUSH_BTN ) ), 1 );
	CtlSetValue ( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, DIAL_SMS_PUSH_BTN ) ), 0 );
	CtlSetValue ( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, DIAL_EMAIL_PUSH_BTN ) ), 0 );
	
	return;
	
} // InitializeDialingForm()

/*
 * SetPhoneNumber()
 */
static void SetPhoneNumber( FormType* pForm, UInt16 ctlID, Boolean set, Char* numberStr, AddressPhoneLabels label, Coord labelLocY )
{
	Char				str[DIAL_DISPLAY_NUMBER_STR_LEN] = "\0";
	Char*				labelStr = NULL;
	UInt16				BmpID = 0;
	
	if ( set )
	{
		MemMove( str, numberStr, DIAL_DISPLAY_NUMBER_STR_LEN - 1 );
		str[DIAL_DISPLAY_NUMBER_STR_LEN - 1] = chrNull;
		
		if ( StrLen( numberStr ) > DIAL_DISPLAY_NUMBER_STR_LEN - 1 )
		{
			str[DIAL_DISPLAY_NUMBER_STR_LEN - 2] = chrHorizontalEllipsis; // 0x0085; // ellipses
		}
		
		labelStr = (Char *) CtlGetLabel( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, ctlID ) ) );
		labelStr[0] = chrNull;
		StrCopy( labelStr, str ); // str is already proper length!
		
		CtlSetLabel( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, ctlID ) ), labelStr );
		
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, ctlID ) );
		CtlSetEnabled( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, ctlID ) ), true );

		switch ( (UInt16)label )
		{
			case workLabel:
				BmpID = PHN_BMP_WORK;
				break;
			case homeLabel:
				BmpID = PHN_BMP_HOME;
				break;
			case emailLabel:
				BmpID = PHN_BMP_EMAIL;
				break;
			case mobileLabel:
				BmpID = PHN_BMP_MOBILE;
				break;
			case faxLabel:
				BmpID = PHN_BMP_FAX;
				break;
			case otherLabel:				
				BmpID = PHN_BMP_OTHER;
				break;
			case mainLabel:
				BmpID = PHN_BMP_MAIN;
				break;
			case pagerLabel:
				BmpID = PHN_BMP_PAGER;
				break;
			default:
				BmpID = 0;
				break;	
		}
		
		if ( BmpID )
		{
			FrmNewBitmap( &pForm, ctlID + 10,  BmpID, PHN_LOC_X, labelLocY );
		}
	}
	else
	{
		FrmCopyLabel( pForm, ctlID, " " );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, ctlID ) );
		CtlSetEnabled( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, ctlID ) ), false ); 
	}
	
	return;

} // SetPhoneNumber()


/*
 * DialOut()
 */
static Err DialOut( DialType_e dialType, Char* number, Char* name, Char* message, UInt16 recordIdx )
{
	Err				error = errNone;
	
	switch ( dialType )
	{
		case messaging:
		
			error = HsCreateNewMessage( number, name, message );
			
			break;
			
		case email:
			
			error = HsCreateNewEmail( number, NULL, NULL, message );
			
			break;

		case addressbook:
			
			error = LaunchAddressbook( recordIdx );
			
			break;
			
		default:
			
			// HsOpenDialNumberDialog( number, name, true );
			error = DialPhoneLaunch( number, name );
			
			break;		
	}
	
	return ( error );
		
} // DialOut()

/*
 * DialPhoneLaunch()
 */
static Err DialPhoneLaunch( Char* number, Char* name )
{
	Err								error = errNone;
	PhoneAppLaunchCmdDialType* 		dialP;
	Char*							phNumber;
	Char*							phName = MemPtrNew( StrLen( name ) + 1 );
	DmSearchStateType 				stateInfo;
   	UInt16 							cardNo = 0;
   	LocalID							dbID = 0;
	
	
	dialP = MemPtrNew( sizeof( PhoneAppLaunchCmdDialType ) );
	
	if ( dialP )
	{
		MemSet( dialP, sizeof( PhoneAppLaunchCmdDialType ), 0 );
	
		dialP->version = 1;
	    dialP->confirm = false;
	    dialP->extraDigits = NULL;
	    dialP->failLaunchCreator = appFileCreator;
	    dialP->failLaunchCode = sysAppLaunchCmdNormalLaunch;
	    dialP->failLaunchParams = 0;
	    dialP->dialMethod = PhoneAppDialMethodNormal;
	    
		if ( number )
		{
			dialP->number = MemPtrNew( StrLen( number ) + 1 );
			StrCopy( dialP->number, number );
			MemPtrSetOwner( dialP->number, 0 );
		}
		
		if ( name )
		{
			dialP->name = MemPtrNew( StrLen( name ) + 1 );
			StrCopy( dialP->name, name );
			MemPtrSetOwner( dialP->name, 0 );
		}	
	
	   MemPtrSetOwner ( dialP, 0 );
	    
	    // LaunchWithCommand( sysFileTApplication, hsFileCPhone, phoneAppLaunchCmdDial, &dial);
	   DmGetNextDatabaseByTypeCreator ( true, &stateInfo, sysFileTApplication, hsFileCPhone, true, 
											&cardNo, &dbID );
										   
		error = SysUIAppSwitch( cardNo, dbID, phoneAppLaunchCmdDial, dialP );
	}
	
	return ( error );
		
} // DialPhoneLaunch()

/*
 * LaunchAddressbook()
 */
static Err LaunchAddressbook( UInt16 recordIdx )
{	
	Err								error = errNone;	
	GoToParamsType*					goToParamsP = NULL;
	DmSearchStateType 				stateInfo;
	UInt16 							cardNo = 0;
   	LocalID							dbID = 0;
   	UInt32							result = 0;
	
	goToParamsP = (GoToParamsType *) MemPtrNew( sizeof( GoToParamsType ) );
	MemSet( goToParamsP, sizeof( GoToParamsType ), 0 );
	
	DmGetNextDatabaseByTypeCreator ( true, &stateInfo, sysFileTApplication, kPalmOneCreatorIDContacts, true, 
											&cardNo, &dbID );

	goToParamsP->dbCardNo = cardNo;
	goToParamsP->dbID = dbID;
	goToParamsP->recordNum = recordIdx;

	MemPtrSetOwner( goToParamsP, 0 );

	error = SysUIAppSwitch( cardNo, dbID, sysAppLaunchCmdGoTo, goToParamsP );
	// SysAppLaunch( cardNo, dbID, 0, sysAppLaunchCmdGoTo, (void *) goToParamsP, (UInt32 *) &result );
	
	return ( error );
		
} // LaunchAddressbook()
 
/*
 * Dial.c
 */
 
 