DllExport void diblibinit();
DllExport void diblibend();
DllExport void diblibResetFileNo();
DllExport void diblibRGBtoHLS(int r, int g, int b, LPDBL H, LPDBL L, LPDBL S);
DllExport int diblibGetClrHLSLen(DBL chkH, DBL chkL, DBL chkS, DBL srcH, DBL srcL, DBL srcS);
DllExport void diblibcreatebmpfile(LPSTR bmpfilename);
DllExport void diblibBmpSmallPictureCreate(LPSTR lpSrcFname, LPSTR lpDstFname, DBL xsize, DBL ysize);
DllExport void diblibBmpOnePlanePictureCreate(LPSTR lpSrcFname, LPSTR lpDstFname, COLORREF chkclr, int nlimit);
DllExport void diblibBmpOnePlanePictureCreateES98(LPSTR lpSrcFname, LPSTR lpDstFname, COLORREF chkclr, int nlimit);
DllExport void diblibBmp256PlanePictureCreate(LPSTR lpSrcFname, LPSTR lpDstFname, DBL xsize, DBL ysize, COLORREF chkclr);
DllExport void diblibCmprimgToBmp(LPSTR dotfname, LPSTR bmpfname);
DllExport void diblibGetDotRGB(int nbitcnt, LPRGBQUAD lpRGBQuad, LPBYTE lpLineImg, int x, int *r, int *g, int *b);
DllExport void diblibSetDotRGB(int nbitcnt, LPRGBQUAD lpRGBQuad, LPBYTE lpLineImg, int x, int r, int g, int b);
DllExport BOOL diblibGlobalLock();
DllExport void diblibGlobalUnlock();
