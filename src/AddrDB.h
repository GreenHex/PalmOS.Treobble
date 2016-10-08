/*
 * AddrDB.h
 */
 
#ifndef __ADDRDB_H__
#define __ADDRDB_H__

#include <Hs.h>
#include <LocaleMgr.h>
#include <DataMgr.h>
#include <palmOneCreators.h>
#include <TextMgr.h>

#include "Global.h"
#include "AppResources.h"

#define addrDBName							"AddressDB" /* "ContactsDB-PAdd" | "AddressDB" */
#define addrDBType							'DATA'
#define addrDBCreatorID						sysFileCAddress /* kPalmOneCreatorIDContacts | sysFileCAddress */

#define firstAddressField					name

typedef enum
{
	name,
	firstName,
	company,
	phone1,
	phone2,
	phone3,
	phone4,
	phone5,
	address,
	city,
	state,
	zipCode,
	country,
	title,
	custom1,
	custom2,
	custom3,
	custom4,
	note,			// This field is assumed to be < 4K
	addressFieldsCount
} AddressFields;

typedef union
{
	struct
	{
		unsigned reserved	:13;
		unsigned note		:1;	// set if record contains a note handle
		unsigned custom4	:1;	// set if record contains a custom4
		unsigned custom3	:1;	// set if record contains a custom3
		unsigned custom2	:1;	// set if record contains a custom2
		unsigned custom1	:1;	// set if record contains a custom1
		unsigned title		:1;	// set if record contains a title
		unsigned country	:1;	// set if record contains a birthday
		unsigned zipCode	:1;	// set if record contains a birthday
		unsigned state		:1;	// set if record contains a birthday
		unsigned city		:1;	// set if record contains a birthday
		unsigned address	:1;	// set if record contains a address
		unsigned phone5		:1;	// set if record contains a phone5
		unsigned phone4		:1;	// set if record contains a phone4
		unsigned phone3		:1;	// set if record contains a phone3
		unsigned phone2		:1;	// set if record contains a phone2
		unsigned phone1		:1;	// set if record contains a phone1
		unsigned company	:1;	// set if record contains a company
		unsigned firstName	:1;	// set if record contains a firstName
		unsigned name		:1;	// set if record contains a name (bit 0)

	} bits;
	UInt32 allBits;
} AddrDBRecordFlags;

typedef union
{
	struct
	{
		unsigned reserved	 :10;
		unsigned phone8      :1;	// set if phone8 label is dirty
		unsigned phone7      :1;	// set if phone7 label is dirty
		unsigned phone6      :1;	// set if phone6 label is dirty
		unsigned note        :1;	// set if note label is dirty
		unsigned custom4     :1;	// set if custom4 label is dirty
		unsigned custom3     :1;	// set if custom3 label is dirty
		unsigned custom2     :1;	// set if custom2 label is dirty
		unsigned custom1     :1;	// set if custom1 label is dirty
		unsigned title       :1;	// set if title label is dirty
		unsigned country	 :1;	// set if country label is dirty
		unsigned zipCode	 :1;	// set if zipCode label is dirty
		unsigned state		 :1;	// set if state label is dirty
		unsigned city		 :1;	// set if city label is dirty
		unsigned address     :1;	// set if address label is dirty
		unsigned phone5      :1;	// set if phone5 label is dirty
		unsigned phone4      :1;	// set if phone4 label is dirty
		unsigned phone3      :1;	// set if phone3 label is dirty
		unsigned phone2      :1;	// set if phone2 label is dirty
		unsigned phone1      :1;	// set if phone1 label is dirty
		unsigned company     :1;	// set if company label is dirty
		unsigned firstName   :1;	// set if firstName label is dirty
		unsigned name        :1;	// set if name label is dirty (bit 0)

	} bits;
	UInt32 allBits;
} AddrDBFieldLabelsDirtyFlags;

typedef enum
{
	workLabel,
	homeLabel,
	faxLabel,
	otherLabel,
	emailLabel,
	mainLabel,
	pagerLabel,
	mobileLabel
} AddressPhoneLabels;

typedef union
{
	struct
	{
		unsigned reserved:8;
		unsigned displayPhoneForList:4;	// The phone displayed for the list view 0 - 4
		unsigned phone5:4;				// Which phone (home, work, car, ...)
		unsigned phone4:4;
		unsigned phone3:4;
		unsigned phone2:4;
		unsigned phone1:4;
	} phones;
	UInt32 phoneBits;
} AddrOptionsType;

typedef struct
{
	AddrOptionsType		options; // Display by company or by name
	Char*				fields[addressFieldsCount];
} AddrDBRecordType;

typedef AddrDBRecordType *AddrDBRecordPtr;

typedef struct {
	AddrOptionsType		options;        // Display by company or by name
	AddrDBRecordFlags	flags;
	UInt8				companyFieldOffset;   // Offset from firstField
	char				firstField;
} PrvAddrPackedDBRecord;

#define NAME_LENGTH					100
#define PHONE_NUM_LENGTH			32

typedef struct {
	// UInt32							uniqueID;
	UInt16							recordIdx;
	Char*							name;
} Contacts_t;

#define NO_NAME_STR					"-Unnamed-"
#define	MAX_NUM_CONTACTS			3001 // 1501
#define PERCENTAGE_STR				"%"
#define PROGRESS_BAR_LENGTH			84
#define PROGRESS_BAR_X				3
#define PROGRESS_BAR_Y				13

Contacts_t*							contacts[MAX_NUM_CONTACTS];
extern UInt16						gCharLocation[(UInt16) 26];
extern UInt16						gNumContactsRecords;
extern UInt16						gCategoryIdx;

// #define IsPhoneNumberValid( idx, flags )   	( ( 1 << ( idx + 5 ) ) & ( ( flags ) ) )

// Prototypes
extern Boolean 		SearchStringInArray( Char* searchStr, UInt16* idx );
extern UInt16		GetNumContactsRecords( UInt16 mode );
extern UInt16 		AddrDBGetRecord( DmOpenRef dbP, UInt16* recordIdxP, AddrDBRecordFlags* flagsP, AddrDBRecordPtr recordP, MemHandle* memHP );
extern Err 			AddrDBGetDatabase( DmOpenRef* dbPP, UInt16 mode );
extern Err			InitializeContactsList( void );
extern void 		DestroyContactsList( void );

#endif /* __ADDRDB_H__ */

/*
 * AddrDB.h
 */