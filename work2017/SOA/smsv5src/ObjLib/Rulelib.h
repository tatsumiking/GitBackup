DllExport void rulelibgetkind(int kindno, long *pattern, int *style, TCHAR *stylename);
DllExport void rulelibmove(HDC PaintDC, DBL x1, DBL y1, long ptn, int style, DBL *sum);
DllExport void rulelibline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, long ptn, int style, DBL *sum);
DllExport void rulelibarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, long ptn, int style, DBL *sum);
DllExport void rulelibbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, long ptn, int style, DBL *sum);
DllExport void rulelibfreecurve(HDC PaintDC, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, long ptn, int style, DBL *sum);
void rulelibenko(HDC PaintDC, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag, long ptn, int style, DBL *sum);
void rulelibragenko(HDC PaintDC, DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, long ptn, int style, DBL *sum);
void rulelibdivragenko(HDC PaintDC, DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2, long ptn, int style, DBL *sum);
void ruleliblineout(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, long ptn, int style, DBL *sum);
void rulelibdotlineout(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, long ptn, DBL *sum);
void rulelibwavelineout(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, long ptn, DBL *sum);
