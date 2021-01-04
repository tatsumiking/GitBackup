int kamonlibfileopen(TCHAR *basefname, TCHAR *mode);
int kamonlibopen(TCHAR *mode);
int kamonlibwakuopen(TCHAR *mode);
void kamonlibclose();
int kamonlibreadhead(int no, kamonheadtype *head);
int kamonlibwritehead(int no, kamonheadtype *head, int pcnt);
int kamonlibreadatrxy(int *atr, DBL *dx, DBL *dy);
int kamonlibwriteatrxy(int atr, DBL dx, DBL dy);
