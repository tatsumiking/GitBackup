DllExport LPTSTR baselibCrDelFgets(LPTSTR buf, int max, FILE *fp);
DllExport int baselibFputs(LPTSTR buf, FILE *fp);
DllExport int baselibTCHARStrToCharStr(TCHAR *lpSrcStr, char *lpDstStr, int nBufMax);
DllExport int baselibCharStrToTCHARStr(char *lpSrcStr, TCHAR *lpDstStr, int nBufMax);
DllExport void basesleep(int imax, int jmax);
DllExport int baselastspacedel(LPTSTR fontname);
DllExport int basechpath(LPTSTR path);
DllExport LPTSTR basecsvstrdivcpy(LPTSTR sp, LPTSTR retstr);
DllExport int basedivcsvdata(LPTSTR csvsp[], LPTSTR csvbuf);
DllExport LPTSTR basecsvstrdiv(LPTSTR sp);
DllExport LPTSTR basedeltopspace(LPTSTR sp);
DllExport void basedellastspace(LPTSTR sp);
DllExport LPTSTR basedeldoublequote(LPTSTR sp);
DllExport LPTSTR basedivstrg(LPTSTR sp, LPTSTR retsp, TCHAR sparator);
DllExport int basestrgcntget(LPTSTR strg);
DllExport LPTSTR basesptodnum(LPTSTR tsp, DBL *retnum);
DllExport LPTSTR basesptoinum(LPTSTR tsp, int *retnum);
DllExport LPTSTR basesptostrg(LPTSTR tsp, LPTSTR retsp);
DllExport void baseaddsafix(LPTSTR lpFileName, LPTSTR lpSafix);
DllExport void basechangesafix(LPTSTR lpFileName, LPTSTR lpSafix);
DllExport LPTSTR basegetsafix(LPTSTR lpFileName);
DllExport void basetimeget(int *hh, int *mm, int *ss, int *tss);
DllExport void basedateget(int *yy, int *mm, int *dd, int *ww);
DllExport int basesetcodetbl(LPTSTR strg, int code[]);
DllExport void baseupperstrg(LPTSTR strg);
DllExport void basespacedelcpy(LPTSTR dststrg, LPTSTR srcstrg);
DllExport int baseCodeHanToZen(int code);
DllExport void basehantozen(LPTSTR dst, LPTSTR src, int max);
DllExport int basegetparentpath(LPTSTR lpPath);
DllExport void baselibSpaceDelete(LPTSTR lpStr);
DllExport int baselibU16ToSjis(int utf8);
DllExport int baselibSjisToU16(int sjis);
DllExport int baseiskanji(int hc);
DllExport int basesjtoj(int sjis);
DllExport int basejtosj(int  jis);
DllExport void basesjtokuten(int sjis, int *ku, int *ten);
DllExport void basejtokuten(int jis, int *ku, int *ten);
DllExport int basekutentojis(int kuten);
DllExport void baseDispatch();
LPSTR basesjishantozen(LPSTR lpSrcStr);
LPSTR basesjisasciihantozen(char ch, LPSTR lpZen);
LPSTR basesjiskanahantozen(char ch, LPSTR lpZen);
LPWSTR baseunihantozen(LPWSTR lpSrcStr);
WCHAR baseuniasciihantozen(WCHAR ch);
WCHAR baseunikanahantozen(WCHAR ch);
