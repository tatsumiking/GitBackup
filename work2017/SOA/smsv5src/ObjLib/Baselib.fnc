LPTSTR baselibCrDelFgets(LPTSTR buf, int max, FILE *fp);
void basesleep(int imax, int jmax);
int baselastspacedel(LPTSTR fontname);
int basechpath(LPTSTR path);
LPTSTR basecsvstrdivcpy(LPTSTR sp, LPTSTR retstr);
int basedivcsvdata(LPTSTR csvsp[], LPTSTR csvbuf);
LPTSTR basedivstrg(LPTSTR sp, LPTSTR retsp, TCHAR sparator);
int basestrgcntget(LPTSTR strg);
LPTSTR basesptodnum(LPTSTR tsp, DBL *retnum);
LPTSTR basesptoinum(LPTSTR tsp, int *retnum);
LPTSTR basesptostrg(LPTSTR tsp, LPTSTR retsp);
void baseaddsafix(LPTSTR FileName, LPTSTR Safix);
void basechangesafix(LPTSTR FileName, LPTSTR Safix);
void basetimeget(int *hh, int *mm, int *ss, int *tss);
void basedateget(int *yy, int *mm, int *dd, int *ww);
int basesetcodetbl(LPTSTR strg, UINT code[]);
void baseupperstrg(LPTSTR strg);
void basespacedelcpy(LPTSTR dststrg, LPTSTR srcstrg);
void basehantozen(TCHAR dst[], TCHAR src[], int max);
int basegetparentpath(LPTSTR lpPath);
int baseiskanji(UINT hc);
int basesjtoj(int sjis);
int basejtosj(int  jis);
void basesjtokuten(int sjis, int *ku, int *ten);
void basejtokuten(int jis, int *ku, int *ten);
int basekutentojis(int kuten);
void  Dispatch();
