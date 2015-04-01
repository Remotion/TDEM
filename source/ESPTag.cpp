/////////////////////////////////////////////////////////////
// Point Particles											   //
/////////////////////////////////////////////////////////////
// VERSION  1.0  										   //
/////////////////////////////////////////////////////////////
// (c) 2002 Remotion, all rights reserved				   //
/////////////////////////////////////////////////////////////

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
//#define PTP_ID				1007708
//-----------------------------------
#define ESP_ID				1007709
//-----------------------------------

#define MAX_OVERSAMPLING	64

#define ESP_ELAST			1001
#define ESP_RESIS			1002

#define ESP_MASSE			1004
#define ESP_GRAVITI			1005 //met

#define ESP_ANIM			1006

#define ESP_SYST1			1007

#define ESP_AXSEGLEN		1003 //met
#define ESP_INSEGLEN		1008 //met

#define ESP_COLL			1011 //STRING
#define ESP_OVER			1013 //LONG
#define ESP_RAD1			1012 //REAL
#define ESP_PRAD2			1014 //REAL

#define OB_TIME				4001
#define OB_MEMSIZE			5001


//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
class ESPDialog : public GeModalDialog
{
	private:
		BaseContainer *data;
	public:
		ESPDialog(BaseContainer *bc) { data = bc; }

		virtual Bool CreateLayout(void);
		virtual Bool InitValues(void);
		virtual Bool Command(LONG id, const BaseContainer &msg);
		//virtual LONG Message(const BaseContainer &msg,BaseContainer &result);
};

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
Bool ESPDialog::CreateLayout(void)
{
	return GeModalDialog::CreateLayout() && LoadDialogResource(DLG_ESP,NULL,0);
	//return GeDialog::CreateLayout() && LoadDialogResource(DLG_ESP,NULL,0);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
Bool ESPDialog::InitValues(void)
{
	//Bool bo;
	//String st = "";

	// first call the parent instance
	if (!GeModalDialog::InitValues()) return FALSE;
	//if (!GeDialog::InitValues()) return FALSE;

	SetPercent(IDC_ESP_ELAST,data,ESP_ELAST,0,200.0,0.1);
	SetReal(IDC_ESP_RESIS,data,ESP_RESIS,0,MAXRANGE,1.0);


	SetMeter(IDC_ESP_PPLEN,data,ESP_AXSEGLEN,0,MAXRANGE,1.0);
	SetMeter(IDC_ESP_MPLEN,data,ESP_INSEGLEN,0,MAXRANGE,1.0);

	SetReal(IDC_ESP_MASSE,data,ESP_MASSE,-MAXRANGE,MAXRANGE,1.0);
	//Enable(IDC_ESP_MASSE,FALSE);

	SetVectorMeter(IDC_ESP_GRAVITI_X,IDC_ESP_GRAVITI_Y,IDC_ESP_GRAVITI_Z,data,ESP_GRAVITI,-MAXRANGE,MAXRANGE,1.0);

	SetBool(IDC_ESP_ANIM,data,ESP_ANIM);

	SetLong(IDC_ESP_SYST1,data,ESP_SYST1);

	//Collision
	SetString(IDC_ESP_COLL,data,ESP_COLL);
	SetLong(IDC_ESP_OVER,data,ESP_OVER,1,MAX_OVERSAMPLING,1);
	SetMeter(IDC_ESP_RAD1,data,ESP_RAD1,0,MAXRANGE,1.0);
	SetMeter(IDC_ESP_PRAD2,data,ESP_PRAD2,0.1,MAXRANGE,1.0);

	return TRUE;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
Bool ESPDialog::Command(LONG id, const BaseContainer &msg)
{
	//Bool bo;
	String st = "";

	switch (id){
		case IDC_OK:
		GetReal(IDC_ESP_ELAST,data,ESP_ELAST);
		GetReal(IDC_ESP_RESIS,data,ESP_RESIS);

		GetReal(IDC_ESP_PPLEN,data,ESP_AXSEGLEN);
		GetReal(IDC_ESP_MPLEN,data,ESP_INSEGLEN);

		GetReal(IDC_ESP_MASSE,data,ESP_MASSE);

		GetVector(IDC_ESP_GRAVITI_X,IDC_ESP_GRAVITI_Y,IDC_ESP_GRAVITI_Z,data,ESP_GRAVITI);
		
		GetBool(IDC_ESP_ANIM,data,ESP_ANIM);

		GetLong(IDC_ESP_SYST1,data,ESP_SYST1);

		//Collision
		GetString(IDC_ESP_COLL,data,ESP_COLL);
		GetLong(IDC_ESP_OVER,data,ESP_OVER);
		GetReal(IDC_ESP_RAD1,data,ESP_RAD1);
		GetReal(IDC_ESP_PRAD2,data,ESP_PRAD2);
		break;
	}

	return TRUE;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
static Bool AllocSettings(PluginTag *tag)
{
	BaseContainer bc;

	bc.SetReal(ESP_ELAST,0.5);
	bc.SetReal(ESP_RESIS,8.0);

	bc.SetReal(ESP_AXSEGLEN,10);
	bc.SetReal(ESP_INSEGLEN,0);

	bc.SetReal(ESP_MASSE,1.0);
	bc.SetVector(ESP_GRAVITI,Vector(0.0,5.0,0.0));

	bc.SetBool(ESP_ANIM,TRUE);

	bc.SetLong(ESP_SYST1,0);

	//Collision
	bc.SetString(ESP_COLL,"");
	bc.SetLong(ESP_OVER,1);
	bc.SetReal(ESP_RAD1,20);
	bc.SetReal(ESP_PRAD2,1);

	tag->SetData(bc);//SD
	
	return TRUE;//SD
}

/*//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
LONG ESPDialog::Message(const BaseContainer &msg,BaseContainer &result)
{
//	switch (msg.GetId())
	{
	}
	return GeDialog::Message(msg,result);
}
*/

//################################# EDIT SETING ###################################
static Bool EditSettings(PluginTag *tag)
{
	BaseContainer data = tag->GetData();
	ESPDialog dlg(&data);

	BaseObject *FormSplineOb = tag->GetObject();
	if (!(FormSplineOb->GetLayerType()&L_pointobject)) return FALSE;
	if (!dlg.Open()) return FALSE;

	//if (!dlg.Open(FALSE,PLUGINS_ID)) return FALSE;
	//BaseObject *op = GetObject();

	tag->SetData(data);
	return TRUE;
}

//##################################### Execute #####################################
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
static Bool Execute(PluginTag *tag, BaseDocument *doc, BaseObject *op)
{
	BaseContainer data = tag->GetData();

	if (op->GetDeformMode()==MODE_ON) {return TRUE;}//TEST
	
	//ANIM
	BaseTime	myobject_lasttime = data.GetTime(OB_TIME);
	Bool		onem = data.GetBool(ESP_ANIM);
	if (onem){
		if (doc->GetTime()!=myobject_lasttime){
			myobject_lasttime = doc->GetTime();
			data.SetTime(OB_TIME,myobject_lasttime);
			tag->SetData(data);
		}else{return TRUE;}
	}//ANIM


	LONG		c,o,p,i,la;
	LONG		Syst = data.GetLong(ESP_SYST1);
	//Real		harg = data.GetReal(ESP_MASSE);
	Real		masse = data.GetReal(ESP_MASSE);
	Real		elasticite = data.GetReal(ESP_ELAST)*masse;
	Real		resistance = data.GetReal(ESP_RESIS);
	Real		maXSegLen = data.GetReal(ESP_AXSEGLEN);
	Real		mInSegLen = data.GetReal(ESP_INSEGLEN);
	Vector		graviti = data.GetVector(ESP_GRAVITI);
	//Collision
	String		CollName = data.GetString(ESP_COLL);
	LONG		overSam = data.GetLong(ESP_OVER);
	Real		pointRadius = data.GetReal(ESP_PRAD2);
	Real		ColRadius = data.GetReal(ESP_RAD1)+pointRadius;//???? +1
	Real		ColRadius2	= ColRadius * ColRadius;//??
	Vector		PosDif;

	//.................
	LONG		cof;
	Real		force;
	Real		leng;

	Vector		prev;
	Vector		current;

	Vector		dist;
	Vector		elast;
	Vector		resist;
	Vector		accel;
	//.................

	Matrix			FormMG = op->GetMg(), iFormMG = !FormMG;
	Matrix			FormMGU = op->GetUpMg();
	Matrix			FormML = op->GetMl(), iFormML = !FormML;
	SplineObject	*FormSplineOb = NULL;
	LONG			FormPointCnt = 0;
	Vector			*FormPoints = NULL;//Array
	LONG			FormSegCnt = 0;

	BaseObject		*ColOb = NULL;//TEST 1
	BaseObject		*SpherOb = NULL;
	Real			coldist = 0;//TEST 1
	Vector			ColPos;


	Matrix			MG;
	LONG			ParticlCnt;
	Particle		*Particles = NULL;

	//MASTER OB
	/*
	PointObject		*masterOb = NULL;
	LONG			masterPcnt;
	Vector			*masterPoints = NULL;
	*/

	
	//---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P
	if (op->GetType() == Opolygon){ //POLYGON
		PolygonObject	*FormOb = NULL;
		LONG			FormPolyCnt = 0;
		CPolygon			*FormPolys = NULL;
		Neighbor		NG;
		LONG			*dadr;//Array
		LONG			dcnt;
		LONG			polyn;
		LONG			pn;

		FormOb = (PolygonObject*)op;

		FormPointCnt = FormOb->GetPointCount();//<<<<<<
		FormPoints =  FormOb->GetPoint(); if (!FormPoints) return TRUE;

		FormPolyCnt = FormOb->GetPolygonCount();//<<<<<<
		FormPolys =  FormOb->GetPolygon(); if (!FormPoints) return TRUE;
		
		if (!NG.Init(FormPointCnt,FormPolys,FormPolyCnt,NULL)) return TRUE;

		//MASTER OB
		/*
		masterOb = (PointObject*)FormOb->down();if (!masterOb) return TRUE;
		masterPcnt = masterOb->GetPointCount();
		masterPoints = masterOb->GetPoint();if (!masterPoints) return TRUE;
		if (FormPointCnt != masterPcnt) return TRUE;
		*/

		for (c=0;c<FormPointCnt;c++){

			//TEST SPRINGSTEST SPRINGSTEST SPRINGS SPRINGSTEST SPRINGS??????
			NG.GetPointPolys(c,&dadr,&dcnt);

			elast=Vector(0.0,0.0,0.0);//???
			current = FormPoints[c];

			for(p=0;p<dcnt;p++){
				polyn = dadr[p];//Polygon number, (number-p of all polys o point) on point number-i

				//WORK !!!!!!!! ABER INSTABIL ??????
				if (FormPolys[polyn].d == c) pn = FormPolys[polyn].c;
				if (FormPolys[polyn].c == c) pn = FormPolys[polyn].b;
				if (FormPolys[polyn].b == c) pn = FormPolys[polyn].a;
				if (FormPolys[polyn].a == c) pn = FormPolys[polyn].d;

				    prev = FormPoints[pn];
					//MASTER LEN ??
					//maXSegLen = Len(masterPoints[pn]-masterPoints[c]);
					//MASTER ??
					dist = prev - current;
  					leng = Len(dist);
					//Max Len
					if (leng > maXSegLen){
						force = elasticite * (leng - maXSegLen);
						elast += (dist / leng) * force;
					}//if (leng > longseg){
					//Min Len TEST-TEST ???
					if (leng < mInSegLen){
						//force = elasticite * (leng - maXSegLen);
						force = elasticite * (leng - mInSegLen);
						elast += (dist / leng) * force;
					}
			}

			accel = elast / masse;//ORIGENAL OK
			accel -= graviti;

			FormPoints[c] += accel;
		}//for (c=0;c<FormPointCnt;c++){


	}//if (op->GetLayerType()&L_pointobject)
	//---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P


	


	//---S---S---S---S---S---S---S---S---S---S---S---S---S---S---S---S---S---S---S
	if (op->GetType() == Ospline){ //SPLINE
		FormSplineOb = ToSpline(op);

		//---------------------------------- Collision
		ColOb = doc->SearchObject(CollName);// if (!ColOb) return TRUE;//TEST 1
		if (ColOb && ColOb->GetType()==Oparticle){
			ParticleTag	*PPPTag = NULL;
			//Get Particle Tag
			PPPTag = (ParticleTag*)ColOb->GetFirstTag();
			while (PPPTag->GetType()!=Tparticle){
				PPPTag = (ParticleTag*)((BaseTag*)PPPTag)->next();
			}
			if (PPPTag){
				ParticlCnt = ((ParticleObject*)ColOb)->GetParticleCount();
				Particles = ((ParticleObject*)ColOb)->GetParticle(PPPTag,0);//if (!Particles) return TRUE;
			}
		}
		//----------------------------------

		FormPointCnt = FormSplineOb->GetPointCount();//<<<<<<
		FormPoints =  FormSplineOb->GetPoint(); if (!FormPoints) {return TRUE;}
		
		FormSegCnt = FormSplineOb->GetSegmentCount();
		Segment *Seg = FormSplineOb->GetSegment();if (!Seg) {return TRUE; }
		//Dont Work Witch one Segment

		/*
		if (FormSegCnt == 0){
			FormSegCnt = 1;
			//Seg = gNew Segment;
			Seg[0].cnt = FormPointCnt;
			Seg[0].closed = FALSE;
		}
		*/

		switch(Syst){
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		case 0://Einfach
		la = 0;
		for (c=0; c < FormSegCnt; c++){
			for (i = la+1; i < la+Seg[c].cnt; i++){//ORIGENAL OK

				elast=Vector(0.0,0.0,0.0);
				current = FormPoints[i];

				if (i > la){
					prev = FormPoints[i-1];
					dist = prev - current;
  					leng = Len(prev-current);
					if (leng > maXSegLen){
						force = elasticite * (leng - maXSegLen);
						elast = elast + (dist / leng) * force;
					}
				}//	if (i>0){
				//SIMPEL
				accel = elast / masse - graviti;//ORIGENAL OK
				FormPoints[i] = FormPoints[i] + accel;
			}
			la = la + Seg[c].cnt;
		}
		break;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		case 1://Hair 1
		la = 0;
		for (c=0; c < FormSegCnt; c++){

			//TESt
			Vector nor = vnorm(FormPoints[la]-FormPoints[la+1])*resistance;//?????
			//TEST

			//for (LONG i = la+1; i < la+Seg[c].cnt; i++){//ORIGENAL OK
			for (i = la+2; i < la+Seg[c].cnt; i++){//TEST

				elast=Vector(0.0,0.0,0.0);
				current = FormPoints[i];

				if (i > la){
					prev = FormPoints[i-1];
					dist = prev - current;
  					leng = Len(prev-current);
					if (leng > maXSegLen){
						force = elasticite * (leng - maXSegLen);
						elast = elast + (dist / leng) * force;
					}
				}//	if (i>0){

				/*
				if (i < la+Seg[c].cnt-1){
					prev = FormPoints[i+1];
					dist = prev - current;
					leng = Len(prev - current);
					if (leng > longseg){
						force = elasticite * (leng - longseg);
						elast = elast + (dist / leng) * force;
					}
				}
				*/

				//resist = -dep[i] * resistance;
				//accel = (elast + resist) / masse - graviti;
				//dep[i] = dep[i] + dt*accel;
				//Points[i] = Points[i] + dep[i];  

				//TEST
					accel = elast / masse - (graviti * (i-la)) - nor;
				//TEST
		
				//SIMPEL
				//accel = elast / masse - graviti;//ORIGENAL OK
				FormPoints[i] = FormPoints[i] + accel;

			}
			la = la + Seg[c].cnt;
		}
		break;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		case 2://Schlauch
		cof = 2;
		la = 0;
		for (c=0; c < FormSegCnt; c++){

			Vector nor = vnorm(FormPoints[la+1]-FormPoints[la])*resistance;//TEST
			//Vector nor1 = vnorm(FormPoints[la]-FormPoints[la+1])*resistance;//TEST

			for (i = la+cof; i < la+Seg[c].cnt-cof;i++){//ORIGENAL OK

				elast=Vector(0.0,0.0,0.0);
				current = FormPoints[i];

				if (i > la){
					prev = FormPoints[i-1];
					dist = prev - current;
  					leng = Len(prev-current);
					if (leng > maXSegLen){
						force = elasticite * (leng - maXSegLen);
						elast = elast + (dist / leng) * force;
					}
				}//	if (i>0){

				if (i < la+Seg[c].cnt-1){
					prev = FormPoints[i+1];
					dist = prev - current;
					leng = Len(prev - current);
					if (leng > maXSegLen){
						force = elasticite * (leng - maXSegLen);
						elast = elast + (dist / leng) * force;
					}
				}//	if (i < la+Seg[c].cnt-1)

				//accel = elast / masse - graviti;//ORIGENAL OK
				accel = elast / masse - (graviti * (i-la)) - nor;//TEST
				FormPoints[i] = FormPoints[i] + accel;
			}
			la = la + Seg[c].cnt;
		}
		break;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		case 3://NICHTS
		break;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}//switch(syst){
	}//	if (op->GetType() == Ospline){
	//---S---S---S---S---S---S---S---S---S---S---S---S---S---S---S---S---S---S---S


	//COLLISION--COLLISION--COLLISION--COLLISION--COLLISION--COLLISION--COLLISION
	if (op->GetLayerType()&L_pointobject){
		//------------------- Collision FORBERATUNG---------------
		PointObject		*FormOb = NULL;
		FormOb =		(PointObject*)op;
		FormPointCnt = FormOb->GetPointCount();//<<<<<<
		FormPoints =  FormOb->GetPoint(); if (!FormPoints) return TRUE;
		Vector parPos;
		ParticleTag	*PPPTag = NULL;

		Vector *colPoint = NULL;
		LONG  colPcnt = 0;
		

		ColOb = doc->SearchObject(CollName);if (!ColOb) return TRUE;//TEST 1
		Matrix colMG = ColOb->GetMg();

		switch (ColOb->GetType()){
		//---------------------------------------------------------------------
		case Oparticle:
			//Get Particle Tag
			PPPTag = (ParticleTag*)ColOb->GetTag(Tparticle);if (!PPPTag) return TRUE;
			ParticlCnt = ((ParticleObject*)ColOb)->GetParticleCount();
			Particles = ((ParticleObject*)ColOb)->GetParticle(PPPTag,0);//if (!Particles) return TRUE;

			//Fast Particle Collision !!!
			for (o=0; o<overSam; o++){//Ovrsampling
				for (i=0;i<FormPointCnt;i++){
					for (p=0;p<ParticlCnt;p++){
						if (!(Particles[p].bits & (PARTICLE_VISIBLE|PARTICLE_ALIVE))) continue;
						parPos = Particles[p].off*iFormMG;//nicht Global Matrix OK
						PosDif = FormPoints[i]-parPos;
						if (PosDif*PosDif < ColRadius2){
							FormPoints[i] = parPos+vnorm(PosDif)*ColRadius;
							//Particles[p].bits = 0;//ANDERN !!!! DONT WORK ???
						}
					}//for (LONG p=0;p<ParticlCnt;p++)
				}//for (c=0;c<FormPointCnt;c++){
			}//for (o=0; o<overSam; o++)

		break;
		case Opolygon:
			colPoint = ToPoly(ColOb)->GetPoint();if (!colPoint) return TRUE;
			colPcnt = ToPoly(ColOb)->GetPointCount();
			//Fast Particle Collision !!!
			for (o=0; o<overSam; o++){//Ovrsampling
				for (i=0;i<FormPointCnt;i++){
					for (p=0;p<colPcnt;p++){

						parPos = colPoint[p]*colMG*iFormMG;//nicht Global Matrix OK
						PosDif = FormPoints[i]-parPos;
						if (PosDif*PosDif < ColRadius2){
							FormPoints[i] = parPos+vnorm(PosDif)*ColRadius;
						}
					}//for (LONG p=0;p<ParticlCnt;p++)
				}//for (c=0;c<FormPointCnt;c++){
			}//for (o=0; o<overSam; o++)
		break;
		//---------------------------------------------------------------------
		default:
			//Slow ???
			if (ColOb) {
				for (o=0; o<overSam; o++){//Ovrsampling
					SpherOb = ColOb->down();
					while (SpherOb){
						//ColPos = SpherOb->GetPos()*iFormML*(SpherOb->GetUpMg());//???
						ColPos = SpherOb->GetPos()*colMG*iFormMG;
						if (SpherOb->GetType()==Oprimitive) ColRadius = SpherOb->GetDataInstance().GetReal(PRIM_SPHERE_RAD)+pointRadius;//????? +1
						else ColRadius = 0; //????
						ColRadius2 = ColRadius * ColRadius;

						for (i=0;i<FormPointCnt;i++){
							//Kugel Collision Fast !!!!
							PosDif = FormPoints[i]-ColPos;
							if (PosDif*PosDif < ColRadius2) FormPoints[i]=ColPos+vnorm(PosDif)*ColRadius;
						}//for (i=0;i<FormPointCnt;i++){
	
						SpherOb = SpherOb->next();//if (!SpherOb) break;
					}//while (SpherOb){
				}//	for (o=0; o<overSam; o++){//Ovrsampling
			}//if (ColOb) {
		break;
		//---------------------------------------------------------------------
		}
	}
	//COLLISION--COLLISION--COLLISION--COLLISION--COLLISION--COLLISION--COLLISION


	op->Message(MSG_UPDATE);//UPDATE 
	return TRUE;
}



/*
//DRAW ???    (DRAW PARTIKEL KOLLISION RADIUS ???)
static	Bool Draw(PluginTag *tag, BaseObject *op, BaseDraw *bd, BaseDrawHelp *bh)
{

	//BaseObject *cam = bd->GetEditorCamera();
	//Matrix camM	= cam->GetMg();
	LONG ParticlCnt;
	Particle *Particles = NULL;
	bd->SetPen(Vector(1.0,0.0,0.0));

	BaseDocument *doc = bh->GetDocument();
	ParticleObject *ColOb = NULL;
	String CollName = "Coll";

		ColOb = (ParticleObject*)doc->SearchObject(CollName);// if (!ColOb) return TRUE;//TEST 1
		if (ColOb && ColOb->GetType()==Oparticle){
			Matrix m = ColOb->GetMg();

			m.v1 = Vector(1,10,10);
			m.v2 = Vector(10,1,10);
			m.v3 = Vector(10,10,1);

			ParticleTag	*PPPTag = NULL;
			//Get Particle Tag
			PPPTag = (ParticleTag*)ColOb->GetFirstTag();
			while (PPPTag->GetType()!=Tparticle){
				PPPTag = (ParticleTag*)((BaseTag*)PPPTag)->next();
			}
			if (PPPTag){
				ParticlCnt = ColOb->GetParticleCount();
				Particles = ColOb->GetParticle(PPPTag,0);//if (!Particles) return TRUE;
				
				for (LONG c=0;c<ParticlCnt;c++){
					if (!(Particles[c].bits & PARTICLE_VISIBLE)) continue;
					m.off = Particles[c].off;
					bd->Circle3D(m);
				}
			
			}
		}
	return TRUE;
}
*/



////###############  SPEEDUP ??? ####################
static void Speedup(PluginTag *tag, BaseDocument *doc, BaseObject *op)
{
	op->SetActiveDependence();
}

static BaseBitmap *icon=NULL;
static String *help=NULL;


//###############  REGISTER ESP TAG ####################
Bool RegisterESPTag(void)
{
	// decide by name if the plugin shall be registered - just for user convenience
	String name=GeLoadString(IDS_ESP); if (!name.Content()) return TRUE;


	help = gNew String();
	icon = AllocBaseBitmap();
	if (!icon || icon->Init(GeGetPluginPath()+String("res")+String("esp1.tif"))!=IMAGE_OK) {GePrint("ESP: esp1.tif is not loaded!");return FALSE;}
	//*help = GeLoadString(IDS_LOOKATCAMERAHELP);

	TAGPLUGIN cs;
	ClearMem(&cs,sizeof(cs));

	cs.add_menu				= TRUE;
	//cs.help				= help->Get();
	cs.icon					= icon;
	cs.info					= TAG_EXPRESSION|TAG_VISIBLE|TAG_MULTIPLE;
	cs.AllocSettings		= AllocSettings;
	//----
	/*
	cs.FreeSettings			= FreeSettings;
	cs.CloneSettings		= CloneSettings;
	cs.ReadSettings			= ReadSettings;
	cs.WriteSettings		= WriteSettings;
	*/
	//-----
	cs.EditSettings			= EditSettings;
	cs.Execute				= Execute;
	//cs.Draw					= Draw; // TEST
	cs.Speedup				= Speedup;
	// be sure to use a unique ID obtained from www.plugincafe.com
	return GeRegisterPlugin(C4DPL_TAG,ESP_ID,name,&cs,sizeof(cs));
}


//###############  FREE ESP TAG ####################
void FreeESPTag(void)
{
	FreeBaseBitmap(icon);
	gDelete(help);
}
