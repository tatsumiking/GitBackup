
/////////////////////////////////////////////////////////////////////////////
// CFileEps

#include "stdafx.h"
#include "stdio.h"
#include "math.h"
#include "ctype.h"

#include "fileeps.h"

#define	RENGE(a, b, c)	if((a) < (b)){(a) = (b);} else if((c) < (a)){(a) = (c);}

extern "C" {
	int basestrgcntget(char far *strg);
	int senvgetpenno(int rval, int gval, int bval);
	int senvgetcrtfillpenno();
	int senvgetcrtlinepenno();
	DBL senvgetcrtlinethick();
	void senvsetcrtfillpenno(int penno);
	void senvsetcrtlinepenno(int penno);
	void senvsetcrtlinethick(DBL thick);
	void senvgetexepath(LPSTR path);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	void TrnsPointToMMXY(LPDBL crtx, LPDBL crty);
	void epsfmmget(LPDBL len);
	int epsffileopen(char *filename);
	int epsfstartfile(char *fname, DBL maxx, DBL maxy);
	int epsfendfile();
	int epsffileclose();
}

CFileEps::CFileEps(CView* pcview)
{
	m_pcView = (CTonView*)pcview;
	m_pcDoc = (CTonDoc*)(m_pcView->GetDocument());
}

void CFileEps::SaveData(LPSTR filename)
{
	DBL		mmxsize, mmysize;

	epsffileopen(filename);
	scrngetmmsize(&mmxsize, &mmysize);
	epsfstartfile(filename, mmxsize, mmysize);
	
	CTypedPtrList<CObList,CData*> *pcObjDataList = m_pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pcData = pcObjDataList->GetNext(pos);
		pcData->SaveEpsFile();
	}

	epsfendfile();

	epsffileclose();
}

void CFileEps::LoadData(LPSTR filename)
{
	FILE	*fp;
	int		backfillpenno;
	int		backlinepenno;
	DBL		backlinethick;

	backfillpenno = senvgetcrtfillpenno();
	backlinepenno = senvgetcrtlinepenno();
	backlinethick = senvgetcrtlinethick();
	m_nFillPenno = 0;
	m_nOutLinePenno = 0;
	m_dOutLineThick = 0;
	m_nTateYoko = 0;
	m_dLXS = 0;
	m_dLYS = 0;
	m_nPassFlag = OFF;

	if((fp = fopen(filename, "rb")) == NULL){
		m_pcDataGrup = NULL;
		return;
	}
	m_pcDataGrup = (CDataGrup*)(m_pcDoc->NewData(DATAGRUPID));
	senvsetcrtfillpenno(m_nFillPenno);
	senvsetcrtlinepenno(m_nOutLinePenno);
	senvsetcrtlinethick(m_dOutLineThick);
	SkipEpsHead(fp);

	EpsLoadLoop(fp);

	fclose(fp);

	m_pcDoc->DataAddTail((CData*)m_pcDataGrup);
	senvsetcrtfillpenno(backfillpenno);
	senvsetcrtlinepenno(backlinepenno);
	senvsetcrtlinethick(backlinethick);
}

void CFileEps::GrupReset()
{
	CData	*pcData;
	CData	*pcpreData;

	if(m_pcDataGrup == NULL){
		return;
	}
	pcData = m_pcDataGrup->m_pcDataTop;
	while(pcData != NULL){
		m_pcDoc->DataAddTail(pcData);
		pcpreData = pcData;
		pcData = pcpreData->GetNext();
		pcpreData->SetNext(NULL);
	}
	m_pcDataGrup->m_pcDataTop = NULL;
	m_pcDoc->DeleteData(m_pcDataGrup);
	delete(m_pcDataGrup);
}

void CFileEps::SkipEpsHead(FILE *fp)
{
	char	buffer[RECMAX2];

	while(1){
		if(CrDelFgets(buffer, RECMAX, fp) == FALSE){
			break;
		}
		if(strncmp(buffer, "%%EndSetup", 10) == 0){
			break;
		}
	}
}

void CFileEps::EpsLoadLoop(FILE * fp)
{
	char	buffer[RECMAX];

	m_pcVect = NULL;
	m_pcSPtnVect = NULL;
	while(1){
		if(CrDelFgets(buffer, RECMAX, fp) == FALSE){
			break;
		}
		if(strncmp(buffer, "%%Trailer", 9) == 0){
			break;
		}
		else if(strncmp(buffer, "%AI5_BeginRaster", 16) == 0){
			EpsBmpCreate(fp);
		}
		else{
			EpsCommandLoop(buffer);
		}
	}
	if(m_pcVect != NULL){
		if(m_nPP == 0){
			delete(m_pcVect);
		}
		else{
			SetVectData(m_pcVect, m_nPP);
		}
	}
}

void CFileEps::EpsCommandLoop(LPSTR buffer)
// 行の最後の文字でコマンド解析
{
	LPSTR	sp;
	char	cmdstr[RECMAX];
	int		len;

	sp = buffer;
	while(1){
		if(*sp == '\0'){
			break;
		}
		sp = GetEpsCommandString(cmdstr, sp);
		len = strlen(cmdstr);
		if(cmdstr[len-2] == 'T'){
			//StrgEpsCommand(cmdstr);
		}
		else if(cmdstr[len-2] == '*'){
			PassEpsCommand(cmdstr);
		}
		else if(cmdstr[len-2] == 'X' && cmdstr[len-1] == 'a'){
			EpsClrCommand(cmdstr);			
		}
		else if(cmdstr[len-2] == ' ' || len == 1){
			switch(cmdstr[len-1]){
			case 'm':
			case 's':	// 線あり、FILLなし
			case 'S':
			case 'b':	// 線あり、FILLあり
			case 'B':
			case 'f':	// 線なし、FILLあり 線ありに
			case 'F':
			case 'n':	// 線なし、FILLなし
			case 'N':
			case 'l':
			case 'L':
			case 'c':
			case 'C':
			case 'v':
			case 'V':
			case 'y':
			case 'Y':
				PlgnEpsCommand(cmdstr);
				break;
			case 'A':
			case 'M':
			case 'O':
			case 'i':
			case 'J':
			case 'j':
			case 'w':
			case 'd':
			case 'k':
			case 'K':
			case 'g':
			case 'G':
			case 'u':
			case 'U':
				EnvEpsCommand(cmdstr);
				break;
			}
		}
	}
}

LPSTR CFileEps::GetEpsCommandString(LPSTR cmdstr, LPSTR sp)
{
	int idx;
	int	flag;

	idx = 0;
	flag = ON;
	while(1){
		if(*sp == '\0'){
			break;
		}
		if(flag == ON && *sp == 'T' && isalpha(*(sp+1)) ){
			if( *(sp+2) == ' ' || *(sp+2) == '\0'){
				cmdstr[idx++] = *sp++;
				cmdstr[idx++] = *sp++;
				break;
			}
		}
		else if( flag == ON && isalpha(*sp) ){
			if( *(sp+1) == ' ' || *(sp+1) == '\0'){
				cmdstr[idx++] = *sp++;
				break;
			}
		}

		if( *sp == ' '){
			flag = ON;
		}
		else{
			flag = OFF;
		}

		cmdstr[idx++] = *sp++;
	}
	cmdstr[idx] = '\0';
	return(sp);
}

void CFileEps::PlgnEpsCommand(LPSTR cmdstr)
{
	DBL		x2, y2, x3, y3, x4, y4;
	int		len;
	int		backfillpenno;
	int		backlinepenno;
	DBL		backlinethick;
	int		penno;
	DBL		thick;

	len = strlen(cmdstr);
	switch(cmdstr[len-1]){
	case 'b':	
	case 'B':	// 
		if(m_pcVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcVect, m_nPP);
			m_pcDataList->SetDefFillClr();
			m_pcDataList->SetDefOutLineClr();
			m_pcVect->OnFlag(REJIONFLAG);
			m_pcVect = NULL;
		}
		break;
	case 'f':	//  アウトラインを持たないデータ
	case 'F':	// 
		if(m_pcVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcVect, m_nPP);
			backlinepenno = senvgetcrtlinepenno();
			backlinethick = senvgetcrtlinethick();

			backfillpenno = senvgetcrtfillpenno();
			penno = backfillpenno;
			thick = 0;

			senvsetcrtlinepenno(penno);
			senvsetcrtlinethick(thick);
			m_pcDataList->SetDefFillClr();
			m_pcDataList->SetDefOutLineClr();
			m_pcVect->OnFlag(REJIONFLAG);
			senvsetcrtlinepenno(backlinepenno);
			senvsetcrtlinethick(backlinethick);
			m_pcVect = NULL;
		}
		break;
	case 'n':	//  アウトラインを持たないデータ
	case 'N':	// 
		if(m_pcVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcVect, m_nPP);
			backlinepenno = senvgetcrtlinepenno();
			backlinethick = senvgetcrtlinethick();
			backfillpenno = senvgetcrtfillpenno();
			penno = -1;
			thick = 0;
			senvsetcrtfillpenno(penno);
			senvsetcrtlinepenno(penno);
			senvsetcrtlinethick(thick);
			m_pcDataList->SetDefFillClr();
			m_pcDataList->SetDefOutLineClr();
			m_pcVect->OnFlag(REJIONFLAG);
			senvsetcrtlinepenno(backlinepenno);
			senvsetcrtlinethick(backlinethick);
			senvsetcrtfillpenno(backfillpenno);
			m_pcVect = NULL;
		}
		break;
	case 's':
		if(m_pcVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcVect, m_nPP);
			backfillpenno = senvgetcrtfillpenno();
			penno = -1;
			senvsetcrtfillpenno(penno);
			m_pcDataList->SetDefFillClr();
			m_pcDataList->SetDefOutLineClr();
			m_pcVect->OnFlag(REJIONFLAG);
			senvsetcrtfillpenno(backfillpenno);
			m_pcVect = NULL;
		}
		break;
	case 'S':
		if(m_pcVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcVect, m_nPP);
			m_pcDataList->SetDefFillClr();
			m_pcDataList->SetDefOutLineClr();
			m_pcVect->OffFlag(REJIONFLAG);
			m_pcVect = NULL;
		}
		break;
	case 'm':
		if(m_nPassFlag == OFF){
			m_pcDataList = (CDataList*)(m_pcDoc->NewData(DATALISTID));
			m_pcDataGrup->SetData((CData*)m_pcDataList);
		}
		m_pcVect =(CVect*)(m_pcDoc->NewData(BASEVECTID));
		m_pcDataList->SetData((CData*)m_pcVect);
		m_nPP = 0;
		sscanf(cmdstr, "%lf %lf", &m_dCrtx, &m_dCrty);
		break;
	case 'l':
	case 'L':
		if(m_pcVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			sscanf(cmdstr, "%lf %lf", &m_dCrtx, &m_dCrty);
		}
		break;
	case 'c':
	case 'C':
		if(m_pcVect != NULL){
			sscanf(cmdstr, "%lf %lf %lf %lf %lf %lf",
					&x2, &y2, &x3, &y3, &x4, &y4);
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcVect->SetAtrDXY(m_nPP, BEZEATR|CURVATR, m_dCrtx, m_dCrty);
			m_nPP++;
			TrnsPointToMMXY(&x2, &y2);
			m_pcVect->SetAtrDXY(m_nPP, BEZEATR, x2, y2);
			m_nPP++;
			TrnsPointToMMXY(&x3, &y3);
			m_pcVect->SetAtrDXY(m_nPP, BEZEATR, x3, y3);
			m_nPP++;
			m_dCrtx = x4; m_dCrty = y4;
		}
		break;
	case 'v':
	case 'V':
		if(m_pcVect != NULL){
			sscanf(cmdstr, "%lf %lf %lf %lf",
					&x3, &y3, &x4, &y4);
			x2 = m_dCrtx; y2 = m_dCrty;
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcVect->SetAtrDXY(m_nPP, BEZEATR|CURVATR, m_dCrtx, m_dCrty);
			m_nPP++;
			TrnsPointToMMXY(&x2, &y2);
			m_pcVect->SetAtrDXY(m_nPP, BEZEATR, x2, y2);
			m_nPP++;
			TrnsPointToMMXY(&x3, &y3);
			m_pcVect->SetAtrDXY(m_nPP, BEZEATR, x3, y3);
			m_nPP++;
			m_dCrtx = x4; m_dCrty = y4;
		}
		break;
	case 'y':
	case 'Y':
		if(m_pcVect != NULL){
			sscanf(cmdstr, "%lf %lf %lf %lf",
						&x2, &y2, &x3, &y3);
			x4 = x3; y4 = y3;
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcVect->SetAtrDXY(m_nPP, BEZEATR|CURVATR, m_dCrtx, m_dCrty);
			m_nPP++;
			TrnsPointToMMXY(&x2, &y2);
			m_pcVect->SetAtrDXY(m_nPP, BEZEATR, x2, y2);
			m_nPP++;
			TrnsPointToMMXY(&x3, &y3);
			m_pcVect->SetAtrDXY(m_nPP, BEZEATR, x3, y3);
			m_nPP++;
			m_dCrtx = x4; m_dCrty = y4;
		}
		break;
	}
}

void CFileEps::StrgEpsCommand(LPSTR cmdstr)
{
	char	strg[RECMAX];
	int		len;
	int		backfillpenno;
	int		backlinepenno;
	DBL		backlinethick;
	int		cnt;
	int		flag;
	DBL		fltx, flty;

	len = strlen(cmdstr);
	switch(cmdstr[len-1]){
	case 'o':	//文字列開始
		m_nTateYoko = -1;
		break;
	case 'O':	//文字列終了
		break;
	case 'p':
		sscanf(cmdstr, "%lf %lf %lf %lf %lf %lf %lf", &m_dA1, &m_dB1, &m_dA2, &m_dB2, &m_dSX, &m_dSY, &m_dXX);
		TrnsPointToMMXY(&m_dSX, &m_dSY);	//
		m_dTopX = m_dSX;
		m_dTopY = m_dSY;
		break;
	case 'f':
		m_nStrIdx = 0;
		cnt = basestrgcntget(cmdstr);
		if(cnt == 3){
			sscanf(cmdstr, "%s %lf", m_szFont, &m_dW);
			m_dH = m_dW;
			m_dMS = 0.0;
		}
		else{
			sscanf(cmdstr, "%s %lf %lf %lf", m_szFont, &m_dW, &m_dH, &m_dMS);
		}
		epsfmmget(&m_dW);
		epsfmmget(&m_dH);
		epsfmmget(&m_dMS);
		m_dMS = 0;
		break;
	case 'd':
		sscanf(cmdstr, "%lf %lf", &m_dLXS, &m_dLYS); 
		break;
	case 't':
		sscanf(cmdstr, "%lf", &m_dSPF); 
		break;
	case 'v':
		sscanf(cmdstr, "%d", &m_nTateYoko);
		break;
	case 'a':
		sscanf(cmdstr, "%d", &flag);
		switch(flag){
		case 0:
			m_nOder = ODERLEFT;
			break;
		case 1:
			m_nOder = ODERCNTR;
			break;
		case 2:
			m_nOder = ODERLEFT;
			break;
		case 3:
		case 4:
			m_nOder = ODEREQUAL;
			break;
		}
		break;
	case 'x':
	case 'X':
		backfillpenno = senvgetcrtfillpenno();
		backlinepenno = senvgetcrtlinepenno();
		backlinethick = senvgetcrtlinethick();
		senvsetcrtlinepenno(backfillpenno);
		senvsetcrtlinethick(0.0);
		GetCodeToStrg(cmdstr, strg);
		AnyStringCreate(strg);
		senvsetcrtfillpenno(backfillpenno);
		senvsetcrtlinepenno(backlinepenno);
		senvsetcrtlinethick(backlinethick);
		break;
	case 'z':
		sscanf(cmdstr, "%lf %lf", &fltx, &flty);
		m_dFltPar = fltx / flty;
		break;
	}
}

void CFileEps::PassEpsCommand(LPSTR cmdstr)
{
	int		len;

	len = strlen(cmdstr);
	switch(cmdstr[len-1]){
	case 'u':	//PASS開始
		m_pcDataList = (CDataList*)(m_pcDoc->NewData(DATALISTID));
		m_pcDataGrup->SetData((CData*)m_pcDataList);
		m_nPassFlag = ON;
		break;
	case 'U':	//PASS終了
		m_nPassFlag = OFF;
		break;
	}
}

void CFileEps::EpsClrCommand(LPSTR cmdstr)
{
	int		cnt;
	DBL		c, m, y, k, r, g, b;
	int		rval, gval, bval;

	cnt = basestrgcntget(cmdstr);
	if(cnt == 4){
		sscanf(cmdstr, "%lf %lf %lf", &r, &g, &b);
	}
	else{
		sscanf(cmdstr, "%lf %lf %lf %lf %lf %lf %lf"
			, &c, &m, &y, &k, &r, &g, &b);
			//rval = 255 - (int)((c + k) * 255.0);
			//gval = 255 - (int)((m + k) * 255.0);
			//bval = 255 - (int)((y + k) * 255.0);
	}
	rval = (int)(r * 255.0);
	gval = (int)(g * 255.0);
	bval = (int)(b * 255.0);
	RENGE(rval, 0, 255);
	RENGE(gval, 0, 255);
	RENGE(bval, 0, 255);
	m_nFillPenno = senvgetpenno(rval, gval, bval);
	senvsetcrtfillpenno(m_nFillPenno);
}

void CFileEps::EnvEpsCommand(LPSTR cmdstr)
{
	int		len;
	int		rval, gval, bval;
	DBL		cyan, magenta, yellow, black;

	len = strlen(cmdstr);
	switch(cmdstr[len-1]){
	case 'k':
		sscanf(cmdstr, "%lf %lf %lf %lf", &cyan, &magenta, &yellow, &black); 
		rval = 255 - (int)((cyan + black) * 255.0);
		gval = 255 - (int)((magenta + black) * 255.0);
		bval = 255 - (int)((yellow + black) * 255.0);
		RENGE(rval, 0, 255);
		RENGE(gval, 0, 255);
		RENGE(bval, 0, 255);
		m_nFillPenno = senvgetpenno(rval, gval, bval);
		senvsetcrtfillpenno(m_nFillPenno);
		break;
	case 'K':
		sscanf(cmdstr, "%lf %lf %lf %lf", &cyan, &magenta, &yellow, &black); 
		rval = 255 - (int)((cyan + black) * 255.0);
		gval = 255 - (int)((magenta + black) * 255.0);
		bval = 255 - (int)((yellow + black) * 255.0);
		RENGE(rval, 0, 255);
		RENGE(gval, 0, 255);
		RENGE(bval, 0, 255);
		m_nOutLinePenno = senvgetpenno(rval, gval, bval);
		senvsetcrtlinepenno(m_nOutLinePenno);
		break;
	case 'g':
		sscanf(cmdstr, "%lf", &black); 
		rval = (int)(black * 255.0);
		gval = (int)(black * 255.0);
		bval = (int)(black * 255.0);
		RENGE(rval, 0, 255);
		RENGE(gval, 0, 255);
		RENGE(bval, 0, 255);
		m_nFillPenno = senvgetpenno(rval, gval, bval);
		senvsetcrtfillpenno(m_nFillPenno);
		break;
	case 'G':
		sscanf(cmdstr, "%lf", &black); 
		rval = (int)(black * 255.0);
		gval = (int)(black * 255.0);
		bval = (int)(black * 255.0);
		RENGE(rval, 0, 255);
		RENGE(gval, 0, 255);
		RENGE(bval, 0, 255);
		m_nOutLinePenno = senvgetpenno(rval, gval, bval);
		senvsetcrtlinepenno(m_nOutLinePenno);
		break;
	case 'w':
		sscanf(cmdstr, "%lf", &m_dOutLineThick); 
		epsfmmget(&m_dOutLineThick);
		senvsetcrtlinethick(m_dOutLineThick);
		break;
	case 'u':	//PASS開始
		m_pcDataList = (CDataList*)(m_pcDoc->NewData(DATALISTID));
		m_pcDataGrup->SetData((CData*)m_pcDataList);
		m_nPassFlag = ON;
		break;
	case 'U':	//PASS終了
		m_nPassFlag = OFF;
		break;
	}
}

void CFileEps::AnyStringCreate(LPSTR strg)
{
	LPSTR	sp;
	int		ch;
	int		len;


	if(m_nTateYoko == -1){
		len = strlen(m_szFont);
		if(m_szFont[len-1] == 'V'){
			m_nTateYoko = 1;
		}
		else if(m_szFont[len-1] == 'H'){
			m_nTateYoko = 0;
		}
	}

	if(m_nTateYoko == 1){
		m_dLS = - m_dW + m_dLXS;
		m_dMS = m_dW * m_dSPF / 1000;
		m_nSetMode = CNTRCNTR;
	}
	else{
		m_dLS = m_dH + m_dLYS;
		m_dMS = m_dH * m_dSPF / 1000;
		m_nSetMode = LEFTDOWN;
	}

	sp = strg;
	while(1){
		if(*sp == '\0'){
			break;
		}
		ch = (*sp) & 0x00ff;
		if(ch == CR){
			if(m_nStrIdx != 0){
				StringCreate(m_szString);
				m_nStrIdx = 0;
			}
			if(m_nTateYoko == 1){
				m_dSX += m_dLS;
				m_dSY = m_dTopY;
			}
			else{
				m_dSX = m_dTopX;
				m_dSY += m_dLS;
			}
			sp++;
		}
		else{
			m_szString[m_nStrIdx++] = *sp++;
			m_szString[m_nStrIdx] = 0;
		}
	}
}

void CFileEps::StringCreate(LPSTR strg)
{
	char	fontname[RECMAX];
	int		len;
	DBL		dflatpar;
	DBL		dpitch;
	DBL		dthickpar;
	DBL		dsloprag;
	int		nroundkind;
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		movex, movey;

	if(strncmp("/_G", m_szFont, 3) == 0){
		strcpy(fontname, "ＭＳゴシック");
	}
	else if(strncmp("/_MS-G", m_szFont, 6) == 0){
		strcpy(fontname, "ＭＳゴシック");
	}
	else{
		strcpy(fontname, "ＭＳ明朝");
	}
	m_pcSPtnVect = (CSPtnVect*)(m_pcDoc->NewData(SPTNVECTID));
	if(m_pcSPtnVect == NULL){
		return;
	}

	if(m_nTateYoko == 1){
		sx = m_dSX - m_dW/2;
		sy = m_dSY;
	}
	else{
		sx = m_dSX;
		sy = m_dSY + m_dH;
	}
	dflatpar = 100.0;
	dpitch = (100 + m_dSPF / 10.0) / 100.0;
	dthickpar = 100.0;
	dsloprag = 0;
	nroundkind = 0;
	m_pcSPtnVect->SetFlatPar(dflatpar);
	m_pcSPtnVect->SetPitch(dpitch);
	m_pcSPtnVect->SetThickPar(dthickpar);
	m_pcSPtnVect->SetSlopRag(dsloprag);
	m_pcSPtnVect->SetOderKind(m_nOder);
	m_pcSPtnVect->SetRoundKind(nroundkind);
	m_pcSPtnVect->SetSetMode(m_nSetMode);
	m_pcSPtnVect->SetSpaceEnv(m_dMS, m_dLS);
	m_pcSPtnVect->SetStrStartXY(sx, sy);
	m_pcSPtnVect->SetFontSize(m_dW, m_dH);
	m_pcSPtnVect->SetStrLen(0.0);
	if(m_nTateYoko == 1){
		m_pcSPtnVect->SetTateYoko(TATEGAKI);
	}
	else{
		m_pcSPtnVect->SetTateYoko(YOKOGAKI);
	}
	m_pcSPtnVect->SetFontName(fontname);
	m_pcSPtnVect->SetString(strg);

	len = strlen(strg);
	sx = m_dSX; sy = m_dSY;
	if(m_nTateYoko == 1){
		m_dSY -= (len*m_dH); 
	}
	else{
		m_dSX += (len*(m_dW/2));
	}
	ex = m_dSX; ey = m_dSY;
	movex = 0;
	movey = 0;
	switch(m_nOder){
	case ODERCNTR:
		if(m_nTateYoko == 1){
			movex = 0;
			movey = -(ey-sy)/2;
		}
		else{
			movex = -(ex-sx)/2;
			movey = 0;
		}
		break;
	case ODERLEFT:
		movex = 0;
		movey = 0;
		break;
	case ODERRIGHT:
		if(m_nTateYoko == 1){
			movex = 0;
			movey = -(ey-sy);
		}
		else{
			movex = -(ex-sx);
			movey = 0;
		}
		break;
	}
	m_pcVect = (CVect*)(m_pcDoc->NewData(BASEVECTID));
	m_pcVect->SetAtrDXY(0, 0, sx+movex, sy+movey);
	m_pcVect->SetAtrDXY(1, 0, ex+movex, ey+movey);
	m_pcVect->SetPointCount(2);
	m_pcVect->OffFlag(REJIONFLAG);
	m_pcSPtnVect->SetFitingVect(m_pcVect);
	m_pcSPtnVect->RenewMiniMax();

	m_pcSPtnVect->StrgLineStruct();
	m_pcSPtnVect->RenewMiniMax();
	
	m_pcDataGrup->SetData((CData*)m_pcSPtnVect);
	m_pcVect = NULL;
}

void CFileEps::GetCodeToStrg(LPSTR cmdstr, LPSTR strg)
{
	LPSTR	sp;
	char	numstr[10];
	int		num;
	int		idx;

	sp = cmdstr;
	while(1){
		if(*sp == '\0'){
			return;
		}
		if(*sp++ == '('){
			break;
		}
	}
	idx = 0;
	while(1){
		if(*sp == '\0' || *sp == ')'){
			break;
		}
		if(*sp == '\\'){
			if(isdigit(*(sp+1))){
				numstr[0] = *(sp+1);
				numstr[1] = *(sp+2);
				numstr[2] = *(sp+3);
				numstr[3] = '\0';
				sscanf(numstr, "%o", &num);
				strg[idx++] = (char)num;
				sp += 4;
			}
			else if(*(sp+1) == 'r'){
				strg[idx++] = CR;
				sp += 2;
			}
			else if(*(sp+1) == '\\'){
				strg[idx++] = '\\';
				sp += 2;
			}
			else{
				sp += 2;
			}

		}
		else{
			strg[idx++] = *sp++;
		}
	}
	strg[idx] = '\0';
}

int CFileEps::CheckNumStrg(LPSTR str, int len)
{
	int		i;

	for(i = 0; i < len; i++){
		if(isdigit(str[i]) == 0){
			if(('.' != str[i])
			&& ('-' != str[i])
			&& (' ' != str[i])){
				return(FALSE);
			}
		}
	}
	return(TRUE);
}

void CFileEps::SetVectData(CVect * pcVect, int pp)
{
	if(pp == 2){
		pcVect->OffFlag(REJIONFLAG);
	}
	else{
		pcVect->OnFlag(REJIONFLAG);
	}
	pcVect->SetPointCount(pp);
	pcVect->RenewMiniMax();
}

BOOL CFileEps::CrDelFgets(char * buffer, int max, FILE * fp)
{
	int		idx, ret;

	idx = 0;
	while(1){
		if(max-2 < idx){
			break;
		}
		ret = fread(&(buffer[idx]), 1, 1, fp);
		if(ret == 0){
			buffer[idx] = '\0';
			return(FALSE);
		}
		if(buffer[idx] != 0x0a){
			if(buffer[idx] == 0x0d){
				buffer[idx] = '\0';
				idx++;
				break;
			}
			idx++;
		}
	}
	return(TRUE);
}

void CFileEps::EpsBmpCreate(FILE *fp)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	char	buffer[RECMAX];
	char	buffer2[RECMAX];
	char	buffer3[RECMAX];
	char	numbuf[16];
	int		len;
	DBL		a11, a12, a13, a21, a22, a23;
	DBL		t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12;
	DBL		xdot, ydot;
	DBL		bytecnt;
	DBL		sx, sy, ex, ey;
	int		i, idx, idx2;
	int		colors[5];
	
	senvgetexepath(exepath);
	sprintf(fname, "%s\\bmp%05d.bmp", exepath, m_pcView->m_nBmpCnt);
	(m_pcView->m_nBmpCnt)++;
	m_nRGBCrt = 0;
	xdot = 0; ydot = 0;
	sx = 0; sy = 0; ex = 0; ey = 0;
	bytecnt = 0;
	while(1){
		if(CrDelFgets(buffer, RECMAX, fp) == FALSE){
			break;
		}
		len = strlen(buffer);
		if(strncmp(&(buffer[len-2]),"XG", 2) == 0){
			//(filename) 0 XG
		}
		else if(strncmp(&(buffer[len-2]),"Xh", 2) == 0){
			sscanf(buffer, "[ %lf %lf %lf %lf %lf %lf ] %lf %lf %lf",
					&a11, &a21, &a12, &a22, &a13, &a23, &t1, &t2, &t3);
			xdot = t1;
			ydot = t2;
			sx = a13;
			ey = a23;
			ex = a13 + (DBL)(xdot*a11+ydot*a21);
			sy = a23 - (DBL)(xdot*a12+ydot*a22);
			if(a21 != 0 && a22 != 0){
				m_nRound = ON;
			}
			else{
				m_nRound = OFF;
			}
			TrnsPointToMMXY(&sx, &sy);
			TrnsPointToMMXY(&ex, &ey);

			if(CrDelFgets(buffer, RECMAX, fp) == FALSE){
				break;
			}
			sscanf(buffer, "[ %lf %lf %lf %lf %lf %lf ] %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf"
					, &a11, &a21, &a12, &a22, &a13, &a23
					, &t1, &t2, &t3, &t4, &t5, &t6, &t7, &t8, &t9, &t10, &t11, &t12);
			bytecnt = t8;
		}
		else if(strncmp(&(buffer[len-2]), "XI", 2) == 0){
			break;
		}
		//[ 1 0 0 1 0 118 ] 0 0 142 118 142 118 8 3 0 0 0 0
		//%%BeginData: 105568
	}
	
	EpsBmpFileInit(fname, (int)xdot, (int)ydot);

	CrDelFgets(buffer, RECMAX, fp);
	idx = 1;

	for(i = 0;i < xdot * ydot;i++){
	// 文字列から１ピクセル分取り出す
		idx2 = 0;
		while(idx2 < bytecnt){
		// チェックして補充
			if(buffer[idx] < 0x10){
				CrDelFgets(buffer2, RECMAX, fp);
				buffer3[0] = '\0';
				strcat(buffer3, "%");
				strcat(buffer3, (buffer + idx));
				strcat(buffer3, (buffer2 + 1));
				strcpy(buffer, buffer3);
				idx = 1;
			}

			// 文字列2文字を数値に変換
			strncpy(numbuf, &(buffer[idx]), 2);
			sscanf(numbuf, "%X", &colors[idx2]);

			idx += 2;
			idx2++;
		}

		// 数値をBMPデータに格納
		switch((int)bytecnt){
		case 1:
			break;
		case 3:
			EpsBmpFileBitSet(colors[0]);
			EpsBmpFileBitSet(colors[1]);
			EpsBmpFileBitSet(colors[2]);
			break;
		case 4:
			colors[2] = 255 - (colors[2] + colors[3]);
			colors[1] = 255 - (colors[1] + colors[3]);
			colors[0] = 255 - (colors[0] + colors[3]);
			EpsBmpFileBitSet(colors[0]);
			EpsBmpFileBitSet(colors[1]);
			EpsBmpFileBitSet(colors[2]);
			break;
		}
	}

	CrDelFgets(buffer, RECMAX, fp);	//XH
	CrDelFgets(buffer, RECMAX, fp);	//%AI5_EndRaster

	EpsBmpFileEnd();
	CDibCData* pcDibCData = (CDibCData*)(m_pcDoc->NewData(DIBCDATAID));
	if(pcDibCData == NULL){
		return;
	}
	pcDibCData->SetBmpFname(fname);
	pcDibCData->DibLoad();
	pcDibCData->SetDstElement(sx, sy, ex-sx, ey-sy);
	pcDibCData->RenewMiniMax();
	if(m_nRound == ON){
		pcDibCData->RoundDib(1);
		pcDibCData->SetDstElement(sx, sy, ex-sx, ey-sy);
		pcDibCData->RenewMiniMax();
	}
	m_pcDoc->DataAddTail((CData*)pcDibCData);
}

void CFileEps::EpsBmpFileBitSet(UINT tt)
{
	switch(m_nRGBCrt){
	case 0:	
		m_szRGB[2] = (BYTE)tt;
		m_nRGBCrt++;
		break;
	case 1:
		m_szRGB[1] = (BYTE)tt;
		m_nRGBCrt++;
		break;
	case 2:
		m_szRGB[0] = (BYTE)tt;
		m_nRGBCrt = 0;
		//fwrite(m_szRGB, 1, 3, m_pFile);
		*m_lpCrtBits++ = m_szRGB[0];
		*m_lpCrtBits++ = m_szRGB[1];
		*m_lpCrtBits++ = m_szRGB[2];
		m_nXCrt++;
		if(m_nXCrt >= m_nXDotSize){
			m_nXCrt = 0;
			m_nLineCrt++;
			if(m_nLineCrt >= m_nYDotSize){
				m_nLineCrt = m_nYDotSize-1;
			}
			m_lpCrtBits = m_lpBits + (m_lBitsSize - (m_nLineCrt * m_nByte));
		}
		break;
	}
}

void CFileEps::EpsBmpFileInit(LPSTR fname, int xsize, int ysize)
{
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;

	if((m_pFile = fopen(fname, "wb")) == NULL){
		return;
	}

	m_nXDotSize = xsize;
	m_nYDotSize = ysize;
	m_nByte = (((xsize * 24 + 7) / 8) + 3L) / 4L * 4L;
	m_lBitsSize = m_nByte * ysize;

	m_lFileSize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + m_lBitsSize;
	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = m_lFileSize;
	bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = xsize;
	bmpinfo.bmiHeader.biHeight = ysize;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lBitsSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, m_pFile);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, m_pFile);
	m_nLineCrt = 1;
	m_nXCrt = 0;
	m_nRGBCrt = 0;
	m_hBits = GlobalAlloc(GHND, m_lBitsSize+m_nByte*2);
	m_lpBits = (BYTE *) GlobalLock(m_hBits);
	m_lpCrtBits = m_lpBits + (m_lBitsSize - (m_nLineCrt * m_nByte));
}

void CFileEps::EpsBmpFileEnd()
{
	int		y;
	int		ret;

	for(y = 0; y < m_nYDotSize; y++){
		m_lpCrtBits = m_lpBits + y * m_nByte;
		ret = fwrite(m_lpCrtBits, 1, m_nByte, m_pFile);
	}
	GlobalUnlock(m_hBits);
	GlobalFree(m_hBits);
	fclose(m_pFile);
}

void CFileEps::TrnsPointToMMXY(LPDBL x, LPDBL y)
{
	DBL	mx, my;

	mx = *x; my = *y;
	*x = mx * MMPARINTI / 72.0;
	*y = my * MMPARINTI / 72.0;
}

void CFileEps::TrnsPointToMMLen(LPDBL len)
{
	DBL	mlen;

	mlen = *len;
	*len = mlen * MMPARINTI / 72.0;
}

