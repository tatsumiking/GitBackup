
//
//
// eswin  imehndl.cpp  ì˙ñ{åÍÇ©Ç»äøéöä÷åWä÷êî
//
//  Copyright (c) 1999 by THEON

#include "stdafx.h"

#include "imehndl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
}

CImeHndl::CImeHndl()
{
}

CImeHndl::~CImeHndl()
{
}

void CImeHndl::ImeHndlOn(HWND hWnd)
{
	BOOL	fOpen, bFlag;
	HIMC	hIMC;

	bFlag = TRUE;
	hIMC = ImmGetContext(hWnd);
	fOpen = ImmGetOpenStatus(hIMC);
	if(fOpen != bFlag){
		ImmSetOpenStatus(hIMC,bFlag);
	}
	ImmReleaseContext(hWnd,hIMC);
}

void CImeHndl::ImeHndlOff(HWND hWnd)
{
	BOOL	fOpen, bFlag;
	HIMC	hIMC;

	bFlag = FALSE;
	hIMC = ImmGetContext(hWnd);
	fOpen = ImmGetOpenStatus(hIMC);
	if(fOpen != bFlag){
		ImmSetOpenStatus(hIMC,bFlag);
	}
	ImmReleaseContext(hWnd,hIMC);
}

void CImeHndl::ImeHndlKanji(HWND hWnd)
{
	DWORD	dwCnvStat;
	DWORD	dwSentence;
	BOOL	fOpen, bFlag;
	HIMC	hIMC;

	bFlag = TRUE;
	hIMC = ImmGetContext(hWnd);
	fOpen = ImmGetOpenStatus(hIMC);
	if(fOpen == TRUE){
		ImmGetConversionStatus(hIMC, &dwCnvStat, &dwSentence);
		dwCnvStat |= IME_CMODE_FULLSHAPE;
		dwCnvStat &= ~IME_CMODE_KATAKANA;
		//dwCnvStat |= IME_CMODE_ROMAN;
		dwSentence = IME_SMODE_PHRASEPREDICT;
		ImmSetConversionStatus(hIMC, dwCnvStat, dwSentence);
	}
	ImmReleaseContext(hWnd,hIMC);
}

void CImeHndl::ImeHndlKana(HWND hWnd)
{
	DWORD	dwCnvStat;
	DWORD	dwSentence;
	BOOL	fOpen, bFlag;
	HIMC	hIMC;

	bFlag = TRUE;
	hIMC = ImmGetContext(hWnd);
	fOpen = ImmGetOpenStatus(hIMC);
	if(fOpen == TRUE){
		ImmGetConversionStatus(hIMC, &dwCnvStat, &dwSentence);
		dwCnvStat &= ~IME_CMODE_FULLSHAPE;
		dwCnvStat |= IME_CMODE_KATAKANA;
		//dwCnvStat |= IME_CMODE_ROMAN;
		dwSentence = IME_SMODE_PHRASEPREDICT;
		ImmSetConversionStatus(hIMC, dwCnvStat, dwSentence);
	}
	ImmReleaseContext(hWnd,hIMC);
}

void CImeHndl::ImeHndlNon(HWND hWnd)
{
	BOOL	fOpen, bFlag;
	HIMC	hIMC;

	bFlag = FALSE;
	hIMC = ImmGetContext(hWnd);
	fOpen = ImmGetOpenStatus(hIMC);
	if(fOpen != bFlag){
		ImmSetOpenStatus(hIMC,bFlag);
	}
	ImmReleaseContext(hWnd,hIMC);
}
