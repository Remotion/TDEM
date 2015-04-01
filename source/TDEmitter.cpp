/////////////////////////////////////////////////////////////
// 3d Emiter											   //
/////////////////////////////////////////////////////////////
// VERSION  1.0  										   //
/////////////////////////////////////////////////////////////
// (c) 2002 Remotion, all rights reserved				   //
/////////////////////////////////////////////////////////////

/*
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

*/
#include "c4d.h"
#include "c4d_symbols.h"


#include "mytools.h"
#include "myprint.h"

//--------------------------------------
#define PLUGINS_ID			1008034 //TDEM
//--------------------------------------


#define MAX_OVERSAMPLING	64


//Glob Var_
#define TDEM_VER			2000
#define TDEM_REVAP			2001
#define TDEM_DRND			2002
//_Glob Var


//------------------------------------
//Emitter
#define TDEM_EMITTERNAME	1011
#define TDEM_NUM_EMITTER	1012

//Quele
#define TDEM_OBJECTNAME		1021
#define TDEM_NUM_OBJECT		1022
#define TDEM_SEL_OBJECT		1023
#define TDEM_SEL_NUM_OBJECT	1024
//Q Vertex
#define TDEM_VERTEX			1031
#define TDEM_VERTEX_NUM		1032

//Ziel
#define TDEM_DESTLOCROT		1040
#define TDEM_USEDEST		1041
#define TDEM_LOCALDES		1042
#define TDEM_DESTINATION	1043
#define TDEM_NUM_DESTIN		1044
#define TDEM_SEL_DESTIN		1045
#define TDEM_SEL_NUM_DESTIN	1046
#define TDEM_USESPCOR		1047
#define TDEM_DESTALWAYS		1048
#define TDEM_DEST_TYP		1049
#define TDEM_DEST_SPEED		1050

//Pfad
#define TDEM_USEPATH        1051
#define TDEM_LOCALPATH		1052
#define TDEM_PATH		    1053
#define TDEM_PATH_NUM	    1054
#define TDEM_PATH_SPEED	    1055
#define TDEM_PATH_TYP	    1056
#define TDEM_PATHLOCROT	    1057

//rest
#define TDEM_DISPERSING_X	1061
#define TDEM_DISPERSING_Y	1062
#define TDEM_DISPERSING_Z	1063
#define TDEM_DIRECTION_TYP	1064
#define TDEM_RANDOM			1065
#define TDEM_SPEED			1066
#define TDEM_SEED			1067


#define TDEM_SPEEDCHAN		1071 //IDC_TDM_CHA1
#define TDEM_SPEEDTEX		1072 //IDC_TDM_TEX_N1

#define TDEM_PPCOLL			1081 //BOOL
#define TDEM_PPOVERSAM		1082 //LONG
#define TDEM_PPRADIUS		1083 //REAL

#define TDEM_GRAVITI		1091 //VECTOR

//----------------------------------

/*
//############################### CREATE EMITTER ##################################
ParticleObject *CreateEmitter(BaseDocument *doc,BaseObject *parent)
{
	ParticleObject *em = NULL;
	BaseContainer bc;

	doc->StartUndo();

	em = (ParticleObject*)AllocObject(Oparticle);if (!em) return NULL;

	bc = em->GetData();
	bc.SetLong(PARTICLEOBJECT_BIRTHEDITOR,100);
	bc.SetLong(PARTICLEOBJECT_BIRTHRAYTRACER,100);
	bc.SetReal(PARTICLEOBJECT_SPEED,0.0);
	bc.SetReal(PARTICLEOBJECT_SPEEDVAR,0.0);
	//bc.SetReal(PARTICLEOBJECT_VISIBILITY,0.0);
	bc.SetReal(PARTICLEOBJECT_SIZEX,0.0);
	bc.SetReal(PARTICLEOBJECT_SIZEY,0.0);
	em->SetData(bc);

	doc->InsertObject(em,NULL,NULL);

	doc->AddUndo(UNDO_OBJECT_NEW,em);
	//em->Message(MSG_UPDATE);
	//em->InsertUnder(parent);

	//tt = (ParticleObject*)em->GetClone();
	//tt->InsertUnder(parent);

	doc->EndUndo();

	GeEventAdd(MSG_DOCUMENTCHANGED);

	return em;
}
*/

#if 0
//#############################  MADAL DIALOG ###########################################
//"""""""""""""""""""""""""""""""""""""""
class TDMDialog : public GeModalDialog
{
	private:
		BaseContainer *data;
		PluginObject *plugOb;
		//BaseContainer *EMdata;//TEST ????
	public:
		TDMDialog(BaseContainer *bc, PluginObject *op) { data = bc;  plugOb = op; }

		virtual Bool CreateLayout(void);
		virtual Bool InitValues(void);
		virtual Bool Command(LONG id, const BaseContainer &msg);
};

//"""""""""""""""""""""""""""""""""""""""
Bool TDMDialog::CreateLayout(void)
{
	return GeModalDialog::CreateLayout() && LoadDialogResource(DLG_TDEM,NULL,0);
}

//"""""""""""""""""""""""""""""""""""""""
Bool TDMDialog::InitValues(void)
{
	Bool bo;
	LONG lo;
	String st = "";

	// first call the parent instance
	if (!GeModalDialog::InitValues()) return FALSE;

	//Emitter
    SetString(IDC_TDM_EDN3,data,TDEM_EMITTERNAME);
	SetLong(IDC_TDM_N3,data,TDEM_NUM_EMITTER,0,1000,1);

	//Quele
    SetString(IDC_TDM_EDN4,data,TDEM_OBJECTNAME);
	SetLong(IDC_TDM_N4,data,TDEM_NUM_OBJECT,0,1000,1);
    SetString(IDC_TDM_SEL4,data,TDEM_SEL_OBJECT);
	SetLong(IDC_TDM_SEL_N4,data,TDEM_SEL_NUM_OBJECT,0,1000,1);
	//Vertex
	SetString(IDC_TDM_VEX4,data,TDEM_VERTEX);
	SetLong(IDC_TDM_VEX_N4,data,TDEM_VERTEX_NUM,0,1000,1);

	//Ziel
	SetBool(IDC_TDM_CHR2,data,TDEM_DESTLOCROT);		
	SetBool(IDC_TDM_CHECK3,data,TDEM_USEDEST);
	SetString(IDC_TDM_EDN5,data,TDEM_DESTINATION);
	SetLong(IDC_TDM_N5,data,TDEM_NUM_DESTIN,0,1000,1);
	SetString(IDC_TDM_SEL5,data,TDEM_SEL_DESTIN);
	SetLong(IDC_TDM_SEL_N5,data,TDEM_SEL_NUM_DESTIN,0,1000,1);
	SetLong(IDC_TDM_DD_OM5,data,TDEM_DEST_TYP);
	SetMeter(IDC_TDM_NN5,data,TDEM_DEST_SPEED,-MAXRANGE,MAXRANGE,1.0);

	//Pfad
	SetBool(IDC_TDM_PT_CB4,data,TDEM_USEPATH);
	SetString(IDC_TDM_PT4,data,TDEM_PATH);
	SetLong(IDC_TDM_PT_N4,data,TDEM_PATH_NUM,0,1000,1);
	SetBool(IDC_TDM_AT_CB4,data,TDEM_LOCALPATH);
	SetLong(IDC_TDM_DD_OM4,data,TDEM_PATH_TYP);
	SetMeter(IDC_TDM_NN4,data,TDEM_PATH_SPEED,-MAXRANGE,MAXRANGE,1.0);
	SetBool(IDC_TDM_AT_FF4,data,TDEM_PATHLOCROT);


	//rest
	SetLong(IDC_TDM_RND5,data,TDEM_SEED,0,10000,1);
	SetLong(IDC_TDM_DIR1,data,TDEM_DIRECTION_TYP);

	SetMeter(IDC_TDM_ED1,data,TDEM_SPEED,-MAXRANGE,MAXRANGE,1.0);
	SetPercent(IDC_TDM_ED2_X,data,TDEM_DISPERSING_X,0.0,5000.0,1.0);
	SetPercent(IDC_TDM_ED2_Y,data,TDEM_DISPERSING_Y,0.0,5000.0,1.0);
	SetPercent(IDC_TDM_ED2_Z,data,TDEM_DISPERSING_Z,0.0,5000.0,1.0);

	SetBool(IDC_TDM_CHECK1,data,TDEM_RANDOM);
	SetBool(IDC_TDM_CHS1,data,TDEM_USESPCOR);
	SetBool(IDC_TDM_CHD2,data,TDEM_LOCALDES);
	SetBool(IDC_TDM_CHS2,data,TDEM_DESTALWAYS);
	//Textur
	SetLong(IDC_TDM_CHA1,data,TDEM_SPEEDCHAN);
	SetLong(IDC_TDM_TEX_N1,data,TDEM_SPEEDTEX,0,100,1);
	//PPCOll
	SetBool(IDC_TDM_PPCOLL,data,TDEM_PPCOLL);
	SetLong(IDC_TDM_PPOVERSAM,data,TDEM_PPOVERSAM,1,MAX_OVERSAMPLING,1);
	SetMeter(IDC_TDM_PPRADIUS,data,TDEM_PPRADIUS,-MAXRANGE,MAXRANGE,1.0);

	//Gravitation
	SetVectorReal(IDC_TDM_GRV1_X,IDC_TDM_GRV1_Y,IDC_TDM_GRV1_Z,data,TDEM_GRAVITI,-MAXRANGE,MAXRANGE,1.0);

	//-------------------------
	//Emitter
	GetString(IDC_TDM_EDN3,st);
	Enable(IDC_TDM_N3,(st != ""));
	//Enable(IDC_TDM_STATIC2,(st == ""));

	//Quele
	GetString(IDC_TDM_EDN4,st);
	Enable(IDC_TDM_N4,(st != ""));
	GetString(IDC_TDM_SEL4,st);
	Enable(IDC_TDM_SEL_N4,(st != ""));
	GetString(IDC_TDM_VEX4,st);
	Enable(IDC_TDM_VEX_N4,(st != ""));

	//Ziel
	GetBool(IDC_TDM_CHD2,bo);
	Enable(IDC_TDM_CHR2,bo);

	GetBool(IDC_TDM_CHECK3,bo);
	Enable(IDC_TDM_EDN5,bo);
	Enable(IDC_TDM_SEL5,bo);
	Enable(IDC_TDM_GRL213,bo);


	GetString(IDC_TDM_EDN5,st);
	Enable(IDC_TDM_N5,((st != "") && bo));

	GetString(IDC_TDM_SEL5,st);
	Enable(IDC_TDM_SEL_N5,((st != "") && bo));


	//WEG
	GetBool(IDC_TDM_AT_CB4,bo);
	Enable(IDC_TDM_AT_FF4,bo);

	GetBool(IDC_TDM_PT_CB4,bo);
	Enable(IDC_TDM_PT4,bo);
	Enable(IDC_TDM_GRL232,bo);


	GetString(IDC_TDM_PT4,st);
	Enable(IDC_TDM_PT_N4,((st != "") && bo));

	//TEXTUR
	GetLong(IDC_TDM_CHA1,lo);
	Enable(IDC_TDM_TEX_N1,(lo != 20));
	//rest
	//GetBool(IDC_TDM_CHECK2,bo);
	//Enable(IDC_TDM_GR3,bo);
	//Enable(IDC_TDM_GR4,bo);
	//Enable(IDC_TDM_GR5,bo);

	//PPCOll
	GetBool(IDC_TDM_PPCOLL,bo);
	Enable(IDC_TDM_PPOVERSAM,bo);  //Collision Oversampling
	Enable(IDC_TDM_PPRADIUS,bo); //Particle Radius

	Enable(IDC_TDM_GRL1,TRUE);//???

	return TRUE;
}

//""""""""""""""""""""" On Cange """""""""""""""""""""""""
Bool TDMDialog::Command(LONG id, const BaseContainer &msg)
{
	Bool bo;
	LONG lo;
	String st = "";
	Random *rnd = NULL;
	//BaseDocument *doc = NULL;
	//ParticleObject *EmiterOb = NULL; 

	switch (id)
	{
		case IDC_OK: 
			//Emitter
			GetString(IDC_TDM_EDN3,data,TDEM_EMITTERNAME);
			GetLong(IDC_TDM_N3,data,TDEM_NUM_EMITTER);

			//Quele
			GetString(IDC_TDM_EDN4,data,TDEM_OBJECTNAME);
			GetLong(IDC_TDM_N4,data,TDEM_NUM_OBJECT);
			GetString(IDC_TDM_SEL4,data,TDEM_SEL_OBJECT);
			GetLong(IDC_TDM_SEL_N4,data,TDEM_SEL_NUM_OBJECT);
			//Vertex
			GetString(IDC_TDM_VEX4,data,TDEM_VERTEX);
			GetLong(IDC_TDM_VEX_N4,data,TDEM_VERTEX_NUM);

			//Ziel
			GetBool(IDC_TDM_CHR2,data,TDEM_DESTLOCROT);
			GetBool(IDC_TDM_CHECK3,data,TDEM_USEDEST);
			GetString(IDC_TDM_EDN5,data,TDEM_DESTINATION);
			GetLong(IDC_TDM_N5,data,TDEM_NUM_DESTIN);
			GetString(IDC_TDM_SEL5,data,TDEM_SEL_DESTIN);
			GetLong(IDC_TDM_SEL_N5,data,TDEM_SEL_NUM_DESTIN);
			GetLong(IDC_TDM_DD_OM5,data,TDEM_DEST_TYP);
			GetReal(IDC_TDM_NN5,data,TDEM_DEST_SPEED);

			//Weg
			GetBool(IDC_TDM_PT_CB4,data,TDEM_USEPATH);
			GetString(IDC_TDM_PT4,data,TDEM_PATH);
			GetLong(IDC_TDM_PT_N4,data,TDEM_PATH_NUM);
			GetBool(IDC_TDM_AT_CB4,data,TDEM_LOCALPATH);
			GetLong(IDC_TDM_DD_OM4,data,TDEM_PATH_TYP);
			GetReal(IDC_TDM_NN4,data,TDEM_PATH_SPEED);
			GetBool(IDC_TDM_AT_FF4,data,TDEM_PATHLOCROT);

			//rest
			GetLong(IDC_TDM_RND5,data,TDEM_SEED);
			GetLong(IDC_TDM_DIR1,data,TDEM_DIRECTION_TYP);

			GetReal(IDC_TDM_ED1,data,TDEM_SPEED);
			GetReal(IDC_TDM_ED2_X,data,TDEM_DISPERSING_X);
			GetReal(IDC_TDM_ED2_Y,data,TDEM_DISPERSING_Y);
			GetReal(IDC_TDM_ED2_Z,data,TDEM_DISPERSING_Z);


			GetBool(IDC_TDM_CHECK1,data,TDEM_RANDOM);
			GetBool(IDC_TDM_CHS1,data,TDEM_USESPCOR);
			GetBool(IDC_TDM_CHD2,data,TDEM_LOCALDES);
			GetBool(IDC_TDM_CHS2,data,TDEM_DESTALWAYS);

			//Textur
			GetLong(IDC_TDM_CHA1,data,TDEM_SPEEDCHAN);
			GetLong(IDC_TDM_TEX_N1,data,TDEM_SPEEDTEX);

			//PPCOll
			GetBool(IDC_TDM_PPCOLL,data,TDEM_PPCOLL);
			GetLong(IDC_TDM_PPOVERSAM,data,TDEM_PPOVERSAM);
			GetReal(IDC_TDM_PPRADIUS,data,TDEM_PPRADIUS);
			//Gravitation
			GetVector(IDC_TDM_GRV1_X,IDC_TDM_GRV1_Y,IDC_TDM_GRV1_Z,data,TDEM_GRAVITI);

			break;	

		//Emitter
		case IDC_TDM_EDN3:
			GetString(IDC_TDM_EDN3,st);
			Enable(IDC_TDM_N3,(st != ""));
			/*
			//TEST TEST TEST TEST SETESTSET
			//doc = plugOb->GetDocument();
			//if (!doc) break;
			st = GetNumName(*data,TDEM_EMITTERNAME,TDEM_NUM_EMITTER);
			print(st);
			//EmiterOb = (ParticleObject*)doc->SearchObject(st);
			//if (!EmiterOb) break; 
			//if (EmiterOb->GetType()!=Tparticle) break;
			//print("EM");
			//TEST TEST TEST TEST SETESTSET
			*/

			//Enable(IDC_TDM_STATIC2,(st == ""));
			break;

		//Quel
		case IDC_TDM_EDN4:
			GetString(IDC_TDM_EDN4,st);
			Enable(IDC_TDM_N4,(st != ""));
			break;
		case IDC_TDM_SEL4:
			GetString(IDC_TDM_SEL4,st);
			Enable(IDC_TDM_SEL_N4,(st != ""));
			break;
		case IDC_TDM_VEX4:
			GetString(IDC_TDM_VEX4,st);
			Enable(IDC_TDM_VEX_N4,(st != ""));
			break;

		//Ziel
		case IDC_TDM_CHECK3:
			GetBool(IDC_TDM_CHECK3,bo);
			Enable(IDC_TDM_EDN5,bo);
			Enable(IDC_TDM_SEL5,bo);
			Enable(IDC_TDM_GRL213,bo);


			GetString(IDC_TDM_EDN5,st);
			Enable(IDC_TDM_N5,(st != "") && bo);

			GetString(IDC_TDM_SEL5,st);
			Enable(IDC_TDM_SEL_N5,(st != "") && bo);
			break;

		case IDC_TDM_EDN5:
			GetString(IDC_TDM_EDN5,st);
			Enable(IDC_TDM_N5,(st != ""));
			break;
		case IDC_TDM_SEL5:
			GetString(IDC_TDM_SEL5,st);
			Enable(IDC_TDM_SEL_N5,(st != ""));
			break;

		//WEG
		case IDC_TDM_PT_CB4:
			GetBool(IDC_TDM_PT_CB4,bo);
			Enable(IDC_TDM_PT4,bo);
			Enable(IDC_TDM_GRL232,bo);

			GetString(IDC_TDM_PT4,st);
			Enable(IDC_TDM_PT_N4,((st != "") && bo));
			break;

		case IDC_TDM_PT4:
			GetString(IDC_TDM_PT4,st);
			Enable(IDC_TDM_PT_N4,((st != "")));
			break;

			//TEXTUR
		case IDC_TDM_CHA1:
			GetLong(IDC_TDM_CHA1,lo);
			Enable(IDC_TDM_TEX_N1,(lo != 20));
			break;

			//Velo
			/*
		case IDC_TDM_CHECK2:
			GetBool(IDC_TDM_CHECK2,bo);
			Enable(IDC_TDM_GR3,bo);
			Enable(IDC_TDM_GR4,bo);
			Enable(IDC_TDM_GR5,bo);
			break;
		*/

		case IDC_TDM_CHD2:	
			GetBool(IDC_TDM_CHD2,bo);
			Enable(IDC_TDM_CHR2,bo);
			break;

		case IDC_TDM_AT_CB4:
			GetBool(IDC_TDM_AT_CB4,bo);
			Enable(IDC_TDM_AT_FF4,bo);
			break;

		case IDC_TDM_PPCOLL:
			//PPCOll
			GetBool(IDC_TDM_PPCOLL,bo);
			Enable(IDC_TDM_PPOVERSAM,bo);  //Collision Oversampling
			Enable(IDC_TDM_PPRADIUS,bo); //Particle Radius
			break;

		/*
		//CREATE EMITTER
		case IDC_TDM_STATIC2://?????????????ß
			if (!plugOb) break;
			EmiterOb = (ParticleObject*)plugOb->down();
			if (!EmiterOb){ CreateEmitter(plugOb->GetDocument(),plugOb);}
			if (EmiterOb) if (EmiterOb->GetType()!=Oparticle) { CreateEmitter(plugOb->GetDocument(),plugOb);}
			break;
		*/
		
	    
			//Random ??????
		case IDC_TDM_BT1:
			//Create Neu SEED
			rnd = gNew(Random);if (!rnd) break;
			rnd->Init(GeGetTimer());
			LONG ran = rnd->Get01()*10000;
			SetLong(IDC_TDM_RND5,ran);
			gDelete(rnd);
			break;	

	}
	return TRUE;
}

//""""""""""""""""""" Offne Dialog """"""""""""""""""""
static Bool EditSettings(PluginObject *op)
{
	BaseContainer data = op->GetData();

	TDMDialog dlg(&data,op);
	if (!dlg.Open()) return FALSE;
	op->SetData(data);
	//if (EmiterOb){EmiterOb->SetData(EMdata);EmiterOb->Message(MSG_UPDATE);}//TEST
	return TRUE;
}

//""""""""""""""""""" Set Standart Werte """"""""""""""""""""
static Bool AllocSettings(PluginObject *op)
{	
	BaseContainer data;
	//Emitter
	data.SetString(TDEM_EMITTERNAME,"");
	data.SetLong(TDEM_NUM_EMITTER,0);

	//Quele
	data.SetString(TDEM_OBJECTNAME,"");
	data.SetLong(TDEM_NUM_OBJECT,0);
	data.SetString(TDEM_SEL_OBJECT,"");
	data.SetLong(TDEM_SEL_NUM_OBJECT,0);
	//VertexQ
	data.SetString(TDEM_VERTEX,"");
	data.SetLong(TDEM_VERTEX_NUM,0);

	//Ziel
	data.SetBool(TDEM_DESTLOCROT,FALSE);
	data.SetBool(TDEM_USEDEST,FALSE);
	data.SetString(TDEM_DESTINATION,"");
	data.SetLong(TDEM_NUM_DESTIN,0);
	data.SetString(TDEM_SEL_DESTIN,"");
	data.SetLong(TDEM_SEL_NUM_DESTIN,0);
	data.SetLong(TDEM_DEST_TYP,0);
	data.SetReal(TDEM_DEST_SPEED,0);

	//Weg
	data.SetBool(TDEM_USEPATH,FALSE);
	data.SetString(TDEM_PATH,"");
	data.SetLong(TDEM_PATH_NUM,0);
	data.SetBool(TDEM_LOCALPATH,FALSE);
	data.SetLong(TDEM_PATH_TYP,0);
	data.SetReal(TDEM_PATH_SPEED,0);
	data.SetBool(TDEM_PATHLOCROT,FALSE);

	//rest
	data.SetLong(TDEM_SEED,0);
	data.SetLong(TDEM_DIRECTION_TYP,0);

	data.SetReal(TDEM_SPEED,150.0);
	data.SetReal(TDEM_DISPERSING_X,0);
	data.SetReal(TDEM_DISPERSING_Y,0);
	data.SetReal(TDEM_DISPERSING_Z,0);

	data.SetBool(TDEM_RANDOM,FALSE);
	data.SetBool(TDEM_USESPCOR,FALSE);
	data.SetBool(TDEM_LOCALDES,FALSE);
	data.SetBool(TDEM_DESTALWAYS,FALSE);

	//Textur
	data.SetLong(TDEM_SPEEDCHAN,20);
	data.SetLong(TDEM_SPEEDTEX,0);

	//PPCOll
	data.SetBool(TDEM_PPCOLL,FALSE);
	data.SetLong(TDEM_PPOVERSAM,1);
	data.SetReal(TDEM_PPRADIUS,5.0);

	//Gravitation
	data.SetVector(TDEM_GRAVITI,0.0);

	op->SetData(data);
	return TRUE;
}

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
static LONG GetDescription(PluginObject *op, LONG id)
{
	switch (id)
	{
		//Emitter
		case TDEM_EMITTERNAME : return PARAM_TOGGLE;
		case TDEM_NUM_EMITTER : return PARAM_LINEAR;

		//Quele
		case TDEM_OBJECTNAME : return PARAM_TOGGLE;
		case TDEM_NUM_OBJECT : return PARAM_LINEAR;
		case TDEM_SEL_OBJECT : return PARAM_TOGGLE;
		case TDEM_SEL_NUM_OBJECT : return PARAM_LINEAR;
		//VertexQ
		case TDEM_VERTEX : return PARAM_TOGGLE;
		case TDEM_VERTEX_NUM : return PARAM_LINEAR;

		//Ziel
		case TDEM_DESTLOCROT : return PARAM_TOGGLE;
		case TDEM_USEDEST : return PARAM_TOGGLE;
		case TDEM_DESTINATION : return PARAM_TOGGLE;
		case TDEM_NUM_DESTIN : return PARAM_LINEAR;
		case TDEM_SEL_DESTIN : return PARAM_TOGGLE; 
		case TDEM_SEL_NUM_DESTIN : return PARAM_LINEAR; 
		case TDEM_DEST_TYP : return PARAM_TOGGLE;
		case TDEM_DEST_SPEED : return PARAM_LINEAR;

		//Weg
		case TDEM_USEPATH : return PARAM_TOGGLE;
		case TDEM_PATH: return PARAM_TOGGLE;
		case TDEM_PATH_NUM : return PARAM_LINEAR;
		case TDEM_LOCALPATH : return PARAM_TOGGLE;
		case TDEM_PATH_TYP : return PARAM_TOGGLE;
		case TDEM_PATH_SPEED : return PARAM_LINEAR;
		case TDEM_PATHLOCROT : return PARAM_TOGGLE;


		//rest
		case TDEM_SEED : return PARAM_LINEAR;
		case TDEM_DIRECTION_TYP : return PARAM_TOGGLE;

		case TDEM_SPEED : return PARAM_SCALE; 
		case TDEM_DISPERSING_X	: return PARAM_LINEAR; 
		case TDEM_DISPERSING_Y	: return PARAM_LINEAR;
		case TDEM_DISPERSING_Z	: return PARAM_LINEAR;

		case TDEM_RANDOM : return PARAM_TOGGLE;
		case TDEM_USESPCOR : return PARAM_TOGGLE;
		case TDEM_LOCALDES : return PARAM_TOGGLE;
		case TDEM_DESTALWAYS : return PARAM_TOGGLE;

		//Textur
		case TDEM_SPEEDCHAN : return PARAM_TOGGLE;
		case TDEM_SPEEDTEX : return PARAM_TOGGLE;

		//PPCOll
		case TDEM_PPCOLL : return PARAM_TOGGLE;
		case TDEM_PPOVERSAM : return PARAM_TOGGLE;
		case TDEM_PPRADIUS : return PARAM_SCALE; //???

		//Gravitation
		case TDEM_GRAVITI : return PARAM_LINEAR; 

		//INTERN
		case TDEM_DRND : return PARAM_SOLID;
		case TDEM_REVAP : return PARAM_SOLID;
		case TDEM_VER : return PARAM_SOLID;
	}
	return PARAM_TOGGLE;
}

#endif

//#############################  PLUGIN ###########################################

//<<<<<<<<<<<<<<<<<<<<<<<< DRAW >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
static Bool Draw(PluginObject *op, BaseDraw *bd, BaseDrawHelp *bh)
{

	BaseContainer &data = *op->GetDataInstance();

	String  EmName = data.GetString(TDEM_EMITTERNAME);
	String  SourceName = data.GetString(TDEM_OBJECTNAME);
	String  DestName = data.GetString(TDEM_DESTINATION);

	BaseDocument  *doc = op->GetDocument();
	if (!doc) return TRUE;

	//Remo: 02.04.2007
	//bd->SetPen(bd->GetObjectColor(op)); 
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
	BaseObject *srcOb = doc->SearchObject(SourceName);
    if (!srcOb) 
	{
		srcOb = op->GetUp();
		if (!srcOb) return TRUE;
	}
	Matrix pmg=srcOb->GetMg();
	bd->Line3D(emg.off,pmg.off);

	//bd->SetPen(bd->GetObjectColor(srcOb));

	//Find Destination
	BaseObject *DestOb = doc->SearchObject(DestName);
	if ((!DestOb) || DestOb == srcOb)
	{
		DestOb = (PointObject*)srcOb->GetUp();
		if (!DestOb) return TRUE;
	}
    if (!DestOb) return TRUE;
	Matrix DestMG=DestOb->GetMg();//Global Matrix of Dest
	bd->Line3D(pmg.off,DestMG.off);

	return TRUE;
}




//####################################################################################
//####################################################################################
//####################################################################################

LONG GetSelection(LONG **SourceSel, BaseObject *srcOb ,const String SelectSource,LONG SourcePointCnt)
{
	LONG c,la;
	BaseSelect *Select = NULL;
	LONG PointSelCnt = 0;
	if (SelectSource != ""){
		SelectionTag *SelPointTag = GetPSTag(srcOb,SelectSource);
		if (SelPointTag){
			Select = SelPointTag->GetBaseSelect();
			PointSelCnt = Select->GetCount();
			if (PointSelCnt > 0){
				(*SourceSel) = (LONG*) GeAlloc(sizeof(LONG)*PointSelCnt+1);
				if (SourceSel){
					la = 0;
					for (c=0;c<SourcePointCnt;c++){
						if (Select->IsSelected(c)){ (*SourceSel)[la] = c; la++; }
					}
				}	
				return PointSelCnt;
			}//if (PointSelCnt > 0){
		}//if (SelPointTag){
	}//end SELECTION
	return 0;
}

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
const Matrix Local(Matrix mg, Vector poff ,Bool loc)
{
	if (loc){
		Matrix lmg;
		lmg = mg;
		lmg.off = poff;
		return lmg;
	}else{
		return mg;
	}
}


//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//!!!!!!!   LANGSAM  !!!!!!!!!!!
const Matrix Local(Matrix mg, Vector poff, Vector pv3, Bool loc, Bool rot)
{
	if (loc){
		Matrix lmg;
		if (rot){
			lmg = HPBToMatrix(VectorToHPB(pv3));//!!!!!!!   LANGSAM  !!!!!!!!!!!
			lmg.off = poff;
			return lmg;
		}else{
			lmg = mg;
			lmg.off = poff;
			return lmg;
		}
	}else{
		return mg;
	}

	/*
	//????????????????????????
	Matrix lmg;

	if (loc){
		lmg = mg;
		lmg.off = poff;
		return lmg;
	}else{
		return mg;
	}



	if (rot){
			lmg = mg;
			lmg = HPBToMatrix(VectorToHPB(pv3));//!!!!!!!   LANGSAM  !!!!!!!!!!!
			return lmg;
	}else{
		return mg;
	}
	*/

}


//"""""""""""""""""""""""""""""""""""""""""""""""""
Vector RndVec(Random *rnd,const Vector disp)//OK
{
	Vector vec;
	vec.x = rnd->Get11()*disp.x;
	vec.y = rnd->Get11()*disp.y;
	vec.z = rnd->Get11()*disp.z;
	return vec;
}

/*
//""""""""""""""""""""""""""""""""""""""
const Vector vnorm(const Vector v)	//VNORM
{
	return v / Len(v);
}
*/


//##########################################################################################
//##########################################################################################
//##########################################################################################
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  ModifyParticles  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
static void ModifyParticles(PluginObject *op, Particle *ps, BaseParticle *ss, LONG pcnt, Real diff)
{
    if (!op->GetDeformMode()) return;//Return wenn Object deaktiwiert ist

	BaseContainer data = op->GetData();//GET SETING
	//---------------------- VAR -----------------------------------
    BaseDocument	*doc = NULL;//Aktuele Document
	doc = op->GetDocument(); if (!doc) {GePrint("TDEM: Document not found!"); return;}
	LONG			fps = doc->GetFps();//Get Doc FPS

	//EMITTER
    ParticleObject	*EmiterOb = NULL;//Emiter zum Modifizieren
	Particle		*EmParticle = NULL;
	ParticleTag		*EmPTag = NULL;

	//SOURCE
    BaseObject		*tempOb= NULL;//SOURCE Temporale Object
	PointObject		*srcOb = NULL;//SOURCE Emiter Start Point Object
	ParticleObject	*ParticlesrcOb = NULL;//SOURCE Emiter Start Particle Object

	LONG			SourcePointCnt = 0;//SOURCE POINT COUNT
	Vector			*SourcePoints = NULL;//SOURCE POINTS ARRAY
	LONG			SourceSelCnt = 0;//SOURCE SELECTION COUNT
	LONG			*SourceSel = NULL;//SOURCE SELECTION ARRAY
	Real			Cspeed = data.GetReal(TDEM_SPEED);
	Real			speed = Cspeed;
	Vector			dispersing;

	dispersing.x = data.GetReal(TDEM_DISPERSING_X);
	dispersing.y = data.GetReal(TDEM_DISPERSING_Y);
	dispersing.z = data.GetReal(TDEM_DISPERSING_Z);
	Bool			DispUse = ((dispersing.x+dispersing.y+dispersing.z)>0);
	Bool			PointRandom = data.GetBool(TDEM_RANDOM);
	LONG			DirectionTyp = data.GetLong(TDEM_DIRECTION_TYP);


	//DEST
	PointObject		*DestOb = NULL;//DEST OBJECT
	LONG			DestObTyp;//DEST TYP
	LONG			DestPointCnt = 0;//DEST POINT COUNT
	Vector			*DestPoints=NULL;//DEST POINT ARRAY
	LONG			DestSelCnt = 0;//DEST SELECTION COUNT
	LONG 			*DestSel = NULL;//DEST SELECTION ARRAY
	Bool			DestLocalRot = data.GetBool(TDEM_DESTLOCROT);
	Bool			UseDest = data.GetBool(TDEM_USEDEST);
	Bool			UseSPcor = data.GetBool(TDEM_USESPCOR);
	Bool			LocalDes = data.GetBool(TDEM_LOCALDES);
	Bool			DestAlwais = data.GetBool(TDEM_DESTALWAYS);
	LONG			Dest_Typ = data.GetBool(TDEM_DEST_TYP);
	Real			Dest_Speed = data.GetReal(TDEM_DEST_SPEED);
	Bool			Dest_SpeedZ = (Dest_Speed == 0.0);//????

	//WEG
	SplineObject	*PathOb = NULL;
	LONG			PathObTyp;
	Bool			UsePath = data.GetBool(TDEM_USEPATH);
	Bool			LocalPath = data.GetBool(TDEM_LOCALPATH);
	LONG			Path_Typ = data.GetLong(TDEM_PATH_TYP);
	Real			Path_Speed = data.GetReal(TDEM_PATH_SPEED);
	if (Path_Speed == 0) Path_Speed = Cspeed;//TEST;;;
	Bool			PathLocalRot = data.GetBool(TDEM_PATHLOCROT);

	//Textur
	LONG			SpeedChannel = data.GetLong(TDEM_SPEEDCHAN);
	LONG			SpeedTexNr = data.GetLong(TDEM_SPEEDTEX);
	Real			*VertexColorArray = NULL;//VERTEX COLOR Array
	Vector			*VertexNormArray = NULL;//VERTEX NORMAL Array

	//INTERN
	LONG		revaP = data.GetLong(TDEM_REVAP);
	LONG		drnd = data.GetLong(TDEM_DRND);//RANDOM 
	LONG		rndinit = data.GetLong(TDEM_SEED);//RANDOM

	//PP Collision
	Bool 		partColl = data.GetBool(TDEM_PPCOLL); //Use PP Collision
	LONG		partOverSam = data.GetLong(TDEM_PPOVERSAM);  //Collision Oversampling
	Real		PartRadius = data.GetReal(TDEM_PPRADIUS); //Particle Radius
	Real		elast = 1.0;

	Real		ColRadius = PartRadius*2;
	Real		ColRadius2 = ColRadius*ColRadius;
	Vector		PosDif;

	Vector		ampGraviti = data.GetVector(TDEM_GRAVITI)*diff*25.0;
	Bool		partGraviti = (ampGraviti != 0);//print(partGraviti);


	Real		visDrag = 10*diff;//print(visDrag);
	//Bool		partReibug = (visDrag != 0);
	Bool		partReibug = TRUE;

	//-------------------------- NAMEN ----------------------------
	//EMITTER NAME
	String  EmName = GetNumName(data,TDEM_EMITTERNAME,TDEM_NUM_EMITTER);
	//SOURCE NAME
    String  SourceName = GetNumName(data,TDEM_OBJECTNAME,TDEM_NUM_OBJECT);
	//SOURCEC SELECTION
	String  SelectSource = GetNumName(data,TDEM_SEL_OBJECT,TDEM_SEL_NUM_OBJECT);
	//SOURCE SPEED VERTEX
	String	VertexSource = GetNumName(data,TDEM_VERTEX,TDEM_VERTEX_NUM);
	Bool	VertexSourceB = (VertexSource != "");
	Bool	SpeedTex = FALSE;//TEST
	//DEST NAME
	String  DestName = GetNumName(data,TDEM_DESTINATION,TDEM_NUM_DESTIN);
	//DEST SELECTION
	String	DestSelName = GetNumName(data,TDEM_SEL_DESTIN,TDEM_SEL_NUM_DESTIN);
	//PATH NAME
	String  PathName = GetNumName(data,TDEM_PATH,TDEM_PATH_NUM);

	//-------------------------FIND OBJEKTE--------------------------------
	//EMITTER Search
    EmiterOb = (ParticleObject*)doc->SearchObject(EmName);
	if (!EmiterOb){ EmiterOb = (ParticleObject*)op->GetDown();}
	if (!EmiterOb) {GePrint("TDEM: Emitter "+EmName+" not found"); return;}
	//if (EmiterOb->GetType()!=Oparticle) {GePrint("TDEM:"+EmiterOb->GetName()+" is not an emitter");return;}
	//EMITTER Particle Tag
	EmPTag = (ParticleTag*)EmiterOb->GetTag(Tparticle);if (!EmPTag) return;
	EmParticle = EmiterOb->GetParticle(EmPTag,0);
	//EMITTER -NICHT RICHTIGE-
	if (EmParticle != ps) return;//Ob die particles EmiterOb gehören

	//SOURCE Search
    tempOb = doc->SearchObject(SourceName);
    if (!tempOb){
		tempOb = op->GetUp();
		if (!tempOb){GePrint("TDEM: Source "+SourceName+" not found"); return;}
		if (tempOb->GetType() == Oparticle) {GePrint("TDEM: isn't an Emitter"); return;}
	}
	LONG TEMPTyp = tempOb->GetType();
	//if (!(tempOb->GetLayerType()&L_pointobject) && (TEMPTyp!=Oparticle) && (TEMPTyp!=Osplineprimitive)) {GePrint("TDEM:"+tempOb->GetName()+" isn't an Point Ob."); return;}
    //if ((TEMPTyp!=Opolygon) && (TEMPTyp!=Ospline) && (TEMPTyp!=Osplineprimitive) && (TEMPTyp!=Oparticle)) {GePrint("TDEM:"+tempOb->GetName()+" is not Point Ob."); return;}
	
	//DEST Search
	if (UseDest){
		DestOb = (PointObject*)doc->SearchObject(DestName);
		if ((!DestOb)){
			DestOb = (PointObject*)tempOb->GetUp();
			if (!DestOb) {UseDest = FALSE; DestOb = (PointObject*)tempOb; GePrint("TDEM: Dest "+DestName+" not found");}
		}
		DestObTyp = DestOb->GetType();
		//if (!(DestOb->GetLayerType()&L_pointobject) && (DestObTyp!=Osplineprimitive)){UseDest = FALSE;DestOb = (PointObject*)tempOb;}//???????
		//if ((DestObTyp!=Opolygon) && (DestObTyp!=Ospline) && (DestObTyp!=Osplineprimitive)) {UseDest = FALSE;DestOb = (PointObject*)tempOb;}//???????
	}
	//PATH Search
	if (UsePath ){
		PathOb = (SplineObject*)doc->SearchObject(PathName);
		if (PathOb) {
			PathObTyp = PathOb->GetType();
			//if ((PathObTyp!=Ospline) && (PathObTyp!=Osplineprimitive)) {UsePath = FALSE;}
			if ((PathObTyp!=Ospline) && (PathObTyp!=Ospline)) {UsePath = FALSE;} //Remo: 02.04.2007
		}else{UsePath = FALSE;}
		if (tempOb == PathOb) {UsePath = FALSE;}//?????????? ABSTÜRZE ???
	}


	//----------------------------Vorbereitung-------------------------------------
	//-------------------------------VAR-2------------------------------
	LONG	c,d,o;  //Lauf Variable
	LONG	la = 0; //Such Counter 
	Real	PDLen; //Source Destination Distanz
	Vector	SPnt;
	Vector  ASPnt;
	Vector  GlobSPnt;
	Vector  DestPnt;
	BaseContainer	nullbc;nullbc.SetId(0);
	Bool    modcom = FALSE;

	//Random Number(1) Init
	Random *rnd = gNew(Random);//SD
	rnd->Init(rndinit);//SD

	//Random Number(2) Init
	Random *rnd2 = gNew(Random);
	if (drnd == 0) drnd = rnd->Get01()*10000; //SD
	rnd2->Init(drnd);//SD

	//<<< SOURCE  MATRIX >>>
	Matrix  SourceML  = tempOb->GetMl(), ISourceML=!SourceML;
	Matrix	SourceMG  = tempOb->GetMg();
	Matrix  SourceMGU = tempOb->GetUpMg();
	Vector	SourcePos = tempOb->GetPos()*SourceMGU;//TEST

	//<<< DEST INFO >>>
	Matrix	DestMG;
	Matrix	DestMGU;
	Vector	DestPos;
	//<<< DEST INFO >>>
	if (UseDest){
		DestMG = DestOb->GetMg();
		DestMGU = DestOb->GetUpMg();
		DestPos = DestOb->GetPos()*DestMGU;//<<<<Zuerst Position und Dann in Line konvert

		/* //OLD
		LONG	DestObTyp = DestOb->GetType();
		if ((DestObTyp==Osplineprimitive) || (DestObTyp==Ospline)) 
		{
			DestOb = static_cast<SplineObject*>(DestOb)->GetLineObject(doc,1);//OK Objekt Immer auf Position = 0,0,0
		}
		*/
		//NEW SYSTEM
		//if (!(DestOb->GetLayerType()&L_pointobject))
		if (!(DestOb->GetInfo() & OBJECT_ISSPLINE)) //Remo: 02.04.2007
		{
			BaseObject *dt = DestOb;
			DestOb = ToPoly(dt->GetDeformCache());
			if (!DestOb) DestOb = ToPoly(dt->GetCache());if (!DestOb) return;
			//if (!(DestOb->GetLayerType()&L_pointobject)) return;
			if (!(DestOb->GetInfo() & OBJECT_ISSPLINE))  return; //Remo: 02.04.2007
		}

		DestPointCnt = DestOb->GetPointCount();//<<<<<<
		if (DestPointCnt<=0) {print("Dest1: 0 points");  return;}
		DestPoints = DestOb->GetPoint();//<<<<<

		if (UseSPcor){
			PDLen = Len(DestPos-SourcePos);
			if (PDLen < speed) PDLen = speed;//????????????????????????????
		}
		//Dest Selection
		DestSelCnt = GetSelection(&DestSel,DestOb,DestSelName,DestPointCnt);

	}//end Use Dest

	//<<< PATH INFO >>>
	Matrix PathMG;		
	Matrix PathMGU;
	Vector PathPos;
	LONG   PathPointCnt;
	LONG   PathSegCnt;
	Real   splen;
	//<<< PATH INFO >>>
	if (UsePath){
		PathMG = PathOb->GetMg();
		PathMGU = PathOb->GetUpMg();
		PathPos = PathOb->GetPos()*DestMGU;//<<<<Zuerst Position und Dann in Line konvert
		//if (PathObTyp==Osplineprimitive) 
		if (PathObTyp==Ospline)  //Remo: 02.04.2007
		{
			PathOb = PathOb->GetRealSpline();//OK Objekt Immer auf Position = 0,0,0
		}
		PathPointCnt = PathOb->GetPointCount();//<<<<<<
		if (PathPointCnt>0){
			PathSegCnt = PathOb->GetSegmentCount();
			if (PathSegCnt == 0) PathSegCnt = 1;
		}else{UsePath = FALSE;}
	}//PATH INFO - END


    //...................................WORKING.....................................

    if (TEMPTyp==Oparticle)//if-------EMITTER OBJEKT---------------------------------
    {
	//..............................EMITTER als EMITTER..............................	
		//VAR-------
		Particle	*PPParticle = NULL;
		ParticleTag	*PPPTag = NULL;
		//Convert tempOb to ParticleObject
		ParticlesrcOb = (ParticleObject*)tempOb;
		//Get Particle Tag
		PPPTag = (ParticleTag*)ParticlesrcOb->GetTag(Tparticle);if (!PPPTag) goto Error;
		SourcePointCnt = ParticlesrcOb->GetParticleCount();
		if (SourcePointCnt<=0) {GePrint("Source1: 0 points");goto Error;}//?????????? Alle Partikel Kilen
		PPParticle = ParticlesrcOb->GetParticle(PPPTag,0);

		Bool PointVelo = (DirectionTyp < 2);
		//---------------------------------------------------------------------------
		for (c = 0; c < pcnt; c++){
			if (!(ps[c].bits & PARTICLE_ALIVE)) continue;
			//ps[c].bits = 3;//TEST
			//Set Start Position
			if (ps[c].t<=diff){
				revaP = c % SourcePointCnt;
				//revaP = (revaP + 1) % SourcePointCnt;
				//TEST
				if (!(PPParticle[revaP].bits & PARTICLE_ALIVE)){
					ps[c].bits = 0;//???????
					ps[c].t = 0;
				}
				SPnt = PPParticle[revaP].off;
				//Set Start Position OK
				ps[c].off = SPnt;
				//Set Vector
				if (PointVelo){	
					if (UseDest){
						//DestPnt = DestPoints[c % DestPointCnt]*Local(DestMG,SPnt,PPParticle[c % SourcePointCnt].v3,LocalDes,DestLocalRot);//ROTATE LOCAL
						if (!DestSel)DestPnt = DestPoints[c % DestPointCnt]*Local(DestMG,SPnt,PPParticle[c % SourcePointCnt].v3,LocalDes,DestLocalRot);
						else DestPnt = DestPoints[DestSel[c % DestSelCnt]]*Local(DestMG,SPnt,PPParticle[c % SourcePointCnt].v3,LocalDes,DestLocalRot);
			
						ASPnt = DestPnt-SPnt;
						if (UseSPcor){
							speed = Cspeed / (PDLen / Len(ASPnt));
						}else{speed = Cspeed;}
					}else{
						ASPnt = PPParticle[revaP].v3;//ORIGENAL OK byt no rotation??
					}
					if (DispUse){
						Vector stt = vnorm(ASPnt);//SD
						stt += RndVec(rnd2,dispersing);//SD
						ss[c].v += stt*speed;//SD
					}else{
						ss[c].v += vnorm(ASPnt)*speed;
					}//if (dispersing > 0)
					ss[c].count++;
				}//if (PointVelo)
			}else{//if (ps[c].t<=diff){ //IMEER

				//DEST ALWAAS!!!!!!!!!!!!!!!!!
				if (UseDest && DestAlwais){
					GlobSPnt = PPParticle[c % SourcePointCnt].off; //Get Point OK??
					//DestPnt = DestPoints[c % DestPointCnt]*Local(DestMG,GlobSPnt,PPParticle[c % SourcePointCnt].v3,LocalDes,DestLocalRot);//ROTATE LOCAL
					if (!DestSel)DestPnt = DestPoints[c % DestPointCnt]*Local(DestMG,GlobSPnt,PPParticle[c % SourcePointCnt].v3,LocalDes,DestLocalRot);
					else DestPnt = DestPoints[DestSel[c % DestSelCnt]]*Local(DestMG,GlobSPnt,PPParticle[c % SourcePointCnt].v3,LocalDes,DestLocalRot);

					ASPnt = DestPnt - ps[c].off;
					if (UseSPcor){
						//speed = Cspeed / (PDLen / Len(DestPnt - GlobSPnt));//?????
						if (Dest_SpeedZ) {
							speed = Len(ps[c].v3) / (PDLen / Len(DestPnt - GlobSPnt));//????
						}else{
							speed = Dest_Speed / (PDLen / Len(DestPnt - GlobSPnt));//ORIGIN
						}

					}else{//speed = Cspeed;
						if (Dest_SpeedZ) {
							speed = Len(ps[c].v3);
						}else{
							speed = Dest_Speed;//ORIGIN
						}
					}
					ss[c].v += vnorm(ps[c].v3+ASPnt)*speed;//OK ??
					ss[c].count++;
				}//if (UseDest && DestAlwais)
			}//if (ps[c].t<=diff)
		}//for (c = 0; c < pcnt; c++)


		//____________________ Spline Weg EMITTER-SOURCE____________________
		if (UsePath){
			for (la=0; la<PathSegCnt; la++){
				PathOb->InitLength(la);
				splen = PathOb->GetLength();
				for (c=la; c<pcnt; c+=PathSegCnt){
					if (!(ps[c].bits & PARTICLE_ALIVE)) continue;
					GlobSPnt = PPParticle[c % SourcePointCnt].off;//?????????????
					Real tt = ps[c].t / (splen / Path_Speed);//ORIGENAL
					if (Path_Typ == 1) { tt = tt-LONG(tt); }
					if (tt < 0.0) {tt = 0.0;}
					if (tt <= 1.0) {
						Vector dp0 = PathOb->GetSplinePoint(PathOb->UniformToNatural(tt),la);
						dp0 *= Local(PathMG,GlobSPnt,PPParticle[c % SourcePointCnt].v3,LocalPath,PathLocalRot);
						Vector rr = dp0 - ps[c].off;
						ss[c].v += vnorm(rr)*Path_Speed;//ORIGENAL
						ss[c].count++;
					}else{
						if (Path_Typ == 2) {ps[c].bits = ps[c].bits & ~PARTICLE_VISIBLE;}
					}
				}//for (c=la; c<pcnt; c+=PathSegCnt)
			}//for (la=0; la<PathSegCnt; la++)
			PathOb->FreeLength();//???
		}
		//~~~~~~~~~~~~~~~~ Spline Weg EMITTER-SOURCE~~~~~~~~~~~~~~~~~~~~~~~~~~~


    }else{//if---------------------POINT OBJEKT--------------------------------------

//....................................................................................
//...............................POINTS als SOURCE...................................
//....................................................................................
		//--------------------------------------------

		//--------------------- NEW SYSTEM 4 ----------------
		BaseObject *def = NULL;
		srcOb = ToPoly(tempOb->GetCache());
		if (srcOb) def = srcOb->GetDeformCache();
		else def = tempOb->GetDeformCache();
		if (def) srcOb = ToPoly(def);
		//if (!srcOb && (tempOb->GetLayerType()&L_pointobject)) srcOb = ToPoly(tempOb);
		if (!srcOb && (tempOb->GetInfo()&OBJECT_ISSPLINE)) srcOb = ToPoly(tempOb); //Remo: 02.04.2007
		if (!srcOb) { //?????????????????
			print("TDEM:  build cache");//DEBUG
			/// TODO 
			//Remo: 02.04.2007
			//srcOb = ToPoly(SendModelingCommand(MCOMMAND_CURRENTSTATETOOBJECT,doc,tempOb,&nullbc,MODIFY_ALL));
			modcom = TRUE;
		}
		if (!srcOb) goto Error;	
		//if (!(srcOb->GetLayerType()&L_pointobject)) goto Error;	
		if (!(srcOb->GetInfo()&OBJECT_ISSPLINE)) goto Error;	
		//-------------------------------------------------------

		/*
		//GET OBJECT CACHE !! (SICHER???) VERSION 3
		BaseObject *def =NULL;
		srcOb = (PolygonObject*)tempOb->GetCache();
		if (srcOb) def = srcOb->GetDeformCache();
		else def = tempOb->GetDeformCache();
		if (def) srcOb = (PolygonObject*)def;
		if ((!srcOb) && (TEMPTyp!=Opolygon)) {
			print("TDEM:  build cache");//DEBUG
			//Wird nur einmal aufgerufen um cache zu erzeugen!
			srcOb = ToPoly(SendModelingCommand(MCOMMAND_CURRENTSTATETOOBJECT,doc,tempOb,&nullbc,MODIFY_ALL));
			modcom = TRUE;
		}
		if (!srcOb) srcOb = (PolygonObject*)tempOb;
		if (!srcOb) {goto Error;}
		*/
		//--------------------------------------------

		SourcePointCnt = srcOb->GetPointCount();if (SourcePointCnt<=0) {GePrint("Source2: 0 points");goto Error;}//TEST
		SourcePoints = srcOb->GetPoint();

		//GET SPEED VERTEX COLOR
		LONG ColorRes = 1;
		if (SpeedChannel != 20){
			if (srcOb->GetType()==Opolygon){//WENN QUELLE POLYGON
				VertexColorArray = (Real*)GeAlloc(sizeof(Real)*SourcePointCnt+1);if (!VertexColorArray) {goto Error; }
				ColorRes = GetVertexColor(&VertexColorArray,(PolygonObject*)srcOb,SourcePointCnt,SourcePoints,doc,SpeedChannel,SpeedTexNr);	

				//print(ColorRes);
			}
		}
		//GET VERTEX NORMAL (DirectionTyp == 0)
		LONG NormalRes = 1;
		if (DirectionTyp == 0){//Wenn Direction dann Get Vertex Normal
			if (srcOb->GetType()==Opolygon){
				VertexNormArray = (Vector*)GeAlloc(sizeof(Vector)*SourcePointCnt+1);if (!VertexNormArray) {goto Error; }
				NormalRes = GetVertexNormalMG(&VertexNormArray,(PolygonObject*)srcOb,SourcePointCnt,SourcePoints);if (NormalRes != 0) DirectionTyp = 1;	
			}else{
				DirectionTyp = 1;//Radial
			}
		}
		//GET SPEED VERTEX MAP
		LONG VMCnt = 1;//Vertex Map Point Count
		Real *VMarr = NULL;//Vertex Map
		if (VertexSourceB){
			VariableTag *VertexTag = GetVMTag(srcOb,VertexSource);
			if (!VertexTag) {VertexSourceB = FALSE;}else{
				VMCnt = VertexTag->GetDataCount();
				VMarr = (Real*)VertexTag->GetDataAddress();
			}
		}
		//SOURCE GET SELECTION
		SourceSelCnt = GetSelection(&SourceSel,srcOb,SelectSource,SourcePointCnt);

		//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		for (c=0; c<pcnt; c++){
			if (!(ps[c].bits & PARTICLE_ALIVE)) continue;
			//ps[c].bits = 3;//TEST

			if (PointRandom){//Random ?????
				if (pcnt > SourcePointCnt){	revaP = rnd->Get01()*pcnt;//Generate Random Point nach Particle
				}else{	revaP = rnd->Get01()*SourcePointCnt; }//Generate Random Point nach Points
			}else{	revaP = c;}//nicht Random

			if (!SourceSel)SPnt = SourcePoints[revaP % SourcePointCnt]; //Get Point STANDARD
			else SPnt = SourcePoints[SourceSel[revaP % SourceSelCnt]];
			GlobSPnt = SPnt * SourceMG;//Global

			if (ps[c].t<=diff){//GEBURT ANFANG

				ps[c].off = GlobSPnt;//Set Start Position OK

				if (DirectionTyp != 2){//Nicht Emitter Richtung ???

					if (UseDest){//Use Dest
						//DestPnt = DestPoints[revaP % DestPointCnt]*Local(DestMG,GlobSPnt,GlobSPnt,LocalDes,DestLocalRot);//ROTATE LOCAL
						if (!DestSel)DestPnt = DestPoints[revaP % DestPointCnt]*Local(DestMG,GlobSPnt,GlobSPnt,LocalDes,DestLocalRot);
						else DestPnt = DestPoints[DestSel[revaP % DestSelCnt]]*Local(DestMG,GlobSPnt,GlobSPnt,LocalDes,DestLocalRot);
			
						ASPnt = DestPnt-GlobSPnt;

						if (UseSPcor){//Tempo Anpassen
							speed = Cspeed / (PDLen / Len(ASPnt));
						}else{
							speed = Cspeed;
						}

					}else{// Kein Dest
						speed = Cspeed;

						switch(DirectionTyp){//RECHTUNG
							case 0://Vertex Normal
								ASPnt = VertexNormArray[revaP % SourcePointCnt]-SourcePos;//VERTEX NORMAL SD -SourcePos ist wichtig
							break;
							case 1://Radial
								//ASPnt = GlobSPnt-SourcePos;//STANDARD (RADIAL)
								ASPnt = GlobSPnt-SourcePos;
							break;
							case 2://Emitter Richtung ???????????
							break;
							default://Sonst Radial
								ASPnt = GlobSPnt-SourcePos;//STANDARD
						}
					}//if (UseDest)

					//Speed Textur
					if (ColorRes == 0){ speed = speed * VertexColorArray[revaP % SourcePointCnt];}
					if (VertexSourceB){	speed = speed * VMarr[revaP % VMCnt]; }//Speed Vertex Work!!!
					//pd[c].lifetime *= VMarr[revaP % VMCnt];//LifeTime Vertex DontWork
					if (DispUse){//Dispersio
						/*
						Vector stt = vnorm(ASPnt);//SD
						stt += RndVec(rnd2,dispersing);//SD
						ss[c].v +=  stt*speed;//SD
						*/
						Vector stt = vnorm(ASPnt);//GUT
						stt += RndVec(rnd2,dispersing)*SourceMG-SourcePos;//GUT
						ss[c].v +=  vnorm(stt)*speed;//GUT
					}else{//Keine Dispersion

						ss[c].v +=  vnorm(ASPnt)*speed;
					}//if (DispUse)
					ss[c].count++;
				}//if (DirectionTyp != 2){

			}else{//if (ps[c].t==pbirt) //IMEER auser GEBURT
				if (UseDest){
					//Rotate Local
					if (DirectionTyp == 0){
						//DestPnt = DestPoints[revaP % DestPointCnt]*Local(DestMG,GlobSPnt,VertexNormArray[revaP % SourcePointCnt],LocalDes,DestLocalRot);//ROTATE LOCAL
						if (!DestSel)DestPnt = DestPoints[c % DestPointCnt]*Local(DestMG,GlobSPnt,VertexNormArray[revaP % SourcePointCnt],LocalDes,DestLocalRot);
						else DestPnt = DestPoints[DestSel[c % DestSelCnt]]*Local(DestMG,GlobSPnt,VertexNormArray[revaP % SourcePointCnt],LocalDes,DestLocalRot);
					}else{
						//DestPnt = DestPoints[revaP % DestPointCnt]*Local(DestMG,GlobSPnt,GlobSPnt,LocalDes,DestLocalRot);//ROTATE LOCAL STANDARD
						if (!DestSel)DestPnt = DestPoints[c % DestPointCnt]*Local(DestMG,GlobSPnt,GlobSPnt,LocalDes,DestLocalRot);
						else DestPnt = DestPoints[DestSel[c % DestSelCnt]]*Local(DestMG,GlobSPnt,GlobSPnt,LocalDes,DestLocalRot);
					}
					ASPnt = DestPnt - ps[c].off;

					//Grawity !!!!!!!!!!!!!!!!!
					if (DestAlwais){   //Anzieung
						if (UseSPcor){//???????????
							//speed = Cspeed / (PDLen / Len(ASPnt));
							//speed = Cspeed / (PDLen / Len(DestPnt - GlobSPnt));//ORIGIN OLD
							//speed = Cspeed / (Len(DestPnt - ps[c].off) / PDLen);//???????????
							if (Dest_SpeedZ) {
								speed = Len(ps[c].v3) / (PDLen / Len(DestPnt - GlobSPnt));//????
							}else{
								speed = Dest_Speed / (PDLen / Len(DestPnt - GlobSPnt));//ORIGIN
							}
						}else{
							//speed = Cspeed;
							if (Dest_SpeedZ) {
								speed = Len(ps[c].v3);
							}else{
								//TEST
								if (ColorRes == 0){//TEXTUR TEST
									speed = Dest_Speed * VertexColorArray[revaP % SourcePointCnt];
								}else{
									speed = Dest_Speed;//ORIGIN
								}
							}
						}
						ss[c].v +=  vnorm(ps[c].v3+ASPnt)*speed;//STANDARD
						ss[c].count++;
					}// if ((UseDest) && (Dest_Typ > 0)){

					//TÖTEN  ??????
					if ((Dest_Typ == 2) && (Len(ASPnt)<(speed * 0.05))){
						ps[c].bits = ps[c].bits & PARTICLE_ALIVE;
					}
				}//if (UseDest){
			}//if (ps[c].t<=diff)
		}//for (c = 0; c < pcnt; c++)
		//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

		//____________________ Spline Weg ____________________
		if (UsePath){//WEG Schleife
			for (la=0; la<PathSegCnt; la++){
				PathOb->InitLength(la);
				splen = PathOb->GetLength();
				for (c=la; c<pcnt; c+=PathSegCnt){//TEST
					if (!(ps[c].bits & PARTICLE_ALIVE)) continue;
					if (!SourceSel)SPnt = SourcePoints[c % SourcePointCnt];
					else SPnt = SourcePoints[SourceSel[c % SourceSelCnt]];
					GlobSPnt = SPnt * SourceMG;//Global ORIGENAL
					Real tt = ps[c].t / (splen / Path_Speed);//ORIGENAL
					if (Path_Typ == 1) { tt = tt-LONG(tt); }
					if (tt < 0.0) {tt = 0.0;}
					if (tt <= 1.0) {
						Vector dp0 = PathOb->GetSplinePoint(PathOb->UniformToNatural(tt),la);
						if (DirectionTyp == 0){
							dp0 *= Local(PathMG,GlobSPnt,VertexNormArray[c % SourcePointCnt],LocalPath,PathLocalRot);
						}else{
							dp0 *= Local(PathMG,GlobSPnt,GlobSPnt,LocalPath,PathLocalRot);
						}
						Vector rr = dp0 - ps[c].off;
						//Real rad = Len(rr);//ist abstand
						ss[c].v += vnorm(rr)*Path_Speed;//ORIGENAL
						ss[c].count++;
					}else{
						if (Path_Typ == 2) {ps[c].bits = ps[c].bits & ~PARTICLE_VISIBLE;}
					}
				}//for (c=la; c<pcnt; c+=PathSegCnt)
			}//for (la=0; la<PathSegCnt; la++)
			PathOb->FreeLength();//???
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~ Spline Weg ~~~~~~~~~~~~~~~~~~~~~~~~
    }//if (TEMPTyp==Oparticle)
//-----------------------------------------------------------------------------



		//____________________ PP Collision Detection SD ____________________
		/*
		if (partColl){//Particle Particle Collision (zu langsam ????)
			//Real timer1 = GeGetTimer();//DEBUG
			for (o=0; o<partOverSam; o++){
				for (c=0; c<pcnt; c++){
					if (!(ps[c].bits & PARTICLE_VISIBLE)) continue;
					for (d=c+1; d<pcnt; d++){
						if (!(ps[d].bits & PARTICLE_VISIBLE)) continue;
						
						PosDif = ps[c].off-ps[d].off;
						if (PosDif*PosDif < ColRadius2){//Fast Collision
							ps[c].off = ps[d].off+vnorm(PosDif)*ColRadius;//Schlechte Reakton???
							//TEST ?
							PosDif = vnorm(PosDif);
							ss[c].v += PosDif;
							//ss[c].count++;
							ss[d].v += PosDif;
							//ss[d].count++;
							//TEST ?
						}
					
					}
				}
			}
			//print(GeGetTimer()-timer1);//DEBUG
		}
		*/
		//~~~~~~~~~~~~~~~~~~ PP Collision Detection SD~~~~~~~~~~~~~~~~~~



		//____________________ PP Collision Detection TEST ____________________
		
		if (partColl){//Particle Particle Collision (zu langsam ????)
			//Real timer1 = GeGetTimer();//DEBUG
			Real jj;
			Vector n;
			Vector di;
			for (o=0; o<partOverSam; o++){
				for (c=0; c<pcnt; c++){
					if (!(ps[c].bits & PARTICLE_VISIBLE)) continue;
					//Vector pa = ps[c].off;//<<<
					for (d=c+1; d<pcnt; d++){
						if (!(ps[d].bits & PARTICLE_VISIBLE)) continue;
						
						PosDif = ps[d].off-ps[c].off;//Position von Kugel1 - Pos Kugel2
						//PosDif = (ps[d].off+(ps[d].v3*diff)) - (ps[c].off+(ps[c].v3*diff));
						if (PosDif*PosDif <= ColRadius2){//Fast Collision (nur langsame partikel)

							n = vnorm(PosDif);//norm von c
							di = ColRadius*n-PosDif;
							ps[c].off = ps[c].off-di;
							ps[d].off = ps[d].off+di;

							//jj = (1.0*(ps[d].v3-ps[c].v3)*n)/(n*n);//Division dürch null
							jj = elast*(ps[d].v3-ps[c].v3)*n;//Work Beter (not perfekt)

							ss[c].v += ps[c].v3 + jj*n;
							ss[c].count++;

							ss[d].v += ps[d].v3 - jj*n;
							ss[d].count++;
						}
					
					}
				}
			}
			//print(GeGetTimer()-timer1);//DEBUG
		}
		//Real		ColRadius = PartRadius*2;
		//Real		ColRadius2 = ColRadius*ColRadius;
		//HELP LINK ->> http://www.gamedev.net/reference/articles/article1234.asp
		//Skalarprodukts = DotProduct = v1.x*v2.x+v1.y*v2.y+v1.z*v2.z = v1*v2 (c4d)
		//~~~~~~~~~~~~~~~~~~ PP Collision Detection TEST~~~~~~~~~~~~~~~~~~



		/*
		//COLLISION--COLLISION--COLLISION--COLLISION--COLLISION--COLLISION--COLLISION
		if (TRUE){
			//------------------- Collision FORBERATUNG---------------
			String		CollName = "coll";
			BaseObject  *ColOb = NULL;
			Vector		parPos;

			ColOb = doc->SearchObject(CollName);
			if (ColOb)
			if (ColOb->GetType()==Oparticle){
				ParticleTag	*PPPTag = PPPTag = (ParticleTag*)ColOb->GetTag(Tparticle);
				if (PPPTag){
					LONG ParticlCnt = ((ParticleObject*)ColOb)->GetParticleCount();
					Particle *Particles = ((ParticleObject*)ColOb)->GetParticle(PPPTag,0);//if (!Particles) return TRUE;
					//Fast Particle Collision !!!
					for (o=0; o<partOverSam; o++){//Oversampling
						for (c=0;c<pcnt;c++){
							if (!(ps[c].bits & (PARTICLE_VISIBLE|PARTICLE_ALIVE))) continue;
							
							for (d=0;d<ParticlCnt;d++){
								if (!(Particles[d].bits & (PARTICLE_VISIBLE|PARTICLE_ALIVE))) continue;
								
								PosDif = ps[c].off-Particles[d].off;
								if (PosDif*PosDif < ColRadius2){
									ps[c].off = Particles[d].off+vnorm(PosDif)*ColRadius;
								}

							}
						}
					}
				}
			}else{//if (ColOb->GetType()==Oparticle){
				BaseObject *SpherOb = NULL;
				Vector  ColPos;
				Real	SpherRadius,SpherRadius2;

				for (o=0; o<partOverSam; o++){//Ovrsampling
					SpherOb = ColOb->down();
					while (SpherOb){
						//ColPos = SpherOb->GetPos()*iFormML*(SpherOb->GetUpMg());//???
						ColPos = SpherOb->GetPos();
						if (SpherOb->GetType()==Oprimitive) SpherRadius = SpherOb->GetDataInstance().GetReal(PRIM_SPHERE_RAD)+ColRadius;//????? +1
						else SpherRadius = 0; //????
						SpherRadius2 = SpherRadius * SpherRadius;

						for (c=0;c<pcnt;c++){
							if (!(ps[c].bits & (PARTICLE_VISIBLE|PARTICLE_ALIVE))) continue;
							//Kugel Collision Fast !!!!
							PosDif = ps[c].off-ColPos;
							if (PosDif*PosDif < SpherRadius2) ps[c].off=ColPos+vnorm(PosDif)*SpherRadius;
						}//for (i=0;i<FormPointCnt;i++){
						SpherOb = SpherOb->next();//if (!SpherOb) break;
					}//while (SpherOb){
				}//	for (o=0; o<overSam; o++){//Ovrsampling
			}//if (ColOb->GetType()==Oparticle){
		}
		//COLLISION--COLLISION--COLLISION--COLLISION--COLLISION--COLLISION--COLLISION
		*/


		/*
		//____________________ Plane Collision ??? ___________________
		if (TRUE){
			Vector				vv;
			for (c=0; c<pcnt; c++){
				if (!(ps[c].bits & (PARTICLE_VISIBLE|PARTICLE_ALIVE))) continue;
				if (ps[c].off.y <= 0){
					ps[c].off.y = 0;
					ss[c].v.y = ss[d].v.y * -1;
					ss[c].count++;
				}

			}
		}
		//~~~~~~~~~~~~~~~~~~~~ Plane Collision ??? ~~~~~~~~~~~~~~~~~~~~
		*/


		//____________________ Gravitation _________________
		if (partGraviti){
			Vector				vv;
			for (c=0; c<pcnt; c++){
				if (!(ps[c].bits & (PARTICLE_VISIBLE|PARTICLE_ALIVE))) continue;

				vv = ps[c].v3;
				vv -= ampGraviti;
				ss[c].v += vv;
				ss[c].count++;
			}
		}
		//~~~~~~~~~~~~~~~~~~ Gravitation ~~~~~~~~~~~~~~~~~~
		


		//____________________ Reibung ??? _________________
		/*
		// FORMEL ???
		if (partReibug){

			//print(visDrag);
			//print(Len(ps[0].v3));

			Vector	vv;
			for (c=0; c<pcnt; c++){
				if (!(ps[c].bits & (PARTICLE_VISIBLE|PARTICLE_ALIVE))) continue;

				vv = ps[c].v3; //print(Len(vv));
				vv -= (vv * 0.025);//print(Len(vv));
				ss[c].v += vv;
				ss[c].count++;
			}
		}
		*/
		//~~~~~~~~~~~~~~~~~~ Reibung ??? ~~~~~~~~~~~~~~~~~~


//---------SET DATA---------------------
drnd = rnd2->Get01()*10000;//RND2 TEST
data.SetLong(TDEM_DRND,drnd);
data.SetLong(TDEM_REVAP,revaP);
op->SetData(data);


Error://-------------ERROR--------------------

if (modcom) blDelete(srcOb);//!!!!!???

gDelete(rnd);
gDelete(rnd2);

GeFree(VertexNormArray);//VERTEX NORMAL Array
GeFree(VertexColorArray);//Vertex Color Array

GeFree(SourceSel);
GeFree(DestSel);

}//Mod Partikel



//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< MenuPrepare >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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
		}else{
			//Wird Nicht Actuallisiert.
			/*
			child = AllocObject(Oparticle);
			if (child){
				//Set Emiter Data
				BaseContainer emdata = child->GetDataInstance();
				emdata.SetLong(PARTICLEOBJECT_BIRTHEDITOR, 100);
				emdata.SetLong(PARTICLEOBJECT_BIRTHRAYTRACER, 100);
				emdata.SetReal(PARTICLEOBJECT_SIZEX,0.0);
				emdata.SetReal(PARTICLEOBJECT_SIZEY,0.0);

				//child->InsertUnder(op);
				doc->InsertObject(child,op,NULL);
				//op->Message(MSG_UPDATE);
				//doc->Message(MSG_UPDATE);
				GeEventAdd(MSG_DOCUMENTCHANGED);
			}	
			*/
		}
	}

	return TRUE;
}

#if 0
//VAR
static BaseBitmap *icon=NULL;
static BaseBitmap *icon_small=NULL;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<< RegisterTDEmitter >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Bool RegisterTDEmitter(void)
{
	// decide by name if the plugin shall be registered - just for user convenience
	String name=GeLoadString(IDS_TDEM); if (!name.Content()) {GePrint("TDEM: name return!");return TRUE;}

	icon = AllocBaseBitmap();
	if (!icon || icon->Init(GeGetPluginPath()+String("res")+String("em1icon.tif"))!=IMAGE_OK) {GePrint("TDEM: em1icon.tif is not loaded!");return FALSE;}
	icon_small = AllocBaseBitmap();
	if (!icon_small || icon_small->Init(GeGetPluginPath()+String("res")+String("em1iconsmal.tif"))!=IMAGE_OK) {GePrint("TDEM: em1iconsmal.tif is not loaded!");return FALSE;}

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
	cs.Draw						= Draw;
	//cs.DrawBox				= Draw; // draw modifier in box mode also
	//cs.MoveHandle			= MoveHandle;
	
	// build description for parameter animation
	cs.GetDescription	= GetDescription;

	// be sure to use a unique ID obtained from www.plugincafe.com
	return GeRegisterPlugin(C4DPL_OBJECT,PLUGINS_ID,name,&cs,sizeof(cs));

}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  FreeTDEmitter  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void FreeTDEmitter(void)
{
	FreeBaseBitmap(icon);
	FreeBaseBitmap(icon_small);
}

#endif

/*
    int ClassifyPoint( CPlane3D *plane, CPoint3D *destPt )
  {
        scalar p = DotProduct( plane->normal, destPt ) + plane->distance;

        if( p > 0.0f ) return PLANE_FRONT;
        else
        if( p < 0.0f ) return PLANE_BACK;

        return PLANE_COINCIDE;
  }
*/