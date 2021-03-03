void fontlibInit();
void fontlibEnd();
void fontlibFontNameAndType(LPSTR fontname, LPSTR fontfullname, int *retftype);
void fontlibTrnsTimeSet(DBL dsx, DBL dsy, DBL dwidth, DBL dhight);
void fontlibPlotOut(UINT sjiscode);
void fontlibDispOut(HDC PaintDC, UINT sjiscode);
