DllExport int kamonlibfileopen(TCHAR *basefname, TCHAR *mode);
DllExport int kamonlibopen(TCHAR *mode);
DllExport int kamonlibwakuopen(TCHAR *mode);
DllExport void kamonlibclose();
DllExport int kamonlibreadhead(int no, kamonheadtype *head);
DllExport int kamonlibwritehead(int no, kamonheadtype *head, int pcnt);
DllExport int kamonlibreadatrxy(int *atr, DBL *dx, DBL *dy);
DllExport int kamonlibwriteatrxy(int atr, DBL dx, DBL dy);
int kamonlibfilenullset(int cnt, LPTSTR fname);
