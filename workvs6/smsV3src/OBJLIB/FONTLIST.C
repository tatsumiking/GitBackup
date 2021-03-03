/*
	Easy Scanner システム共通ソース	1992.12.01	by stan
base		fontlist.c	スクリーン関係関数
	Copyright 株）ハリマ
*/

#include	<windows.h>

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

#include	"fontlist.fnc"

int PASCAL CALLBACK TTSiftjisFontList(NEWLOGFONT FAR *lpnlf, NEWTEXTMETRIC FAR *lpntm, int nFontType, LPARAM lParam)
{
	ENUMER FAR *penumer;
	LPSTR       lpFontData, lpNextName;
	DWORD		dwSize;
	int			len;

	penumer = (ENUMER FAR *)lParam;
	if(nFontType & TRUETYPE_FONTTYPE && lpnlf->lfFaceName[0] != '@'){
		if(lpnlf->lfCharSet == SHIFTJIS_CHARSET){
			dwSize = (DWORD)LF_FACESIZE * (penumer->nCount + 1);
			if(!GlobalReAlloc(penumer->hGmem, dwSize, GMEM_MOVEABLE)){
				return(0);
			}
			lpFontData = GlobalLock(penumer->hGmem);
			lpNextName = lpFontData + ((penumer->nCount) * LF_FACESIZE);
			len = strlen((LPSTR)lpnlf->lfFaceName);
			lstrcpy(lpNextName, (LPSTR)lpnlf->lfFaceName);
			lpNextName[len] = '\0';
			lpNextName[len+1] = '\0';
			GlobalUnlock(penumer->hGmem);
			penumer->nCount++;
		}
	}
	return(1);
}

int PASCAL CALLBACK TTAnsiFontList(NEWLOGFONT FAR *lpnlf, NEWTEXTMETRIC FAR *lpntm, int nFontType, LPARAM lParam)
{
	ENUMER FAR *penumer;
	LPSTR       lpFontData, lpNextName;
	DWORD		dwSize;
	int			len;

	penumer = (ENUMER FAR *)lParam;
	if(nFontType & TRUETYPE_FONTTYPE && lpnlf->lfFaceName[0] != '@'){
		if(lpnlf->lfCharSet == ANSI_CHARSET){
			dwSize = (DWORD)LF_FACESIZE * (penumer->nCount + 1);
			if(!GlobalReAlloc(penumer->hGmem, dwSize, GMEM_MOVEABLE)){
				return(0);
			}
			lpFontData = GlobalLock(penumer->hGmem);
			lpNextName = lpFontData + ((penumer->nCount) * LF_FACESIZE);
			len = strlen((LPSTR)lpnlf->lfFaceName);
			lstrcpy(lpNextName, (LPSTR)lpnlf->lfFaceName);
			lpNextName[len] = '\0';
			lpNextName[len+1] = '\0';
			GlobalUnlock(penumer->hGmem);
			penumer->nCount++;
		}
	}
	return(1);
}

