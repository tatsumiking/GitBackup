
#include "stdafx.h"

#include <string.h>
#include <atlbase.h>
#include <atlstr.h>
#include <comutil.h>

#include "CharLib.h"

CCharLib::CCharLib(void)
{
}

CCharLib::~CCharLib(void)
{
}
int CCharLib::TCHARStr_CharStr(TCHAR *lpSrcStr, char *lpDstStr, int nBufMax)
{
	int nSrcLen, nDstLen;

	nSrcLen = _tcslen(lpSrcStr);
#ifdef UNICODE
	nDstLen = WideCharToMultiByte(CP_ACP, 0, lpSrcStr, -1, lpDstStr, nBufMax, NULL, NULL);
#else
	strcpy(lpDstStr, lpSrcStr);
	nDstLen = strlen(lpDstStr);
#endif
	return(nDstLen);
}
int CCharLib::CharStr_TCHARStr(char *lpSrcStr, TCHAR *lpDstStr, int nBufMax)
{
	int nSrcLen, nDstLen;
	nSrcLen = strlen(lpSrcStr);
#ifdef UNICODE
	nDstLen = MultiByteToWideChar(CP_ACP, 0, lpSrcStr, -1, lpDstStr, nBufMax);
	//MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, lpSrcStr, nSrcLen, lpDstStr, nBufMax);
#else
	strcpy(lpDstStr, lpSrcStr);
	nDstLen = _tcslen(lpDstStr);
#endif
	return(nDstLen);
}
int CCharLib::WCharStr_CString(WCHAR *lpSrcStr, CString *pcsDstStr)
{
	CString csStr(lpSrcStr);
	size_t  tRetLen;

	pcsDstStr->SetString(csStr); 
	tRetLen = pcsDstStr->GetLength();
	return (int)tRetLen;
}

int CCharLib::CString_WCharStr(CString csSrcStr, WCHAR *lpDstStr, int nBufMax)
{
	size_t	tSrcLen;
	size_t  tRetLen;

	tSrcLen = csSrcStr.GetLength() + 1;
	wcscpy_s(lpDstStr, nBufMax, csSrcStr.GetBuffer(nBufMax));
	tRetLen = wcslen(lpDstStr);
	return (int)tRetLen;
}

int CCharLib::CharStr_CString(char *lpSrcStr, CString *pcsDstStr)
{
	CString csStr(lpSrcStr);
	size_t  tRetLen;

	pcsDstStr->SetString(csStr); 
	tRetLen = pcsDstStr->GetLength();
	return (int)tRetLen;
}

int CCharLib::CString_CharStr(CString csSrcStr, char *lpDstStr, int nBufMax)
{
	int		len;

	len = WideCharToMultiByte(CP_ACP,0,csSrcStr,-1,lpDstStr,nBufMax,NULL,NULL);
	return(len);
}

int CCharLib::WCharStr_CharStr(WCHAR *lpSrcStr, char *lpDstStr, int nBufMax)
{
	int		len;

	len = WideCharToMultiByte(CP_ACP,0,lpSrcStr,-1,lpDstStr,nBufMax,NULL,NULL);
	return(len);
}

int CCharLib::CharStr_WCharStr(char *lpSrcStr, WCHAR *lpDstStr, int nBufMax)
{
	int		len;

	len = MultiByteToWideChar(CP_ACP,0,lpSrcStr,-1,lpDstStr,nBufMax);
	return(len);
}

int	CCharLib::DivCsvStr(char *lpSrcStr, char *lpDstStr[])
{
	LPSTR	lpNextSP;
	int		idx;

	idx = 0;
	while(1){
		if(*lpSrcStr == '\0'){
			break;
		}
		lpNextSP = DivCsvStr(lpSrcStr);
		lpSrcStr = SkipSpace(lpSrcStr);
		lpSrcStr = DelDoubleQuartStr(lpSrcStr);
		lpDstStr[idx] = lpSrcStr;
		lpSrcStr = lpNextSP;
		idx++;
	}
	return(idx);
}

LPSTR CCharLib::DivCsvStr(char *lpSrcStr)
{
	int	nDoubleQuart;

	nDoubleQuart = OFF;
	while(1){
		if(*lpSrcStr == '\0'){
			break;
		}
		if(IsKanji(*lpSrcStr) == TRUE){
			lpSrcStr += 2;
		}else if(*lpSrcStr == '\\'){
			// エスケープシーケンスをセット
			lpSrcStr += 2;
		}else{
			if(nDoubleQuart == OFF){
				if(*lpSrcStr == ','){
					*lpSrcStr = '\0';
					lpSrcStr++;
					break;
				}else if(*lpSrcStr == '"'){
					nDoubleQuart = ON;
				}
			}else{	// nDoubleQuart == ON;
				if(*lpSrcStr == '"'){
					nDoubleQuart = OFF;
				}
			}
			lpSrcStr++;
		}
	}
	return(lpSrcStr);
}

LPSTR CCharLib::DelDoubleQuartStr(char *lpSrcStr)
{
	int	nDoubleQuart;
	LPSTR	lpTopSp;

	nDoubleQuart = OFF;
	lpTopSp = lpSrcStr;
	while(1){
		if(*lpSrcStr == '\0'){
			break;
		}
		if(IsKanji(*lpSrcStr) == TRUE){
			lpSrcStr += 2;
		}else if(*lpSrcStr == '\\'){
			// エスケープシーケンス
			lpSrcStr += 2;
		}else{
			if(nDoubleQuart == ON){
				if(*lpSrcStr == '"'){
					*lpSrcStr = '\0';
				}
			}else{
				if(*lpSrcStr == '"'){	// 最初のダブルクォート
					nDoubleQuart = ON;
					*lpSrcStr = '\0';
					lpTopSp = (lpSrcStr + 1);
				}
			}
			lpSrcStr++;
		}
	}
	return(lpTopSp);
}

LPSTR CCharLib::SkipSpace(char *lpSrcStr)
{
	while(1){
		if(*lpSrcStr == '\0'){
			break;
		}
		if(IsKanji(*lpSrcStr) != TRUE){
			if(strncmp(lpSrcStr, "　", 2) != 0){
				break;
			}
			lpSrcStr += 2;
		}else if(*lpSrcStr == ' '){
			lpSrcStr++;
		}else if(*lpSrcStr == '\t'){
			lpSrcStr++;
		}else{
			break;
		}
	}
	return(lpSrcStr);
}
int	CCharLib::TrnsCodeTable(char *lpSrcStr, UINT nCodeTbl[])
{
	LPSTR	sp;
	UINT	hc, lc;
	int		idx;

	sp = lpSrcStr;
	idx = 0;
	while(1){
		if(*sp == '\0'){
			nCodeTbl[idx] = 0;
			break;
		}
		if(IsKanji(*sp)){
			hc = (*sp) & 0x00ff;
			lc = (*(sp+1)) & 0x00ff;
			nCodeTbl[idx] = ((hc << 8) & 0xff00) | (lc & 0x00ff);
			idx++;
			sp += 2;
		}else{
			nCodeTbl[idx] = (*sp) & 0x00ff;
			idx++;
			sp++;
		}
	}
	return(idx);
}


int CCharLib::IsKanji(char ch)
{
	UINT	hc;

	hc = ch & 0x00ff;
	if((0x0080 <= hc && hc <= 0x009f)
	|| (0x00e0 <= hc && hc <= 0x00ff)){
		return(TRUE);
	}
	else{
		return(FALSE);
	}
}

int CCharLib::AddSafix(LPSTR FileName, LPSTR Safix)
{
	int		len1;
	int		len2;

	len1 = (int)strlen(FileName);
	len2 = (int)strlen(Safix);
	if(FileName[len1-len2-1] != '.'){
		strcat_s(FileName, FNAMEMAX, ".");
		strcat_s(FileName, FNAMEMAX, Safix);
		return(TRUE);
	}
	return(FALSE);
}

int CCharLib::ChangeSafix(LPSTR FileName, LPSTR Safix)
{
	int		len1;
	int		len2;

	len1 = (int)strlen(FileName);
	len2 = (int)strlen(Safix);
	if(FileName[len1-len2-1] != '.'){
		strcat_s(FileName, FNAMEMAX, ".");
		strcat_s(FileName, FNAMEMAX, Safix);
		return(TRUE);
	}
	return(FALSE);
}

int CCharLib::CmpSafixs(LPSTR lpFileName, LPSTR lpOrgSafixs)
{
	int		len1, len2;
	char	szSafixs[QRECMAX];
	char	szCmpSafixs[QRECMAX];

	strcpy_s(szCmpSafixs, QRECMAX, lpOrgSafixs);
	len1 = strlen(lpFileName);
	len2 = strlen(szCmpSafixs);
	strcpy_s(szSafixs, QRECMAX, &(lpFileName[len1-len2]));
	_strupr_s(szCmpSafixs);
	_strupr_s(szSafixs);
	return(strncmp(szSafixs, szCmpSafixs, len2));
}

// basegetcode
UINT CCharLib::GetCharCode(LPSTR strg)
{
	UINT	hc, lc, code;

	hc = strg[0] & 0x00ff;
	if(hc == 0){
		code = 0;
	}
	lc = strg[1] & 0x00ff;
	if(lc != 0){
		code = hc << 8 | lc;
	}else{
		code = _mbbtombc(hc);
		if((0x00b6 <= hc && hc <= 0x00c4)
		|| (0x00ca <= hc && hc <= 0x00ce)){
			if(strg[1] == ' '){
				code += 1;
			}
			else if(0x00ca <= hc && hc <= 0x00ce){
				if(strg[1] == ' '){
					code += 2;
				}
			}
			else if(0x00b3 == hc && strg[1] == ' '){
				code = 0x8394;
			}
			else if(0x003c == hc){		// <
				code = 0x8171;
			}
			else if(0x003e == hc){		// >
				code = 0x8172;
			}
		}
	}
	return(code);
}

// basesjtoj
int CCharLib::SJisToJis(int far sjis)
{
	int		jis;
	int		hcode, lcode;
	int		dhcode, dlcode;

	hcode = (sjis >> 8) & 0x00ff;
	lcode = sjis & 0x00ff;

	if(hcode <= 0x9f)
		dhcode = hcode - 0x00e1;
	else
		dhcode = hcode - 0x0161;

	dhcode = hcode + dhcode;

	if(lcode <= 0x9e)
		if(lcode > 0x7f)
			dlcode = lcode - 0x20;
		else
			dlcode = lcode - 0x1f;
	else{
		dlcode = lcode - 0x7e;
		dhcode++;
	}
	jis = (dhcode << 8) | dlcode;
	return(jis);
}

// basejistosjis
int CCharLib::JisToSJis(int far jis)
{
	int		sjis;
	int		hcode, lcode;
	int		dhcode, dlcode;

	hcode = (jis >> 8) & 0x00ff;
	lcode = jis & 0x00ff;
	dhcode = (hcode-1) >> 1;
	if(hcode <= 0x5e){
		dhcode += 0x71;
	}
	else{
		dhcode += 0xb1;
	}
	if(hcode & 0x01){
		if(lcode < 0x60){
			dlcode = lcode + 0x1f;
		}
		else{
			dlcode = lcode + 0x20;
		}
	}
	else{
		dlcode = lcode + 0x7e;
	}
	sjis = (dhcode << 8) | dlcode;
	return(sjis);
}

// basesjtokuten
void CCharLib::SJisToKuten(int far sjis, int far *ku, int far *ten)
{
	int		hcode, lcode;
	int		dhcode, dlcode;

	hcode = (sjis >> 8) & 0x00ff;
	lcode = sjis & 0x00ff;

	if(hcode <= 0x9f)
		dhcode = hcode - 0x00e1;
	else
		dhcode = hcode - 0x0161;

	dhcode = hcode + dhcode;

	if(lcode <= 0x9e)
		if(lcode > 0x7f)
			dlcode = lcode - 0x20;
		else
			dlcode = lcode - 0x1f;
	else{
		dlcode = lcode - 0x7e;
		dhcode++;
	}
	*ku = dhcode - 0x20;
	*ten = dlcode - 0x20;
}

// basekutentojis
int CCharLib::KutenToJis(int kuten)
{
	int		ku, ten;
	int		hcode, lcode;
	int		jis;

	ku = kuten / 100;
	ten = kuten % 100;
	hcode = ku + 0x20;
	lcode = ten + 0x20;
	jis = ((hcode << 8) & 0x0000ff00) + (lcode & 0x000000ff);
	return(jis);
}

