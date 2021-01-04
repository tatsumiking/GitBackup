DllExport BOOL wkftblopen(LPTSTR wakupath);
DllExport void wkftblclose();
DllExport BOOL wkftblelementread(int idx, LPTSTR wakuname, DBL *rsx, DBL *rsy, DBL *rex, DBL *rey, int *rkind);
DllExport BOOL wkftblelementwrite(int idx, LPTSTR wakuname, DBL sx, DBL sy, DBL ex, DBL ey, int kind);
DllExport void wkftblbackup(LPTSTR wakupath);
DllExport BOOL wkftblinit(LPTSTR wakupath);
DllExport void wkftblrecbufset(TCHAR *recbuf, LPTSTR wakuname, DBL sx, DBL sy, DBL ex, DBL ey, int kind);
DllExport BOOL wkftblwakminimaxget(LPTSTR fname, DBL *dsx, DBL *dsy, DBL *dex, DBL *dey, int *kind);
