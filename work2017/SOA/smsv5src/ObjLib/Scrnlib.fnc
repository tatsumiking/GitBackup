void scrninit();
void scrnend();
void scrnsetwakunodisp(int flag);
int scrngetwakunodisp();
void scrnsetnamedisp(int flag);
int scrngetnamedisp();
void scrnonswflag(int bit);
void scrnoffswflag(int bit);
int scrncheckswflag(int bit);
void scrnonundoflag();
void scrnoffundoflag();
int scrncheckundoflag();
void scrnonnoactflag();
void scrnoffnoactflag();
int scrnchecknoactflag();
void scrnsetwinarea(int sx, int sy, int ex, int ey);
void scrnsetwintop(DBL topx, DBL topy);
void scrngetwinarea(int *sx, int *sy, int *ex, int *ey);
void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
void scrnsetmmsize(DBL Xsize, DBL Ysize);
void scrnsetmmworkstart(DBL sx, DBL sy);
void scrnsetmmworksize(DBL xsize, DBL ysize);
void scrngetmmworkstart(DBL *sx, DBL *sy);
void scrngetmmworksize(DBL *xsize, DBL *ysize);
void scrninitsizeelement(LPINT width, LPINT hight);
void scrnsetrltvelement();
void scrngetrltvsize(DBL *pdRXsize, DBL *pdRYsize);
void scrnsetrltvsize(DBL dRXsize, DBL dRYsize);
void scrnsetcntrmoveflag(int flag);
int scrngetcntrmoveflag();
void scrnsetstartmoveflag(int flag);
int scrngetstartmoveflag();
void scrnsetstartmovemm(DBL xmm, DBL ymm);
void scrngetstartmovemm(LPDBL xmm, LPDBL ymm);
void scrnsetrltvmove(DBL xmove, DBL ymove);
void scrngetrltvmove(LPDBL xmove, LPDBL ymove);
void scrnsetdisprltvmove(DBL xmove, DBL ymove);
void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove);
void scrnsetrltvtop(DBL xTop, DBL yTop);
void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
void scrnsetrltvtopreverce(DBL xTop, DBL yTop, DBL xSize, DBL ySize);
void scrnrevercecheck(int *flagx, int *flagy);
DBL scrngetbasetime();
void scrnsetbasetime(DBL time);
DBL scrnsetxytime(DBL xtime, DBL ytime);
void scrngetxytime(LPDBL xtime, LPDBL ytime);
void scrnresetzoomtime();
BOOL scrncheckzoomall();
void scrnenvbasetimezoomup();
void scrnenvbasetimezoomdown();
void scrngetwinunitsize(int *pnUnitXsize, int *pnUnitYsize);
void scrndxyrltvtomm(LPDBL x, LPDBL y);
void scrndxymmtorltv(LPDBL x, LPDBL y);
void scrndlenrltvtomm(LPDBL len);
void scrndlenmmtorltv(LPDBL len);
void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
void scrnrltvareaget(DBL *sx, DBL *sy, DBL *ex, DBL *ey);
int scrnrltvareaychk(DBL y);
int scrnrltvareaxychk(DBL x, DBL y);
int scrnrltvareachk(DBL sx, DBL sy, DBL ex, DBL ey);
int scrnrltvarealegal(LPDBL sx, LPDBL sy, LPDBL ex, LPDBL ey);