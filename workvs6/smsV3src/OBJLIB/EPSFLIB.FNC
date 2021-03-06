DllExport int epsffileopen(char *filename);
DllExport int epsffileclose();
DllExport int epsfstartfile(char *fname, DBL maxx, DBL maxy);
DllExport int epsfboxsstrgsave(LPSTR strg, int ty, int od, DBL fl, DBL sp, DBL sx, DBL sy, DBL ex, DBL ey);
DllExport int epsfendfile();
DllExport int epsfstartblock();
DllExport int epsfoutlinethick(DBL thickmm);
DllExport int epsffillclrset(int rval, int gval, int bval);
DllExport int epsfoutlineclrset(int rval, int gval, int bval);
DllExport int epsfendblock();
DllExport int epsfstartrejion();
DllExport int epsfendrejion();
DllExport int epsfstartnorejion();
DllExport int epsfendnorejion();
DllExport int epsflinesave(DBL x1, DBL y1, DBL x2, DBL y2);
DllExport int epsfarcsave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
int epsfenkotobeze(double sx, double sy, double ex, double ey, double x0, double y0, double r, double srag, double erag);
DllExport int epsffreecurvesave(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
int epsfsaveragcurve(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2);
int epsfsaveragbeze(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2);
DllExport int epsfbezesave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
int epsfenkosave(DBL sx, DBL sy, DBL x0, DBL y0, DBL r, DBL srag, DBL erag);
DllExport void epsfmmxyget(DBL *x, DBL *y);
DllExport void epsfmmget(DBL *len);
int epsfstepxyget(DBL *x, DBL *y);
int epsfdottostep(DBL *num);
int epsfmmtostep(DBL *num);
int epsfheadwrite(char *fname, DBL maxx, DBL maxy);
int epsffilecopy(char fname[]);
int epsfstrgout(LPSTR sp);
int epsfputs(char *buffer);
