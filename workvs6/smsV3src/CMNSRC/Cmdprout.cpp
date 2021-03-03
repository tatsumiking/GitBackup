
/////////////////////////////////////////////////////////////////////////////
// CCmdPrint

#include "stdafx.h"
#include "stdio.h"
#include "math.h"

#include "cmd_base.h"
#include "dldirect.h"
#include "dsnddata.h"
#include "cmdprout.h"

#define PRINTSPACE	8		// def 10
#define PRINTBLOCK	256		// def 256
#define PRINTXMAX	10000	// def 15000

extern "C" {
	void senvgetexepath(LPSTR path);
	void senvgetdatpath(LPSTR path);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	DBL scrnsetxytime(DBL timex, DBL timey);
	void scrngetxytime(LPDBL timex, LPDBL timey);
	void scrnsetrltvelement();
	void scrnsetrltvtop(DBL xTop, DBL yTop);
	void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
	void scrnsetdisprltvmove(DBL xmove, DBL ymove);
	void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove);
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void  scrndxymmtorltv(LPDBL x, LPDBL y);
	void  scrndlenmmtorltv(LPDBL len);
	void scrnonnoactflag();
	void scrnoffnoactflag();
	int senvsetdrawmode(int mode);
	int senvchckflag(int flag);
	void senvonflag(int flag);
	void senvoffflag(int flag);
	void clrprlibSetCmdKind(int kind);
	void clrprlibSetProcess(int process);
	void clrprlibSetClrTrnsElement(DBL rn, DBL gn, DBL bn, DBL rad, DBL gad, DBL bad, DBL rcr, DBL gcr, DBL bcr);
	void clrprlibSetStep(DBL step);
	void clrprlibSetDpi(int xdpi, int ydpi);
	void clrprlibInitClrTrns(DBL rnum, DBL gnum, DBL bnum, DBL radd, DBL gadd, DBL badd, DBL rcntrst, DBL gcntrst, DBL bcntrst);
	void clrprlibSetMaxSize(DBL xdotsize, DBL ydotsize);
	void clrprlibSetMMSize(DBL mmx, DBL mmy);
	void clrprlibTableInit(int kind);
	void clrprlibMemInit();
	void clrprlibMemEnd();
	void clrprlibOutPortOpen(int outflag);
	void clrprlibOutPortClose();
	void clrprlibOutStartCommand(DBL xdotsize, DBL ydotsize);
	void clrprlibPaintEndCommand();
	void clrprlibOutEndCommand();
	void clrprlibOutPageStartCommand(int clrno);
	void clrprlibOutPageEndCommand();
	void clrprlibOutPaintData(LPBYTE fullclrimg, int imgxsize, int xspace, int xcpycnt);
	void clrprlibOutFileTest();
	void dizaptnload(LPSTR fname);
	void dizaptnloadcam(LPSTR fname);
	void poutmmstartset(DBL sx, DBL sy);
	void poutarcflagget(int *flag);
	void poutarcflagset(int flag);
	void poutlinemodeget(int *flag);
	void poutlinemodeset(int flag);
	void poutfillflagset(int flag);
	void poutfillflagget(int *flag);
	void poutfillflagon();
	void poutfillflagoff();
	void pouttypeget(int *type);
	void pouttypeset(int type);
	void poutmodeset(int homemode, int linemode, int omitmode);
	void poutoutareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void poutstepoutareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void poutelementinit();
	void poutpaintinit(int nComID);
	void poutpaintend(int nComID);
	void poutplotinit(int nComID);
	void poutplotend(int nComID);
	void  poutarc(int nComID, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	int   poutareachk(DBL sx, DBL sy, DBL ex, DBL ey);
	void  poutbeze(int nComID, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	int   poutfillflagchk();
	void  poutfreecurve(int nComID, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void  poutline(int nComID, DBL sx, DBL sy, DBL ex, DBL ey);
	void  poutmove(int nComID, DBL dx, DBL dy);
	void  poutoutline(int nComID, DBL sx, DBL sy, DBL ex, DBL ey);
	void  poutrejionend(int nComID);
	void  poutrejionstart(int nComID);
	int   poutpaintmodechk();
	void plotmovexyset(DBL xmove, DBL ymove);
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrcurvechk(int atr1);
}

CCmdPrint::CCmdPrint(CScrollView* pcview)
:CCmdBase(pcview)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	char	buf[HRECMAX];
	FILE	*fp;

	m_nCmdKind = CAMJET;
	strcpy(m_szPaperName, "標準用紙");
	m_bChkX = TRUE;
	m_bChkY = FALSE;
	m_dXMMOut = 900;
	m_dYMMOut = 1800;
	m_dXMMPaper = 1300;
	m_dYMMPaper = 5000;
	m_dXMMNori = 100;
	m_dYMMNori = 100;
	m_nCopyCount = 1;
	m_nProcess = 1;
	m_nOutFlag = 0;
	m_bChkMark = 0;
	m_nPrintCut = 2;
	m_dSendTime = 1.0;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmddirect.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &m_nCmdKind);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", &m_szPaperName);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d %d", &m_bChkX, &m_bChkY);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf %lf", &m_dXMMOut, &m_dYMMOut);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf %lf", &m_dXMMPaper, &m_dYMMPaper);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf %lf", &m_dXMMNori, &m_dYMMNori);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &m_nCopyCount);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d %d %d %d", &m_nProcess, &m_nOutFlag, &m_bChkMark, &m_nPrintCut);
		CrDelFgets(buf, HRECMAX, fp);
		strcpy(m_szSaverPath, buf);
		CrDelFgets(buf, HRECMAX, fp);
		strcpy(m_szSendPath, buf);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf", &m_dSendTime);
		fclose(fp);
	}
}

CCmdPrint::~CCmdPrint()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmddirect.env", exepath);
	if((fp = fopen(fname, "wr")) != NULL){
		fprintf(fp, "%d\n", m_nCmdKind);
		fprintf(fp, "%s\n", m_szPaperName);
		fprintf(fp, "%d %d\n", m_bChkX, m_bChkY);
		fprintf(fp, "%lf %lf\n", m_dXMMOut, m_dYMMOut);
		fprintf(fp, "%lf %lf\n", m_dXMMPaper, m_dYMMPaper);
		fprintf(fp, "%lf %lf\n", m_dXMMNori, m_dYMMNori);
		fprintf(fp, "%d\n", m_nCopyCount);
		fprintf(fp, "%d %d %d %d\n", m_nProcess, m_nOutFlag, m_bChkMark, m_nPrintCut);
		fprintf(fp, "%s\n", m_szSaverPath);
		fprintf(fp, "%s\n", m_szSendPath);
		fprintf(fp, "%lf\n", m_dSendTime);
		fclose(fp);
	}
	
}

UINT CCmdPrint::ExecSendData(UINT event, DBL dx, DBL dy)
{
	char	essfname[FNAMEMAX];
	char	xxxfname[FNAMEMAX];
	char	tmpfname[FNAMEMAX];
	char	envfname[FNAMEMAX];

	if(event != INITEVENT){
		return(0);
	}

	if(SetDialogElement() == FALSE){
		return(ENDEVENT);
	}

	sprintf(essfname, "%s\\ess%04d.ess", m_szSendPath, m_pcView->m_nBmpCnt);
	if(SaveEssData(essfname) == FALSE){
		unlink(essfname);
		return(ENDEVENT);
	}

	sprintf(xxxfname, "%s\\ess%04d", m_szSendPath, m_pcView->m_nBmpCnt);
	if(SaveXXXData(xxxfname) == FALSE){
		unlink(essfname);
		return(ENDEVENT);
	}
	
	sprintf(tmpfname, "%s\\ess%04d.tmp", m_szSendPath, m_pcView->m_nBmpCnt);
	if(SaveEnvData(tmpfname) == FALSE){
		unlink(essfname);
		unlink(tmpfname);
		return(ENDEVENT);
	}
	sprintf(envfname, "%s\\ess%04d.env", m_szSendPath, m_pcView->m_nBmpCnt);
	unlink(envfname);
	if(rename(tmpfname, envfname)){
		unlink(essfname);
		unlink(tmpfname);
		return(ENDEVENT);
	}

	(m_pcView->m_nBmpCnt)++;
	return(ENDEVENT);
}

UINT CCmdPrint::ExecOutData(UINT event, DBL dx, DBL dy)
{
	char	fname[FNAMEMAX];
	FILE	*fp;

	if(event != INITEVENT){
		return(0);
	}
	sprintf(fname, "%s\\%s", m_szSaverPath, m_pcView->m_szSaverSW);
	if((fp = fopen(fname, "w")) == NULL){
		return(ENDEVENT);
	}
	fprintf(fp, "1\n");
	fclose(fp);

	return(ENDEVENT);
}

UINT CCmdPrint::ExecStopData(UINT event, DBL dx, DBL dy)
{
	char	fname[FNAMEMAX];
	FILE	*fp;

	if(event != INITEVENT){
		return(0);
	}
	sprintf(fname, "%s\\%s", m_szSaverPath, m_pcView->m_szSaverSW);
	if((fp = fopen(fname, "w")) == NULL){
		return(ENDEVENT);
	}
	fprintf(fp, "8\n");
	fclose(fp);

	return(ENDEVENT);
}

UINT CCmdPrint::ExecPrintOut(UINT event, DBL dx, DBL dy)
{
	CDialogDirectOut *pDialogDirectOut;
	DBL		xmm, ymm;
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];

	if(event != INITEVENT){
		return(0);
	}
	// ダイアログで出力機器　のりしろ　紙サイズ　

	m_pcApp = (CTonApp*)AfxGetApp();
	m_pcWnd = (CMainFrame*)m_pcApp->GetMainWnd();
	m_pcDoc = m_pcView->GetDocument();
	pDialogDirectOut = new CDialogDirectOut(m_pcView);
	pDialogDirectOut->m_nCmdKind = m_nCmdKind;
	strcpy(pDialogDirectOut->m_szPaperName, m_szPaperName);
	pDialogDirectOut->m_bChkX = m_bChkX;
	pDialogDirectOut->m_bChkY = m_bChkY;
	pDialogDirectOut->m_dXMMOut = m_dXMMOut;
	pDialogDirectOut->m_dYMMOut = m_dYMMOut;
	pDialogDirectOut->m_dXMMPaper = m_dXMMPaper;
	pDialogDirectOut->m_dYMMPaper = m_dYMMPaper;
	pDialogDirectOut->m_dXMMNori = m_dXMMNori;
	pDialogDirectOut->m_dYMMNori = m_dYMMNori;
	pDialogDirectOut->m_nCopyCount = m_nCopyCount;
	pDialogDirectOut->m_nProcess = m_nProcess;
	pDialogDirectOut->m_nOutFlag = m_nOutFlag;
	pDialogDirectOut->m_bChkMark = m_bChkMark;
	pDialogDirectOut->m_nPrintCut = m_nPrintCut;
	pDialogDirectOut->m_dSendTime = m_dSendTime;
	if(pDialogDirectOut->DoModal() == IDOK){
		m_nCmdKind = pDialogDirectOut->m_nCmdKind;
		strcpy(m_szPaperName, pDialogDirectOut->m_szPaperName);
		m_nXdpi = pDialogDirectOut->m_nXdpi;
		m_nYdpi = pDialogDirectOut->m_nYdpi;
		m_dStep = pDialogDirectOut->m_dStep;
		m_bChkX = pDialogDirectOut->m_bChkX;
		m_bChkY = pDialogDirectOut->m_bChkY;
		m_dXMMOut = pDialogDirectOut->m_dXMMOut;
		m_dYMMOut = pDialogDirectOut->m_dYMMOut;
		m_dXMMPaper = pDialogDirectOut->m_dXMMPaper;
		m_dYMMPaper = pDialogDirectOut->m_dYMMPaper;
		m_dXMMNori = pDialogDirectOut->m_dXMMNori;
		m_dYMMNori = pDialogDirectOut->m_dYMMNori;
		m_dSendTime = pDialogDirectOut->m_dSendTime;
		m_nCopyCount = pDialogDirectOut->m_nCopyCount;
		m_dRgnum = pDialogDirectOut->m_dRgnum;
		m_dGgnum = pDialogDirectOut->m_dGgnum;
		m_dBgnum = pDialogDirectOut->m_dBgnum;
		m_dRadd = pDialogDirectOut->m_dRadd;
		m_dGadd = pDialogDirectOut->m_dGadd;
		m_dBadd = pDialogDirectOut->m_dBadd;
		m_dRcntrst = pDialogDirectOut->m_dRcntrst;
		m_dGcntrst = pDialogDirectOut->m_dGcntrst;
		m_dBcntrst = pDialogDirectOut->m_dBcntrst;
		m_nProcess = pDialogDirectOut->m_nProcess;
		m_nOutFlag = pDialogDirectOut->m_nOutFlag;
		m_bChkMark = pDialogDirectOut->m_bChkMark;
		m_nPrintCut = pDialogDirectOut->m_nPrintCut;
	}
	else{
		return(ENDEVENT);
	}

	((CTonView*)m_pcView)->m_nDrawIn = ON;
	m_pcDataList = NULL;
	senvgetexepath(exepath);
	if(m_nCmdKind == CLRCAM){
		sprintf(fname, "%s\\dizacam%d.env", exepath, m_nProcess);
		dizaptnloadcam(fname);
	}
	else{
		sprintf(fname, "%s\\dizaorg%d.env", exepath, m_nProcess);
		dizaptnload(fname);
	}
	scrngetmmsize(&xmm, &ymm);
	m_dXTime = ((DBL)m_nXdpi / MMPARINTI) * (m_dXMMOut / xmm);
	m_dYTime = ((DBL)m_nYdpi / MMPARINTI) * (m_dYMMOut / ymm);
	// 出力用紙の有効出力範囲
	m_dXDotMax = m_dXMMPaper * ((DBL)m_nXdpi / MMPARINTI);
	m_dYDotMax = m_dYMMPaper * m_dSendTime * ((DBL)m_nYdpi / MMPARINTI);
	m_dXDotOut = m_dXMMOut * ((DBL)m_nXdpi / MMPARINTI);
	m_dYDotOut = m_dYMMOut * m_dSendTime * ((DBL)m_nYdpi / MMPARINTI);
	m_dXDotNori = m_dXMMNori * ((DBL)m_nXdpi / MMPARINTI);
	m_dYDotNori = m_dYMMNori * m_dSendTime * ((DBL)m_nYdpi / MMPARINTI);
	m_dMarkDot = 6.0 * ((DBL)m_nYdpi / MMPARINTI);
	senvoffflag(STOPFLAG);
	if(m_nPrintCut == 0){
		if(m_bChkMark == TRUE){
			m_dMarkAddX = MMPARINTI / 2.0 * (xmm / m_dXMMOut);
			m_dMarkAddY = MMPARINTI / 2.0 * (ymm / (m_dYMMOut*m_dSendTime));
			m_pcDoc->MoveTrns(m_dMarkAddX, -m_dMarkAddY);
		}
	}
	m_pcWnd->HelpTextOut(2, "初期化処理中");
	clrprlibSetCmdKind(m_nCmdKind);
	clrprlibSetClrTrnsElement(m_dRgnum, m_dGgnum, m_dBgnum, m_dRadd, m_dGadd, m_dBadd, m_dRcntrst, m_dGcntrst, m_dBcntrst);
	clrprlibSetStep(m_dStep);
	clrprlibSetProcess(m_nProcess);
	clrprlibSetDpi(m_nXdpi, m_nYdpi);
	clrprlibSetMaxSize(m_dXDotMax, m_dXDotMax);
	clrprlibMemInit();	// ダイレクト印刷出力メモリーの確保
	clrprlibTableInit(m_nCmdKind);
	clrprlibOutPortOpen(m_nOutFlag);		// ポートのオープン

	MemDCInit((int)m_dXDotMax, (int)(PRINTBLOCK+PRINTSPACE*2));

	if(m_dXMMPaper < m_dXMMOut){
		// 分割出力
		DivPrintOut();
	}
	else{
		// 割付出力
		CopyPrintOut();
	}

	MemDCEnd();

	clrprlibOutPortClose();
	clrprlibMemEnd();	// ダイレクト印刷出力メモリーの開放

	if(m_nPrintCut == 0){
		if(m_bChkMark == TRUE){
			if(m_pcDataList != NULL){
				m_pcDoc->DeleteData(m_pcDataList);
				delete(m_pcDataList);
			}
			m_pcDoc->MoveTrns(-m_dMarkAddX, m_dMarkAddY);
		}
	}
	((CTonView*)m_pcView)->m_nDrawIn = OFF;
	return(ENDEVENT);
}

void CCmdPrint::CopyPrintOut()
{
	DBL		dotsx, dotsy;
	DBL		dotex, dotey;
	int		xcpycnt;
	int		divcnt, divrest;
	int		ycnt;
	int		ret;

	if(m_nCopyCount == 1){
		dotsx = 0;
		dotsy = 0;
		dotex = m_dXDotMax;
		dotey = m_dYDotOut;
		if(m_nPrintCut == 0){
			if(m_bChkMark == TRUE){
				dotey += m_dMarkDot*6.0;
				CreateMark(m_dMarkAddX, m_dMarkAddY);
			}
		}
		ret = OutBlockData(dotsx, dotsy, dotex, dotey, 1);
		if(ret == FALSE){
			goto ENDCPYPRN;
		}
	}
	else{
		dotsx = 0;
		dotsy = 0;
		dotex = m_dXDotOut+m_dXDotNori;
		dotey = m_dYDotOut+m_dYDotNori;
		xcpycnt = (int)((m_dXDotMax + m_dXDotNori) / (m_dXDotOut+m_dXDotNori)); 
		divcnt = m_nCopyCount / xcpycnt;
		divrest = m_nCopyCount % xcpycnt;
		for(ycnt = 0; ycnt < divcnt; ycnt++){
			ret = OutBlockData(dotsx, dotsy, dotex, dotey, xcpycnt);
			if(ret == FALSE){
				goto ENDCPYPRN;
			}
		}
		if(divrest != 0){
			ret = OutBlockData(dotsx, dotsy, dotex, dotey, divrest);
			if(ret == FALSE){
				goto ENDCPYPRN;
			}
		}
	}
ENDCPYPRN:
	return;
}

void CCmdPrint::DivPrintOut()
{
	DBL		xdotrest, ydotrest;
	DBL		dotsx, dotsy;
	DBL		dotex, dotey;
	DBL		xdivmax, ydivmax;
	DBL		xdiv, ydiv;
	int		ret;

	// ノリシロ付き出力回数の計算
	xdivmax = (long)(m_dXDotOut) / (long)(m_dXDotMax - m_dXDotNori);
	xdotrest = (long)(m_dXDotOut) % (long)(m_dXDotMax - m_dXDotNori);
	ydivmax = (long)(m_dYDotOut) / (long)(m_dYDotMax - m_dYDotNori);
	ydotrest = (long)(m_dYDotOut) % (long)(m_dYDotMax - m_dYDotNori);
	if(xdotrest <= m_dXDotNori){
		// 余りがノリシロのみの出力になるため出力しない
		xdotrest = 0;
	}
	if(ydotrest <= m_dYDotNori){
		// 余りがノリシロのみの出力になるため出力しない
		ydotrest = 0;
	}
	// ノリシロ単位の出力ＬＯＯＰ処理
	for(xdiv = 0; xdiv < xdivmax; xdiv++){
		if(xdiv == 0){
			dotsx = 0;
		}
		else{
			dotsx = (m_dXDotMax - m_dXDotNori) * xdiv;
		}
		dotex = dotsx + m_dXDotMax;
		for(ydiv = 0; ydiv < ydivmax; ydiv++){
			if(ydiv == 0){
				dotsy = 0;
			}
			else{
				dotsy = (m_dYDotMax - m_dYDotNori) * ydiv;
			}
			dotey = dotsy + m_dYDotMax;
			ret = OutBlockData(dotsx, dotsy, dotex, dotey, 1);
			if(ret == FALSE){
				goto ENDDIVPRN;
			}
		}
		// Ｙ方向の残り出力
		if(ydotrest != 0){
			dotsy = (m_dYDotMax - m_dYDotNori) * ydiv;
			dotey = dotsy + ydotrest;
			ret = OutBlockData(dotsx, dotsy, dotex, dotey, 1);
			if(ret == FALSE){
				goto ENDDIVPRN;
			}
		}
	}
	// Ｘ方向の残り出力
	if(xdotrest != 0){
		dotsx = (m_dXDotMax - m_dXDotNori) * xdiv;
		dotex = dotsx + xdotrest;
		for(ydiv = 0; ydiv < ydivmax; ydiv++){
			if(ydiv == 0){
				dotsy = 0;
			}
			else{
				dotsy = (m_dYDotMax - m_dYDotNori)  * ydiv;
			}
			dotey = dotsy + m_dYDotMax;
			ret = OutBlockData(dotsx, dotsy, dotex, dotey, 1);
			if(ret == FALSE){
				goto ENDDIVPRN;
			}
		}
		if(ydotrest != 0){
			dotsy = (m_dYDotMax - m_dYDotNori)  * ydiv;
			dotey = dotsy + ydotrest;
			ret = OutBlockData(dotsx, dotsy, dotex, dotey, 1);
			if(ret == FALSE){
				goto ENDDIVPRN;
			}
		}
	}
ENDDIVPRN:
	return;
}

int CCmdPrint::OutBlockData(DBL dotsx, DBL dotsy, DBL dotex, DBL dotey, int cpycnt)
{
	int		divmax, divrest;
	DBL		sx, sy, ex, ey;
	int		div;
	DBL		xmm, ymm;
	int		clrno;
	int		idx[4], idxno;
	int		ret;

	ret = TRUE;
	xmm = m_dXMMPaper;
	ymm = m_dYMMPaper * m_dSendTime;

	clrprlibSetMMSize(xmm, ymm);

	m_dXBlockSize = (dotex - dotsx) * (DBL)cpycnt;
	m_dYBlockSize = dotey - dotsy;

	clrprlibOutStartCommand(m_dXBlockSize, m_dYBlockSize);

	if(m_nPrintCut == 1){
		goto ENDBLOCK;
	}
	// 塗り潰し単位の出力ループ処理
	divmax = (long)m_dYBlockSize / PRINTBLOCK;
	divrest = (long)m_dYBlockSize % PRINTBLOCK;
	if(m_nCmdKind == CLRCAM){
		//idx[0] = 2; idx[1] = 1; idx[2] = 0;
		idx[0] = 0; idx[1] = 1; idx[2] = 2;
		for(idxno = 0; idxno < 3; idxno++){
			clrno = idx[idxno];
			clrprlibOutPageStartCommand(clrno);
			m_nCrtDotY = 0;
			sx = dotsx; ex = dotex;
			for(div = 0; div < divmax; div++){
				sy = dotsy + PRINTBLOCK * div;
				ey = sy + PRINTBLOCK;
				AreaPaintData(sx, sy-PRINTSPACE, ex, ey+PRINTSPACE);
				ret = OutPaintBlock(0.0, 0.0, (ex - sx), PRINTBLOCK+PRINTSPACE*2, cpycnt);
				if(ret == FALSE){
					goto LOOPENDBLOCK;
				}
			}
			if(divrest != 0){
				sy = dotsy + PRINTBLOCK * div;
				ey = sy + PRINTBLOCK;
				AreaPaintData(sx, sy-PRINTSPACE, ex, ey+PRINTSPACE);
				ret = OutPaintBlock(0.0, 0.0, (ex - sx), PRINTBLOCK+PRINTSPACE*2, cpycnt);
				if(ret == FALSE){
					goto LOOPENDBLOCK;
				}
			}

LOOPENDBLOCK:
			clrprlibOutPageEndCommand();
			if(ret == FALSE){
				goto ENDBLOCK;
			}
		}
	}
	else{
		m_nCrtDotY = 0;
		sx = dotsx; 
		ex = dotex;
		for(div = 0; div < divmax; div++){
			sy = dotsy + PRINTBLOCK * div;
			ey = sy + PRINTBLOCK;
			AreaPaintData(sx, sy-PRINTSPACE, ex, ey+PRINTSPACE);
			ret = OutPaintBlock(0.0, 0.0, (ex - sx), PRINTBLOCK+PRINTSPACE*2, cpycnt);
			if(ret == FALSE){
				goto ENDBLOCK;
			}
		}
		if(divrest != 0){
			sy = dotsy + PRINTBLOCK * div;
			ey = sy + PRINTBLOCK;
			AreaPaintData(sx, sy-PRINTSPACE, ex, ey+PRINTSPACE);
			ret = OutPaintBlock(0.0, 0.0, (ex - sx), PRINTBLOCK+PRINTSPACE*2, cpycnt);
			if(ret == FALSE){
				goto ENDBLOCK;
			}
		}
	}
ENDBLOCK:
	clrprlibPaintEndCommand();
	if(ret != FALSE){
		if(m_nPrintCut != 0){
			CutingDataOut(cpycnt);
		}
	}
	clrprlibOutEndCommand();
	if(ret == FALSE){
		return(FALSE);
	}
	return(TRUE);
}

void CCmdPrint::MemDCInit(int xdot, int ydot)
{
	int		ret;
	long	bytesize;
	int		i;

	m_nBMPYSize = ydot;
	for(i = 0; i < BMPDIVMAX; i++){
		m_nBMPXSize[i] = 0;
	}
	for(i = 0; i < BMPDIVMAX; i++){
		if(xdot < PRINTXMAX){
			m_nBMPXSize[i] = PRINTXMAX+PRINTSPACE*2;
			i++;
			break;
		}
		else{
			m_nBMPXSize[i] = PRINTXMAX+PRINTSPACE*2;
			xdot -= PRINTXMAX;
		}
	}
	m_nBmpDivMax = i;
	//bytesize = (xdot * 3 + 3) / 4L * 4L + 256;
	bytesize = (PRINTXMAX+PRINTSPACE*2) * m_nBmpDivMax * 3 + 1024;
	m_hOneLine = GlobalAlloc(GHND, bytesize);
	m_lpOneLine = (LPBYTE)GlobalLock(m_hOneLine);

	BeginDraw();
	DrawModeSET();
	xdot = PRINTXMAX+PRINTSPACE*2;
	m_nBmpStat = m_cbmpCBitmap.CreateCompatibleBitmap(m_pcDC, xdot, ydot);
	ret = m_cdcMemCDC.CreateCompatibleDC(m_pcDC);
	EndDraw();

	CRect	rectClip;
	m_pcbackbmp = (CBitmap*)m_cdcMemCDC.SelectObject(&m_cbmpCBitmap);
	m_crgnClip.CreateRectRgn(0, 0, PRINTXMAX+PRINTSPACE*2, m_nBMPYSize);
	m_cdcMemCDC.SelectClipRgn(&m_crgnClip);
	m_cdcMemCDC.GetClipBox(&rectClip);
	for(i = 0; i < m_nBmpDivMax; i++){
		m_lXByte[i] = (m_nBMPXSize[i] * 3 + 3) / 4L * 4L;
		m_lBitsSize[i] = m_lXByte[i] * ydot;
		m_hBits[i] = GlobalAlloc(GHND, m_lBitsSize[i]+256);
		m_lpBits[i] = (BYTE *) GlobalLock(m_hBits[i]);
	}
}
	
void CCmdPrint::MemDCEnd()
{
	int		i;

	for(i = 0; i < m_nBmpDivMax; i++){
		if(m_lpBits[i] != NULL){
			GlobalUnlock(m_hBits[i]);
		}
		if(m_hBits[i] != 0){
			GlobalFree(m_hBits[i]);
		}
	}
	m_crgnClip.DeleteObject();
	m_cdcMemCDC.SelectObject(m_pcbackbmp);
	m_cbmpCBitmap.DeleteObject();
	m_cdcMemCDC.DeleteDC();
	if(m_lpOneLine != NULL){
		GlobalUnlock(m_hOneLine);
	}
	if(m_hOneLine != 0){
		GlobalFree(m_hOneLine);
	}
}

void CCmdPrint::AreaPaintData(DBL sx, DBL sy, DBL ex, DBL ey)
{
	DBL		sizex, sizey;
	DBL		tsx, tex;
	DBL		tsizex;
	int		bmode;
	DBL		btime;
	DBL		bmvx, bmvy;
	int		i;
	BITMAPINFO	bmpinfo;
	int		ret;

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = PRINTXMAX+PRINTSPACE*2;
	bmpinfo.bmiHeader.biHeight = m_nBMPYSize;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = (PRINTXMAX+PRINTSPACE*2) * 3 * m_nBMPYSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	bmode = senvsetdrawmode(DIRCTVIEW);
	btime = scrnsetxytime(1.0 / m_dXTime, 1.0 / m_dYTime);
	scrngetdisprltvmove(&bmvx, &bmvy);
	scrnsetrltvelement();

	scrnsetdisprltvmove(0.0, 0.0);
	sizex = ex - sx;
	sizey = ey - sy;
	for(i = 0; i < m_nBmpDivMax; i++){
		tsizex = m_nBMPXSize[i];
		tsx = i * PRINTXMAX + sx;
		tex = tsx + tsizex;
		if(tex > ex){
			tsizex = ex - tsx;
		}
		scrnsetrltvtop(tsx-PRINTSPACE, sy);
		scrnrltvareaset(-m_dMarkDot, -m_dMarkDot, tsizex+PRINTSPACE*2, sizey);
		m_cdcMemCDC.BitBlt(0, 0, (int)(tsizex+PRINTSPACE*2), (int)sizey, NULL, 0, 0, WHITENESS);

		((CTonView*)m_pcView)->DirectDraw(&m_cdcMemCDC);

#ifdef _DEBUG
		BeginDraw();
		DrawModeSET();
		m_pcDC->BitBlt(0, 0, (int)tsizex, (int)sizey, &m_cdcMemCDC, 0, 0, SRCCOPY); 
		EndDraw();
		//char msgbuf[RECMAX];
		//sprintf(msgbuf, "確認　sx %4.1lf sy %4.1lf ex %4.1lf ey %4.1lf", sx, sy, ex, ey); 
		//AfxMessageBox(msgbuf, MB_OK);
#endif

		bmpinfo.bmiHeader.biWidth = PRINTXMAX+PRINTSPACE*2;
		bmpinfo.bmiHeader.biSizeImage = (PRINTXMAX+PRINTSPACE*2) * 3 * m_nBMPYSize;
		ret = ::GetDIBits(m_cdcMemCDC.m_hDC,(HBITMAP)(m_cbmpCBitmap.m_hObject),0,(int)sizey,m_lpBits[i],&bmpinfo,DIB_RGB_COLORS);
	}
	scrnsetxytime(btime, btime);
	senvsetdrawmode(bmode);
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetdisprltvmove(bmvx, bmvy);
}

int CCmdPrint::OutPaintBlock(DBL sx, DBL sy, DBL ex, DBL ey, int cpycnt)
{
	int		xsize, ysize, xsp;
	LPBYTE	lpimgtop;
	LPBYTE	lpimg;
	int		y;
	char	msgbuf[RECMAX];
	long	max, crt;
	int		ret, idx, i;

	xsp = (int)m_dXDotNori;
	xsize = (int)(ex - sx);
	ysize = (int)(ey - sy);

#ifdef _DEBUG
	/*
	for(i = 0; i < m_nBmpDivMax; i++){
		BmpFileOut(ysize, i);
	}
	*/
#endif

	ret = m_nBmpStat;

	for(y = ysize-PRINTSPACE; y >= PRINTSPACE; y--){
		if(y == PRINTSPACE){
			y = PRINTSPACE;
		}
		idx = 0;
		for(i = 0; i < m_nBmpDivMax; i++){
			lpimgtop = m_lpBits[i];
			lpimg = lpimgtop + (m_lXByte[i] * y) + PRINTSPACE*3;
			memcpy(&(m_lpOneLine[idx]), lpimg, m_lXByte[i]-PRINTSPACE*2*3);
			idx += (m_lXByte[i]-PRINTSPACE*2*3);
		}
		//FileDataOut(m_lpOneLine, idx);
		clrprlibOutPaintData(m_lpOneLine, xsize, xsp, cpycnt);

		if(senvchckflag(STOPFLAG) == ON){
			return(FALSE);
		}

		if((m_nCrtDotY % 8) == 0){
			crt = m_nCrtDotY;
			max = (long)m_dYBlockSize;
			if(max < crt){
				crt = max;
			}
			sprintf(msgbuf, "%5ld％(%5ld/%5ld)", crt*100L/max, crt, max);
			m_pcWnd->HelpTextOut(2, msgbuf);
		}
		m_nCrtDotY++;
	}
	return(TRUE);
}

void CCmdPrint::BmpFileOut(int ysize, int i)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	long	dwsize;

	sprintf(fname, "c:\\test%02d.bmp", i);
	if((fp = fopen(fname, "wb")) == NULL){
		return;
	}
	dwsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + m_lBitsSize[i];
	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = dwsize;
	bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = m_nBMPXSize[i];
	bmpinfo.bmiHeader.biHeight = ysize;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lBitsSize[i];
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(m_lpBits[i], m_lBitsSize[i], 1, fp);
	fclose(fp);
}

void CCmdPrint::CutingDataOut(int cpycnt)
{
	int		nComID;
	int		i;
	int		oldarc, oldline, oldfill, oldkind;
	DBL		xmm, ymm;
	DBL		areasx, areasy, areaex, areaey;
	DBL		btime;
	DBL		bmvx, bmvy;

	nComID = 0;
	scrngetmmsize(&xmm, &ymm);
	m_dPlotXTime = (m_dXMMOut / xmm);
	m_dPlotYTime = (m_dYMMOut / ymm);
	scrngetdisprltvmove(&bmvx, &bmvy);
	btime = scrnsetxytime(m_dPlotXTime, m_dPlotYTime);
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetdisprltvmove(0.0, 0.0);
	
	poutarcflagget(&oldarc);
	poutlinemodeget(&oldline);
	poutfillflagget(&oldfill);
	pouttypeget(&oldkind);

	poutfillflagon();
	pouttypeset(HPGL);
	poutarcflagset(OFF);
	poutlinemodeset(ON);
	poutmodeset(OFF, ON, OFF);
	poutmmstartset(0, 0);
	for(i = 0; i < cpycnt; i++){
		m_dPlotAddX = (m_dXMMOut + m_dXMMNori) * i;
		m_dPlotAddY = 0.0;
		areasy = m_dPlotAddX;
		areasx = 0;
		areaey = (m_dPlotAddX + m_dXMMOut);
		areaex = m_dYMMOut * m_dSendTime;
		poutoutareaset(areasx, areasy, areaex, areaey);
		poutelementinit();
		poutstepoutareaset(areasx/m_dStep, areasy/m_dStep, areaex/m_dStep, areaey/m_dStep);
		poutpaintinit(nComID);
		poutpaintend(nComID);
		poutplotinit(nComID);
		PlotOutData(nComID);
		poutplotend(nComID);
	}

	poutarcflagset(oldarc);
	poutlinemodeset(oldline);
	poutfillflagset(oldfill);
	pouttypeset(oldkind);

	scrnsetxytime(btime, btime);
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetdisprltvmove(bmvx, bmvy);
}

void CCmdPrint::PlotOutData(int nComID)
{
	CTonDoc*	pcDoc;
	POSITION	pos;
	CVect	*pcVect;
	int		ret;

	((CTonView*)m_pcView)->m_nDrawIn = ON;
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	//ASSERT(!m_cPtrList.IsEmpty()); // リストが空。
	//ret = SetDataListSelectID(BASEVECTID, VCUTFLAG);
	// 2001.05.09 立身 VCUTFLAGがCVectのみにセットされるため取得不能
	ret = SetDataListSelectID(BASEVECTID, 0);
	if(ret == 0){
		((CTonView*)m_pcView)->m_nDrawIn = OFF;
		return;
	}
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		if(pcVect == NULL){
			break;
		}
		if(pcVect->CheckFlag(ACTIVEFLAG) == ON
		&& pcVect->CheckFlag(VCUTFLAG) == ON){
			PlotOutVect(nComID, pcVect);
		}
	}
	m_cPtrList.RemoveAll();
	//ASSERT(m_cPtrList.IsEmpty()); // リストが空。
	((CTonView*)m_pcView)->m_nDrawIn = OFF;
}

void CCmdPrint::PlotOutVect(int nComID, CVect *pcVect)
{
	int		pcnt, pp;
	int		p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	pcnt = pcVect->GetPointCount();
	if(pcnt == 0
	|| pcnt == 1){
		return;
	}
	poutrejionstart(nComID);
	if(pcnt == 2){
		pcVect->GetAtrDXY(0, &atr0, &x0, &y0);
		pcVect->GetAtrDXY(1, &atr1, &x1, &y1);
		TrnsDXYMMToStep(&x1, &y1);
		TrnsDXYMMToStep(&x0, &y0);
		poutmove(nComID, x0, y0);
		poutline(nComID, x0, y0, x1, y1);
		poutrejionend(nComID);
		return;
	}

	p0 = pcnt-1;
	p1 = 0;
	p2 = 1;
	pcVect->GetAtrDXY(p0, &atr0, &x0, &y0);
	pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
	pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
	TrnsDXYMMToStep(&x0, &y0);
	TrnsDXYMMToStep(&x1, &y1);
	if(pdrwatrarcchk(atr0, atr1, atr2) == ON){
		poutmove(nComID, x0, y0);
	}
	else{
		poutmove(nComID, x1, y1);
	}
	for(pp = 0; pp < pcnt;){
		p0 = pcVect->PointLegal(pcnt, pp-1);
		p1 = pcVect->PointLegal(pcnt, pp+0);
		p2 = pcVect->PointLegal(pcnt, pp+1);
		p3 = pcVect->PointLegal(pcnt, pp+2);
		p4 = pcVect->PointLegal(pcnt, pp+3);
		pcVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		pcVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		TrnsDXYMMToStep(&x0, &y0);
		TrnsDXYMMToStep(&x1, &y1);
		TrnsDXYMMToStep(&x2, &y2);
		TrnsDXYMMToStep(&x3, &y3);
		TrnsDXYMMToStep(&x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				poutline(nComID, x1, y1, x2, y2);
				poutline(nComID, x2, y2, x3, y3);
			}
			else if(p2 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				poutbeze(nComID, x1, y1, x2, y2, x3, y3, x4, y4);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				poutarc(nComID, x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					poutarc(nComID, x0, y0, x1, y1, x2, y2);
				}
				else{
					poutfreecurve(nComID, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				}
			}
			else{
				poutline(nComID, x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
	poutrejionend(nComID);
}

void CCmdPrint::TrnsDXYMMToStep(LPDBL x, LPDBL y)
{
	DBL		tx, ty;
	DBL		xmm, ymm;

	ty = *x;
	tx = *y;
	scrngetmmsize(&xmm, &ymm);
	*y = (((ty) * m_dPlotXTime) + m_dPlotAddX) / m_dStep;
	*x = (((ymm - tx) * m_dPlotYTime) + m_dPlotAddY) / m_dStep;
}

void CCmdPrint::FileDataOut(LPBYTE m_lpOneLine, int idx)
{
	FILE	*fp;
	
	if((fp = fopen("c:\\test.bin", "r+b")) == NULL){
		if((fp = fopen("c:\\test.bin", "wb")) == NULL){
			return;
		}
	}
	fseek(fp, 0L, 2);
	fwrite(m_lpOneLine, 1, idx, fp);
	fclose(fp);
}

BOOL CCmdPrint::SetDialogElement()
{
	CDialogSendData *pDialogSendData;

	// ダイアログで出力機器　のりしろ　紙サイズ　
	pDialogSendData = new CDialogSendData(m_pcView);
	pDialogSendData->m_nCmdKind = m_nCmdKind;
	strcpy(pDialogSendData->m_szPaperName, m_szPaperName);
	pDialogSendData->m_bChkX = m_bChkX;
	pDialogSendData->m_bChkY = m_bChkY;
	pDialogSendData->m_dXMMOut = m_dXMMOut;
	pDialogSendData->m_dYMMOut = m_dYMMOut;
	pDialogSendData->m_dXMMPaper = m_dXMMPaper;
	pDialogSendData->m_dYMMPaper = m_dYMMPaper;
	pDialogSendData->m_dXMMNori = m_dXMMNori;
	pDialogSendData->m_dYMMNori = m_dYMMNori;
	pDialogSendData->m_nCopyCount = m_nCopyCount;
	pDialogSendData->m_nProcess = m_nProcess;
	pDialogSendData->m_csSaverPath = _T(m_szSaverPath);
	pDialogSendData->m_csSendPath = _T(m_szSendPath);
	pDialogSendData->m_dSendTime = m_dSendTime;
	if(pDialogSendData->DoModal() == IDOK){
		m_nCmdKind = pDialogSendData->m_nCmdKind;
		strcpy(m_szPaperName, pDialogSendData->m_szPaperName);
		m_bChkX = pDialogSendData->m_bChkX;
		m_bChkY = pDialogSendData->m_bChkY;
		m_dXMMOut = pDialogSendData->m_dXMMOut;
		m_dYMMOut = pDialogSendData->m_dYMMOut;
		m_dXMMPaper = pDialogSendData->m_dXMMPaper;
		m_dYMMPaper = pDialogSendData->m_dYMMPaper;
		m_dXMMNori = pDialogSendData->m_dXMMNori;
		m_dYMMNori = pDialogSendData->m_dYMMNori;
		m_nCopyCount = pDialogSendData->m_nCopyCount;
		m_dRgnum = pDialogSendData->m_dRgnum;
		m_dGgnum = pDialogSendData->m_dGgnum;
		m_dBgnum = pDialogSendData->m_dBgnum;
		m_dRadd = pDialogSendData->m_dRadd;
		m_dGadd = pDialogSendData->m_dGadd;
		m_dBadd = pDialogSendData->m_dBadd;
		m_dRcntrst = pDialogSendData->m_dRcntrst;
		m_dGcntrst = pDialogSendData->m_dGcntrst;
		m_dBcntrst = pDialogSendData->m_dBcntrst;
		m_nProcess = pDialogSendData->m_nProcess;
		m_dSendTime = pDialogSendData->m_dSendTime;
		strcpy(m_szSaverPath, pDialogSendData->m_csSaverPath.GetBuffer(FNAMEMAX));
		strcpy(m_szSendPath, pDialogSendData->m_csSendPath.GetBuffer(FNAMEMAX));
		//::PathRemoveBackslash(m_szSaverPath);
		//::PathRemoveBackslash(m_szSendPath);
	}
	else{
		return(FALSE);
	}
	return(TRUE);
}

BOOL CCmdPrint::SaveEssData(LPSTR fname)
{
	CTonDoc*	pcDoc;
	BOOL	ret;
	int		nVerNo;
	int		nResave;
	char	szResaveBuf[RECMAX];
	int		i;
	long	lFileSize;
	CString	csTemp;

	csTemp = _T("dummy");
	nVerNo = 105;
	nResave = 32;
	memset(szResaveBuf, 0, RECMAX);
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	ret = TRUE;
	CFile file;
	file.Open(fname,  CFile::modeCreate|CFile::modeWrite, NULL);
	CArchive ar(&file, CArchive::store);
	scrnonnoactflag();

	ar << nVerNo;
	lFileSize = 0;
	ar << lFileSize;
	ar << csTemp;
	ar << csTemp;
	ar << csTemp;
	ar << csTemp;
	ar << csTemp;
	// 環境などの保存用処理
	ar << nResave;
	for(i = 0; i < nResave; i++){
		ar << szResaveBuf[i];
	}
	pcDoc->Serialize(ar);
	scrnoffnoactflag();
	ar.Close();
	file.Close();

	return(ret);
}

BOOL CCmdPrint::SaveXXXData(LPSTR fname)
{
	DBL		sx, sy, ex, ey;
	int		cnt;
	char	xxxfname[FNAMEMAX];
	FILE	*fp;

	cnt = 0;
	ey = 0;
	for(sy = 0; ey < m_dYMMOut * m_dSendTime;){
		ey = sy + m_dYMMPaper * m_dSendTime;
		if(m_dYMMOut * m_dSendTime < ey){
			ey = m_dYMMOut * m_dSendTime;
		}
		ex = 0;
		for(sx = 0; ex < m_dXMMOut;){
			ex = sx + m_dXMMPaper;
			if(m_dXMMOut < ex){
				ex = m_dXMMOut;
			}
			sprintf(xxxfname, "%s.%03d", fname, cnt);
			if((fp = fopen(xxxfname, "w")) == NULL){
				return(FALSE);
			}
			fprintf(fp, "%-4.3lf\n", sx);
			fprintf(fp, "%-4.3lf\n", sy);
			fprintf(fp, "%-4.3lf\n", ex);
			fprintf(fp, "%-4.3lf\n", ey);
			fclose(fp);
			cnt++;
			sx = ex - m_dXMMNori;
		}
		sy = ey - m_dYMMNori * m_dSendTime;
	}
	return(TRUE);
}

BOOL CCmdPrint::SaveEnvData(LPSTR fname)
{
	FILE	*fp;

	if((fp = fopen(fname, "w")) == NULL){
		return(FALSE);
	}
	fprintf(fp, "%d\n", m_nCmdKind);
	fprintf(fp, "\"ESS\"\n");
	fprintf(fp, "%4.3lf, %4.3lf, %4.3lf, %4.3lf, %4.3lf, %4.3lf, %4.3lf, %4.3lf, %4.3lf\n",
		m_dRgnum, m_dGgnum, m_dBgnum, m_dRadd, m_dGadd, m_dBadd, m_dRcntrst, m_dGcntrst, m_dBcntrst);
	fprintf(fp, "%4.3lf, %4.3lf, %4.3lf, %4.3lf, %4.3lf\n",
		m_dXMMOut, m_dYMMOut * m_dSendTime, m_dXMMNori, m_dXMMPaper, m_dYMMNori * m_dSendTime);
	fprintf(fp, "0\n");
	fprintf(fp, "%d\n", m_nCopyCount);
	fprintf(fp, "%d\n", m_nProcess);
	fprintf(fp, "0\n");
	fclose(fp);
	return(TRUE);
}

void CCmdPrint::CreateMark(DBL xsize, DBL ysize)
{
	DBL		xmm, ymm;
	DBL		scrnxmm;
	CVect	*pcVect;
	DBL		sx, sy, ex, ey;

	scrngetmmsize(&scrnxmm, &ymm);
	ymm *= m_dSendTime;
	xmm = m_dXMMPaper * (scrnxmm / m_dXMMOut);
	m_pcDataList = (CDataList *)m_pcDoc->NewData(DATALISTID);
	m_pcDoc->DataAddTail(m_pcDataList);
	m_pcDataList->SetFillPen(0);
	m_pcDataList->SetOutLinePen(0);

	pcVect = (CVect*)m_pcDoc->NewData(BASEVECTID);
	m_pcDataList->SetData(pcVect);
	sx = 0;
	ex = xsize;
	sy = ymm; ey = ymm-ysize;
	CreateCrcl(pcVect, sx, sy, ex, ey);
	pcVect = (CVect*)m_pcDoc->NewData(BASEVECTID);
	m_pcDataList->SetData(pcVect);
	pcVect->SetAtrDXY(0, 0, ex-xsize/6.0, ey);
	pcVect->SetAtrDXY(1, 0, ex, ey);
	pcVect->SetAtrDXY(2, 0, ex, ey+ysize/6.0);
	pcVect->SetPointCount(3);
	pcVect->OffFlag(REJIONFLAG);

	pcVect = (CVect*)m_pcDoc->NewData(BASEVECTID);
	m_pcDataList->SetData(pcVect);
	sx = xmm-1;
	ex = sx-xsize;
	sy = ymm;
	ey = ymm-ysize;
	CreateCrcl(pcVect, sx, sy, ex, ey);
	pcVect = (CVect*)m_pcDoc->NewData(BASEVECTID);
	m_pcDataList->SetData(pcVect);
	pcVect->SetAtrDXY(0, 0, ex+xsize/6.0, ey);
	pcVect->SetAtrDXY(1, 0, ex, ey);
	pcVect->SetAtrDXY(2, 0, ex, ey+ysize/6.0);
	pcVect->SetPointCount(3);
	pcVect->OffFlag(REJIONFLAG);

	pcVect = (CVect*)m_pcDoc->NewData(BASEVECTID);
	m_pcDataList->SetData(pcVect);
	sx = 0;
	ex = xsize;
	sy = -ysize * 2;
	ey = -ysize;
	CreateCrcl(pcVect, sx, sy, ex, ey);
	pcVect = (CVect*)m_pcDoc->NewData(BASEVECTID);
	m_pcDataList->SetData(pcVect);
	pcVect->SetAtrDXY(0, 0, ex-xsize/6.0, ey);
	pcVect->SetAtrDXY(1, 0, ex, ey);
	pcVect->SetAtrDXY(2, 0, ex, ey-ysize/6.0);
	pcVect->SetPointCount(3);
	pcVect->OffFlag(REJIONFLAG);

	m_pcDataList->RenewMiniMax();
}

void CCmdPrint::CreateCrcl(CVect *pcVect, DBL sx, DBL sy, DBL ex, DBL ey)
{
	DBL		tx, ty;
	DBL		a, b;
	DBL		ta, tb;

	a = (ex - sx) / 2.0;
	b = (ey - sy) / 2.0;
	ta = (a * 1.41421356 * 1.9525) / 5.0;
	tb = (b * 1.41421356 * 1.9525) / 5.0;
	tx = (ex + sx) / 2.0;
	ty = (ey + sy) / 2.0;
	pcVect->SetAtrDXY(  0, BEZEATR|CURVATR, tx,    sy);
	pcVect->SetAtrDXY(  1, BEZEATR,         tx+ta, sy);
	pcVect->SetAtrDXY(  2, BEZEATR,         ex, ty-tb);
	pcVect->SetAtrDXY(  3, BEZEATR|CURVATR, ex, ty);
	pcVect->SetAtrDXY(  4, BEZEATR,         ex, ty+tb);
	pcVect->SetAtrDXY(  5, BEZEATR,         tx+ta, ey);
	pcVect->SetAtrDXY(  6, BEZEATR|CURVATR, tx,    ey);
	pcVect->SetAtrDXY(  7, BEZEATR,         tx-ta, ey);
	pcVect->SetAtrDXY(  8, BEZEATR,         sx, ty+tb);
	pcVect->SetAtrDXY(  9, BEZEATR|CURVATR, sx, ty);
	pcVect->SetAtrDXY( 10, BEZEATR,         sx, ty-tb);
	pcVect->SetAtrDXY( 11, BEZEATR,         tx-ta, sy);
	pcVect->SetPointCount(12);
}
