/////////////////////////////////////////////////////////////
// EMITER TOOLS											   //
/////////////////////////////////////////////////////////////
// VERSION  1.0  										   //
/////////////////////////////////////////////////////////////
// (c) 2002 Remotion, all rights reserved				   //
/////////////////////////////////////////////////////////////

/*
#include "c4d_general.h"
#include "c4d_shaderplugin.h"
#include "c4d_memory.h"
#include "c4d_resource.h"
#include "c4d_menuplugin.h"
*/

#include "c4d.h"

// forward declarations

Bool RegisterTDEmitter(void);
void FreeTDEmitter(void);

Bool RegisterPOLYEM(void);
void FreePOLYEM(void);

Bool RegisterTDMenu(void);
void FreeTDMenu(void);

Bool RegisterPTPTag(void);
void FreePTPTag(void);

Bool RegisterESPTag(void);
void FreeESPTag(void);

/*
Bool RegisterMEMTag(void);
void FreeMEMTag(void);

Bool RegisterForcer(void);
void FreeForcer(void);

//PaMod
Bool RegisterPaMod1(void);
void FreePaMod1(void);
Bool RegisterPaMod2(void);
void FreePaMod2(void);
Bool RegisterPaMod3(void);
void FreePaMod3(void);
Bool RegisterPaMod4(void);
void FreePaMod4(void);
Bool RegisterPaMod5(void);
void FreePaMod5(void);
Bool RegisterPaMod6(void);
void FreePaMod6(void);
Bool RegisterPaMod7(void);
void FreePaMod7(void);
Bool RegisterPaMod8(void);
void FreePaMod8(void);
Bool RegisterPaMod9(void);
void FreePaMod9(void);
Bool RegisterPaMod10(void);
void FreePaMod10(void);
//PaMod


Bool RegisterMOD(void);
void FreeMOD(void);

Bool RegisterOESP(void);
void FreeOESP(void);

Bool RegisterTDSubDialog(void);
void FreeTDSubDialog(void);
*/
#if 0
//########################## C4D  START ##########################
Bool C4D_PlStart(void *p1, void *p2)
{
	if (!resource.Init()) return FALSE; // don't start plugin without resource


	if (!RegisterTDEmitter()) return FALSE;
	if (!RegisterPOLYEM()) return FALSE;
	if (!RegisterTDMenu()) return FALSE;

	//TAG
	if (!RegisterPTPTag()) return FALSE;
	if (!RegisterESPTag()) return FALSE;

	//if (!RegisterMEMTag()) return FALSE;
	//if (!RegisterForcer()) return FALSE;

	//---PaMod---
	//if (!RegisterPaMod1()) return FALSE;
	//if (!RegisterPaMod2()) return FALSE;
	//if (!RegisterPaMod3()) return FALSE;
	//if (!RegisterPaMod4()) return FALSE;
	//if (!RegisterPaMod5()) return FALSE;
	//if (!RegisterPaMod6()) return FALSE;
	//if (!RegisterPaMod7()) return FALSE;
	//if (!RegisterPaMod8()) return FALSE;
	//if (!RegisterPaMod9()) return FALSE;
	//if (!RegisterPaMod10()) return FALSE;
	//
	//if (!RegisterMOD()) return FALSE;
	//if (!RegisterOESP()) return FALSE;


	//if (!RegisterTDSubDialog()) return FALSE;

	GePrint("Remotion TDEM Loaded!");//???
 
	return TRUE;
}

//########################## C4D END ##########################
void C4D_PlEnd(void)
{
	FreeTDEmitter();
	FreePOLYEM();
	FreeTDMenu();

	//TAG
	FreePTPTag();
	FreeESPTag();

	//FreeMEMTag();
	//FreeForcer();

	//---PaMod---
	//FreePaMod1();
	//FreePaMod2();
	//FreePaMod3();
	//FreePaMod4();
	//FreePaMod5();
	//FreePaMod6();
	//FreePaMod7();
	//FreePaMod8();
	//FreePaMod9();
	//FreePaMod10();
	//
	//FreeMOD();//TEST
	//FreeOESP();

	//FreeTDSubDialog();
}
#endif