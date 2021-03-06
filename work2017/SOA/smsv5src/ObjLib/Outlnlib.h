DllExport int outlnlibcheckragsharp(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
DllExport void outlnlibstartpoint(DBL dot, DBL x1, DBL y1, DBL x2, DBL y2, DBL *x, DBL *y);
DllExport void outlnlibendpoint(DBL dot, DBL x1, DBL y1, DBL x2, DBL y2, DBL *x, DBL *y);
DllExport void outlnlibtreepoint(int atr, DBL dot, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL *x, DBL *y);
DllExport void outlnlibgetroundrag(DBL x0, DBL y0, DBL x1, DBL y1, DBL *rag);
DllExport void outlnlibsetroundpoint(DBL x0, DBL y0, DBL rag, DBL x1, DBL y1, DBL *x, DBL *y);
DllExport void outlnlibgetcrosspoint(DBL x1, DBL y1, DBL rag1, DBL x2, DBL y2, DBL rag2, DBL *x, DBL *y);
DllExport void outlnliblegallength(int atr, double dot, double x0, double y0, double *dx, double *dy);
DllExport void outlnlibfilldrowoutbeze(HDC hDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
DllExport void outlnlibfilldrowoutenko(HDC hDC, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag);
DllExport void outlnlibfilldrowoutmove(HDC hDC, DBL dx, DBL dy);
DllExport void outlnlibfilldrowoutline(HDC hDC, DBL x1, DBL y1, DBL x2, DBL y2);
DllExport void outlnlibdrowoutbeze(HDC hDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
DllExport void outlnlibdrowoutenko(HDC hDC, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag);
DllExport void outlnlibdrowoutmove(HDC hDC, DBL dx, DBL dy);
DllExport void outlnlibdrowoutline(HDC hDC, DBL x1, DBL y1, DBL x2, DBL y2);
