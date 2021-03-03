
extern "C" {
void mdfyinit(DBL setlimit);
int mdfyfirst(XYType sp[], int smax, XYType dp[]);
int mdfysecond(XYType sp[], int smax, XYType dp[]);
int mdfychkline(XYType sp[], int si, int ei);
int mdfystartendget(XYType sp[], int smax, int si);
int mdfychkfure(int sx, int sy, int ex, int ey, XYType sp[], int start, int end);
int mdfyoverlap(XYType sp[], int smax, XYType dp[]);
int mdfycorner(XYType sp[], int smax, XYType dp[]);
int mdfycornerchk(XYType sp[], int smax, XYType dp[], int dmax, int p1, int *x, int *y, int *xx, int *yy);
int nextpget(XYType sp[], int smax, XYType dp[], int dmax, int p1);
int mdfyburepoint(XYType sp[], int smax, XYType dp[]);
int mdfysubragget(XYType sp[], int smax, int p1);
int mdfyisshoutline(XYType sp[], int smax, int p1);
}
