/*
 * FormUtils.c
 */

#include "FormUtils.h"

/*
 * GetNumFromFieldText()
 */
UInt32 GetNumFromFieldText( FormType* pForm, UInt16 FieldID )
{
	FieldType*		pFld = NULL;
	MemHandle		memH = NULL;
	Char*			strP = NULL;
	UInt32			num = 0;
	
	pFld = (FieldType *) FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, FieldID ) );
	memH = FldGetTextHandle( pFld );
	if ( memH ) 
	{
		strP = (Char *) MemHandleLock( memH );
	
		num = StrAToI( strP ); 
	
		MemHandleUnlock( memH );
	}
	
	return ( num );

} // GetNumFromFieldText()
	
/* 
 * SetFieldTextFromInt()
 */
void SetFieldTextFromInt( FormType* pForm, UInt16 FieldID, UInt32 num )
{
	FieldType*		pFld = NULL;
	MemHandle		memH = NULL;
	Char*			strP = NULL;
	Char			numStr[maxStrIToALen] = "\0";
	Char			numMemH = NULL;
	
	StrIToA( numStr, num );

	pFld = (FieldType *) FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, FieldID ) );
	memH = FldGetTextHandle( pFld );
	
	if ( memH )
	{
		FldSetTextHandle( pFld, NULL );
	
		MemHandleResize( memH, StrLen( numStr ) + 1 );
	}
	else
	{
		memH = MemHandleNew( StrLen( numStr ) + 1 );
	}
			
	strP = (Char *) MemHandleLock( memH );	
	
	StrCopy( strP, numStr);
	
	MemHandleUnlock( memH );
	
	FldSetTextHandle( pFld, memH );

	return;	
	
} // SetFieldTextFromInt()

/*
 * FormUtils.c
 */