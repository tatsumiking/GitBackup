DllExport void wakuenvinit(LPTSTR envfile);
DllExport void wakuenvsetkaisyaname(LPTSTR name);
DllExport void wakuenvgetpaperelement();
DllExport void wakuenvlistgetpapername(int idx, LPTSTR papername);
DllExport void wakuenvsetpaperelement();
DllExport void wakuenvdelpaper(LPTSTR delpapername);
int wakuenvdeletefile(LPTSTR sp);
DllExport void wakuenvsaveoutputenv(LPTSTR wakusavefname);
DllExport void wakuenvloadoutputenv(LPTSTR wakuloadfname);
DllExport void wakuenvfilebackup(LPTSTR infname, LPTSTR outfname);
DllExport void wakuenvend();
DllExport void wakuenvsetgrupname(LPTSTR name);
DllExport void wakuenvgetgrupname(LPTSTR name);
DllExport void wakuenvsetpapername(LPTSTR name);
DllExport void wakuenvgetpapername(LPTSTR name);
DllExport void wakuenvsetpapersize(DBL xsize, DBL ysize);
DllExport void wakuenvgetpapersize(DBL *xsize, DBL *ysize);
DllExport void wakuenvsetpaperstart(DBL sx, DBL sy);
DllExport void wakuenvgetpaperstart(DBL *sx, DBL *sy);
DllExport void wakuenvsetpaperarea(DBL width, DBL hight);
DllExport void wakuenvgetpaperarea(DBL *width, DBL *hight);
DllExport void wakuenvsetpaperpath(LPTSTR path);
DllExport void wakuenvgetpaperpath(LPTSTR path);
DllExport void wakuenvgetfullpaperpath(LPTSTR retpath);
DllExport void wakuenvsetselectwakuidx(int idx);
DllExport void wakuenvgetselectwakuidx(int *idx);
DllExport void wakuenvlistaddwakuname(int slctidx, LPTSTR wakuname);
DllExport void wakuenvlistsort(int slctidx1, int slctidx2);
DllExport void wakuenvlistdelwakuname(int slctidx);
DllExport void wakuenvlistgetwakuname(int slctidx, LPTSTR wakuname);
DllExport int wakuenvlistsrchwakuname(LPTSTR srchname);
DllExport void wakuenvgetfixflag(int *flag);
DllExport void wakuenvsetfixflag(int flag);
DllExport void wakuenvgettrnspar(DBL *par);
DllExport void wakuenvsettrnspar(DBL par);
DllExport void wakuenvgetmatching(LPTSTR mstr);
DllExport void wakuenvsetmatching(LPTSTR mstr);