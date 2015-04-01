#ifndef __CHSAMPLER_H
#define __CHSAMPLER_H

class ChSampler
{
	private:
		BaseChannel *chan1;
		Real t;
		Vector d;
		Vector n;
		LONG texflag;
		Real offsetX;
		Real offsetY;
		Real lenX;
		Real lenY;

	public:
		ChSampler(BaseDocument *doc, TextureTag *textag,LONG chacol = 0);
		~ChSampler(void);

		Vector Sample(const Vector &p);



};

#endif//end