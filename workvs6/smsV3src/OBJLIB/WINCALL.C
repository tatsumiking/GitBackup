
/*
	Easy Scanner システム共通ソース	1992.12.01	by stan
base		wincall.c	処理関数
	Copyright 株）ハリマ
*/

#include	<windows.h>

#include	<stdio.h>
#include	<math.h>
#include	"..\stdef.h"
#include	"dlldef.h"

typedef struct{
    GLOBALHANDLE	hGmem;
	int				nCount;
}ENUMER;

typedef struct tagNEWLOGFONT{
    int     lfHeight;
    int     lfWidth;
    int     lfEscapement;
    int     lfOrientation;
    int     lfWeight;
    BYTE    lfItalic;
    BYTE    lfUnderline;
    BYTE    lfStrikeOut;
    BYTE    lfCharSet;
    BYTE    lfOutPrecision;
    BYTE    lfClipPrecision;
    BYTE    lfQuality;
    BYTE    lfPitchAndFamily;
    char    lfFaceName[LF_FACESIZE];
    BYTE    lfFullName [2 * LF_FACESIZE];
    BYTE    lfStyle [LF_FACESIZE];
} NEWLOGFONT;

#include	"wincall.fnc"

HWND hWinParent;

DllExport int CALLBACK TTFontList(NEWLOGFONT FAR *lpnlf, NEWTEXTMETRIC FAR *lpntm, int nFontType, LPARAM lParam)
{
	ENUMER FAR *penumer;
	LPSTR       lpFontData, lpNextName;
	DWORD		dwSize;

	penumer = (ENUMER FAR *)lParam;
	if(nFontType & TRUETYPE_FONTTYPE && lpnlf->lfFaceName[0] != '@'){
		dwSize = (DWORD)LF_FACESIZE * (penumer->nCount + 1);
		if(!GlobalReAlloc(penumer->hGmem, dwSize, GMEM_MOVEABLE)){
			return(0);
		}
		lpFontData = GlobalLock(penumer->hGmem);
		lpNextName = lpFontData + ((penumer->nCount) * LF_FACESIZE);
		lstrcpy(lpNextName, (LPSTR)lpnlf->lfFaceName);
		GlobalUnlock(penumer->hGmem);
		penumer->nCount++;
	}
	return(1);
}

DllExport int DispatchWindows(HWND hWin)
{
	MSG		msg;

	while(1){
		if(PeekMessage(&msg, hWin, 0, 0, PM_REMOVE)){
			if(msg.message == WM_CHAR && msg.wParam == VK_ESCAPE){
				return(FAIL);
			}
			else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else{
			break;
		}
	}
	return(0);
}

DllExport void SetParentWindows(HWND hWin)
{
	hWinParent = hWin;
}

DllExport int DispatchParentWindows()
{
	MSG		msg;

	while(1){
		if(PeekMessage(&msg, hWinParent, 0, 0, PM_REMOVE)){
			if(msg.message == WM_CHAR && msg.wParam == VK_ESCAPE){
				return(FAIL);
			}
			else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else{
			break;
		}
	}
	return(0);
}

