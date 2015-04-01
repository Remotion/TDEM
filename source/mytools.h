#ifndef __MYTOOLS_H
#define __MYTOOLS_H

//"""""""""""""""  ColorFilter """"""""""""""""""""""""""2
//Zu LANGSAMM ????????
inline Real ColorFilter(const Vector &v,const LONG t)
{
	switch (t){
		case 0: return (v.x+v.y+v.z)*THIRD;//Alle Gray
		case 1: return (v.x);//Red
		case 2: return (v.y);//Gren
		case 3: return (v.z);//Blu
		case 4: return RGBToHSV(v).x;//H
		case 5: return RGBToHSV(v).y;//S
		case 6: return RGBToHSV(v).z;//V
	}
	return (v.x+v.y+v.z)*THIRD;
}


 //################ ZU LANGSAM ??
inline Bool IsIn(Vector pp, Real ri, LONG form)
{
		//???????
		Real			ri2 = ri*ri; //Torus Tube Radius
		Real			ro = 1.0; // R
		Real			ro2 = ro*ro; //Torus Radius ???
		Real			aa = (1.0-ri)*0.5;
		Real			cc = ri + aa;
		aa = aa*aa;
		cc = cc*cc;
		//???????

		switch(form){//TEST ?????
		case 0: 
			return TRUE;
		break;
		case 1://Cube OK
			return (pp.x>=-1.0 && pp.x<=1.0 && pp.y>=-1.0 && pp.y<=1.0 && pp.z>=-1.0&& pp.z<=1.0);
		break;
		case 2://Ellipse OK
			return (pp*pp <= 1.0);
		break;
		case 3://Zylinder OK
			return ((pp.x*pp.x+pp.z*pp.z) <= 1.0 && pp.y>=-1.0 && pp.y<=1.0);
		break;
		case 4://Kegel OK
			return ((pp.x*pp.x+pp.z*pp.z) <= ((pp.y+1) * (pp.y+1) / 4) && pp.y>=-1.0 && pp.y<=1.0);
		break;
		case 5://Torus  DONT WORK ??? ??????????????????
			//return ((sqrt(pp.x*pp.x+pp.z*pp.z)-cc) * (sqrt(pp.x*pp.x+pp.z*pp.z)-cc) + pp.y*pp.y > aa);
			return ((sqrt(pp.x*pp.x+pp.z*pp.z)-2.0) * (sqrt(pp.x*pp.x+pp.z*pp.z)-2.0) + pp.y*pp.y-1.0 < 0.0);
			//(sqrt(x^2+y^2)-2)^2+z^2-1
		break;
		case 6://Tube OK
			return ((pp.x*pp.x+pp.z*pp.z) <= 1.0 && (pp.x*pp.x+pp.z*pp.z) > ri2 && pp.y>=-1.0 && pp.y<=1.0);
		break;
		}

	return FALSE;
}

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
Bool ComputeBarycenter (const Vector &rkV0, const Vector &rkV1,
    const Vector &rkV2, const Vector &rkV3, const Vector &rkP,
    Real afNumer[4], Real &rfDenom);

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
const String GetNumName(const BaseContainer data,const LONG name_id,const LONG num_id);

//""""""""""""""""""""""""""""""""""""""
SelectionTag *GetYPTag(BaseObject *ob,const String name);// PolygonSel

//""""""""""""""""""""""""""""""""""""""
SelectionTag *GetPSTag(BaseObject *ob,const String name);// PointSel

//""""""""""""""""""""""""""""""""""""""
VariableTag *GetVMTag(BaseObject *ob,const String name);//Vertex(VariableTag)

//""""""""""""""""""""""""""""""""""""""
PluginTag *GetPlugTag(BaseObject *ob,LONG id);//Find Plugins Tag

//""""""""""""""""""""""""""""""""""""""
LONG GetVertexNormal(Vector **noradr, PolygonObject *ob, LONG SourcePointCnt, Vector *SourcePoints);

//""""""""""""""""""""""""""""""""""""""
LONG GetVertexNormalSafe(Vector **noradr, PolygonObject *ob);

//""""""""""""""""""""""""""""""""""""""
LONG GetVertexNormalMG(Vector **noradr, PolygonObject *ob, LONG SourcePointCnt, Vector *SourcePoints);

//""""""""""""""""""""""""""""""""""""""
LONG GetVertexColor(Real **coladr, PolygonObject *ob,  LONG SourcePointCnt, Vector *SourcePoints, 
					BaseDocument *doc, LONG chacol = 0,LONG tnum = 0);//Gray

//""""""""""""""""""" Gibt Vertex Displacer (langsammer ???)""""""""""""""""""""""""
LONG GetVertexDisplace(Vector **noradr, BaseDocument *doc, PolygonObject *ob,
					   TextureTag *texturetag = NULL,const LONG tnum = 0, const LONG chnr = 0);

#endif//end