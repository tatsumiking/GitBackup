int pdrawchkclipmode();
void pdrawclipmode(int flag);
void pdrwpoint(HDC PaintDC, DBL x, DBL y);
void pdrwmove(HDC PaintDC, DBL x1, DBL y1);
void pdrwline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2);
void pdrwarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
void pdrwbezecntrl(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
void pdrwbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
void pdrwbezelow(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
void pdrwfreecurve(HDC PaintDC, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
void pdrwmoveto(HDC PaintDC, DBL dx, DBL dy);
void pdrwlineto(HDC PaintDC, DBL sx, DBL sy, DBL ex, DBL ey);
BOOL pdrwatrbezechk(int atr1, int atr2, int atr3);
BOOL pdrwatrarcchk(int atr1, int atr2, int atr3);
BOOL pdrwatrcurvechk(int atr1);
BOOL pdrwatrcurvecontchk(int atr1);
DBL pdrwstartragget(int atr0, int atr1, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2);
DBL pdrwendragget(int atr1, int atr2, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
int pdrwreverseatrget(int atr1, int atr2);
