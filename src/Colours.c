/*
 * Colours.c
 */

#include "Colours.h"

// Prototypes
// Code 2 // EXTRA_SECTION_ONE
static void 					InitializeColorSelForm( FormType* pForm, Colors_t* colorsP ) EXTRA_SECTION_ONE;
static void 					CopyColorPrefs( FormType* pForm, Colors_t* colorsP ) EXTRA_SECTION_ONE;
// Code 1
Boolean 						ColorSelFormHandler( EventType* pEvent );
Boolean 						GadgetHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP );

/*
 * ShowColorSelForm()
 */
Boolean ShowColorSelForm( Colors_t* colorsP )
{
	Boolean 				retVal = false;
	FormType*				pForm = FrmInitForm( COLOR_SEL_FORM );
	FormActiveStateType 	curFrmState;
	
	if ( pForm )
	{
		FrmSaveActiveState ( &curFrmState );
		
		FrmSetActiveForm( pForm );
		
		InitializeColorSelForm( pForm, (Colors_t *) colorsP );
		
		FrmSetEventHandler( pForm, (FormEventHandlerType *) ColorSelFormHandler );
		
		FrmDrawForm( pForm );
		retVal = ( FrmDoDialog( pForm ) == COLOR_SEL_DONE_BUTTON );

		if ( retVal )
		{
			CopyColorPrefs( pForm, (Colors_t *) colorsP );
		}
		
		FrmEraseForm( pForm );
		FrmDeleteForm( pForm );

		pForm = NULL;
		
		FrmRestoreActiveState ( &curFrmState );
	}

	return ( retVal );
	
} // ShowColorSelForm()

/*
 * InitializeColorSelForm()
 */
static void InitializeColorSelForm( FormType* pForm, Colors_t* colorsP )
{
	ControlType*		pCtl = NULL;
	
	if ( ( !pForm ) || ( !colorsP ) ) return;
	
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_BG_GADGET ), ( FormGadgetHandlerType *) GadgetHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_FG_GADGET ), ( FormGadgetHandlerType *) GadgetHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_C1_GADGET ), ( FormGadgetHandlerType *) GadgetHandler );
	// FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_C2_GADGET ), ( FormGadgetHandlerType *) GadgetHandler );
	// FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_C3_GADGET ), ( FormGadgetHandlerType *) GadgetHandler );
	
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_BG_GADGET ), &( colorsP->background ) );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_FG_GADGET ), &( colorsP->foreground ) );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_C1_GADGET ), &( colorsP->color1 ) );
	// FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_C2_GADGET ), &( colorsP->color2 ) );
	// FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_C3_GADGET ), &( colorsP->color3 ) );
	
	pCtl = (ControlType *) FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, COLOR_CLICK_SOUNDS ) );
	CtlSetValue( pCtl, colorsP->bSoundClicks );
	
	pCtl = (ControlType *) FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, COLOR_SLOW_TO_A_STOP ) );
	CtlSetValue( pCtl, colorsP->bSlowToAStop );
	
	return;
	
} // InitializeColorSelForm()

/*
 * CopyColorPrefs()
 */
static void CopyColorPrefs( FormType* pForm, Colors_t* colorsP )
{
	ControlType*		pCtl = NULL;
	
	if ( ( !pForm ) || ( !colorsP ) ) return;

	pCtl = (ControlType *) FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, COLOR_CLICK_SOUNDS ) );
	colorsP->bSoundClicks = ( CtlGetValue( pCtl ) == 1 );
	
	pCtl = (ControlType *) FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, COLOR_SLOW_TO_A_STOP ) );
	colorsP->bSlowToAStop = ( CtlGetValue( pCtl ) == 1 );
	
	return;

} // CopyColorPrefs()


/*
 * ColorSelFormHandler()
 */
Boolean ColorSelFormHandler( EventType* pEvent )
{
	Boolean 				handled = false;
	FormType* 				pForm = FrmGetActiveForm();
	
	switch ( pEvent->eType )
	{
		case ctlSelectEvent:
			switch ( pEvent->data.ctlSelect.controlID )
			{
				case COLOR_SEL_DEFAULT_BUTTON:
					{
						Colors_t				colors;
						RGBColorType*			dataP = NULL;
						
						InitializeColorPrefs( &colors );
						
						dataP = (RGBColorType *)FrmGetGadgetData( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_BG_GADGET ) );
						*dataP = colors.background;
						
						dataP = (RGBColorType *)FrmGetGadgetData( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_FG_GADGET ) );
						*dataP = colors.foreground;
						
						dataP = (RGBColorType *)FrmGetGadgetData( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_C1_GADGET ) );
						*dataP = colors.color1;
						
						// dataP = (RGBColorType *)FrmGetGadgetData( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_C2_GADGET ) );
						// *dataP = colors.color2;
						
						// dataP = (RGBColorType *)FrmGetGadgetData( pForm, FrmGetObjectIndex( pForm, COLOR_SEL_C3_GADGET ) );
						// *dataP = colors.color3;
						
						FrmDrawForm( pForm );
						
						handled = true;
					}
				
					break;
					
				default:
					break;
			}
			break;
			
		default:
			break;
	}
	
	return ( handled );
	
} // ColorSelFormHandler()

/* 
 * GadgetHandler()
 */
Boolean GadgetHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP )
{
	Boolean 		handled = false;
	FormType*		pForm = FrmGetActiveForm();
	
	switch ( cmd )
	{
		case formGadgetDeleteCmd:
		
			// handled = true;
			
			break;
		
		case formGadgetDrawCmd:
			{
				FormType*				pForm = FrmGetActiveForm();
				RectangleType			rect = gadgetP->rect;
				IndexedColorType 		colorIdx;
				IndexedColorType		colorOutline;
				RGBColorType 			rgbOutline;
				RGBColorType*			dataP = NULL;
			
				dataP = (RGBColorType *)FrmGetGadgetData( pForm, FrmGetObjectIndex( pForm, gadgetP->id ) );
				
				WinPushDrawState();
				
				// Patch
				colorIdx = WinRGBToIndex( dataP );
				WinSetForeColor( colorIdx );
				WinDrawRectangle( &rect, 0 );
				
				// Outline
				rgbOutline.r = 0 ; rgbOutline.g = 0 ; rgbOutline.b = 0 ;
				colorOutline = WinRGBToIndex( &rgbOutline );
				WinSetForeColor( colorOutline );
				WinDrawRectangleFrame( rectangleFrame, &rect );
				
				WinPopDrawState();
			}
			handled = true; 
			gadgetP->attr.visible = true;
			
			break;
			
		case formGadgetEraseCmd:
		
			// handled = true; 
			
			break;
			
		case formGadgetHandleEventCmd:

			if ( ( (EventType *)paramP)->eType == frmGadgetEnterEvent )
			{
			   	FormType*				pForm = FrmGetActiveForm();
			   	RGBColorType 			rgb;
			   	IndexedColorType 		colorIdx;
			   	Char					selTitle[32];
				RGBColorType*			dataP = NULL;
			
				dataP = (RGBColorType *)FrmGetGadgetData( pForm, FrmGetObjectIndex( pForm, gadgetP->id ) );
			   	
			   	rgb = *dataP;
				
				colorIdx = WinRGBToIndex( &rgb );
				
				switch ( gadgetP->id )
				{
					case COLOR_SEL_BG_GADGET:
						StrCopy( selTitle, "Background..." );
						break;
										
					case COLOR_SEL_FG_GADGET:
						StrCopy( selTitle, "Text..." );
						break;

					case COLOR_SEL_C1_GADGET:
						StrCopy( selTitle, "Separator Lines..." );
						break;
					/*	
					case COLOR_SEL_C2_GADGET:
						StrCopy( selTitle, "Alert Text..." );
						break;
						
					case COLOR_SEL_C3_GADGET:
						StrCopy( selTitle, "No-Alert Text..." );
						break;
					*/
					default:
						StrCopy( selTitle, "Select Color" );
						break;	
				}
				
				if ( UIPickColor( &colorIdx, &rgb, UIPickColorStartPalette, selTitle, selTitle ) )
				{
					*dataP = rgb;
				}
				
				FrmUpdateForm( FrmGetFormId( pForm ), frmRedrawUpdateCode );
				
				handled = true;
			}
			
			break;
			
		default:
			break;
		
	}
	
	return ( handled );
	
} // GadgetHandler()

/*
 * Colours.c
 */