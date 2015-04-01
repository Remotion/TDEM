/////////////////////////////////////////////////////////////
// CINEMA SDK : MENU PLUGINS															 //
/////////////////////////////////////////////////////////////
// VERSION    : CINEMA 4D V6 AMBER												 //
/////////////////////////////////////////////////////////////
// (c) 1989-2001 MAXON Computer GmbH, all rights reserved	 //
/////////////////////////////////////////////////////////////
#include "c4d_gui.h"
#include "c4d_listview.h"
#include "c4d_baseselect.h"
#include "c4d_plugin.h"
#include "c4d_menuplugin.h"
#include "c4d_general.h"
#include "c4d_memory.h"
#include "c4d_basebitmap.h"
#include "c4d_file.h"
#include "c4d_tools.h"
#include "c4d_gui.h"
#include "c4d_basedocument.h"
#include "c4d_basecontainer.h"
#include "c4d_baseobject.h"
#include "c4d_basematerial.h"
#include "c4d_basetag.h"
#include "c4d_symbols.h"
#include "c4d_resource.h"

// be sure to use a unique ID obtained from www.plugincafe.com
#define SUBDIALOGID 1009608 //TEST ID

//----------------------------------
class MySubDialog1 : public SubDialog
{
	virtual Bool CreateLayout(void)
	{
		GroupBegin(0,BFH_SCALEFIT|BFV_SCALEFIT,2,0,"",0);
			AddButton(1000,BFH_SCALEFIT|BFV_SCALEFIT,0,0,"SubDialog1");
			AddButton(1001,BFH_SCALEFIT|BFV_SCALEFIT,0,0,"SubDialog1");
			AddButton(1002,BFH_SCALEFIT|BFV_SCALEFIT,0,0,"SubDialog1");
			AddButton(1003,BFH_SCALEFIT|BFV_SCALEFIT,0,0,"SubDialog1");
			AddEditNumberArrows(1006,BFH_SCALEFIT);
		GroupEnd();
		return TRUE;
	}
	virtual Bool InitValues(void)
	{
		SetLong(1006,0L,0,10);
		return TRUE;
	}
	virtual Bool Command(LONG id,const BaseContainer &msg)
	{
		return TRUE;
	}
};

class MySubDialog2 : public SubDialog
{
	virtual Bool CreateLayout(void)
	{
		GroupBegin(0,BFH_SCALEFIT|BFV_SCALEFIT,2,0,"",0);
			AddStaticText(1000,BFH_SCALEFIT|BFV_SCALEFIT,0,0,"SubDialog2",BORDER_THIN_IN);
			AddStaticText(1001,BFH_SCALEFIT|BFV_SCALEFIT,0,0,"SubDialog2",BORDER_THIN_IN);
			AddStaticText(1002,BFH_SCALEFIT|BFV_SCALEFIT,0,0,"SubDialog2",BORDER_THIN_IN);
			AddStaticText(1003,BFH_SCALEFIT|BFV_SCALEFIT,0,0,"SubDialog2",BORDER_THIN_IN);
			AddStaticText(1004,BFH_SCALEFIT|BFV_SCALEFIT,0,0,"SubDialog2",BORDER_THIN_IN);
			AddStaticText(1005,BFH_SCALEFIT|BFV_SCALEFIT,0,0,"SubDialog2",BORDER_THIN_IN);
			AddEditNumberArrows(1006,BFH_SCALEFIT);
		GroupEnd();
		return TRUE;
	}
	virtual Bool InitValues(void)
	{
		SetLong(1006,0L,0,10);
		return TRUE;
	}
	virtual Bool Command(LONG id,const BaseContainer &msg)
	{
		return TRUE;
	}
};
//----------------------------------

class MainDialog : public GeDialog
{
	private:
		MySubDialog1 subdialog1;
		MySubDialog2 subdialog2;
		//PAMOD10Dialog subdialog3;
		SubDialog    *lastdlg;

	public:
		MainDialog(void);
		virtual ~MainDialog(void);

		virtual Bool CreateLayout(void);
		virtual Bool InitValues(void);
		virtual Bool Command(LONG id,const BaseContainer &msg);
		virtual LONG Message(const BaseContainer &msg,BaseContainer &result);
};

MainDialog::MainDialog(void)
{
	lastdlg = NULL;
}

MainDialog::~MainDialog(void)
{
}

Bool MainDialog::CreateLayout(void)
{
	// first call the parent instance
	Bool res = GeDialog::CreateLayout();

	res = LoadDialogResource(DLG_TDSUB,NULL,0);

	if (res)
	{
		AttachSubDialog(&subdialog1,IDC_TDSUB_SUBDIALOG);
		lastdlg = &subdialog1;
		//AttachSubDialog(&subdialog3,IDC_TDSUB_SUBDIALOG);
		//lastdlg = &subdialog3;
		
	}

	return res;
}

Bool MainDialog::InitValues(void)
{
	// first call the parent instance
	if (!GeDialog::InitValues()) return FALSE;

	return TRUE;
}

Bool MainDialog::Command(LONG id,const BaseContainer &msg)
{
	/*
	switch (id)
	{
		
		case GADGET_SUB1:
			if (!lastdlg || !lastdlg->CheckClose())
			{
				AttachSubDialog(&subdialog1,GADGET_SUBDIALOG);
				LayoutChanged(GADGET_SUBDIALOG);
				lastdlg = &subdialog1;
			}
			break;

		case GADGET_SUB2:
			if (!lastdlg || !lastdlg->CheckClose())
			{
				AttachSubDialog(&subdialog2,GADGET_SUBDIALOG);
				LayoutChanged(GADGET_SUBDIALOG);
				lastdlg = &subdialog2;
			}
			break;
		
	}
	*/
	return TRUE;
}


LONG MainDialog::Message(const BaseContainer &msg,BaseContainer &result)
{
//	switch (msg.GetId())
//	{
//	}
	return GeDialog::Message(msg,result);
}

static String			*help = NULL;
static MainDialog	*dlg  = NULL;

static LONG GetState(BaseDocument *doc)
{
	return CMD_ENABLED;
}

static void Execute(BaseDocument *doc)
{
	dlg->Open(TRUE,SUBDIALOGID,-1,-1);
}

static Bool RestoreLayout(void *secret)
{
	if (!dlg) dlg = gNew MainDialog;
	if (!dlg) return FALSE;

	return dlg->RestoreLayout(SUBDIALOGID,secret);
}

Bool RegisterTDSubDialog(void)
{
	// decide by name if the plugin shall be registered - just for user convenience
	String name=GeLoadString(IDS_TDSUBDIALOG); if (!name.Content()) return TRUE;

	MENUPLUGIN mp;
	ClearMem(&mp,sizeof(mp));

	dlg  = gNew MainDialog;
	help = gNew String(name);
	if (!dlg || !help) return FALSE;

	mp.help = help->Get();
	mp.icon = NULL; // provide no icon for this plugin
	mp.GetState=GetState;
	mp.Execute=Execute;
	mp.RestoreLayout=RestoreLayout;

	return GeRegisterPlugin(C4DPL_MENU,SUBDIALOGID,name,&mp,sizeof(mp));
}

void FreeTDSubDialog(void)
{
	gDelete(help);
	gDelete(dlg);
}

