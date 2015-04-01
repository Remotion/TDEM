/////////////////////////////////////////////////////////////
// 3d Emiter											   //
/////////////////////////////////////////////////////////////
// VERSION  1.0  										   //
/////////////////////////////////////////////////////////////
// (c) 2002 Remotion, all rights reserved				   //
/////////////////////////////////////////////////////////////

/*
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
*/
#include "c4d.h"
#include "c4d_symbols.h"

#include "myprint.h"
#include "polyem/DefPolyEm.h"//PolyEm Defines

#define TDMENU_ID 1007707

/*
enum
{
	GADGET_ADDROW = 5000,
	GADGET_SUBROW,
	GROUP_DYNAMIC,
	GROUP_SCROLL,

	GADGET_DRAG = 6000,

	_dummy
};

class AsyncDialog : public GeDialog
{
	private:
		LONG rows;
		String array_org[10],array_drag[10];

		Bool CheckDropArea(LONG id,const BaseContainer &msg);
		void CreateDynamicGroup(void);
		void ReLayout(void);

	public:
		AsyncDialog(void);
		virtual Bool CreateLayout(void);
		virtual Bool InitValues(void);
		virtual Bool Command(LONG id,const BaseContainer &msg);
		virtual LONG Message(const BaseContainer &msg,BaseContainer &result);

};

enum
{
	IDC_OFFSET = 1001,
	IDC_ACCESS = 1002
};

AsyncDialog::AsyncDialog(void)
{
	rows = 1;
}

Bool AsyncDialog::CreateLayout(void)
{
	// first call the parent instance
	Bool res = GeDialog::CreateLayout();

	SetTitle("TDEmitter");

	GroupBegin(0,BFH_SCALEFIT,5,0,"",0);
	{
		GroupBorderSpace(4,4,4,4);
		AddButton(GADGET_ADDROW,BFH_FIT,0,0,"add row");
		AddButton(GADGET_SUBROW,BFH_FIT,0,0,"sub row");
	}
	GroupEnd();

	GroupBegin(0,BFV_SCALEFIT|BFH_SCALEFIT,1,0,"Drop objects, tags, materials here",0);
	{
		GroupBorder(BORDER_GROUP_IN|BORDER_WITH_TITLE);
		GroupBorderSpace(4,4,4,4);

		ScrollGroupBegin(GROUP_SCROLL,BFH_SCALEFIT|BFV_SCALEFIT,SCROLLGROUP_VERT);
		{
			GroupBegin(GROUP_DYNAMIC,BFV_TOP|BFH_SCALEFIT,2,0,"",0);
			{
				CreateDynamicGroup();
			}
			GroupEnd();
		}
		GroupEnd();
	}
	GroupEnd();
	

	//MenuFlushAll();	
	//	MenuSubBegin("Menu1");
	//		MenuAddString(GADGET_ADDROW,"add row");
	//		MenuAddString(GADGET_SUBROW,"sub row");
	//		MenuAddSeparator();
	//	MenuSubEnd();
	//MenuFinished();

	
	return res;
}

void AsyncDialog::ReLayout(void)
{
	LayoutFlushGroup(GROUP_DYNAMIC);
	CreateDynamicGroup();
	LayoutChanged(GROUP_DYNAMIC);
}

Bool AsyncDialog::InitValues(void)
{
	// first call the parent instance
	if (!GeDialog::InitValues()) return FALSE;

	SetLong(IDC_OFFSET,100,0,100,1);
	SetBool(IDC_ACCESS,TRUE);

	return TRUE;
}


Bool AsyncDialog::CheckDropArea(LONG id,const BaseContainer &msg)
{
	LONG x,y,w,h,dx,dy;
	GetDragPosition(msg,&dx,&dy);
	GetItemDim(id,&x,&y,&w,&h);
	return dy>y && dy<y+h;
}

void AsyncDialog::CreateDynamicGroup(void)
{
	LONG i;
	for (i=0;i<rows;i++)
	{
		//AddCheckbox(0,BFH_LEFT,0,0,"Rows "+LongToString(i+1));

		AddStaticText(GADGET_DRAG+i,BFH_SCALEFIT,260,0,array_org[i],BORDER_THIN_IN);
		AddEditNumberArrows(0,BFH_SCALEFIT,0,0);
	}
}

Bool AsyncDialog::Command(LONG id,const BaseContainer &msg)
{
	switch (id)
	{
		case GADGET_ADDROW:
			if (rows < 10)
			{
				rows++;
				ReLayout();
			}
			break;

		case GADGET_SUBROW:
			if (rows>1)
			{
				rows--;
				ReLayout();
			}
			break;
	}
	return TRUE;
}


LONG AsyncDialog::Message(const BaseContainer &msg,BaseContainer &result)
{
	switch (msg.GetId())
	{
		case BFM_DRAGRECEIVE:
			{
				String prefix = "Dragging ";
				LONG i,id=-1;
				if (msg.GetLong(BFM_DRAG_FINISHED)) prefix = "Dropped ";

				if (CheckDropArea(GROUP_SCROLL,msg))
				{
					for (i=0;i<rows;i++)
					{
						if (CheckDropArea(GADGET_DRAG+i,msg)) { id = i; break; }
					}
				}
				if (id!=-1)
				{
					if (msg.GetLong(BFM_DRAG_LOST))
					{
						for (i=0;i<rows;i++)
						{
							SetString(GADGET_DRAG+i,array_org[i]);
						}
					}
					else
					{
						String string;
						LONG type = 0;
						void *object = NULL;
						
						GetDragObject(msg,&type,&object);

						switch (type)
						{
							case DRAGTYPE_OBJECT:
								{
									BaseObject *obj = (BaseObject*)object;
									string = prefix+"BaseObject: "+obj->GetName();
								}
								break;

							case DRAGTYPE_TAG:
								{
									BaseTag *tag = (BaseTag*)object;
									string = prefix+"BaseTag "+LongToString(tag->GetType());
								}
								break;

							case DRAGTYPE_MATERIAL:
								{
									BaseMaterial *mat = (BaseMaterial*)object;
									string = prefix+"BaseMaterial: "+mat->GetName();
								}
								break;

							default:
								string = prefix+"unknown object";
								break;
						}

						for (i=0;i<rows;i++)
						{
							array_drag[i] = array_org[i];
						}
						array_drag[id] = string;

						if (msg.GetLong(BFM_DRAG_FINISHED))
							array_org[id] = string;

						for (i=0;i<rows;i++)
						{
							SetString(GADGET_DRAG+i,array_drag[i]);
						}

						return SetDragDestination(MOUSE_POINT_HAND);
					}
				}
			}
			break;
	}
	return GeDialog::Message(msg,result);
}
*/
static String		*help=NULL;
static BaseBitmap   *icon=NULL;
//static AsyncDialog	*dlg=NULL;

static LONG GetState(BaseDocument *doc)
{
	return CMD_ENABLED;
}


//CREATE POLYEM SET??
static void Execute(BaseDocument *doc)
{
	//dlg->Open(TRUE,PLUGID,-1,-1);
	BaseObject *em = NULL;
	PluginObject *poem = NULL;
	BaseObject *poly = NULL;
	BaseObject *part = NULL;

	BaseContainer res;
	if (GetInputState(BFM_INPUT_KEYBOARD,KEY_CONTROL,res) && res.GetLong(BFM_INPUT_VALUE)){
		poly = doc->GetActiveObject();
	}	

		poem = (PluginObject*)AllocPluginObject(POLYEM_ID);if (!poem) return;
		em = AllocObject(Oparticle);if (!em) return;
		part = AllocObject(Oprimitive,PRIMITIVE_SPHERE);if (!part) return;
		part->SetPhong(80.0);//SetPhong of poly
		BaseContainer partdata = part->GetDataInstance();
		partdata.SetReal(PRIM_SPHERE_RAD,5.0);
		partdata.SetLong(PRIM_SPHERE_SUB,5);

		//Set Emiter Data
		BaseContainer emdata = em->GetDataInstance();
		emdata.SetLong(PARTICLEOBJECT_BIRTHEDITOR, 100);
		emdata.SetLong(PARTICLEOBJECT_BIRTHRAYTRACER, 100);
		emdata.SetReal(PARTICLEOBJECT_SPEED,0.0);
		BaseTime tim(1000.0,25.0);
		emdata.SetTime(PARTICLEOBJECT_LIFETIME,tim);
		emdata.SetReal(PARTICLEOBJECT_SIZEX,0.0);
		emdata.SetReal(PARTICLEOBJECT_SIZEY,0.0);
		emdata.SetReal(PARTICLEOBJECT_VISIBILITY,0.0);

		//Set PolyEm Data
		BaseContainer poemdata = poem->GetDataInstance();
		poemdata.SetBool(PLE_VISIBLE,TRUE);

		if (!poly){
			poly = AllocObject(Oprimitive,PRIMITIVE_SPHERE);if (!poly) return;
			poly->SetName("PolyEM-Set");//????????
			poly->SetPhong(80.0);//SetPhong of pol
			doc->AddUndo(UNDO_OBJECT_NEW,poly);
			doc->InsertObject(poly,NULL,NULL,TRUE);
		}else{
			doc->AddUndo(UNDO_OBJECT_NEW,poem);
		}

		//Insert Objects
		//doc->InsertObject(poem,poly,NULL,TRUE);
		//doc->InsertObject(em,poem,NULL,TRUE);
		//doc->InsertObject(part,em,NULL,TRUE);
		poem->InsertUnder(poly);
		em->InsertUnder(poem);
		part->InsertUnder(em);

		doc->SetActiveObject(poly);

	doc->Message(MSG_UPDATE);
	GeEventAdd(MSG_DOCUMENTCHANGED);
}


static Bool RestoreLayout(void *secret)
{
	//if (!dlg) dlg = gNew AsyncDialog;
	//if (!dlg) return FALSE;

	//return dlg->RestoreLayout(PLUGID,secret);
	return TRUE;
}

Bool RegisterTDMenu(void)
{
	// decide by name if the plugin shall be registered - just for user convenience
	String name=GeLoadString(IDS_TDMENU); if (!name.Content()) return TRUE;

	MENUPLUGIN mp;
	ClearMem(&mp,sizeof(mp));

	//dlg  = gNew AsyncDialog;
	help = gNew String;
	icon = AllocBaseBitmap();
	if (!help || !icon) return FALSE;
	if (icon->Init(GeGetPluginPath()+String("res")+String("polyem.tif"))!=IMAGE_OK) return FALSE;
	*help = "CREATE POLYEM SET Menu";
	//if (!dlg || !help) return FALSE;

	mp.help = help->Get();
	mp.icon = icon; // provide no icon for this plugin
	mp.GetState=GetState;
	mp.Execute=Execute;
	mp.RestoreLayout=RestoreLayout;

	return GeRegisterPlugin(C4DPL_MENU,TDMENU_ID,name,&mp,sizeof(mp));
}

void FreeTDMenu(void)
{
	gDelete(help);
	FreeBaseBitmap(icon);
	//gDelete(dlg);
}

