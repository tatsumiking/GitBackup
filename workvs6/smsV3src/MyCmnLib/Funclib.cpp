// funclib.cpp : アプリケーション用環境定義
//

#include "stdafx.h"

#include "funclib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
}

CFuncLib::CFuncLib()
{
}

CFuncLib::~CFuncLib()
{
}

void CFuncLib::NameSetSapce(LPSTR name1, LPSTR name2, LPSTR name)
{
	int		num1, num2;

	num1 = strlen(name1);
	num2 = strlen(name2);
	if(num1 == 2){
		if(num2 == 2){
			sprintf(name, "%s  %s ", name1, name2);
		}
		else if(num2 == 4 || num2 == 6 || num2 == 8){
			sprintf(name, "%s %s", name1, name2);
		}
		else{
			sprintf(name, "%s%s", name1, name2);
		}
	}
	else if(num2 == 2){
		if(num1 == 4 || num1 == 6 || num1 == 8){
			sprintf(name, "%s %s", name1, name2);
		}
		else{
			sprintf(name, "%s%s", name1, name2);
		}
	}
	else{
		sprintf(name, "%s%s", name1, name2);
	}
}

void CFuncLib::CreateDateStr(LPSTR lpDateStr, int yyyy, int mm, int dd, int hh, int min)
{
	sprintf(lpDateStr, "%04d/%02d/%02d %02d:%02d", yyyy, mm, dd, hh, min);
}

void CFuncLib::ConverteDateStr(LPSTR lpDateStr, int *yyyy, int *mm, int *dd, int *hh, int *min)
{
	sscanf(lpDateStr, "%d/%d/%d %d:%d", yyyy, mm, dd, hh, min);
}

int CFuncLib::DivCsvData(LPSTR csvsp[], LPSTR csvbuf)
{
	int		i;
	UINT	hc;
	LPSTR	sp;
	int		nDoubleQ;

	sp = csvbuf;
	i = 0;
	while(1){
		while(*sp){
			hc = *sp;
			if(!isspace(hc)){
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
			hc = *sp;
			if(IsKanji(hc) == ON){
				sp += 2;
			}
			else{
				if(nDoubleQ == TRUE){
					if(hc == '\"'){
						*sp = '\0';
						sp++;
						while(*sp){
							hc = *sp;
							if(IsKanji(hc) == ON){
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
				}
				else if(hc == ','){
					*sp = '\0';
					sp++;
					break;
				}
				sp++;
			}
		}
		i++;
	}
	return(i);
}

int CFuncLib::IsKanji(int hc)
{
	hc = hc & 0x00ff;
	if((0x0080 <= hc && hc <= 0x009f)
	|| (0x00e0 <= hc && hc <= 0x00ff)){
		return(ON);
	}
	else{
		return(OFF);
	}
}

int CFuncLib::GetLastCharPos(char ch, LPSTR str)
{
	int		pos;
	int		idx;
	int		hc;

	pos = 0;
	idx = 0;
	while(1){
		hc = (str[idx]) & 0x00ff;
		if(hc == '\0'){
			break;
		}
		if(IsKanji(hc) == ON){
			idx += 2;
		}
		else{
			if(hc == ch){
				pos = idx;
			}
			idx++;
		}
	}
	return(pos);
}

void CFuncLib::GetCrtTime(int *hh, int *min, int *ss)
{
	time_t		timer;
	struct tm	*datetime;

	timer = time(NULL);
	datetime = localtime(&timer);
	*hh = datetime->tm_hour;
	*min = datetime->tm_min;
	*ss = datetime->tm_sec;
}

void CFuncLib::GetCrtDate(int *yy, int *mm, int *dd, int *ww)
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

void CFuncLib::GetCrtDateStr(LPSTR lpDateStr)
{
	time_t		timer;
	struct tm	*datetime;
	int		yy, mm, dd, hh, min;

	timer = time(NULL);
	datetime = localtime(&timer);
	yy = datetime->tm_year + 1900;
	mm = datetime->tm_mon + 1;
	dd = datetime->tm_mday;
	hh = datetime->tm_hour;
	min = datetime->tm_min;
	sprintf(lpDateStr, "%04d/%02d/%02d %02d:%02d", yy, mm, dd, hh, min);
}

BOOL CFuncLib::DelCRfgets(LPSTR lpBuf, int max, FILE *fp)
{
	int		len;
	LPSTR	ret;

	ret = fgets(lpBuf, max, fp);
	len = strlen(lpBuf);
	lpBuf[len-1] = '\0';
	if(ret == NULL){
		return(FALSE);
	}
	return(TRUE);
}

void CFuncLib::AddSafix(LPSTR fname, LPSTR safixs)
{
	int		len1;
	int		len2;

	len1 = strlen(fname);
	len2 = strlen(safixs);
	if(fname[len1-len2-1] != '.'){
		strcat(fname, ".");
		strcat(fname, safixs);
	}
}

void CFuncLib::ChangeSafix(LPSTR fname, LPSTR safixs)
{
	LPSTR	sp;
	UINT	hc;
	int		len, dot;

	sp = fname;
	len = 0;
	dot = 0;
	while(*sp){
		hc = *sp;
		if(IsKanji(hc) == ON){
			sp += 2;
			len += 2;
		}
		else{
			if(*sp == '.'){
				dot = len;
			}
			sp += 1;
			len += 1;
		}
	}
	if(dot == 0){
		fname[len] = '.';
		strcpy(&(fname[len+1]), safixs);
	}
	else{
		strcpy(&(fname[dot+1]), safixs);
	}
}
