int		nswflag;		// 0 UNDO(ON,OFF) 1 CLIP(ON,OFF) 2 MJONE(ON,OFF);
DllExport void scrninit();
DllExport void scrnend();
DllExport void scrnsetwakunodisp(int flag);
DllExport int scrngetwakunodisp();
DllExport void scrnsetnamedisp(int flag);
DllExport int scrngetnamedisp();
DllExport void scrnonswflag(int bit);
DllExport void scrnoffswflag(int bit);
DllExport int scrncheckswflag(int bit);
DllExport void scrnonundoflag();
DllExport void scrnoffundoflag();
DllExport int scrncheckundoflag();
DllExport void scrnonnoactflag();
DllExport void scrnoffnoactflag();
DllExport int scrnchecknoactflag();
DllExport void scrnsetwinarea(int sx, int sy, int ex, int ey);
DllExport void scrnsetwintop(DBL topx, DBL topy);
DllExport void scrngetwinarea(int *sx, int *sy, int *ex, int *ey);
DllExport void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
DllExport void scrnsetmmsize(DBL Xsize, DBL Ysize);
DllExport void scrnsetmmworkstart(DBL sx, DBL sy);
DllExport void scrnsetmmworksize(DBL xsize, DBL ysize);
DllExport void scrngetmmworkstart(DBL *sx, DBL *sy);
DllExport void scrngetmmworksize(DBL *xsize, DBL *ysize);
DllExport void scrninitsizeelement(LPINT width, LPINT hight);
DllExport void scrnsetrltvelement();
DllExport void scrngetrltvsize(DBL *pdRXsize, DBL *pdRYsize);
DllExport void scrnsetrltvsize(DBL dRXsize, DBL dRYsize);
DllExport void scrnsetcntrmoveflag(int flag);
DllExport int scrngetcntrmoveflag();
DllExport void scrnsetstartmoveflag(int flag);
DllExport int scrngetstartmoveflag();
DllExport void scrnsetstartmovemm(DBL xmm, DBL ymm);
DllExport void scrngetstartmovemm(LPDBL xmm, LPDBL ymm);
DllExport void scrnsetrltvmove(DBL xmove, DBL ymove);
DllExport void scrngetrltvmove(LPDBL xmove, LPDBL ymove);
DllExport void scrnsetdisprltvmove(DBL xmove, DBL ymove);
DllExport void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove);
DllExport void scrnsetrltvtop(DBL xTop, DBL yTop);
DllExport void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
DllExport void scrnsetrltvtopreverce(DBL xTop, DBL yTop, DBL xSize, DBL ySize);
DllExport void scrnrevercecheck(int *flagx, int *flagy);
DllExport DBL scrngetbasetime();
DllExport void scrnsetbasetime(DBL time);
DllExport DBL scrnsetxytime(DBL xtime, DBL ytime);
DllExport void scrngetxytime(LPDBL xtime, LPDBL ytime);
DllExport void scrnresetzoomtime();
DllExport BOOL scrncheckzoomall();
DllExport void scrnenvbasetimezoomup();
DllExport void scrnenvbasetimezoomdown();
DllExport void scrngetwinunitsize(int *pnUnitXsize, int *pnUnitYsize);
DllExport void scrndxyrltvtomm(LPDBL x, LPDBL y);
DllExport void scrndxymmtorltv(LPDBL x, LPDBL y);
DllExport void scrndlenrltvtomm(LPDBL len);
DllExport void scrndlenmmtorltv(LPDBL len);
DllExport void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
DllExport void scrnrltvareaget(DBL *sx, DBL *sy, DBL *ex, DBL *ey);
DllExport int scrnrltvareaychk(DBL y);
DllExport int scrnrltvareaxychk(DBL x, DBL y);
DllExport int scrnrltvareachk(DBL sx, DBL sy, DBL ex, DBL ey);
DllExport int scrnrltvarealegal(LPDBL sx, LPDBL sy, LPDBL ex, LPDBL ey);
int scrnareasignget(DBL num, DBL mini, DBL max);
