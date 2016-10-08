/*
 * Colours.h
 */

#ifndef __COLOURS_H__
#define __COLOURS_H__

#include <Hs.h>
#include <HsPhone.h>
#include <HsCreators.h>
#include <palmOneResources.h>
#include <PalmTypes.h>
#include <Window.h>
#include <Form.h>
#include <UIColor.h>

#include "Sections.h"
#include "Prefs.h"

/*
typedef struct {
  UInt16 id;
  FormGadgetAttrType attr;
  RectangleType rect;
  const void *data;
  FormGadgetHandlerType *handler;
} FormGadgetTypeInCallback
*/

#define COLOR_SEL_FORM							6000
#define COLOR_SEL_DONE_BUTTON					6001
#define COLOR_SEL_CANCEL_BUTTON					6002
#define COLOR_SEL_DEFAULT_BUTTON				6003
#define COLOR_SEL_BG_GADGET						6101
#define COLOR_SEL_FG_GADGET						6102
#define COLOR_SEL_C1_GADGET						6103
#define COLOR_SEL_C2_GADGET						6104
#define COLOR_SEL_C3_GADGET						6105
#define COLOR_CLICK_SOUNDS						6010
#define COLOR_SLOW_TO_A_STOP					6020

// Prototypes
// Code 2 // EXTRA_SECTION_ONE
extern Boolean 									ShowColorSelForm( Colors_t* colorsP ) EXTRA_SECTION_ONE;

#endif /* __COLOURS_H__ */

/*
 * Colours.h
 */