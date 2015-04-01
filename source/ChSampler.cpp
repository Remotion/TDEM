#include "c4d_general.h"
#include "c4d_memory.h"
#include "c4d_tools.h"
#include "c4d_basedocument.h"
#include "c4d_basecontainer.h"
#include "c4d_baseobject.h"
#include "c4d_basematerial.h"
#include "c4d_basetag.h"
#include "c4d_baseselect.h"
#include "c4d_basechannel.h"
#include "c4d_shaderplugin.h"

#include "ChSampler.h"


//CONSTRUCTOR
ChSampler::ChSampler(BaseDocument *doc, TextureTag *textag,LONG chacol)
{
	if (!doc || !textag) return;

	BaseMaterial *mat = textag->GetMaterial(); if (!mat) return;//No Material
	chan1 = mat->GetChannel(chacol); if (!chan1) return;//NO Channel
	
	//--------------------------------------------
	BaseContainer texdata = textag->GetData();
	Bool TTile = texdata.GetBool(TEXTUREKEY_TILE);//Kacheln
	Bool TSaml = texdata.GetBool(TEXTUREKEY_SEAMLESS);//Natlös
	//String TSel = texdata.GetString(TEXTUREKEY_RESTRICTION);//Selection
	offsetX = texdata.GetReal(TEXTUREKEY_OFFSETX)+0.0001;//TEST 0.0001 ???
	offsetY = texdata.GetReal(TEXTUREKEY_OFFSETY)+0.0001;//TEST 0.0001 ???
	lenX = texdata.GetReal(TEXTUREKEY_LENGTHX);
	lenY = texdata.GetReal(TEXTUREKEY_LENGTHY);
	//--------------------------------------------

	//----InitRenderStruct------
	InitRenderStruct irs;
	LONG fps = doc->GetFps();
	BaseTime BTtime = doc->GetTime();
	irs.time = BTtime;
	irs.fps = fps;
	Filename dpath = doc->GetPath();
	irs.docpath  = dpath.Get();
	irs.doc = doc;

	chan1->InitTexture(&irs);
	//----------------------------

	t=BTtime.Get();// current time in seconds
	texflag = (TTile ? TEX_TILE : 0) + (TSaml ? TEX_MIRROR : 0); 

	//p=Vector(0.0,0.0,0.0);	// texture position in UVW coordinates (x(1 or 0) und y(1 or 0))
	d=Vector(0.0,0.0,0.0);		// MIP sample radius in UVW coordinates
	n=Vector(0.0,0.0,0.0);	// surface normal
}

//DESTRUCTOR
ChSampler::~ChSampler(void)
{
	if (chan1) chan1->FreeTexture();
}


Vector ChSampler::Sample(const Vector &p)
{
	//LANGSAM?? 
	Vector pp = p;
	pp.x = (p.x-offsetX) / lenX; pp.y = (p.y-offsetY) / lenY;//Kaheln Problem ????
	if (chan1) return chan1->Sample(NULL,&pp,&d,&n,t,texflag,0.0,1.0);
	else return Vector(0.0);
}
