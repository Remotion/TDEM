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

#define PLUGINS_ID		1008036 //MEM_TAG_ID

#define OB_MEMSIZE		5001 //????

//######### MEMORY STRUCTUR ##########
struct MEMdata
{
	MEMdata(LONG t_cnt);
	MEMdata(LONG t_cnt, void *data);
	MEMdata(MEMdata *data);
	~MEMdata(void);

	LONG	cnt;
	Vector *mem;

};

//Constructor cnt
MEMdata::MEMdata(LONG t_cnt)
{
	cnt = t_cnt;
	mem = bNew Vector [cnt];
}

//copy Constructor 1 ??
MEMdata::MEMdata(MEMdata *data)
{
	cnt = data->cnt;
	mem = bNew Vector [cnt];
	CopyMem(data->mem,mem,sizeof(Vector)*cnt);//???? WORK????
}

//copy Constructor 2 ??
MEMdata::MEMdata(LONG t_cnt, void *data)
{
	cnt = t_cnt;
	mem = bNew Vector [cnt];
	CopyMem(data,mem,sizeof(Vector)*cnt);//???? WORK????
}

//Destructor
MEMdata::~MEMdata(void)
{
	cnt = 0;
	bDelete(mem);
}
//######### MEMORY STRUCTUR ##########

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
static Bool AllocSettings(PluginTag *tag)
{
	print("MEM AllocSettings");


	//BaseContainer bc;
	//LONG PointCnt = 0;
	//PointObject *SourceOb = (PointObject*)tag->GetObject();
	//if (SourceOb && SourceOb->GetLayerType()&L_pointobject) PointCnt = SourceOb->GetPointCount();
	//bc.GetLong(OB_MEMSIZE,cnt);//????????????????	
	
	LONG cnt = 10;
	MEMdata *data = gNew MEMdata(cnt);
	//TEST
	
	for (LONG c=0; c<cnt; c++){
		data->mem[c] = Vector(1.0,Real(c),8.0);
	}
	
	//TEST
	tag->SetPluginData(data);
	return data!=NULL;
}

static void FreeSettings(PluginTag *tag)
{	
	print("MEM FreeSettings");

	MEMdata *data = (MEMdata*)tag->GetPluginData();
	//TEST
	/*
	for (LONG c=0; c<data->cnt; c++){
		print(data->mem[c]);
	}
	*/
	//TEST
	gDelete(data);
}

static Bool CloneSettings(PluginTag *src, PluginTag *dst)
{	
	print("MEM CloneSettings");
	
	//BaseContainer bc;
	//bc = src->GetData();
	//LONG memsize = bc.GetLong(OB_MEMSIZE);//?????
	//print(memsize);

	//Vector *memsrc = (Vector*)src->GetPluginData();	if (!memsrc) return FALSE;
	//Vector *memdst = bNew Vector [memsize];	if (!memdst) return FALSE;
	//CopyMem(memsrc,memdst,sizeof(Vector)*memsize); if (!memdst) return FALSE;
	//dst->SetPluginData(memdst);

	MEMdata *srcdata = (MEMdata*)src->GetPluginData();
	MEMdata *dstdata = gNew MEMdata(srcdata);if (!dstdata) return FALSE;
	//TEST
	/*
	for (LONG c=0; c<dstdata->cnt; c++){
		print(dstdata->mem[c]);
	}
	*/
	//TEST
	dst->SetPluginData(dstdata);
	return TRUE;
}

static Bool ReadSettings(PluginTag *tag, HyperFile *hf, LONG level)
{	
	print("MEM ReadSettings");
	
	
	//if (level>=0)
	//{
	//	String *mem = gNew String;
	//	if (!mem) return FALSE;
	//	hf->ReadString(mem);
	//	op->SetPluginData(mem);
	//}

	if (level>=0)
	{
		//BaseContainer bc;
		//bc = tag->GetData();
		//LONG memsize = bc.GetLong(OB_MEMSIZE);//?????
		//print(memsize);

		//TEST
		LONG cnt = 0;
		LONG size = 0;
		void *rmem=NULL;


		hf->ReadLong(&cnt);//<< READ LONG
		size = sizeof(Vector)*cnt;
		hf->ReadMemory(&rmem,&size);//<< READ MEM
		MEMdata *data = gNew MEMdata(cnt,rmem);if (!data) return FALSE;
		GeFree(rmem);

		//TEST
		/*
		for (LONG c=0; c<data->cnt; c++){
			print(data->mem[c]);
		}
		*/
		//TEST

		tag->SetPluginData(data);//MEMORY
		//TEST
	}

	return TRUE;
}


static Bool WriteSettings(PluginTag *tag, HyperFile *hf)
{	
	print("MEM WriteSettings");

	//BaseContainer bc;
	//bc = tag->GetData();
	//LONG memsize = bc.GetLong(OB_MEMSIZE);//?????

	/*
	Vector *memsrc = (Vector*)src->GetPluginData();	if (!memsrc) return FALSE;
	for(LONG c=0;c<memsize;){
	
	  */
	//String *dat=(String*)op->GetPluginData();
	//hf->WriteString(*dat);

	MEMdata *data = (MEMdata*)tag->GetPluginData();
	hf->WriteLong(data->cnt);//<< WRITE LONG
	hf->WriteMemory(data->mem,sizeof(Vector)*data->cnt);//<< WRITE MEM
	
	return TRUE;
}

//############## EDIT SETING #############
/*
static Bool EditSettings(PluginTag *tag)
{
	BaseContainer data = tag->GetData();
	ESPDialog dlg(&data);

	if (!dlg.Open()) return FALSE;
	tag->SetData(data);
	return TRUE;
}
*/

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
Bool RegisterMEMTag(void)
{
	// decide by name if the plugin shall be registered - just for user convenience
	String name=GeLoadString(IDS_MEM); if (!name.Content()) return TRUE;

	help = gNew String();
	icon = AllocBaseBitmap();
	if (!icon || icon->Init(GeGetPluginPath()+String("res")+String("esp1.tif"))!=IMAGE_OK) {GePrint("ESP: esp1.tif is not loaded!");return FALSE;}
	//*help = GeLoadString(IDS_LOOKATCAMERAHELP);

	TAGPLUGIN cs;
	ClearMem(&cs,sizeof(cs));

	cs.add_menu				= TRUE;
	//cs.help				= help->Get();
	cs.icon					= icon;
	cs.info					= TAG_VISIBLE|TAG_MULTIPLE;
	//cs.disklevel			= TAGLAYER_VARIABLETAG;

	cs.AllocSettings		= AllocSettings;
	cs.FreeSettings			= FreeSettings;
	cs.CloneSettings		= CloneSettings;
	cs.ReadSettings			= ReadSettings;
	cs.WriteSettings		= WriteSettings;
	//-----
	//cs.EditSettings			= EditSettings;
	//cs.Execute				= Execute;
	//cs.Draw					= Draw; // TEST
	cs.Speedup				= Speedup;
	// be sure to use a unique ID obtained from www.plugincafe.com
	return GeRegisterPlugin(C4DPL_TAG,PLUGINS_ID,name,&cs,sizeof(cs));
}


//###############  FREE ESP TAG ####################
void FreeMEMTag(void)
{
	FreeBaseBitmap(icon);
	gDelete(help);
}
