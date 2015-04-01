/////////////////////////////////////////////////////////////
// 3d Emiter											   //
/////////////////////////////////////////////////////////////
// VERSION  1.0  										   //
/////////////////////////////////////////////////////////////
// (c) 2002 Remotion, all rights reserved				   //
/////////////////////////////////////////////////////////////

#include "c4d_file.h"
#include "c4d_general.h"
#include "c4d_memory.h"
#include "c4d_thread.h"
#include "c4d_basedocument.h"
#include "c4d_baseobject.h"
#include "c4d_basebitmap.h"
#include "c4d_basematerial.h"
#include "c4d_basechannel.h"
#include "c4d_basedraw.h"
#include "c4d_resource.h"
#include "c4d_particleobject.h"
#include "c4d_objectplugin.h"
#include "c4d_gui.h"
#include "c4d_baseselect.h"
#include "c4d_basetag.h"
#include "c4d_symbols.h"
#include "c4d_tools.h"

#include "../mytools.h"
#include "../myprint.h"
#include "../ChSampler.h"
#include "../bitmappreview.h"
#include "../VDSampler.h"

//Hiden Defines
#define MAX_OVERSAMPLING	64
#define MAX_CHANELL			9

//Glob Var_
#define PLE_VER				2000
#define PLE_REVAP			2001
#define PLE_DRND			2002
//_Glob Var

#include "DefPolyEm.h"//PolyEm Defines

//------------------------------------

//############### Weigh >>> ##############
struct Weigh
{
	Weigh(void)
	{	
		w0 = 0.0;
		w1 = 0.0;
		w2 = 0.0;
		w3 = 0.0;
	};
	void rand3(Random *rnd);
	void rand4(Random *rnd);

	Real w0;
	Real w1;
	Real w2;
	Real w3;
};

void Weigh::rand3(Random *rnd)
{

	//www.exaflop.org/docs/cgafaq/cga6.html
	//http://www.cg.tuwien.ac.at/studentwork/CESCG97/olearnik/txmap.htm
	w0 = rnd->Get01();
	w1 = rnd->Get01();
	if(w0+w1 > 1.0) {w0 = 1.0-w0; w1 = 1.0-w1;}
	w2 = 1.0-w0-w1;
	w3 = 0.0;
}
void Weigh::rand4(Random *rnd)
{
	//WORK ABER TRIANGULATION IST SICHTBAR??? NUR CONVEX???
	if (rnd->Get01() > 0.5){
		w0 = rnd->Get01();
		w1 = rnd->Get01();
		if(w0+w1 > 1.0) {w0 = 1.0-w0; w1 = 1.0-w1;}
		w2 = 1.0-w0-w1;
		w3 = 0.0;
	}else{
		w0 = rnd->Get01();
		w3 = rnd->Get01();
		if(w0+w3 > 1.0) {w0 = 1.0-w0; w3 = 1.0-w3;}
		w2 = 1.0-w0-w3;
		w1 = 0.0;
	}
}
//############### <<< Weigh ##############


//#############################  MADAL DIALOG ###########################################
//"""""""""""""""""""""""""""""""""""""""
class PLEDialog : public GeModalDialog
{
	friend class BitmapPreview;

	private:
		BitmapPreview	gad_preview[MAX_CHANELL];
		Bool PreviewChannel(LONG gadnr = 0, Bool test = TRUE, LONG chnr = 0,LONG tnum = 0);
		Bool EditChannel(LONG gadnr = 0,LONG chnr = 0,LONG tnum = 0);

		BaseContainer *data;
		PluginObject *plugOb;
	public:
		PLEDialog(BaseContainer *bc, PluginObject *op) { data = bc;  plugOb = op; }

		virtual Bool CreateLayout(void);
		virtual Bool InitValues(void);
		virtual Bool Command(LONG id, const BaseContainer &msg);
};

//########################## PREVIEW ##########################
Bool PLEDialog::PreviewChannel(LONG gadnr, Bool test, LONG chnr,LONG tnum)//test -> testeo ob gleiche kanall oder zeit
{	
	if (!plugOb) return FALSE;
	gadnr = gadnr % MAX_CHANELL;//Sicherheit

	BaseDocument	*doc = NULL;
	BaseObject		*op = NULL;
	TextureTag		*texturetag = NULL;	
	BaseMaterial	*mat = NULL;
	BaseChannel		*cha = NULL;
	BaseTime		actTime;

	//doc = GetActiveDocument();if (!doc)	return FALSE;
	doc = plugOb->GetDocument();if (!doc)	return FALSE;
	gad_preview[gadnr].doc = doc;

	op = plugOb->GetUp(); if (!op) goto Error; //Remo: 02.04.2007
	//op = plugOb->up(); if (!op) goto Error;
	gad_preview[gadnr].ob = op;

	if (tnum < 0) goto Error;
	texturetag = (TextureTag*)op->GetTag(Ttexture,tnum);if (!texturetag) goto Error;
	mat = texturetag->GetMaterial();if (!mat) goto Error;
	cha = mat->GetChannel(chnr);if (!cha) goto Error;

	actTime = doc->GetTime();
	if (test){
		if (gad_preview[gadnr].chan!=cha || gad_preview[gadnr].lastTime!=actTime){//Draw nur wen andere Kanall oder andere Zeit!);
			gad_preview[gadnr].lastTime = actTime;
			gad_preview[gadnr].chan = cha;
			gad_preview[gadnr].CalcPreview();
		}
	}else{//Draw immer
		gad_preview[gadnr].lastTime = actTime;
		gad_preview[gadnr].chan = cha;
		gad_preview[gadnr].CalcPreview();
	}

	return TRUE;

Error:
	gad_preview[gadnr].chan = NULL;
	gad_preview[gadnr].CalcPreview();
	return FALSE;
}


//????????
//########################## EDIT ##########################
Bool PLEDialog::EditChannel(LONG gadnr,LONG chnr,LONG tnum)
{	
	if (!plugOb) return FALSE;
	gadnr = gadnr % MAX_CHANELL;//Sicherheit

	StopAllThreads();//????????????????

	BaseDocument *doc = NULL;
	BaseObject *op = NULL;
	TextureTag *texturetag = NULL;	
	BaseMaterial *mat = NULL;
	BaseChannel *cha = NULL;
	CHANNELSHADER *sha = NULL;

	doc = plugOb->GetDocument();if (!doc) return FALSE;
	gad_preview[gadnr].doc = doc;

	op = plugOb->GetUp();if (!op)	return FALSE;
	gad_preview[gadnr].ob = op;

	gad_preview[gadnr].doc = doc;
	gad_preview[gadnr].lastTime = doc->GetTime();

	if (tnum < 0) {
		gad_preview[gadnr].chan = NULL;
		gad_preview[gadnr].CalcPreview();
		return FALSE;
	}
	texturetag = (TextureTag*)op->GetTag(Ttexture,tnum);if (!texturetag) return FALSE;
	mat = texturetag->GetMaterial();if (!mat) return FALSE;
	cha = mat->GetChannel(chnr);if (!cha) return FALSE;

	PLUGIN_DATA	*plug =	cha->GetPluginData();
	sha = cha->GetShader();
	if (sha) {//TEST
		CHANNELSHADER tt = *sha;	
		if (tt.EditSettings != NULL) sha->EditSettings(plug,doc);

		//WORK ???
		gad_preview[gadnr].chan = cha;
		gad_preview[gadnr].CalcPreview();

		mat->Message(MSG_UPDATE);
		mat->Update(MATERIALUPDATE_PREVIEW,TRUE);
		//GeEventAdd(MSG_MATCHANGED);
		//DrawViews(DA_ONLY_ACTIVE_VIEW | DA_NO_ANIMATION | DA_NO_KEYFRAME);
	}

	return TRUE;
}


//"""""""""""""""""""""""""""""""""""""""
Bool PLEDialog::CreateLayout(void)
{
	Bool res = 
	GeModalDialog::CreateLayout() && 
	LoadDialogResource(DLG_POLYEM,NULL,0) &&
	AttachUserArea(gad_preview[1],IDC_PLE_USER1) &&
	AttachUserArea(gad_preview[2],IDC_PLE_USER2) &&
	AttachUserArea(gad_preview[3],IDC_PLE_USER3) &&
	AttachUserArea(gad_preview[4],IDC_PLE_USER4) &&
	AttachUserArea(gad_preview[5],IDC_PLE_USER5) &&
	AttachUserArea(gad_preview[6],IDC_PLE_USER6);
	//AttachUserArea(gad_preview[7],IDC_PLE_USER7) &&
	//AttachUserArea(gad_preview[8],IDC_PLE_USER8);

	return res;
}

//"""""""""""""""""""""""""""""""""""""""
Bool PLEDialog::InitValues(void)
{
	//Bool bo;
	//LONG lo;
	String st = "";

	// first call the parent instance
	if (!GeModalDialog::InitValues()) return FALSE;

	//Emitter
    SetString(IDC_PLE_EDN3,data,PLE_EMITTERNAME);
	SetLong(IDC_PLE_N3,data,PLE_NUM_EMITTER,0,1000,1);

	//Quele
    SetString(IDC_PLE_EDN4,data,PLE_OBJECTNAME);
	SetLong(IDC_PLE_N4,data,PLE_NUM_OBJECT,0,1000,1);

	//rest
	SetLong(IDC_PLE_RND5,data,PLE_SEED,0,10000,1);
	SetLong(IDC_PLE_DIR1,data,PLE_DIRECTION_TYP);

	SetMeter(IDC_PLE_ED1,data,PLE_SPEED,-MAXRANGE,MAXRANGE,1.0);
	SetPercent(IDC_PLE_ED2_X,data,PLE_DISPERSING_X,0.0,5000.0,1.0);
	SetPercent(IDC_PLE_ED2_Y,data,PLE_DISPERSING_Y,0.0,5000.0,1.0);
	SetPercent(IDC_PLE_ED2_Z,data,PLE_DISPERSING_Z,0.0,5000.0,1.0);

	SetBool(IDC_PLE_VISIBLE,data,PLE_VISIBLE);
	SetBool(IDC_PLE_CHECK1,data,PLE_RANDOM);

	//PARAMETER
	SetLong(IDC_PLE_SPEEDCHAN,data,PLE_SPEEDCHAN);
	SetLong(IDC_PLE_SPEEDFIL,data,PLE_SPEEDFIL);

	SetLong(IDC_PLE_VISICHAN,data,PLE_VISICHAN);
	SetLong(IDC_PLE_VISIFIL,data,PLE_VISIFIL);

	SetLong(IDC_PLE_OFFCHAN,data,PLE_OFFCHAN);
	SetLong(IDC_PLE_OFFFIL,data,PLE_OFFFIL);
	SetReal(IDC_PLE_OFFMAX,data,PLE_OFFMAX);


	//Chanels 
	SetLong(IDC_PLE_CHANNEL1,data,PLE_CHANNEL1);
	SetLong(IDC_PLE_CHANNEL2,data,PLE_CHANNEL2);
	SetLong(IDC_PLE_CHANNEL3,data,PLE_CHANNEL3);
	SetLong(IDC_PLE_CHANNEL4,data,PLE_CHANNEL4);
	SetLong(IDC_PLE_CHANNEL5,data,PLE_CHANNEL5);
	SetLong(IDC_PLE_CHANNEL6,data,PLE_CHANNEL6);
	//SetLong(IDC_PLE_CHANNEL7,data,PLE_CHANNEL7);
	//SetLong(IDC_PLE_CHANNEL8,data,PLE_CHANNEL8);

	//Textur
	SetLong(IDC_PLE_TEXTURE1,data,PLE_TEXTURE1,-1,1000,1);
	SetLong(IDC_PLE_TEXTURE2,data,PLE_TEXTURE2,-1,1000,1);
	SetLong(IDC_PLE_TEXTURE3,data,PLE_TEXTURE3,-1,1000,1);
	SetLong(IDC_PLE_TEXTURE4,data,PLE_TEXTURE4,-1,1000,1);
	SetLong(IDC_PLE_TEXTURE5,data,PLE_TEXTURE5,-1,1000,1);
	SetLong(IDC_PLE_TEXTURE6,data,PLE_TEXTURE6,-1,1000,1);
	//SetLong(IDC_PLE_TEXTURE7,data,PLE_TEXTURE7);
	//SetLong(IDC_PLE_TEXTURE8,data,PLE_TEXTURE8);
	//-------------------------
	//Emitter
	GetString(IDC_PLE_EDN3,st);
	Enable(IDC_PLE_N3,(st != ""));
	//Enable(IDC_PLE_STATIC2,(st == ""));

	//Quele
	GetString(IDC_PLE_EDN4,st);
	Enable(IDC_PLE_N4,(st != ""));
	//TEXTUR
	/*
	GetLong(IDC_PLE_CHA1,lo);
	Enable(IDC_PLE_TEX_N1,(lo != 20));

	GetLong(IDC_PLE_CHA2,lo);
	Enable(IDC_PLE_TEX_N2,(lo != 20));

	GetLong(IDC_PLE_CHA3,lo);
	Enable(IDC_PLE_TEX_N3,(lo != 20));
	*/

	//rest
	//GetBool(IDC_PLE_CHECK2,bo);
	//Enable(IDC_PLE_GR3,bo);
	//Enable(IDC_PLE_GR4,bo);
	//Enable(IDC_PLE_GR5,bo);


	LONG chnr,tnum;
	GetLong(IDC_PLE_CHANNEL1,chnr);
	GetLong(IDC_PLE_TEXTURE1,tnum);
	PreviewChannel(1,TRUE,chnr,tnum);

	GetLong(IDC_PLE_CHANNEL2,chnr);
	GetLong(IDC_PLE_TEXTURE2,tnum);
	PreviewChannel(2,TRUE,chnr,tnum);

	GetLong(IDC_PLE_CHANNEL3,chnr);
	GetLong(IDC_PLE_TEXTURE3,tnum);
	PreviewChannel(3,TRUE,chnr,tnum);

	GetLong(IDC_PLE_CHANNEL4,chnr);
	GetLong(IDC_PLE_TEXTURE4,tnum);
	PreviewChannel(4,TRUE,chnr,tnum);

	GetLong(IDC_PLE_CHANNEL5,chnr);
	GetLong(IDC_PLE_TEXTURE5,tnum);
	PreviewChannel(5,TRUE,chnr,tnum);

	GetLong(IDC_PLE_CHANNEL6,chnr);
	GetLong(IDC_PLE_TEXTURE6,tnum);
	PreviewChannel(6,TRUE,chnr,tnum);

	/*
	GetLong(IDC_PLE_CHANNEL6,chnr);
	GetLong(IDC_PLE_TEXTURE6,tnum);
	PreviewChannel(6,TRUE,chnr,tnum);

	GetLong(IDC_PLE_CHANNEL7,chnr);
	GetLong(IDC_PLE_TEXTURE7,tnum);
	PreviewChannel(7,TRUE,chnr,tnum);
	*/

	//TEMPORAL

	Enable(IDC_PLE_ED2_X,FALSE);
	Enable(IDC_PLE_ED2_Y,FALSE);
	Enable(IDC_PLE_ED2_Z,FALSE);

	return TRUE;
}

//""""""""""""""""""""" On Cange """""""""""""""""""""""""
Bool PLEDialog::Command(LONG id, const BaseContainer &msg)
{
	//Bool bo;
	//LONG lo;
	String st = "";
	Random *rnd = NULL;
	LONG chnr,tnum;
	//BaseDocument *doc = NULL;
	//ParticleObject *EmiterOb = NULL; 

	switch (id)
	{
		//Preview 1
		case IDC_PLE_TEXTURE1: case IDC_PLE_CHANNEL1:
			GetLong(IDC_PLE_CHANNEL1,chnr);
			GetLong(IDC_PLE_TEXTURE1,tnum);
			PreviewChannel(1,TRUE,chnr,tnum);
		break;
		//Preview 2
		case IDC_PLE_TEXTURE2: case IDC_PLE_CHANNEL2:
			GetLong(IDC_PLE_CHANNEL2,chnr);
			GetLong(IDC_PLE_TEXTURE2,tnum);
			PreviewChannel(2,TRUE,chnr,tnum);
		break;
		//Preview 3
		case IDC_PLE_TEXTURE3: case IDC_PLE_CHANNEL3:
			GetLong(IDC_PLE_CHANNEL3,chnr);
			GetLong(IDC_PLE_TEXTURE3,tnum);
			PreviewChannel(3,TRUE,chnr,tnum);
		break;
		//Preview 4
		case IDC_PLE_TEXTURE4: case IDC_PLE_CHANNEL4:
			GetLong(IDC_PLE_CHANNEL4,chnr);
			GetLong(IDC_PLE_TEXTURE4,tnum);
			PreviewChannel(4,TRUE,chnr,tnum);
		break;
		//Preview 5
		case IDC_PLE_TEXTURE5: case IDC_PLE_CHANNEL5:
			GetLong(IDC_PLE_CHANNEL5,chnr);
			GetLong(IDC_PLE_TEXTURE5,tnum);
			PreviewChannel(5,TRUE,chnr,tnum);
		break;
		//Preview 6
		case IDC_PLE_TEXTURE6: case IDC_PLE_CHANNEL6:
			GetLong(IDC_PLE_CHANNEL6,chnr);
			GetLong(IDC_PLE_TEXTURE6,tnum);
			PreviewChannel(6,TRUE,chnr,tnum);
		break;
		/*
		case IDC_PLE_TEXTURE7: case IDC_PLE_CHANNEL7:
			GetLong(IDC_PLE_CHANNEL7,chnr);
			GetLong(IDC_PLE_TEXTURE7,tnum);
			PreviewChannel(7,TRUE,chnr,tnum);
		break;

		case IDC_PLE_TEXTURE8: case IDC_PLE_CHANNEL8:
			GetLong(IDC_PLE_CHANNEL8,chnr);
			GetLong(IDC_PLE_TEXTURE8,tnum);
			PreviewChannel(8,TRUE,chnr,tnum);
		break;
		*/

		//EDIT 1
		case IDC_PLE_EDIT1:
			GetLong(IDC_PLE_CHANNEL1,chnr);
			GetLong(IDC_PLE_TEXTURE1,tnum);
			EditChannel(1,chnr,tnum);
		break;
		//EDIT 2
		case IDC_PLE_EDIT2:
			GetLong(IDC_PLE_CHANNEL2,chnr);
			GetLong(IDC_PLE_TEXTURE2,tnum);
			EditChannel(2,chnr,tnum);
		break;
		//EDIT 3
		case IDC_PLE_EDIT3:
			GetLong(IDC_PLE_CHANNEL3,chnr);
			GetLong(IDC_PLE_TEXTURE3,tnum);
			EditChannel(3,chnr,tnum);
		break;
		//EDIT 4
		case IDC_PLE_EDIT4:
			GetLong(IDC_PLE_CHANNEL4,chnr);
			GetLong(IDC_PLE_TEXTURE4,tnum);
			EditChannel(4,chnr,tnum);
		break;
		//EDIT 5
		case IDC_PLE_EDIT5:
			GetLong(IDC_PLE_CHANNEL5,chnr);
			GetLong(IDC_PLE_TEXTURE5,tnum);
			EditChannel(5,chnr,tnum);
		break;
		//EDIT 6
		case IDC_PLE_EDIT6:
			GetLong(IDC_PLE_CHANNEL6,chnr);
			GetLong(IDC_PLE_TEXTURE6,tnum);
			EditChannel(6,chnr,tnum);
		break;

		case IDC_OK: 
			//Emitter
			GetString(IDC_PLE_EDN3,data,PLE_EMITTERNAME);
			GetLong(IDC_PLE_N3,data,PLE_NUM_EMITTER);

			//Quele
			GetString(IDC_PLE_EDN4,data,PLE_OBJECTNAME);
			GetLong(IDC_PLE_N4,data,PLE_NUM_OBJECT);

			//rest
			GetLong(IDC_PLE_RND5,data,PLE_SEED);
			GetLong(IDC_PLE_DIR1,data,PLE_DIRECTION_TYP);

			GetReal(IDC_PLE_ED1,data,PLE_SPEED);
			GetReal(IDC_PLE_ED2_X,data,PLE_DISPERSING_X);
			GetReal(IDC_PLE_ED2_Y,data,PLE_DISPERSING_Y);
			GetReal(IDC_PLE_ED2_Z,data,PLE_DISPERSING_Z);

			GetBool(IDC_PLE_VISIBLE,data,PLE_VISIBLE);
			GetBool(IDC_PLE_CHECK1,data,PLE_RANDOM);

			GetLong(IDC_PLE_SPEEDCHAN,data,PLE_SPEEDCHAN);
			GetLong(IDC_PLE_SPEEDFIL,data,PLE_SPEEDFIL);

			GetLong(IDC_PLE_VISICHAN,data,PLE_VISICHAN);
			GetLong(IDC_PLE_VISIFIL,data,PLE_VISIFIL);

			GetLong(IDC_PLE_OFFCHAN,data,PLE_OFFCHAN);
			GetLong(IDC_PLE_OFFFIL,data,PLE_OFFFIL);
			GetReal(IDC_PLE_OFFMAX,data,PLE_OFFMAX);

			//Chanels 
			GetLong(IDC_PLE_CHANNEL1,data,PLE_CHANNEL1);
			GetLong(IDC_PLE_CHANNEL2,data,PLE_CHANNEL2);
			GetLong(IDC_PLE_CHANNEL3,data,PLE_CHANNEL3);
			GetLong(IDC_PLE_CHANNEL4,data,PLE_CHANNEL4);
			GetLong(IDC_PLE_CHANNEL5,data,PLE_CHANNEL5);
			GetLong(IDC_PLE_CHANNEL6,data,PLE_CHANNEL6);
			//GetLong(IDC_PLE_CHANNEL7,data,PLE_CHANNEL7);
			//GetLong(IDC_PLE_CHANNEL8,data,PLE_CHANNEL8);

			GetLong(IDC_PLE_TEXTURE1,data,PLE_TEXTURE1);
			GetLong(IDC_PLE_TEXTURE2,data,PLE_TEXTURE2);
			GetLong(IDC_PLE_TEXTURE3,data,PLE_TEXTURE3);
			GetLong(IDC_PLE_TEXTURE4,data,PLE_TEXTURE4);
			GetLong(IDC_PLE_TEXTURE5,data,PLE_TEXTURE5);
			GetLong(IDC_PLE_TEXTURE6,data,PLE_TEXTURE6);
			//GetLong(IDC_PLE_TEXTURE7,data,PLE_TEXTURE7);
			//GetLong(IDC_PLE_TEXTURE8,data,PLE_TEXTURE8);
			break;	

		//Emitter
		case IDC_PLE_EDN3:
			GetString(IDC_PLE_EDN3,st);
			Enable(IDC_PLE_N3,(st != ""));
			/*
			//TEST TEST TEST TEST SETESTSET
			//doc = plugOb->GetDocument();
			//if (!doc) break;
			st = GetNumName(*data,PLE_EMITTERNAME,PLE_NUM_EMITTER);
			print(st);
			//EmiterOb = (ParticleObject*)doc->SearchObject(st);
			//if (!EmiterOb) break; 
			//if (EmiterOb->GetType()!=Tparticle) break;
			//print("EM");
			//TEST TEST TEST TEST SETESTSET
			*/

			//Enable(IDC_PLE_STATIC2,(st == ""));
			break;

		//Quel
		case IDC_PLE_EDN4:
			GetString(IDC_PLE_EDN4,st);
			Enable(IDC_PLE_N4,(st != ""));
			break;

			//TEXTUR
			/*
		case IDC_PLE_CHA1:
			GetLong(IDC_PLE_CHA1,lo);
			Enable(IDC_PLE_TEX_N1,(lo != 20));
			break;

		case IDC_PLE_CHA2:
			GetLong(IDC_PLE_CHA2,lo);
			Enable(IDC_PLE_TEX_N2,(lo != 20));
			break;

		case IDC_PLE_CHA3:
			GetLong(IDC_PLE_CHA3,lo);
			Enable(IDC_PLE_TEX_N3,(lo != 20));
			break;
			*/

			/*
		case IDC_PLE_CHECK2:
			GetBool(IDC_PLE_CHECK2,bo);
			Enable(IDC_PLE_GR3,bo);
			Enable(IDC_PLE_GR4,bo);
			Enable(IDC_PLE_GR5,bo);
			break;
		*/
		   
			//Random ??????
		case IDC_PLE_BT1:
			//Create Neu SEED
			rnd = gNew(Random);if (!rnd) break;
			rnd->Init(GeGetTimer());
			LONG ran = rnd->Get01()*10000;
			SetLong(IDC_PLE_RND5,ran);
			gDelete(rnd);
			break;	

	}
	return TRUE;
}

//""""""""""""""""""" Offne Dialog """"""""""""""""""""
static Bool EditSettings(PluginObject *op)
{
	BaseContainer data = op->GetData();

	PLEDialog dlg(&data,op);
	if (!dlg.Open()) return FALSE;
	op->SetData(data);
	return TRUE;
}

//""""""""""""""""""" Set Standart Werte """"""""""""""""""""
static Bool AllocSettings(PluginObject *op)
{	
	BaseContainer data;
	//Emitter
	data.SetString(PLE_EMITTERNAME,"");
	data.SetLong(PLE_NUM_EMITTER,0);

	//Quele
	data.SetString(PLE_OBJECTNAME,"");
	data.SetLong(PLE_NUM_OBJECT,0);
	data.SetString(PLE_SEL_OBJECT,"");
	data.SetLong(PLE_SEL_NUM_OBJECT,0);
	//VertexQ
	data.SetString(PLE_VERTEX,"");
	data.SetLong(PLE_VERTEX_NUM,0);

	//rest
	data.SetLong(PLE_SEED,0);
	data.SetLong(PLE_DIRECTION_TYP,0);

	data.SetReal(PLE_SPEED,150.0);
	data.SetReal(PLE_DISPERSING_X,0);
	data.SetReal(PLE_DISPERSING_Y,0);
	data.SetReal(PLE_DISPERSING_Z,0);

	data.SetBool(PLE_VISIBLE,FALSE);
	data.SetBool(PLE_RANDOM,TRUE);

	//PARAMETER
	data.SetLong(PLE_SPEEDCHAN,0);
	data.SetLong(PLE_SPEEDFIL,0);

	data.SetLong(PLE_VISICHAN,0);
	data.SetLong(PLE_VISIFIL,0);

	data.SetLong(PLE_OFFCHAN,0);
	data.SetLong(PLE_OFFFIL,0);
	data.SetReal(PLE_OFFMAX,100.0);

	
	//Chanels 
	data.SetLong(PLE_CHANNEL1,0);
	data.SetLong(PLE_CHANNEL2,0);
	data.SetLong(PLE_CHANNEL3,0);
	data.SetLong(PLE_CHANNEL4,0);
	data.SetLong(PLE_CHANNEL5,0);
	data.SetLong(PLE_CHANNEL6,0);
	data.SetLong(PLE_CHANNEL7,0);
	data.SetLong(PLE_CHANNEL8,0);

	//Texturen
	data.SetLong(PLE_TEXTURE1,0);
	data.SetLong(PLE_TEXTURE2,-1);
	data.SetLong(PLE_TEXTURE3,-1);
	data.SetLong(PLE_TEXTURE4,-1);
	data.SetLong(PLE_TEXTURE5,-1);
	data.SetLong(PLE_TEXTURE6,-1);
	data.SetLong(PLE_TEXTURE7,-1);
	data.SetLong(PLE_TEXTURE8,-1);
	//Textur
	/*
	data.SetLong(PLE_SPEEDCHAN,20);
	data.SetLong(PLE_SPEEDTEX,0);

	data.SetLong(PLE_VISICHAN,20);
	data.SetLong(PLE_VISITEX,0);

	data.SetLong(PLE_ANGLECHAN,20);
	data.SetLong(PLE_ANGLETEX,0);
	*/

	op->SetData(data);
	return TRUE;
}

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
static LONG GetDescription(PluginObject *op, LONG id)
{
	switch (id)
	{
		//Emitter
		case PLE_EMITTERNAME	: return PARAM_TOGGLE;
		case PLE_NUM_EMITTER	: return PARAM_LINEAR;

		//Quele
		case PLE_OBJECTNAME		: return PARAM_TOGGLE;
		case PLE_NUM_OBJECT		: return PARAM_LINEAR;
		case PLE_SEL_OBJECT		: return PARAM_TOGGLE;
		case PLE_SEL_NUM_OBJECT : return PARAM_LINEAR;
		//VertexQ
		case PLE_VERTEX			: return PARAM_TOGGLE;
		case PLE_VERTEX_NUM		: return PARAM_LINEAR;

		//rest
		case PLE_SEED			: return PARAM_LINEAR;
		case PLE_DIRECTION_TYP	: return PARAM_TOGGLE;

		case PLE_SPEED			: return PARAM_SCALE; 
		case PLE_DISPERSING_X	: return PARAM_LINEAR; 
		case PLE_DISPERSING_Y	: return PARAM_LINEAR;
		case PLE_DISPERSING_Z	: return PARAM_LINEAR;

		case PLE_VISIBLE		: return PARAM_TOGGLE;
		case PLE_RANDOM			: return PARAM_TOGGLE;

		case PLE_OFFMAX			: return PARAM_LINEAR;

		//INTERN
		case PLE_DRND			: return PARAM_SOLID;
		case PLE_REVAP			: return PARAM_SOLID;
		case PLE_VER			: return PARAM_SOLID;
	}
	return PARAM_TOGGLE;
}

//#############################  PLUGIN ###########################################

//<<<<<<<<<<<<<<<<<<<<<<<< DRAW >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
static Bool Draw(PluginObject *op, BaseDraw *bd, BaseDrawHelp *bh)
{

	BaseContainer data = op->GetDataInstance();

	String  EmName = data.GetString(PLE_EMITTERNAME);
	String  SourceName = data.GetString(PLE_OBJECTNAME);

	BaseDocument  *doc = op->GetDocument();
	if (!doc) return TRUE;

	bd->SetPen(bd->GetObjectColor(op));
	Matrix mg=bh->GetMg();

	//Find Emitter
	BaseObject *EmiterOb = doc->SearchObject(EmName);//Emiter zum Modifizieren
	if (!EmiterOb)
	{
		EmiterOb = op->GetDown();
		if (!EmiterOb) return TRUE;
	}	
	Matrix emg=EmiterOb->GetMg();
	bd->Line3D(mg.off,emg.off);

	//Find Points 
	BaseObject *SourceOb = doc->SearchObject(SourceName);
    if (!SourceOb) 
	{
		SourceOb = op->GetUp();
		if (!SourceOb) return TRUE;
	}
	Matrix pmg=SourceOb->GetMg();
	bd->Line3D(emg.off,pmg.off);


	/*
	//Find Destination
	BaseObject *DestOb = doc->SearchObject(DestName);
	if ((!DestOb) || DestOb == SourceOb)
	{
		DestOb = (PointObject*)SourceOb->up();
		if (!DestOb) return TRUE;
	}
    if (!DestOb) return TRUE;
	Matrix DestMG=DestOb->GetMg();//Global Matrix of Dest
	bd->Line3D(pmg.off,DestMG.off);
	*/

	return TRUE;
}




//####################################################################################
//####################################################################################
//####################################################################################

//"""""""""""""""""""""""""""""""""""""""""""""""""
static Vector RndVec(Random *rnd,const Vector disp)//OK
{
	Vector vec;
	vec.x = rnd->Get11()*disp.x;
	vec.y = rnd->Get11()*disp.y;
	vec.z = rnd->Get11()*disp.z;
	return vec;
}


//##########################################################################################
//##########################################################################################
//##########################################################################################
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  ModifyParticles  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
static void ModifyParticles(PluginObject *op, Particle *ps, BaseParticle *ss, LONG pcnt, Real diff)
{
    if (!op->GetDeformMode()) return;//Return wenn Object deaktiwiert ist

	BaseContainer data = op->GetData();//GET SETING

	//---------------------- VAR -----------------------------------
	BaseDocument	*doc = op->GetDocument();if (!doc) {return;}
	LONG			fps = doc->GetFps();//Get Doc FPS

	//EMITTER
    ParticleObject	*emOb = NULL;//Emiter zum Modifizieren
	Particle		*emParticle = NULL;
	ParticleTag		*emPTag = NULL;

	//SOURCE
    BaseObject		*tempOb= NULL;//SOURCE Temporale Object
	PolygonObject	*srcOb = NULL;//SOURCE Emiter Start Point Object

	LONG			srcPCnt = 0;//SOURCE POINT COUNT
	Vector			*srcPoints = NULL;//SOURCE POINTS ARRAY
	LONG			srcPolyCnt = 0;//SOURCE POLYGON COUNT
	CPolygon			*srcPolys = NULL;//SOURCE POLYGONS

	Real			cSpeed = data.GetReal(PLE_SPEED);
	Real			speed = cSpeed;

	Vector			dispersing;
	dispersing.x = data.GetReal(PLE_DISPERSING_X);
	dispersing.y = data.GetReal(PLE_DISPERSING_Y);
	dispersing.z = data.GetReal(PLE_DISPERSING_Z);

	Bool			dispUse = ((dispersing.x+dispersing.y+dispersing.z)>0);
	Bool			pointRandom = data.GetBool(PLE_RANDOM);
	Bool			makevisible = data.GetBool(PLE_VISIBLE);
	LONG			directionTyp = data.GetLong(PLE_DIRECTION_TYP);

	//CHANELLS
	LONG			channel[MAX_CHANELL];
	channel[0] = 0;
	channel[1] = data.GetLong(PLE_CHANNEL1);
	channel[2] = data.GetLong(PLE_CHANNEL2);
	channel[3] = data.GetLong(PLE_CHANNEL3);
	channel[4] = data.GetLong(PLE_CHANNEL4);
	channel[5] = data.GetLong(PLE_CHANNEL5);
	channel[6] = data.GetLong(PLE_CHANNEL6);
	//channel[7] = data.GetLong(PLE_CHANNEL7);
	//channel[8] = data.GetLong(PLE_CHANNEL8);
	channel[7] = 20;
	channel[8] = 20;
	//TEXTURES
	LONG			texture[MAX_CHANELL];
	texture[0] = 0;
	texture[1] = data.GetLong(PLE_TEXTURE1);
	texture[2] = data.GetLong(PLE_TEXTURE2);
	texture[3] = data.GetLong(PLE_TEXTURE3);
	texture[4] = data.GetLong(PLE_TEXTURE4);
	texture[5] = data.GetLong(PLE_TEXTURE5);
	texture[6] = data.GetLong(PLE_TEXTURE6);
	//texture[7] = data.GetLong(PLE_TEXTURE7);
	//texture[8] = data.GetLong(PLE_TEXTURE8);
	texture[7] = -1;
	texture[8] = -1;

	
	//SPEED CHANEL
	LONG			speedChannel = data.GetLong(PLE_SPEEDCHAN) % MAX_CHANELL;
	LONG			speedFilter = data.GetLong(PLE_SPEEDFIL);
	Bool			useSpeedChannel = speedChannel > 0;

	//VISIBLITY CHANEL
	LONG			visiChannel = data.GetLong(PLE_VISICHAN) % MAX_CHANELL;
	LONG			visiFilter = data.GetLong(PLE_VISIFIL);
	Bool			useVisiChannel = visiChannel > 0;

	//OFFSET CHANEL
	LONG			offsetChannel = data.GetLong(PLE_OFFCHAN) % MAX_CHANELL;
	LONG			offsetFilter = data.GetLong(PLE_OFFFIL);
	Real			offsetMax = data.GetReal(PLE_OFFMAX);
	Bool			useOffsetChannel = offsetChannel > 0;

	//ANGLE CHANEL
	//LONG			angleChannel = data.GetLong(PLE_ANGLECHAN) % MAX_CHANELL;
	//LONG			angleFilter = data.GetLong(PLE_ANGLEFIL);
	Bool			useAngleChannel = FALSE;

	//USE TEXTUR
	Bool			useTextur = TRUE;

	//INTERN
	LONG			revaP = data.GetLong(PLE_REVAP);
	LONG			drnd = data.GetLong(PLE_DRND);//RANDOM 
	LONG			rndinit = data.GetLong(PLE_SEED);//RANDOM

	//-------------------------- NAMEN ----------------------------
	String  emName = GetNumName(data,PLE_EMITTERNAME,PLE_NUM_EMITTER);//EMITTER NAME
    String  sourceName = GetNumName(data,PLE_OBJECTNAME,PLE_NUM_OBJECT);//POLYGON SOURCE NAME
	String  selectSource = GetNumName(data,PLE_SEL_OBJECT,PLE_SEL_NUM_OBJECT);//SOURCEC SELECTION

	//------------------------- FIND OBJEKTE --------------------------------
	//EMITTER Search
    emOb = (ParticleObject*)doc->SearchObject(emName);
	if (!emOb){ emOb = (ParticleObject*)op->GetDown();}
	if (!emOb) {print("PLE: Emitter "+emName+" not found"); return;}
	//if (EmiterOb->GetType()!=Oparticle) {GePrint("PLE:"+EmiterOb->GetName()+" is not an emitter");return;}
	
	//EMITTER Particle Tag
	emPTag = (ParticleTag*)emOb->GetTag(Tparticle);if (!emPTag) return;
	emParticle = emOb->GetParticle(emPTag,0);if (emParticle != ps) return;//Ob die particles EmiterOb gehören

	//SOURCE Search
    tempOb = doc->SearchObject(sourceName);
    if (!tempOb){
		tempOb = op->GetUp();
		if (!tempOb){print("PLE: Source "+sourceName+" not found"); return;}
		if (tempOb->GetType() == Oparticle) {print("PLE: isn't an Emitter"); return;}
	}
    //if ((tempOb->GetType()!=Opolygon) && (tempOb->GetType()!=Oprimitive)) {print("PLE: "+tempOb->GetName()+" is not Polygon Ob."); return;}

	//--------------------------  Vorbereitung  ----------------------------------
	LONG	c = 0;  //Lauf Variable
	LONG	pn = 0;  //Polygon NUMMER
	LONG    s = 0; //Sampler Variable
	Vector  polyNormal;
	Vector  direct;
	Vector  lokStartPnt;
	Vector	globStartPnt;
	Weigh	w;
	BaseContainer	nullbc;nullbc.SetId(0);
	Bool    modcom = FALSE;
	//Bool	prinmitiv;

	//Random Number(1) Init
	Random *rnd1 = gNew(Random);//SD
	rnd1->Init(rndinit);//SD
	//????   http://farside.ph.utexas.edu/teaching/329/lectures/node98.html

	//Random Number(2) Init
	Random *rnd2 = gNew(Random);
	if (drnd == 0) drnd = rnd1->Get01()*10000; //SD
	rnd2->Init(drnd);//SD

	//KAMARE DIRECTION ??? (NICT WIRKLICH NUTZLICH???)
	BaseObject *cp = NULL;
	Vector     kampos;
	if (directionTyp==3){
		BaseDraw   *bd = doc->GetRenderBaseDraw(); 
		if (bd){
			cp = bd->GetSceneCamera(doc); 
			if (!cp) cp=bd->GetEditorCamera();
		}
		if (cp) {
			kampos = cp->GetPos();
		}else{
			directionTyp = 0;
		}
	}


	Vector bump1 = 0.0;
	Real	high = 0.0;

	//--- TEXTURS --- CHANELS ---
	UVWTag		*uvwtag = NULL;
	LONG		uvwcnt = 0;
	UVWStruct	uvw;
	Vector		puv;
	TextureTag	*textag = NULL;
	Vector		color;//TEMPORAL ???
	Matrix		bump;

	VdSampler	*sampler[MAX_CHANELL]; for (s=0; s<MAX_CHANELL; s++) sampler[s]=NULL;
	//ChSampler	*sampler[MAX_CHANELL]; for (s=0; s<MAX_CHANELL; s++) sampler[s]=NULL;
	Vector		colSam[MAX_CHANELL]; for (s=0; s<MAX_CHANELL; s++) colSam[s]=1.0;
	Bool		useSam[MAX_CHANELL]; //for (s=0; s<MAX_CHANELL; s++) useSam[s]=FALSE;

	/*
	if (useTextur){
		//SAMPLER INIT OPTIMIEREN ???
		for (s=1; s<MAX_CHANELL; s++){
			useSam[s]=FALSE;
			if ((texture[s]<0) || (channel[s] >= 20)) continue;
			textag = (TextureTag*)tempOb->GetTag(Ttexture,texture[s]);if (!textag) continue;
			sampler[s] = gNew ChSampler(doc,textag,channel[s]);
			if (sampler[s]) useSam[s]=TRUE;//Sample Kann Benutzt Werden
		}
	}
	*/
	//-----------------
	if (useTextur){
		//SAMPLER INIT OPTIMIEREN ???
		for (s=1; s<MAX_CHANELL; s++){
			useSam[s]=FALSE;
			if ((texture[s]<0) || (channel[s] >= 20)) continue;
			textag = (TextureTag*)tempOb->GetTag(Ttexture,texture[s]);if (!textag) continue;
			sampler[s] = gNew(VdSampler);
			if (sampler[s]){
				if (sampler[s]->Init(doc,tempOb,tempOb->GetMg(),textag,channel[s])!=0) {gDelete(sampler[s]); }
				useSam[s]=TRUE;//Sample Kann Benutzt Werden
			}
		}
	}

	//<<< SOURCE  MATRIX >>>
	Matrix  sourceML  = tempOb->GetMl(), isourceML=!sourceML;
	Matrix	sourceMG  = tempOb->GetMg();
	Matrix  sourceMGU = tempOb->GetUpMg();
	Vector	sourcePos = tempOb->GetPos()*sourceMGU;
//...................................WORKING.....................................
	//--------------------------------------------
		//--------------------- NEW SYSTEM 4 ----------------
		BaseObject *def = NULL;
		srcOb = ToPoly(tempOb->GetCache());
		if (srcOb) def = srcOb->GetDeformCache();
		else def = tempOb->GetDeformCache();
		if (def) srcOb = ToPoly(def);
		if (!srcOb && (tempOb->GetLayerType()&L_pointobject)) srcOb = ToPoly(tempOb);
		if (!srcOb) { //?????????????????
			print("TDEM:  build cache");//DEBUG
			srcOb = ToPoly(SendModelingCommand(MCOMMAND_CURRENTSTATETOOBJECT,doc,tempOb,&nullbc,MODIFY_ALL));
			modcom = TRUE;
		}
		if (!srcOb) goto Error;	
		if (!(srcOb->GetLayerType()&L_pointobject)) goto Error;	
		//-------------------------------------------------------

	//--- TEXTURS --- CHANELS ---
	if (useTextur){//UVW 
		uvwtag =  (UVWTag*)srcOb->GetTag(Tuvw); 
		if (uvwtag) uvwcnt = uvwtag->GetDataCount();
		else useTextur = FALSE;//No Uvw Tag
	}//if (useTextur)

	//SOURCE POINTS
	srcPoints = srcOb->GetPoint(); if (!srcPoints) goto Error;
	srcPCnt = srcOb->GetPointCount();
	//SOURCE POLYGONS
	srcPolys = srcOb->GetPolygon(); if (!srcPolys) goto Error;
	srcPolyCnt = srcOb->GetPolygonCount();



	//*******************************************************
	for (c=0; c<pcnt; c++){
		if (!(ps[c].bits & PARTICLE_ALIVE)) continue;

			//RANDOM POINT ????
			if (pointRandom){
				if (pcnt > srcPolyCnt){	
					revaP = rnd1->Get01()*pcnt;//Generate Random Point nach Particle
				}else{	
					revaP = rnd1->Get01()*srcPolyCnt;//Generate Random Point nach Points 
				}
			}else{
				revaP = c;
			}//if (pointRandom){


		if (ps[c].t<=diff){//GEBURT ANFANG
			if (makevisible) ps[c].bits = ps[c].bits | PARTICLE_VISIBLE;//MAKE VISIBLE
			//http://olli.informatik.uni-oldenburg.de/Grafiti3/grafiti/flow10/page5.html#Ref_ID239
			pn = revaP % srcPolyCnt;
			if (srcPolys[pn].c == srcPolys[pn].d){//Dreieck
				w.rand3(rnd2);
				lokStartPnt = srcPoints[srcPolys[pn].a]*w.w0+srcPoints[srcPolys[pn].b]*w.w1+srcPoints[srcPolys[pn].c]*w.w2;
			}else{//Viereck
				w.rand4(rnd2);
				lokStartPnt = srcPoints[srcPolys[pn].a]*w.w0+srcPoints[srcPolys[pn].b]*w.w1+srcPoints[srcPolys[pn].c]*w.w2+srcPoints[srcPolys[pn].d]*w.w3;	
			}
			polyNormal = CalcFaceNormal(srcPoints,srcPolys[pn]);

			//----- TEXTURS ----- CHANELS -----
			if (useTextur){//http://www.cebas.de/de/particle_dist_.htm
				uvw = uvwtag->Get(pn);
				puv = uvw.a*w.w0 + uvw.b*w.w1 + uvw.c*w.w2 + uvw.d*w.w3;//TEX SAMPLER ??????

				//SAMPLE ALL SAMPLER LANGSAMM ???? !!!!
				for (s=1; s<MAX_CHANELL; s++){
					if (useSam[s]) colSam[s]=sampler[s]->Sample3D(lokStartPnt,puv);
				}

				//SPEED CHANEL
				if (useSpeedChannel){speed = cSpeed * ColorFilter(colSam[speedChannel],speedFilter);}

				//VISIBLITY CHANNEL
				if (useVisiChannel){
					if (ColorFilter(colSam[visiChannel],visiFilter)<0.5){ 
						ps[c].bits = ps[c].bits & ~PARTICLE_VISIBLE;
					}else{
						ps[c].bits = ps[c].bits | PARTICLE_VISIBLE;//Sichtbar Machen????
					}

				}

				if (useOffsetChannel){
					lokStartPnt += ColorFilter(colSam[offsetChannel],offsetFilter)*vnorm(polyNormal)*offsetMax;
				}
				
				//ANGLE CHANEL
				if (useAngleChannel){//????? DONT WORK
					/*
					//http://www14.in.tum.de/lehre/1998WS/proseminar/krause/#2.%20Bump%20Mapping
					//http://www.cs.kuleuven.ac.be/~graphics/H331.0102/PRACT3/cool.html
					color  = sampler[3]->Sample(puv);
					color1 = sampler[3]->Sample(puv+Vector(0.1,0.0,0.0));//u
					color2 = sampler[3]->Sample(puv+Vector(0.0,0.1,0.0));//v

					Real u = VectorGray(color)-VectorGray(color1);
					Real v = VectorGray(color)-VectorGray(color2);
					//bump1 =	Vector(u,1.0,0.0) * Vector(1.0,v,0.0);

					//VectorToHPB
					//bump1 = !bump1 * (VectorGray(color)*10);
					//bump1 = GetHPB(Vector(u,1.0,0.0),Vector(1.0,v,0.0),Vector(0.0,0.0,0.0));
					//bump = HPBToMatrix(VectorToHPB(bump1));
					//bump = bump * MatrixRotX(u*pi) * MatrixRotZ(v*pi);
					*/
				}
			}//if (useTextur)

			globStartPnt = lokStartPnt * sourceMG;

			//Set Start Position
			ps[c].off = globStartPnt;

			//DIRECTION
			switch(directionTyp){//RICHTUNG
				case 0://Normal
					//direct = CalcFaceNormal(srcPoints,srcPolys[pn]);
					direct = polyNormal;
					ss[c].v +=  vnorm(direct)*speed;//Set Velocity
					ss[c].count++;
				break;
				case 1://Radial
					direct = lokStartPnt;
					ss[c].v +=  vnorm(direct)*speed;//Set Velocity
					ss[c].count++;
				break;
				case 2://Emitter Richtung
				break;
				case 3://Kamera Richtung
					direct = kampos-globStartPnt;
					ss[c].v +=  vnorm(direct)*speed;//Set Velocity
					ss[c].count++;
				break;

				default://Normal
					direct = CalcFaceNormal(srcPoints,srcPolys[pn]);
					ss[c].v +=  vnorm(direct)*speed;//Set Velocity
					ss[c].count++;
			}

		}//	if (ps[c].t<=diff){	
		
	}// for (c=0; c<pcnt; c++){
	//*******************************************************



//--------------SET DATA------------------
drnd = rnd2->Get01()*10000;//RND2 TEST
data.SetLong(PLE_DRND,drnd);
data.SetLong(PLE_REVAP,revaP);
op->SetData(data);


//--------------ERROR--------------------
Error:

if (modcom) {blDelete(srcOb);}

for (c=0; c<MAX_CHANELL; c++) gDelete(sampler[c]);

gDelete(rnd1);
gDelete(rnd2);

}//Mod Partikel



//################################### MenuPrepare ###################################
static Bool MenuPrepare(PluginObject *op, BaseDocument *doc)
{
	op->SetDeformMode(TRUE);

	BaseObject *child = NULL;	
	BaseContainer res;
	//Insert Active Object when Child of PluginObject
	if (GetInputState(BFM_INPUT_KEYBOARD,KEY_CONTROL,res) && res.GetLong(BFM_INPUT_VALUE)){
		child = doc->GetActiveObject();
		if (child && child->GetType()==Oparticle){
			child->Remove();
			child->InsertUnder(op);
		}

	}
	return TRUE;
}


//VAR
static BaseBitmap *icon=NULL;
static BaseBitmap *icon_small=NULL;


//################################ RegisterPOLYEM ###############################
Bool RegisterPOLYEM(void)
{
	// decide by name if the plugin shall be registered - just for user convenience
	String name=GeLoadString(IDS_POLYEM); if (!name.Content()) {GePrint("POLYEM: name return!");return TRUE;}

	icon = AllocBaseBitmap();
	if (!icon || icon->Init(GeGetPluginPath()+String("res")+String("polyem.tif"))!=IMAGE_OK) {GePrint("PLE: polyem.tif is not loaded!");return FALSE;}
	icon_small = AllocBaseBitmap();
	if (!icon_small || icon_small->Init(GeGetPluginPath()+String("res")+String("polyem_smal.tif"))!=IMAGE_OK) {GePrint("POLYEM: polyem_smal.tif is not loaded!");return FALSE;}

	OBJECTPLUGIN cs;
	ClearMem(&cs,sizeof(cs));

	cs.add_menu			= TRUE;
	cs.icon				= icon;
	cs.icon_small		= icon_small;
	cs.info				= 0;
	cs.MenuPrepare		= MenuPrepare;
	cs.ModifyParticles	= ModifyParticles;
	cs.AllocSettings	= AllocSettings;
	cs.EditSettings		= EditSettings;
	cs.Draw				= Draw;
	//cs.DrawBox		= Draw; // draw modifier in box mode also
	//cs.MoveHandle		= MoveHandle;
	
	// build description for parameter animation
	cs.GetDescription	= GetDescription;

	// be sure to use a unique ID obtained from www.plugincafe.com
	return GeRegisterPlugin(C4DPL_OBJECT,POLYEM_ID,name,&cs,sizeof(cs));
}

//########################### FreePOLYEM ##############################
void FreePOLYEM(void)
{
	FreeBaseBitmap(icon);
	FreeBaseBitmap(icon_small);
}



