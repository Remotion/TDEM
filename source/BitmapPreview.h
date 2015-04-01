#ifndef __BITMAPPREVIEW
#define __BITMAPPREVIEW

#include "c4d_gui.h"
#include "c4d_basebitmap.h"

class BaseBitmap;
class GeUserArea;

class BitmapPreview : public GeUserArea
{
	private:
		BaseBitmap *col;
	public:
		BitmapPreview(void);
		~BitmapPreview(void);

		BaseChannel *chan;
		BaseDocument *doc;
		BaseObject   *ob;
		BaseTime lastTime;
		LONG filter;
		Bool enabled;

		void CalcPreview(void);

		virtual Bool GetMinSize(LONG &w,LONG &h);
		virtual void Sized(LONG w,LONG h);
		virtual void Draw(LONG x1,LONG y1,LONG x2,LONG y2);
		virtual Bool InputEvent   (const BaseContainer &msg);
};

#endif