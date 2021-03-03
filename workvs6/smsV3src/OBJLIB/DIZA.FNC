DllExport void dizargbtblinit(LPSTR fname);
void dizargbtbltrns(DBL rnum, DBL gnum, DBL bnum, DBL radd, DBL gadd, DBL badd);
void dizacntrstset(DBL cntrst);
DllExport void dizaclrchng(int *r, int *g, int *b);
DllExport void dizargbtocmy(int r, int g, int b, int *c, int *m, int *y, int *bk);
void dizargbtotrgb(int r, int g, int b, int *tr, int *tg, int *tb);
int dizabitchk(int x, int y, int c);
DllExport void dizabitcmychk(int x, int y, int *rc, int *rm, int *ry);
