#include "c4d_tools.h"
#include "c4d_memory.h"
#include "c4d_shaderplugin.h"
#include "c4d_general.h"
#include "c4d_basechannel.h"
#include "c4d_file.h"
#include "c4d_basedocument.h"
#include "c4d_basecontainer.h"
#include "c4d_baseobject.h"
#include "bitmappreview.h"


BitmapPreview::BitmapPreview(void)
{
	chan = NULL;
	doc = NULL;
	ob = NULL;
	filter = -1;
	enabled = TRUE;

	col=AllocBaseBitmap();
}

BitmapPreview::~BitmapPreview(void)
{
	FreeBaseBitmap(col);
}

Bool BitmapPreview::GetMinSize(LONG &w,LONG &h)
{
	w = h = 80;
	return TRUE;
}

void BitmapPreview::Sized(LONG w,LONG h)
{
	if (!col) return;
	col->Init(w,h);
	CalcPreview();
}

void BitmapPreview::Draw(LONG x1,LONG y1,LONG x2,LONG y2)
{
	if (!col) return;
	LONG w = col->GetBw();
	LONG h = col->GetBh();
	if (chan && enabled) DrawBitmap(col,0,0,w,h,0,0,w,h,BMP_NORMAL);
	else DrawBitmap(col,0,0,w,h,0,0,w,h,BMP_EMBOSSED);
}

//InputEvent
Bool BitmapPreview::InputEvent(const BaseContainer &msg)
{
	switch (msg.GetLong(BFM_INPUT_CHANNEL)){
		case BFM_INPUT_MOUSELEFT: 
			CalcPreview();//CalcPreview if user Clieck to Area
		break;
		case BFM_INPUT_MOUSERIGHT: 
			enabled = !enabled;//??? 
			CalcPreview();
		break;
	}
	//GePrint("IMPUT");
	return TRUE;
}

void BitmapPreview::CalcPreview(void)
{
	if (!col) return;
	//GePrint("DRAW");

	LONG x,y;
	UWORD r,g,b;
	LONG bw=col->GetBw();
	LONG bh=col->GetBh();
	if (!bh || !bw) return;

	//if (!doc || !chan || !enabled){//Error
	if (!chan || !enabled){//Error
		if (doc){
			//??????????????????????????
			//ICON WORK ???
			BaseObject *op = NULL;
			IconData idat;
			op = ob;
			if (!op) op = doc->GetActiveObject();if (!op) return;
			op->GetIcon(&idat);//if (!idat) return;
			if (idat.bmp){
				if (!idat.h || !idat.w) return;

				LONG ah = (bh/2-idat.h/2)%bh;
				LONG aw = (bw/2-idat.w/2)%bw;

				col->Clear(0,0,0);

				for (y=0; y<idat.h; y++){
					for (x=0; x<idat.w; x++){
						idat.bmp->GetPixel(	idat.x + (x % idat.w),
											idat.y + (y % idat.h),
											&r, &g, &b);
						col->SetPixel(x+aw,y+ah,LONG(r),LONG(g),LONG(b));
					}
				}

			}
			Redraw();
			return;
			//??????????????????????????
		}else{
			col->Clear(0,0,0);//?????
			Redraw();
			return;
		}
	}

	Real bwi=1.0/Real(bw),bhi=1.0/Real(bh);

	PLUGIN_DATA *plug = NULL;
	plug = chan->GetPluginData();if (!plug) return;

	//ChannelData cd;
	Vector	v;
	LONG vr,vg,vb;

	//InitRenderStruct
	InitRenderStruct irs;
	LONG fps = doc->GetFps();
	irs.fps = fps;
	BaseTime BTtime = doc->GetTime();
	irs.time = BTtime;
	irs.doc = doc;
	Filename dpath = doc->GetPath();
	irs.docpath  = dpath.Get();

	VolumeData *vd=NULL;//STANDATD
	Vector p=Vector(0.0,0.0,0.0);		// texture position in UVW coordinates (x(1 or 0) und y(1 or 0))
	Vector d=Vector(0.5*bwi,0.5*bhi,0.0);		// MIP sample radius in UVW coordinates
	Vector n=Vector(0.0,1.0,0.0);		// surface normal
	Real t=BTtime.Get();//WICHTIG

	chan->InitTexture(&irs);

	for (y=0; y<bh; y++){
		for (x=0; x<bw; x++){
			p = Vector(x*bwi,y*bhi,0.0);

			v=CutColor(chan->Sample(vd,&p,&d,&n,t,0,0.0,1.0));
			//FILTER ???
			/*
			switch (filter){
				case 0: v = (v.x+v.y+v.z)*THIRD;//Alle Gray
					break;
				case 1: v = (v.x);//Red
					break;
				case 2: v = (v.y);//Gren
					break;
				case 3: v =(v.z);//Blu
					break;
				case 4: v = RGBToHSV(v).x;//H
					break;
				case 5: v = RGBToHSV(v).y;//S
					break;
				case 6: v = RGBToHSV(v).z;//V
					break;
			}
			*/
			switch (filter){//Alternative Filter??
				case 0: v = v;//Alle Gray
					break;
				case 1: v = Vector(v.x,0.0,0.0);//Red
					break;
				case 2: v = Vector(0.0,v.y,0.0);//Gren
					break;
				case 3: v = Vector(0.0,0.0,v.z);//Blu
					break;
				case 4: v = RGBToHSV(v).x;//H
					break;
				case 5: v = RGBToHSV(v).y;//S
					break;
				case 6: v = RGBToHSV(v).z;//V
					break;
			}

			vr = v.x*COLOR;
			vg = v.y*COLOR;
			vb = v.z*COLOR;
			col->SetPixel(x,y,vr,vg,vb);
			/*
			v=CutColor(chan->Sample(vd,&p,&d,&n,t,0,0.0,1.0))*COLOR;
			col->SetPixel(x,y,v.x,v.y,v.z);
			*/
		}
	}

	chan->FreeTexture();

	Redraw();
}