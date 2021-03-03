
/////////////////////////////////////////////////////////////////////////////
// CCmdSpclStrg

#include "stdafx.h"
#include "math.h"

#include "cmd_base.h"
#include "cmdsstrg.h"

extern "C" {
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrcurvechk(int atr1);
	DBL   pdrwendragget(int atr1, int atr2, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL   pdrwstartragget(int atr0, int atr1, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2);
	int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0);
	int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r);
	void curvebezecontrolpointget(DBL x1, DBL y1, DBL x2, DBL y2, DBL rag, DBL *xp, DBL *yp);
	void curvenoragtocurverag(DBL *rag1, DBL *rag2);
	int curveperpendicular(DBL sx, DBL sy, DBL ex, DBL ey, DBL x, DBL y, DBL *cx, DBL *cy);
	int curveonlinechk(DBL x1, DBL y1, DBL x2, DBL y2, DBL x, DBL y);
}

CCmdSpclStrg::CCmdSpclStrg(CScrollView* pcview)
:CCmdBase(pcview)
{
}

UINT CCmdSpclStrg::ExecStrgLineStruct(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;
	DBL		xx, yy;
	int		tpp;

	switch(event){
	case INITEVENT:
		m_pcDialogStrgVStruct = new CDialogStrgVStruct(m_pcView);
		if(m_pcDialogStrgVStruct->Create(IDD_STRG_VSTRUCT, m_pcView)){
			LocateStrgDlg();
		}
		
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			//pcData = SrchData(ACTIVEFLAG, dx, dy);
			pcData = SrchDataOnLinePoint(dx, dy, &xx, &yy, &tpp);
			if(pcData != NULL 
			&& pcData->CheckID() == BASEVECTID){
				m_dSX = dx;
				m_dSY = dy;
				m_pcVect = (CVect*)pcData;
				BeginDraw();
				m_pcVect->OnFlag(ATNTION1FLAG);
				m_pcVect->AtntionDraw(m_pcDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else if(m_nMouseSetCount == 1){
			m_nIDary[0] = STRGVECTID;
			m_nIDcnt = 1;
			pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
			if(pcData != NULL 
			&& pcData->CheckID() == STRGVECTID){
				m_pcStrgVect = (CStrgVect*)pcData;
				m_pcStrgVect->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				m_pcDialogStrgVStruct->GetItemElement();
				m_bEqual = m_pcDialogStrgVStruct->m_chkStrgEqual;
				m_bNoround = m_pcDialogStrgVStruct->m_chkNoround;
				m_nSetMode = m_pcDialogStrgVStruct->m_nSetMode;
				SaveUndo();
				StrgLineStructMain();
				m_pcVect->OffFlag(ATNTION1FLAG);
				ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				ScrnDataRedraw(m_pcStrgVect);
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
	case DLGENDEVENT:
	case ENDEVENT:
		m_pcDialogStrgVStruct->DestroyWindow();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

void CCmdSpclStrg::LocateStrgDlg()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;
	int		ret;

	m_pcView->GetWindowRect(&prect);
	m_pcDialogStrgVStruct->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	ret = m_pcDialogStrgVStruct->SetWindowPos(theWnd, x, y, cx, cy, 0);
	m_pcDialogStrgVStruct->ShowWindow(SW_SHOW);
}

void CCmdSpclStrg::StrgLineStructMain()
{
	if(m_bEqual == FALSE){
		SetLenStrgLineStructSpace();
	}
	else{
		SetLenStrgLineStructEqual();
	}
	StrgLineStructTrns();
}

void CCmdSpclStrg::SetLenStrgLineStructSpace()
{
	CMojiVect*	pcMojiVect;
	DBL		tmplinelen;
	DBL		dmojispace, dstrgspace;
	DBL		dwidth, dhight;
	int		tategaki;
	DBL		sminix, sminiy, smaxx, smaxy;
	DBL		mminix, mminiy, mmaxx, mmaxy;
	DBL		tlen, setlen;
	int		tpp;

	m_pcStrgVect->GetWakuDMiniMax(&sminix, &sminiy, &smaxx, &smaxy);
	m_pcStrgVect->GetSpaceEnv(&dmojispace, &dstrgspace);
	m_pcStrgVect->GetFontSize(&dwidth, &dhight);
	tategaki = m_pcStrgVect->CheckTateYoko();
	if(tategaki == ON){
		m_dStrglen = smaxy - sminiy;
		m_dHalffontsize = dhight / 2;
	}	
	else{
		m_dStrglen = smaxx - sminix;
		m_dHalffontsize = dwidth / 2;
	}

	m_dLinelen = GetVectLength();
	if(m_pcVect->CheckFlag(REJIONFLAG) == ON
	&& m_pcVect->GetPointCount() != 2){
		m_dStrglen = m_dStrglen+dmojispace;
		m_pcVect->SrchNearOnLinePoint(m_dSX, m_dSY, &tlen, &m_dCX, &m_dCY, &tpp);
		m_dStartlen = GetVectPointLength(m_dCX, m_dCY);
		m_dTrnstime = m_dLinelen / m_dStrglen;
	}
	else{
		//文字の中心までの長さをひく
		if(tategaki == ON){
			m_dStrglen = m_dStrglen - dhight;
			tmplinelen = m_dLinelen - dhight;
		}
		else{
			m_dStrglen = m_dStrglen - dwidth;
			tmplinelen = m_dLinelen - dwidth;
		}
		m_dStartlen = 0;
		m_dTrnstime = tmplinelen / m_dStrglen;
	}

	pcMojiVect = m_pcStrgVect->m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetWakuDMiniMax(&mminix, &mminiy, &mmaxx, &mmaxy);
		if(tategaki == ON){
			setlen = (mmaxy + mminiy) / 2 - (sminiy + m_dHalffontsize);
		}
		else{
			setlen = (mmaxx + mminix) / 2 - (sminix + m_dHalffontsize);
		}
		pcMojiVect->SetLenFromStrgTop(setlen);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CCmdSpclStrg::SetLenStrgLineStructEqual()
{
	int	tategaki;
	CMojiVect*	pcMojiVect;
	DBL	mminix, mminiy, mmaxx, mmaxy;
	DBL	dwd, dhi;
	DBL	setlen;
	int	tpp;
	DBL	tlen;
	DBL	tmplinelen;

	tategaki = m_pcStrgVect->CheckTateYoko();
	m_dStrglen = 0;
	pcMojiVect = m_pcStrgVect->m_pcMojiTop;
	pcMojiVect->GetWakuDMiniMax(&mminix, &mminiy, &mmaxx, &mmaxy);
	m_dHalffontsize = (mmaxx - mminix)/2;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetFontSize(&dwd, &dhi);
		setlen = m_dStrglen;
		if(tategaki == ON){
			m_dStrglen += dhi;
		}
		else{
			m_dStrglen += dwd;
		}
		pcMojiVect->SetLenFromStrgTop(setlen);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}

	m_dLinelen = GetVectLength();
	if(m_pcVect->CheckFlag(REJIONFLAG) == ON
	&& m_pcVect->GetPointCount() != 2){
		m_pcVect->SrchNearOnLinePoint(m_dSX, m_dSY, &tlen, &m_dCX, &m_dCY, &tpp);
		m_dStartlen = GetVectPointLength(m_dCX, m_dCY);
		m_dTrnstime = m_dLinelen / m_dStrglen;
	}
	else{
		//文字の中心までの長さをひく
		pcMojiVect = m_pcStrgVect->m_pcMojiTop;
		pcMojiVect->GetFontSize(&dwd, &dhi);
		if(tategaki == ON){
			m_dStrglen = m_dStrglen - dhi;
			tmplinelen = m_dLinelen - dhi;
		}
		else{
			m_dStrglen = m_dStrglen - dwd;
			tmplinelen = m_dLinelen - dwd;
		}
		m_dStartlen = 0;
		m_dTrnstime = tmplinelen / m_dStrglen;
	}
}

void CCmdSpclStrg::StrgLineStructTrns()
{
	CMojiVect*	pcMojiVect;
	DBL		setlen;
	DBL		mminix, mminiy, mmaxx, mmaxy;
	DBL		cx, cy;
	DBL		movex, movey;
	DBL		tsin, tcos;
	DBL		rag, dx, dy;

	pcMojiVect = m_pcStrgVect->m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		setlen = pcMojiVect->GetLenFromStrgTop();
		setlen = setlen*m_dTrnstime + m_dHalffontsize + m_dStartlen;
		if(m_dLinelen < setlen){
			setlen = setlen - m_dLinelen;
		}
		pcMojiVect->GetWakuDMiniMax(&mminix, &mminiy, &mmaxx, &mmaxy);
		switch(m_nSetMode){
		case LEFTDOWN:
			cx = (mminix + mmaxx) / 2;
			cy = mminiy;
			break;
		case CNTRCNTR:
			cx = (mminix + mmaxx) / 2;
			cy = (mminiy + mmaxy) / 2;
			break;
		case RIGHTUP:
			cx = (mminix + mmaxx) / 2;
			cy = mmaxy;
			break;
		}

		rag = GetVectPointRagAndCood(&setlen, &dx, &dy);
		movex = dx - cx;
		movey = dy - cy;
		tsin = sin(rag);
		tcos = cos(rag);
		pcMojiVect->MoveTrns(movex, movey);
		if(m_bNoround == FALSE){
			pcMojiVect->RoundTrns(dx, dy, tsin, tcos);
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

DBL CCmdSpclStrg::GetVectLength()
{
	DBL		sumlen;

	sumlen = m_pcVect->GetLength();
	return(sumlen);
}


// 点までの長さを求める
DBL CCmdSpclStrg::GetVectPointLength(DBL xx, DBL yy)
{
	int		pcnt;
	int		flag;
	DBL		x1, y1, x2, y2;
	DBL		sumlen;

	flag = OFF;
	sumlen = 0;
	pcnt = m_pcVect->GetPointCount();
	if(pcnt <= 1){
		return(sumlen);
	}
	if(pcnt == 2){
		m_pcVect->GetDXY(0, &x1, &y1);
		m_pcVect->GetDXY(1, &x2, &y2);
		sumlen = GetLinePointLength(&flag, xx, yy, x1, y1, x2, y2);
		return(sumlen);
	}
	if(m_pcVect->CheckFlag(REJIONFLAG) == ON){
		sumlen = GetRejionPointLength(&flag, xx, yy);
	}
	else{
		sumlen = GetNorejionPointLength(&flag, xx, yy);
	}
	return(sumlen);
}

DBL CCmdSpclStrg::GetRejionPointLength(int *flag, DBL xx, DBL yy)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	pcnt = m_pcVect->GetPointCount();
	for(pp = 0; pp < pcnt;){
		p0 = m_pcVect->PointLegal(pcnt, pp-1);
		p1 = m_pcVect->PointLegal(pcnt, pp+0);
		p2 = m_pcVect->PointLegal(pcnt, pp+1);
		p3 = m_pcVect->PointLegal(pcnt, pp+2);
		p4 = m_pcVect->PointLegal(pcnt, pp+3);
		m_pcVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
				len = GetLinePointLength(flag, xx, yy, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else if(p2 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else{
				len = GetBezePointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3, x4, y4);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				len = GetArcPointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					len = GetArcPointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3);
					sumlen += len;
					if(*flag == ON){
						return(sumlen);
					}
				}
				else{
					len = GetFreeCurvePointLength(flag, xx, yy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
					sumlen += len;
					if(*flag == ON){
						return(sumlen);
					}
				}
			}
			else{
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 1;
		}
	}
	return(sumlen);
}

DBL CCmdSpclStrg::GetNorejionPointLength(int *flag, DBL xx, DBL yy)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	pcnt = m_pcVect->GetPointCount();
	for(pp = 0; pp < pcnt-1;){
		p0 = m_pcVect->PointLegal(pcnt, pp-1);
		p1 = m_pcVect->PointLegal(pcnt, pp+0);
		p2 = m_pcVect->PointLegal(pcnt, pp+1);
		p3 = m_pcVect->PointLegal(pcnt, pp+2);
		p4 = m_pcVect->PointLegal(pcnt, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		m_pcVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
				len = GetLinePointLength(flag, xx, yy, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else if(p3 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else{
				len = GetBezePointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3, x4, y4);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else{
				len = GetArcPointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				len = GetFreeCurvePointLength(flag, xx, yy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else{
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 1;
		}
	}
	return(sumlen);
}

DBL CCmdSpclStrg::GetBezePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL		t, t1, b1, b2, b3, b4;
	DBL		sx, sy, ex, ey;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		len = GetLinePointLength(flag, xx, yy, sx, sy, ex, ey);
		sumlen += len;
		if(*flag == ON){
			return(sumlen);
		}
		sx = ex; sy = ey;
	}
	ex = x4; ey = y4;
	len = GetLinePointLength(flag, xx, yy, sx, sy, ex, ey);
	sumlen += len;
	return(sumlen);
}

DBL CCmdSpclStrg::GetArcPointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		x0, y0, rag01, rag03, dr;
	int		stat;
	DBL		rag, steprag;
	DBL		sx, sy, ex, ey;
	int		count, idx;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
		sumlen += len;
		if(*flag == ON){
			return(sumlen);
		}
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
		sumlen += len;
		return(sumlen);
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
		sumlen += len;
		if(*flag == ON){
			return(sumlen);
		}
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
		sumlen += len;
		return(sumlen);
	}

	count = (int)(fabs(rag03 - rag01) / M_5RAG);
	if(count < 2){
		count = 2;
	}
	steprag = (rag03 - rag01) / (DBL)count;

	sx = x1; sy = y1;
	for(idx = 1, rag = rag01+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		len = GetLinePointLength(flag, xx, yy, sx, sy, ex, ey);
		sumlen += len;
		if(*flag == ON){
			return(sumlen);
		}
		sx = ex; sy = ey;
	}
	ex = x3; ey = y3;
	len = GetLinePointLength(flag, xx, yy, sx, sy, ex, ey);
	sumlen += len;
	return(sumlen);
}

DBL CCmdSpclStrg::GetFreeCurvePointLength(int *flag, DBL xx, DBL yy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag1, rag2;
	DBL	xp1, yp1, xp2, yp2;
	DBL	len;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
		curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
		len = GetBezePointLength(flag, xx, yy, x1, y1, xp1, yp1, xp2, yp2, x2, y2);
	}
	return(len);
}

DBL CCmdSpclStrg::GetLinePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;
	DBL		cx, cy;
	int		ret;
	DBL		tlen;

	*flag = OFF;
	len = hypot(x1-x2, y1-y2);
	ret = curveperpendicular(x1, y1, x2, y2, xx, yy, &cx, &cy);
	if(ret != FAIL){
		ret = curveonlinechk(x1, y1, x2, y2, cx, cy);
		if(ret == ON){
			tlen = hypot(xx - cx, yy - cy);
			if(tlen < 1.0){
				*flag = ON;
				len = hypot(x1-cx, y1-cy);
			}
		}
	}
	return(len);
}

// 開始点からの距離が０になった角度と位置を求める
DBL CCmdSpclStrg::GetVectPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
{
	int		pcnt;
	DBL		x1, y1, x2, y2;
	DBL		rag;

	rag = 0;
	pcnt = m_pcVect->GetPointCount();
	if(pcnt <= 1){
		return(rag);
	}
	if(pcnt == 2){
		m_pcVect->GetDXY(0, &x1, &y1);
		m_pcVect->GetDXY(1, &x2, &y2);
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		return(rag);
	}
	if(m_pcVect->CheckFlag(REJIONFLAG) == ON){
		rag = GetRejionPointRagAndCood(setlen, dx, dy);
	}
	else{
		rag = GetNorejionPointRagAndCood(setlen, dx, dy);
	}
	return(rag);
}

DBL CCmdSpclStrg::GetRejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rag;

	rag = 0;
	pcnt = m_pcVect->GetPointCount();
	for(pp = 0; pp < pcnt;){
		p0 = m_pcVect->PointLegal(pcnt, pp-1);
		p1 = m_pcVect->PointLegal(pcnt, pp+0);
		p2 = m_pcVect->PointLegal(pcnt, pp+1);
		p3 = m_pcVect->PointLegal(pcnt, pp+2);
		p4 = m_pcVect->PointLegal(pcnt, pp+3);
		m_pcVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
				rag = GetLinePointRagAndCood(setlen, dx, dy, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else if(p2 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else{
				rag = GetBezePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3, x4, y4);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				rag = GetArcPointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					rag = GetArcPointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
				}
				else{
					rag = GetFreeCurvePointRagAndCood(setlen, dx, dy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
				}
			}
			else{
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 1;
		}
	}
	return(rag);
}

DBL CCmdSpclStrg::GetNorejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rag;

	rag = 0;
	pcnt = m_pcVect->GetPointCount();
	for(pp = 0; pp < pcnt-1;){
		p0 = m_pcVect->PointLegal(pcnt, pp-1);
		p1 = m_pcVect->PointLegal(pcnt, pp+0);
		p2 = m_pcVect->PointLegal(pcnt, pp+1);
		p3 = m_pcVect->PointLegal(pcnt, pp+2);
		p4 = m_pcVect->PointLegal(pcnt, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		m_pcVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
				rag = GetLinePointRagAndCood(setlen, dx, dy, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else if(p3 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else{
				rag = GetBezePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3, x4, y4);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else{
				rag = GetArcPointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				rag = GetFreeCurvePointRagAndCood(setlen, dx, dy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else{
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 1;
		}
	}
	m_pcVect->GetAtrDXY(pcnt-1, &atr1, dx, dy);
	return(rag);
}

DBL CCmdSpclStrg::GetBezePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL		t, t1, b1, b2, b3, b4;
	DBL		sx, sy, ex, ey;
	DBL		len;
	DBL		rag;

	rag = 0;
	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		rag = GetLinePointRagAndCood(setlen, dx, dy, sx, sy, ex, ey);
		if((*setlen) <= 0){
			return(rag);
		}
		sx = ex; sy = ey;
	}
	ex = x4; ey = y4;
	rag = GetLinePointRagAndCood(setlen, dx, dy, sx, sy, ex, ey);
	if((*setlen) <= 0){
		return(rag);
	}
	len = hypot(sx-ex, sy-ey);
	return(rag);
}

DBL CCmdSpclStrg::GetArcPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		x0, y0, rag01, rag03, dr;
	int		stat;
	DBL		rag, steprag;
	DBL		sx, sy, ex, ey;
	int		count, idx;
	DBL		retrag;

	rag = 0;
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
		return(rag);
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
		return(rag);
	}

	count = (int)(fabs(rag03 - rag01) / M_5RAG);
	if(count < 2){
		count = 2;
	}
	steprag = (rag03 - rag01) / (DBL)count;

	sx = x1; sy = y1;
	for(idx = 1, rag = rag01+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		retrag = GetLinePointRagAndCood(setlen, dx, dy, sx, sy, ex, ey);
		if((*setlen) <= 0){
			return(retrag);
		}
		sx = ex; sy = ey;
	}
	ex = x3; ey = y3;
	retrag = GetLinePointRagAndCood(setlen, dx, dy, sx, sy, ex, ey);
	if((*setlen) <= 0){
		return(retrag);
	}
	return(retrag);
}

DBL CCmdSpclStrg::GetFreeCurvePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag;
	DBL	rag1, rag2;
	DBL	xp1, yp1, xp2, yp2;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
		curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
		rag = GetBezePointRagAndCood(setlen, dx, dy, x1, y1, xp1, yp1, xp2, yp2, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
	}
	return(rag);
}

DBL CCmdSpclStrg::GetLinePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;
	DBL		rag;

	len = hypot(x1-x2, y1-y2);
	if((*setlen) <= len){
		rag = atan2(y2-y1, x2-x1);
		(*dx) = (x2 - x1) * (*setlen) / len + x1;
		(*dy) = (y2 - y1) * (*setlen) / len + y1;
		(*setlen) = 0;
	}
	else{
		(*setlen) = (*setlen) - len;
		rag = atan2(y2-y1, x2-x1);
	}
	return(rag);
}
