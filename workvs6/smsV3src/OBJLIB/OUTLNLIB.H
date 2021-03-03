
extern "C" {
int outlnlibcheckragsharp(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
void outlnlibstartpoint(DBL dot, DBL x1, DBL y1, DBL x2, DBL y2, DBL *x, DBL *y);
void outlnlibendpoint(DBL dot, DBL x1, DBL y1, DBL x2, DBL y2, DBL *x, DBL *y);
void outlnlibtreepoint(int atr, DBL dot, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL *x, DBL *y);
void outlnlibgetroundrag(DBL x0, DBL y0, DBL x1, DBL y1, DBL *rag);
void outlnlibsetroundpoint(DBL x0, DBL y0, DBL rag, DBL x1, DBL y1, DBL *x, DBL *y);
void outlnlibgetcrosspoint(DBL x1, DBL y1, DBL rag1, DBL x2, DBL y2, DBL rag2, DBL *x, DBL *y);
void outlnliblegallength(int atr, double dot, double x0, double y0, double *dx, double *dy);
void outlnlibfilldrowoutbeze(HDC hDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
void outlnlibfilldrowoutenko(HDC hDC, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag);
void outlnlibfilldrowoutmove(HDC hDC, DBL dx, DBL dy);
void outlnlibfilldrowoutline(HDC hDC, DBL x1, DBL y1, DBL x2, DBL y2);
void outlnlibdrowoutbeze(HDC hDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
void outlnlibdrowoutenko(HDC hDC, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag);
void outlnlibdrowoutmove(HDC hDC, DBL dx, DBL dy);
void outlnlibdrowoutline(HDC hDC, DBL x1, DBL y1, DBL x2, DBL y2);
}
