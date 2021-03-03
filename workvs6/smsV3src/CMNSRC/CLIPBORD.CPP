
/////////////////////////////////////////////////////////////////////////////
// CClipBord

#include "stdafx.h"
#include "math.h"

#include "..\cmnsrc\clipbord.h"

extern "C" {
}

CClipBord::CClipBord()
{
}

int CClipBord::SaveData()
{
/*		
	HANDLE		hndl;
	LPVOID		lptr;
	char		exepath[FNAMEMAX];
	char		fname[FNAMEMAX];
	FILE		*fp;
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER *lpbminfo;
	int			xbyte;
	int			pltsize;
	long		size;

		if(IsClipboardFormatAvailable(CF_TEXT)){
			hndl = GetClipboardData(CF_TEXT);
			CClipBord.SaveText(hndl);
			lptr = GlobalLock(hndl);
			
			senvgetexepath(exepath);
			sprintf(fname, "%s\\strgtext.lst", exepath);
			if((fp = fopen(fname, "w")) != NULL){
				fputs((LPSTR)lptr, fp);
				fclose(fp);
				type = CF_TEXT;
			}
			if(lptr != NULL){
				GlobalUnlock(hndl);
			}
		}
		else if(IsClipboardFormatAvailable(CF_DIB)){
			hndl = GetClipboardData(CF_DIB);
			lptr = GlobalLock(hndl);
			lpbminfo = (BITMAPINFOHEADER*)lptr;
			switch(lpbminfo->biBitCount){
			case 1:
				pltsize = sizeof(RGBQUAD) * 2;
				break;
			case 4:
				pltsize = sizeof(RGBQUAD) * 16;
				break;
			case 8:
				pltsize = sizeof(RGBQUAD) * 256;
				break;
			case 24:
				pltsize = sizeof(RGBQUAD) * 0;
				break;
			}
			size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pltsize;

			bmf.bfType = 0x4d42;
			bmf.bfSize = size + lpbminfo->biSizeImage;
			bmf.bfReserved1 = 0;
			bmf.bfReserved2 = 0;
			bmf.bfOffBits = size;
			senvgetexepath(exepath);
			sprintf(fname, "%s\\clipb%03d.bmp", exepath, m_nBmpCnt);
			m_nBmpCnt++;

			CFile	cpFile;
			if((cpFile.Open(fname, CFile::modeCreate)) == TRUE){
				cpFile.Write(&bmf, sizeof(BITMAPFILEHEADER));
				size = bmf.bfSize - sizeof(BITMAPFILEHEADER);
				cpFile.WriteHuge(lptr, size);
				cpFile.Close();
				type = CF_DIB;
			}
			if(lptr != NULL){
				GlobalUnlock(hndl);
			}
		}
*/
	return(0);
}
