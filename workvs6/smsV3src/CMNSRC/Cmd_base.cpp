
/////////////////////////////////////////////////////////////////////////////
// CCmdBase

#include "stdafx.h"
#include "math.h"
#include <mbstring.h>
#include "..\cmnsrc\cmd_base.h"

extern "C" {
	int  baseiskanji(int far hc);
	void scrnsetrltvelement();
	void scrnresetzoomtime();
	void senvgetlslctclr(int *rval, int *gval, int *bval);
	void scrnsetslctpen(HDC PaintDC);
	void scrnsetfigupen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrndlenrltvtomm(LPDBL len);
	void scrndlenmmtorltv(LPDBL len);
	void senvgetareadotsize(int *ndot);
	void senvgeteditconerkind(LPINT nkind);
}

//P コマンド処理基本クラスのコンストラクタ
CCmdBase::CCmdBase(CView* pcview)
{
	m_nMouseSetCount = -1;
	m_pcView = (CTonView*)pcview;
	m_pcApp = (CTonApp*)AfxGetApp();
	m_pcWnd = (CMainFrame*)m_pcApp->GetMainWnd();
	m_pcSysEnv = m_pcApp->m_pcSysEnv;
	m_pcPtrList = &m_cPtrList;
}

void CCmdBase::InitClass()
{
	m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
}

//P 描画開始処理関数
void CCmdBase::BeginDraw()
{
	m_pcDC = m_pcView->GetDC();
	m_pcDC->SetROP2(R2_XORPEN);
}

//P 描画終了処理関数
void CCmdBase::EndDraw()
{
	m_pcView->ReleaseDC(m_pcDC);
}

//P ＸＯＲ描画モード変更関数
void CCmdBase::DrawModeXOR()
{
	m_pcDC->SetROP2(R2_XORPEN);
}

//P ＳＥＴ描画モード変更関数
void CCmdBase::DrawModeSET()
{
	m_pcDC->SetROP2(R2_COPYPEN);
}

//P ステータスバーに文字列を表示する関数
void CCmdBase::HlpMsgDsp(LPSTR msg)
{
	m_pcWnd->HelpTextOut(STATBARHELP, msg);
}

//P 指定位置にステータスバーに文字列を表示する関数
void CCmdBase::HlpMsgDsp(int no, LPSTR msg)
{
	m_pcWnd->HelpTextOut(no, msg);
}

void CCmdBase::DispSelectDataSize()
{
	CTonDoc *pcDoc;
	DBL		minx, miny, maxx, maxy;
	DBL		sizex, sizey;
	char	msgbuf[HRECMAX];

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minx, &miny, &maxx, &maxy);
	sizex = maxx - minx;
	sizey = maxy - miny;
	if(sizex < 0){
		sizex = 0;
	}
	if(sizey < 0){
		sizey = 0;
	}
	sprintf(msgbuf, "ｻｲｽﾞ%4.1lfmm %4.1lfmm", sizex, sizey);
	HlpMsgDsp(1, msgbuf);
}

//P ツールバーの初期化関数
void CCmdBase::InitToolBarElement()
{
	m_pcWnd->InitItemElement();
}

//P ツールバーの表示関数
void CCmdBase::ShowToolBar()
{
	m_pcWnd->ShowItemElement();
}

//P ツールバーの非表示関数
void CCmdBase::HideToolBar()
{
	m_pcWnd->HideItemElement();
}

//P ツールバーの要素設定関数
void CCmdBase::SetToolBarElement()
{
	m_pcWnd->SetItemElement();
}

//P ツールバーの要素取得関数
void CCmdBase::GetToolBarElement()
{
	m_pcWnd->GetItemElement();
}


//P いちばん近いデータを探す関数
CData* CCmdBase::SrchData(int flag, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	return(pcDoc->SrchData(flag, dx, dy));	
}

//P 複数のフラグのどれかにマッチしたいちばん近いデータのを探す関数
CData* CCmdBase::SrchIdsData(int *nIDary, int nIDcnt, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	return(pcDoc->SrchIdsData(nIDary, nIDcnt, dx, dy));	
}

//P いちばん近い点を探す関数
CData* CCmdBase::SrchDataPoint(DBL dx, DBL dy, int *retpp)
{
	CTonDoc*	pcDoc;
	CData	*retData;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	retData = pcDoc->SrchDataPoint(dx, dy, retpp);
	return(retData);
}

//P いちばん近い線を探す関数
CData* CCmdBase::SrchDataOnLinePoint(DBL dx, DBL dy, DBL *retx, DBL *rety, int *retpp)
{
	CTonDoc*	pcDoc;
	CData	*retData;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	retData = pcDoc->SrchDataOnLinePoint(dx, dy, retx, rety, retpp);
	return(retData);
}

CData* CCmdBase::SrchDataFromPtrList(int flag, DBL dx, DBL dy)
{
	CData* pData;
	CData* pretData;
	POSITION pos;
	DBL		len, tlen;

	pretData = NULL;
	len = INTMAX;
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_cPtrList.GetNext(pos);
		if(pData == NULL){
			break;
		}
		if(pData->CheckFlag(ALLLOCKFLAG) == ON){
			continue;
		}
		if(flag == 0){
			tlen = INTMAX;
			pData->SrchDataBoxs(dx, dy, &tlen);
			if(tlen <= len){
				len = tlen;
				pretData = pData;
			}
		}
		else if(pData->CheckFlag(flag) == ON){
			tlen = INTMAX;
			pData->SrchDataBoxs(dx, dy, &tlen);
			if(tlen <= len){
				len = tlen;
				pretData = pData;
			}
		}
	}
	scrndlenmmtorltv(&len);
	if(SLCTNEARLIMIT < len){
		return(NULL);
	}
	return(pretData);
}

//P 注目状態の線表示関数
void CCmdBase::DrawAtnLine(CDC *pcDC)
{
	DBL		sx, sy;
	DBL		ex, ey;

	sx = m_dSX;
	sy = m_dSY;
	ex = m_dEX;
	ey = m_dEY;

	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);

	scrnsetslctpen(pcDC->m_hDC);
	pcDC->MoveTo((long)sx, (long)sy);
	pcDC->LineTo((long)ex, (long)ey);

	scrnpenend(pcDC->m_hDC);
}

//P 注目状態の矩形を表示する関数
void CCmdBase::DrawAtnBoxs(CDC *pcDC)
{
	DBL		sx, sy;
	DBL		ex, ey;

	sx = m_dSX;
	sy = m_dSY;
	ex = m_dEX;
	ey = m_dEY;

	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);

	scrnsetslctpen(pcDC->m_hDC);
	DrawBoxs(pcDC, sx, sy, ex, ey);
	scrnpenend(pcDC->m_hDC);
}

//P 矩形を表示する関数
void CCmdBase::DrawBoxs(CDC *pcDC, DBL dsx, DBL dsy, DBL dex, DBL dey)
{
	int		sx, sy, ex, ey;

	sx = (int)dsx; sy = (int)dsy;
	ex = (int)dex; ey = (int)dey;
	pcDC->MoveTo(sx, sy);
 	pcDC->LineTo(sx, ey);
 	pcDC->LineTo(ex, ey);
 	pcDC->LineTo(ex, sy);
 	pcDC->LineTo(sx, sy);
}

//P １行読み出し改行を削除する関数
LPSTR CCmdBase::CrDelFgets(char * buf, int max, FILE * fp)
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

//P オブジェクトをデータ構造にリンクする関数
void CCmdBase::AddLinkDoc(CData * pcData)
{
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	// pcDoc->OffFlag(ACTIVEFLAG); // ＭＰでは有効に
	pcDoc->DataAddTail((CData*)pcData);
}

//P ダミー関数
UINT CCmdBase::Exec(UINT event, DBL dx, DBL dy)
{
	return(ENDEVENT);
}

//P 矩形領域初期化関数
void CCmdBase::InitDataAreaBoxs()
{
	CTonDoc*	pcDoc;
	DBL			minix, miniy, maxx, maxy;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);

	m_dSX = minix;
	m_dSY = miniy;
	m_dEX = maxx;
	m_dEY = maxy;

	m_dBSX = minix;
	m_dBSY = miniy;
	m_dBEX = maxx;
	m_dBEY = maxy;

	m_dX1 = minix;
	m_dY1 = miniy;
	m_dX2 = minix;
	m_dY2 = maxy;
	m_dX3 = maxx;
	m_dY3 = maxy;
	m_dX4 = maxx;
	m_dY4 = miniy;
}

//P 矩形領域描画関数
void CCmdBase::DrawAreaBoxs(CDC *pcDC)
{
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		cx, cy;
	DBL		tx, ty;
	DBL		x1, y1, x2, y2;
	int		rval, gval, bval;
	int		ndot, nhdot;

	sx = m_dSX; sy = m_dSY;
	ex = m_dEX; ey = m_dEY;
	tx = 0; ty = 0;

	scrndxymmtorltv(&tx, &ty);
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	cx = (sx + ex) / 2;
	cy = (sy + ey) / 2;

	senvgetlslctclr(&rval, &gval, &bval);
	senvgetareadotsize(&ndot);
	nhdot = ndot/2;

	CPen *pcPen = new CPen(PS_SOLID, 1, RGB(rval, gval, bval));
	CPen *pOldPen = pcDC->SelectObject(pcPen);

	DrawBoxs(pcDC, sx, sy, ex, ey);

	x1 = sx - nhdot; y1 = sy - nhdot;
	x2 = x1 + ndot; y2 = y1 + ndot;
	DrawBoxs(pcDC, x1, y1, x2, y2);
	x1 = sx - nhdot; y1 = ey - nhdot;
	x2 = x1 + ndot; y2 = y1 + ndot;
	DrawBoxs(pcDC, x1, y1, x2, y2);
	x1 = ex - nhdot; y1 = ey - nhdot;
	x2 = x1 + ndot; y2 = y1 + ndot;
	DrawBoxs(pcDC, x1, y1, x2, y2);
	x1 = ex - nhdot; y1 = sy - nhdot;
	x2 = x1 + ndot; y2 = y1 + ndot;
	DrawBoxs(pcDC, x1, y1, x2, y2);

	x1 = sx - nhdot; y1 = cy - nhdot;
	x2 = x1 + ndot; y2 = y1 + ndot;
	DrawBoxs(pcDC, x1, y1, x2, y2);
	x1 = ex - nhdot; y1 = cy - nhdot;
	x2 = x1 + ndot; y2 = y1 + ndot;
	DrawBoxs(pcDC, x1, y1, x2, y2);
	x1 = cx - nhdot; y1 = sy - nhdot;
	x2 = x1 + ndot; y2 = y1 + ndot;
	DrawBoxs(pcDC, x1, y1, x2, y2);
	x1 = cx - nhdot; y1 = ey - nhdot;
	x2 = x1 + ndot; y2 = y1 + ndot;
	DrawBoxs(pcDC, x1, y1, x2, y2);

	pcDC->SelectObject(pOldPen);
	delete(pcPen);
}

//P 操作支点取得関数
void CCmdBase::GetDataCenter(DBL minix, DBL miniy, DBL maxx, DBL maxy, LPDBL cx, LPDBL cy)
{
	int		kind;

	senvgeteditconerkind(&kind);

	switch(kind){
	case LEFTDOWN:
		*cx = minix;
		*cy = miniy;
		break;
	case LEFTUP:
		*cx = minix;
		*cy = maxy;
		break;
	case RIGHTUP:
		*cx = maxx;
		*cy = maxy;
		break;
	case RIGHTDOWN:
		*cx = maxx;
		*cy = miniy;
		break;
	case CNTRCNTR:
		*cx = (minix + maxx) / 2.0;
		*cy = (miniy + maxy) / 2.0;
		break;
	}
}

//P いちばん近い文字を探す関数
CData* CCmdBase::SrchMoji(DBL dx, DBL dy, int *idx)
{
	CTonDoc*	pcDoc;
	CData*		pcData;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcData = pcDoc->SrchMoji(dx, dy, idx);
	// pcData = NULL;	// ＭＰでは
	return(pcData);
}

//P 指定領域の再描画する関数
void CCmdBase::ScrnAreaRedraw(DBL minx, DBL miny, DBL maxx, DBL maxy)
{
	RECT	rect;
	int		ndot;

	if(minx >= maxx && miny >= maxy){
		return;
	}
	sprintf(m_szMsgBuf, "%1.1lf %1.1lf %1.1lf %1.1lf\n", minx, maxx, miny, maxy);
	TRACE(m_szMsgBuf);
	//m_pcView->ValidateRect(NULL); 
	scrndxymmtorltv(&minx, &miny);
	scrndxymmtorltv(&maxx, &maxy);
	if(minx < maxx){
		rect.left = (long)minx;
		rect.right = (long)maxx;
	}
	else{
		rect.left = (long)maxx;
		rect.right = (long)minx;
	}
	if(miny < maxy){
		rect.top = (long)miny;
		rect.bottom = (long)maxy;
	}
	else{
		rect.top = (long)maxy;
		rect.bottom = (long)miny;
	}

	senvgetareadotsize(&ndot);
	ndot += 2;

	rect.left -= ndot; 
	rect.right += ndot; 
	rect.top -= ndot; 
	rect.bottom += ndot; 
	m_pcView->InvalidateRect(&rect, TRUE);
	//m_pcView->InvalidateRect(&rect, FALSE);
}

//P 指定データを再描画する関数
void CCmdBase::ScrnDataRedraw(CData *pcData)
{
	DBL		minx, miny, maxx, maxy;
	RECT	rect;
	int		ndot;

	if(pcData == NULL){
		return;
	}
	pcData->GetDMiniMax(&minx, &miny, &maxx, &maxy);
	//m_pcView->ValidateRect(NULL); 
	scrndxymmtorltv(&minx, &miny);
	scrndxymmtorltv(&maxx, &maxy);
	if(minx < maxx){
		rect.left = (long)minx;
		rect.right = (long)maxx;
	}
	else{
		rect.left = (long)maxx;
		rect.right = (long)minx;
	}
	if(miny < maxy){
		rect.top = (long)miny;
		rect.bottom = (long)maxy;
	}
	else{
		rect.top = (long)maxy;
		rect.bottom = (long)miny;
	}

	senvgetareadotsize(&ndot);
	ndot += 2;

	rect.left -= ndot; 
	rect.right += ndot; 
	rect.top -= ndot; 
	rect.bottom += ndot; 
	m_pcView->InvalidateRect(&rect, TRUE);
}

//P ＡＣＴ状態データ領域を再描画する関数
void CCmdBase::ScrnRedraw()
{
	CTonDoc*	pcDoc;
	DBL		minx, miny, maxx, maxy;
	RECT	rect;
	int		ndot;

	//m_pcView->ValidateRect(NULL); 
	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minx, &miny, &maxx, &maxy);
	scrndxymmtorltv(&minx, &miny);
	scrndxymmtorltv(&maxx, &maxy);
	if(minx < maxx){
		rect.left = (long)minx;
		rect.right = (long)maxx;
	}
	else{
		rect.left = (long)maxx;
		rect.right = (long)minx;
	}
	if(miny < maxy){
		rect.top = (long)miny;
		rect.bottom = (long)maxy;
	}
	else{
		rect.top = (long)maxy;
		rect.bottom = (long)miny;
	}

	senvgetareadotsize(&ndot);
	ndot += 2;

	rect.left -= ndot; 
	rect.right += ndot; 
	rect.top -= ndot; 
	rect.bottom += ndot; 
	m_pcView->InvalidateRect(&rect, TRUE);
}

//P ＡＴＮ（注目）状態データ領域を再描画する関数
void CCmdBase::ScrnAtntionRedraw()
{
	CTonDoc*	pcDoc;
	DBL		minx, miny, maxx, maxy;
	RECT	rect;
	int		ndot;

	//m_pcView->ValidateRect(NULL); 
	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minx, &miny, &maxx, &maxy);
	scrndxymmtorltv(&minx, &miny);
	scrndxymmtorltv(&maxx, &maxy);
	if(minx < maxx){
		rect.left = (long)minx;
		rect.right = (long)maxx;
	}
	else{
		rect.left = (long)maxx;
		rect.right = (long)minx;
	}
	if(miny < maxy){
		rect.top = (long)miny;
		rect.bottom = (long)maxy;
	}
	else{
		rect.top = (long)maxy;
		rect.bottom = (long)miny;
	}

	senvgetareadotsize(&ndot);
	ndot += 2;

	rect.left -= ndot; 
	rect.right += ndot; 
	rect.top -= ndot; 
	rect.bottom += ndot; 
	m_pcView->m_fAtntion = ON;
	m_pcView->InvalidateRect(&rect, TRUE);
}

//P 画面全体の再描画する関数
void CCmdBase::ScrnAllRedraw()
{
	m_pcView->RedrawFlagOn();
	m_pcView->Invalidate(TRUE);
}

//P 画面サイズのリセット関数
void CCmdBase::ScrnSizeReset()
{
	scrnresetzoomtime();
	scrnsetrltvelement();
	m_pcView->InitScrollElement();
}

//P ダイアログを右隅に移動させる関数
void CCmdBase::LocateRightFitDialog(CDialog * pcDialog)
{
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;

	m_pcView->GetWindowRect(&prect);
	pcDialog->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	//pcDialog->ShowWindow(SW_SHOW);
	::SetWindowPos(pcDialog->m_hWnd, HWND_NOTOPMOST, x, y, cx, cy, 0);
	//::SetWindowPos(pcDialog->m_hWnd, HWND_TOP, x, y, cx, cy, 0);
	pcDialog->ShowWindow(SW_SHOW);
	//::SetWindowPos(pcDialog->m_hWnd, HWND_TOPMOST, x, y, cx, cy, 0);
}

void CCmdBase::LocateRightBottomFitDialog(CDialog * pcDialog)
{
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;

	m_pcView->GetWindowRect(&prect);
	pcDialog->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.bottom - crect.Height() + 24;
	cx = crect.Width();
	cy = crect.Height();

	//pcDialog->ShowWindow(SW_SHOW);
	::SetWindowPos(pcDialog->m_hWnd, HWND_NOTOPMOST, x, y, cx, cy, 0);
	//::SetWindowPos(pcDialog->m_hWnd, HWND_TOP, x, y, cx, cy, 0);
	pcDialog->ShowWindow(SW_SHOW);
	//::SetWindowPos(pcDialog->m_hWnd, HWND_TOPMOST, x, y, cx, cy, 0);
}

void CCmdBase::LocateRightOverFitDialog(CDialog *pcDialog)
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;

	m_pcView->GetWindowRect(&prect);
	pcDialog->GetWindowRect(&crect);
	x = prect.right;
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	//pcDialog->ShowWindow(SW_SHOW);
	::SetWindowPos(pcDialog->m_hWnd, HWND_NOTOPMOST, x, y, cx, cy, 0);
	//::SetWindowPos(pcDialog->m_hWnd, HWND_TOP, x, y, cx, cy, 0);
	pcDialog->ShowWindow(SW_SHOW);
	//::SetWindowPos(pcDialog->m_hWnd, HWND_TOPMOST, x, y, cx, cy, 0);
}

//  すべての指定されたIDを持つデータをpcDataListにリンクします
int CCmdBase::SetDataListSelectID(int nSelectID, int nCheckFlag)
{
	CTonDoc	*pcDoc;
	int		cnt;
	int		ret;
	CData	*pcData;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	cnt = 0;
	m_cPtrList.RemoveAll( );
	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = pcObjDataList->GetNext(pos);
		if(nCheckFlag == 0){
			ret = SetDataListDispath(nSelectID, pcData);
			cnt += ret;
		}
		else if(pcData->CheckFlag(nCheckFlag) == ON){
			ret = SetDataListDispath(nSelectID, pcData);
			cnt += ret;
		}
	}
	return(cnt);
}

// リンク構造にリンクするための分岐処理
int CCmdBase::SetDataListDispath(int nSelectID, CData *pcSelectData)
{
	int		nretID;
	int		ret;

	nretID = pcSelectData->CheckID();
	if(nretID == nSelectID){
		m_cPtrList.AddTail(pcSelectData);
		return(1);
	}
	ret = 0;
	switch(nretID){
	case DATAGRUPID:
		ret = SetDataListForDataGrup(nSelectID, (CDataGrup*)pcSelectData);
		break;
	case DATALISTID:
		ret = SetDataListForDataList(nSelectID, (CDataList*)pcSelectData);
		break;
	case BASEDATAID:
		ret = 0;
		break;
	case MOJIVECTID:
		ret = SetDataListForMojiVect(nSelectID, (CMojiVect*)pcSelectData);
		break;
	case MOJITEXTID:
		ret = SetDataListForMojiText(nSelectID, (CMojiText*)pcSelectData);
		break;
	case WAKUVECTID:
		ret = SetDataListForWakuVect(nSelectID, (CWakuVect*)pcSelectData);
		break;
	case SWAKVECTID:
		ret = SetDataListForSWakVect(nSelectID, (CSWakVect*)pcSelectData);
		break;
	case STRGVECTID:
		ret = SetDataListForStrgVect(nSelectID, (CStrgVect*)pcSelectData);
		break;
	case SPTNTEXTID:
		ret = SetDataListForSPtnText(nSelectID, (CSPtnText*)pcSelectData);
		break;
	case SWAKTEXTID:
		ret = SetDataListForSWakText(nSelectID, (CSWakText*)pcSelectData);
		break;
	case STRGTEXTID:
		ret = SetDataListForStrgText(nSelectID, (CStrgText*)pcSelectData);
		break;
	case NAMEGRUPID:
		ret = SetDataListForNameGrup(nSelectID, (CNameGrup*)pcSelectData);
		break;
	case DATALOGOID:
		ret = SetDataListForDataLogo(nSelectID, (CDataLogo*)pcSelectData);
		break;
	case DATAFDIBID:
		ret = SetDataListForDataFDib(nSelectID, (CDataFDib*)pcSelectData);
		break;
	case SPTNVECTID:
		ret = SetDataListForSPtnVect(nSelectID, (CSPtnVect*)pcSelectData);
		break;
	}
	return(ret);
}

// リンク構造にDATAGRUPの要素をリンクする処理
int CCmdBase::SetDataListForDataGrup(int nSelectID, CDataGrup *pcSelectData)
{
	CData*	pcData;
	int		cnt, ret;

	cnt = 0;
	pcData = pcSelectData->m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		ret = SetDataListDispath(nSelectID, pcData);
		pcData = pcData->GetNext();
		cnt += ret;
	}
	return(cnt);
}

// リンク構造にDATALISTの要素をリンクする処理
int CCmdBase::SetDataListForDataList(int nSelectID, CDataList *pcSelectData)
{
	CData*	pcData;
	int		cnt, ret;

	cnt = 0;
	pcData = pcSelectData->m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		ret = SetDataListDispath(nSelectID, pcData);
		pcData = pcData->GetNext();
		cnt += ret;
	}
	return(cnt);
}

// リンク構造にWAKUVECTの要素をリンクする処理
int CCmdBase::SetDataListForWakuVect(int nSelectID, CWakuVect *pcSelectData)
{
	int		ret;

	ret = SetDataListForStrgVect(nSelectID, (CStrgVect*)pcSelectData);
	return(ret);
}

// リンク構造にSWAKVECTの要素をリンクする処理
int CCmdBase::SetDataListForSWakVect(int nSelectID, CSWakVect *pcSelectData)
{
	int		ret;

	ret = SetDataListForStrgVect(nSelectID, (CStrgVect*)pcSelectData);
	return(ret);
}

// リンク構造にSPTNVECTの要素をリンクする処理
int CCmdBase::SetDataListForSPtnVect(int nSelectID, CSPtnVect *pcSelectData)
{
	int		ret;

	ret = SetDataListForStrgVect(nSelectID, (CStrgVect*)pcSelectData);
	return(ret);
}

// リンク構造にSTRGVECTの要素をリンクする処理
int CCmdBase::SetDataListForStrgVect(int nSelectID, CStrgVect *pcSelectData)
{
	CMojiVect*	pcMojiVect;
	int		cnt, ret;

	cnt = 0;
	pcMojiVect = pcSelectData->m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		ret = SetDataListDispath(nSelectID, (CData*)pcMojiVect);
		pcMojiVect = pcMojiVect->GetNextMoji();
		cnt += ret;
	}
	return(cnt);
}

// リンク構造にMOJIVECTの要素をリンクする処理
int CCmdBase::SetDataListForMojiVect(int nSelectID, CMojiVect *pcSelectData)
{
	int		ret;

	ret = 0;
	if(pcSelectData->m_pcDataList != NULL){
		ret = SetDataListForDataList(nSelectID, pcSelectData->m_pcDataList);
	}
	return(ret);
}

// リンク構造にSWAKTEXTの要素をリンクする処理
int CCmdBase::SetDataListForSWakText(int nSelectID, CSWakText *pcSelectData)
{
	int		ret;

	if(pcSelectData->m_nWakuKind == BMPWAKU || pcSelectData->m_nWakuKind == LOGOWAKU){
		if(pcSelectData->m_pcDataFDib == NULL){
			return(0);
		}
		ret = SetDataListForDataFDib(nSelectID, pcSelectData->m_pcDataFDib);
	}
	else{
		ret = SetDataListForStrgText(nSelectID, (CStrgText*)pcSelectData);
	}
	return(ret);
}

// リンク構造にSPTNVECTの要素をリンクする処理
int CCmdBase::SetDataListForSPtnText(int nSelectID, CSPtnText *pcSelectData)
{
	int		ret;

	ret = SetDataListForStrgText(nSelectID, (CStrgText*)pcSelectData);
	return(ret);
}

// リンク構造にSTRGVECTの要素をリンクする処理
int CCmdBase::SetDataListForStrgText(int nSelectID, CStrgText *pcSelectData)
{
	CMojiText*	pcMojiText;
	int		cnt, ret;

	cnt = 0;
	POSITION pos = pcSelectData->m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMojiText = pcSelectData->m_cMojiTextList.GetNext(pos);
		ret = SetDataListDispath(nSelectID, (CData*)pcMojiText);
		cnt += ret;
	}
	return(cnt);
}

// リンク構造にNAMEGRUPの要素をリンクする処理
int CCmdBase::SetDataListForNameGrup(int nSelectID, CNameGrup *pcSelectData)
{
	CData*	pcData;
	int		cnt, ret;

	cnt = 0;
	pcData = pcSelectData->m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		ret = SetDataListDispath(nSelectID, pcData);
		pcData = pcData->GetNext();
		cnt += ret;
	}
	return(cnt);
}

// リンク構造にMOJIVECTの要素をリンクする処理
int CCmdBase::SetDataListForMojiText(int nSelectID, CMojiText *pcSelectData)
{
	int		ret;

	ret = 0;
	if(pcSelectData->m_pcDataList != NULL){
		ret = SetDataListForDataList(nSelectID, pcSelectData->m_pcDataList);
	}
	return(ret);
}

// リンク構造にDATALOGOの要素をリンクする処理
int CCmdBase::SetDataListForDataLogo(int nSelectID, CDataLogo *pcSelectData)
{
	CData*	pcData;
	int		cnt, ret;

	cnt = 0;
	pcData = pcSelectData->m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		ret = SetDataListDispath(nSelectID, pcData);
		pcData = pcData->GetNext();
		cnt += ret;
	}
	return(cnt);
}

// リンク構造にDATAFDIBの要素をリンクする処理
int CCmdBase::SetDataListForDataFDib(int nSelectID, CDataFDib *pcSelectData)
{
	CData*	pcData;
	int		cnt, ret;

	cnt = 0;
	ret = SetDataListDispath(nSelectID, (CData*)(pcSelectData->m_pcDibCData));
	cnt += ret;
	pcData = pcSelectData->m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		ret = SetDataListDispath(nSelectID, pcData);
		pcData = pcData->GetNext();
		cnt += ret;
	}
	return(cnt);
}

// ACTフラグをすべてOFFにする関数
void CCmdBase::ActiveDataAllOff()
{
	CTonDoc	*pcDoc;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->OffFlag(ACTIVEFLAG);
}

//p m_cPtrList構造内データの最大最小値を求める関数
int CCmdBase::DataListGetDMiniMax(LPDBL minx, LPDBL miny, LPDBL maxx, LPDBL maxy)
//id DBL　*minx, *miny, *maxx, *maxy 不定
//od DBL　*minx, *miny, *maxx, *maxy 最大最小値
//rd 成功　TRUE　、失敗　FALSE
{
	POSITION	pos;
	CData	*pcData;
	DBL		tminx, tminy, tmaxx, tmaxy;

	(*minx) = INTMAX;
	(*miny) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		if(pcData == NULL){
			break;
		}
		pcData->GetDMiniMax(&tminx, &tminy, &tmaxx, &tmaxy);
		(*minx) = min((*minx), tminx);
		(*miny) = min((*miny), tminy);
		(*maxx) = max((*maxx), tmaxx);
		(*maxy) = max((*maxy), tmaxy);
	}
	return(TRUE);
}

// 絶対座標をMM座標に変換する関数
void CCmdBase::ScrnCnvCoodToMM(LPDBL mmx, LPDBL mmy)
{
	m_pcSysEnv->ScrnCnvCoodToMM(mmx, mmy);	// ＭＰでは無効に
}

// MM座標を絶対座標に変換する関数
void CCmdBase::ScrnCnvMMToCood(LPDBL coodx, LPDBL coody)
{
	m_pcSysEnv->ScrnCnvMMToCood(coodx, coody);	// ＭＰでは無効に
}

// 絶対座標移動値を取得する関数
void CCmdBase::ScrnGetCoodMoveXY(LPDBL movex, LPDBL movey)
{

}

// UNDO保存関数
void CCmdBase::SaveUndo()
{
	CTonDoc	*pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->SaveUndo();
}

// UNDO処理関数
void CCmdBase::LoadUndo()
{
	CTonDoc	*pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->LoadUndo();
}

// ベクトルデータの指定領域をコピーする関数
int CCmdBase::AreaCopy(CVect * srcVect, int spp, int epp, CVect * dstVect, int dp)
{
	int		pp;
	int		atr;
	DBL		dx, dy;

	for(pp = spp; pp <= epp; pp++, dp++){
		srcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		dstVect->SetAtrDXY(dp, atr, dx, dy);
	}
	return(dp);
}

//P 操作支点初期化関数
DBL CCmdBase::CntrKindInitNew(DBL dx, DBL dy)
{
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		cx, cy;
	DBL		tx, ty;
	DBL		len, tlen;
	int		ndot;
	DBL		ddot;

	tx = 0; ty = 0;
	sx = m_dSX;
	sy = m_dSY;
	ex = m_dEX;
	ey = m_dEY;

	cx = (sx + ex) / 2;
	cy = (sy + ey) / 2;

	senvgetareadotsize(&ndot);
	ddot = ndot;
	scrndlenrltvtomm(&ddot);

	len = INTMAX;
	if(fabs(m_dSX - m_dEX) > ddot*2 && fabs(m_dSY - m_dEY) > ddot*2){
		tlen = hypot(sx-dx, sy-dy);
		if(len > tlen){
			m_nCntrKind = LEFTUP;
			len = tlen;
			tx = sx;
			ty = sy;
		}
		tlen = hypot(sx-dx, ey-dy);
		if(len > tlen){
			m_nCntrKind = LEFTDOWN;
			len = tlen;
			tx = sx;
			ty = ey;
		}
		tlen = hypot(ex-dx, ey-dy);
		if(len > tlen){
			m_nCntrKind = RIGHTDOWN;
			len = tlen;
			tx = ex;
			ty = ey;
		}
		tlen = hypot(ex-dx, sy-dy);
		if(len > tlen){
			m_nCntrKind = RIGHTUP;
			len = tlen;
			tx = ex;
			ty = sy;
		}
	}
	if(fabs(m_dSX - m_dEX) > ddot*2){
		tlen = hypot(sx-dx, cy-dy);
		if(len > tlen){
			m_nCntrKind = LEFTCNTR;
			len = tlen;
			tx = sx;
			ty = cy;
		}
		tlen = hypot(ex-dx, cy-dy);
		if(len > tlen){
			m_nCntrKind = RIGHTCNTR;
			len = tlen;
			tx = ex;
			ty = cy;
		}
	}
	if(fabs(m_dSY - m_dEY) > ddot*2){
		tlen = hypot(cx-dx, sy-dy);
		if(len > tlen){
			m_nCntrKind = CNTRUP;
			len = tlen;
			tx = cx;
			ty = sy;
		}
		tlen = hypot(cx-dx, ey-dy);
		if(len > tlen){
			m_nCntrKind = CNTRDOWN;
			len = tlen;
			tx = cx;
			ty = ey;
		}
	}
	m_dBX = tx;
	m_dBY = ty;
	m_dCX = dx;
	m_dCY = dy;
	m_dBSX = m_dSX;
	m_dBEX = m_dEX;
	m_dBSY = m_dSY;
	m_dBEY = m_dEY;
	return(len);
}

//P 操作支点移動関数
void CCmdBase::CntrKindMoveNew(DBL dx, DBL dy)
{
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		movex, movey;
	DBL		tmovex, tmovey;

	//CTonApp *theApp = (CTonApp*)AfxGetApp();
	//CSysEnv *pcSysEnv = theApp->m_pcSysEnv;

	if(m_dBSX == m_dBEX){
		m_dXYTime = 1;
	}
	else{
		switch(m_nCntrKind){
		case LEFTUP:
		case RIGHTDOWN:
			m_dXYTime = (m_dBEX - m_dBSX) / (m_dBEY - m_dBSY);
			break;
		case LEFTDOWN:
		case RIGHTUP:
			m_dXYTime = (m_dBEX - m_dBSX) / (m_dBSY - m_dBEY);
			break;
		}
		if(-0.00001 < m_dXYTime && m_dXYTime < 0.00001){
			m_dXYTime = 1.0;
		}
		if(m_dXYTime < -10000 || 10000 < m_dXYTime){
			m_dXYTime = 1.0;
		}
	}

	movex = dx - m_dBX;
	movey = dy - m_dBY;

	if(m_pcView->m_nKeyFlags & MK_SHIFT){
		if(fabs(m_dXYTime)){
			tmovex = movex;
			tmovey = movex / m_dXYTime;
		}
		else{
			tmovex = movey / m_dXYTime;
			tmovey = movey;
		}
	
	}
	else{
		tmovex = movex;
		tmovey = movey;
	}
	sx = m_dBSX;
	ex = m_dBEX;
	sy = m_dBSY;
	ey = m_dBEY;
	switch(m_nCntrKind){
	case LEFTUP:
		sx += tmovex;
		sy += tmovey;
		break;
	case LEFTDOWN:
		sx += tmovex;
		ey += tmovey;
		break;
	case RIGHTDOWN:
		ex += tmovex;
		ey += tmovey;
		break;
	case RIGHTUP:
		ex += tmovex;
		sy += tmovey;
		break;
	case LEFTCNTR:
		sx += movex;
		break;
	case RIGHTCNTR:
		ex += movex;
		break;
	case CNTRUP:
		sy += movey;
		break;
	case CNTRDOWN:
		ey += movey;
		break;
	}
	m_dSX = sx;
	m_dSY = sy;
	m_dEX = ex;
	m_dEY = ey;

	m_dCX = dx;
	m_dCY = dy;
}

//P 操作支点初期化関数
DBL CCmdBase::CntrKindInit(DBL dx, DBL dy)
{
	m_dBX = dx;
	m_dBY = dy;
	m_nCntrKind = CNTRCNTR;
	m_dCX = dx;
	m_dCY = dy;
	return(0.0);
}

//P 操作支点移動関数
void CCmdBase::CntrKindMove(DBL dx, DBL dy)
{
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		movex, movey;

	movex = dx - m_dBX;
	movey = dy - m_dBY;
	sx = m_dBSX;
	ex = m_dBEX;
	sy = m_dBSY;
	ey = m_dBEY;
	if(m_pcView->m_nKeyFlags & MK_SHIFT){
		if(fabs(movex) > fabs(movey)){
			sx += movex;
			ex += movex;
		}
		else{
			sy += movey;
			ey += movey;
		}
	}
	else{
		sx += movex;
		sy += movey;
		ex += movex;
		ey += movey;
	}
	m_dSX = sx;
	m_dSY = sy;
	m_dEX = ex;
	m_dEY = ey;

	m_dCX = dx;
	m_dCY = dy;
}

void CCmdBase::CheckSetLimit(DBL *num, DBL min, DBL max)
{
	if(*num < min){
		*num = min;
	}
	if(*num > max){
		*num = max;
	}
}

void CCmdBase::CheckSetLimit(int *num, int min, int max)
{
	if(*num < min){
		*num = min;
	}
	if(*num > max){
		*num = max;
	}
}

CData* CCmdBase::DataChange(CData *pcData)
{
	CDataGrup*	pcDataGrup;
	CTonDoc*	pcDoc;

	if(pcData->CheckDataChange() != TRUE){
		return(NULL);
	}
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDataGrup = (CDataGrup*)(pcDoc->NewData(DATAGRUPID));
	pcDoc->DataAddTail((CData *)pcDataGrup);
	pcData->DataChangeSetDataGrup(pcDataGrup);
	pcDoc->DeleteData(pcData);
	return(pcDataGrup);
}

void CCmdBase::ActiveDataChange(int nID)
{
	CData	*pcData;
	CSWakText	*pcSWakText;

	SetDataListSelectID(nID, ACTIVEFLAG);
	POSITION pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		if(nID == SWAKTEXTID){
			pcSWakText = (CSWakText *)pcData;
			if(pcSWakText->m_nWakuKind == BMPWAKU || pcSWakText->m_nWakuKind == LOGOWAKU){
				//pcSWakText->OffFlag(ACTIVEFLAG);
				continue;
			}
		}
		DataChange(pcData);
	}	
}

void CCmdBase::SWakBmpLogoActiveOff()
{
	CData	*pcData;
	CSWakText	*pcSWakText;

	SetDataListSelectID(SWAKTEXTID, ACTIVEFLAG);
	POSITION pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		pcSWakText = (CSWakText *)pcData;
		if(pcSWakText->m_nWakuKind == BMPWAKU || pcSWakText->m_nWakuKind == LOGOWAKU){
			pcSWakText->OffFlag(ACTIVEFLAG);
		}
	}	
}

void CCmdBase::MoveMousePoint(DBL crtx, DBL crty, DBL dstx, DBL dsty)
{
	POINT MousePt;
	DBL		adx, ady;
	DBL		tadx, tady;

	::GetCursorPos(&MousePt);
	tadx = m_pcView->m_nBx - MousePt.x;
	tady = m_pcView->m_nBy - MousePt.y;
	scrndxymmtorltv(&crtx, &crty);
	adx = MousePt.x - crtx;
	ady = MousePt.y - crty;
	scrndxymmtorltv(&dstx, &dsty);
	((CTonView*)m_pcView)->MouseSetCrt(dstx, dsty);
	MousePt.x = (long)(dstx+adx);
	MousePt.y = (long)(dsty+ady);
	m_pcView->m_nBx = MousePt.x + (int)tadx;
	m_pcView->m_nBy = MousePt.y + (int)tady;
	::SetCursorPos(MousePt.x, MousePt.y);
}

void CCmdBase::SetMousePoint(DBL dx, DBL dy)
{
	CRect	rect;
	POINT	MousePt;

	scrndxymmtorltv(&dx, &dy);
	MousePt.x = (int)dx;
	MousePt.y = (int)dy;
	m_pcView->ClientToScreen(&MousePt);
	m_pcView->m_nBx = MousePt.x;
	m_pcView->m_nBy = MousePt.y;
	::SetCursorPos(MousePt.x, MousePt.y);
}

DBL CCmdBase::GetSumLength(CVect *pcVect, int sp, int ep)
{
	int		pcnt, pp, tp;
	int		atr;
	DBL		x1, y1, x2, y2;
	DBL		len, sumlen;

	pcnt = pcVect->GetPointCount();
	if(sp > ep){
		ep = ep + pcnt;
	}
	tp = pcVect->PointLegal(pcnt, sp);
	pcVect->GetAtrDXY(tp, &atr, &x1, &y1);
	sumlen = 0;
	for(pp = sp+1; pp <= ep; pp++){
		tp = pcVect->PointLegal(pcnt, pp);
		pcVect->GetAtrDXY(tp, &atr, &x2, &y2);
		len = hypot(x2-x1, y2-y1);
		sumlen = sumlen + len;
		x1 = x2; y1 = y2;
	}
	return(sumlen);
}

void CCmdBase::GetLengthPoint(CVect *pcVect, int sp, int ep, DBL len, DBL *x, DBL *y)
{
	int		pcnt, pp, tp;
	int		atr;
	DBL		x1, y1, x2, y2;
	DBL		tlen;

	x2 = 0; y2 = 0;
	pcnt = pcVect->GetPointCount();
	if(sp > ep){
		ep = ep + pcnt;
	}
	tp = pcVect->PointLegal(pcnt, sp);
	pcVect->GetAtrDXY(tp, &atr, &x1, &y1);
	for(pp = sp+1; pp <= ep; pp++){
		tp = pcVect->PointLegal(pcnt, pp);
		pcVect->GetAtrDXY(tp, &atr, &x2, &y2);
		tlen = hypot(x2-x1, y2-y1);
		if(tlen > len){
			*x = (x2-x1) * len / tlen + x1;
			*y = (y2-y1) * len / tlen + y1;
			return;
		}
		else{
			len -= tlen;
		}
		x1 = x2; y1 = y2;
	}
	*x = x2;
	*y = y2;
}

