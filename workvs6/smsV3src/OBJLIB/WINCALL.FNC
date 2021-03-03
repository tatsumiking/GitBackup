DllExport int CALLBACK TTFontList(NEWLOGFONT FAR *lpnlf, NEWTEXTMETRIC FAR *lpntm, int nFontType, LPARAM lParam);
DllExport int DispatchWindows(HWND hWin);
DllExport void SetParentWindows(HWND hWin);
DllExport int DispatchParentWindows();
