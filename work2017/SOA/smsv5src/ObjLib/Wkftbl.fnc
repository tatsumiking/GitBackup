BOOL wkftblopen(LPTSTR wakupath);
void wkftblclose();
BOOL wkftblelementread(int idx, LPTSTR wakuname, DBL *rsx, DBL *rsy, DBL *rex, DBL *rey, int *rkind);
BOOL wkftblelementwrite(int idx, LPTSTR wakuname, DBL sx, DBL sy, DBL ex, DBL ey, int kind);
void wkftblbackup(LPTSTR wakupath);
BOOL wkftblinit(LPTSTR wakupath);
void wkftblrecbufset(TCHAR *recbuf, LPTSTR wakuname, DBL sx, DBL sy, DBL ex, DBL ey, int kind);
BOOL wkftblwakminimaxget(LPTSTR fname, DBL *dsx, DBL *dsy, DBL *dex, DBL *dey, int *kind);
