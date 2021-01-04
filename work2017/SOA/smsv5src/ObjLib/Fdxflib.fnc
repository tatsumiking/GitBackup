void fdxflinemodeset(int mode);
int fdxffileopen(TCHAR *filename);
int fdxffileclose();
void fdxfstartfile();
void fdxfendfile();
void fdxflinesave(DBL x1, DBL y1, DBL x2, DBL y2);
void fdxfarcsave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
void fdxfbezesave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
void fdxffreecurvesave(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
