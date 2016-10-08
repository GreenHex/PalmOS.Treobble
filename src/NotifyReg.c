/*
 * NotifyReg.c
 */

#include "NotifyReg.h"

/*
 * RegisterForNotifications()
 */
void RegisterForNotifications(AppPreferences_t* pPrefs)
{
	Err				error = errNone;	
	// Int32			NotifyPriority = 96; // sysNotifyNormalPriority = 0
	UInt16 			cardNo; 
	LocalID  		dbID;
	
	SysCurAppDatabase(&cardNo, &dbID);			

	// disable all notifications
	SysNotifyUnregister(cardNo, dbID, sysNotifyVirtualCharHandlingEvent, sysNotifyNormalPriority);

	// reenable
	if ( pPrefs->bEnabled )
	{
		SysNotifyRegister(cardNo, dbID, sysNotifyVirtualCharHandlingEvent, NULL, sysNotifyNormalPriority, NULL);								
	}
	
	return;
	
} // RegisterForNotifications()

/*
 * NotifyReg.c
 */