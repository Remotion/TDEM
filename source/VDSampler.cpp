#include "VdSampler.h"

/*
//####################### SampleUV #######################
Vector VdSampler::SampleUV(const Vector &uv)
{
	pp			= uv;
	pp.x		= (uv.x-offsetX) / lenX; pp.y = (uv.y-offsetY) / lenY;//Kaheln Problem ????
	return TexChan1->Sample(NULL,&pp,&d,&n,t,texflag,bluoff,bluscal);
}
//####################### Sample3D #######################
Real VdSampler::SampleFilterUV(const Vector &uv,const LONG fil)
{
	pp			= uv;
	pp.x		= (uv.x-offsetX) / lenX; pp.y = (uv.y-offsetY) / lenY;//Kaheln Problem ????
	return ColorFilter(TexChan1->Sample(NULL,&pp,&d,&n,t,texflag,bluoff,bluscal),fil);
}

//####################### Sample3D #######################
Vector VdSampler::Sample3D(const Vector &p, const Vector &uv)
{
	vd->p		= p*omg;
	vd->back_p	= vd->p;
	pp			= uv;
	return TexChan1->Sample(vd,&pp,&d,&n,t,texflag,bluoff,bluscal);
}
//####################### Sample3D #######################
Real VdSampler::SampleFilter3D(const Vector &p, const Vector &uv,const LONG fil)
{
	vd->p		= p*omg;
	vd->back_p	= vd->p;
	pp			= uv;
	return ColorFilter(TexChan1->Sample(vd,&pp,&d,&n,t,texflag,bluoff,bluscal),fil);
}
*/

//############################ Init ###############################################
LONG VdSampler::Init(BaseDocument *doc, BaseObject *ob, const Matrix op_mg, TextureTag *texturetag,const LONG chnr)
{
	if (!doc || !ob || !texturetag)							return 1;//NO doc, ob, texturetag
	LONG			err = 0;
	Real			mipr;
	BaseContainer	chandata;
	omg = op_mg;

	BaseMaterial *mat = texturetag->GetMaterial();if(!mat)  return 2;//NO Material
	TexChan1    = mat->GetChannel(chnr); if (!TexChan1)	return 3;//NO Channel
	TexChan1    = TexChan1->GetClone(); if (!TexChan1)	return 3;//NO Channel
	
	//---------- PLUGINS ID SAFE ----------
	LONG	pid = TexChan1->GetPluginID();
	if (   (pid==1005008) //dirtyNUTs	
		|| (pid==1007539) //ChanLum	
		|| (pid==1009789) //Various Demo
		|| (pid==1000007) //Simbiont
		)return 4;
	//--------------------------------------

	//-------------- Texture Data --------------------------
	BaseContainer	texdata = texturetag->GetData();
	Bool			TTile = texdata.GetBool(TEXTUREKEY_TILE);//Kacheln
	Bool			TSaml = texdata.GetBool(TEXTUREKEY_SEAMLESS);//Natlös
					offsetX = texdata.GetReal(TEXTUREKEY_OFFSETX)+0.0001;//TEST 0.0001 ???
					offsetY = texdata.GetReal(TEXTUREKEY_OFFSETY)+0.0001;//TEST 0.0001 ???
					lenX = texdata.GetReal(TEXTUREKEY_LENGTHX);
					lenY = texdata.GetReal(TEXTUREKEY_LENGTHY);
	//--------------------------------------------------------

	//----------------- Doc Data -----------------
	LONG			fps = doc->GetFps();
	BaseTime		BTtime = doc->GetTime();
	Filename		dpath = doc->GetPath();

	//--------------- VolumeData -----------------
	vd = AllocVolumeData(); if (!vd) {FreeVolumeData(vd);  return 5;}
	vd->version		= 0; //LONG (0 or 1)
	vd->fps			= fps; //LONG
	vd->ambient		= 0.5;//0.0; //Vector
	vd->time		= BTtime.Get(); //Real
	vd->bumpn		= Vector(0.0,1.0,0.0);//Vector !!!!!!!!
	vd->back_orign	= Vector(0.0,1.0,0.0);//Vector
	vd->back_n		= Vector(0.0,1.0,0.0);//Vector
	vd->back_delta	= 0.0;//Vector
	vd->global_mip	= 1.0;//Vector
	vd->orign		= Vector(0.0,1.0,0.0);;//Vector !!!!!!!!
	vd->n			= Vector(0.0,1.0,0.0);;//Vector !!!!!!!!
	vd->dispn		= Vector(0.0,1.0,0.0);;//Vector !!!!!!!!
	vd->delta		= 0.0;//Vector
	vd->dist		= 0.0;//Real
	vd->lhit		= 0;//LONG ?????????????
	vd->cosc		= 1.0;//Real Angle
	vd->ddu			= Vector(1.0,0.0,0.0);//Vector
	vd->ddv			= Vector(0.0,1.0,0.0);//Vector
	vd->tray		= NULL;//TRay
	vd->rray		= NULL;//RRay
	vd->ray			= NULL;//Ray
	vd->xlight		= NULL;//RayLight


	//---------------- Tex Data ---------------
	tex	= AllocTexData(); if (!tex)	{FreeVolumeData(vd); FreeTexData(tex); return 6;}
	InitTexData(tex);
	tex->mp			= mat;//<<POINTER>> ???
	tex->m			= texturetag->GetMl();
	tex->im			= !tex->m;//TEX
	tex->proj		= texdata.GetLong(TEXTUREKEY_PROJECTION);
	tex->texflag	= (TTile)?TEX_TILE:0;tex->texflag += (TSaml)?TEX_MIRROR:0;
	tex->side		= texdata.GetLong(TEXTUREKEY_SIDE);
	tex->lenx		= lenX;
	tex->leny		= lenY;
	tex->ox			= offsetX;
	tex->oy			= offsetY;
	vd->tex			= tex; //Projector Ned It
	//---------------------------------------------
	
	//---------------- RAY OBJECT -----------------
	rop = gNew(RayObject);if (!rop) goto Error;
	rop->link		= ob; //PROB
	rop->texture_link = ob;
	rop->mg			= op_mg;
	rop->visibility	= 1.0;
	rop->mp			= ob->GetMp()*op_mg;
	rop->rad			= ob->GetRad();
	rop->type		= O_POLYGON;
	if (ob->GetLayerType()&L_pointobject){
		rop->pcnt	= ToPoly(ob)->GetPointCount();
		rop->padr	= ToPoly(ob)->GetPoint();
		rop->vcnt	= ToPoly(ob)->GetPolygonCount();
		rop->vadr	= (RayPolygon*)ToPoly(ob)->GetPolygon();
	}else{
		rop->pcnt	= 0;
		rop->padr	= NULL;
		rop->vcnt	= 0;
		rop->vadr	= NULL;
	}
	rop->texcnt		= 0;
	rop->texadr		= NULL;
	rop->uvwadr		= NULL;
	rop->rsadr		= NULL;
	vd->op			= rop;
	//--------------------------------------------
	
	//------------ InitRenderStruct --------------
	irs = gNew(InitRenderStruct);if (!irs) goto Error;
	irs->fps		= fps;
	irs->time		= BTtime;
	irs->doc		= doc;
	irs->docpath	= dpath.Get();
	irs->vd			= vd;
	//--------------------------------------------

	//---------------- Samplin -------------------
	chandata = TexChan1->GetData();
	bluoff = chandata.GetReal(BASECHANNEL_BLUR_OFFSET,0.0);//BLUR OFFSET
	bluscal = 0.0;
	mipr = (bluoff > 0.0)?0.001:0.0; //??
	d=Vector(mipr,mipr,mipr);
	n=Vector(0.0,1.0,0.0);
	t=BTtime.Get();	
	texflag = (TTile)?TEX_TILE:0;texflag += (TSaml)?TEX_MIRROR:0;
	
	if (TexChan1->InitTexture(irs)==LOAD_OK) return 0; //CRASH ???
	else err = 7;

	//---------------------------------------------------
Error:
	TexChan1->FreeTexture();//FREE
	FreeTexData(tex);//FREE
	FreeVolumeData(vd);//FREE
	gDelete(rop);
	gDelete(irs);
	//if (err > 0) print("Get3DVertexRGB Error: "+LongToString(err));

	return err;
}
//################### VdSampler ###########################
VdSampler::VdSampler()
{
	TexChan1	= NULL;
	vd			= NULL;
	tex			= NULL;
	rop			= NULL;
	irs			= NULL;
	t			= 0.0;
	d			= 0.0;
	n			= Vector(0.0,1.0,0.0);
	texflag		= 0;
	offsetX		= 1.0;
	offsetY		= 1.0;
	lenX		= 1.0;
	lenY		= 1.0;
	bluoff		= 1.0;
	bluscal		= 0.0;
	pp			= 0.0;
}
//#################### ~VdSampler ##########################
VdSampler::~VdSampler(void)//DESTRUCTOR
{
	if (TexChan1) TexChan1->FreeTexture();
	if (tex) FreeTexData(tex);//FREE
	if (vd) FreeVolumeData(vd);//FREE
	FreeBaseChannel(TexChan1);//!!!!!!
	gDelete(rop);
	gDelete(irs);
}
