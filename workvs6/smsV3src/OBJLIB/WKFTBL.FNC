DllExport BOOL wkftblopen(LPSTR wakupath);
DllExport void wkftblclose();
DllExport BOOL wkftblelementread(int idx, LPSTR wakuname, DBL *rsx, DBL *rsy, DBL *rex, DBL *rey, int *rkind);
DllExport BOOL wkftblelementwrite(int idx, LPSTR wakuname, DBL sx, DBL sy, DBL ex, DBL ey, int kind);
DllExport void wkftblbackup(LPSTR wakupath);
DllExport BOOL wkftblinit(LPSTR wakupath);
DllExport void wkftblrecbufset(char *recbuf, LPSTR wakuname, DBL sx, DBL sy, DBL ex, DBL ey, int kind);
DllExport BOOL wkftblwakminimaxget(LPSTR fname, DBL *dsx, DBL *dsy, DBL *dex, DBL *dey, int *kind);
