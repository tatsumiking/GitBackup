//
// Easy Scanner System Ver1.0
//
// ess  baselib.c  低レベル共有関数
//
//  Copyright (c) 1998 by THEON

#include	<windows.h>

#pragma optimize( "", off )

#include	<stdio.h>
#include	<math.h>
#include	<mbstring.h>
#include 	<direct.h>
#include 	<io.h>
#include 	<time.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"baselib.fnc"

DllExport LPSTR baselibCrDelFgets(LPSTR buf, int max, FILE *fp)
{
	char	*ret;
	int		len;

	ret = fgets(buf, max, fp);
	if(ret != NULL){
		len = strlen(buf);
		if(buf[len-1] == 0x0a){
			if(buf[len-2] == 0x0d){
				if(buf[len-3] == 0x0d){
					buf[len-3] = '\0';
				}
				else{
					buf[len-2] = '\0';
				}
			}
			else{
				buf[len-1] = '\0';
			}
		}
		else if(buf[len-1] == 0x0d){
			buf[len-1] = '\0';
		}
	}
	return(ret);
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

DllExport int baselastspacedel(char *fontname)
{
	int		len;
	int		idx;

	len = strlen(fontname);
	for(idx = len-1; idx >= 0; idx--){
		if(fontname[idx] != ' '){
			fontname[idx+1] = '\0';
			break;
		}
	}
	return(0);
}

DllExport int basechpath(char far *path)
{
	char	msgbuf[HRECMAX];
	char	temppath[FNAMEMAX];
	int		drvno;

	if(path[1] == ':'){
		if(path[0] < 0x60){
			drvno = path[0] - 'A';
		}
		else{
			drvno = path[0] - 'a';
		}
		_chdrive(drvno);
	}
	SetCurrentDirectory(path);
	if(strlen(path) == 2){
		sprintf(temppath, "%s\\.", path);
		if(_access(temppath, 0) != 0){
			return(FAIL);
		}
	}
	else{
		strcpy(temppath, path);
		if(_access(temppath, 0) != 0){
			sprintf(msgbuf, "<%s>が存在しません", path);
			MessageBox(NULL, msgbuf, "ディレクトリエラー", MB_OK );
			return(FAIL);
		}
	}
	return(0);
}

DllExport LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR retstr)
{
	int		i;
	BYTE	hc;
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
		hc = (*sp++) & 0x00ff;
		if(baseiskanji(hc) == ON){
			retstr[i++] = hc;
			retstr[i++] = *sp++;
		}
		else{
			if(hc == '"'){
				if(nDoubleQ == FALSE){
					nDoubleQ = TRUE;
				}else{
					nDoubleQ = FALSE;
				}
			}else{
				if(nDoubleQ == FALSE){
					if(hc == ','){
						retstr[i] = '\0';
						break;
					}
				}
				if(hc != '\n'){
					retstr[i++] = hc;
				}
			}
		}
	}
	retstr[i] = '\0';
	return(sp);
}

DllExport int basedivcsvdata(LPSTR csvsp[], LPSTR csvbuf)
{
	int		i;
	UINT	hc;
	LPSTR	sp;
	int		nDoubleQ;

	sp = csvbuf;
	i = 0;
	while(1){
		while(*sp){
			hc = (*sp) & 0x00ff;
			if(baseiskanji(hc) == ON){
				break;
			}
			else if(hc != 0x20 && hc != 0x09){
				break;
			}
			sp++;
		}

		if(*sp == '\0'){
			break;
		}

		if(*sp == '\"'){
			nDoubleQ = TRUE;
			sp++;
		}
		else{
			nDoubleQ = FALSE;
		}

		csvsp[i] = sp;
		while(*sp){
			hc = (*sp) & 0x00ff;
			if(baseiskanji(hc) == ON){
				sp += 2;
			}
			else{
				if(nDoubleQ == TRUE){
					if(hc == '\"'){
						*sp = '\0';
						sp++;
						// 次のカンマまでスキップ
						while(*sp){
							hc = (*sp) & 0x00ff;
							if(baseiskanji(hc) == ON){
								sp += 2;
							}
							else{
								if(hc == ','){
									*sp = '\0';
									sp++;
									break;
								}
								sp++;
							}
						}
						break;
					}
					sp++;
				}
				else{
					if(hc == ','){
						*sp = '\0';
						sp++;
						break;
					}
					sp++;
				}
			}
		}
		i++;
	}
	return(i);
}

DllExport LPSTR basedivstrg(LPSTR sp, LPSTR retsp, char far sparator)
{
	int		hc;

	if(*sp == '\0'){
		*retsp = '\0';
		return(sp);
	}
	// 2001.10.18 立身 いちばん　途中に文字が入っていない枠があると以降処理されない
	if(*sp == sparator){
		*retsp = '\0';
		*sp++;
		return(sp);
	}
	while(1){
		if(*sp == sparator){
			*retsp = '\0';
			break;
		}
		hc = (*sp++) & 0x00ff;
		if(baseiskanji(hc) == ON){
			*retsp++ = hc;
			*retsp++ = *sp++;
		}
		else{
			*retsp++ = hc;
		}
		if(*sp == sparator){
			sp++;
			*retsp = '\0';
			break;
		}
		if(*sp == '\0'){
			*retsp = '\0';
			break;
		}
	}
	return(sp);
}


DllExport int basestrgcntget(char far *strg)
{
	int		idx, fldcnt, spflag;
	int		hc, lc;

	idx = 0;
	fldcnt = 0;
	spflag = ON;
	while(1){
		hc = strg[idx] & 0x00ff;
		lc = strg[idx+1] & 0x00ff;
		if(hc == '\0'){
			break;
		}
		if(spflag == OFF){
			if(hc == ' '){
				spflag = ON;
			}
			else if(hc == 0x81 && lc == 0x40){
				spflag = ON;
				idx++;
			}
		}
		else{
			if(hc == ' '){
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
	}
	return(fldcnt);
}

DllExport LPSTR basesptodnum(LPSTR tsp, DBL far *retnum)
{
	DBL		hnum, lnum, sign;
	LPBYTE	sp;

	sp = (LPBYTE)tsp;
	while(1){
		if(*sp > ' ' || *sp == '\0'){
			break;
		}
		sp++;
	}
	if(*sp == '\0'){
		*retnum = 0.0;
		return(sp);
	}
	hnum = 0.0;
	lnum = 0.1;
	sign = 1.0;
	if(*sp == '-'){
		sign = -1.0;
		*sp++;
	}
	if(*sp != '.'){
		if(isdigit(*sp) == 0){
			while(1){
				if(*sp <= ' '){
					break;
				}
				sp++;
			}
			*retnum = 0.0;
			return(sp);
		}
		while(1){
			hnum = hnum + ((*sp) - '0');
			*sp++;
			if(isdigit(*sp) == 0){
				break;
			}
			hnum *= 10.0;
		}
	}
	if(*sp == '.'){
		sp++;
		if(isdigit(*sp)){
			while(1){
				hnum = hnum + lnum * ((*sp) - '0');
				*sp++;
				if(isdigit(*sp) == 0){
					break;
				}
				lnum /= 10.0;
			}
		}
	}
	*retnum = hnum * sign;
	while(1){
		if(*sp <= ' '){
			break;
		}
		sp++;
	}
	return(sp);
}

DllExport LPSTR basesptoinum(LPSTR tsp, int far *retnum)
{
	int		hnum, sign;
	LPBYTE	sp;

	sp = (LPBYTE)tsp;
	while(1){
		if(*sp > ' ' || *sp == '\0'){
			break;
		}
		sp++;
	}
	if(*sp == '\0'){
		*retnum = 0;
		return(sp);
	}
	hnum = 0;
	sign = 1;
	if(*sp == '-'){
		sign = -1;
		*sp++;
	}
	if(isdigit(*sp) == 0){
		while(1){
			if(*sp <= ' '){
				break;
			}
			sp++;
		}
		*retnum = 0;
		return(sp);
	}
	while(1){
		hnum = hnum + ((*sp) - '0');
		*sp++;
		if(isdigit(*sp) == 0){
			break;
		}
		hnum *= 10;
	}
	*retnum = hnum * sign;
	while(1){
		if(*sp <= ' '){
			break;
		}
		sp++;
	}
	return(sp);
}

DllExport LPSTR basesptostrg(LPSTR tsp, LPSTR retsp)
{
	LPBYTE	sp;

	sp = (LPBYTE)tsp;
	while(1){
		if(*sp > ' ' || *sp == '\0'){
			break;
		}
		sp++;
	}
	if(*sp == '\0'){
		*retsp = '\0';
		return(sp);
	}
	while(1){
		*retsp++ = *sp++;
		if(*sp <= ' '){
			*retsp = '\0';
			break;
		}
	}
	return(sp);
}

DllExport void baseaddsafix(LPSTR FileName, LPSTR Safix)
{
	int		len1;
	int		len2;

	len1 = strlen(FileName);
	len2 = strlen(Safix);
	if(FileName[len1-len2-1] != '.'){
		strcat(FileName, ".");
		strcat(FileName, Safix);
	}
}

DllExport void basechangesafix(LPSTR FileName, LPSTR Safix)
{
	int		len1;
	int		len2;

	len1 = strlen(FileName);
	len2 = strlen(Safix);
	FileName[len1-len2] = '\0';
	strcat(FileName, Safix);
}

DllExport void basetimeget(int far *hh, int far *mm, int far *ss, int far *tss)
{
	time_t		timer;
	struct tm	*datetime;

	timer = time(NULL);
	datetime = localtime(&timer);
	*hh = datetime->tm_hour;
	*mm = datetime->tm_min;
	*ss = datetime->tm_sec;
	*ss = 0;
}

DllExport void basedateget(int far *yy, int far *mm, int far *dd, int far *ww)
{
	time_t	 	timer;
	struct tm 	*datetime;

	timer = time(NULL);
	datetime = localtime(&timer);
	*yy = datetime->tm_year + 1900;
	*mm = datetime->tm_mon + 1;
	*dd = datetime->tm_mday;
	*ww = datetime->tm_wday;
}

DllExport int basesetcodetbl(LPSTR strg, UINT code[])
{
	int		len, idx;
	UINT	hc, lc;
	int		cnt;

	len = strlen(strg);
	idx = 0;
	cnt = 0;
	while(1){
		if(idx >= len){
			break;
		}
		hc = strg[idx] & 0x00ff;
		if(hc == 0){
			break;
		}
		if(baseiskanji(hc) == ON){
			lc = strg[idx+1] & 0x00ff;
			code[cnt] = ((hc << 8) & 0xff00) | (lc & 0x00ff);
			idx += 2;
		}
		else{
			code[cnt] = hc;
			idx++;
		}
		cnt++;
	}
	return(cnt);
}

DllExport void baseupperstrg(LPSTR strg)
{
	int		len, idx, hc;

	len = strlen(strg);
	idx = 0;
	while(1){
		if(idx >= len){
			break;
		}
		hc = strg[idx] & 0x00ff;
		if(baseiskanji(hc) == ON){
			idx += 2;
		}
		else{
			strg[idx] = toupper(strg[idx]);
			idx += 1;
		}
	}
}

DllExport void basespacedelcpy(LPSTR dststrg, LPSTR srcstrg)
{
	int		hc, lc;
	int		i, j;

	i = 0;
	j = 0;
	while(1){
		hc = srcstrg[i] & 0x00ff;
		if(hc == '\0'){
			dststrg[j] = '\0';
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
			if(hc == ' '){
				i += 1;
			}
			else{
				dststrg[j] = srcstrg[i];
				i += 1;
				j += 1;
			}
		}
	}
}

DllExport void basehantozen(char dst[], char src[], int max)
{
	int		sp, dp;
	UINT	hc, code;

	sp = 0;
	dp = 0;
	while(1){
		if(max <= dp){
			dst[dp++] = '\0';
			break;
		}
		hc = src[sp++] & 0x00ff;
		if(hc == '\0'){
			dst[dp++] = '\0';
			break;
		}
		if(baseiskanji(hc) == ON){
			dst[dp++] = hc;
			dst[dp++] = src[sp++];
		}
		else{
			code = _mbbtombc(hc);
			if((0x00b6 <= hc && hc <= 0x00c4)
			|| (0x00ca <= hc && hc <= 0x00ce)){
				if(src[sp] == 'ﾞ'){
					sp++;
					code += 1;
				}
				else if(0x00ca <= hc && hc <= 0x00ce){
					if(src[sp] == 'ﾟ'){
						sp++;
						code += 2;
					}
				}
			}
			else if(0x00b3 == hc && src[sp] == 'ﾞ'){
				sp++;
				code = 0x8394;
			}
			else if(0x003c == hc){		// <
				code = 0x8171;
			}
			else if(0x003e == hc){		// >
				code = 0x8172;
			}
			dst[dp++] = (code >> 8) & 0x00ff;
			dst[dp++] = (code) & 0x00ff;
		}
	}
}

DllExport int basegetparentpath(LPSTR lpPath)
{
	LPSTR	sp;
	LPSTR	lpLast;
	int		idx, lastidx;
	int		len;
	UINT	hc;

	lastidx = 0;
	sp = lpPath;
	len = strlen(lpPath);
	if(len <= 2 && lpPath[1] == ':'){
		return(2);
	}
	if(lpPath[1] == ':'){
		lpLast = &(lpPath[2]);
		lastidx = 2;
	}
	idx = 0;
	while(1){
		if((*sp) == '\0'){
			break;
		}
		hc = (*sp) & 0x00ff;
		if(baseiskanji(hc) == ON){
			sp += 2;
			idx += 2;
		}
		else{
			if(*sp == '\\'){
				lpLast = sp;
				lastidx = idx;
			}
			sp++;
			idx++;
		}
	}
	*lpLast = '\0';
	return(lastidx);
}

DllExport int baseiskanji(UINT far hc)
{
	if((0x0080 <= hc && hc <= 0x009f)
	|| (0x00e0 <= hc && hc <= 0x00ff)){
		return(ON);
	}
	else{
		return(OFF);
	}
}

DllExport int basesjtoj(int far sjis)
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

DllExport int basejtosj(int far jis)
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

DllExport void basesjtokuten(int far sjis, int far *ku, int far *ten)
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


DllExport void basejtokuten(int far jis, int far *ku, int far *ten)
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

DllExport void  Dispatch()
{
	MSG	msg;

	while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )){
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

