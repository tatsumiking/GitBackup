DllExport void strctpaintdc(HDC hDC);
DllExport void strctboxsbasepointset(DBL x[], DBL y[]);
DllExport void strctboxstrnspointset(DBL x[], DBL y[]);
DllExport int strcttrnsboxspoint(DBL *x, DBL *y);
DllExport int strcconrarc(DBL *xa, DBL *ya, DBL *xb, DBL *yb, DBL *xc, DBL *yc, DBL *xd, DBL *yd, DBL cnrr);
int conrarcpoint(DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL r, DBL *x, DBL *y);
DllExport int strclinecrossget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, DBL *x, DBL *y);
void strctcrcllinecrosschk(DBL x0, DBL y0, DBL rag1, DBL rag2, DBL x1, DBL y1, DBL x3, DBL y3, DBL dx1, DBL dy1, DBL dx2, DBL dy2, DBL *dx, DBL *dy);
void strctcrclcrosschk(DBL xx0, DBL xy0, DBL xrag1, DBL xrag2, DBL yx0, DBL yy0, DBL yrag1, DBL yrag2, DBL dx1, DBL dy1, DBL dx2, DBL dy2, DBL *dx, DBL *dy);
void strctcrclcdtrns(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL dlt, DBL *dx, DBL *dy);
int strccrclcross(DBL x01, DBL y01, DBL r1, DBL x02, DBL y02, DBL r2, DBL *x1, DBL *y1, DBL *x2, DBL *y2);
int strccrcllinecross(DBL x0, DBL y0, DBL r, DBL sx, DBL sy, DBL ex, DBL ey, DBL *x1, DBL *y1, DBL *x2, DBL *y2);
void strctDrawArcLine(DBL x0, DBL y0, DBL srag, DBL erag, DBL dr);
void strctDrawPoint(DBL x, DBL y);
