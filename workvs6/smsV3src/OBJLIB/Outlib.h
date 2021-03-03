typedef struct tagTTYINFO
{
   HANDLE  idComDev ;
   BYTE    bPort;
   BOOL    fConnected, fXonXoff, fLocalEcho, fNewLine, fAutoWrap,
           fUseCNReceive, fDisplayErrors;
   BYTE    bByteSize, bFlowCtrl, bParity, bStopBits ;
   DWORD   dwBaudRate ;
   WORD    wCursorState ;
   HFONT   hTTYFont ;
   LOGFONT lfTTYFont ;
   DWORD   rgbFGColor ;
   int     xSize, ySize, xScroll, yScroll, xOffset, yOffset,
           nColumn, nRow, xChar, yChar ;
   HANDLE      hPostEvent, hWatchThread, hWatchEvent ;
   HWND        hTermWnd ;
   DWORD       dwThreadID ;
   OVERLAPPED  osWrite, osRead ;
} TTYINFO, NEAR *NPTTYINFO ;

