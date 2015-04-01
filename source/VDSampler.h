#ifndef __VDSAMPLER_H
#define __VDSAMPLER_H

//#include "c4d_general.h"
#include "c4d_memory.h"
//#include "c4d_tools.h"
#include "c4d_basedocument.h"
#include "c4d_basecontainer.h"
#include "c4d_baseobject.h"
#include "c4d_basematerial.h"
#include "c4d_basetag.h"
#include "c4d_basechannel.h"
#include "c4d_shaderplugin.h"
#include "mytools.h"
#include "myprint.h"

class VdSampler
{
	private:
		BaseChannel *TexChan1;
		VolumeData	*vd;
		TexData		*tex;
		RayObject	*rop;
		InitRenderStruct  *irs;
		Matrix		omg;
		Real		t;
		Vector		d;
		Vector		n;
		LONG		texflag;
		Real		offsetX;
		Real		offsetY;
		Real		lenX;
		Real		lenY;
		Real		bluoff;
		Real		bluscal;
		Vector		pp;

	public:
		VdSampler(void);
		~VdSampler(void);
		LONG Init(BaseDocument *doc, BaseObject *ob,const Matrix op_mg, TextureTag *texturetag, const LONG chnr = 0);

		
		/*
		//####################### SampleUV #######################
		Vector SampleUV(const Vector &uv);
		//####################### Sample3D #######################
		Real SampleFilterUV(const Vector &uv = 0.0,const LONG fil = 0);
		//####################### Sample3D #######################
		Vector Sample3D(const Vector &p = 0.0, const Vector &uv = 0.0);
		//####################### Sample3D #######################
		Real SampleFilter3D(const Vector &p = 0.0, const Vector &uv = 0.0,const LONG fil = 0);
		*/

		
		//####################### SampleUV #######################
		inline Vector SampleUV(const Vector &uv)
		{
			pp			= uv;
			pp.x		= (uv.x-offsetX) / lenX; pp.y = (uv.y-offsetY) / lenY;//Kaheln Problem ????
			return TexChan1->Sample(NULL,&pp,&d,&n,t,texflag,bluoff,bluscal);
		}
		//####################### Sample3D #######################
		inline Real SampleFilterUV(const Vector &uv = 0.0,const LONG fil = 0)
		{
			pp			= uv;
			pp.x		= (uv.x-offsetX) / lenX; pp.y = (uv.y-offsetY) / lenY;//Kaheln Problem ????
			return ColorFilter(TexChan1->Sample(NULL,&pp,&d,&n,t,texflag,bluoff,bluscal),fil);
		}


		//####################### Sample3D #######################
		inline Vector Sample3D(const Vector &p = 0.0, const Vector &uv = 0.0)
		{
			vd->p		= p*omg;
			vd->back_p	= vd->p;
			pp			= uv;
			return TexChan1->Sample(vd,&pp,&d,&n,t,texflag,bluoff,bluscal);
		}
		//####################### Sample3D #######################
		inline Real SampleFilter3D(const Vector &p = 0.0, const Vector &uv = 0.0,const LONG fil = 0)
		{
			vd->p		= p*omg;
			vd->back_p	= vd->p;
			pp			= uv;
			return ColorFilter(TexChan1->Sample(vd,&pp,&d,&n,t,texflag,bluoff,bluscal),fil);
		}
};

#endif//end