
/////////////////////////////////////////////////////////////////////////////
// CCmdStrgProp

#include "stdafx.h"
#include "math.h"

#include "cmd_base.h"
#include "cmdsprop.h"

extern "C" {
	int baseiskanji(UINT far hc);
	void senvgetexepath(LPSTR path);
	DBL scrnsetxytime(DBL xtime, DBL ytime);
	void scrngetmmsize(LPDBL bxmm, LPDBL bymm);
	void scrnsetmmsize(DBL xsize, DBL ysize);
	void scrnsetrltvelement();
	void scrngetrltvtop(LPDBL sx, LPDBL sy); 
	void scrnsetrltvtop(DBL sx, DBL sy); 
	void scrnsetdisprltvmove(DBL xmove, DBL ymove);
	void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove);
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void scrndxymmtorltv(LPDBL x, LPDBL y);
}

CCmdStrgProp::CCmdStrgProp(CScrollView* pcview)
:CCmdBase(pcview)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	char	buf[HRECMAX];
	FILE	*fp;

	m_dSpace = 10.0;
	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmdsprop.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf", &m_dSpace);
		fclose(fp);
	}
}

CCmdStrgProp::~CCmdStrgProp()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmdsprop.env", exepath);
	if((fp = fopen(fname, "w")) != NULL){
		fprintf(fp, "%4.1lf\n", m_dSpace);
		fclose(fp);
	}
}

UINT CCmdStrgProp::ExecStrgProp(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		m_pcDialogInputNum = new CDialogInputNum(m_pcView);
		m_pcDialogInputNum->SetModaless();
		m_pcDialogInputNum->SetDialogTitle("文字列プロポーショナル");
		m_pcDialogInputNum->SetUnitText("mm");
		m_pcDialogInputNum->SetTitleText("目安文字間隔");
		m_pcDialogInputNum->m_dInputNum = FLOOR100(m_dSpace);
		if(m_pcDialogInputNum->Create(IDD_INNUM, m_pcView)){
			m_pcDialogInputNum->SetItemElement();
			m_pcDialogInputNum->ShowWindow(SW_SHOW);
		}
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_nIDary[0] = STRGVECTID;
			m_nIDary[1] = SWAKVECTID;
			m_nIDcnt = 2;
			pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
			if(pcData != NULL){
				SaveUndo();
				m_nMouseSetCount = 1;
				m_pcStrgVect = (CStrgVect*)pcData;
				m_pcStrgVect->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				m_pcDialogInputNum->GetItemElement();
				m_dSpace = m_pcDialogInputNum->m_dInputNum;
				StrgPropMain();
				ScrnAllRedraw();
				//ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				//ScrnDataRedraw(m_pcStrgVect);
				m_nMouseSetCount = 0;
				//return(REDRAWEVENT);
			}
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case DLGRESETEVENT:
	case DLGSETEVENT:
	case ENDEVENT:
		m_pcDialogInputNum->DestroyWindow();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

void CCmdStrgProp::StrgPropMain()
{
	CDC		*pcDC;
	CMojiVect*	pcMojiVect;
	DBL		dWsize, dHsize;
	DBL		dTrnsTime, btime;
	DBL		bxmm, bymm;
	DBL		bsx, bsy; 
	DBL		bmvx, bmvy;
	DBL		areaxmm, areaymm;
	DBL		minix, miniy, maxx, maxy;
	DBL		basex, basey;
	HPEN	hPen, oldhPen;
	HBRUSH	hBrush, oldhBrush;
	int		rval, gval, bval;
	char	sString[STRGMAX];
	int		len, idx, hc, lc, i;

	m_pcDibLib = new CDibLib(m_pcView);
	pcDC = m_pcDibLib->CreateDIBDC(BITMAPSIZE, BITMAPSIZE);
	if(pcDC == NULL){
		return;
	}
	PropTableAlloc();
	if(m_hPrePost == NULL){
		return;
	}
	m_pcStrgVect->GetFontSize(&dWsize, &dHsize);
	m_nTateYoko = m_pcStrgVect->CheckTateYoko();
	if(m_nTateYoko == ON){
		dTrnsTime = dHsize / (DBL)BITMAPSIZE;
	}
	else{
		dTrnsTime = dWsize / (DBL)BITMAPSIZE;
	}
	rval = 0; gval = 0; bval = 0;
	hPen = CreatePen(PS_INSIDEFRAME, 1, RGB(rval, gval, bval));
	hBrush = CreateSolidBrush(RGB(rval, gval, bval));
	oldhPen = (HPEN)SelectObject(pcDC->m_hDC, hPen);
	oldhBrush = (HBRUSH)SelectObject(pcDC->m_hDC, hBrush);
	m_pcStrgVect->GetString(sString);
	PropTableInit();
	len = strlen(sString);
	idx = 0;

	pcMojiVect = m_pcStrgVect->m_pcMojiTop;
	while(1){
		if(idx >= len){
			break;
		}
		hc = sString[idx] & 0x00ff;
		lc = sString[idx+1] & 0x00ff;
		if(hc == 0x81 && lc == 0x40){
			for(i = 0; i < PREPOSTTBLSIZE; i++){
				m_lpPreTbl[i] = 0;
				m_lpPostTbl[i] = BITMAPSIZE;
			}
		}
		else if(hc == 0x20){
			for(i = 0; i < PREPOSTTBLSIZE; i++){
				m_lpPreTbl[i] = 0;
				m_lpPostTbl[i] = BITMAPSIZE/2;
			}
		}
		else{
			pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
			areaxmm = maxx - minix;
			areaymm = maxy - miniy;
			m_pcDibLib->AreaCls(pcDC);
			btime = scrnsetxytime(dTrnsTime, dTrnsTime);
			scrngetmmsize(&bxmm, &bymm);
			scrngetrltvtop(&bsx, &bsy); 
			scrngetdisprltvmove(&bmvx, &bmvy);
			scrnsetrltvtop(0.0, 0.0);
			scrnsetdisprltvmove(0.0, 0.0);
			scrnsetmmsize(areaxmm, areaymm);
			scrnsetrltvelement();
			scrndxymmtorltv(&minix, &maxy);
			scrnsetrltvtop(minix, maxy);
			scrnrltvareaset(-INTMAX, -INTMAX, INTMAX, INTMAX);
			pcMojiVect->CheckFlagDraw(ACTIVEFLAG, pcDC);
			m_pcDibLib->GetBitImage();
			PropTableMiniMaxSet();
		}
		if(idx != 0){
			if(m_nTateYoko == ON){
				PropMoveMain(pcMojiVect, basey, dHsize);
			}
			else{
				PropMoveMain(pcMojiVect, basex, dWsize);
			}
			pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
			if(m_nTateYoko == ON){
				if(baseiskanji(hc) == ON){
					basey = miniy;
				}
				else{
					basey = miniy - (maxy - miniy);
				}
			}
			else{
				if(baseiskanji(hc) == ON){
					basex = maxx;
				}
				else{
					basex = maxx + (maxx - minix);
				}
			}
		}
		else{
			pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
			if(m_nTateYoko == ON){
				if(baseiskanji(hc) == ON){
					basey = miniy;
				}
				else{
					basey = miniy - (maxy - miniy);
				}
			}
			else{
				if(baseiskanji(hc) == ON){
					basex = maxx;
				}
				else{
					basex = maxx + (maxx - minix);
				}
			}
		}
		PropTableCopy();
		scrnsetxytime(btime, btime);
		scrnsetmmsize(bxmm, bymm);
		scrnsetrltvtop(bsx, bsy); 
		scrnsetrltvelement();
		scrnsetdisprltvmove(bmvx, bmvy);

		hc = sString[idx] & 0x00ff;
		if(baseiskanji(hc) == ON){
			idx += 2;
		}
		else{
			idx += 1;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();	
	}

	m_pcStrgVect->RenewMiniMax();

	SelectObject(pcDC->m_hDC, oldhPen);
	SelectObject(pcDC->m_hDC, oldhBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);

	PropTableFree();
	m_pcDibLib->DeleteDIBDC();
	delete(m_pcDibLib);
}

void CCmdStrgProp::PropTableAlloc()
{
	DWORD	dwSize;

	dwSize = sizeof(WORD) * PREPOSTTBLSIZE * 3;
	m_hPrePost = GlobalAlloc(GHND, dwSize);
	if(m_hPrePost == NULL){
		return;
	}
	m_lpPrePost = (WORD FAR *)GlobalLock(m_hPrePost);
	m_lpPreTbl = &(m_lpPrePost[PREPOSTTBLSIZE * 0]);
	m_lpPostTbl = &(m_lpPrePost[PREPOSTTBLSIZE * 1]);
	m_lpBFTbl = &(m_lpPrePost[PREPOSTTBLSIZE * 2]);
}

void CCmdStrgProp::PropTableInit()
{
	int		i;

	for(i = 0; i < PREPOSTTBLSIZE; i++){
		m_lpBFTbl[i] = BITMAPSIZE;
	}
}

void CCmdStrgProp::PropMoveMain(CMojiVect *pcMojiVect, DBL base, DBL dd)
{
	int		i;
	DBL		basespace, space, subspace;
	DBL		minispace, sumspace;
	DBL		movebase, move;
	DBL		sx, sy;
	DBL		tmovex, tmovey;
	int		spacetbl[PREPOSTTBLSIZE];	

	minispace = BITMAPSIZE;
	sumspace = 0;
	for(i = 0; i < PREPOSTTBLSIZE; i++){
		space = (BITMAPSIZE - m_lpBFTbl[i]) + m_lpPreTbl[i];
		spacetbl[i] = (int)space;
		if(space < minispace){
			minispace = space;
		}
		sumspace += space;
	}
	basespace = m_dSpace * BITMAPSIZE / dd;
	space = sumspace / (DBL)PREPOSTTBLSIZE;
	subspace = (BITMAPSIZE/4 - (space - minispace)) / 2;
	space = basespace + subspace;
	if(basespace / 10.0 > space){
		space = basespace/10.0;
	}
	if(m_nTateYoko == ON){
		movebase = minispace - space;
		move = movebase * dd / BITMAPSIZE;
		pcMojiVect->GetStartXY(&sx, &sy);
		tmovey = (base + move) - (sy+dd);
		tmovex = 0;
	}
	else{
		movebase = minispace - space;
		move = movebase * dd / BITMAPSIZE;
		pcMojiVect->GetStartXY(&sx, &sy);
		tmovex = (base - move) - sx;
		tmovey = 0;
	}
	pcMojiVect->MoveTrns(tmovex, tmovey);
}

void CCmdStrgProp::PropTableCopy()
{
	int		i;

	for(i = 0; i < PREPOSTTBLSIZE; i++){
		m_lpBFTbl[i] = m_lpPostTbl[i];
	}
}

void CCmdStrgProp::PropTableMiniMaxSet()
{
	int		i;
	int		pretbl[PREPOSTTBLSIZE];
	int		psttbl[PREPOSTTBLSIZE];

	for(i = 0; i < PREPOSTTBLSIZE; i++){
		if(m_nTateYoko == ON){
			m_lpPreTbl[i] = m_pcDibLib->PreFirstYOnBit(i);
			pretbl[i] = m_lpPreTbl[i];
			if(BITMAPSIZE/2 < m_lpPreTbl[i]){
				m_lpPreTbl[i] = BITMAPSIZE/2;
				pretbl[i] = m_lpPreTbl[i];
			}
			m_lpPostTbl[i] = m_pcDibLib->PostFirstYOnBit(i);
			psttbl[i] = m_lpPostTbl[i];
			if(m_lpPostTbl[i] < BITMAPSIZE/2){
				m_lpPostTbl[i] = BITMAPSIZE/2;
				psttbl[i] = m_lpPostTbl[i];
			}
		}
		else{
			m_lpPreTbl[i] = m_pcDibLib->PreFirstXOnBit(i);
			pretbl[i] = m_lpPreTbl[i];
			if(BITMAPSIZE/2 < m_lpPreTbl[i]){
				m_lpPreTbl[i] = BITMAPSIZE/2;
				pretbl[i] = m_lpPreTbl[i];
			}
			m_lpPostTbl[i] = m_pcDibLib->PostFirstXOnBit(i);
			psttbl[i] = m_lpPostTbl[i];
			if(m_lpPostTbl[i] < BITMAPSIZE/2){
				m_lpPostTbl[i] = BITMAPSIZE/2;
				psttbl[i] = m_lpPostTbl[i];
			}
		}
	}
}

void CCmdStrgProp::PropTableFree()
{
	GlobalUnlock(m_hPrePost);
	GlobalFree(m_hPrePost);
}

