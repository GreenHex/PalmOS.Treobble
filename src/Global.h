/*
 * Global.h
 */
 
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

// Application constants
#define appFileCreator					'TrTu'
#define appVersionNum					0x01
#define appPrefVersionNum				0x03
#define appPrefID						0x00

#define ALERT_MEMORY_ERROR				"Memory Error"
#define ALERT_INCOMPATIBLE_ERROR		"Incompatible Device"
#define ADDR_DB_NOT_FOUND_ERROR			"AddressDB Not Found!\n\nContinue anyway..."
#define ADDR_DB_TOO_MANY_RECORDS		"Address Book has too many records...\n\nExiting!\n\nSorry... This is a BETA."
#define DIAL_OUT_ERROR					"Error dialing out..."

typedef enum {
	up				= -1,
	halt 			= 0,
	down			= 1,
} Direction_e;

typedef enum {
	stop			= 0,
	low				= 1,
	medium			= 4,
	high			= 8
} Speed_e;

typedef enum {
	a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
} letters_e;

#define SEARCH_STR_MAX_LEN				12

#endif /* __GLOBAL_H__ */

/*
 * Global.h
 */