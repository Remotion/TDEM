/////////////////////////////////////////////////////////////
// FORMEL OBJECT MODIFIKATOR							   //
/////////////////////////////////////////////////////////////
// DATE   17.04.2002 										//
/////////////////////////////////////////////////////////////
// VERSION  1.0  										   //
/////////////////////////////////////////////////////////////
// (c) 2002 Remotion, all rights reserved				   //
/////////////////////////////////////////////////////////////

#include "c4d_file.h"
#include "c4d_objectplugin.h"
#include "c4d_general.h"
#include "c4d_memory.h"
#include "c4d_thread.h"
#include "c4d_baseobject.h"
#include "c4d_basedocument.h"
#include "c4d_basebitmap.h"
#include "c4d_basedraw.h"
#include "c4d_resource.h"
#include "c4d_symbols.h"
#include "c4d_gui.h"

#include "myprint.h"
#include "mytools.h"

//--------------------------
#define PLUGINS_ID  1008113
//--------------------------

//Glob Var_
#define MOD_VER			2000

//#define MOD_RADIUS		1000
//#define MOD_STRENGTH	1001

#define MOD_POS_X		1011
#define MOD_POS_Y		1012
#define MOD_POS_Z		1013
#define MOD_POS			1014

#define MOD_SIZE		1031 //Vector
#define MOD_FORM		1032 //LONG
#define MOD_RAD			1033 //Percent
#define MOD_INVERT		1034 //Percent

#define HANDLES			3 //Handles Count

//#############################	ModDialog #############################
class ModDialog : public GeModalDialog
{
	private:
		BaseContainer *data;
	public:
		ModDialog(BaseContainer *bc) { data = bc; }

		virtual Bool CreateLayout(void);
		virtual Bool InitValues(void);
		virtual Bool Command(LONG id, const BaseContainer &msg);
};

//#############################	CreateLayout #############################
Bool ModDialog::CreateLayout(void)
{
	return GeModalDialog::CreateLayout() && LoadDialogResource(DLG_MOD,NULL,0);
}

//#############################	InitValues #############################
Bool ModDialog::InitValues(void)
{
	// first call the parent instance
	if (!GeModalDialog::InitValues()) return FALSE;

	SetString(IDC_MOD_POS_X,data,MOD_POS_X);
	SetString(IDC_MOD_POS_Y,data,MOD_POS_Y);
	SetString(IDC_MOD_POS_Z,data,MOD_POS_Z);
	SetBool(IDC_MOD_POS,data,MOD_POS);

	SetLong(IDC_MOD_FORM,data,MOD_FORM);
	SetVectorMeter(IDC_MOD_SIZE_X,IDC_MOD_SIZE_Y,IDC_MOD_SIZE_Z,data,MOD_SIZE,0.0,MAXRANGE,1.0);
	SetPercent(IDC_MOD_RAD,data,MOD_RAD,0.0,100.0,1.0);
	SetBool(IDC_MOD_INVERT,data,MOD_INVERT);

	return TRUE;
}

//#############################	Command #############################
Bool ModDialog::Command(LONG id, const BaseContainer &msg)
{
	switch (id)
	{
		case IDC_OK: 
			GetString(IDC_MOD_POS_X,data,MOD_POS_X);
			GetString(IDC_MOD_POS_Y,data,MOD_POS_Y);
			GetString(IDC_MOD_POS_Z,data,MOD_POS_Z);
			GetBool(IDC_MOD_POS,data,MOD_POS);

			GetLong(IDC_MOD_FORM,data,MOD_FORM);
			GetVector(IDC_MOD_SIZE_X,IDC_MOD_SIZE_Y,IDC_MOD_SIZE_Z,data,MOD_SIZE);
			GetReal(IDC_MOD_RAD,data,MOD_RAD);
			GetBool(IDC_MOD_INVERT,data,MOD_INVERT);
			break;			
	}
	
	return TRUE;
}

//#############################	EditSettings #############################
static Bool EditSettings(PluginObject *op)
{
	BaseContainer data = op->GetData();
	ModDialog dlg(&data);
	if (!dlg.Open()) return FALSE;
	op->SetData(data);
	return TRUE;

}

//#############################	AllocSettings #############################
static Bool AllocSettings(PluginObject *op)
{	
	BaseContainer data;

	data.SetString(MOD_POS_X,"x");
	data.SetString(MOD_POS_Y,"y");
	data.SetString(MOD_POS_Z,"z");
	data.SetBool(MOD_POS,FALSE);

	data.SetLong(MOD_FORM,0);
	data.SetVector(MOD_SIZE,100);
	data.SetReal(MOD_RAD,0.5);
	data.SetBool(MOD_INVERT,FALSE);

	op->SetData(data);
	return TRUE;
}

//#############################	GetDescription #############################
static LONG GetDescription(PluginObject *op, LONG id)
{
	switch (id)
	{

		// this parameter can be animated
		case MOD_SIZE: return PARAM_LINEAR;
		case MOD_RAD: return PARAM_LINEAR;
		//case MOD_NEM: return PARAM_LINEAR;

		case MOD_VER: return PARAM_SOLID;
	}
	
	return PARAM_TOGGLE;
}

//#############################	GetDimension #############################
static void GetDimension(PluginObject *op, Vector *mp, Vector *rad)
{
	BaseContainer data = op->GetDataInstance(); 
	LONG form =   data.GetLong(MOD_FORM);

	if (form > 0){
		*rad=data.GetVector(MOD_SIZE);
	}else{
		*rad=Vector(0.0,0.0,0.0);
	}
}

//#############################	Draw #############################
static Bool Draw(PluginObject *op, BaseDraw *bd, BaseDrawHelp *bh)
{
	/*
	BaseContainer data = op->GetDataInstance(); 
	Real   rad=data.GetReal(MOD_RADIUS);
	Vector h;
	Matrix m = bh->GetMg();
	
	m.v1*=rad;
	m.v2*=rad;
	m.v3*=rad;
	
	bd->SetPen(bd->GetObjectColor(op));
	bd->Circle3D(m);
	h=m.v2; m.v2=m.v3; m.v3=h;
	bd->Circle3D(m);
	h=m.v1; m.v1=m.v3; m.v3=h;
	bd->Circle3D(m);
	*/

	BaseContainer	data = op->GetData();//GET SETING
	Vector	rad = data.GetVector(MOD_SIZE);
	LONG	form =   data.GetLong(MOD_FORM);
	Real	ri = data.GetReal(MOD_RAD); //Torus Tube Radius

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
	
	return TRUE;
}

//HANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLES
//HANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLES
//#############################	GetHandle #############################
static Vector GetHandle(PluginObject *op, LONG id)
{
	BaseContainer data = op->GetDataInstance();

	Vector		p;
	Vector		rad = data.GetVector(MOD_SIZE);

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
	Vector		rad = data.GetVector(MOD_SIZE);
	LONG		form = data.GetLong(MOD_FORM);

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
	Vector rad = src.GetVector(MOD_SIZE);

	switch (hit_id)
	{
		case 0: 
			val+=rad.x+val;
			dst.SetVector(MOD_SIZE,Vector(Cut(val,0.0,MAXRANGE),rad.y,rad.z)); 
			break;

		case 1: 
			val+=rad.y+val;
			dst.SetVector(MOD_SIZE,Vector(rad.x,Cut(val,0.0,MAXRANGE),rad.z)); 
			break;

		case 2: 
			val+=rad.z+val;
			dst.SetVector(MOD_SIZE,Vector(rad.x,rad.y,Cut(val,0.0,MAXRANGE))); 
			break;
		
	}
	return TRUE;
}
//HANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLES
//HANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLESHANDLES

/*
Real GetImOb(LONG form, Bool invert, Real ri, Vector pp)
{

		Real			ri2 = ri*ri; //Torus Tube Radius
		Real			ro = 1.0; // R
		Real			ro2 = ro*ro; //Torus Radius ???
		Real			aa = (1.0-ri)*0.5;
		//Real			cc = (1.0+ri)*0.5 + aa;
		Real			cc = ri + aa;
		aa = aa*aa;
		cc = cc*cc;


		switch(form){//TEST ?????
		case 1://Cube OK
			if(invert) {if(pp.x>=-1.0 && pp.x<=1.0 && pp.y>=-1.0 && pp.y<=1.0 && pp.z>=-1.0&& pp.z<=1.0); }
			else {if(!(pp.x>=-1.0 && pp.x<=1.0 && pp.y>=-1.0 && pp.y<=1.0 && pp.z>=-1.0&& pp.z<=1.0)); }
		break;
		case 2://Ellipse OK
			if(invert) {if(pp*pp <= 1.0); }
			else {if(!(pp*pp <= 1.0)); }
		break;
		case 3://Zylinder OK
			if(invert) {if((pp.x*pp.x+pp.z*pp.z) <= 1.0 && pp.y>=-1.0 && pp.y<=1.0); }
			else {if (!((pp.x*pp.x+pp.z*pp.z) <= 1.0 && pp.y>=-1.0 && pp.y<=1.0)); }
		break;
		case 4://Kegel OK
			if(invert) {if((pp.x*pp.x+pp.z*pp.z) <= ((pp.y+1) * (pp.y+1) / 4) && pp.y>=-1.0 && pp.y<=1.0); }
			else {if(!((pp.x*pp.x+pp.z*pp.z) <= ((pp.y+1) * (pp.y+1) / 4) && pp.y>=-1.0 && pp.y<=1.0)); }
		break;
		case 5://Torus  DONT WORK ??? ??????????????????
			if(invert) {if ((sqrt(pp.x*pp.x+pp.z*pp.z)-cc) * (sqrt(pp.x*pp.x+pp.z*pp.z)-cc) + pp.y*pp.y > aa); }
			else {if (!(sqrt(pp.x*pp.x+pp.z*pp.z)-cc) * (sqrt(pp.x*pp.x+pp.z*pp.z)-cc) + pp.y*pp.y > aa); }
	    break;
		case 6://Tube OK
			if(invert) {if((pp.x*pp.x+pp.z*pp.z) <= 1.0 && (pp.x*pp.x+pp.z*pp.z) > ri2 && pp.y>=-1.0 && pp.y<=1.0); }
			else {if(!((pp.x*pp.x+pp.z*pp.z) <= 1.0 && (pp.x*pp.x+pp.z*pp.z) > ri2 && pp.y>=-1.0 && pp.y<=1.0)); }
		break;
		}
		return 0.0;
}
*/

//#############################	ModifyObject #############################
static Bool ModifyObject(PluginObject *mod, BaseDocument *doc, BaseObject *op, const Matrix &op_mg, const Matrix &mod_mg, Real lod, LONG flags, BaseThread *thread)
{
	BaseContainer	data = mod->GetDataInstance(); 

	LONG			fps = doc->GetFps();//Get Doc FPS
	BaseTime		BTtime = doc->GetTime();
	Real			time=BTtime.Get();

	Vector			p,*padr=NULL;
	Matrix			m,im;
	LONG			i,pcnt;
	//Real			rad=data.GetReal(MOD_RADIUS),
	Real			strength=1.0;
	Real			s,*weight=NULL;
	//IT WORK !!!
	LONG			form = data.GetLong(MOD_FORM);//COLL FORM
	Bool			invert = data.GetBool(MOD_INVERT);
	Vector			rad = data.GetVector(MOD_SIZE);//COLL SIZE
	Real			ri = data.GetReal(MOD_RAD); // r
	//TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST
	Vector			pp;
	Real			ri2 = ri*ri; //Torus Tube Radius
	Real			ro = 1.0; // R
	Real			ro2 = ro*ro; //Torus Radius ???
	Real			aa = (1.0-ri)*0.5;
	//Real			cc = (1.0+ri)*0.5 + aa;
	Real			cc = ri + aa;
	aa = aa*aa;
	cc = cc*cc;
	//TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST
	String			eva_pos_x = data.GetString(MOD_POS_X);
	String			eva_pos_y = data.GetString(MOD_POS_Y);
	String			eva_pos_z = data.GetString(MOD_POS_Z);
	Bool			eva_pos = data.GetBool(MOD_POS);

	if (!(op->GetLayerType()&L_pointobject)) return TRUE;
	padr = ToPoint(op)->GetPoint();
	pcnt = ToPoint(op)->GetPointCount(); if (!pcnt) return TRUE;
	weight = ToPoint(op)->CalcVertexMap(mod);
	m  = (!mod_mg) * op_mg; // op  ->  world  ->  modifier
	im = !m;

	Matrix tm;
	tm.v1*=rad.x;
	tm.v2*=rad.y;
	tm.v3*=rad.z;
	Matrix itm = !tm;
	//IT WORK !!!

	Real			x,y,z,n;
	Real			res_pos_x,res_pos_y,res_pos_z;//RESULT
	LONG			err = 0;//ERRORS ???
	//parser
	Parser			*parser = AllocParser();//ALLOC PARESER
	parser->AddVar("x",&x);//Particle Position
	parser->AddVar("y",&y);
	parser->AddVar("z",&z);

	parser->AddVar("n",&n);
	//parser->AddVar("lt",&lt);
	//parser->AddVar("rnd",&rnd);
	//parser->AddVar("diff",&diff);
	parser->AddVar("t",&time);


	for (i=0; i<pcnt; i++)
	{
		
		if (thread && !(i&63) && thread->TestBreak()) break;
		pp = m * padr[i] * itm;//locale position
		
		switch(form){//TEST ?????
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
			if(invert) {if ((sqrt(pp.x*pp.x+pp.z*pp.z)-cc) * (sqrt(pp.x*pp.x+pp.z*pp.z)-cc) + pp.y*pp.y > aa) continue;}
			else {if (!(sqrt(pp.x*pp.x+pp.z*pp.z)-cc) * (sqrt(pp.x*pp.x+pp.z*pp.z)-cc) + pp.y*pp.y > aa) continue;}
	    break;
		case 6://Tube OK
			if(invert) {if((pp.x*pp.x+pp.z*pp.z) <= 1.0 && (pp.x*pp.x+pp.z*pp.z) > ri2 && pp.y>=-1.0 && pp.y<=1.0) continue;}
			else {if(!((pp.x*pp.x+pp.z*pp.z) <= 1.0 && (pp.x*pp.x+pp.z*pp.z) > ri2 && pp.y>=-1.0 && pp.y<=1.0)) continue;}
		break;
		}
		
		
		p = m*padr[i];
		//--------------------

		x = p.x;
		y = p.y;
		z = p.z;
		n = Real(i);//"n" ????

		s = strength;
		if (weight) s*=weight[i];

		parser->Eval(eva_pos_x,&err,&res_pos_x,UNIT_M,ANGLE_RAD,10);
		parser->Eval(eva_pos_y,&err,&res_pos_y,UNIT_M,ANGLE_RAD,10);
		parser->Eval(eva_pos_z,&err,&res_pos_z,UNIT_M,ANGLE_RAD,10);

		//p = s * (!p + Vector(res_pos_x,res_pos_y,res_pos_z))+(1.0-s)*p;
		p = s * Vector(res_pos_x,res_pos_y,res_pos_z);

		//--------------------
		padr[i] = p*im;
	}

	FreeParser(parser);//FREE PARSER
	GeFree(weight);
	op->Message(MSG_UPDATE);
	
	return TRUE;
}
//#############################	MenuPrepare #############################
static Bool MenuPrepare(PluginObject *op, BaseDocument *doc)
{
	op->SetDeformMode(TRUE);
	return TRUE;
}

static BaseBitmap *icon=NULL;
static BaseBitmap *icon_small=NULL;

//#############################	RegisterMOD #############################
Bool RegisterMOD(void)
{
	// decide by name if the plugin shall be registered - just for user convenience
	String name=GeLoadString(IDS_MOD); if (!name.Content()) return TRUE;

	icon = AllocBaseBitmap();
	if (!icon || icon->Init(GeGetPluginPath()+String("res")+String("mod.tif"))!=IMAGE_OK) return FALSE;
	icon_small = AllocBaseBitmap();
	if (!icon_small || icon_small->Init(GeGetPluginPath()+String("res")+String("mod_small.tif"))!=IMAGE_OK) return FALSE;

	OBJECTPLUGIN cs;
	ClearMem(&cs,sizeof(cs));

	cs.add_menu			= TRUE;
	cs.icon				= icon;
	cs.icon_small		= icon_small;
	cs.info				= OBJECT_MODIFIER;
	cs.MenuPrepare		= MenuPrepare;
	cs.ModifyObject		= ModifyObject;
	cs.AllocSettings	= AllocSettings;
	cs.EditSettings		= EditSettings;
	cs.Draw				= Draw;
	cs.DrawBox			= Draw; // draw modifier in box mode also
	cs.DrawHandles		= DrawHandles;
	cs.GetDimension		= GetDimension;
	cs.DetectHandle		= DetectHandle;
	cs.MoveHandle		= MoveHandle;
	cs.GetDescription	= GetDescription;

	// be sure to use a unique ID obtained from www.plugincafe.com
	return GeRegisterPlugin(C4DPL_OBJECT,PLUGINS_ID,name,&cs,sizeof(cs));
}

//#############################	FreeMOD #############################
void FreeMOD(void)
{
	FreeBaseBitmap(icon);
	FreeBaseBitmap(icon_small);
}
