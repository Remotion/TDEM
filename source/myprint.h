#ifndef __MYPRINT_H
#define __MYPRINT_H

#include "c4d_general.h"
#include "c4d_baseobject.h"

//""""""""""""""""""""""""""""""""""""""
inline Vector vnorm(const Vector v)	//VNORM
{
	//return v / Len(v);
	return v / Sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

//""""""""""""""""  PRINT String  """"""""""""""""""""""
inline void print(String t)
{
  GePrint(t);
}

//""""""""""""""""  PRINT Bool  """"""""""""""""""""""
inline void print(Bool t)
{
  if (t) GePrint("TRUE");
  else GePrint("FALSE");
}

//""""""""""""""""  PRINT BaseObject  """"""""""""""""""""""
inline void print(BaseObject *t)
{
	if (t) GePrint(t->GetName());
}

//""""""""""""""""  PRINT REAL  """"""""""""""""""""""
inline void print(Real t)
{
  GePrint(RealToString(t,-1,6));
}

//""""""""""""""""  PRINT LONG  """"""""""""""""""""""
inline void print(LONG t)
{
  GePrint(LongToString(t));
}

//""""""""""""""""  PRINT LONG  """"""""""""""""""""""
inline void print(CHAR t)
{
  GePrint(LongToString(t));
}

//""""""""""""""""  PRINT VECTOR """"""""""""""""""""""
inline void print(Vector t)
{
  GePrint(RealToString(t.x)+" | "+RealToString(t.y)+" | "+RealToString(t.z));
}

//""""""""""""""""  PRINT VECTOR """"""""""""""""""""""
inline void print(Matrix t)
{
  GePrint(RealToString(t.off.x)+" ~ "+RealToString(t.off.y)+" ~ "+RealToString(t.off.z));
}

/*
inline void printtyp(LONG t)
{
	switch (t){
		case 5100: GePrint("Opolygon");break;
		case 5101: GePrint("Ospline	");break;
		case 5102: GePrint("");break;
		case 5103: GePrint("");break;		
		case 5104: GePrint("");break;
		case 5105: GePrint("");break;
		case 5106: GePrint("");break;
		case 5107: GePrint("");break;
		case 5108: GePrint("");break;
		case 5109: GePrint("");break;
		case 5110: GePrint("");break;
		case 5111: GePrint("");break;
		case 5112: GePrint("");break;		
		case 5113: GePrint("");break;
		case 5114: GePrint("");break;
		case 5115: GePrint("");break;
		case 5116: GePrint("");break;
		case 5117: GePrint("");break;
		case 5118: GePrint("");break;
		case 5119: GePrint("");break;

		case 5154: GePrint("");break;		
	}
}
*/

#endif//end