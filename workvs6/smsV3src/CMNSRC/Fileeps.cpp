
/////////////////////////////////////////////////////////////////////////////
// CFileEps

#include "stdafx.h"
#include "stdio.h"
#include "math.h"
#include "ctype.h"

#include "fileeps.h"

#define	RENGE(a, b, c)	if((a) < (b)){(a) = (b);} else if((c) < (a)){(a) = (c);}

#define	GRABMPXSIZE		256
#define	GRABMPYSIZE		256
#define	CLIPBMPXSIZE	1024
#define	CLIPBMPYSIZE	1024

extern "C" {
	int baseiskanji(UINT far hc);
	int basestrgcntget(char far *strg);
	int senvsetdrawmode(int mode);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	int senvgetpenno(int rval, int gval, int bval);
	int senvgetcrtfillpenno();
	int senvgetcrtlinepenno();
	DBL senvgetcrtlinethick();
	void senvsetcrtfillpenno(int penno);
	void senvsetcrtlinepenno(int penno);
	void senvsetcrtlinethick(DBL thick);
	void senvgetexepath(LPSTR path);
	DBL scrnsetxytime(DBL timex, DBL timey);
	void scrngetxytime(LPDBL timex, LPDBL timey);
	void scrnsetrltvelement();
	void scrnsetrltvtop(DBL xTop, DBL yTop);
	void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
	void scrnsetdisprltvmove(DBL xmove, DBL ymove);
	void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove);
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void diblibcreatebmpfile(LPSTR bmpfname);
	void epsfmmget(LPDBL len);
	int epsffileopen(char *filename);
	int epsfstartfile(char *fname, DBL maxx, DBL maxy);
	int epsfendfile();
	int epsffileclose();
}

CFileEps::CFileEps(CScrollView* pcview)
:CCmdBase(pcview)
{
	m_pcView = (CTonView*)pcview;
	m_pcDoc = (CTonDoc*)(m_pcView->GetDocument());
}

void CFileEps::SaveData(LPSTR filename)
{
	char	exepath[FNAMEMAX];
	DBL		mmxsize, mmysize;

	senvgetexepath(exepath);
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

	m_nPassFlag = OFF;
	m_nQIdx = 0;
	m_nGraTblMax = 0;
	m_pcCrtDataList = NULL;
	m_pcCrtClipGrup = NULL;

	m_nTateYoko = 0;
	m_dLXS = 0; m_dLYS = 0;
	m_dBaseX = 0; m_dBaseY = 0;
	m_dTopX = 0; m_dTopY = 0;
	m_dA1 = 0; m_dB1 = 0; m_dA2 = 0; m_dB2 = 0;

	m_nFillPenno = 0;
	m_nOutlinePenno = 0;
	m_dOutlineThick = 0;
	senvsetcrtfillpenno(m_nFillPenno);
	senvsetcrtlinepenno(m_nOutlinePenno);
	senvsetcrtlinethick(m_dOutlineThick);

	m_pcTopDataGrup = (CDataGrup*)(m_pcDoc->NewData(DATAGRUPID));
	m_pcCrtDataGrup = m_pcTopDataGrup;
	if((fp = fopen(filename, "rb")) == NULL){
		return;
	}
	MemDCInit();
	SkipEpsHead(fp);
	EpsLoadLoop(fp);
	fclose(fp);
	m_pcDoc->DataAddTail((CData*)m_pcTopDataGrup);
	MemDCEnd();

	senvsetcrtfillpenno(backfillpenno);
	senvsetcrtlinepenno(backlinepenno);
	senvsetcrtlinethick(backlinethick);
}

CData *CFileEps::LoadDataNotAdd(LPSTR filename)
{
	FILE	*fp;
	int		backfillpenno;
	int		backlinepenno;
	DBL		backlinethick;

	backfillpenno = senvgetcrtfillpenno();
	backlinepenno = senvgetcrtlinepenno();
	backlinethick = senvgetcrtlinethick();

	m_nTateYoko = 0;
	m_dLXS = 0;
	m_dLYS = 0;
	m_nPassFlag = OFF;
	m_nQIdx = 0;
	m_nGraTblMax = 0;
	m_pcCrtDataList = NULL;
	m_pcCrtClipGrup = NULL;
	m_dBaseX = 0; m_dBaseY = 0;
	m_dTopX = 0; m_dTopY = 0;
	m_dA1 = 0; m_dB1 = 0; m_dA2 = 0; m_dB2 = 0;

	m_nFillPenno = 0;
	m_nOutlinePenno = 0;
	m_dOutlineThick = 0;
	senvsetcrtfillpenno(m_nFillPenno);
	senvsetcrtlinepenno(m_nOutlinePenno);
	senvsetcrtlinethick(m_dOutlineThick);

	m_pcTopDataGrup = (CDataGrup*)(m_pcDoc->NewData(DATAGRUPID));
	m_pcCrtDataGrup = m_pcTopDataGrup;
	if((fp = fopen(filename, "rb")) == NULL){
		return(NULL);
	}
	MemDCInit();
	SkipEpsHead(fp);
	EpsLoadLoop(fp);
	fclose(fp);
	MemDCEnd();

	senvsetcrtfillpenno(backfillpenno);
	senvsetcrtlinepenno(backlinepenno);
	senvsetcrtlinethick(backlinethick);
	return((CData*)m_pcTopDataGrup);
}

void CFileEps::GrupReset()
{
	CData	*pcData;
	CData	*pcpreData;

	pcData = m_pcCrtDataGrup->m_pcDataTop;
	while(pcData != NULL){
		m_pcDoc->DataAddTail(pcData);
		pcpreData = pcData;
		pcData = pcpreData->GetNext();
		pcpreData->SetNext(NULL);
	}
	m_pcCrtDataGrup->m_pcDataTop = NULL;
	m_pcDoc->DeleteData(m_pcCrtDataGrup);
	delete(m_pcCrtDataGrup);
}

void CFileEps::SkipEpsHead(FILE *fp)
{
	char	buffer[RECMAX];
	char	head[HRECMAX];

	while(1){
		if(EPSCrDelFgets(buffer, RECMAX, fp) == FALSE){
			break;
		}
		if(strncmp(buffer, "%%EndSetup", 10) == 0){
			break;
		}
		else if(strncmp(buffer, "%%BoundingBox", 13) == 0){
			sscanf(buffer, "%s %lf %lf %lf %lf", head, &m_dBindSX, &m_dBindSY, &m_dBindEX, &m_dBindEY);
			m_dOffsetX = m_dBindSX;
			m_dOffsetY = m_dBindSY;
			TrnsPointToMMLen(&m_dBindSX);
			TrnsPointToMMLen(&m_dBindSY);
			TrnsPointToMMLen(&m_dBindEX);
			TrnsPointToMMLen(&m_dBindEY);
			m_dPaperXMM = m_dBindEX - m_dBindSX;
			m_dPaperYMM = m_dBindEY - m_dBindSY;
		}
		else if(strncmp(buffer, "%AI5_ArtSize", 12) == 0){
			sscanf(buffer, "%s %lf %lf", head, &m_dPaperXSize, &m_dPaperYSize);
			m_dPaperXMM = m_dPaperXSize;
			m_dPaperYMM = m_dPaperYSize;
			TrnsPointToMMLen(&m_dPaperXMM);
			TrnsPointToMMLen(&m_dPaperYMM);
		}
		else if(strncmp(buffer, "%AI5_BeginGradient", 18) == 0){
			SetGraName(buffer, m_szGraName[m_nGraTblMax]);
			m_lGraSeekPoint[m_nGraTblMax] = ftell(fp);
			m_nGraTblMax++;
		}
	}
}

void CFileEps::EpsLoadLoop(FILE * fp)
{
	char	buffer[RECMAX];

	m_pcCrtVect = NULL;
	while(1){
		if(EPSCrDelFgets(buffer, RECMAX, fp) == FALSE){
			break;
		}
     	if(strncmp(buffer, "%%Trailer", 9) == 0){
			break;
		}
		else if(strncmp(buffer, "%AI5_BeginRaster", 16) == 0){
			EpsBmpCreate(fp);
		}
		else{
			EpsCommandLoop(fp, buffer);
		}
	}
	if(m_pcCrtVect != NULL){
		if(m_nPP == 0){
			delete(m_pcCrtVect);
		}
		else{
			SetVectData(m_pcCrtVect, m_nPP);
		}
	}
	SetAddDataGrup();
}

void CFileEps::EpsCommandLoop(FILE *fp, LPSTR buffer)
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
			EpsStrgCommand(cmdstr);
		}
		else if(cmdstr[len-2] == '*'){	// *u *U
			EpsPassCommand(cmdstr);
		}
		else if(cmdstr[len-2] == 'B'){	// Bb Bh Bg Bm BB
			EpsGraCommandLoop(fp, cmdstr);
		}
		else if(cmdstr[len-2] == 'X' && cmdstr[len-1] == 'a'){
			EpsClrCommand(cmdstr);			
		}
		else if(cmdstr[len-2] == ' ' || len == 1){
			switch(cmdstr[len-1]){
			case 'h':
			case 'q':
			case 'Q':
				EpsClipCommand(cmdstr);
				break;
			case '*':
			case 'n':	// 閉図形、線なし、FILLなし
			case 'N':	// 開放図形、線なし、FILLなし
			case 's':	// 閉図形、線あり、FILLなし
			case 'S':	// 開放図形、線あり、FILLなし
			case 'b':	// 閉図形、線あり、FILLあり
			case 'B':	// 開放図形、線あり、FILLあり
			case 'f':	// 閉図形、線なし、FILLあり
			case 'F':	// 開放図形、線なし、FILLあり
			case 'm':
			case 'l':
			case 'L':
			case 'c':
			case 'C':
			case 'v':
			case 'V':
			case 'y':
			case 'Y':
				EpsPlgnCommand(cmdstr);
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
				EpsEnvCommand(cmdstr);
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

void CFileEps::EpsPlgnCommand(LPSTR cmdstr)
{
	DBL		x1, y1, x2, y2, x3, y3, x4, y4;
	int		len;
	int		backfillpenno;
	int		backlinepenno;
	DBL		backlinethick;
	int		penno;
	DBL		thick;

	len = strlen(cmdstr);
	switch(cmdstr[len-1]){
	case '*':
		if(m_pcCrtVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcCrtVect, m_nPP);
			m_pcCrtDataList->SetDefFillClr();
			m_pcCrtDataList->SetDefOutLineClr();
			m_pcCrtVect->OnFlag(REJIONFLAG);
			m_pcCrtVect = NULL;
		}
		break;
	case 'n':	// 閉図形、線なし、FILLなし
		if(m_pcCrtVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcCrtVect, m_nPP);
			backfillpenno = senvgetcrtfillpenno();
			backlinepenno = senvgetcrtlinepenno();
			backlinethick = senvgetcrtlinethick();
			penno = -1;
			thick = 0;
			senvsetcrtfillpenno(penno);
			senvsetcrtlinepenno(penno);
			senvsetcrtlinethick(thick);

			m_pcCrtDataList->SetDefFillClr();
			m_pcCrtDataList->SetDefOutLineClr();
			m_pcCrtVect->OnFlag(REJIONFLAG);
			m_pcCrtVect = NULL;

			senvsetcrtlinepenno(backlinepenno);
			senvsetcrtlinethick(backlinethick);
			senvsetcrtfillpenno(backfillpenno);
		}
		break;
	case 'N':	// 開放図形、線なし、FILLなし
		if(m_pcCrtVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcCrtVect, m_nPP);
			backfillpenno = senvgetcrtfillpenno();
			backlinepenno = senvgetcrtlinepenno();
			backlinethick = senvgetcrtlinethick();
			penno = -1;
			thick = 0;
			senvsetcrtfillpenno(penno);
			senvsetcrtlinepenno(penno);
			senvsetcrtlinethick(thick);

			m_pcCrtDataList->SetDefFillClr();
			m_pcCrtDataList->SetDefOutLineClr();
			m_pcCrtVect->OffFlag(REJIONFLAG);
			m_pcCrtVect = NULL;

			senvsetcrtlinepenno(backlinepenno);
			senvsetcrtlinethick(backlinethick);
			senvsetcrtfillpenno(backfillpenno);
		}
		break;
	case 'f':	// 閉図形、線なし、FILLあり
		if(m_pcCrtVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcCrtVect, m_nPP);
			backfillpenno = senvgetcrtfillpenno();
			backlinepenno = senvgetcrtlinepenno();
			backlinethick = senvgetcrtlinethick();
			penno = -1;
			thick = 0;
			senvsetcrtlinepenno(penno);
			senvsetcrtlinethick(thick);

			m_pcCrtDataList->SetDefFillClr();
			m_pcCrtDataList->SetDefOutLineClr();
			m_pcCrtVect->OnFlag(REJIONFLAG);
			m_pcCrtVect = NULL;

			senvsetcrtlinepenno(backlinepenno);
			senvsetcrtlinethick(backlinethick);
			senvsetcrtfillpenno(backfillpenno);
		}
		break;
	case 'F':	// 開放図形、線なし、FILLあり
		if(m_pcCrtVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcCrtVect, m_nPP);
			backfillpenno = senvgetcrtfillpenno();
			backlinepenno = senvgetcrtlinepenno();
			backlinethick = senvgetcrtlinethick();
			penno = -1;
			thick = 0;
			senvsetcrtlinepenno(penno);
			senvsetcrtlinethick(thick);

			m_pcCrtDataList->SetDefFillClr();
			m_pcCrtDataList->SetDefOutLineClr();
			m_pcCrtVect->OffFlag(REJIONFLAG);
			m_pcCrtVect = NULL;

			senvsetcrtlinepenno(backlinepenno);
			senvsetcrtlinethick(backlinethick);
			senvsetcrtfillpenno(backfillpenno);
		}
		break;
	case 's':	// 閉図形、線あり、FILLなし
		if(m_pcCrtVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcCrtVect, m_nPP);
			backfillpenno = senvgetcrtfillpenno();
			backlinepenno = senvgetcrtlinepenno();
			backlinethick = senvgetcrtlinethick();
			penno = -1;
			thick = 0;
			senvsetcrtfillpenno(penno);

			m_pcCrtDataList->SetDefFillClr();
			m_pcCrtDataList->SetDefOutLineClr();
			m_pcCrtVect->OnFlag(REJIONFLAG);
			m_pcCrtVect = NULL;

			senvsetcrtlinepenno(backlinepenno);
			senvsetcrtlinethick(backlinethick);
			senvsetcrtfillpenno(backfillpenno);
		}
		break;
	case 'S':	// 開放図形、線あり、FILLなし
		if(m_pcCrtVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcCrtVect, m_nPP);
			backfillpenno = senvgetcrtfillpenno();
			backlinepenno = senvgetcrtlinepenno();
			backlinethick = senvgetcrtlinethick();
			penno = -1;
			thick = 0;
			senvsetcrtfillpenno(penno);

			m_pcCrtDataList->SetDefFillClr();
			m_pcCrtDataList->SetDefOutLineClr();
			m_pcCrtVect->OffFlag(REJIONFLAG);
			m_pcCrtVect = NULL;

			senvsetcrtlinepenno(backlinepenno);
			senvsetcrtlinethick(backlinethick);
			senvsetcrtfillpenno(backfillpenno);
		}
		break;
	case 'b':	// 閉図形、線あり、FILLあり
		if(m_pcCrtVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcCrtVect, m_nPP);

			m_pcCrtDataList->SetDefFillClr();
			m_pcCrtDataList->SetDefOutLineClr();
			m_pcCrtVect->OnFlag(REJIONFLAG);
			m_pcCrtVect = NULL;
		}
		break;
	case 'B':	// 開放図形、線あり、FILLあり
		if(m_pcCrtVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcCrtVect, m_nPP);

			m_pcCrtDataList->SetDefFillClr();
			m_pcCrtDataList->SetDefOutLineClr();
			m_pcCrtVect->OffFlag(REJIONFLAG);
			m_pcCrtVect = NULL;
		}
		break;
	case 'm':
		if(m_pcCrtDataList == NULL){
			m_pcCrtDataList = (CDataList*)(m_pcDoc->NewData(DATALISTID));
		}
		m_pcCrtVect =(CVect*)(m_pcDoc->NewData(BASEVECTID));
		m_pcCrtVect->OnFlag(REJIONFLAG);
		m_pcCrtDataList->SetData(m_pcCrtVect);
		m_nPP = 0;
		sscanf(cmdstr, "%lf %lf", &m_dCrtx, &m_dCrty);
		x1 = m_dCrtx; y1 = m_dCrty;
		TrnsPointToMMXY(&x1, &y1);
		m_pcCrtVect->SetAtrDXY(m_nPP, 0, x1, y1);
		m_pcCrtVect->SetPointCount(1);
		break;
	case 'l':
	case 'L':
		if(m_pcCrtVect != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			sscanf(cmdstr, "%lf %lf", &m_dCrtx, &m_dCrty);
		}
		break;
	case 'c':
	case 'C':
		if(m_pcCrtVect != NULL){
			sscanf(cmdstr, "%lf %lf %lf %lf %lf %lf",
					&x2, &y2, &x3, &y3, &x4, &y4);
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, BEZEATR|CURVATR, m_dCrtx, m_dCrty);
			m_nPP++;
			TrnsPointToMMXY(&x2, &y2);
			m_pcCrtVect->SetAtrDXY(m_nPP, BEZEATR, x2, y2);
			m_nPP++;
			TrnsPointToMMXY(&x3, &y3);
			m_pcCrtVect->SetAtrDXY(m_nPP, BEZEATR, x3, y3);
			m_nPP++;
			m_dCrtx = x4; m_dCrty = y4;
		}
		break;
	case 'v':
	case 'V':
		if(m_pcCrtVect != NULL){
			sscanf(cmdstr, "%lf %lf %lf %lf",
					&x3, &y3, &x4, &y4);
			x2 = m_dCrtx; y2 = m_dCrty;
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, BEZEATR|CURVATR, m_dCrtx, m_dCrty);
			m_nPP++;
			TrnsPointToMMXY(&x2, &y2);
			m_pcCrtVect->SetAtrDXY(m_nPP, BEZEATR, x2, y2);
			m_nPP++;
			TrnsPointToMMXY(&x3, &y3);
			m_pcCrtVect->SetAtrDXY(m_nPP, BEZEATR, x3, y3);
			m_nPP++;
			m_dCrtx = x4; m_dCrty = y4;
		}
		break;
	case 'y':
	case 'Y':
		if(m_pcCrtVect != NULL){
			sscanf(cmdstr, "%lf %lf %lf %lf",
						&x2, &y2, &x3, &y3);
			x4 = x3; y4 = y3;
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, BEZEATR|CURVATR, m_dCrtx, m_dCrty);
			m_nPP++;
			TrnsPointToMMXY(&x2, &y2);
			m_pcCrtVect->SetAtrDXY(m_nPP, BEZEATR, x2, y2);
			m_nPP++;
			TrnsPointToMMXY(&x3, &y3);
			m_pcCrtVect->SetAtrDXY(m_nPP, BEZEATR, x3, y3);
			m_nPP++;
			m_dCrtx = x4; m_dCrty = y4;
		}
		break;
	}
}

void CFileEps::EpsStrgCommand(LPSTR cmdstr)
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
	case 'p':	//文字列開始位置セット 
		sscanf(cmdstr, "%lf %lf %lf %lf %lf %lf %lf", &m_dA1, &m_dB1, &m_dA2, &m_dB2, &m_dSX, &m_dSY, &m_dXX);
		TrnsPointToMMXY(&m_dSX, &m_dSY);	//
		m_dTopX = m_dSX;
		m_dTopY = m_dSY;
		break;
	case 'f':	// 書体、文字サイズ、文字間隔
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

void CFileEps::EpsPassCommand(LPSTR cmdstr)
{
	int		len;

	len = strlen(cmdstr);
	switch(cmdstr[len-1]){
	case 'u':		//*uによりPASS開始
		SetAddDataGrup();
		if(m_nQIdx == 0){
			m_nPassFlag = ON;
		}
		break;
	case 'U':	//*UによるPASS終了
		if(m_nQIdx == 0){
			m_nPassFlag = OFF;
		}
		SetAddDataGrup();
		break;
	}
}

void CFileEps::EpsClrCommand(LPSTR cmdstr)
{
	DBL		c, m, y, k, r, g, b;
	int		rval, gval, bval;
	int		penno;
	int		cnt;

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
	r = r * 255.0;
	g = g * 255.0;
	b = b * 255.0;
	ClrRGBTrns(r, g, b, &rval, &gval, &bval);
	penno = senvgetcrtfillpenno();
	m_nFillPenno = senvgetpenno(rval, gval, bval);
	if(penno != m_nFillPenno){
		SetAddDataGrup();
		senvsetcrtfillpenno(m_nFillPenno);
	}
}

void CFileEps::EpsClipCommand(LPSTR cmdstr)
{
	int		len;
	len = strlen(cmdstr);
	switch(cmdstr[len-1]){
	case 'h':
		if(m_pcCrtDataList != NULL){
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcCrtVect, m_nPP);
			m_pcCrtDataList->SetDefFillClr();
			m_pcCrtDataList->SetDefOutLineClr();
			m_pcCrtVect->OnFlag(REJIONFLAG);
			m_pcCrtVect = NULL;
		}
		SetAddClipGrup();
		// クリッピングデータ
		break;
	case 'q':	//クリッピング開始
		SetAddDataGrup();
		m_pcQClipData[m_nQIdx] = (CClipData*)(m_pcDoc->NewData(CLIPDATAID));
		m_pcQClipGrup[m_nQIdx] = (CDataGrup*)(m_pcDoc->NewData(DATAGRUPID));
		m_pcQDataGrup[m_nQIdx] = (CDataGrup*)(m_pcDoc->NewData(DATAGRUPID));
		m_pcQClipData[m_nQIdx]->AddClip(m_pcQClipGrup[m_nQIdx]);
		m_pcQClipData[m_nQIdx]->AddData(m_pcQDataGrup[m_nQIdx]);
		m_pcCrtClipData = m_pcQClipData[m_nQIdx];
		m_pcCrtClipGrup = m_pcQClipGrup[m_nQIdx];
		m_pcCrtDataGrup = m_pcQDataGrup[m_nQIdx];
		m_nQIdx++;
		//h　W　n
		break;
	case 'Q':	//クリッピング開始
		m_nQIdx--;
		if(m_nQIdx <= 0){
			m_pcCrtClipGrup = NULL;
			m_pcCrtDataGrup = m_pcTopDataGrup;
			m_nQIdx = 0;
		}
		else{
			m_pcCrtClipGrup = m_pcQClipGrup[m_nQIdx-1];
			m_pcCrtDataGrup = m_pcQDataGrup[m_nQIdx-1];
		}
		m_pcCrtDataGrup->SetData((CData*)m_pcCrtClipData);
		if(m_nQIdx == 0){
			m_pcCrtClipData = NULL;
		}
		else{
			m_pcCrtClipData = m_pcQClipData[m_nQIdx-1];
		}
		break;
	}
}

void CFileEps::EpsEnvCommand(LPSTR cmdstr)
{
	int		len;
	DBL		r, g, b;
	int		rval, gval, bval;
	DBL		cyan, magenta, yellow, black;
	int		penno;
	DBL		dthick;

	len = strlen(cmdstr);
	switch(cmdstr[len-1]){
	case 'k':
		sscanf(cmdstr, "%lf %lf %lf %lf", &cyan, &magenta, &yellow, &black); 
		r = 255.0 - (cyan + black) * 255.0;
		g = 255.0 - (magenta + black) * 255.0;
		b = 255.0 - (yellow + black) * 255.0;
		ClrRGBTrns(r, g, b, &rval, &gval, &bval);
		penno = senvgetcrtfillpenno();
		m_nFillPenno = senvgetpenno(rval, gval, bval);
		if(penno != m_nFillPenno){
			SetAddDataGrup();
			senvsetcrtfillpenno(m_nFillPenno);
		}
		break;
	case 'K':
		sscanf(cmdstr, "%lf %lf %lf %lf", &cyan, &magenta, &yellow, &black); 
		r = 255.0 - (cyan + black) * 255.0;
		g = 255.0 - (magenta + black) * 255.0;
		b = 255.0 - (yellow + black) * 255.0;
		ClrRGBTrns(r, g, b, &rval, &gval, &bval);
		penno = senvgetcrtlinepenno();
		m_nOutlinePenno = senvgetpenno(rval, gval, bval);
		if(penno != m_nFillPenno){
			SetAddDataGrup();
			senvsetcrtlinepenno(m_nOutlinePenno);
		}
		break;
	case 'g':
		sscanf(cmdstr, "%lf", &black); 
		rval = (int)(black * 255.0);
		gval = (int)(black * 255.0);
		bval = (int)(black * 255.0);
		RENGE(rval, 0, 255);
		RENGE(gval, 0, 255);
		RENGE(bval, 0, 255);
		penno = senvgetcrtfillpenno();
		m_nFillPenno = senvgetpenno(rval, gval, bval);
		if(penno != m_nFillPenno){
			SetAddDataGrup();
			senvsetcrtfillpenno(m_nFillPenno);
		}
		break;
	case 'G':
		sscanf(cmdstr, "%lf", &black); 
		rval = (int)(black * 255.0);
		gval = (int)(black * 255.0);
		bval = (int)(black * 255.0);
		RENGE(rval, 0, 255);
		RENGE(gval, 0, 255);
		RENGE(bval, 0, 255);
		penno = senvgetcrtlinepenno();
		m_nOutlinePenno = senvgetpenno(rval, gval, bval);
		if(penno != m_nFillPenno){
			SetAddDataGrup();
			senvsetcrtlinepenno(m_nOutlinePenno);
		}
		break;
	case 'w':
		sscanf(cmdstr, "%lf", &m_dOutlineThick); 
		TrnsPointToMMLen(&m_dOutlineThick);
		dthick = senvgetcrtlinethick();
		if(dthick != m_dOutlineThick){
			SetAddDataGrup();
			senvsetcrtlinethick(m_dOutlineThick);
		}
		break;
	case 'u':	//PASS開始
		SetAddDataGrup();
		break;
	case 'U':	//PASS終了
		SetAddDataGrup();
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
	CSPtnVect*	pcSPtnVect;
	CVect*		pcVect;
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
	pcSPtnVect = (CSPtnVect*)(m_pcDoc->NewData(SPTNVECTID));
	if(pcSPtnVect == NULL){
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
	pcSPtnVect->SetFlatPar(dflatpar);
	pcSPtnVect->SetPitch(dpitch);
	pcSPtnVect->SetThickPar(dthickpar);
	pcSPtnVect->SetSlopRag(dsloprag);
	pcSPtnVect->SetOderKind(m_nOder);
	pcSPtnVect->SetRoundKind(nroundkind);
	pcSPtnVect->SetSetMode(m_nSetMode);
	pcSPtnVect->SetSpaceEnv(m_dMS, m_dLS);
	pcSPtnVect->SetStrStartXY(sx, sy);
	pcSPtnVect->SetFontSize(m_dW, m_dH);
	pcSPtnVect->SetStrLen(0.0);
	if(m_nTateYoko == 1){
		pcSPtnVect->TateYokoSet(TATEGAKI);
	}
	else{
		pcSPtnVect->TateYokoSet(YOKOGAKI);
	}
	pcSPtnVect->SetFontName(fontname);
	pcSPtnVect->SetString(strg);

	len = strlen(strg);
	sx = m_dSX; sy = m_dSY;
	if(m_nTateYoko == 1){
		m_dSY -= (len*m_dH); 
	}
	else{
		m_dSX += (len*(m_dW/2));
	}
	ex = m_dSX; ey = m_dSY;
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
	default:
		movex = 0;
		movey = 0;
		break;
	}
	pcVect = (CVect*)(m_pcDoc->NewData(BASEVECTID));
	pcVect->SetAtrDXY(0, 0, sx+movex, sy+movey);
	pcVect->SetAtrDXY(1, 0, ex+movex, ey+movey);
	pcVect->SetPointCount(2);
	pcVect->OffFlag(REJIONFLAG);
	pcSPtnVect->SetFitingVect(pcVect);
	pcSPtnVect->RenewMiniMax();

	pcSPtnVect->StrgLineStruct();
	pcSPtnVect->RenewMiniMax();
	
	m_pcCrtDataGrup->SetData((CData*)pcSPtnVect);
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
				strg[idx++] = num;
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

void CFileEps::SetVectData(CVect *pcVect, int pp)
{
	if(pcVect == NULL){
		return;
	}
	if(pp == 2){
		pcVect->OffFlag(REJIONFLAG);
	}
	else{
		pcVect->OnFlag(REJIONFLAG);
	}
	pcVect->SetPointCount(pp);
	pcVect->RenewMiniMax();
}

BOOL CFileEps::EPSCrDelFgets(char * buffer, int max, FILE * fp)
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
	buffer[idx] = '\0';
	return(TRUE);
}

void CFileEps::EpsBmpCreate(FILE *fp)
{
	char	exepath[FNAMEMAX];
	char	bmpfname[FNAMEMAX];
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
	
	xdot = 0;
	ydot = 0;
	sx = 0;
	sy = 0;
	ex = 0;
	ey = 0;
	bytecnt = 0;
	senvgetexepath(exepath);
	sprintf(bmpfname, "%s\\bmp%05d.bmp", exepath, m_pcView->m_nBmpCnt);
	(m_pcView->m_nBmpCnt)++;
	m_nRGBCrt = 0;
	while(1){
		if(EPSCrDelFgets(buffer, RECMAX, fp) == FALSE){
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
			sy = a23;
			ex = a13 + (DBL)(xdot*a11+ydot*a21);
			ey = a23 - (DBL)(xdot*a12+ydot*a22);
			if(a21 != 0 && a22 != 0){
				m_nRound = ON;
			}
			else{
				m_nRound = OFF;
			}
			TrnsPointToMMXY(&sx, &sy);
			TrnsPointToMMXY(&ex, &ey);

			if(EPSCrDelFgets(buffer, RECMAX, fp) == FALSE){
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
	
	EpsBmpFileInit(bmpfname, (int)xdot, (int)ydot);

	EPSCrDelFgets(buffer, RECMAX, fp);
	idx = 1;

	for(i = 0;i < xdot * ydot;i++){
	// 文字列から１ピクセル分取り出す
		idx2 = 0;
		while(idx2 < bytecnt){
		// チェックして補充
			if(buffer[idx] < 0x10){
				EPSCrDelFgets(buffer2, RECMAX, fp);
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
			RENGE(colors[0], 0, 255);
			RENGE(colors[1], 0, 255);
			RENGE(colors[2], 0, 255);
			EpsBmpFileBitSet(colors[0]);
			EpsBmpFileBitSet(colors[1]);
			EpsBmpFileBitSet(colors[2]);
			break;
		}
	}

	EPSCrDelFgets(buffer, RECMAX, fp);	//XH
	EPSCrDelFgets(buffer, RECMAX, fp);	//%AI5_EndRaster

	EpsBmpFileEnd();
	CDibCData* pcDibCData = (CDibCData*)(m_pcDoc->NewData(DIBCDATAID));
	if(pcDibCData == NULL){
		return;
	}
	pcDibCData->SetBmpFname(bmpfname);
	pcDibCData->DibLoad();
	pcDibCData->SetDstElement(sx, ey, ex-sx, sy-ey);
	pcDibCData->RenewMiniMax();
	pcDibCData->SetDstElement(sx, ey, ex-sx, sy-ey);
	pcDibCData->RenewMiniMax();
	if(pcDibCData != NULL){
		m_pcCrtDataGrup->SetData((CData*)pcDibCData);
	}
}

void CFileEps::EpsBmpFileBitSet(UINT tt)
{
	switch(m_nRGBCrt){
	case 0:	
		m_szRGB[2] = tt;
		m_nRGBCrt++;
		break;
	case 1:
		m_szRGB[1] = tt;
		m_nRGBCrt++;
		break;
	case 2:
		m_szRGB[0] = tt;
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

	mx = *x;
	my = *y;
	mx = mx * MMPARINTI / 72.0;
	my = my * MMPARINTI / 72.0;
	*x = mx;
	*y = my; 
}

void CFileEps::TrnsPointToMMLen(LPDBL len)
{
	DBL	mlen;

	mlen = *len;
	*len = mlen * MMPARINTI / 72.0;
}

void CFileEps::SetDibCData(CDataFDib *pcDataFDib, CDataGrup *pcDataGrup)
{
	DBL		sx, sy, ex, ey;
	DBL		sizex, sizey;
	DBL		timex, timey;
	DBL		movex, movey;
	int		nBmode;
	DBL		dBtime;
	DBL		dBmvx, dBmvy;
	DBL		dBsx, dBsy;
	char	bmpfname[FNAMEMAX];
	FILE	*fp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	long	dwsize;
	int		ret;
	CDibCData	*pcDibCData;

	pcDataFDib->GetDMiniMax(&sx, &sy, &ex, &ey);
	sizex = ex - sx;
	sizey = ey - sy;
	timex = sizex / CLIPBMPXSIZE;
	timey = sizey / CLIPBMPYSIZE;
	movex = -sx;
	movey = -sy;

	scrngetrltvtop(&dBsx, &dBsy);
	scrngetdisprltvmove(&dBmvx, &dBmvy);
	nBmode = senvsetdrawmode(DIRCTVIEW);
	dBtime = scrnsetxytime(timex, timey);
	scrnsetrltvelement();
	scrnsetdisprltvmove(0.0, 0.0);
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0, 0, CLIPBMPXSIZE, CLIPBMPYSIZE);

	m_cdcClipMemCDC.BitBlt(0, 0, CLIPBMPXSIZE, CLIPBMPYSIZE, NULL, 0, 0, WHITENESS);
	pcDataGrup->GetDMiniMax(&sx, &sy, &ex, &ey);
	sizex = ex - sx;
	sizey = ey - sy;
	pcDataGrup->MoveTrns(movex, movey);
	m_cdcClipMemCDC.SetROP2(R2_COPYPEN);
	pcDataGrup->OnDraw(&m_cdcClipMemCDC);

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = CLIPBMPXSIZE;
	bmpinfo.bmiHeader.biHeight = CLIPBMPYSIZE;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lClipBitsSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;
	m_lpClipBits = (BYTE *) GlobalLock(m_hClipBits);
	ret = ::GetDIBits(m_cdcClipMemCDC.m_hDC,(HBITMAP)(m_cbmpClipCBitmap.m_hObject),0,CLIPBMPYSIZE,m_lpClipBits,&bmpinfo,DIB_RGB_COLORS);

	diblibcreatebmpfile(bmpfname);
	if((fp = fopen(bmpfname, "wb")) != NULL){
		dwsize = (long)(sizeof(BITMAPFILEHEADER))
				 + (long)(sizeof(BITMAPINFOHEADER))
				 + m_lClipBitsSize;
		bmpfh.bfType = 0x4d42;
		bmpfh.bfSize = dwsize;
		bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

		fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fp);
		fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);
		fwrite(m_lpClipBits, m_lClipBitsSize, 1, fp);
		fclose(fp);

		pcDibCData = (CDibCData *)m_pcDoc->NewData(DIBCDATAID);
		pcDibCData->SetBmpFname(bmpfname);
		pcDibCData->DibReload();
		pcDibCData->SetDstElement(sx, sy, sizex, sizey);

		pcDataFDib->m_pcDibCData = pcDibCData;
	}
	if(m_lpClipBits != NULL){
		GlobalUnlock(m_hClipBits);
	}

	senvsetdrawmode(nBmode);
	scrnsetxytime(dBtime, dBtime);
	scrnsetrltvelement();
	scrnsetrltvtop(dBsx, dBsy);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetdisprltvmove(dBmvx, dBmvy);
}

void CFileEps::MemDCInit()
{
	int		ret;
	CRect	rectClip;

	m_lClipXByte = (CLIPBMPXSIZE * 3 + 3) / 4L * 4L;
	BeginDraw();
	DrawModeSET();
	m_nClipBmpStat = m_cbmpClipCBitmap.CreateCompatibleBitmap(m_pcDC, CLIPBMPXSIZE, CLIPBMPYSIZE);
	ret = m_cdcClipMemCDC.CreateCompatibleDC(m_pcDC);
	EndDraw();
	m_pcClipBackbmp = (CBitmap*)m_cdcClipMemCDC.SelectObject(&m_cbmpClipCBitmap);
	m_crgnClipClip.CreateRectRgn(0, 0, CLIPBMPXSIZE, CLIPBMPYSIZE);
	m_cdcClipMemCDC.SelectClipRgn(&m_crgnClipClip);
	m_cdcClipMemCDC.GetClipBox(&rectClip);
	m_lClipBitsSize = m_lClipXByte * CLIPBMPYSIZE;
	m_hClipBits = GlobalAlloc(GHND, m_lClipBitsSize+256);

	m_lGraXByte = (GRABMPXSIZE * 3 + 3) / 4L * 4L;
	BeginDraw();
	DrawModeSET();
	m_nGraBmpStat = m_cbmpGraCBitmap.CreateCompatibleBitmap(m_pcDC, GRABMPXSIZE, GRABMPYSIZE);
	ret = m_cdcGraMemCDC.CreateCompatibleDC(m_pcDC);
	EndDraw();
	m_pcGraBackbmp = (CBitmap*)m_cdcGraMemCDC.SelectObject(&m_cbmpGraCBitmap);
	m_crgnGraClip.CreateRectRgn(0, 0, GRABMPXSIZE, GRABMPYSIZE);
	m_cdcGraMemCDC.SelectClipRgn(&m_crgnGraClip);
	m_cdcGraMemCDC.GetClipBox(&rectClip);
	m_lGraBitsSize = m_lGraXByte * GRABMPYSIZE;
	m_hGraBits = GlobalAlloc(GHND, m_lGraBitsSize+256);
}

void CFileEps::MemDCEnd()
{
	if(m_hGraBits != 0){
		GlobalFree(m_hGraBits);
	}
	m_crgnGraClip.DeleteObject();
	m_cdcGraMemCDC.SelectObject(m_pcGraBackbmp);
	m_cbmpGraCBitmap.DeleteObject();
	m_cdcGraMemCDC.DeleteDC();

	if(m_hClipBits != 0){
		GlobalFree(m_hClipBits);
	}
	m_crgnClipClip.DeleteObject();
	m_cdcClipMemCDC.SelectObject(m_pcClipBackbmp);
	m_cbmpClipCBitmap.DeleteObject();
	m_cdcClipMemCDC.DeleteDC();
}

void CFileEps::SetAddDataGrup()
{
	if(m_nPassFlag == ON){
		return;
	}
	if(m_pcCrtDataList != NULL){
		if(m_nQIdx != 0){
			m_pcCrtDataList->m_nOutLinePen = m_pcCrtDataList->m_nFillPen;
		}
		m_pcCrtDataGrup->SetData((CData*)m_pcCrtDataList);
		m_pcCrtDataList = NULL;
	}
}

void CFileEps::SetAddClipGrup()
{
	if(m_pcCrtDataList != NULL){
		if(m_nQIdx != 0){
			m_pcCrtClipGrup->SetData((CData*)m_pcCrtDataList);
		}
		else{
			m_pcCrtDataGrup->SetData((CData*)m_pcCrtDataList);
		}
		m_pcCrtDataList = NULL;
	}
}

void CFileEps::CreateBmpFile(LPSTR bmpfname)
{
	long	dwsize;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	FILE	*fp;

	m_lGraXByte = (GRABMPXSIZE * 3 + 3) / 4L * 4L;
	m_lGraBitsSize = m_lGraXByte * GRABMPYSIZE;
	dwsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + m_lGraBitsSize;
	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = dwsize;
	bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = GRABMPXSIZE;
	bmpinfo.bmiHeader.biHeight = GRABMPYSIZE;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lGraBitsSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	if((fp = fopen(bmpfname, "wb")) != NULL){
		fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fp);
		fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);
		fwrite(m_lpGraBits, m_lGraBitsSize, 1, fp);
		fclose(fp);
	}
}

LPSTR CFileEps::SetGraName(LPSTR cmdstr, LPSTR graname)
{
	LPSTR	sp;
	LPSTR	setsp;

	sp = cmdstr;
	while(1){
		if(*sp == '\0'){
			return(NULL);
		}
		if(*sp == '('){
			break;
		}
		sp++;
	}
	setsp = graname;
	while(1){
		if(*sp == '\0'){
			break;
		}
		*setsp = *sp;
		if(*sp == ')'){
			setsp++;
			sp++;
			break;
		}
		setsp++;
		sp++;

	}
	*setsp = '\0';
	return(sp);
}

void CFileEps::EpsGraCommandLoop(FILE *fp, LPSTR cmdstr)
{
	int		backfillpenno;
	int		backlinepenno;
	DBL		backlinethick;
	DBL		thick;
	int		len;
	char	graname[RECMAX];
	DBL		sx, sy, rag, vlen;
	long	crtseekp;
	LPSTR	sp;

	len = strlen(cmdstr);
	switch(cmdstr[len-1]){
	case 'b':
			TrnsPointToMMXY(&m_dCrtx, &m_dCrty);
			m_pcCrtVect->SetAtrDXY(m_nPP, 0, m_dCrtx, m_dCrty);
			m_nPP++;
			SetVectData(m_pcCrtVect, m_nPP);
			m_pcCrtVect->OnFlag(REJIONFLAG);

			backlinepenno = senvgetcrtlinepenno();
			backlinethick = senvgetcrtlinethick();
			backfillpenno = senvgetcrtfillpenno();
			thick = 0;
			senvsetcrtfillpenno(backfillpenno);
			senvsetcrtlinepenno(backfillpenno);
			senvsetcrtlinethick(thick);
			m_pcCrtDataList->SetDefFillClr();
			m_pcCrtDataList->SetDefOutLineClr();
			senvsetcrtlinepenno(backlinepenno);
			senvsetcrtlinethick(backlinethick);
			senvsetcrtfillpenno(backfillpenno);
			m_pcCrtVect = NULL;
			break;
	case 'g':
		crtseekp= ftell(fp);
		sp = SetGraName(cmdstr, graname);
		SetGraClrTable(fp, graname);
		GetGraElement(sp, &sx, &sy, &rag, &vlen);
		CreateGraPattern(fp, sx, sy, rag, vlen);
		fseek(fp, crtseekp, 0);
		break;
	}
}

void CFileEps::GetGraElement(LPSTR prm, DBL *retsx, DBL *retsy, DBL *retrag, DBL *retvlen)
{
	DBL		sx, sy, ex, ey;
	DBL		xx, yy;
	DBL		rag, vlen;
	dmtxtype	mtx;

	sscanf(prm, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &sx, &sy, &rag, &vlen, &(mtx.a11), &(mtx.a12), &(mtx.a21), &(mtx.a22), &(mtx.a31), &(mtx.a32));
  	ex = sx + cos(rag*M_1RAG) * vlen;
	ey = sy + sin(rag*M_1RAG) * vlen;
	xx = sx; yy = sy;
	sx = xx * mtx.a11 - yy * mtx.a21 + mtx.a31;
	sy = -xx * mtx.a12 + yy * mtx.a22 - mtx.a32;
	xx = ex; yy = ey;
	ex = xx * mtx.a11 - yy * mtx.a21 + mtx.a31;
	ey = -xx * mtx.a12 + yy * mtx.a22 - mtx.a32;
	*retrag = atan2(ey - sy, ex - sx);
	*retvlen = hypot(ex - sx, ey - sy);
	TrnsPointToMMLen(retvlen);
	TrnsPointToMMXY(&sx, &sy);
	*retsx = sx; *retsy = sy; 
}

void CFileEps::CreateGraPattern(FILE *fp, DBL csx, DBL csy, DBL rag, DBL vlen)
{
	CDataFDib	*pcDataFDib;
	CDibCData	*pcDibCData;
	char	bmpfname[FNAMEMAX];
	int		x, y;
	DBL		sx, sy, ex, ey;
	DBL		sizex, sizey;
	DBL		dx, dy, dlen;
	DBL		aa, bb, cc, dd;
	DBL		signe;
	DBL		par;
	COLORREF	setclr;
	LPBYTE	sp;
	int		r, g, b;
//	int		ret;

	aa = 0;
	bb = 0;
	cc = 0;
	dd = 1.0;
	signe = 1;
	if(m_pcCrtDataList == NULL){
		return;
	}
	m_cdcGraMemCDC.BitBlt(0, 0, GRABMPXSIZE, GRABMPYSIZE, NULL, 0, 0, WHITENESS);
	m_lpGraBits = (BYTE *) GlobalLock(m_hGraBits);
	if(m_lpGraBits == NULL){
		return;
	}
	m_pcCrtDataList->GetDMiniMax(&sx, &sy, &ex, &ey);
	sizex = ex - sx; sizey = ey - sy;
	if(m_nGraKind == 0){
		aa = sin(rag+M_PI_2);
		bb = cos(rag+M_PI_2);
		cc = -(aa*csx+bb*csy);
		dd = sqrt(aa*aa+bb*bb);
		signe = ((sx+ex)/2 * aa + (sy+ey)/2 * bb + cc)/dd;
		if(signe < 0){
			signe = -1;
		}
		else{
			signe = 1;
		}
	}
	GetLPBits();
	for(y = 0; y < GRABMPYSIZE; y++){
		dy = (DBL)(GRABMPYSIZE - y) * sizey / GRABMPYSIZE + sy;
		for(x = 0; x < GRABMPXSIZE; x++){
			dx = (DBL)x * sizex / GRABMPXSIZE + sx;
			if(m_nGraKind == 1){
				dlen = hypot(csx - dx, csy - dy);
			}
			else{
				dlen = (aa*dx + bb*dy + cc) / dd * signe;
			}
			par = dlen / vlen * 100.0;
			if(par < 100){
				r = 0;
			}
			setclr = GetSetGraClr(par);
			r = GetRValue(setclr);
			g = GetGValue(setclr);
			b = GetBValue(setclr);
			RENGE(r, 0, 255);
			RENGE(g, 0, 255);
			RENGE(b, 0, 255);
			sp = m_lpGraBits + y * m_lGraXByte + x * 3;
			*sp++ = b;
			*sp++ = g;
			*sp = r;
		}
	}
	GlobalUnlock(m_hGraBits);

	pcDataFDib = (CDataFDib*)(m_pcDoc->NewData(DATAFDIBID));
	m_pcCrtDataGrup->SetData((CData*)pcDataFDib);
	pcDataFDib->SetData((CData*)m_pcCrtDataList);
	pcDibCData = (CDibCData *)m_pcDoc->NewData(DIBCDATAID);
	pcDataFDib->m_pcDibCData = pcDibCData;
	diblibcreatebmpfile(bmpfname);
	CreateBmpFile(bmpfname);
	pcDibCData->SetBmpFname(bmpfname);
	pcDibCData->DibReload();
	pcDibCData->SetDstElement(sx, sy, sizex, sizey);
	m_pcCrtDataList = NULL;
}

long CFileEps::GetSeekpoint(LPSTR graname)
{
	int		i;

	for(i = 0; i < m_nGraTblMax; i++){
		if(strcmp(m_szGraName[i], graname) == 0){
			return(m_lGraSeekPoint[i]);
		}
	}
	return(0L);
}

void CFileEps::SetGraClrTable(FILE *fp, LPSTR graname)
{
	long	seekp;
	char	buffer[RECMAX];
	int		len;
	char	name[RECMAX];
	LPSTR	sp;
	int		idx, i;
	char	str[16][16];
	DBL		c, m, y, k;
	int		r, g, b;
	COLORREF	clrGra[GRAPOINTMAX];
	DBL		dCGra[GRAPOINTMAX];
	DBL		dPGra[GRAPOINTMAX];

	seekp = GetSeekpoint(graname);
	if(seekp == 0){
		return;
	}
	fseek(fp, seekp, 0);
	idx = 0;
	m_nPcnt = 0;
	while(1){
		if(EPSCrDelFgets(buffer, RECMAX, fp) == FALSE){
			break;
		}
		len = strlen(buffer);
		if(buffer[len-2] == 'B' && buffer[len-1] == 'd'){
			sp = SetGraName(buffer, name);
			sscanf(sp, "%d %d", &m_nGraKind, &m_nPcnt);
			idx = m_nPcnt-1;
		}
		else if(buffer[len-2] == 'B' && buffer[len-1] == 's'){
			if(buffer[0] == '%'){
				sp = &(buffer[2]);
			}
			else{
				sp = buffer;
			}
			sscanf(sp, "%s %s %s %s %s %s %s %s", 
				str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7]);
			if(str[4][0] == '%'){
				c = 0; m = 0; y = 0;
				k = 1.0 - atof(str[0]);
				m_dCGra[idx] = atof(str[2]);
				m_dPGra[idx] = atof(str[3]);
			}
			else{
				c = atof(str[0]); m = atof(str[1]);
				y = atof(str[2]); k = atof(str[3]);
				m_dCGra[idx] = atof(str[5]);
				m_dPGra[idx] = atof(str[6]);
			}
			r = 255 - (int)((c + k) * 255.0);
			g = 255 - (int)((m + k) * 255.0);
			b = 255 - (int)((y + k) * 255.0);
			RENGE(r, 0, 255);
			RENGE(g, 0, 255);
			RENGE(b, 0, 255);
			m_clrGra[idx] = RGB(r, g, b);
			idx--;
		}
		if(strncmp(buffer, "%AI5_EndGradient", 16) == 0){
			if(m_dPGra[0] > m_dPGra[m_nPcnt-1]){
				for(i = 0; i < m_nPcnt; i++){
					clrGra[m_nPcnt-i-1] = m_clrGra[i];
					dCGra[m_nPcnt-i-1] = m_dCGra[i];
					dPGra[m_nPcnt-i-1] = m_dPGra[i];
				}
				for(i = 0; i < m_nPcnt; i++){
					m_clrGra[i] = clrGra[i];
					m_dCGra[i] = dCGra[i];
					m_dPGra[i] = dPGra[i];
				}
			}
			return;
		}
	}
}

COLORREF CFileEps::GetSetGraClr(DBL par)
{
	int		i;
	DBL		sr, sg, sb;
	DBL		er, eg, eb;
	DBL		r, g, b;

	if(par <= m_dPGra[0]){
		return(m_clrGra[0]);
	}
	if(m_dPGra[m_nPcnt-1] <= par){
		return(m_clrGra[m_nPcnt-1]);
	}
	for(i = 0; i < m_nPcnt-1; i++){
		if(m_dPGra[i] <= par && par <= m_dPGra[i+1]){
			sr = GetRValue(m_clrGra[i]);
			sg = GetGValue(m_clrGra[i]);
			sb = GetBValue(m_clrGra[i]);
			er = GetRValue(m_clrGra[i+1]);
			eg = GetGValue(m_clrGra[i+1]);
			eb = GetBValue(m_clrGra[i+1]);
			par = (par - m_dPGra[i]) / (m_dPGra[i+1] - m_dPGra[i]) * 100;
			if(par < 50.0){
				par = par *  m_dCGra[i] / 50.0;
			}
			else{
				par = par * (100 - m_dCGra[i]) / 50;
			}
			r = (er-sr)*par/100.0 + sr;
			g = (eg-sg)*par/100.0 + sg;
			b = (eb-sb)*par/100.0 + sb;
			RENGE(r, 0, 255);
			RENGE(g, 0, 255);
			RENGE(b, 0, 255);
			return(RGB(r, g, b));
		}
	}
	return(m_clrGra[m_nPcnt-1]);
}

void CFileEps::GetLPBits()
{
	BITMAPINFO	bmpinfo;
	int		ret;

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = GRABMPXSIZE;
	bmpinfo.bmiHeader.biHeight = GRABMPYSIZE;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lGraBitsSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;
	ret = ::GetDIBits(m_cdcGraMemCDC.m_hDC,(HBITMAP)(m_cbmpGraCBitmap.m_hObject),0,GRABMPYSIZE,m_lpGraBits,&bmpinfo,DIB_RGB_COLORS);
}

void CFileEps::ClrRGBTrns(DBL r, DBL g, DBL b, int *rval, int *gval, int *bval)
{
	DBL	ry1x1, ry1x2, ry2x1, ry2x2, ry1, ry2;
	DBL	gy1x1, gy1x2, gy2x1, gy2x2, gy1, gy2;
	DBL	by1x1, by1x2, by2x1, by2x2, by1, by2;

	CTonApp *cpApp = (CTonApp*)AfxGetApp();
	CMainFrame *cpWnd = (CMainFrame*)cpApp->GetMainWnd();
	if(cpWnd->m_nEpsClrTrns == ON){
		// 000 000 000   019 000 124
		//(000 000 000) (000 000 255)
		// 000 136 055   000 160 198
		//(000 255 000) (000 255 255)
		//
		// 255 000 000   240 002 127
		//(255 000 000) (255 000 255)
		// 255 255 000   255 255 255
		//(255 255 000) (255 255 255)
 
		ry1x1 = (19.0-0.0) / 255.0 * b + 0.0;
		ry2x1 = (0.0-0.0) / 255.0 * b + 0.0;
		ry1x2 = (240.0-255.0) / 255.0 * b + 255.0;
		ry2x2 = (255.0-255.0) / 255.0 * b + 255.0;
		ry1 = (ry2x1-ry1x1) / 255.0 * g + ry1x1;
		ry2 = (ry2x2-ry1x2) / 255.0 * g + ry1x2;
		*rval = (int)((ry2-ry1) / 255.0 * r + ry1);

		gy1x1 = (0.0-0.0) / 255.0 * b + 0.0;
		gy2x1 = (160.0-136.0) / 255.0 * b + 136.0;
		gy1x2 = (2.0-0.0) / 255.0 * b + 0.0;
		gy2x2 = (255.0-255.0) / 255.0 * b + 255.0;
		gy1 = (gy2x1-gy1x1) / 255.0 * g + gy1x1;
		gy2 = (gy2x2-gy1x2) / 255.0 * g + gy1x2;
		*gval = (int)((gy2-gy1) / 255.0 * r + gy1);

		by1x1 = (124.0-0.0) / 255.0 * b + 0.0;
		by2x1 = (198.0-55.0) / 255.0 * b + 55.0;
		by1x2 = (127.0-0.0) / 255.0 * b + 0.0;
		by2x2 = (255.0-0.0) / 255.0 * b + 0.0;
		by1 = (by2x1-by1x1) / 255.0 * g + by1x1;
		by2 = (by2x2-by1x2) / 255.0 * g + by1x2;
		*bval = (int)((by2-by1) / 255.0 * r + by1);
	}
	else{
		*rval = (int)r;
		*gval = (int)g;
		*bval = (int)b;
	}
	RENGE((*rval), 0, 255);
	RENGE((*gval), 0, 255);
	RENGE((*bval), 0, 255);
}

