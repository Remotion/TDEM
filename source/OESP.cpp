/////////////////////////////////////////////////////////////
// 3d Emiter											   //
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
#include "c4d_basetag.h"
#include "c4d_baseobject.h"
#include "c4d_basedocument.h"
#include "c4d_basebitmap.h"
#include "c4d_basedraw.h"
#include "c4d_resource.h"
#include "c4d_symbols.h"
#include "c4d_gui.h"

#include "mytools.h"
#include "myprint.h"

//--------------------------------------
#define PLUGINS_ID			1008035
//--------------------------------------
#define MEM_TAG_ID			1008036 //MEM_TAG_ID

#define MAX_OVERSAMPLING	64

#define OSP_ELAST			1001
#define OSP_RESIS			1002

#define OSP_MASSE			1004
#define OSP_GRAVITI			1005 //met

#define OSP_ANIM			1006

#define OSP_SYST1			1007

#define OSP_AXSEGLEN		1003 //met
#define OSP_INSEGLEN		1008 //met

#define OSP_COLL			1011 //STRING
#define OSP_OVER			1013 //LONG
#define OSP_RAD1			1012 //REAL
#define OSP_PRAD2			1014 //REAL

#define OB_MEMSIZE			5001

// object dialog
class AtomDialog : public GeModalDialog
{
	private:
		BaseContainer *data;
	public:
		AtomDialog(BaseContainer *bc) { data = bc; }

		virtual Bool CreateLayout(void);
		virtual Bool InitValues(void);
		virtual Bool Command(LONG id, const BaseContainer &msg);
};

// load and create dialog
Bool AtomDialog::CreateLayout(void)
{
	return GeModalDialog::CreateLayout() && LoadDialogResource(DLG_OESP,NULL,0);
}

// initialize dialog values
Bool AtomDialog::InitValues(void)
{
	// first call the parent instance
	if (!GeModalDialog::InitValues()) return FALSE;

	SetPercent(IDC_OSP_ELAST,data,OSP_ELAST,0,200.0,0.1);
	SetReal(IDC_OSP_RESIS,data,OSP_RESIS,-MAXRANGE,MAXRANGE,1.0);

	SetMeter(IDC_OSP_PPLEN,data,OSP_AXSEGLEN,-MAXRANGE,MAXRANGE,1.0);
	SetMeter(IDC_OSP_MPLEN,data,OSP_INSEGLEN,-MAXRANGE,MAXRANGE,1.0);

	SetReal(IDC_OSP_MASSE,data,OSP_MASSE,-MAXRANGE,MAXRANGE,1.0);
	Enable(IDC_OSP_MASSE,FALSE);

	SetVectorMeter(IDC_OSP_GRAVITI_X,IDC_OSP_GRAVITI_Y,IDC_OSP_GRAVITI_Z,data,OSP_GRAVITI,-MAXRANGE,MAXRANGE,1.0);

	SetBool(IDC_OSP_ANIM,data,OSP_ANIM);

	SetLong(IDC_OSP_SYST1,data,OSP_SYST1);

	//Collision
	SetString(IDC_OSP_COLL,data,OSP_COLL);
	SetLong(IDC_OSP_OVER,data,OSP_OVER,1,MAX_OVERSAMPLING,1);
	SetMeter(IDC_OSP_RAD1,data,OSP_RAD1,0,MAXRANGE,1.0);
	SetMeter(IDC_OSP_PRAD2,data,OSP_PRAD2,0.1,MAXRANGE,1.0);

	//AddButton(BUTTON1,BFH_FIT,50,20,"NEW");
	//AddButton(BUTTON2,BFH_FIT,50,20,"GET");

	return TRUE;
}

// read dialog values when user presses OK
Bool AtomDialog::Command(LONG id, const BaseContainer &msg)
{
	switch (id)
	{
		case IDC_OK: 
			GetReal(IDC_OSP_ELAST,data,OSP_ELAST);
			GetReal(IDC_OSP_RESIS,data,OSP_RESIS);

			GetReal(IDC_OSP_PPLEN,data,OSP_AXSEGLEN);
			GetReal(IDC_OSP_MPLEN,data,OSP_INSEGLEN);

			GetReal(IDC_OSP_MASSE,data,OSP_MASSE);

			GetVector(IDC_OSP_GRAVITI_X,IDC_OSP_GRAVITI_Y,IDC_OSP_GRAVITI_Z,data,OSP_GRAVITI);
		
			GetBool(IDC_OSP_ANIM,data,OSP_ANIM);

			GetLong(IDC_OSP_SYST1,data,OSP_SYST1);

			//Collision
			GetString(IDC_OSP_COLL,data,OSP_COLL);
			GetLong(IDC_OSP_OVER,data,OSP_OVER);
			GetReal(IDC_OSP_RAD1,data,OSP_RAD1);
			GetReal(IDC_OSP_PRAD2,data,OSP_PRAD2);
			break;
	}
	
	return TRUE;
}

static Bool EditSettings(PluginObject *op)
{
	// copy data
	BaseContainer data = op->GetData();
	AtomDialog dlg(&data);
	if (!dlg.Open()) return FALSE;
	// copy data back if OK was pressed
	op->SetData(data);
	return TRUE;
}

// initialize settings
static Bool AllocSettings(PluginObject *op)
{	
	BaseContainer data;
	data.SetReal(OSP_ELAST,0.5);
	data.SetReal(OSP_RESIS,8.0);

	data.SetReal(OSP_AXSEGLEN,10);
	data.SetReal(OSP_INSEGLEN,0);

	data.SetReal(OSP_MASSE,1.0);
	data.SetVector(OSP_GRAVITI,Vector(0.0,5.0,0.0));

	data.SetBool(OSP_ANIM,TRUE);

	data.SetLong(OSP_SYST1,0);

	//Collision
	data.SetString(OSP_COLL,"");
	data.SetLong(OSP_OVER,1);
	data.SetReal(OSP_RAD1,20);
	data.SetReal(OSP_PRAD2,1);
	op->SetData(data);


	// local data not stored in containers is in 99% absolutely unnecessary and only complicates things
	// this is just to show the usage...
	String *mem=gNew String("TEST");
	op->SetPluginData(mem);

	return mem!=NULL;
	//return TRUE;
}

static void FreeSettings(PluginObject *op)
{	
	// local data not stored in containers is in 99% absolutely unnecessary and only complicates things
	// this routine normally doesn't have to be implemented

	//print("osp free");
	String *mem=(String*)op->GetPluginData();
	gDelete(mem);
}

static Bool CloneSettings(PluginObject *src, PluginObject *dst)
{	
	// local data not stored in containers is in 99% absolutely unnecessary and only complicates things
	// this routine normally doesn't have to be implemented

	///print("osp clone");

	String *memsrc=(String*)src->GetPluginData();
	String *memdst=gNew String(*memsrc);	
	if (!memdst) return FALSE;
	dst->SetPluginData(memdst);
	return TRUE;
}


static Bool ReadSettings(PluginObject *op, HyperFile *hf, LONG level)
{	
	// local data not stored in containers is in 99% absolutely unnecessary and only complicates things
	// this routine normally doesn't have to be implemented

	//print("osp read");
	if (level>=0)
	{
		String *mem = gNew String;
		if (!mem) return FALSE;
		hf->ReadString(mem);
		op->SetPluginData(mem);
	}
	return TRUE;
}


static Bool WriteSettings(PluginObject *op, HyperFile *hf)
{	
	// local data not stored in containers is in 99% absolutely unnecessary and only complicates things
	// this routine normally doesn't have to be implemented
	//print("osp write");

	String *dat=(String*)op->GetPluginData();
	hf->WriteString(*dat);
	return TRUE;
}




// build description for parameter animation
static LONG GetDescription(PluginObject *op, LONG id)
{
	/*
	switch (id)
	{
		// these parameters can be scaled by the model tool and animated
		//case ATOMOBJECT_CRAD: 
		//case ATOMOBJECT_SRAD: 
		//	return PARAM_SCALE; 
		
		// this parameter can be animated
		//case ATOMOBJECT_SUB: 
		//	return PARAM_LINEAR;
	}
	*/
	

	return PARAM_TOGGLE;
}

/*
// build a rectangular matrix system with a given normal
static void RectangularSystem(const Vector &n, Vector *v1, Vector *v2)
{
	*v2 = Vector(n.y,n.z,n.x);
	*v1 = !((*v2)%n);
	*v2 = !(n%(*v1));
}
*/

/*
// build a single polygonal atom object
static PolygonObject *BuildPolyHull(PolygonObject *op, const Matrix &ml, Real srad, Real crad, LONG sub, 
																		Real lod, Neighbor *n, BaseThread *bt)
{
	BaseContainer bc,cc;
	LONG     spcnt,svcnt,cpcnt,cvcnt,poff,voff,i,j,a,b,side;
	Vector   *spadr=NULL,*cpadr=NULL,*rpadr=NULL,off,pa,pb;
	Polygon  *svadr=NULL,*cvadr=NULL,*rvadr=NULL;
	UVWTag   *suvw =NULL,*cuvw =NULL,*ruvw =NULL;
	Vector   *padr = op->GetPoint();
	Polygon	 *vadr = op->GetPolygon();
	LONG     pcnt  = op->GetPointCount();
	LONG		 vcnt  = op->GetPolygonCount();
	PolyInfo *pli  = NULL;
	Bool     ok=FALSE;
	Matrix   m;

	// set sphere default values
	bc.SetReal(PRIM_SPHERE_RAD,srad);
	bc.SetReal(PRIM_SPHERE_SUB,sub);

	// set cylinder default values (cylinders are a special case of cone objects)
	cc.SetReal(PRIM_CONE_TRAD,crad);
	cc.SetReal(PRIM_CONE_BRAD,crad);
	cc.SetReal(PRIM_CONE_HEIGHT,1.0);
	cc.SetLong(PRIM_CONE_TYPE,0);
	cc.SetLong(PRIM_CONE_HSUB,1);
	cc.SetLong(PRIM_CONE_SEG,sub);
	cc.SetReal(PRIM_AXIS,4);

	// generate both primitives
	PolygonObject *sphere=(PolygonObject*)GeneratePrimitive(NULL,PRIMITIVE_SPHERE,bc,lod,FALSE,bt),*pp=NULL;
	PolygonObject *cyl=(PolygonObject*)GeneratePrimitive(NULL,PRIMITIVE_CYLINDER,cc,lod,FALSE,bt);
	if (!sphere || !cyl) goto Error;

	spcnt = sphere->GetPointCount();
	svcnt = sphere->GetPolygonCount();
	spadr = sphere->GetPoint();
	svadr = sphere->GetPolygon();
	suvw  = (UVWTag*)sphere->GetTag(Tuvw);

	cpcnt = cyl->GetPointCount();
	cvcnt = cyl->GetPolygonCount();
	cpadr = cyl->GetPoint();
	cvadr = cyl->GetPolygon();
	cuvw  = (UVWTag*)cyl->GetTag(Tuvw);

	// allocate main object
	pp=AllocPolygonObject(spcnt*pcnt+cpcnt*n->GetEdgeCount(),svcnt*pcnt+cvcnt*n->GetEdgeCount());
	if (!pp) goto Error;
	
	// add phong tag
	if (!pp->MakeTag(Tphong)) goto Error;
	
	// add UVW tag
	ruvw=(UVWTag*)pp->MakeVariableTag(Tuvw,pp->GetPolygonCount());
	if (!ruvw) goto Error;
	
	// copy sphere geometry for each point
	rpadr = pp->GetPoint();
	rvadr = pp->GetPolygon();
	poff  = 0;
	voff  = 0;

	for (i=0; i<pcnt; i++)
	{
		// test every 256th time if there has been a user break, delete object in this case
		if (!(i&255) && bt && bt->TestBreak()) goto Error;

		off=padr[i]*ml;
		for (j=0; j<spcnt; j++)
			rpadr[poff+j] = off + spadr[j];

		for (j=0; j<svcnt; j++)
		{
			rvadr[voff+j] = Polygon(svadr[j].a+poff,svadr[j].b+poff,svadr[j].c+poff,svadr[j].d+poff);
			ruvw->Cpy(voff+j,suvw,j);
		}

		poff+=spcnt;
		voff+=svcnt;
	}

	// copy cylinder geometry for each edge
	for (i=0; i<vcnt; i++)
	{
		pli = n->GetPolyInfo(i);

		// test every 256th time if there has been a user break, delete object in this case
		if (!(i&255) && bt && bt->TestBreak()) goto Error;

		for (side=0; side<4; side++)
		{
			// only proceed if edge has not already been processed
			// and edge really exists (for triangles side 2 from c..d does not exist as c==d)
			if (pli->mark[side] || side==2 && vadr[i].c==vadr[i].d) continue;

			switch (side)
			{
				case 0: a=vadr[i].a; b=vadr[i].b; break;
				case 1: a=vadr[i].b; b=vadr[i].c; break;
				case 2: a=vadr[i].c; b=vadr[i].d; break;
				case 3: a=vadr[i].d; b=vadr[i].a; break;
			}

			// build edge matrix
			pa = padr[a]*ml;
			pb = padr[b]*ml;

			m.off=(pa+pb)*0.5;
			RectangularSystem(!(pb-pa),&m.v1,&m.v2);
			m.v3=pb-pa;

			for (j=0; j<cpcnt; j++)
				rpadr[poff+j] = cpadr[j]*m;

			for (j=0; j<cvcnt; j++)
			{
				rvadr[voff+j] = Polygon(cvadr[j].a+poff,cvadr[j].b+poff,cvadr[j].c+poff,cvadr[j].d+poff);
				ruvw->Cpy(voff+j,cuvw,j);
			}

			poff+=cpcnt;
			voff+=cvcnt;
		}
	}

	// update object as point geometry has changed
	pp->Message(MSG_UPDATE);

	ok=TRUE;
Error:
	blDelete(sphere);
	blDelete(cyl);

	if (!ok) blDelete(pp);
	return pp;
}
*/


/*
// build a single isoparm atom object
static LineObject *BuildIsoHull(PolygonObject *op, const Matrix &ml, Real srad, Real crad, LONG sub, 
																Real lod, Neighbor *n, BaseThread *bt)
{
	LONG     poff,soff,i,j,a,b,side;
	Vector   *rpadr=NULL,off,pa,pb;
	Segment  *rsadr=NULL;
	Vector   *padr = op->GetPoint();
	Polygon	 *vadr = op->GetPolygon();
	LONG		 vcnt  = op->GetPolygonCount();
	PolyInfo *pli  = NULL;
	Matrix   m;
	Vector   p[8];

	// allocate isoparm object
	LineObject *pp=AllocLineObject(8*n->GetEdgeCount(),4*n->GetEdgeCount());
	if (!pp) return NULL;
	
	rpadr = pp->GetPoint();
	rsadr = pp->GetSegment();
	poff  = 0;
	soff  = 0;

	p[0]=Vector(-crad,  0.0,-0.5); 
	p[1]=Vector(-crad,  0.0, 0.5); 
	p[2]=Vector( crad,  0.0,-0.5); 
	p[3]=Vector( crad,  0.0, 0.5); 
	p[4]=Vector(  0.0,-crad,-0.5); 
	p[5]=Vector(  0.0,-crad, 0.5); 
	p[6]=Vector(  0.0, crad,-0.5);
	p[7]=Vector(  0.0, crad, 0.5);

	for (i=0; i<vcnt; i++)
	{
		// test every 256th time if there has been a user break, delete object in this case
		if (!(i&255) && bt && bt->TestBreak()) 
		{
			blDelete(pp);
			return NULL;
		}

		pli = n->GetPolyInfo(i);

		for (side=0; side<4; side++)
		{
			// only proceed if edge has not already been processed
			// and edge really exists (for triangles side 2 from c..d does not exist as c==d)
			if (pli->mark[side] || side==2 && vadr[i].c==vadr[i].d) continue;

			switch (side)
			{
				case 0: a=vadr[i].a; b=vadr[i].b; break;
				case 1: a=vadr[i].b; b=vadr[i].c; break;
				case 2: a=vadr[i].c; b=vadr[i].d; break;
				case 3: a=vadr[i].d; b=vadr[i].a; break;
			}

			// build edge matrix
			pa = padr[a]*ml;
			pb = padr[b]*ml;

			m.off=(pa+pb)*0.5;
			RectangularSystem(!(pb-pa),&m.v1,&m.v2);
			m.v3=pb-pa;

			for (j=0; j<8; j++)
				rpadr[poff+j] = p[j]*m;

			for (j=0; j<4; j++)
			{
				rsadr[soff+j].closed = FALSE;
				rsadr[soff+j].cnt    = 2;
			}

			poff+=8;
			soff+=4;
		}
	}

	// update object as point geometry has changed
	pp->Message(MSG_UPDATE);

	return pp;
}
 */

/*
// go through every (child) object
static Bool Recurse(HierarchyHelp *hh, BaseThread *bt, BaseObject *main, BaseObject *op, const Matrix &ml, Real srad, Real crad, LONG sub, Bool single)
{
	// test if input object if polygonal
	if (op->GetType()==Opolygon)
	{
		BaseObject *tp   = NULL;
		PolyInfo	 *pli  = NULL;
		Vector     *padr = ToPoly(op)->GetPoint(),pa,pb;
		LONG       pcnt  = ToPoly(op)->GetPointCount(),i,side,a,b;
		Polygon		 *vadr = ToPoly(op)->GetPolygon();
		LONG       vcnt  = ToPoly(op)->GetPolygonCount();
		Matrix     m;
		Neighbor	 n;
		
		// load names from resource
		String		 pstr = GeLoadString(IDS_ATOM_POINT);
		String		 estr = GeLoadString(IDS_ATOM_EDGE);

		// initialize neighbor class
		if (!n.Init(pcnt,vadr,vcnt,NULL)) return FALSE;

		// create separate objects 
		// if this option is enabled no polygonal geometry is build - more parametric objects
		// are returned instead
		if (single)
		{
			for (i=0; i<pcnt; i++)
			{
				// alloc sphere primitive
				tp=AllocPrimitiveObject(PRIMITIVE_SPHERE);
				if (!tp) return FALSE;
				
				// add phong tag
				if (!tp->MakeTag(Tphong)) return FALSE;
				tp->SetName(pstr+" "+LongToString(i));

				// set object parameters
				BaseContainer bc = tp->GetDataInstance();
				bc.SetReal(PRIM_SPHERE_RAD,srad);
				bc.SetReal(PRIM_SPHERE_SUB,sub);

				// insert as last object under main
				tp->InsertUnderLast(main);

				// set position in local coordinates
				tp->SetPos(padr[i]*ml);
			}

			for (i=0; i<vcnt; i++)
			{
				// get polygon info for i-th polygon 
				pli = n.GetPolyInfo(i);

				for (side=0; side<4; side++)
				{
					// only proceed if edge has not already been processed
					// and edge really exists (for triangles side 2 from c..d does not exist as c==d)
					if (pli->mark[side] || side==2 && vadr[i].c==vadr[i].d) continue;

					// alloc cylinder primitive
					tp=AllocPrimitiveObject(PRIMITIVE_CYLINDER);
					if (!tp) return FALSE;

					// add phong tag
					if (!tp->MakeTag(Tphong)) return FALSE;

					switch (side)
					{
						case 0: a=vadr[i].a; b=vadr[i].b; break;
						case 1: a=vadr[i].b; b=vadr[i].c; break;
						case 2: a=vadr[i].c; b=vadr[i].d; break;
						case 3: a=vadr[i].d; b=vadr[i].a; break;
					}

					tp->SetName(estr+" "+LongToString(pli->edge[side]));

					pa = padr[a]*ml;
					pb = padr[b]*ml;

					// set object parameters
					BaseContainer bc = tp->GetDataInstance();
					bc.SetReal(PRIM_CONE_TRAD,crad);
					bc.SetReal(PRIM_CONE_BRAD,crad);
					bc.SetReal(PRIM_CONE_HEIGHT,Len(pb-pa));
					bc.SetReal(PRIM_AXIS,4);
					bc.SetLong(PRIM_CONE_TYPE,0);
					bc.SetLong(PRIM_CONE_HSUB,1);
					bc.SetLong(PRIM_CONE_SEG,sub);

					// place cylinder at edge center
					tp->SetPos((pa+pb)*0.5);

					// build edge matrix
					m.v3=!(pb-pa);
					RectangularSystem(m.v3,&m.v1,&m.v2);
					tp->SetRot(MatrixToHPB(m));

					// insert as last object under main
					tp->InsertUnderLast(main);
				}
			}
		}
		else
		{
			// check if polygonal geometry has to be built
			if (hh->GetVFlags()&VFLAG_POLYGONAL)
				tp = BuildPolyHull(ToPoly(op),ml,srad,crad,sub,hh->GetLOD(),&n,bt);
			else
				tp = AllocPolygonObject(0,0);

			if (tp)
			{
				tp->SetName(op->GetName());
				tp->InsertUnderLast(main);
				
				// check if isoparm geometry has to be built
				if (hh->GetVFlags()&VFLAG_ISOPARM)
				{
					LineObject *ip = BuildIsoHull(ToPoly(op),ml,srad,crad,sub,hh->GetLOD(),&n,bt);
					
					// isoparm always needs to be set into a polygon object
					if (ip) tp->SetIsoparm(ip);
				}
			}
		}
	}

	for (op=op->down(); op; op=op->next())
		if (!Recurse(hh,bt,main,op,ml*op->GetMl(),srad,crad,sub,single)) return FALSE;

	// check for user break
	return !bt || !bt->TestBreak();
}
*/


/*
// main routine: build virtual atom objects
static BaseObject *GetVirtualObjects(PluginObject *op, HierarchyHelp *hh)
{

	print("get virtual");

	VariableTag *tg = (VariableTag*)op->GetTag(Tpoint);if (!tg) {op->MakeVariableTag(Tpoint,1000);return NULL;}

	print(tg->GetDataCount());

	return NULL; 
//-----------------------------------------



	BaseObject *orig = op->down(); 
	
	// return if no input object is available
	if (!orig) return NULL; 

	// request polygonized input
	hh->AddVFlags(VFLAG_POLYGONAL); 
	
	Bool dirty = FALSE;

	// generate polygonalized clone of input object
	BaseObject *main=NULL,*res=op->GetAndCheckHierarchyClone(hh,orig,HCLONE_ASPOLY,&dirty,FALSE); 

	// if !dirty object is already cached and doesn't need to be rebuilt
	if (!dirty) return res; 
	if (!res)   return NULL; 

	//LONG sub;
	//Bool single;
	//Real srad,crad;
	
	// get object container
	BaseContainer bc=op->GetDataInstance(); 
	BaseThread    *bt=hh->GetThread();

	// group all further objects with this null object
	main = AllocObject(Onull); 
	if (!main) goto Error;

	// get object settings
	//srad   = bc.GetReal(ATOMOBJECT_SRAD);
	//crad   = bc.GetReal(ATOMOBJECT_CRAD);
	//sub    = bc.GetLong(ATOMOBJECT_SUB);
	//single = bc.GetBool(ATOMOBJECT_SINGLE);

	// go through all child hierarchies
	//if (!Recurse(hh,bt,main,res,orig->GetMl(),srad,crad,sub,single)) goto Error;
	//blDelete(res);


	return main;

Error:
	blDelete(res);
	blDelete(main);
	return NULL;
}
*/



//#####################################################################################
static Bool ModifyObject(PluginObject *mod, BaseDocument *doc, BaseObject *op, const Matrix &op_mg, const Matrix &mod_mg, Real lod, LONG flags, BaseThread *thread)
{
	BaseContainer data = mod->GetDataInstance(); 

	Vector		p;
	Matrix		m,im;
	LONG		c,pcnt,o,i,la;
	//Real		rad = data.GetReal(DFO_HEIGHT);
	Real		strength = 1.0;
	LONG		TexturNr = 0.0;
	LONG		ChannelNr = 0.0;
	Real		s;
	LONG		ColorRes = 1;//GET VERTEX COLOR RESULT
	LONG		NormalRes = 1;//
	Real		*weight=NULL;
	Vector		*padr=NULL;
	Vector		*mpadr = NULL;
	//Vector	*mem=NULL;
	StickTextureTag *pt = NULL;

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
	Segment			*Seg = NULL;

	BaseObject		*ColOb = NULL;//TEST 1
	BaseObject		*SpherOb = NULL;
	Real			coldist = 0;//TEST 1
	Vector			ColPos;

	Matrix			MG;
	//LONG			ParticlCnt;
	//Particle		*Particles = NULL;
	//.................

	//Vector	*VertexDisplaceArray=NULL;//VERTEX NORMAL Array

	PointObject *uop = (PointObject*)mod->up(); if (!uop) return TRUE;
	if (!(uop->GetLayerType()&L_pointobject)) return TRUE;

	LONG		Syst = data.GetLong(OSP_SYST1);
	//Real		harg = data.GetReal(OSP_MASSE);
	Real		masse = data.GetReal(OSP_MASSE);
	Real		elasticite = data.GetReal(OSP_ELAST)*masse;
	Real		resistance = data.GetReal(OSP_RESIS);
	Real		maXSegLen = data.GetReal(OSP_AXSEGLEN);
	Real		mInSegLen = data.GetReal(OSP_INSEGLEN);
	Vector		graviti = data.GetVector(OSP_GRAVITI);
	//Collision
	String		CollName = data.GetString(OSP_COLL);
	LONG		overSam = data.GetLong(OSP_OVER);
	Real		pointRadius = data.GetReal(OSP_PRAD2);
	Real		ColRadius = data.GetReal(OSP_RAD1)+pointRadius;//???? +1
	Real		ColRadius2	= ColRadius * ColRadius;//??
	Vector		PosDif;


	padr = uop->GetPoint();
	pcnt = uop->GetPointCount(); if (!pcnt) return TRUE;
	weight = uop->CalcVertexMap(mod);

	
	/*
	for (c=0; c<pcnt; c++ ){
		print(padr[c]);
	}
	*/


	/*
	PluginTag *pt = GetPlugTag(op,1008036);if (!pt) {AllocTag(Tplugin,1008036,pcnt); return TRUE;}
	pt->GetData();

	if (data.GetLong(OB_MEMSIZE)==pcnt){
		mem = (Vector*)pt->GetPluginData();	if (!mem) return TRUE;

		//for (c=0; c<pcnt; c++) print(mem[c]);


		CopyMem(padr,mem,sizeof(Vector)*pcnt);
		
		for (c=0; c<pcnt; c++){
			print(mem[c]);
			mem[c] = padr[c];
			print(mem[c]);
		}
	}else{
		data.SetLong(OB_MEMSIZE,pcnt);
		pt->SetData(data);
	}
	*/
	/*
	print("Stiki");

	//TEST ??
	pt = (StickTextureTag*)uop->GetTag(Tsticktexture);if (!pt) {
		pt = (StickTextureTag*)uop->MakeVariableTag(Tsticktexture,pcnt);
		if (pt) pt->Record(uop,TRUE);
		 goto Error; }
	//LONG		mpcnt = pt->GetDataCount();if (mpcnt != pcnt) {op->MakeVariableTag(Tsticktexture,pcnt);return TRUE;}
	mpadr = (Vector*)pt->GetDataAddress();if (!mpadr) goto Error; 
	//TEST ???
	


	//if (GetVertexDisplace(&VertexDisplaceArray,doc,ToPoly(op),NULL,TexturNr,ChannelNr)!=0) goto Error;

	m  = (!mod_mg) * op_mg; // op  ->  world  ->  modifier
	im = !m;

	for (c=0; c<pcnt; c++){
		if (thread && !(c&63) && thread->TestBreak()) break;//???????

		p = m*padr[c];
		s = strength;
		if (weight) s*=weight[c];
		//p = s * (p + VertexDisplaceArray[c])+(1.0-s)*p;
		p = mpadr[c];
		//p = s*(!p*rad)+(1.0-s)*p;//<<<<<<<<<<
		padr[c] = p*im;
	}
	*/

	//print("TEST");
	//---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P
	if (uop->GetType() == Opolygon){ //POLYGON
		PolygonObject	*FormOb = NULL;
		LONG			FormPolyCnt = 0;
		Polygon			*FormPolys = NULL;
		Neighbor		NG;
		LONG			*dadr;//Array
		LONG			dcnt;
		LONG			polyn;
		LONG			pn;

		FormOb = (PolygonObject*)uop;

		FormPointCnt = FormOb->GetPointCount();//<<<<<<
		FormPoints =  FormOb->GetPoint(); if (!FormPoints) return TRUE;

		FormPolyCnt = FormOb->GetPolygonCount();//<<<<<<
		FormPolys =  FormOb->GetPolygon(); if (!FormPoints) return TRUE;
		
		if (!NG.Init(FormPointCnt,FormPolys,FormPolyCnt,NULL)) return TRUE;

		for (c=0;c<FormPointCnt;c++){

			//TEST SPRINGSTEST SPRINGSTEST SPRINGS SPRINGSTEST SPRINGS??????
			NG.GetPointPolys(c,&dadr,&dcnt);

			elast=Vector(0.0,0.0,0.0);//???
			current = FormPoints[c];

			for(o=0;o<dcnt;o++){
				polyn = dadr[o];//Polygon number, (number-p of all polys o point) on point number-i

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
					if (leng > maXSegLen){
						force = elasticite * (leng - maXSegLen);
						elast = elast + (dist / leng) * force;
					}//if (leng > longseg){

			}

			accel = elast / masse - graviti;//ORIGENAL OK
			FormPoints[c] = FormPoints[c] + accel;

			//TEST SPRINGS TEST SPRINGSTEST SPRINGSTEST SPRINGS ??????

		}//for (c=0;c<FormPointCnt;c++){
	}//if (op->GetLayerType()&L_pointobject)
	//---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P---P

//-------------------------------------------------------
Error:
	//GeFree(VertexDisplaceArray);//VERTEX NORMAL Array
	GeFree(weight);

	op->Message(MSG_UPDATE);

	return TRUE;
}



void CheckDirty(PluginObject *op, BaseDocument *doc)
{
	//TESTEN OB NEU MATERIAL ???

	op->SetDirty(DIRTY_DATA);

/*
BaseContainer data = op->GetData();
BaseTime myobject_lasttime = data.GetTime(OB_TIME);
Bool	anim = data.GetBool(DFO_ANIM,FALSE);

if ((doc->GetTime()!=myobject_lasttime) && (anim)){
	op->SetDirty(DIRTY_DATA);
	myobject_lasttime = doc->GetTime();
	data.SetTime(OB_TIME,myobject_lasttime);
	op->SetData(data);
}
*/


}


// plugin icons
static BaseBitmap *icon=NULL;
static BaseBitmap *icon_small=NULL;

// routine to initialize plugin
Bool RegisterOESP(void)
{
	// decide by name if the plugin shall be registered - just for user convenience
	String name=GeLoadString(IDS_OESP); if (!name.Content()) return TRUE;

	icon = AllocBaseBitmap();// allocate and read two icons from plugin resource folder
	if (!icon || icon->Init(GeGetPluginPath()+String("res")+String("oesp1.tif"))!=IMAGE_OK) return FALSE;
	icon_small = AllocBaseBitmap();
	if (!icon_small || icon_small->Init(GeGetPluginPath()+String("res")+String("oesp1smal.tif"))!=IMAGE_OK) return FALSE;

	OBJECTPLUGIN cs;
	// clear all structure elements
	ClearMem(&cs,sizeof(cs));

	cs.add_menu					= TRUE;// object generation command will automatically be created
	cs.icon						= icon;// assign two icons (32x32 for command, 24x24 for object manager)
	cs.icon_small				= icon_small;
	//cs.info						= OBJECT_GENERATOR|OBJECT_INPUT;// set flags for generator objects that use other objects as input 
	//cs.GetVirtualObjects		= GetVirtualObjects;// main routine to generate virtual objects
	cs.info						= OBJECT_MODIFIER;
	cs.ModifyObject				= ModifyObject;
	cs.EditSettings				= EditSettings;// edit routine
	cs.AllocSettings			= AllocSettings;// initialize settings
	cs.FreeSettings				= FreeSettings; // read, write, free and clone normally are unnecessary - see above
	cs.CloneSettings			= CloneSettings;
	cs.ReadSettings				= ReadSettings;
	cs.WriteSettings			= WriteSettings;
	cs.GetDescription			= GetDescription;// build description for parameter animation

	cs.CheckDirty				= CheckDirty;

	// be sure to use a unique ID obtained from www.plugincafe.com
	return GeRegisterPlugin(C4DPL_OBJECT,PLUGINS_ID,name,&cs,sizeof(cs));
}

// routine to free remaining icon data
void FreeOESP(void)
{
	FreeBaseBitmap(icon);
	FreeBaseBitmap(icon_small);
}
