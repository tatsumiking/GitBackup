void rulelibgetkind(int kindno, long *pattern, int *style, TCHAR *stylename);
void rulelibmove(HDC PaintDC, DBL x1, DBL y1, long ptn, int style, DBL *sum);
void rulelibline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, long ptn, int style, DBL *sum);
void rulelibarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, long ptn, int style, DBL *sum);
void rulelibbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, long ptn, int style, DBL *sum);
void rulelibfreecurve(HDC PaintDC, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, long ptn, int style, DBL *sum);
