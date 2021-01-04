#ifndef __CFIGURE_H
#define __CFIGURE_H

//C 交点検出処理クラス
class CFigure : public CObject
{
public:
	CFigure();
public:
	int type;
	DBL x1, y1;
	DBL x2, y2;
	DBL x3, y3;
	DBL x4, y4;
};

#endif
