/*
 * FormUtils.h
 */
 
#ifndef __FORMUTILS_H__
#define __FORMUTILS_H__

#include <Hs.h>
#include <HsPhone.h>
#include <HsNav.h>
#include <HsExt.h>
#include <palmOneResources.h>
#include <Form.h>
#include <Field.h>
#include <StringMgr.h>

#include "AppResources.h"

// Prototypes
extern UInt32 				GetNumFromFieldText( FormType* pForm, UInt16 FieldID );
extern void 				SetFieldTextFromInt( FormType* pForm, UInt16 FieldID, UInt32 num );

#endif /* __FORMUTILS_H__ */

/*
 * FormUtils.h
 */