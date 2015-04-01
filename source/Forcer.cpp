/////////////////////////////////////////////////////////////
// FORMEL PARTICLE MODIFIKATOR							   //
/////////////////////////////////////////////////////////////
// DATE   13.04.2002 										//
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
#include "c4d_basebitmap.h"
#include "c4d_basedraw.h"
#include "c4d_resource.h"
#include "c4d_symbols.h"
#include "c4d_particleobject.h"
#include "c4d_objectplugin.h"
#include "c4d_gui.h"

#include "myprint.h"
#include "mytools.h"

//--------------------------
#define PLUGINS_ID  1008112
//--------------------------

//Glob Var_
#define FCR_VER			2000

#define FCR_VELO_X		1001
#define FCR_VELO_Y		1002
#define FCR_VELO_Z		1003
#define FCR_VELO		1004

#define FCR_POS_X		1011
#define FCR_POS_Y		1012
#define FCR_POS_Z		1013
#define FCR_POS			1014

#define FCR_EM1			1021
#define FCR_NEM1		1022
#define FCR_EMITTER		1023

#define FCR_SIZE		1031 //Vector
#define FCR_FORM		1032 //LONG
#define FCR_RAD			1033 //Percent
#define FCR_INVERT		1034 //Percent

//Variablen ???
#define FCR_VAR1		1041 //Bool
#define FCR_VAR2		1042 //Bool
#define FCR_VAR3		1043 //Bool
#define FCR_VAR4		1044 //Bool
#define FCR_VAR5		1045 //Bool
#define FCR_VAR6		1046 //Bool
#define FCR_VAR7		1047 //Bool
#define FCR_VAR8		1048 //Bool
#define FCR_VAR9		1049 //Bool
//----

#define HANDLES			3 //Handles Count
//-----------------------------------------------------------------------



//------------------- DONT WORK KORREKT ??-------------
Bool TestEvalStr(const String str,BaseContainer	data)
{
	Bool			res = FALSE;
	//--------- VAR
	Bool			var1 = data.GetBool(FCR_VAR1);
	Bool			var2 = data.GetBool(FCR_VAR2);
	Bool			var3 = data.GetBool(FCR_VAR3);
	Bool			var4 = data.GetBool(FCR_VAR4);
	Bool			var5 = data.GetBool(FCR_VAR5);
	Bool			var6 = data.GetBool(FCR_VAR6);
	Bool			var7 = data.GetBool(FCR_VAR7);
	Bool			var8 = data.GetBool(FCR_VAR8);
	Bool			var9 = data.GetBool(FCR_VAR9);
	//--------- VAR

	//parser
	Parser			*parser = AllocParser();//ALLOC PARESER
	Real			px,py,pz,vx,vy,vz,sx,sy,sz,u,v,w;
	Real			rc,lt,rnd,diff,time;
	Real			ant = 0.0;
	LONG			err = 0;//ERRORS ???

	//PARSER ADD VARIABLE 
	if (var1){// VAR 1
		parser->AddVar("vx",&vx);//Particle Velociti
		parser->AddVar("vy",&vy);
		parser->AddVar("vz",&vz);
	}
	if (var2){// VAR 2
		parser->AddVar("px",&px);//Particle Position
		parser->AddVar("py",&py);
		parser->AddVar("pz",&pz);
	}
	if (var3){// VAR 3
		parser->AddVar("sx",&sx);//Position
		parser->AddVar("sy",&sy);
		parser->AddVar("sz",&sz);
	}
	if (var4){ parser->AddVar("n",&rc); }//Particle Number VAR 4
	if (var5){ parser->AddVar("lt",&lt); }//Particle Life Time VAR 5
	if (var6){ parser->AddVar("rnd",&rnd); }//Random VAR 6
	if (var7){ parser->AddVar("diff",&diff); }//diff VAR 7
	if (var8){ parser->AddVar("t",&time); }//Document Time VAR 8

	if (var9){// VAR 9
		parser->AddVar("u",&u);//Position
		parser->AddVar("v",&v);
		parser->AddVar("w",&w);
	}

	res = parser->Eval(str,&err,&ant,UNIT_M,ANGLE_RAD,10);
	print(ant);

	FreeParser(parser);//FREE PARSER
return res;
}



//############################################
class FCRDialog : public GeModalDialog
{
	private:
		BaseContainer *data;
	public:
		FCRDialog(BaseContainer *bc) { data = bc; }

		virtual Bool CreateLayout(void);
		virtual Bool InitValues(void);
		virtual Bool Command(LONG id, const BaseContainer &msg);
		//virtual LONG Message(const BaseContainer &msg, BaseContainer &result);
		//virtual Bool AskClose();
};

Bool FCRDialog::CreateLayout()
{
  return GeModalDialog::CreateLayout() && LoadDialogResource(DLG_FCR, NULL, 0);
}

Bool FCRDialog::InitValues()
{
  if (!GeModalDialog::InitValues()) return FALSE;

  Bool bo;
  String st = "";
  SetBool(IDC_FCR_EMITTER,data,FCR_EMITTER);
  SetString(IDC_FCR_EM1,data,FCR_EM1);
  SetLong(IDC_FCR_NEM1,data,FCR_NEM1,0,1000,1);

  SetString(IDC_FCR_VELO_X,data,FCR_VELO_X);
  SetString(IDC_FCR_VELO_Y,data,FCR_VELO_Y);
  SetString(IDC_FCR_VELO_Z,data,FCR_VELO_Z);
  SetBool(IDC_FCR_VELO,data,FCR_VELO);

  SetString(IDC_FCR_POS_X,data,FCR_POS_X);
  SetString(IDC_FCR_POS_Y,data,FCR_POS_Y);
  SetString(IDC_FCR_POS_Z,data,FCR_POS_Z);
  SetBool(IDC_FCR_POS,data,FCR_POS);

  //---------
  SetBool(IDC_FCR_VAR1,data,FCR_VAR1);
  SetBool(IDC_FCR_VAR2,data,FCR_VAR2);
  SetBool(IDC_FCR_VAR3,data,FCR_VAR3);
  SetBool(IDC_FCR_VAR4,data,FCR_VAR4);
  SetBool(IDC_FCR_VAR5,data,FCR_VAR5);
  SetBool(IDC_FCR_VAR6,data,FCR_VAR6);
  SetBool(IDC_FCR_VAR7,data,FCR_VAR7);
  SetBool(IDC_FCR_VAR8,data,FCR_VAR8);
  SetBool(IDC_FCR_VAR9,data,FCR_VAR9);
  //---------

  SetLong(IDC_FCR_FORM,data,FCR_FORM);
  SetVectorMeter(IDC_FCR_SIZE_X,IDC_FCR_SIZE_Y,IDC_FCR_SIZE_Z,data,FCR_SIZE,0.0,MAXRANGE,1.0);
  SetPercent(IDC_FCR_RAD,data,FCR_RAD,0.0,100.0,1.0);
  SetBool(IDC_FCR_INVERT,data,FCR_INVERT);

  GetBool(IDC_FCR_EMITTER,bo);
  Enable(IDC_FCR_EM1,bo);
  Enable(IDC_FCR_NEM1,bo);
  GetString(IDC_FCR_EM1,st);
  Enable(IDC_FCR_NEM1,(st != ""));

  GetBool(IDC_FCR_VELO,bo);
  Enable(IDC_FCR_VELO_X,bo);
  Enable(IDC_FCR_VELO_Y,bo);
  Enable(IDC_FCR_VELO_Z,bo);

  GetBool(IDC_FCR_POS,bo);
  Enable(IDC_FCR_POS_X,bo);
  Enable(IDC_FCR_POS_Y,bo);
  Enable(IDC_FCR_POS_Z,bo);


  return TRUE;
}

Bool FCRDialog::Command(LONG id, const BaseContainer &msg)
{
	Bool bo;
	String st = "";
	switch (id)
	{
		case IDC_OK:
			GetBool(IDC_FCR_EMITTER,data,FCR_EMITTER);
			GetString(IDC_FCR_EM1,data,FCR_EM1);
			GetLong(IDC_FCR_NEM1,data,FCR_NEM1);

			GetString(IDC_FCR_VELO_X,data,FCR_VELO_X);
			GetString(IDC_FCR_VELO_Y,data,FCR_VELO_Y);
			GetString(IDC_FCR_VELO_Z,data,FCR_VELO_Z);
			GetBool(IDC_FCR_VELO,data,FCR_VELO);

			GetString(IDC_FCR_POS_X,data,FCR_POS_X);
			GetString(IDC_FCR_POS_Y,data,FCR_POS_Y);
			GetString(IDC_FCR_POS_Z,data,FCR_POS_Z);
			GetBool(IDC_FCR_POS,data,FCR_POS);

			//---------
			GetBool(IDC_FCR_VAR1,data,FCR_VAR1);
			GetBool(IDC_FCR_VAR2,data,FCR_VAR2);
			GetBool(IDC_FCR_VAR3,data,FCR_VAR3);
			GetBool(IDC_FCR_VAR4,data,FCR_VAR4);
			GetBool(IDC_FCR_VAR5,data,FCR_VAR5);
			GetBool(IDC_FCR_VAR6,data,FCR_VAR6);
			GetBool(IDC_FCR_VAR7,data,FCR_VAR7);
			GetBool(IDC_FCR_VAR8,data,FCR_VAR8);
			GetBool(IDC_FCR_VAR9,data,FCR_VAR9);
			//---------

			GetLong(IDC_FCR_FORM,data,FCR_FORM);
			GetVector(IDC_FCR_SIZE_X,IDC_FCR_SIZE_Y,IDC_FCR_SIZE_Z,data,FCR_SIZE);
			GetReal(IDC_FCR_RAD,data,FCR_RAD);
			GetBool(IDC_FCR_INVERT,data,FCR_INVERT);
		break;

		case IDC_FCR_EMITTER:
			GetBool(IDC_FCR_EMITTER,bo);
			Enable(IDC_FCR_EM1,bo);
			Enable(IDC_FCR_NEM1,bo);
		//break;

		case IDC_FCR_EM1:
			GetString(IDC_FCR_EM1,st);
			Enable(IDC_FCR_NEM1,(st != ""));
		break;

		case IDC_FCR_VELO:
			GetBool(IDC_FCR_VELO,bo);

			Enable(IDC_FCR_VELO_X,bo);
			Enable(IDC_FCR_VELO_Y,bo);
			Enable(IDC_FCR_VELO_Z,bo);

			//SendMessage(IDC_FCR_VELO,BFM_VISIBLE_OFF);
			//SendMessage(IDC_FCR_VELO,BFM_ADJUSTSIZE);
			//SendMessage(IDC_FCR_VELO,BFM_DRAW);
		break;

		case IDC_FCR_POS:
			GetBool(IDC_FCR_POS,bo);
			Enable(IDC_FCR_POS_X,bo);
			Enable(IDC_FCR_POS_Y,bo);
			Enable(IDC_FCR_POS_Z,bo);
		break;

		/*
		//----------------
		case IDC_FCR_VELO_X:
		case IDC_FCR_VELO_Y:
		case IDC_FCR_VELO_Z:
		case IDC_FCR_POS_X:
		case IDC_FCR_POS_Y:
		case IDC_FCR_POS_Z:

			//--------- ???
			GetBool(IDC_FCR_VAR1,data,FCR_VAR1);
			GetBool(IDC_FCR_VAR2,data,FCR_VAR2);
			GetBool(IDC_FCR_VAR3,data,FCR_VAR3);
			GetBool(IDC_FCR_VAR4,data,FCR_VAR4);
			GetBool(IDC_FCR_VAR5,data,FCR_VAR5);
			GetBool(IDC_FCR_VAR6,data,FCR_VAR6);
			GetBool(IDC_FCR_VAR7,data,FCR_VAR7);
			GetBool(IDC_FCR_VAR8,data,FCR_VAR8);
			GetBool(IDC_FCR_VAR9,data,FCR_VAR9);
			//---------  ???

			GetString(id,st);//???
			if (TestEvalStr(st,*data)){
				SetString(IDC_FCR_EM1,"ok");
			}else{
				SetString(IDC_FCR_EM1,"error");
			}
		break;//<<<
		*/

	}
	return TRUE;
}

/*
LONG FCRDialog::Message(const BaseContainer &msg, BaseContainer &result)
{
  switch (msg.GetId())
  {
  default:
    break;
  }
  return GeModalDialog::Message(msg, result);
}

Bool FCRDialog::AskClose()
{
  // return TRUE, if the dialog should not be closed
  return FALSE;
}
*/

static Bool EditSettings(PluginObject *op)
{
	// copy data
	BaseContainer data = op->GetData();
	FCRDialog dlg(&data);
	if (!dlg.Open()) return FALSE;
	// copy data back if OK was pressed
	op->SetData(data);
	return TRUE;
}

// initialize settings
static Bool AllocSettings(PluginObject *op)
{	
	BaseContainer data;

	data.SetBool(FCR_EMITTER,FALSE);
	data.SetString(FCR_EM1,"");
	data.SetLong(FCR_NEM1,0);

	data.SetString(FCR_VELO_X,"vx");
	data.SetString(FCR_VELO_Y,"vy");
	data.SetString(FCR_VELO_Z,"vz");
	data.SetBool(FCR_VELO,TRUE);

	data.SetString(FCR_POS_X,"px");
	data.SetString(FCR_POS_Y,"py");
	data.SetString(FCR_POS_Z,"pz");
	data.SetBool(FCR_POS,FALSE);

	//---------
	data.SetBool(FCR_VAR1,TRUE);
	data.SetBool(FCR_VAR2,FALSE);
	data.SetBool(FCR_VAR3,FALSE);
	data.SetBool(FCR_VAR4,TRUE);
	data.SetBool(FCR_VAR5,TRUE);
	data.SetBool(FCR_VAR6,TRUE);
	data.SetBool(FCR_VAR7,TRUE);
	data.SetBool(FCR_VAR8,TRUE);
	data.SetBool(FCR_VAR9,FALSE);
	//---------

	data.SetLong(FCR_FORM,0);
	data.SetVector(FCR_SIZE,100);
	data.SetReal(FCR_RAD,0.5);
	data.SetBool(FCR_INVERT,FALSE);

	op->SetData(data);
	return TRUE;
}

// build description for parameter animation
static LONG GetDescription(PluginObject *op, LONG id)
{
	switch (id)
	{
		/*
		// these parameters can be scaled by the model tool and animated
		case FCR_VELO_X: 
		case FCR_VELO_Y:
		case FCR_VELO_Z: 
		return PARAM_TOGGLE; 
		*/
		
		
		// this parameter can be animated
		case FCR_SIZE: return PARAM_LINEAR;
		case FCR_RAD: return PARAM_LINEAR;
		case FCR_NEM1: return PARAM_LINEAR;

		case FCR_VER: return PARAM_SOLID;
	}

	return PARAM_TOGGLE;
}

//-----------------------------------------------------------------------
static void GetDimension(PluginObject *op, Vector *mp, Vector *rad)
{
	BaseContainer data = op->GetDataInstance(); 
	LONG form =   data.GetLong(FCR_FORM);

	if (form > 0){
		*rad=data.GetVector(FCR_SIZE);
	}else{
		*rad=Vector(0.0,0.0,0.0);
	}
}

//-----------------------------------------------------------------------
static Bool Draw(PluginObject *op, BaseDraw *bd, BaseDrawHelp *bh)
{
	BaseContainer	data = op->GetData();//GET SETING
	Vector	rad = data.GetVector(FCR_SIZE);
	LONG	form =   data.GetLong(FCR_FORM);
	Real	ri = data.GetReal(FCR_RAD); //Torus Tube Radius

	Vector h;
	Vector rirad;
	Matrix m,mg = bh->GetMg();
	bd->SetPen(bd->GetObjectColor(op));
	Vector p[8],p9,p10,p11,p12,p13;
	Real segl = 20;

	p9  = mg.off;
	//DRAW  > F < 
	p10 = p9  + Vector( 0.0,-segl, 0.0);
	p11 = p9  + Vector( 0.0,  segl, 0.0);
	p12 = p9  + Vector( segl,  0.0, 0.0);
	p13 = p11  + Vector( segl,  0.0, 0.0);
	bd->Line3D(p10,p11);
	bd->Line3D(p9,p12);
	bd->Line3D(p11,p13);


	switch(form)
	{
	case 1://DRAW Cube
		p[0] = Vector(-rad.x,-rad.y,-rad.z)*mg;
		p[1] = Vector( rad.x,-rad.y,-rad.z)*mg;
		p[2] = Vector( rad.x,-rad.y, rad.z)*mg;
		p[3] = Vector(-rad.x,-rad.y, rad.z)*mg;
	
		p[4] = Vector(-rad.x, rad.y,-rad.z)*mg;
		p[5] = Vector( rad.x, rad.y,-rad.z)*mg;
		p[6] = Vector( rad.x, rad.y, rad.z)*mg;
		p[7] = Vector(-rad.x, rad.y, rad.z)*mg;

		bd->Line3D(p[0],p[1]); bd->Line3D(p[1],p[2]); bd->Line3D(p[2],p[3]); bd->Line3D(p[3],p[0]);
		bd->Line3D(p[4],p[5]); bd->Line3D(p[5],p[6]); bd->Line3D(p[6],p[7]); bd->Line3D(p[7],p[4]);
		bd->Line3D(p[0],p[4]); bd->Line3D(p[1],p[5]); bd->Line3D(p[2],p[6]); bd->Line3D(p[3],p[7]);
	break;
	//--------------------
	case 2://DRAW Ellipse
		mg.v1*=rad.x;
		mg.v2*=rad.y;
		mg.v3*=rad.z;

		bd->Circle3D(mg);
		h=mg.v2; mg.v2=mg.v3; mg.v3=h;
		bd->Circle3D(mg);
		h=mg.v1; mg.v1=mg.v3; mg.v3=h;
		bd->Circle3D(mg);
	break;
	//--------------------
	case 3://DRAW Zylinder

		//LINIEN OK
		p[0] = Vector(-rad.x,-rad.y,   0.0)*mg;
		p[1] = Vector( rad.x,-rad.y,   0.0)*mg;
		p[2] = Vector(   0.0,-rad.y,-rad.z)*mg;
		p[3] = Vector(   0.0,-rad.y, rad.z)*mg;
		p[4] = Vector(-rad.x, rad.y,   0.0)*mg;
		p[5] = Vector( rad.x, rad.y,   0.0)*mg;
		p[6] = Vector(   0.0, rad.y,-rad.z)*mg;
		p[7] = Vector(   0.0, rad.y, rad.z)*mg;
		bd->Line3D(p[0],p[4]); bd->Line3D(p[1],p[5]); bd->Line3D(p[2],p[6]); bd->Line3D(p[3],p[7]);

		//Elipsen OK
		m = mg;
		m.v1*=rad.x;
		m.v2*=rad.y;
		m.v3*=rad.z;
		h=m.v2; m.v2=m.v3; m.v3=h;

		m.off = Vector(0.0,rad.y,0.0)*mg;
		bd->Circle3D(m);

		m.off = Vector(0.0,-rad.y,0.0)*mg;
		bd->Circle3D(m);
	break;
	//--------------------
	case 4://DRAW Kegel

		//LINIEN OK
		/*
		p[0] = Vector(-rad.x,-rad.y,   0.0)*mg;
		p[1] = Vector( rad.x,-rad.y,   0.0)*mg;
		p[2] = Vector(   0.0,-rad.y,-rad.z)*mg;
		p[3] = Vector(   0.0,-rad.y, rad.z)*mg;
		*/

		p[0] = Vector(   0.0,-rad.y,   0.0)*mg;
		p[1] = Vector(   0.0,-rad.y,   0.0)*mg;
		p[2] = Vector(   0.0,-rad.y,   0.0)*mg;
		p[3] = Vector(   0.0,-rad.y,   0.0)*mg;
		
		p[4] = Vector(-rad.x, rad.y,   0.0)*mg;
		p[5] = Vector( rad.x, rad.y,   0.0)*mg;
		p[6] = Vector(   0.0, rad.y,-rad.z)*mg;
		p[7] = Vector(   0.0, rad.y, rad.z)*mg;
		
		bd->Line3D(p[0],p[4]); bd->Line3D(p[1],p[5]); bd->Line3D(p[2],p[6]); bd->Line3D(p[3],p[7]);

		//Elipsen OK
		m = mg;
		m.v1*=rad.x;
		m.v2*=rad.y;
		m.v3*=rad.z;
		h=m.v2; m.v2=m.v3; m.v3=h;

		m.off = Vector(0.0,rad.y,0.0)*mg;
		bd->Circle3D(m);

		//m.off = Vector(0.0,-rad.y,0.0)*mg;
		//bd->Circle3D(m);
	break;
	//--------------------
	case 5://DRAW Torus OK

		//Elipsen
		m = mg;
		m.v1*=rad.x;
		m.v2*=rad.y;
		m.v3*=rad.z;
		h=m.v2; m.v2=m.v3; m.v3=h;
		bd->Circle3D(m);//out

		m = mg;
		m.v1*=(rad.x-(ri*rad.x));
		m.v2*=rad.y;
		m.v3*=(rad.z-(ri*rad.z));
		h=m.v2; m.v2=m.v3; m.v3=h;
		bd->Circle3D(m);//IN ???

		m = mg;
		m.v1*=(rad.x-(ri*rad.x)/2);
		m.v2*=rad.y;
		m.v3*=(rad.z-(ri*rad.z)/2);
		h=m.v2; m.v2=m.v3; m.v3=h;
		
		m.off = Vector(0.0,rad.y,0.0)*mg;//Oben ??
		bd->Circle3D(m);
		m.off = Vector(0.0,-rad.y,0.0)*mg;//Unten ??
		bd->Circle3D(m);

		//----------------------
		m = mg;
		m.v1*=((ri*rad.x)/2);
		m.v2*=rad.y;
		m.v3*=((ri*rad.z)/2);

		m.off = Vector(0.0,0.0,(rad.z-(ri*rad.z)/2))*mg;
		bd->Circle3D(m);
		h=m.v1; m.v1=m.v3; m.v3=h;
		bd->Circle3D(m);

		h=m.v1; m.v1=m.v3; m.v3=h;
		m.off = Vector(0.0,0.0,-(rad.z-(ri*rad.z)/2))*mg;
		bd->Circle3D(m);

		h=m.v1; m.v1=m.v3; m.v3=h;
		bd->Circle3D(m);


		//-------------------
		h=m.v1; m.v1=m.v3; m.v3=h;

		m.off = Vector((rad.x-(ri*rad.x)/2),0.0,0.0)*mg;
		bd->Circle3D(m);
		h=m.v1; m.v1=m.v3; m.v3=h;
		bd->Circle3D(m);

		h=m.v1; m.v1=m.v3; m.v3=h;
		m.off = Vector(-(rad.x-(ri*rad.x)/2),0.0,0.0)*mg;
		bd->Circle3D(m);
		h=m.v1; m.v1=m.v3; m.v3=h;
		bd->Circle3D(m);

	break;
	//--------------------
	case 6://DRAW Tube
		//LINIEN
		p[0] = Vector(-rad.x,-rad.y,   0.0)*mg;
		p[1] = Vector( rad.x,-rad.y,   0.0)*mg;
		p[2] = Vector(   0.0,-rad.y,-rad.z)*mg;
		p[3] = Vector(   0.0,-rad.y, rad.z)*mg;
		p[4] = Vector(-rad.x, rad.y,   0.0)*mg;
		p[5] = Vector( rad.x, rad.y,   0.0)*mg;
		p[6] = Vector(   0.0, rad.y,-rad.z)*mg;
		p[7] = Vector(   0.0, rad.y, rad.z)*mg;
		bd->Line3D(p[0],p[4]); bd->Line3D(p[1],p[5]); bd->Line3D(p[2],p[6]); bd->Line3D(p[3],p[7]);

		//Elipsen OK
		m = mg;
		m.v1*=rad.x;
		m.v2*=rad.y;
		m.v3*=rad.z;
		h=m.v2; m.v2=m.v3; m.v3=h;

		m.off = Vector(0.0,rad.y,0.0)*mg;
		bd->Circle3D(m);

		m.off = Vector(0.0,-rad.y,0.0)*mg;
		bd->Circle3D(m);

		m = mg;
		m.v1*=rad.x*ri;
		m.v2*=rad.y;
		m.v3*=rad.z*ri;
		h=m.v2; m.v2=m.v3; m.v3=h;

		m.off = Vector(0.0,rad.y,0.0)*mg;
		bd->Circle3D(m);

		m.off = Vector(0.0,-rad.y,0.0)*mg;
		bd->Circle3D(m);
	break;

	}//	switch(form)

	//Kegel
	//Torus

	return TRUE;
}

//HANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLES
//HANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLES
//HANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLES
//#############################	GetHandle #############################

static Vector GetHandle(PluginObject *op, LONG id)
{
	BaseContainer data = op->GetDataInstance();

	Vector		p;
	Vector		rad = data.GetVector(FCR_SIZE);

	switch (id)
	{
		case 0: p=Vector(rad.x,0.0,0.0); break;
		case 1: p=Vector(0.0,rad.y,0.0); break;
		case 2: p=Vector(0.0,0.0,rad.z); break;
	}

	return p;
}

//#############################	DrawHandles #############################
static Bool DrawHandles(PluginObject *op, BaseDraw *bd, BaseDrawHelp *bh)
{
	if (!op->GetBit(BIT_ACTIVE)) return TRUE;

	BaseContainer data = op->GetDataInstance();

	LONG		c;
	Matrix		m   = bh->GetMg();
	Vector		rad = data.GetVector(FCR_SIZE);
	LONG		form = data.GetLong(FCR_FORM);

	bd->SetPen(GetWorldColor(COLOR_ACTIVEPOINT));

	if (form > 0){
		for (c=0; c<HANDLES; c++) bd->Handle3D(GetHandle(op,c)*m,HANDLE_BIG);
	}

	return TRUE;
}
//#############################	DetectHandle #############################
static LONG DetectHandle(PluginObject *op, BaseDraw *bd, LONG x, LONG y, LONG qualifier)
{
	if (qualifier&QUALIFIER_CTRL) return NOTOK;

	Matrix	mg = op->GetMg();
	LONG    c,ret=NOTOK;
	Vector	p;

	for (c=0; c<HANDLES; c++)
	{
		p = GetHandle(op,c);
		if (bd->PointInRange(p*mg,x,y)) 
		{
			ret=c;
			if (!(qualifier&QUALIFIER_SHIFT)) break;
		}
	}

	return ret;
}
//#############################	MoveHandle #############################
static Bool	MoveHandle(PluginObject *op, PluginObject *undo, const Matrix &tm, LONG hit_id, LONG qualifier)
{
	BaseContainer src = undo->GetDataInstance();
	BaseContainer dst = op  ->GetDataInstance();
	
	Real val = tm.off.x;//???
	Vector rad = src.GetVector(FCR_SIZE);

	switch (hit_id)
	{
		case 0: 
			val+=rad.x+val;
			dst.SetVector(FCR_SIZE,Vector(Cut(val,0.0,MAXRANGE),rad.y,rad.z)); 
			break;

		case 1: 
			val+=rad.y+val;
			dst.SetVector(FCR_SIZE,Vector(rad.x,Cut(val,0.0,MAXRANGE),rad.z)); 
			break;

		case 2: 
			val+=rad.z+val;
			dst.SetVector(FCR_SIZE,Vector(rad.x,rad.y,Cut(val,0.0,MAXRANGE))); 
			break;
		
	}
	return TRUE;
}

//HANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLES
//HANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLES
//HANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLES



static void ModifyParticles(PluginObject *op, Particle *ps, BaseParticle *ss, LONG pcnt, Real diff)
{
    if (!op->GetDeformMode()) return;//Return wenn Object deaktiwiert ist

	BaseContainer	data = op->GetData();//GET SETING

	BaseDocument	*doc = op->GetDocument(); if (!doc) return;
	LONG			fps = doc->GetFps();//Get Doc FPS
	BaseTime		BTtime = doc->GetTime();
	Real			time=BTtime.Get();
	LONG			c;
	Vector			pp,vv,gpp;
	Matrix			mg=op->GetMg(), img=!mg;

	//-----------------------------------------------------
	Bool			useemi = data.GetBool(FCR_EMITTER);
	ParticleObject	*EmiterOb = NULL;//Emiter zum Modifizieren
	Particle		*EmParticle = NULL;
	ParticleTag		*EmPTag = NULL;
	//EMITTER Search
	if (useemi){
		String  EmName = GetNumName(data,FCR_EM1,FCR_NEM1);
		EmiterOb = (ParticleObject*)doc->SearchObject(EmName);
		if (!EmiterOb){ EmiterOb = (ParticleObject*)op->down();}
		//if (EmiterOb->GetType()!=Oparticle)
		if (EmiterOb) {
			EmPTag = (ParticleTag*)EmiterOb->GetTag(Tparticle);
			if (EmPTag) {
				EmParticle = EmiterOb->GetParticle(EmPTag,0);
				if (EmParticle != ps) return;//Ob die particles EmiterOb gehören
			}// if (EmPTag) {
		}// if (EmiterOb) {
	}//	if (useemi){
	//-----------------------------------------------------

	//--------- VAR
	Bool			var1 = data.GetBool(FCR_VAR1);
	Bool			var2 = data.GetBool(FCR_VAR2);
	Bool			var3 = data.GetBool(FCR_VAR3);
	Bool			var4 = data.GetBool(FCR_VAR4);
	Bool			var5 = data.GetBool(FCR_VAR5);
	Bool			var6 = data.GetBool(FCR_VAR6);
	Bool			var7 = data.GetBool(FCR_VAR7);
	Bool			var8 = data.GetBool(FCR_VAR8);
	Bool			var9 = data.GetBool(FCR_VAR9);
	//--------- VAR

	//IT WORK !!!
	LONG			form = data.GetLong(FCR_FORM);//COLL FORM
	Bool			invert = data.GetBool(FCR_INVERT);
	Vector			rad = data.GetVector(FCR_SIZE);//COLL SIZE
	Real			ri = data.GetReal(FCR_RAD); // r
	Real			ri2 = ri*ri; //Torus Tube Radius

	Real			ro = 1.0; // R
	Real			ro2 = ro*ro; //Torus Radius ???

	Real			aa = (1.0-ri)*0.5;
	//Real			cc = (1.0+ri)*0.5 + aa;
	Real			cc = ri + aa;
	aa = aa*aa;
	cc = cc*cc;

	//Real			tri2 = (ri+((1.0-ri)/2.0)) * (ri+((1.0-ri)/2.0));

	Matrix tm;
	tm.v1*=rad.x;
	tm.v2*=rad.y;
	tm.v3*=rad.z;
	Matrix itm = !tm;
	//IT WORK !!!

	String			eva_velo_x = data.GetString(FCR_VELO_X);
	String			eva_velo_y = data.GetString(FCR_VELO_Y);
	String			eva_velo_z = data.GetString(FCR_VELO_Z);
	Bool			eva_velo = data.GetBool(FCR_VELO);

	String			eva_pos_x = data.GetString(FCR_POS_X);
	String			eva_pos_y = data.GetString(FCR_POS_Y);
	String			eva_pos_z = data.GetString(FCR_POS_Z);
	Bool			eva_pos = data.GetBool(FCR_POS);

	//parser
	Parser			*parser = AllocParser();//ALLOC PARESER
	//parser variable
	Real			px,py,pz,vx,vy,vz,u,v,w;
	Real			rc,lt;//"c"
	Real			rnd;
	Real			res_velo_x,res_velo_y,res_velo_z;//RESULT
	Real			res_pos_x,res_pos_y,res_pos_z;//RESULT
	LONG			err = 0;//ERRORS ???
	Vector			spos = op->GetPos();//POSITION
	Real			sx = spos.x;		
	Real			sy = spos.y;
	Real			sz = spos.z;
	//------

	Random *rndGer = gNew(Random);//SD
	rndGer->Init(4567);//SD

	//PARSER ADD VARIABLE 
	if (var1){// VAR 1
		parser->AddVar("vx",&vx);//Particle Velociti
		parser->AddVar("vy",&vy);
		parser->AddVar("vz",&vz);
	}
	if (var2){// VAR 2
		parser->AddVar("px",&px);//Particle Position
		parser->AddVar("py",&py);
		parser->AddVar("pz",&pz);
	}
	if (var3){// VAR 3
		parser->AddVar("sx",&sx);//Position
		parser->AddVar("sy",&sy);
		parser->AddVar("sz",&sz);
	}
	if (var4){ parser->AddVar("n",&rc); }//Particle Number VAR 4
	if (var5){ parser->AddVar("lt",&lt); }//Particle Life Time VAR 5
	if (var6){ parser->AddVar("rnd",&rnd); }//Random VAR 6
	if (var7){ parser->AddVar("diff",&diff); }//diff VAR 7
	if (var8){ parser->AddVar("t",&time); }//Document Time VAR 8

	if (var9){// VAR 9
		parser->AddVar("u",&u);//Position
		parser->AddVar("v",&v);
		parser->AddVar("w",&w);
	}

	//------------------------------
	//OPTIMIEREN ???? !!!!!
	for (c=0; c<pcnt; c++){
		if (!(ps[c].bits & (PARTICLE_VISIBLE|PARTICLE_ALIVE))) continue;
		gpp = ps[c].off; // Globale Particle Position
		pp = ps[c].off * img * itm;//locale position
		vv = ps[c].v3;//velocity

		switch(form)//TEST ?????
		{
		case 1://Cube OK
			if(invert) {if(pp.x>=-1.0 && pp.x<=1.0 && pp.y>=-1.0 && pp.y<=1.0 && pp.z>=-1.0&& pp.z<=1.0) continue;}
			else {if(!(pp.x>=-1.0 && pp.x<=1.0 && pp.y>=-1.0 && pp.y<=1.0 && pp.z>=-1.0&& pp.z<=1.0)) continue;}
		break;
		case 2://Ellipse OK
			if(invert) {if(pp*pp <= 1.0) continue;}
			else {if(!(pp*pp <= 1.0)) continue;}
		break;
		case 3://Zylinder OK
			if(invert) {if((pp.x*pp.x+pp.z*pp.z) <= 1.0 && pp.y>=-1.0 && pp.y<=1.0) continue;}
			else {if (!((pp.x*pp.x+pp.z*pp.z) <= 1.0 && pp.y>=-1.0 && pp.y<=1.0)) continue;}
		break;
		case 4://Kegel OK
			if(invert) {if((pp.x*pp.x+pp.z*pp.z) <= ((pp.y+1) * (pp.y+1) / 4) && pp.y>=-1.0 && pp.y<=1.0) continue;}
			else {if(!((pp.x*pp.x+pp.z*pp.z) <= ((pp.y+1) * (pp.y+1) / 4) && pp.y>=-1.0 && pp.y<=1.0)) continue;}
		break;
		case 5://Torus  DONT WORK ??? ??????????????????
			//if (!((pp.x*pp.x+pp.z*pp.z) <= ((pp.y+1) * (pp.y+1) / 4) && pp.y>=-1.0 && pp.y<=1.0)) continue;
			/*
			if (!((pp.x*pp.x+pp.z*pp.z) <= (0.5 + tr) && (pp.x*pp.x+pp.z*pp.z) > (0.5 - tr) && 
				pp*pp <= ((vnorm(pp)^Vector(0.5,0.0,0.5))-pp)*(vnorm(pp)^Vector(0.5,0.0,0.5))-pp
				)) continue;
				*/
			//if (!((pp*pp+(0.5*0.5)-0.1*0.1)*(pp*pp+(0.5*0.5)-0.1*0.1) == 4 * (0.5*0.5)*(pp.x*pp.x+pp.y*pp.y))) continue;
			//TUBE TES
			/*
			if(invert) {if((pp.x*pp.x+pp.z*pp.z) <= 1.0 && (pp.x*pp.x+pp.z*pp.z) > ri2 && 
				pp.y*pp.y>=-(pp.x*pp.x+pp.z*pp.z)-(ri+(1-(ri)/2)*(ri+(1-(ri)/2))) && pp.y*pp.y <= 1-(pp.x*pp.x+pp.z*pp.z)-(ri+(1-(ri)/2)*(ri+(1-(ri)/2)))) continue;}
			
			else {if(!((pp.x*pp.x+pp.z*pp.z) <= 1.0 && (pp.x*pp.x+pp.z*pp.z) > ri2 && 
				pp.y*pp.y>=-(1.0-((pp.x*pp.x+pp.z*pp.z)-tri2)) && pp.y*pp.y <= (1.0-((pp.x*pp.x+pp.z*pp.z)-tri2))  )) continue;}//<<<< ????
			*/

			//(x2+y2+z2-1-0.25)2-4(x2+y2) = 0


			if(invert) {}
			else {
				//if ((pp*pp-1.0-ri2)*(pp*pp-1.0-ri2) > (4*(pp.x*pp.x+pp.z*pp.z))) continue;
				if ( (sqrt(pp.x*pp.x+pp.z*pp.z)-cc) * (sqrt(pp.x*pp.x+pp.z*pp.z)-cc) + pp.y*pp.y > aa) continue;//BEST
			}

	    break;

		case 6://Tube OK
			if(invert) {if((pp.x*pp.x+pp.z*pp.z) <= 1.0 && (pp.x*pp.x+pp.z*pp.z) > ri2 && pp.y>=-1.0 && pp.y<=1.0) continue;}
			else {if(!((pp.x*pp.x+pp.z*pp.z) <= 1.0 && (pp.x*pp.x+pp.z*pp.z) > ri2 && pp.y>=-1.0 && pp.y<=1.0)) continue;}
		break;
		}

			//PARSER VORBEREITUG
			rc = Real(c);//"c"

			lt = ps[c].t;

			vx = vv.x;
			vy = vv.y;
			vz = vv.z;

			px = gpp.x;
			py = gpp.y;
			pz = gpp.z;

			u = ((rc / Real(pcnt) * px) * pi);//((n / 100 * px) * pi)
			v = ((rc / Real(pcnt) * py) * pi);//???
			w = ((rc / Real(pcnt) * pz) * pi);//???

			//PARSEN
			rnd = rndGer->Get11();
			if (eva_velo) parser->Eval(eva_velo_x,&err,&res_velo_x,UNIT_M,ANGLE_RAD,10);//VELO
			if (eva_pos) parser->Eval(eva_pos_x,&err,&res_pos_x,UNIT_M,ANGLE_RAD,10);//POS

			rnd = rndGer->Get11();
			if (eva_velo) parser->Eval(eva_velo_y,&err,&res_velo_y,UNIT_M,ANGLE_RAD,10);//VELO
			if (eva_pos) parser->Eval(eva_pos_y,&err,&res_pos_y,UNIT_M,ANGLE_RAD,10);//???????

			rnd = rndGer->Get11();
			if (eva_velo) parser->Eval(eva_velo_z,&err,&res_velo_z,UNIT_M,ANGLE_RAD,10);//VELO
			if (eva_pos) parser->Eval(eva_pos_z,&err,&res_pos_z,UNIT_M,ANGLE_RAD,10);//???????
			//------------------

			//POSITION
			if (eva_pos) ps[c].off = Vector(res_pos_x,res_pos_y,res_pos_z);

			if (eva_velo){//VELOCITY
				vv = Vector(res_velo_x,res_velo_y,res_velo_z);
				ss[c].v += vv;
				ss[c].count++;
			}
	}
	//------------------------------

	gDelete(rndGer);
	FreeParser(parser);//FREE PARSER

}


static Bool MenuPrepare(PluginObject *op, BaseDocument *doc)
{
	op->SetDeformMode(TRUE);
	return TRUE;
}

static BaseBitmap *icon=NULL;
static BaseBitmap *icon_small=NULL;

Bool RegisterForcer(void)
{
	// decide by name if the plugin shall be registered - just for user convenience
	String name=GeLoadString(IDS_FCR); if (!name.Content()) return TRUE;

	icon = AllocBaseBitmap();
	if (!icon || icon->Init(GeGetPluginPath()+String("res")+String("fcr.tif"))!=IMAGE_OK) return FALSE;
	icon_small = AllocBaseBitmap();
	if (!icon_small || icon_small->Init(GeGetPluginPath()+String("res")+String("fcr_small.tif"))!=IMAGE_OK) return FALSE;

	OBJECTPLUGIN cs;
	ClearMem(&cs,sizeof(cs));

	cs.add_menu					= TRUE;
	cs.icon						= icon;
	cs.icon_small				= icon_small;
	cs.info						= 0;
	cs.MenuPrepare				= MenuPrepare;
	cs.ModifyParticles			= ModifyParticles;
	cs.AllocSettings			= AllocSettings;
	cs.EditSettings				= EditSettings;
	cs.GetDescription			= GetDescription;
	cs.GetDimension				= GetDimension;
	cs.Draw						= Draw;

	cs.DrawHandles				= DrawHandles;
	cs.DetectHandle				= DetectHandle;
	cs.MoveHandle				= MoveHandle;

	// be sure to use a unique ID obtained from www.plugincafe.com
	return GeRegisterPlugin(C4DPL_OBJECT,PLUGINS_ID,name,&cs,sizeof(cs));
}

void FreeForcer(void)
{
	FreeBaseBitmap(icon);
	FreeBaseBitmap(icon_small);
}
