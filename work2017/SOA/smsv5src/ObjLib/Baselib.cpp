//
// Easy Scanner System Ver1.0
//
// ess  baselib.c  ’áƒŒƒxƒ‹‹¤—LŠÖ”
//
//  Copyright (c) 1998 by THEON

#include	"stdafx.h"
#include	<time.h>
#include	"BaseLib.h"

DllExport LPTSTR baselibCrDelFgets(LPTSTR buf, int max, FILE *fp)
{
	char	szBuf[RECMAX];
	LPSTR	lpRet;
	int		len;

	lpRet = fgets(szBuf, RECMAX, fp);
	if (lpRet == NULL) {
		return(NULL);
	}
	len = strlen(szBuf);
	if (szBuf[len - 1] == 0x0a) {
		if (szBuf[len - 2] == 0x0d) {
			if (szBuf[len - 3] == 0x0d) {
				szBuf[len - 3] = '\0';
			}
			else {
				szBuf[len - 2] = '\0';
			}
		}
		else {
			szBuf[len - 1] = '\0';
		}
	}
	else if (szBuf[len - 1] == 0x0d) {
		szBuf[len - 1] = '\0';
	}
	baselibCharStrToTCHARStr(szBuf, buf, max);
	return(buf);
}
DllExport int baselibFputs(LPTSTR buf, FILE *fp)
{
	char	szBuf[RECMAX];
	int		ret;

	baselibTCHARStrToCharStr(buf, szBuf, RECMAX);
	ret = fputs(szBuf, fp);
	return(ret);
}
DllExport int baselibTCHARStrToCharStr(TCHAR *lpSrcStr, char *lpDstStr, int nBufMax)
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
DllExport int baselibCharStrToTCHARStr(char *lpSrcStr, TCHAR *lpDstStr, int nBufMax)
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
DllExport void basesleep(int imax, int jmax)
{
	int i, j;

	for(i = 0; i < imax; i++){
		for(j = 0; j < jmax; j++){
			;
		}
	}
}

DllExport int baselastspacedel(LPTSTR fontname)
{
	int		len;
	int		idx;

	len = _tcslen(fontname);
	for(idx = len-1; idx >= 0; idx--){
		if(fontname[idx] != _T(' ')){
			fontname[idx+1] = _T('\0');
			break;
		}
	}
	return(0);
}

DllExport int basechpath(LPTSTR path)
{
	TCHAR	msgbuf[HRECMAX];
	TCHAR	temppath[FNAMEMAX];

	SetCurrentDirectory(path);
	if(_tcslen(path) == 2){
		_stprintf_s(temppath, _T("%s\\."), path);
		if(_taccess_s(temppath, 0) != 0){
			return(FAIL);
		}
	}
	else{
		_tcscpy_s(temppath, FNAMEMAX, path);
		if(_taccess_s(temppath, 0) != 0){
			_stprintf_s(msgbuf, _T("<%s>‚ª‘¶İ‚µ‚Ü‚¹‚ñ"), path);
			MessageBox(NULL, msgbuf, _T("ƒfƒBƒŒƒNƒgƒŠƒGƒ‰["), MB_OK );
			return(FAIL);
		}
	}
	return(0);
}

DllExport LPTSTR basecsvstrdivcpy(LPTSTR sp, LPTSTR retstr)
{
	int		i;
	int		nDoubleQ;

	i = 0;
	nDoubleQ = FALSE;
	while(*sp){
		if(!isspace(*sp)){
			break;
		}
		sp++;
	}
	while(*sp){
#ifdef UNICODE
#else
		BYTE	hc;
		hc = (*sp) & 0x00ff;
		if(baseiskanji(hc) == ON){
			retstr[i++] = hc;
			sp++;
			retstr[i++] = *sp++;
			continue;
		}
#endif
		if(*sp == _T('"')){
			if(nDoubleQ == FALSE){
				nDoubleQ = TRUE;
			}else{
				nDoubleQ = FALSE;
			}
		}
		else if (nDoubleQ == FALSE) {
			if (*sp == _T(',')) {
				sp++;
				retstr[i] = _T('\0');
				break;
			}
			if (*sp != _T('\n')) {
				retstr[i++] = *sp;
			}
		}
		else {
			if (*sp == _T('\\')) {
				retstr[i++] = *sp++;
				if ((_istdigit(*sp) && _istdigit(*(sp + 1))) || *sp == _T('x')) { // \377 \xff
					retstr[i++] = *sp++;
					retstr[i++] = *sp++;
					retstr[i++] = *sp;
				}
				else {
					retstr[i++] = *sp;
				}
			}
			else {
				retstr[i++] = *sp;
			}
		}
		sp++;
	}
	retstr[i] = _T('\0');
	return(sp);
}
DllExport int basedivcsvdata(LPTSTR csvsp[], LPTSTR csvbuf)
{
	int		i;
	LPTSTR	sp;

	sp = csvbuf;
	i = 0;
	while (*sp) {
		csvsp[i] = sp;
		sp = basecsvstrdiv(sp);
		csvsp[i] = basedeltopspace(csvsp[i]);
		basedellastspace(csvsp[i]);
		csvsp[i] = basedeldoublequote(csvsp[i]);
		i++;

	}
	return(i);
}
DllExport LPTSTR basecsvstrdiv(LPTSTR sp)
{
	int		nDoubleQ;

	nDoubleQ = FALSE;
	while (*sp) {
#ifdef UNICODE
#else
		BYTE	hc;
		hc = (*sp) & 0x00ff;
		if (baseiskanji(hc) == ON) {
			sp++;
			sp++;
			continue;
		}
#endif
		if (*sp == _T('"')) {
			if (nDoubleQ == FALSE) {
				nDoubleQ = TRUE;
			}
			else {
				nDoubleQ = FALSE;
			}
			sp++;
		}
		else if (nDoubleQ == FALSE) {
			if (!_istspace(*sp)) {
				if (*sp == _T(',')) {
					*sp = _T('\0');
					sp++;
					break;
				}
				if (*sp != _T('\n')) {
					*sp = _T('\0');
					sp++;
					break;
				}
			}
		}
		else {
			if (*sp == _T('\\')) {
				sp++;
				if ((_istdigit(*sp) && _istdigit(*(sp + 1))) || *sp == _T('x')) { // \377 \xff
					sp++;
					sp++;
					sp++;
				}
				else { // \a \b \n \r \f \t \v \\ \? \' \" \0
					sp++;
				}
			}
			else {
				sp++;
			}
		}
	}
	return(sp);
}
DllExport LPTSTR basedeltopspace(LPTSTR sp)
{
	while (*sp) {
		if (!_istspace(*sp)) {
			break;
		}
		sp++;
	}
	return(sp);
}
DllExport void basedellastspace(LPTSTR sp)
{
	int len, idx;

	len = _tcslen(sp);
	for (idx = len - 1; idx <= 0; idx--) {
		if (!_istspace(sp[idx])) {
			sp[idx] = _T('\0');
		}
		else {
			break;
		}
	}
}
DllExport LPTSTR basedeldoublequote(LPTSTR sp)
{
	int len;

	if (*sp == _T('"')) {
		sp++;
		len = _tcslen(sp);
		if (*(sp + len - 1) == _T('"')) {
			*(sp + len - 1) = _T('\0');
		}
	}
	return(sp);
}
DllExport LPTSTR basedivstrg(LPTSTR sp, LPTSTR retsp, TCHAR sparator)
{
	while(1){
		if (*sp == _T('\0')) {
			*retsp = _T('\0');
			break;
		}
		if (*sp == sparator) {
			*retsp = _T('\0');
			*sp++;
			break;
		}
#ifdef UNICODE
		*retsp++ = *sp++;
#else
		int		hc;
		hc = (*sp++) & 0x00ff;
		if (baseiskanji(hc) == ON) {
			*retsp++ = hc;
			*retsp++ = *sp++;
		}
		else {
			*retsp++ = hc;
		}
#endif
	}
	return(sp);
}
DllExport int basestrgcntget(LPTSTR strg)
{
	int		idx, fldcnt, spflag;

	idx = 0;
	fldcnt = 0;
	spflag = ON;
	while(1){
#ifdef UNICODE
		if (strg[idx] == _T('\0')) {
			break;
		}
		if (spflag == OFF) {
			if (_istspace(strg[idx])) {
				spflag = ON;
			}
		}
		else {
			if (!_istspace(strg[idx])) {
				fldcnt++;
				spflag = OFF;
			}
		}
		idx++;
#else
		int		hc, lc;
		hc = strg[idx] & 0x00ff;
		lc = strg[idx+1] & 0x00ff;
		if(hc == _T('\0')){
			break;
		}
		if(spflag == OFF){
			if(hc == _T(' ')){
				spflag = ON;
			}
			else if(hc == 0x81 && lc == 0x40){
				spflag = ON;
				idx++;
			}
		}
		else{
			if(hc == _T(' ')){
			}
			else if(hc == 0x81 && lc == 0x40){
				idx++;
			}
			else{
				fldcnt++;
				spflag = OFF;
			}
		}
		idx++;
#endif
	}
	return(fldcnt);
}

DllExport LPTSTR basesptodnum(LPTSTR tsp, DBL *retnum)
{
	DBL		hnum, lnum, sign;
	LPTSTR	sp;

	sp = (LPTSTR)tsp;
	while(1){
		if(*sp > _T(' ') || *sp == _T('\0')){
			break;
		}
		sp++;
	}
	if(*sp == _T('\0')){
		*retnum = 0.0;
		return(sp);
	}
	hnum = 0.0;
	lnum = 0.1;
	sign = 1.0;
	if(*sp == _T('-')){
		sign = -1.0;
		*sp++;
	}
	if(*sp != _T('.')){
		if(!_istdigit(*sp)){
			while(1){
				if(*sp <= _T(' ')){
					break;
				}
				sp++;
			}
			*retnum = 0.0;
			return(sp);
		}
		while(1){
			hnum = hnum + ((*sp) - _T('0'));
			*sp++;
			if(!_istdigit(*sp)){
				break;
			}
			hnum *= 10.0;
		}
	}
	if(*sp == _T('.')){
		sp++;
		if(_istdigit(*sp)){
			while(1){
				hnum = hnum + lnum * ((*sp) - _T('0'));
				*sp++;
				if(!_istdigit(*sp)){
					break;
				}
				lnum /= 10.0;
			}
		}
	}
	*retnum = hnum * sign;
	while(1){
		if(*sp <= _T(' ')){
			break;
		}
		sp++;
	}
	return(sp);
}

DllExport LPTSTR basesptoinum(LPTSTR tsp, int *retnum)
{
	int		hnum, sign;
	LPTSTR	sp;

	sp = (LPTSTR)tsp;
	while(1){
		if(*sp > _T(' ') || *sp == _T('\0')){
			break;
		}
		sp++;
	}
	if(*sp == _T('\0')){
		*retnum = 0;
		return(sp);
	}
	hnum = 0;
	sign = 1;
	if(*sp == _T('-')){
		sign = -1;
		*sp++;
	}
	if(!_istdigit(*sp)){
		while(1){
			if(*sp <= _T(' ')){
				break;
			}
			sp++;
		}
		*retnum = 0;
		return(sp);
	}
	while(1){
		hnum = hnum + ((*sp) - _T('0'));
		*sp++;
		if(!_istdigit(*sp)){
			break;
		}
		hnum *= 10;
	}
	*retnum = hnum * sign;
	while(1){
		if(*sp <= _T(' ')){
			break;
		}
		sp++;
	}
	return(sp);
}

DllExport LPTSTR basesptostrg(LPTSTR tsp, LPTSTR retsp)
{
	LPTSTR	sp;

	sp = (LPTSTR)tsp;
	while(1){
		if(*sp > _T(' ') || *sp == _T('\0')){
			break;
		}
		sp++;
	}
	if(*sp == _T('\0')){
		*retsp = _T('\0');
		return(sp);
	}
	while(1){
		*retsp++ = *sp++;
		if(*sp <= _T(' ')){
			*retsp = _T('\0');
			break;
		}
	}
	return(sp);
}

DllExport void baseaddsafix(LPTSTR lpFileName, LPTSTR lpSafix)
{
	int		len1;
	int		len2;

	len1 = _tcslen(lpFileName);
	len2 = _tcslen(lpSafix);
	if(lpFileName[len1-len2-1] != _T('.')){
		_tcscat_s(lpFileName, FNAMEMAX, _T("."));
		_tcscat_s(lpFileName, FNAMEMAX, lpSafix);
	}
}

DllExport void basechangesafix(LPTSTR lpFileName, LPTSTR lpSafix)
{
	int		len1;
	int		len2;

	len1 = _tcslen(lpFileName);
	len2 = _tcslen(lpSafix);
	lpFileName[len1-len2] = _T('\0');
	_tcscat_s(lpFileName, FNAMEMAX, lpSafix);
}

DllExport LPTSTR basegetsafix(LPTSTR lpFileName)
{
	int		len;
	TCHAR	szSafix[QRECMAX];

	len = _tcslen(lpFileName);
	if (lpFileName[len - 5] == _T('.')) {
		// case jpeg
		_tcscpy_s(szSafix, &(lpFileName[len - 4]));
	}
	else if (lpFileName[len - 4] == _T('.')) {
		_tcscpy_s(szSafix, &(lpFileName[len - 3]));
	}
	else if (lpFileName[len - 3] == _T('.')) {
		_tcscpy_s(szSafix, &(lpFileName[len - 2]));
	}
	else if (lpFileName[len - 2] == _T('.')) {
		_tcscpy_s(szSafix, &(lpFileName[len - 1]));
	}
	else {
		szSafix[0] = _T('\0');
	}
	return(szSafix);
}

DllExport void basetimeget(int *hh, int *mm, int *ss, int *tss)
{
	time_t		timer;
	struct tm	datetime;

	timer = time(NULL);
	localtime_s(&datetime, &timer);
	*hh = datetime.tm_hour;
	*mm = datetime.tm_min;
	*ss = datetime.tm_sec;
	*ss = 0;
}

DllExport void basedateget(int *yy, int *mm, int *dd, int *ww)
{
	time_t	 	timer;
	struct tm 	datetime;

	timer = time(NULL);
	localtime_s(&datetime, &timer);
	*yy = datetime.tm_year + 1900;
	*mm = datetime.tm_mon + 1;
	*dd = datetime.tm_mday;
	*ww = datetime.tm_wday;
}

DllExport int basesetcodetbl(LPTSTR strg, int code[])
{
	int		len, idx;
	int		hc, lc;
	int		cnt;

	len = _tcslen(strg);
	idx = 0;
	cnt = 0;
	while(1){
		if(idx >= len){
			break;
		}
		if (strg[idx] == _T('\n')) {
			break;
		}
#ifdef UNICODE
		code[cnt] = (strg[idx]) & 0x0000ffff;
		cnt++;
		idx++;
#else
		hc = strg[idx] & 0x00ff;
		if(baseiskanji(hc) == ON){
			lc = strg[idx+1] & 0x00ff;
			code[cnt] = (((hc << 8) & 0xff00) | (lc & 0x00ff)) & 0x0000ffff;
			idx += 2;
		}
		else{
			code[cnt] = hc;
			idx++;
		}
		cnt++;
#endif
	}
	return(cnt);
}

DllExport void baseupperstrg(LPTSTR strg)
{
	int		len, idx;
	UINT	code;

	len = _tcslen(strg);
	idx = 0;
	while(1){
		if(idx >= len){
			break;
		}
		if (strg[idx] == _T('\n')) {
			break;
		}
#ifdef UNICODE
		code = (strg[idx]) & 0x0000ffff;
		if (code < 0x0100) {
			strg[idx] = _toupper(strg[idx]);
		}
		idx++;
#else
		hc = strg[idx] & 0x00ff;
		if(baseiskanji(hc) == ON){
			idx += 2;
		}
		else{
			strg[idx] = _toupper(strg[idx]);
			idx += 1;
		}
#endif
	}
}

DllExport void basespacedelcpy(LPTSTR dststrg, LPTSTR srcstrg)
{
	int		hc, lc;
	int		i, j;

	i = 0;
	j = 0;
	while(1){
		if (srcstrg[i] == _T('\n')) {
			dststrg[j] = _T('\0');
			break;
		}
#ifdef UNICODE
		if (srcstrg[i] != _T(' ') && srcstrg[i] != _T('@')) {
			dststrg[j] = srcstrg[i];
			j++;
		}
		i++;
#else
		hc = srcstrg[i] & 0x00ff;
		if(hc == _T('\0')){
			dststrg[j] = _T('\0');
			break;
		}
		if(baseiskanji(hc) == ON){
			lc = srcstrg[i] & 0x00ff;
			if(hc == 0x0080 && lc == 0x0040){
				i += 2;
			}
			else{
				dststrg[j] = srcstrg[i];
				dststrg[j+1] = srcstrg[i+1];
				i += 2;
				j += 2;
			}
		}
		else{
			if(hc == _T(' ')){
				i += 1;
			}
			else{
				dststrg[j] = srcstrg[i];
				i += 1;
				j += 1;
			}
		}
#endif
	}
}
DllExport int baseCodeHanToZen(int code)
{
	WCHAR wSrcCode;
	WCHAR wDstCode;

	wSrcCode = (WCHAR)code;
	wDstCode = baseuniasciihantozen(wSrcCode);
	if (wDstCode == wSrcCode) { // •ÏŠ·‚³‚ê‚È‚©‚Á‚½‚Æ‚«
		wDstCode = baseunikanahantozen(wSrcCode);
	}
	return(wDstCode);

}
DllExport void basehantozen(LPTSTR lpDstStr, LPTSTR lpSrcStr, int max)
{
#ifdef UNICODE
	LPWSTR lpStr;
	lpStr = baseunihantozen(lpSrcStr);
	wcscpy_s(lpDstStr, max, lpSrcStr);
#else
	LPSTR lpStr;
	lpStr = basesjishantozen(lpSrcStr);
	strcpy(lpDstStr, lpSrcStr);
#endif
}
DllExport int basegetparentpath(LPTSTR lpPath)
{
	LPTSTR	sp;
	LPTSTR	lpLast;
	int		idx, lastidx;
	int		len;
	int		hc;

	lastidx = 0;
	sp = lpPath;
	len = _tcslen(lpPath);
	if(len <= 2 && lpPath[1] == _T(':')){
		return(2);
	}
	if(lpPath[1] == _T(':')){
		lpLast = &(lpPath[2]);
		lastidx = 2;
	}
	idx = 0;
	while(1){
		if((*sp) == _T('\0')){
			break;
		}
		hc = (*sp) & 0x00ff;
		if(baseiskanji(hc) == ON){
			sp += 2;
			idx += 2;
		}
		else{
			if(*sp == _T('\\')){
				lpLast = sp;
				lastidx = idx;
			}
			sp++;
			idx++;
		}
	}
	*lpLast = _T('\0');
	return(lastidx);
}
DllExport void baselibSpaceDelete(LPTSTR lpStr)
{
	int len, i, j;
	TCHAR sStr[RECMAX];
	int hc;

	len = _tcslen(lpStr);
	i = 0; j = 0;
	while (true) {
		if (lpStr[i] == _T('\0')) {
			sStr[j] = _T('\0');
			break;
		}
#ifdef UNICODE
		if (lpStr[i] != _T(' ') && lpStr[i] != _T('@')) {
			sStr[j] = lpStr[i];
			j++;
		}
		i++;
#else
		hc = lpStr[i] & 0x00ff;
		if (baseiskanji(hc) == ON) {
			if (!(lpStr[i] == 0x81 && lpStr[i] == 0x40)) {
				sStr[j] = lpStr[i];
				sStr[j] = lpStr[i];
				j = j + 2;
			}
			i = i + 2;
		}
		else {
			if (!(lpStr[i] == 0x20)) {
				sStr[j] = lpStr[i];
				j++;
			}
			i++;
		}
#endif
	}
	len = _tcslen(sStr) + 1;
	_tcscpy_s(lpStr, len, sStr);
}
DllExport int baseiskanji(int hc)
{
#ifdef UNICODE
	if (hc < 0x0000007e) {
		return(OFF);
	}
	else if (0x0000ff61 <= hc && hc <= 0x0000ff9f) {
		return(OFF);
	}
	else {
		return(ON);
	}
#else
	if ((0x0080 <= hc && hc <= 0x009f)
		|| (0x00e0 <= hc && hc <= 0x00ff)) {
		return(ON);
	}
	else {
		return(OFF);
	}
#endif
}
DllExport int baselibU16ToSjis(int utf8)
{
	WCHAR	wStr[2];
	char	sStr[4];
	int		len;
	int		sjis;

	wStr[0] = (WCHAR)utf8;
	wStr[1] = 0;
	len = WideCharToMultiByte(CP_ACP, 0, wStr, -1, sStr, 4, NULL, NULL);
	sjis = (((sStr[0] << 8) & 0x0000ff00) | (sStr[1] & 0x000000ff)) & 0x0000ffff;
	return(sjis);
}
DllExport int baselibSjisToU16(int sjis)
{
	char	sStr[4];
	WCHAR	wStr[2];
	int		len;
	int		utf8;

	sStr[0] = ((sjis & 0x00ff00) >> 8);
	sStr[1] = sjis & 0x0000ff;
	sStr[2] = 0;
	len = MultiByteToWideChar(CP_ACP, 0, sStr, -1, wStr, 2);
	utf8 = wStr[0] & 0x00ffff;
	return(utf8);
}
DllExport int basesjtoj(int sjis)
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
DllExport int basejtosj(int  jis)
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

DllExport void basesjtokuten(int sjis, int *ku, int *ten)
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
DllExport void basejtokuten(int jis, int *ku, int *ten)
{
	int		hcode, lcode;

	hcode = (jis >> 8) & 0x00ff;
	lcode = jis & 0x00ff;
	*ku = hcode - 0x20;
	*ten = lcode - 0x20;
}
DllExport int basekutentojis(int kuten)
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
DllExport void baseDispatch()
{
	MSG	msg;

	while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )){
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

LPSTR basesjishantozen(LPSTR lpSrcStr)
{
	int max, idx;
	char szDstStr[RECMAX];
	int	 hc;
	char szStr[3];
	LPSTR lpStr;

	szDstStr[0] = '\0';
	idx = 0;
	max = strlen(lpSrcStr);
	while (true) {
		if (lpSrcStr[idx] == '\0') {
			break;
		}
		if (max <= idx) {
			break;
		}
		hc = lpSrcStr[idx];
		if (baseiskanji(hc) == ON) {
			szStr[0] = lpSrcStr[idx++];
			szStr[1] = lpSrcStr[idx++];
			szStr[2] = '\0';
		}
		else {
			lpStr = basesjisasciihantozen(lpSrcStr[idx], szStr);
			if (lpStr == NULL) {
				lpStr = basesjiskanahantozen(lpSrcStr[idx], szStr);
			}
			if (lpStr == NULL) {
				szStr[0] = lpSrcStr[idx++];
				szStr[1] = '\0';
			}
		}
		strcat_s(szDstStr, RECMAX, szStr);
	}
	strcpy_s(lpSrcStr, RECMAX, szDstStr);
	return(lpSrcStr);
}
const char sjisasciihan[] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}";
const char sjisasciizen[] = "IúW”“•úVij–{C|D^‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚XFGƒ„H—‚`‚a‚b‚c‚d‚e‚f‚g‚h‚i‚j‚k‚l‚m‚n‚o‚p‚q‚r‚s‚t‚u‚v‚w‚x‚ym_nOQM‚‚‚‚ƒ‚„‚…‚†‚‡‚ˆ‚‰‚Š‚‹‚Œ‚‚‚‚‚‘‚’‚“‚”‚•‚–‚—‚˜‚™‚šobp";
LPSTR basesjisasciihantozen(char ch, LPSTR lpZen)
{
	int max, idx;

	max = strlen(sjisasciihan);
	for (idx = 0; idx < max; idx++) {
		if (ch == sjisasciihan[idx]) {
			lpZen[0] = sjisasciizen[idx * 2];
			lpZen[1] = sjisasciizen[idx * 2 + 1];
			lpZen[2] = '\0';
			return(lpZen);
		}
	}
	return(NULL);
}

const char sjiskanahan[] = "¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß";
const char sjiskanazen[] = "BuvAEƒ’ƒ@ƒBƒDƒFƒHƒƒƒ…ƒ‡ƒb[ƒAƒCƒEƒGƒIƒJƒLƒNƒPƒRƒTƒVƒXƒZƒ\ƒ^ƒ`ƒcƒeƒgƒiƒjƒkƒlƒmƒnƒqƒtƒwƒzƒ}ƒ~ƒ€ƒƒ‚ƒ„ƒ†ƒˆƒ‰ƒŠƒ‹ƒŒƒƒƒ“JK";
LPSTR basesjiskanahantozen(char ch, LPSTR lpZen)
{
	int max, idx;

	max = strlen(sjiskanahan);
	for (idx = 0; idx < max; idx++) {
		if (ch == sjiskanahan[idx]) {
			lpZen[0] = sjiskanazen[idx * 2];
			lpZen[1] = sjiskanazen[idx * 2 + 1];
			lpZen[2] = '\0';
			return(lpZen);
		}
	}
	return(NULL);
}

LPWSTR baseunihantozen(LPWSTR lpSrcStr)
{
	int max, idx;
	WCHAR ch;
	int	code;

	max = wcslen(lpSrcStr);
	for (idx = 0; idx < max; idx++) {
		code = lpSrcStr[idx] & 0x0000ffff;
		if (baseiskanji(code) == OFF) {
			ch = baseuniasciihantozen(lpSrcStr[idx]);
			if (ch == lpSrcStr[idx]) { // •ÏŠ·‚³‚ê‚È‚©‚Á‚½‚Æ‚«
				ch = baseunikanahantozen(lpSrcStr[idx]);
			}
			lpSrcStr[idx] = ch;
		}
	}
	return(lpSrcStr);
}

const WCHAR uniasciihan[] = L"!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}";
const WCHAR uniasciizen[] = L"IúW”“•úVij–{C|D^‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚XFGƒ„H—‚`‚a‚b‚c‚d‚e‚f‚g‚h‚i‚j‚k‚l‚m‚n‚o‚p‚q‚r‚s‚t‚u‚v‚w‚x‚ym_nOQM‚‚‚‚ƒ‚„‚…‚†‚‡‚ˆ‚‰‚Š‚‹‚Œ‚‚‚‚‚‘‚’‚“‚”‚•‚–‚—‚˜‚™‚šobp";
WCHAR baseuniasciihantozen(WCHAR ch)
{
	int max, idx;

	max = wcslen(uniasciihan);
	for (idx = 0; idx < max; idx++) {
		if (ch == uniasciihan[idx]) {
			ch = uniasciizen[idx];
			return(ch);
		}
	}
	if (ch == L'~') {
		ch = L'`';
	}
	return(ch);
}

const WCHAR unikanahan[] = L"¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß";
const WCHAR unikanazen[] = L"BuvAEƒ’ƒ@ƒBƒDƒFƒHƒƒƒ…ƒ‡ƒb[ƒAƒCƒEƒGƒIƒJƒLƒNƒPƒRƒTƒVƒXƒZƒ\ƒ^ƒ`ƒcƒeƒgƒiƒjƒkƒlƒmƒnƒqƒtƒwƒzƒ}ƒ~ƒ€ƒƒ‚ƒ„ƒ†ƒˆƒ‰ƒŠƒ‹ƒŒƒƒƒ“JK";
WCHAR baseunikanahantozen(WCHAR ch)
{
	int max, idx;

	max = wcslen(unikanahan);
	for (idx = 0; idx < max; idx++) {
		if (ch == unikanahan[idx]) {
			ch = unikanazen[idx];
			return(ch);
		}
	}
	return(ch);
}

