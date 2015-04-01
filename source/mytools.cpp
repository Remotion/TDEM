//#include "c4d_plugin.h"
//#include "c4d_menuplugin.h"
#include "c4d_general.h"
#include "c4d_memory.h"
//#include "c4d_basebitmap.h"
//#include "c4d_file.h"
#include "c4d_tools.h"
//#include "c4d_gui.h"
#include "c4d_basedocument.h"
#include "c4d_basecontainer.h"
#include "c4d_baseobject.h"
#include "c4d_basematerial.h"
#include "c4d_basetag.h"
#include "c4d_baseselect.h"
#include "c4d_basechannel.h"
#include "c4d_shaderplugin.h"


Bool ComputeBarycenter (const Vector &rkV0, const Vector &rkV1,
    const Vector &rkV2, const Vector &rkV3, const Vector &rkP,
    Real afNumer[4], Real &rfDenom)
{
	/*
    Vector kV03 = rkV0 - rkV3;
    Vector kV13 = rkV1 - rkV3;
    Vector kV23 = rkV2 - rkV3;
    Vector kPV3 = rkP - rkV3;

    Real fM00 = kV03*kV03;
    Real fM01 = kV03*kV13;
    Real fM02 = kV03*kV23;
    Real fM11 = kV13*kV13;
    Real fM12 = kV13*kV23;
    Real fM22 = kV23*kV23;
    Real fR0  = kV03*kPV3;
    Real fR1  = kV13*kPV3;
    Real fR2  = kV23*kPV3;

    Real fC00 = fM11*fM22-fM12*fM12;
    Real fC01 = fM02*fM12-fM01*fM22;
    Real fC02 = fM01*fM12-fM02*fM11;
    Real fC11 = fM00*fM22-fM02*fM02;
    Real fC12 = fM02*fM01-fM00*fM12;
    Real fC22 = fM00*fM11-fM01*fM01;
    rfDenom = fM00*fC00 + fM01*fC01 + fM02*fC02;
    //assert( rfDenom != 0.0f );

    afNumer[0] = fC00*fR0+fC01*fR1+fC02*fR2;
    afNumer[1] = fC01*fR0+fC11*fR1+fC12*fR2;
    afNumer[2] = fC02*fR0+fC12*fR1+fC22*fR2;
    afNumer[3] = rfDenom-afNumer[0]-afNumer[1]-afNumer[2];

	return rfDenom != 0.0f;
	*/

	return TRUE;
}


//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
const String GetNumName(const BaseContainer data,const LONG name_id,const LONG num_id)
{
	String  Name = data.GetString(name_id);
	LONG	Num = data.GetLong(num_id);
	if ((Num > 0) & (Name != "")) {Name = Name + LongToString(Num);}
	return Name;
}
/*
//""""""""""""""""""""""""""""""""""""""
SelectionTag *GetYPTag(BaseObject *ob,const String name)// PolygonSel
{
	SelectionTag *PStag;
	BaseContainer PSdat;
	LONG tn = 0;
	do{
		PStag = (SelectionTag*)ob->GetTag(Tpolygonselection,tn);
		if (!PStag) break;
		PSdat = PStag->GetData();
		tn++;
	}while(PSdat.GetString(POLYSELECTIONTAG_NAME)!=name);
return PStag;
}


//""""""""""""""""""""""""""""""""""""""
SelectionTag *GetPSTag(BaseObject *ob,const String name)// PointSel
{
	SelectionTag *PStag;
	BaseContainer PSdat;
	LONG tn = 0;
	do{
		PStag = (SelectionTag*)ob->GetTag(Tpointselection,tn);
		if (!PStag) break;
		PSdat = PStag->GetData();
		tn++;
	}while(PSdat.GetString(POINTSELECTIONTAG_NAME)!=name);
return PStag;
}

//""""""""""""""""""""""""""""""""""""""
VariableTag *GetVMTag(BaseObject *ob,const String name)//Vertex(VariableTag)
{
	VariableTag *VMtag;
	BaseContainer VMdat;
	LONG tn = 0;
	do{
		VMtag = (VariableTag*)ob->GetTag(Tvertexmap,tn);
		if (!VMtag) break;
		VMdat = VMtag->GetData();
		tn++;
	}while(VMdat.GetString(VERTEXMAPTAG_NAME)!=name);
return VMtag;
}
*/
//""""""""""""""""""""""""""""""""""""""
PluginTag *GetPlugTag(BaseObject *ob,LONG id)
{
	PluginTag *tag;
	LONG tn = 0;
	do{
		tag = (PluginTag*)ob->GetTag(Tplugin,tn);
		if (!tag) break;
		tn++;
	}while(tag->GetType() != id); //Remo: 02.04.2007
	//}while(tag->GetPluginID() != id);

return tag;
}

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//"""""""""""""""""""""""""""""""""""  GET VERTEX NORMAL """"""""""""""""""""""""""
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
LONG GetVertexNormal(Vector **noradr, PolygonObject *ob, LONG SourcePointCnt, Vector *SourcePoints)
{
	if (!(*noradr)) return 1;

	if(SourcePointCnt<=0){return 2;}

	//Get Polygon
	CPolygon	*sourcePolygon = NULL;
	LONG SourcePolygonCnt = ob->GetPolygonCount(); if(SourcePolygonCnt<=0){return 3;}
	sourcePolygon = ob->GetPolygon(); if (!sourcePolygon){return 4;}

	//Neighbor
	Neighbor NG;
	Vector VertexN; //Vertex Normal
	LONG *dadr;
	LONG dcnt;
	if (!NG.Init(SourcePointCnt,sourcePolygon,SourcePolygonCnt,NULL)) {  return 5;}

	for (LONG c=0;c<SourcePointCnt;c++){//Point Count
		NG.GetPointPolys(c,&dadr,&dcnt);
		VertexN = Vector(0.0,0.0,0.0);
		for(LONG i=0;i<dcnt;i++){
			VertexN += CalcFaceNormal(SourcePoints,sourcePolygon[dadr[i]]);
		}
		(*noradr)[c] = (VertexN / dcnt);//OK ??
	}//for (c=0;c<SourcePointCnt;c++){

	return 0;
}//GetVertexNormal



//""""""""""""""" Gibt Vertex Normale Sicher (langsammenr ???)"""""""""""""""""""""""
LONG GetVertexNormalSafe(Vector **noradr, PolygonObject *ob)
{
	LONG c;
	Neighbor NG;
	Vector VertexN; //Vertex Normal
	LONG *dadr;
	LONG dcnt;

	//Get Points
	LONG SourcePointCnt = ob->GetPointCount();
	Vector *SourcePoints = ob->GetPoint(); if (!SourcePoints) return 2; 

	//Get Polygon
	LONG		sourcePolygonCnt = ob->GetPolygonCount();
	CPolygon	*sourcePolygon = ob->GetPolygon(); if (!sourcePolygon) return 3; 

	(*noradr) = (Vector*)GeAlloc(sizeof(Vector)*SourcePointCnt+1);if (!(*noradr)) goto Error;

	//Neighbor
	if (!NG.Init(SourcePointCnt,sourcePolygon,sourcePolygonCnt,NULL)) goto Error;

	for (c=0;c<SourcePointCnt;c++){//Point Count
		NG.GetPointPolys(c,&dadr,&dcnt);
		VertexN = Vector(0.0,0.0,0.0);
		for(LONG i=0;i<dcnt;i++){
			VertexN += CalcFaceNormal(SourcePoints,sourcePolygon[dadr[i]]);
		}
		(*noradr)[c] = (VertexN / dcnt);//OK ??
	}//for (c=0;c<SourcePointCnt;c++){
	return 0;

Error:
	GeFree((*noradr));
	return 1;
}//GetVertexNormalSafe
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//"""""""""""""""""""""""""""""""""""  GET VERTEX NORMAL """"""""""""""""""""""""""
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
LONG GetVertexNormalMG(Vector **noradr, PolygonObject *ob, LONG SourcePointCnt, Vector *SourcePoints)
{
	if (!(*noradr)) return 1;

	if(SourcePointCnt<=0){return 2;}

	//<<< SOURCE  MATRIX >>>
	//Matrix  SourceML  = ob->GetMl(), ISourceML=!SourceML;
	Matrix	SourceMG  = ob->GetMg();
	//Matrix  SourceMGU = ob->GetUpMg();
	//Vector	SourcePos = ob->GetPos()*SourceMGU;//TEST

	//Get Polygon
	CPolygon	*SourcePolygon = NULL;
	LONG SourcePolygonCnt = ob->GetPolygonCount(); if(SourcePolygonCnt<=0){return 3;}
	SourcePolygon = ob->GetPolygon(); if (!SourcePolygon) {return 4;}

	Neighbor NG;
	Vector VertexN; //Vertex Normal
	LONG *dadr;
	LONG dcnt;

	if (!NG.Init(SourcePointCnt,SourcePolygon,SourcePolygonCnt,NULL)) {  return 5;}
	//VertexNormArray = (Vector*)GeAlloc(sizeof(Vector)*SourcePointCnt+1);if (!VertexNormArray) {	return 3;}
			
	for (LONG c=0;c<SourcePointCnt;c++){//Point Count
		NG.GetPointPolys(c,&dadr,&dcnt);
		VertexN = Vector(0.0,0.0,0.0);
		for(LONG i=0;i<dcnt;i++){
			VertexN += CalcFaceNormal(SourcePoints,SourcePolygon[dadr[i]]);
		}
		(*noradr)[c] = (VertexN / dcnt)*SourceMG;//OK
	}//for (c=0;c<SourcePointCnt;c++){

	return 0;
}//GetVertexNormalMG

/*
//Etwa 2++ mal Schneller je nahc Schader!!!
//???????????????????????????????????????????????????????????????????
LONG GetVertexColor(Real **coladr, PolygonObject *ob,  LONG SourcePointCnt, Vector *SourcePoints, 
					BaseDocument *doc, LONG chacol = 0,LONG tnum = 0)//Gray
{
	if (!(*coladr)) return 1;//No Array

	//Get Polygon
	LONG SourcePolygonCnt;
	CPolygon	*SourcePolygon = NULL;//Array
	SourcePolygonCnt = ob->GetPolygonCount();
	if (SourcePolygonCnt<=0) {return 2;}//No Polygon
	SourcePolygon = ob->GetPolygon();

	//Get Texture Tag
	TextureTag *texturetag = (TextureTag*)ob->GetTag(Ttexture,tnum); if (!texturetag) return 3;//No Textur
	BaseMaterial *mat = texturetag->GetMaterial(); if (!mat) return 4;//No Material

	//Get UVW TAG
	UVWTag *uvwtag =  (UVWTag*)ob->GetTag(Tuvw); if (!uvwtag) return 5;//No Uvw Tag
	LONG uvwcnt = uvwtag->GetDataCount();

	BaseChannel *TexChan1 = mat->GetChannel(chacol); if (!TexChan1) return 6;//NO Channel

	BaseContainer texdata = texturetag->GetData();
	Bool TTile = texdata.GetBool(TEXTUREKEY_TILE);//Kacheln
	Bool TSaml = texdata.GetBool(TEXTUREKEY_SEAMLESS);//Natlös
	String TSel = texdata.GetString(TEXTUREKEY_RESTRICTION);//Selection
	Real OffsetX = texdata.GetReal(TEXTUREKEY_OFFSETX)+0.0001;//TEST 0.0001 ???
	Real OffsetY = texdata.GetReal(TEXTUREKEY_OFFSETY)+0.0001;//TEST 0.0001 ???
	Real LenX = texdata.GetReal(TEXTUREKEY_LENGTHX);
	Real LenY = texdata.GetReal(TEXTUREKEY_LENGTHY);

	SelectionTag *PSelTag;
	BaseSelect *PSel;
	Bool Bsel = FALSE;
	if (TSel != ""){
		PSelTag = GetYPTag(ob,TSel);if (!PSelTag) return 7;//No Selection
		PSel = PSelTag->GetBaseSelect();//if (!PSel) return 14;
		Bsel = TRUE;
	}

	//----------------------
	//InitRenderStruct
	InitRenderStruct irs;
	LONG fps = doc->GetFps();
	BaseTime BTtime = doc->GetTime();
	irs.time = BTtime;
	irs.fps = fps;
	Filename dpath = doc->GetPath();
	irs.docpath  = dpath.Get();
	irs.doc = doc;

	TexChan1->InitTexture(&irs);

	Vector p=Vector(0.0,0.0,0.0);		// texture position in UVW coordinates (x(1 or 0) und y(1 or 0))
	Vector d=Vector(0.0,0.0,0.0);		// MIP sample radius in UVW coordinates
	Vector n=Vector(0.0,0.0,0.0);		// surface normal
	Real t=BTtime.Get();				// current time in seconds
	//LONG texflag=0;					// texture flags like TEX_TILE or TEX_MIRROR
	LONG tf = 0;// = (TTile&TEX_TILE)+(TSaml&TEX_MIRROR);//???????
	if (TTile) tf = tf + TEX_TILE;
	if (TSaml) tf = tf + TEX_MIRROR;
	LONG texflag = tf;
			
	Vector snm;
	//LONG nn;
	UVWStruct uvw;


	Neighbor NG;
	LONG *dadr;
	LONG dcnt;
	LONG pn;

	if (!NG.Init(SourcePointCnt,SourcePolygon,SourcePolygonCnt,NULL)) {  return 4;}
	
	for (LONG c=0;c<SourcePointCnt;c++){//Point Count
		NG.GetPointPolys(c,&dadr,&dcnt);

		pn = dadr[0];
		uvw = uvwtag->Get(pn);

		if (SourcePolygon[pn].a == c){p=uvw.a;}
		if (SourcePolygon[pn].b == c){p=uvw.b;}
		if (SourcePolygon[pn].c == c){p=uvw.c;}
		if (SourcePolygon[pn].c != SourcePolygon[pn].d){
			if (SourcePolygon[pn].d == c){p=uvw.d;}
		}

		p.x = (p.x-OffsetX) / LenX; p.y = (p.y-OffsetY) / LenY;
		(*coladr)[c] = VectorGray(TexChan1->Sample(NULL,&p,&d,&n,t,texflag,0.0,1.0));
	}//for (c=0;c<SourcePointCnt;c++){
	TexChan1->FreeTexture();//FREE

	return 0;	
}//GetVertexColor
*/


/*
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//""""""""""""""""""" Gibt Vertex Displacer (langsammer ???)""""""""""""""""""""""""
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
LONG GetVertexDisplace(Vector **noradr, BaseDocument *doc, PolygonObject *ob,
					   TextureTag *texturetag = NULL,const LONG tnum = 0, const LONG chnr = 0)
{
	if (!ob || !doc) return 1;

	LONG			c;
	Neighbor		NG;
	LONG			*dadr;
	LONG			dcnt;
	UVWStruct		uvw;			
	Vector			snm;
	LONG			pn;
	Vector			VertexN;
	//BaseDocument	*doc = ob->GetDocument();if (!doc) return 2; 
	//Get Points
	LONG			SourcePointCnt = ob->GetPointCount();
	Vector			*SourcePoints = ob->GetPoint(); if (!SourcePoints) return 2; 
	//Get Polygon
	LONG			SourcePolygonCnt = ob->GetPolygonCount();
	CPolygon			*SourcePolygon = ob->GetPolygon(); if (!SourcePolygon) return 3; 

	//Create Array
	if (!(*noradr)) (*noradr) = (Vector*)GeAlloc(sizeof(Vector)*SourcePointCnt+1);if (!(*noradr)) return 4;
	
	//Get Texture Tag
	if (!texturetag) texturetag = (TextureTag*)ob->GetTag(Ttexture,tnum); if (!texturetag) return 5;//No Textur
	BaseMaterial	*mat = texturetag->GetMaterial(); if (!mat) return 6;//No Material
	BaseChannel		*TexChan1 = mat->GetChannel(chnr); if (!TexChan1) return 7;//NO Channel

	//Get UVW TAG
	UVWTag			*uvwtag =  (UVWTag*)ob->GetTag(Tuvw); if (!uvwtag) return 8;//No Uvw Tag
	LONG			uvwcnt = uvwtag->GetDataCount();

	//Get Texture Data
	BaseContainer	texdata = texturetag->GetData();
	Bool			TTile = texdata.GetBool(TEXTUREKEY_TILE);//Kacheln
	Bool			TSaml = texdata.GetBool(TEXTUREKEY_SEAMLESS);//Natlös
	String			TSel = texdata.GetString(TEXTUREKEY_RESTRICTION);//Selection
	Real			OffsetX = texdata.GetReal(TEXTUREKEY_OFFSETX)+0.0001;//TEST 0.0001 ???
	Real			OffsetY = texdata.GetReal(TEXTUREKEY_OFFSETY)+0.0001;//TEST 0.0001 ???
	Real			LenX = texdata.GetReal(TEXTUREKEY_LENGTHX);
	Real			LenY = texdata.GetReal(TEXTUREKEY_LENGTHY);


	//SelectionTag *PSelTag;
	//BaseSelect *PSel;
	//Bool Bsel = FALSE;
	//if (TSel != ""){
	//	PSelTag = GetYPTag(ob,TSel);if (!PSelTag) return 9;//No Selection
	//	PSel = PSelTag->GetBaseSelect();//if (!PSel) return 14;
	//	Bsel = TRUE;
	//}
	

	//----------------------
	//InitRenderStruct
	InitRenderStruct irs;
	LONG fps = doc->GetFps();
	irs.fps = fps;
	BaseTime BTtime = doc->GetTime();
	irs.time = BTtime;
	irs.doc = doc;
	Filename dpath = doc->GetPath();
	irs.docpath  = dpath.Get();

	Vector p=Vector(0.0,0.0,0.0);		// texture position in UVW coordinates (x(1 or 0) und y(1 or 0))
	Vector d=Vector(0.0,0.0,0.0);		// MIP sample radius in UVW coordinates
	Vector n=Vector(0.0,0.0,0.0);		// surface normal
	Real t=BTtime.Get();				// current time in seconds				
	LONG tf = 0;// = (TTile&TEX_TILE)+(TSaml&TEX_MIRROR);//???????
	if (TTile) tf = tf + TEX_TILE;
	if (TSaml) tf = tf + TEX_MIRROR;
	LONG texflag = tf;// texture flags like TEX_TILE or TEX_MIRROR

	TexChan1->InitTexture(&irs);
	if (!NG.Init(SourcePointCnt,SourcePolygon,SourcePolygonCnt,NULL)) return 10;
	
	for (c=0;c<SourcePointCnt;c++){//Point Count
		NG.GetPointPolys(c,&dadr,&dcnt);

		VertexN = Vector(0.0,0.0,0.0);
		for(LONG i=0;i<dcnt;i++){
			VertexN += CalcFaceNormal(SourcePoints,SourcePolygon[dadr[i]]);
		}
		//(*noradr)[c] = (VertexN / dcnt);

		pn = dadr[0];
		uvw = uvwtag->Get(pn);

		if (SourcePolygon[pn].a == c){p=uvw.a;}
		if (SourcePolygon[pn].b == c){p=uvw.b;}
		if (SourcePolygon[pn].c == c){p=uvw.c;}
		if (SourcePolygon[pn].c != SourcePolygon[pn].d){
			if (SourcePolygon[pn].d == c){p=uvw.d;}
		}

		p.x = (p.x-OffsetX) / LenX; p.y = (p.y-OffsetY) / LenY;
		(*noradr)[c] = (VertexN / dcnt)*VectorGray(TexChan1->Sample(NULL,&p,&d,&n,t,texflag,0.0,1.0));
		//(*coladr)[c] = VectorGray(TexChan1->Sample(NULL,&p,&d,&n,t,texflag,0.0,1.0));
	}//for (c=0;c<SourcePointCnt;c++){
	TexChan1->FreeTexture();//FREE

	return 0;	
}//GetVertexColNor
*/