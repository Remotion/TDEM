/////////////////////////////////////////////////////////////
// Point Particles											   //
/////////////////////////////////////////////////////////////
// VERSION  1.0  										   //
/////////////////////////////////////////////////////////////
// (c) 2002 Remotion, all rights reserved				   //
/////////////////////////////////////////////////////////////

// "look at editor camera" expression example

#include "c4d_tagplugin.h"
#include "c4d_general.h"
#include "c4d_memory.h"
#include "c4d_baseobject.h"
#include "c4d_basedocument.h"
#include "c4d_basetag.h"
#include "c4d_basebitmap.h"
#include "c4d_basedraw.h"
#include "c4d_particleobject.h"
#include "c4d_resource.h"
#include "c4d_symbols.h"
#include "c4d_gui.h"

#include "mytools.h"
#include "myprint.h"

//-----------------------------------
#define PTP_ID				1007708
//-----------------------------------
#define ESP_ID				1007709
//-----------------------------------

#define PTP_POITOBJECT		1001
#define PTP_POITOBJECT_NUM	1002
//#define PTP_VERTEX			1003
//#define PTP_VERTEX_NUM		1004

//#define PTP_SOURCE			1005
//#define PTP_SOURCE_NUM		1006
#define PTP_SYSTYP			1007
#define PTP_COUNT			1008
#define PTP_ANIM			1009
#define PTP_PST				1010

#define OB_TIME				4001


//DONT WORK????
//-----------------------------------------------------------------------------------
BaseObject *CreateParticleSpline(BaseDocument *doc, ParticleObject *PSourceOb, LONG ppcnt = 4, Real le = 20)
{
	if ((!doc) || (!PSourceOb)) return NULL;

	LONG		c,i;
	LONG		ppcnt1 = ppcnt - 1;
	SplineObject *MultiSpline = NULL;
	Vector		*SplinePoints = NULL;//Array
	Segment		*SplineSegment = NULL;

	BaseContainer res;
	LONG		ParticleCnt;
	Particle	*Particles = NULL;//Array
	ParticleTag	*PPPTag = NULL;

	//Get Particle Tag
	PPPTag = (ParticleTag*)PSourceOb->GetTag(Tparticle);if (!PPPTag) {goto Error; }
	ParticleCnt = PSourceOb->GetParticleCount();
	Particles = PSourceOb->GetParticle(PPPTag,0); //if (!Particles) {goto Error; }

	doc->StartUndo();
	{
		MultiSpline = AllocSplineObject(ParticleCnt * ppcnt,(ParticleCnt>10000)?Tlinear:Takima);
		if (!MultiSpline || !MultiSpline->MakeVariableTag(Tsegment,ParticleCnt)) {goto Error; }

		doc->InsertObject(MultiSpline,NULL,NULL);

		MultiSpline->SetName(PSourceOb->GetName() + "-ms." + LongToString(ppcnt));
		//MultiSpline->SetPos(PSourceOb->GetPos());
		//MultiSpline->SetScale(PSourceOb->	GetScale());
		//MultiSpline->SetRot(PSourceOb->GetRot());

		doc->AddUndo(UNDO_OBJECT_NEW,MultiSpline);

		SplinePoints = MultiSpline->GetPoint();if (!SplinePoints) goto Error;
		SplineSegment = MultiSpline->GetSegment();if (!SplineSegment) goto Error;

		for (c=0;c<ParticleCnt;c++){
			if (!(c&1023) && GetInputState(BFM_INPUT_KEYBOARD,KEY_ESC,res) && res.GetLong(BFM_INPUT_VALUE)) goto Error;//TEST
			//if (!(Particles[c].bits & PARTICLE_VISIBLE)) continue; //BUG
			SplineSegment[c].cnt = ppcnt;
			SplinePoints[c*ppcnt] = Particles[c].off;
			for (i=1; i<ppcnt;  i++){
				SplinePoints[c*ppcnt+i] = Particles[c].off+(vnorm(Particles[c].v3)*-le)*(Real(i) / ppcnt1);
				//SplinePoints[c*ppcnt+i] = Particles[c].off-Particles[c].v3*(Real(i) / ppcnt1);
			}
		}
		MultiSpline->Message(MSG_UPDATE);
		GeEventAdd(MSG_DOCUMENTCHANGED);
	}
	doc->EndUndo();
	return MultiSpline;

Error:
 blDelete(MultiSpline);
 return NULL;
}


//-----------------------------------------------------------------------------------
BaseObject *CreatePolySpline(BaseDocument *doc, BaseObject *FormOb, LONG ppcnt = 4, Real le = 20)
{
	//if ((!doc) || (!FormOb) || (FormOb->GetType()!=Opolygon)) return NULL;
	if ((!doc) || (!FormOb)) return NULL;

	LONG			c,i;
	LONG			ppcnt1 = ppcnt - 1;
	SplineObject	*multiSpline = NULL;
	Vector			*msPoints = NULL;
	Segment			*msSegment = NULL;
	LONG			FormPointCnt;
	Vector			*FormPoints = NULL;//Array
	//LONG			FormPolygonCnt;
	CPolygon			*FormPolygon = NULL;//Array

	BaseContainer	res;
	Vector			*VertexNormalArray = NULL;
	PolygonObject	*PolyOb = NULL;
	BaseObject		*CurStOb = NULL;
	BaseObject		*JoinOb = NULL;
	BaseContainer	nullbc;nullbc.SetId(0);

	CurStOb = (BaseObject*)SendModelingCommand(MCOMMAND_CURRENTSTATETOOBJECT,doc,FormOb,&nullbc,MODIFY_ALL);if (!CurStOb) {goto Error; }
	JoinOb = (BaseObject*)SendModelingCommand(MCOMMAND_JOIN,doc,CurStOb,&nullbc,MODIFY_ALL);if (!JoinOb) {goto Error; }
	PolyOb = (PolygonObject*)JoinOb; if (!PolyOb) {goto Error; } 

	FormPointCnt = PolyOb->GetPointCount();
	FormPoints = PolyOb->GetPoint(); if (!FormPoints) {goto Error; }

	GetVertexNormalSafe(&VertexNormalArray,PolyOb);if (!VertexNormalArray)  {goto Error; }

	doc->StartUndo();
	{
		multiSpline = AllocSplineObject(FormPointCnt * ppcnt,(FormPointCnt>10000)?Tlinear:Takima);
		if (!multiSpline || !multiSpline->MakeVariableTag(Tsegment,FormPointCnt)) {goto Error; }

		//MultiSpline->InsertUnder(FormOb->up());//Crasch
		doc->InsertObject(multiSpline,NULL,NULL);

		multiSpline->SetName(PolyOb->GetName() + "-ms." + LongToString(ppcnt));

		multiSpline->SetPos(PolyOb->GetPos());
		//MultiSpline->SetScale(PolyOb->GetScale());
		multiSpline->SetRot(PolyOb->GetRot());

		doc->AddUndo(UNDO_OBJECT_NEW,multiSpline);

		msPoints = multiSpline->GetPoint();if (!msPoints) goto Error;
		msSegment = multiSpline->GetSegment();if (!msSegment) goto Error;

		for (c=0;c<FormPointCnt;c++){
			if (!(c&1023) && GetInputState(BFM_INPUT_KEYBOARD,KEY_ESC,res) && res.GetLong(BFM_INPUT_VALUE)) goto Error;//TEST
			msSegment[c].cnt = ppcnt;
			msPoints[c*ppcnt] = FormPoints[c];
			for (i=1; i<ppcnt;  i++){
				msPoints[c*ppcnt+i] = FormPoints[c]+VertexNormalArray[c]*le*(Real(i) / ppcnt1);
			}
		}
		GeFree(VertexNormalArray);//???
		multiSpline->Message(MSG_UPDATE);
		GeEventAdd(MSG_DOCUMENTCHANGED);//???
	}
	doc->EndUndo();
	blDelete(CurStOb);
	blDelete(JoinOb);
	return multiSpline;

Error:
 blDelete(multiSpline);
 blDelete(CurStOb);
 blDelete(JoinOb);
 GeFree(VertexNormalArray);//???
 return NULL;
}
//-----------------------------------------------------------------------------------



//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
class PTPDialog : public GeModalDialog
{
	private:
		BaseContainer *data;
		PluginTag *ptag;
	public:
		PTPDialog(BaseContainer *bc,PluginTag *tag) { data = bc; ptag = tag; }

		virtual Bool CreateLayout(void);
		virtual Bool InitValues(void);
		virtual Bool Command(LONG id, const BaseContainer &msg);
		//virtual LONG Message(const BaseContainer &msg,BaseContainer &result);
};

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
Bool PTPDialog::CreateLayout(void)
{
	return GeModalDialog::CreateLayout() && LoadDialogResource(DLG_PTP,NULL,0);
	//return GeDialog::CreateLayout() && LoadDialogResource(DLG_PTP,NULL,0);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
Bool PTPDialog::InitValues(void)
{
	//Bool bo;
	String st = "";
	LONG lo;
	PointObject		*FormOb = NULL;
	PointObject		*SourceOb = NULL;
	LONG			PointCnt = 0;


	// first call the parent instance
	if (!GeModalDialog::InitValues()) return FALSE;
	//if (!GeDialog::InitValues()) return FALSE;

	//FORM
	SetString(IDC_PTP_POITOBJECT,data,PTP_POITOBJECT);
	SetLong(IDC_PTP_POITOBJECT_NUM,data,PTP_POITOBJECT_NUM,0,10000,1);
	GetString(IDC_PTP_POITOBJECT,st);Enable(IDC_PTP_POITOBJECT_NUM,(st != ""));

	//VERTEX
	//SetString(IDC_PTP_VERTEX,data,PTP_VERTEX);
	//SetLong(IDC_PTP_VERTEX_NUM,data,PTP_VERTEX_NUM,0,10000,1);
	//GetString(IDC_PTP_VERTEX,st);
	//Enable(IDC_PTP_VERTEX_NUM,(st != ""));
	//Enable(IDC_PTP_VERTEX,FALSE);//TEST
	//Enable(IDC_PTP_VERTEX_NUM,FALSE);//TEST

	//SOURCE
	//SetString(IDC_PTP_SOURCE,data,PTP_SOURCE);
	//SetLong(IDC_PTP_SOURCE_NUM,data,PTP_SOURCE_NUM,0,10000,1);
	//GetString(IDC_PTP_SOURCE,st);
	//Enable(IDC_PTP_SOURCE_NUM,(st != ""));
	//Enable(IDC_PTP_SOURCE,FALSE);//TEST
	//Enable(IDC_PTP_SOURCE_NUM,FALSE);//TEST

	SetLong(IDC_PTP_SYSTYP,data,PTP_SYSTYP);
	SetLong(IDC_PTP_COUNT1,data,PTP_COUNT,0,MAXRANGE,1);
	GetLong(IDC_PTP_SYSTYP,lo);
	//Enable(IDC_PTP_COUNT1,(lo==0));

	SetBool(IDC_PTP_ANIM,data,PTP_ANIM);

	SetLong(IDC_PTP_PST1,data,PTP_PST);

	SetLong(IDC_PTP_PPC1,5,3,1000,1);//TEST

	//----- GET OB POINT / PARTICLE COUNT 
	SourceOb = (PointObject*)ptag->GetObject();
	if (SourceOb){
		if (SourceOb->GetLayerType()&L_pointobject){
			PointCnt = SourceOb->GetPointCount();//<<<<<<
			SetLong(IDC_PTP_COUNT1,data,PTP_COUNT,0,PointCnt-1,1);
			SetString(IDC_PTP_IN1,LongToString(PointCnt));
		}else{
			if (SourceOb->GetType()==Oparticle){
				PointCnt = ((ParticleObject*)SourceOb)->GetParticleCount();//<<<<<<
				SetLong(IDC_PTP_COUNT1,data,PTP_COUNT,0,PointCnt-1,1);
				SetString(IDC_PTP_IN1,LongToString(PointCnt));
				//print(PointCnt);
			}else{
				SetLong(IDC_PTP_COUNT1,data,PTP_COUNT,0,MAXRANGE,1);
				SetString(IDC_PTP_IN1,"---");
			}
		}
	}
	//-----



	//----- FIX OB POINT COUNT
	GetString(IDC_PTP_POITOBJECT,st);
	SourceOb = (PointObject*)ptag->GetObject();
	if (SourceOb){
		BaseDocument *doc = SourceOb->GetDocument();
		if (doc){
		GetLong(IDC_PTP_POITOBJECT_NUM,lo);
		if (lo==0) FormOb = (PointObject*)doc->SearchObject(st);
		else FormOb = (PointObject*)doc->SearchObject(st+LongToString(lo));
		if (FormOb){
			if (FormOb->GetLayerType()&L_pointobject){
				PointCnt = FormOb->GetPointCount();//<<<<<<
				SetString(IDC_PTP_IN2,LongToString(PointCnt));
			}

		}else{
			SetString(IDC_PTP_IN2,"---");
		}
		}
	}
	//-----

	return TRUE;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
Bool PTPDialog::Command(LONG id, const BaseContainer &msg)
{
	//Bool bo;
	String st = "";
	LONG lo;
	PointObject		*FormOb = NULL;
	PointObject		*SourceOb = NULL;
	BaseObject		*PolyOb = NULL;
	LONG			PointCnt = 0;
	BaseObject		*Spl = NULL;

	switch (id){
		case IDC_OK:
			//FORM
			GetString(IDC_PTP_POITOBJECT,data,PTP_POITOBJECT);
			GetLong(IDC_PTP_POITOBJECT_NUM,data,PTP_POITOBJECT_NUM);
			//VERTEX
			//GetString(IDC_PTP_VERTEX,data,PTP_VERTEX);
			//GetLong(IDC_PTP_VERTEX_NUM,data,PTP_VERTEX_NUM);
			//SOURCE
			//GetString(IDC_PTP_SOURCE,data,PTP_SOURCE);
			//GetLong(IDC_PTP_SOURCE_NUM,data,PTP_SOURCE_NUM);

			GetLong(IDC_PTP_SYSTYP,data,PTP_SYSTYP);

			GetLong(IDC_PTP_COUNT1,data,PTP_COUNT);
			GetBool(IDC_PTP_ANIM,data,PTP_ANIM);
			GetLong(IDC_PTP_PST1,data,PTP_PST);
		break;


		case IDC_PTP_POITOBJECT_NUM:
		case IDC_PTP_POITOBJECT:
			GetString(IDC_PTP_POITOBJECT,st);
			Enable(IDC_PTP_POITOBJECT_NUM,(st != ""));

			//----- FIX OB POINT COUNT
			SourceOb = (PointObject*)ptag->GetObject();
			if (SourceOb){
			BaseDocument *doc = SourceOb->GetDocument();
			if (doc){
				GetLong(IDC_PTP_POITOBJECT_NUM,lo);
				if (lo==0) FormOb = (PointObject*)doc->SearchObject(st);
				else FormOb = (PointObject*)doc->SearchObject(st+LongToString(lo));
				if (FormOb){
					if (FormOb->GetLayerType()&L_pointobject){
						PointCnt = FormOb->GetPointCount();
						SetString(IDC_PTP_IN2,LongToString(PointCnt));
					}
				}else{
					SetString(IDC_PTP_IN2,"---");
				}
			}
			}
			//-----

		break;

		/*case IDC_PTP_VERTEX:
			GetString(IDC_PTP_VERTEX,st);
			Enable(IDC_PTP_VERTEX_NUM,(st != ""));
		break;

		case IDC_PTP_SOURCE:
			GetString(IDC_PTP_SOURCE,st);
			Enable(IDC_PTP_SOURCE_NUM,(st != ""));
		break;*/

		case IDC_PTP_SYSTYP:
			GetLong(IDC_PTP_SYSTYP,lo);
			//Enable(IDC_PTP_COUNT1,(lo==0));

			//-----
			SourceOb = (PointObject*)ptag->GetObject();
			if (SourceOb){
				if (SourceOb->GetLayerType()&L_pointobject){
					PointCnt = SourceOb->GetPointCount();//<<<<<<
					SetLong(IDC_PTP_COUNT1,data,PTP_COUNT,0,PointCnt-1,1);
					SetString(IDC_PTP_IN1,LongToString(PointCnt));
				}else{SetLong(IDC_PTP_COUNT1,data,PTP_COUNT,0,MAXRANGE,1);}
			}
			//-----
		break;

		case IDC_PTP_MS1://Create Multi Spline	
			PolyOb = ptag->GetObject();
			if (PolyOb){
				GetLong(IDC_PTP_PPC1,lo);
				if (PolyOb->GetType()==Oparticle)
					Spl = CreateParticleSpline(PolyOb->GetDocument(),(ParticleObject*)PolyOb,lo);
				else
					Spl = CreatePolySpline(PolyOb->GetDocument(),PolyOb,lo);
				if (Spl) {
					SetString(IDC_PTP_POITOBJECT,Spl->GetName());
					//SetString(IDC_PTP_POITOBJECT,"--");
					SetLong(IDC_PTP_SYSTYP,0);
					SetLong(IDC_PTP_PST1,1);
					BaseTag *tg = AllocTag(Tplugin,ESP_ID,0);
					if (tg) Spl->InsertTag(tg,NULL);
				}
			}
		break;
	}

	return TRUE;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
static Bool AllocSettings(PluginTag *tag)
{
	BaseContainer bc;


	//FORM
	bc.SetString(PTP_POITOBJECT,"");
	bc.SetLong(PTP_POITOBJECT_NUM,0);
	//VERTEX
	//bc.SetString(PTP_VERTEX,"");
	//bc.SetLong(PTP_VERTEX_NUM,0);
	//SOURCE
	//bc.SetString(PTP_SOURCE,"");
	//bc.SetLong(PTP_SOURCE_NUM,0);

	bc.SetLong(PTP_SYSTYP,1);
	bc.SetLong(PTP_COUNT,0);
	bc.SetBool(PTP_ANIM,TRUE);
	bc.SetLong(PTP_PST,0);

	tag->SetData(bc);
	return TRUE;
}

/*//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
LONG PTPDialog::Message(const BaseContainer &msg,BaseContainer &result)
{
//	switch (msg.GetId())
	{
	}
	return GeDialog::Message(msg,result);
}
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
static Bool EditSettings(PluginTag *tag)
{
	BaseContainer data = tag->GetData();

	PTPDialog dlg(&data,tag);

	//Test ob op is Emitter ???
	//BaseObject *EmiterOb = tag->GetObject();
	//LONG EmiterObTyp = EmiterOb->GetType();
	//if (EmiterObTyp!=Oparticle) return FALSE;
	
	if (!dlg.Open()) return FALSE;

	//if (!dlg.Open(FALSE,PLUGINS_ID)) return FALSE;
	//BaseObject *op = GetObject();

	tag->SetData(data);
	return TRUE;
}


//##################################### PTP Execute #####################################
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
static Bool Execute(PluginTag *tag, BaseDocument *doc, BaseObject *op)
{//PTP

BaseContainer data = tag->GetData();

//ANIM
BaseTime	myobject_lasttime = data.GetTime(OB_TIME);
Bool		onem = data.GetBool(PTP_ANIM);
if (onem){
	if (doc->GetTime()!=myobject_lasttime){
		myobject_lasttime = doc->GetTime();
		data.SetTime(OB_TIME,myobject_lasttime);
		tag->SetData(data);
	}else{return TRUE;}
}
//ANIM

//-------------------------------------
Real fps = doc->GetFps();
LONG SysTyp = data.GetBool(PTP_SYSTYP);
LONG Number = data.GetLong(PTP_COUNT);
LONG PsTyp	= data.GetLong(PTP_PST);
//------------------------------------

PointObject *bb = NULL;//Cache
BaseObject *def = NULL;//Deformed Cache

ParticleObject	*EmiterOb = (ParticleObject*)op;//Emiter zum Modifizieren
Particle		*Particles = NULL;
ParticleTag		*EmPTag = NULL;
LONG			ParticleCnt = 0;

PointObject		*SourceOb = NULL;
LONG			SourcePointCnt = 0;
Vector			*SourcePoints = NULL;

LONG			c,la,nu;

Matrix  SourceML , iSourceML;
Matrix	SourceMG, iSourceMG;
Matrix  SourceMGU;
Matrix  cm;

Vector			*VertexNormalArray = NULL;


LONG SourceObTyp = op->GetType();
if (SourceObTyp==Oparticle){
	//Get Emitter Particle
	bb = (PointObject*)op;
	EmPTag = (ParticleTag*)EmiterOb->GetFirstTag();if (!EmPTag) return TRUE;
	while (EmPTag->GetType()!=Tparticle){EmPTag = (ParticleTag*)((BaseTag*)EmPTag)->next();}
	ParticleCnt = EmiterOb->GetParticleCount();if (ParticleCnt<=0) return TRUE;
	Particles = EmiterOb->GetParticle(EmPTag,0);if (!Particles) return TRUE;
}else{
	//GET OBJECT CACHE !! (SICHER???)
	bb = (PointObject*)op->GetCache();
	if (bb) def = bb->GetDeformCache();
	else def = op->GetDeformCache();
	if (def) bb = (PointObject*)def;
	if (!bb) bb = (PointObject*)op;

	if (bb->GetLayerType()&L_pointobject){
		SourceOb = ToPoint(bb);
		SourcePointCnt = SourceOb->GetPointCount();if (SourcePointCnt<=0) {goto Error; }
		SourcePoints =  SourceOb->GetPoint();if (!SourcePoints) {goto Error; }
		SourceML	=	op->GetMl(), iSourceML = !SourceML;
		SourceMG	=	op->GetMg(), iSourceMG = !SourceMG;
		SourceMGU	=	op->GetUpMg();
	}else{
		return TRUE;
	}
}

switch(SysTyp){
case 0:{//Multi Spline 1 Point
	//######################################################################

	SplineObject	*FormSplineOb = NULL;
	LONG			FormPointCnt = 0;
	Vector			*FormPoints = NULL;//Array
	LONG			FormSegCnt = 0;
	Segment			*Seg = NULL;
	//Vector			*VertexNormalArray = NULL;

	//Form Name
	String FormObName = GetNumName(data,PTP_POITOBJECT,PTP_POITOBJECT_NUM);
	PointObject		*FormOb = NULL;//Emiter Start Point Object
	//Find Form Object
    FormOb = (PointObject*)doc->SearchObject(FormObName);
    if (!FormOb){
		FormOb = (PointObject*)op->up();if (!FormOb){GePrint("PTP: Multi Spline Fix-Object "+FormObName+" is not found!"); goto Error;}
	}

	if (FormOb->GetType() == Ospline){

		FormSplineOb = ToSpline(FormOb);
		FormPointCnt = FormSplineOb->GetPointCount();//<<<<<<
		if (FormPointCnt>0){
			Matrix  FormSplineML=FormSplineOb->GetMl(), iFormSplineML = !FormSplineML;
			Matrix	FormSplineMG=FormSplineOb->GetMg(), iFormSplineMG = !FormSplineMG;
			Matrix  FormSplineMGU=FormSplineOb->GetUpMg();

			FormPoints =  FormSplineOb->GetPoint();if (!FormPoints) {goto Error; }
			FormSegCnt = FormSplineOb->GetSegmentCount();
			if (FormSegCnt == 0){// FormSegCnt = 1; //??????
				if (Particles) FormPoints[0] = Particles[Number % ParticleCnt].off*iFormSplineMG;
				if (SourcePoints) FormPoints[0] = SourcePoints[Number % SourcePointCnt]*SourceMG*iFormSplineMG;
				FormSplineOb->Message(MSG_UPDATE);
			}else{
				Seg = FormSplineOb->GetSegment();if (!Seg) {goto Error; }
				switch(PsTyp){
				//!!!!!!!!!!!!!!!!!!!!!!!!!!
				case 0://Standard
					if (Particles) {
						la = 0;
						for (c=0; c < FormSegCnt; c++){
							FormPoints[la % FormPointCnt] = Particles[(c+Number) % ParticleCnt].off*iFormSplineMG;
							la = la + Seg[c].cnt;
						}
					}
					if (SourcePoints){
						la = 0;
						for (c=0; c < FormSegCnt; c++){
							FormPoints[la % FormPointCnt] = SourcePoints[(c+Number) % SourcePointCnt]*SourceMG*iFormSplineMG;
							la = la + Seg[c].cnt;
						}
					}
				break;
				//!!!!!!!!!!!!!!!!!!!!!!!!!!
				case 1://Vertex Normal
					if (Particles) {//PARTIKEL
						la = 0;
						for (c=0; c < FormSegCnt; c++){
							nu = (c+Number) % ParticleCnt;
							FormPoints[la % FormPointCnt] = Particles[nu].off*iFormSplineMG;
							FormPoints[(la+1) % FormPointCnt] = (vnorm(Particles[nu].v3)*-10)+Particles[nu].off*iFormSplineMG;
							la = la + Seg[c].cnt;
						}
					}
					if (SourcePoints){//PUNKTE
						GetVertexNormalSafe(&VertexNormalArray,ToPoly(SourceOb)); if (!VertexNormalArray) break;
						la = 0;
						for (c=0; c < FormSegCnt; c++){
							nu = (c+Number) % SourcePointCnt;
							FormPoints[la % FormPointCnt] = SourcePoints[nu]*SourceMG*iFormSplineMG;
							FormPoints[(la+1) % FormPointCnt] = ((VertexNormalArray[nu]*10)+SourcePoints[nu])*SourceMG*iFormSplineMG;
							la = la + Seg[c].cnt;
						}
						GeFree(VertexNormalArray);
					}
				break;
				//!!!!!!!!!!!!!!!!!!!!!!!!!!
				}
				FormSplineOb->Message(MSG_UPDATE);
			}//if (FormSegCnt == 0){
		}
	}

	{goto Error; }
	//######################################################################
}break;


case 3:{//Multi Spline Last Point
	//######################################################################

	SplineObject	*FormSplineOb = NULL;
	LONG			FormPointCnt = 0;
	Vector			*FormPoints = NULL;//Array
	LONG			FormSegCnt = 0;
	Segment			*Seg = NULL;
	//Vector			*VertexNormalArray = NULL;

	//Form Name
	String FormObName = GetNumName(data,PTP_POITOBJECT,PTP_POITOBJECT_NUM);
	PointObject		*FormOb = NULL;//Emiter Start Point Object
	//Find Form Object
    FormOb = (PointObject*)doc->SearchObject(FormObName);
    if (!FormOb){
		FormOb = (PointObject*)op->up();
		if (!FormOb){GePrint("PTP: Multi Spline Fix-Object "+FormObName+" is not found!"); goto Error; }
	}

	if (FormOb->GetType() == Ospline){
		FormSplineOb = ToSpline(FormOb);
		FormPointCnt = FormSplineOb->GetPointCount();//<<<<<<
		if (FormPointCnt>0){
			FormPoints =  FormSplineOb->GetPoint();if (!FormPoints) {goto Error; }
			Matrix  FormSplineML=FormSplineOb->GetMl(), IFormSplineML=!FormSplineML; 
			Matrix	FormSplineMG=FormSplineOb->GetMg();
			Matrix  FormSplineMGU=FormSplineOb->GetUpMg();

			FormSegCnt = FormSplineOb->GetSegmentCount();
			if (FormSegCnt == 0){// FormSegCnt = 1; //??????
				if (Particles) FormPoints[FormPointCnt-1] = Particles[Number % ParticleCnt].off*IFormSplineML;
				if (SourcePoints) FormPoints[FormPointCnt-1] = SourcePoints[Number % SourcePointCnt]*SourceMG*IFormSplineML;
				FormSplineOb->Message(MSG_UPDATE);
			}else{
				Seg = FormSplineOb->GetSegment();if (!Seg) {goto Error; }
				switch(PsTyp){
				//!!!!!!!!!!!!!!!!!!!!!!!!!!
				case 0://Standard
					if (Particles) {
						la = FormPointCnt-1;
						for (c=FormSegCnt-1; c >= 0; c--){
							FormPoints[la % FormPointCnt] = Particles[(c+Number) % ParticleCnt].off*IFormSplineML;
							la = la - Seg[c].cnt;
						}
					}
					if (SourcePoints){
						la = FormPointCnt-1;
						for (c=FormSegCnt-1; c >= 0; c--){
							FormPoints[la % FormPointCnt] = SourcePoints[(c+Number) % SourcePointCnt]*SourceMG*IFormSplineML;
							la = la - Seg[c].cnt;
						}
					}
				break;
				//!!!!!!!!!!!!!!!!!!!!!!!!!!
				case 1://Vertex Normal
					if (Particles) {//PARTIKEL
						la = FormPointCnt-1;
						for (c=FormSegCnt-1; c >= 0; c--){
							nu = (c+Number) % ParticleCnt;
							FormPoints[la % FormPointCnt] = Particles[nu].off*IFormSplineML;
							FormPoints[(la-1) % FormPointCnt] = (vnorm(Particles[nu].v3)*10)+Particles[nu].off*IFormSplineML;
							la = la - Seg[c].cnt;
						}
					}
					if (SourcePoints){//PUNKTE
						GetVertexNormalSafe(&VertexNormalArray,ToPoly(SourceOb)); if (!VertexNormalArray) break;
						la = FormPointCnt-1;
						for (c=FormSegCnt-1; c >= 0; c--){
							nu = (c+Number) % SourcePointCnt;
							FormPoints[la % FormPointCnt] = SourcePoints[nu]*SourceMG*IFormSplineML;
							FormPoints[(la-1) % FormPointCnt] = ((VertexNormalArray[nu]*10)+SourcePoints[nu])*SourceMG*IFormSplineML;
							la = la - Seg[c].cnt;
						}
						GeFree(VertexNormalArray);
					}
				break;
				//!!!!!!!!!!!!!!!!!!!!!!!!!!
				}
				FormSplineOb->Message(MSG_UPDATE);
			}//if (FormSegCnt == 0){
		}
	}

	{goto Error; }
	//######################################################################
}break;


case 1:{//PUNKTE
	//#####################################################################
	PointObject		*FormOb = NULL;//Emiter Start Point Object
	LONG			FormPointCnt = 0;
	Vector			*FormPoints = NULL;//Array
	//Form Name
	String			FormObName = GetNumName(data,PTP_POITOBJECT,PTP_POITOBJECT_NUM);
	//Find Form Object
    FormOb = (PointObject*)doc->SearchObject(FormObName);
    if (!FormOb){
		FormOb = (PointObject*)op->up();if (!FormOb) {GePrint("PTP: Point Fix-Object "+FormObName+" is not found!"); goto Error; }
	}

	Matrix  FormML=FormOb->GetMl(), IFormML=!FormML;
	Matrix	FormMG=FormOb->GetMg();
	Matrix  FormMGU=FormOb->GetUpMg();
	Vector	FormPos = FormOb->GetPos()*FormMGU;//TEST
	//Matrix	EmitterML = EmiterOb->GetMl(), IEmitterML=!EmitterML;

	if (FormOb->GetLayerType()&L_pointobject){//If Form is Point Object
		//FORM
		FormPointCnt = FormOb->GetPointCount();if (FormPointCnt<=0) {goto Error; }
		FormPoints = FormOb->GetPoint();if (!FormPoints) {goto Error; }

		if (Particles){//Source Particle Object
			//EMITTER DATA
			/*
			BaseContainer EMdata = EmiterOb->GetData();
			LONG BirtRE = EMdata.GetLong(PARTICLEOBJECT_BIRTHRAYTRACER);
			LONG BirtED = EMdata.GetLong(PARTICLEOBJECT_BIRTHEDITOR);
			LONG PointsPerFrame = FormPointCnt * fps;//ORIGENAL
		
			if ((BirtRE != PointsPerFrame) || (BirtED != PointsPerFrame)){
				EMdata.SetLong(PARTICLEOBJECT_BIRTHEDITOR, PointsPerFrame);
				EMdata.SetLong(PARTICLEOBJECT_BIRTHRAYTRACER, PointsPerFrame);

				EmiterOb->SetData(EMdata);
				EmiterOb->Message(MSG_UPDATE);
			}
			*/
			//EMITTER DATA

			for (LONG c = 0; c < FormPointCnt; c++){
				FormPoints[c] = Particles[(c+Number) % ParticleCnt].off*IFormML;
			}
			FormOb->Message(MSG_UPDATE);
		}//if (Particles){

		if (SourcePoints){//Source Point Objekt
			for (LONG c = 0; c < FormPointCnt; c++){
				FormPoints[c] = SourcePoints[(c+Number) % SourcePointCnt]*SourceMG*IFormML;
			}
			FormOb->Message(MSG_UPDATE);
		}//if (SourcePoints){

	}//if (FormOb->GetLayerType()&L_pointobject){/

	//######################################################################
}break;


case 2:{//GROUP
	//######################################################################
	PointObject		*FormOb = NULL;
	String			FormObName = data.GetString(PTP_POITOBJECT);

	if (Particles){
		for (LONG c = 0; c < ParticleCnt; c++){
			FormOb = (PointObject*)doc->SearchObject(FormObName+LongToString(c));if (!FormOb) continue;
			nu = (c+Number) % ParticleCnt;
			FormOb->SetPos(Particles[nu].off);
			FormOb->SetEditorMode((Particles[nu].bits & PARTICLE_VISIBLE)?MODE_UNDEF:MODE_OFF);
			FormOb->SetRenderMode((Particles[nu].bits & PARTICLE_VISIBLE)?MODE_UNDEF:MODE_OFF);		
		}
	}

	if (SourcePoints){
		for (LONG c = 0; c < SourcePointCnt; c++){
			FormOb = (PointObject*)doc->SearchObject(FormObName+LongToString(c));if (!FormOb) continue;
			FormOb->SetPos(SourcePoints[(c+Number) % SourcePointCnt]*SourceMG);
		}
	}
	//######################################################################
}break;//GROUP


case 4:{//An Last Point
	//######################################################################
	SplineObject	*SourceSplineOb = NULL;
	LONG			SourcePointCnt = 0;
	Vector			*SourcePoints = NULL;//Array
	LONG			SourceSegCnt = 0;
	Segment			*Seg = NULL;

	//Form Name
	PointObject		*ClothOb = NULL;
	String ClothObName = GetNumName(data,PTP_POITOBJECT,PTP_POITOBJECT_NUM);
    ClothOb = (PointObject*)doc->SearchObject(ClothObName);if (!ClothOb){goto Error;}
	LONG			ClothPointCnt = ClothOb->GetPointCount();
	Vector			*ClothPoints = ClothOb->GetPoint(); if (!ClothPoints) {goto Error;}

	if (SourceOb->GetType() == Ospline){
		SourceSplineOb = ToSpline(SourceOb);
		SourcePointCnt = SourceSplineOb->GetPointCount();//<<<<<<

		if (SourcePointCnt>0){
			SourcePoints =  SourceSplineOb->GetPoint();if (!SourcePoints) {goto Error; }
			//Matrix	ClothSplineML=ClothOb->GetMl(), IClothSplineML=!ClothSplineML; 
			Matrix	ClothSplineMG = ClothOb->GetMg();
			//Matrix	ClothSplineMGU=ClothOb->GetUpMg();
			SourceSegCnt = SourceSplineOb->GetSegmentCount();
			if (SourceSegCnt > 0){
				Seg = SourceSplineOb->GetSegment();if (!Seg) {goto Error; }
					if (SourcePoints){
						la = SourcePointCnt-1;
						for (c=SourceSegCnt-1; c >= 0; c--){
							ClothPoints[c % ClothPointCnt] = SourcePoints[la % SourcePointCnt]*ClothSplineMG;
							la = la - Seg[c].cnt;
						}
					}
				ClothOb->Message(MSG_UPDATE);
			}//if (FormSegCnt == 0){
		}
	}

	{goto Error; }
	//######################################################################
}break;


}//swich(SysTyp)

Error:
GeFree(VertexNormalArray);

Vector *tt = NULL;
GeFree(tt);

return TRUE;
}
//###########################################################################


static void Speedup(PluginTag *tag, BaseDocument *doc, BaseObject *op)
{
	op->SetActiveDependence();
}


static BaseBitmap *icon=NULL;
static String *help=NULL;

Bool RegisterPTPTag(void)
{
	// decide by name if the plugin shall be registered - just for user convenience
	String name=GeLoadString(IDS_PTP); if (!name.Content()) return TRUE;


	help = gNew String();
	icon = AllocBaseBitmap();
	if (!icon || icon->Init(GeGetPluginPath()+String("res")+String("ptp1.tif"))!=IMAGE_OK) {GePrint("PTP: ptp1.tif is not loaded!");return FALSE;}
	//*help = GeLoadString(IDS_LOOKATCAMERAHELP);

	TAGPLUGIN cs;
	ClearMem(&cs,sizeof(cs));

	cs.add_menu				= TRUE;
	//cs.help				= help->Get();
	cs.icon					= icon;
	cs.info					= TAG_EXPRESSION|TAG_VISIBLE|TAG_MULTIPLE;
	cs.AllocSettings		= AllocSettings;
	cs.EditSettings			= EditSettings;
	cs.Execute				= Execute;
	cs.Speedup				= Speedup;
	// be sure to use a unique ID obtained from www.plugincafe.com
	return GeRegisterPlugin(C4DPL_TAG,PTP_ID,name,&cs,sizeof(cs));
}

void FreePTPTag(void)
{
	FreeBaseBitmap(icon);
	gDelete(help);
}
