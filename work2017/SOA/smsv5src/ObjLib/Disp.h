DllExport void dispsethdc(HDC hDC);
DllExport void dispinit();
DllExport void dispend();
void dispmove(DBL dx, DBL dy);
void dispdrawline(DBL x1, DBL y1, DBL x2, DBL y2);
void dispdraw3penko(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
void dispdrawenko(DBL x0, DBL y0, DBL dr, DBL rag1, DBL rag2, DBL x1, DBL y1, DBL x2, DBL y2);
