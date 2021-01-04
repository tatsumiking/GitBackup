
/////////////////////////////////////////////////////////////////////////////
// CCmdDibC

#include "stdafx.h"
#include "stdio.h"
//#include "..\imgkit\imagekit.h"

#include "CmdBase.h"
#include "CmdBaseBmp.h"
#include "CmdDibC.h"

#include "BmpFile.h"
#include "DialogBmpRound.h"
#include "DialogImageSave.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Diblib.h"
#include "../../ObjLib/Curvelib.h"
#include "../../ObjLib/Mdfylib.h"
//}

#define	POINTMAX	8192

static TCHAR BASED_CODE szFilter[] = _T("画像ファイル (*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png||");

CCmdDibC::CCmdDibC(CScrollView* pcview)
:CCmdBaseBmp(pcview)
{
	TCHAR	datpath[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	FILE	*fp;

	//_tcscpy_s(m_szEditProg, "Pbrush");
	_tcscpy_s(m_szEditProg, _T("mspaint"));
	m_nImageXSize = 1024;
	m_nImageYSize = 768;
	m_nQuality = 50;
	m_nRotate = ON;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_dibc.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), m_szEditProg, FNAMEMAX);
		if(baselibCrDelFgets(buf, RECMAX, fp) != NULL){
			_stscanf_s(buf, _T("%d %d"), &m_nImageXSize, &m_nImageYSize);
		}
		else{
			m_nImageXSize = 1024;
			m_nImageYSize = 768;
		}
		if(baselibCrDelFgets(buf, RECMAX, fp) != NULL){
			_stscanf_s(buf, _T("%d"), &m_nQuality);
		}
		else{
			m_nQuality = 50;
		}
		if(baselibCrDelFgets(buf, RECMAX, fp) != NULL){
			_stscanf_s(buf, _T("%d"), &m_nRotate);
		}
		else{
			m_nRotate = ON;
		}
		fclose(fp);
	}

	senvgetdatpath(datpath);
	_tcscpy_s(m_szJpgInitPath, datpath);
	m_nJpgXSize = 1024;
	m_nJpgYSize = 768;
	m_nJpgQuality = 50;
	m_nJpgRotate = ON;
	m_szJpgFname[0] = _T('\0');
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\ImageOut.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		if(baselibCrDelFgets(m_szJpgInitPath, FNAMEMAX, fp) != NULL){
		}
		if(baselibCrDelFgets(buf, RECMAX, fp) != NULL){
			_stscanf_s(buf, _T("%d %d"), &m_nJpgXSize, &m_nJpgYSize);
		}
		if(baselibCrDelFgets(buf, RECMAX, fp) != NULL){
			_stscanf_s(buf, _T("%d"), &m_nJpgQuality);
		}
		if(baselibCrDelFgets(buf, RECMAX, fp) != NULL){
			_stscanf_s(buf, _T("%d"), &m_nJpgRotate);
		}
		fclose(fp);
	}
}

CCmdDibC::~CCmdDibC()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\ImageOut.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if(fp != NULL){
		_stprintf_s(buf, _T("%s\n"), m_szJpgInitPath);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d %d\n"), m_nJpgXSize, m_nJpgYSize);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d\n"), m_nJpgQuality);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d\n"), m_nJpgRotate);
		baselibFputs(buf, fp);
		fclose(fp);
	}
}

UINT CCmdDibC::ExecBmpLoad(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		if(LoadDibCData(NULL) == FALSE){
			return(ENDEVENT);
		}
		InitDataAreaBoxs();
		BeginDraw();
		DrawAreaBoxs(m_pcDC);
		EndDraw();
		m_nMouseSetCount = 1;	
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			CntrKindInitNew(dx, dy);
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 2;
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			CntrKindMoveNew(dx, dy);
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 2){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			CntrKindMoveNew(dx, dy);
			DrawAreaBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			SaveUndo();
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			SetDibCDataSize();
			m_pcDibCData->RenewMiniMax();
			ScrnRedraw();
			ActiveDataAllOff();
			m_pcDoc->DataAddHead(m_pcDibCData);
			m_nMouseSetCount = 0;
			ScrnDataRedraw((CData*)m_pcDibCData);
			return(ENDEVENT);
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			CntrKindMoveNew(m_dBX, m_dBY);
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			EndDraw();
		}
		delete(m_pcDibCData);
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdDibC::ExecTrace(UINT event, DBL dx, DBL dy)
{
	POSITION pos;
	int		cnt;

	switch(event){
	case INITEVENT:
		InitClass();
		m_dFureLimit = 1;
		cnt = SetDataListSelectID(DIBCDATAID, ACTIVEFLAG);
		if(cnt == 0){
			return(ENDEVENT);
		}
		pos = m_cPtrList.GetHeadPosition();
		m_pcDibCData = (CDibCData *)m_cPtrList.GetNext(pos);
		diblibcreateimgfilename(m_szBmpFname);
		m_pcDialogTraceImage = new CDialogTraceImage(m_pcView);
		m_pcDialogTraceImage->m_nFureLimit = (int)m_dFureLimit;
		if(m_pcDialogTraceImage->Create(IDD_TRACE_IMAGE, m_pcView)){
			LocateRightFitDialog(m_pcDialogTraceImage);
			m_pcDialogTraceImage->InitItemElement();
			m_pcDialogTraceImage->SetItemElement();
		}
		m_clrPicup = 0;
		SetTraceClr();
		m_nTrace = OFF;
		break;
	case SETEVENT:
		if(m_nTrace == OFF){
			m_clrPicup = m_pcDibCData->GetPicupClr(dx, dy);
			SetTraceClr();
		}
		break;
	case DLGSETEVENT:
		if(m_nTrace == OFF){
			m_nTrace = ON;
			SaveUndo();
			TraceMain();
			m_pcDialogTraceImage->DestroyWindow();
			if(::AfxMessageBox(_T("元画像は削除しますか"), MB_YESNO) == IDYES){
				m_pcDoc->DeleteData(m_pcDibCData);
				delete(m_pcDibCData);
			}
			m_nTrace = OFF;
			//m_pcCmdCrdnt->ExecListReset(INITEVENT, 0.0, 0.0);
			ScrnAllRedraw();
			return(ENDEVENT);
		}
		break;
	case DLGRESETEVENT:
		if(m_nTrace == OFF){
			m_clrPicup = m_pcDialogTraceImage->m_clrPicup;
			SetTraceClr();
		}
		break;
	case DLGENDEVENT:
	case RESETEVENT:
	case ENDEVENT:
		if(m_nTrace == OFF){
			m_pcDialogTraceImage->DestroyWindow();
			return(ENDEVENT);
		}
		break;
	}
	return(0);
}

UINT CCmdDibC::ExecRound(UINT event, DBL dx, DBL dy)
{
	int		mode;
	DBL		cntrx, cntry;
	DBL		sx, sy, ex, ey;
	DBL		size;

	switch(event){
	case INITEVENT:
		InitClass();
		break;
	case SETEVENT:
		m_pcDibCData = m_pcDoc->SrchDibData(dx, dy);
		if(m_pcDibCData != NULL){
			SaveUndo();
			CDialogBmpRound *pcDialogBmpRound = new CDialogBmpRound(m_pcView); 
			if(pcDialogBmpRound->DoModal() == IDOK){
				mode = pcDialogBmpRound->m_nMode;
				m_pcDibCData->SetTrnsMode(mode+1);
				m_pcDibCData->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				cntrx = (m_dBSX + m_dBEX) / 2;
				cntry = (m_dBSY + m_dBEY) / 2;

				sx = m_pcDibCData->m_dDstSx;
				sy = m_pcDibCData->m_dDstSy;
				ex = sx + m_pcDibCData->m_dDstWidth;
				ey = sy + m_pcDibCData->m_dDstHight;
				if(mode == 1 || mode == 2){
					size  = m_pcDibCData->m_dDstWidth;
					m_pcDibCData->m_dDstWidth = m_pcDibCData->m_dDstHight;
					m_pcDibCData->m_dDstHight = size;
				}
				if(mode == 1 || mode == 2){
					sx = sx - cntrx;
					sy = sy - cntry;
					m_pcDibCData->m_dDstSx = sy + cntrx;
					m_pcDibCData->m_dDstSy = sx + cntry;
					m_pcDibCData->RenewMiniMax();
				}
				m_pcDibCData->FileTrnsSave();
				m_pcDibCData->FileLoad();
				m_pcView->RedrawFlagOn();
				ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				ScrnDataRedraw((CData*)m_pcDibCData);
				//return(REDRAWEVENT);
			}
		}
		break;
	case DLGSETEVENT:
		return(ENDEVENT);
	case DLGENDEVENT:
	case RESETEVENT:
	case ENDEVENT:
		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdDibC::ExecDotedit(UINT event, DBL dx, DBL dy)
{
	TCHAR	OutProg[RECMAX];

	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		m_pcDibCData = NULL;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_nMouseSetCount = 1;
			m_pcDibCData = m_pcDoc->SrchDibData(dx, dy);
			if(m_pcDibCData != NULL){
				SaveUndo();
				_stprintf_s(OutProg, _T("%s \"%s\""), m_szEditProg, m_pcDibCData->m_szImgFileName);
				::WinExec((LPCSTR)OutProg, SW_SHOW);
				// HlpMsgDsp("右ボタンでＢＭＰを更新します");
			}
		}
		break;
	case SETFOCUSEVENT:
		if(m_pcDibCData != NULL){
			m_pcDibCData->InitElement();
			ScrnAllRedraw();
		}
		//return(REDRAWEVENT);
		break;
	case ENDEVENT:
		if(m_pcDibCData != NULL){
			m_pcDibCData->InitElement();
			ScrnAllRedraw();
		}
		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdDibC::ExecBmping(UINT event, DBL dx, DBL dy)
{
	DBL		sx, sy, ex, ey;
	DBL		wd, hi;
	TCHAR	fname[FNAMEMAX];
	COLORREF	clr1, clr2;
	DBL			thick;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	//m_pcCmdCrdnt->ExecListSet(INITEVENT, 0.0, 0.0);
	m_pcDibCData = (CDibCData*)(m_pcDoc->NewData(DIBCDATAID));
	if(m_pcDibCData == NULL){
		return(ENDEVENT);
	}
	if(m_pcDoc->DataActiveCheck() == FALSE){
		return(ENDEVENT);
	}
	SaveUndo();
	m_pcDoc->GetDMiniMax(ACTIVEFLAG, &sx, &sy, &ex, &ey);
	wd = ex - sx;
	hi = ey - sy;

	fname[0] = _T('\0');
	diblibcreateimgfilename(fname);
	InitActiveDrawArea();
	InitImage();
	//m_pcMemDC->BitBlt(0, 0, m_nXSize, m_nYSize, NULL, 0, 0, WHITENESS);
	m_pcMemDC->BitBlt(0, 0, m_nXSize, m_nYSize, NULL, 0, 0, BLACKNESS);
	clr1 = RGB(255, 255, 255);
	clr2 = clr1;
	thick = 0;
	m_pcView->CheckFlagDrawSetClr(ACTIVEFLAG, m_pcMemDC, clr1, clr2, thick);
	InitBits();
	int ret = m_pcBitmap->GetBitmapBits(m_lBitsSize, m_lpBits);
	int	i;
	for(i = 0; i < m_lBitsSize; i++){
		m_lpBits[i] = ~m_lpBits[i];
	}
	CreateBmpFile(fname, m_lpBits);
	EndBits();
	EndImage();	
	EndActiveDrawArea();

	wd = m_dAreaXMM;
	hi = m_dAreaYMM;

	m_pcDibCData->SetFileName(fname);
	m_pcDibCData->InitElement();
	m_pcDibCData->SetDstElement(sx, sy, wd, hi);
	m_pcDibCData->RenewMiniMax();

	if(::AfxMessageBox(_T("元図形は削除しますか"), MB_YESNO) == IDYES){
		m_pcDoc->DataActiveCls();
	}
	m_pcDoc->DataAddHead(m_pcDibCData);

	ScrnAllRedraw();
	return(ENDEVENT);
}
// 処理できない
UINT CCmdDibC::ExecAreaDel(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_dSX = dx; m_dSY = dy;
			m_dEX = dx; m_dEY = dy;
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			m_dEX = dx; m_dEY = dy;
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
			dx = (m_dSX + m_dEX) / 2.0;
			dy = (m_dSY + m_dEY) / 2.0;
			m_pcDibCData = m_pcDoc->SrchDibData(dx, dy);
			if(m_pcDibCData != NULL){
				if(m_pcView->m_nKeyFlags & MK_SHIFT){
					m_pcDibCData->SetArea(m_dSX, m_dSY, m_dEX, m_dEY);
				}
				else{
					m_pcDibCData->DelArea(m_dSX, m_dSY, m_dEX, m_dEY);
				}
				m_pcDibCData->FileDibSave();
				ScrnDataRedraw(m_pcDibCData);
			}
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			m_dEX = dx; m_dEY = dy;
			DrawAreaBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			EndDraw();
		}
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdDibC::ExecSave(UINT event, DBL dx, DBL dy)
{
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	m_szJpgFname[0] = _T('\0');
	// ビットマップイメージ保存
	ret = SaveImageFile();
	if(ret == TRUE){
		ChangeCopyItiFile();
		SaveDocument();
	}

	ScrnAllRedraw();
	return(ENDEVENT);		
}

UINT CCmdDibC::ExecJpgSave(UINT event, DBL dx, DBL dy)
{
	int		ret;
	CDialogImageSave	*pcDialogImageSave;
	CString	csFileName;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	m_szJpgFname[0] = _T('\0');
	pcDialogImageSave = new CDialogImageSave(m_szJpgInitPath);
	pcDialogImageSave->m_nXSize = m_nJpgXSize;
	pcDialogImageSave->m_nYSize = m_nJpgYSize;
	pcDialogImageSave->m_nQuality = m_nJpgQuality;
	pcDialogImageSave->m_nRotate = m_nJpgRotate;

	ret = pcDialogImageSave->DoModal();
	if(ret == IDOK){
		m_nJpgXSize = pcDialogImageSave->m_nXSize;
		m_nJpgYSize = pcDialogImageSave->m_nYSize;
		m_nJpgQuality = pcDialogImageSave->m_nQuality;
		m_nJpgRotate = pcDialogImageSave->m_nRotate;
		csFileName = pcDialogImageSave->GetPathName();
		_tcscpy_s(m_szJpgFname, csFileName.GetBuffer(FNAMEMAX));
		_tcscpy_s(m_szJpgInitPath, m_szJpgFname);
		baseaddsafix(m_szJpgFname, _T("jpg"));
	}
	delete(pcDialogImageSave);
	if(ret != IDOK){
		return(ENDEVENT);		
	}

	// ビットマップイメージ保存
	m_nImageXSize = m_nJpgXSize;
	m_nImageYSize = m_nJpgYSize;
	m_nQuality = m_nJpgQuality;
	m_nRotate = m_nJpgRotate;
	SaveImageFile();
	ScrnAllRedraw();
	return(ENDEVENT);		
}

UINT CCmdDibC::ExecClip(UINT event, DBL dx, DBL dy)
{
	POSITION pos;
	CData* pcData;
	int nID;
	CClipData *pcClipData;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	m_pcDibCData = NULL;
	m_pcDataList = NULL;
	pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			nID = pcData->CheckID();
			if(nID == DIBCDATAID){
				m_pcDibCData = (CDibCData*)pcData;
			}
			else if(nID == DATALISTID){
				m_pcDataList = (CDataList*)pcData;
			}
		}
	}
	if(m_pcDibCData == NULL){
		::AfxMessageBox(_T("選択状態の画像がありません"), MB_OK);
		return(ENDEVENT);
	}
	if(m_pcDataList == NULL){
		::AfxMessageBox(_T("選択状態の図形データがありません"), MB_OK);
		return(ENDEVENT);
	}
	m_pcDoc->DeleteData(m_pcDibCData);
	m_pcDoc->DeleteData(m_pcDataList);
	pcClipData = (CClipData*)(m_pcDoc->NewData(CLIPDATAID));
	pcClipData->AddClip(m_pcDataList);
	pcClipData->AddData(m_pcDibCData);
	pcClipData->RenewMiniMax();
	m_pcDoc->DataAddTail(pcClipData);
	ScrnAllRedraw();
	return(ENDEVENT);		
}
UINT CCmdDibC::ExecBkLoad(UINT event, DBL dx, DBL dy)
{
	DBL xsize, ysize;

	if(event != INITEVENT){
		return(0);
	}

	InitClass();
	if(LoadDibCData(m_pcApp->m_szBackImagePath) == FALSE){
		return(ENDEVENT);
	}
	scrngetmmsize(&xsize, &ysize);
	m_pcDibCData->SetDstElement(0.0, 0.0, xsize, ysize);
	m_pcDibCData->RenewMiniMax();
	m_pcDoc->DataAddTail0(m_pcDibCData);
	ScrnAllRedraw();
	return(ENDEVENT);		
}

UINT CCmdDibC::ExecBkReset(UINT event, DBL dx, DBL dy)
{
	POSITION pos;
	CData*		pcData;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pos = m_pcDoc->m_ObjDataList0.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_pcDoc->m_ObjDataList0.GetNext(pos);
		m_pcDoc->DataAddTail(pcData);
	}
	m_pcDoc->m_ObjDataList0.RemoveAll();
	return(ENDEVENT);		
}

BOOL CCmdDibC::LoadDibCData(LPTSTR lpPath)
{
	TCHAR	fname[FNAMEMAX];

	CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, _T("*.bmp;*.jpg;*.png"), NULL, szFilter, m_pcView);
	if(lpPath != NULL){
		pcFileDialog->SetInitPath(lpPath);
	}
	pcFileDialog->SetTitle(_T("画像読込"));
	if(pcFileDialog->DoModal( ) == IDOK){
		CString cpStr = pcFileDialog->GetPathName();	
		_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
		m_pcDibCData = (CDibCData*)(m_pcDoc->NewData(DIBCDATAID));
		if(m_pcDibCData == NULL){
			return(FALSE);
		}
		m_pcDibCData->SetFileName(fname);
		m_pcDibCData->InitElement();
		m_pcDibCData->RenewMiniMax();
		return(TRUE);
	}
	return(FALSE);
}

void CCmdDibC::InitDataAreaBoxs()
{
	DBL	minix, miniy, maxx, maxy;
	DBL	xmm, ymm, xsize, ysize;
	DBL	time;

	m_pcDibCData->GetDMiniMax(&minix, &miniy, &maxx, &maxy);

	xsize = maxx - minix;
	ysize = maxy - miniy;

	scrngetmmsize(&xmm, &ymm);
	xmm = xmm / 3.0 * 2.0;
	ymm = ymm / 3.0 * 2.0;
	if(xsize > xmm || ysize > ymm){
		if(xsize > xmm){
			time = xmm / xsize;
			xsize = xmm;
			ysize = ysize * time;
		}
		if(ysize > ymm){
			time = ymm / ysize;
			ysize = ymm;
			xsize = xsize * time;
		}

		minix = xmm / 6;
		miniy = ymm / 6;
		maxx = minix + xsize;
		maxy = miniy + ysize;
	}

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

void CCmdDibC::SetDibCDataSize()
{
	m_pcDibCData->SetDstElement(m_dSX, m_dSY, m_dEX-m_dSX, m_dEY-m_dSY);
}

void CCmdDibC::SetTraceClr()
{
	DBL		xsize, ysize;
	DBL		dpix, dpiy, wd, hi;
	DBL		timex, timey, time;
	TCHAR	srcfname[FNAMEMAX];

	m_pcDialogTraceImage->SetClr(m_clrPicup);
	m_pcDialogTraceImage->GetImageAreaSize(&xsize, &ysize);
	m_pcDibCData->GetFileName(srcfname);
	m_pcDibCData->GetSrcElement(&dpix, &dpiy, &wd, &hi);
	timex = xsize / wd;
	timey = ysize / hi;
	if(timex > timey){
		time = timey;
	}
	else{
		time = timex;
	}
	xsize = wd * time;
	ysize = hi * time;
	_tremove(m_szBmpFname);
	diblibBmp256PlanePictureCreate(srcfname, m_szBmpFname, xsize, ysize, m_clrPicup);
	m_pcDialogTraceImage->SetImage(m_szBmpFname);
}

void CCmdDibC::LocateTraceImgeDlg()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;
	int		ret;

	m_pcView->GetWindowRect(&prect);
	m_pcDialogTraceImage->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	ret = m_pcDialogTraceImage->SetWindowPos(theWnd, x, y, cx, cy, 0);
	m_pcDialogTraceImage->ShowWindow(SW_SHOW);
}

void CCmdDibC::TraceMain()
{
	int		nlimit;
	TCHAR	srcfname[FNAMEMAX];
	CFile	cpFile;
	ULONG	filesize;
	ULONG	size;
	LPBITMAPFILEHEADER lpbmf;
	LPBITMAPINFOHEADER lpbminfo;
	CTonDoc*	pcDoc;
	CDibCData*	pDibCData;
	DBL		dsx, dsy, dwd, dhi;
	DBL		dpix, dpiy, swd, shi;
	DBL		ssx, ssy;
	DBL		timex, timey;
	DBL		movex, movey;

	m_pcDialogTraceImage->UpdateData(TRUE);
	m_clrPicup = m_pcDialogTraceImage->m_clrPicup;
	nlimit = m_pcDialogTraceImage->m_nAreaLimit;
	m_dFureLimit = m_pcDialogTraceImage->m_nFureLimit;
	m_pcDibCData->GetFileName(srcfname);
	diblibBmpOnePlanePictureCreateES98(srcfname, m_szBmpFname, m_clrPicup, nlimit);

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	m_pcDataLogo = (CDataLogo*)pcDoc->NewData(DATALOGOID);

	pDibCData = (CDibCData*)(pcDoc->NewData(DIBCDATAID));
	pDibCData->SetFileName(m_szBmpFname);
	pDibCData->InitElement();
	m_pcDibCData->GetDstElement(&dsx, &dsy, &dwd, &dhi);

	m_pcDibCData->GetSrcElement(&dpix, &dpiy, &swd, &shi);
	ssx = 0; ssy = 0;
	pDibCData->SetDstElement(ssx, ssy, swd, shi);
	m_pcDataLogo->SetDibCData(pDibCData);

	if((cpFile.Open(m_szBmpFname, CFile::modeRead)) == FALSE){
		return;
	}

	filesize = (ULONG)cpFile.GetLength();
	m_hDib1 = GlobalAlloc(GHND, filesize);	
	m_hDib2 = GlobalAlloc(GHND, filesize);	
	m_lpDib1 = (LPBYTE)GlobalLock(m_hDib1);
	cpFile.SeekToBegin();
	cpFile.Read(m_lpDib1, filesize);
	GlobalUnlock(m_hDib1);
	cpFile.Close();

	size = POINTMAX;
	m_hCusTbl = GlobalAlloc(GHND, size);
	size = sizeof(XYType) * (POINTMAX);
	m_hXYTbl1 = GlobalAlloc(GHND, size);
	m_hXYTbl2 = GlobalAlloc(GHND, size);
	m_lpCusTbl = (LPBYTE)GlobalLock(m_hCusTbl);
	m_lpXYTbl1 = (XYType *)GlobalLock(m_hXYTbl1);
	m_lpXYTbl2 = (XYType *)GlobalLock(m_hXYTbl2);

	m_lpDib1 = (LPBYTE)GlobalLock(m_hDib1);
	m_lpDib2 = (LPBYTE)GlobalLock(m_hDib2);

	lpbmf = (LPBITMAPFILEHEADER)m_lpDib1;
	lpbminfo = (LPBITMAPINFOHEADER)(m_lpDib1 + sizeof(BITMAPFILEHEADER));
	m_lpBits1= (LPBYTE)(m_lpDib1 + lpbmf->bfOffBits);
	m_lpBits2= (LPBYTE)(m_lpDib2 + lpbmf->bfOffBits);
	m_nXSize = lpbminfo->biWidth;
	m_nXByte = (((DWORD)(m_nXSize * 1 + 7) / 8) + 3L) / 4L * 4L;
	m_nYSize = lpbminfo->biHeight;
	BitMapOutLineSet(m_lpBits1, m_lpBits2, m_nXByte, m_nXSize, m_nYSize);

	//BmpFileOut("c:\\test0001.bmp", m_lpBits1, m_nXByte, m_nXSize, m_nYSize);
	// トレースメイン処理
	BitMapTrace(m_lpBits1, m_lpBits2, m_nXByte, m_nXSize, m_nYSize);

	//BmpFileOut("c:\\test0001.bmp", m_lpBits1, m_nXByte, m_nXSize, m_nYSize);

	pcDoc->DataAddTail(m_pcDataLogo);

	m_pcDibCData->GetDstElement(&dsx, &dsy, &dwd, &dhi);
	timex = dwd / m_nXSize;
	timey = dhi / m_nYSize;
	ssx = 0.0;
	ssy = 0.0;
	m_pcDataLogo->SizeTrns(ssx, ssy, timex, timey);
	movex = dsx - ssx;
	movey = dsy - ssy;
	m_pcDataLogo->MoveTrns(movex, movey);

	pDibCData->GetDstElement(&dsx, &dsy, &dwd, &dhi);

	GlobalUnlock(m_hDib1);
	GlobalUnlock(m_hDib2);

	GlobalUnlock(m_hCusTbl);
	GlobalUnlock(m_hXYTbl1);
	GlobalUnlock(m_hXYTbl2);

	GlobalFree(m_hCusTbl);
	GlobalFree(m_hXYTbl1);
	GlobalFree(m_hXYTbl2);

	GlobalFree(m_hDib1);
	GlobalFree(m_hDib2);
}

void CCmdDibC::BmpFileOut(LPTSTR fname, BYTE far *m_lpBits, int xbyte, int xsize, int ysize)
{
	FILE				*fp;
	BITMAPFILEHEADER	bif;
	BITMAPINFOHEADER	bih;
	RGBQUAD				clr[2];
	int					headsize;
	BYTE				*sp;
	int					y;
	int					txbyte;

	memset((LPTSTR)(&bif), 0, sizeof(BITMAPFILEHEADER));
	memset((LPTSTR)(&bih), 0, sizeof(BITMAPINFOHEADER));
	headsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*2;
	bif.bfType = 0x4D42;
	bif.bfSize = xbyte * ysize + headsize;
	bif.bfOffBits = headsize;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = xsize;
	bih.biHeight = ysize;
	bih.biPlanes = 1;
	bih.biBitCount = 1;
	bih.biCompression = 0;

	clr[0].rgbBlue = 0;
	clr[0].rgbGreen = 0;
	clr[0].rgbRed = 0;
	clr[0].rgbReserved = 0;
	clr[1].rgbBlue = 255;
	clr[1].rgbGreen = 255;
	clr[1].rgbRed = 255;
	clr[1].rgbReserved = 0;

	_tfopen_s(&fp, fname, _T("wb"));
	fwrite(&bif, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(clr, sizeof(RGBQUAD), 2, fp);
	txbyte = (((long)xsize + 7L) / 8L + 1L) / 2L * 2L;
	for(y = 0; y < ysize; y++){
		sp = m_lpBits + (long)y * (long)txbyte;
		fwrite(sp, 1, xbyte, fp);
	}
	fclose(fp);
}

void CCmdDibC::BitMapOutLineSet(BYTE *lpBits, BYTE *lpTBits, int xbyte, int xsize, int ysize)
{
	int		x, y;

	// アウトラインドットを取り出す
	for(y = 0; y < ysize; y++){
		for(x = 0; x < xsize; x++){
			if(BitOnOffCheck(lpBits, xbyte, x, y) == 1){
				if(BitOnOffCheck(lpBits, xbyte, x-1, y) == 0){
					BitOnSet(lpTBits, xbyte, x, y);
				}
				else if(BitOnOffCheck(lpBits, xbyte, x+1, y) == 0){
					BitOnSet(lpTBits, xbyte, x, y);
				}
				else if(BitOnOffCheck(lpBits, xbyte, x, y-1) == 0){
					BitOnSet(lpTBits, xbyte, x, y);
				}
				else if(BitOnOffCheck(lpBits, xbyte, x, y+1) == 0){
					BitOnSet(lpTBits, xbyte, x, y);
				}
			}
		}
	}
}

void CCmdDibC::BitMapTrace(BYTE *lpBits, BYTE *lpTBits, int xbyte, int xsize, int ysize)
{
	int		x, y;

	// アウトライントレース処理
	for(y = 0; y < ysize; y++){
		for(x = 0; x < xsize; x++){
			if(BitOnOffCheck(lpTBits, xbyte, x, y) == 1){
				BitMapTarceSetPlgn(lpBits, lpTBits, xbyte, x, y);
			}
		}
	}
}

void CCmdDibC::BitMapTarceSetPlgn(BYTE *lpBits, BYTE *lpTBits, int xbyte, int x, int y)
{
	int		rsx, rsy;
	int		tx, ty;
	int		tsx, tsy;
	int		cus, bcus;
	long	idx;
	FILE	*fp;
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];

	SetPlgnInit();
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\pointdel.lst"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	rsx = x; rsy = y;
	tsx = x; tsy = y;
	tx = x; ty = y;
	cus = FirstCusGet(lpBits, 0, xbyte, x, y);
	_stprintf_s(buf, _T("%d %d\n"), x, y);
	baselibFputs(buf, fp);
	idx = 0;
	while(1){
		bcus = cus;
		NextCusGet(lpTBits, xbyte, &tx, &ty, &cus);
		_stprintf_s(buf, _T("%d %d\n"), tx, ty);
		baselibFputs(buf, fp);
		if(cus == -1){
			break;
		}
		m_lpCusTbl[idx] = cus;
		idx++;
		if(POINTMAX/4 <= idx){
			SetXYTblSetPlgn(lpTBits, xbyte, idx, tsx, tsy);
			idx = 0;
			tsx = tx;
			tsy = ty;
			//BmpFileOut("c:\\test0001.bmp", lpTBits, xbyte, m_nXSize, m_nYSize);
		}
		if(tx == rsx && ty == rsy){
			if(idx != 0){
				SetXYTblSetPlgn(lpTBits, xbyte, idx, tsx, tsy);
			}
			break;
		}
	}
	fclose(fp);

	//BmpFileOut("c:\\test0001.bmp", lpTBits, xbyte, m_nXSize, m_nYSize);

	_tfopen_s(&fp, fname, _T("r"));
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%d %d"), &tx, &ty);
		BitOffSet(lpTBits, xbyte, tx, ty);
	}
	fclose(fp);
	//BmpFileOut("c:\\test0001.bmp", lpTBits, xbyte, m_nXSize, m_nYSize);
	//BmpFileOut("c:\\test0002.bmp", lpBits, xbyte, m_nXSize, m_nYSize);

	SetPlgnEnd();
}

void CCmdDibC::SetXYTblSetPlgn(BYTE *lpTBits, int xbyte, int idx, int tsx, int tsy)
{
	int		tx, ty;
	int		bx, by;
	int		point;
	int		cus;
	int		addx, addy;
	int		i;

	tx = tsx;
	ty = tsy;
	bx = tsx;
	by = tsy;
	point = 0;
	PlgnPointSet(m_lpXYTbl1, point, tx, ty);
	point++;
	for(i = 0; i < idx; i++){
		cus = m_lpCusTbl[i];
		CusAddGet(cus, &addx, &addy);
		tx += addx;
		ty += addy;
		if(bx != tx && by != ty){
			PlgnPointSet(m_lpXYTbl1, point, tx, ty);
			point++;
			bx = tx;
			by = ty;
		}
	}	
	PlgnPointSet(m_lpXYTbl1, point, tx, ty);
	point++;
	SetDataLogoPolygon(m_lpXYTbl1, m_lpXYTbl2, point);
}

void CCmdDibC::NextCusGet(BYTE *lpTBits, int xbyte, int *retx, int *rety, int *retcus)
{
	int		cus;
	int		x, y;
	int		addx, addy;
	int		i;

	cus = CusLegal(*retcus -3);
	x = *retx;
	y = *rety;
	*retcus = -1;
	for(i = 0; i < 8; i++){
		CusAddGet(cus, &addx, &addy);
		if(BitOnOffCheck(lpTBits, xbyte, x+addx, y+addy) == 1){
			*retx = x+addx;
			*rety = y+addy;
			*retcus = cus;
			break;
		}
		cus = CusLegal(cus+1);
	}
}

int CCmdDibC::FirstCusGet(BYTE *lpBits, int scus, int xbyte, int x, int y)
{
	int	cus;
	int	addx, addy;

	for(cus = scus+1; cus != scus; cus++){
		if(cus == 8){
			cus = 0;
		}
		CusAddGet(cus, &addx, &addy);
		if(BitOnOffCheck(lpBits, xbyte, x+addx, y+addy) == 0){
			break;
		}
	}
	return(cus);
}

void CCmdDibC::CusAddGet(int cus, int *addx, int *addy)
{
	switch(cus){
	case 7:
	case 0:
	case 1:
		*addx = -1;
		break;
	case 2:
	case 6:
		*addx = 0;
		break;
	case 3:
	case 4:
	case 5:
		*addx = 1;
		break;
	}
	switch(cus){
	case 1:
	case 2:
	case 3:
		*addy = 1;
		break;
	case 0:
	case 4:
		*addy = 0;
		break;
	case 5:
	case 6:
	case 7:
		*addy = -1;
		break;
	}
}

int CCmdDibC::CusLegal(int cus)
{
	if(cus < 0){
		cus += 8;
	}
	if(7 < cus){
		cus -= 8;
	}
	return(cus);
}

int CCmdDibC::BitOnOffCheck(BYTE *lpBits, int xbyte, int x, int y)
{
	int		nbyte;
	int		mask;
	BYTE	*sp;

	if(x < 0){
		return(0);
	}
	if(m_nXSize <= x){
		return(0);
	}
	if(y < 0){
		return(0);
	}
	if(m_nYSize <= y){
		return(0);
	}
	nbyte = x / 8;
	mask = 0x0080 >> (x % 8);
	sp = lpBits + (long)y * (long)xbyte + (long)nbyte;
	if(*sp & mask){
		return(1);
	}
	else{
		return(0);
	}
}

void CCmdDibC::BitOnSet(BYTE *lpBits, int xbyte, int x, int y)
{
	int		nbyte;
	int		mask;
	BYTE	*sp;

	if(x < 0){
		return;
	}
	if(m_nXSize <= x){
		return;
	}
	if(y < 0){
		return;
	}
	if(m_nYSize <= y){
		return;
	}
	nbyte = x / 8;
	mask = 0x0080 >> (x % 8);
	sp = lpBits + (long)y * (long)xbyte + (long)nbyte;
	(*sp) |= mask;
}

void CCmdDibC::BitOffSet(BYTE *lpBits, int xbyte, int x, int y)
{
	int		nbyte;
	int		mask;
	BYTE	*sp;

	if(x < 0){
		return;
	}
	if(m_nXSize <= x){
		return;
	}
	if(y < 0){
		return;
	}
	if(m_nYSize <= y){
		return;
	}
	nbyte = x / 8;
	mask = 0x0080 >> (x % 8);
	sp = lpBits + (long)y * (long)xbyte + (long)nbyte;
	(*sp) &= ~mask;
}

void CCmdDibC::PlgnPointSet(XYType *lpXYTbl, int points, int tx, int ty)
{
	lpXYTbl[points].x = tx;
	lpXYTbl[points].y = ty;
}

void CCmdDibC::SetDataLogoPolygon(XYType *lpXYTbl1, XYType *lpXYTbl2, int points)
{
	int		cnt1, cnt2;
	int		tp, pp;
	DBL		dx, dy;

	cnt1 = points;
	mdfyinit(m_dFureLimit);
	cnt2 = mdfyfirst(lpXYTbl1, cnt1, lpXYTbl2);
	if(cnt2 == 0){
		return;
	}
	cnt1 = mdfysecond(lpXYTbl2, cnt2, lpXYTbl1);
	if(cnt1 == 0){
		return;
	}
	for(tp = 0, pp = m_nPcnt; pp < m_nPcnt + cnt1; tp++, pp++){
		dx = lpXYTbl1[tp].x;
		dy = lpXYTbl1[tp].y;
		m_pcVect->SetAtrDXY(pp, 0, dx, dy);
	}
	m_nPcnt = m_nPcnt + cnt1;
}

void CCmdDibC::SetPlgnInit()
{
	m_nPcnt = 0;
	m_pcVect = new CVect(BASEVECTID);
	//m_pcVect->OffFlag(REJIONFLAG);
}

void CCmdDibC::SetPlgnEnd()
{
	m_pcVect->SetPointCount(m_nPcnt);
	PlgnLineToArcRejionSet(m_pcVect);
	m_nPcnt = m_pcVect->GetPointCount();
	if(2 < m_nPcnt){
		m_pcVect->RenewMiniMax();
		m_pcVect->OnFlag(ATNTION1FLAG);
		m_pcDataLogo->SetData((CData*)m_pcVect);
		m_pcVect = NULL;
	}
	else{
		delete(m_pcVect);
		m_pcVect = NULL;
	}
}

void CCmdDibC::PlgnLineToArcRejionSet(CVect *pcVect)
{
	int		pcnt;
	int		pp1, pp3;
	int		atr;
	DBL		x1, y1, x2, y2;
	CVect	*pcTmpVect;
	int		dp;

	dp = 0;
	pcnt = pcVect->GetPointCount();
	if(pcnt < 5){
		return;
	}
	pcTmpVect = new CVect(BASEVECTID);
	for(pp1 = 0; pp1 < pcnt-1;){
		if(PlgnCurveChk(pcVect, pp1, &x2, &y2, &pp3) == ON){
			pcVect->GetDXY(pp1, &x1, &y1);
			pcTmpVect->SetAtrDXY(dp, CURVATR, x1, y1);
			dp++;
			pcTmpVect->SetAtrDXY(dp, CONTATR|CURVATR, x2, y2);
			dp++;
			pp1 = pp3;
		}
		else{
			pcVect->GetDXY(pp1, &x1, &y1);
			pcTmpVect->SetAtrDXY(dp, 0, x1, y1);
			dp++;
			pp1++;
		}
	}
	pcnt = dp;
	for(pp1 = 0; pp1 < pcnt; pp1++){
		pcTmpVect->GetAtrDXY(pp1, &atr, &x1, &y1);
		pcVect->SetAtrDXY(pp1, atr, x1, y1);
	}
	pcVect->SetPointCount(pcnt);
	delete(pcTmpVect);
}

int CCmdDibC::PlgnCurveChk(CVect *pcVect, int pp1, DBL *retx, DBL *rety, int *retpp)
{
	int		pcnt;
	int		pp2, pp3, tpp;
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		x0, y0, r;
	DBL		rag01, rag03;
	DBL		subrag;
	DBL		tx, ty, thx, thy;
	DBL		limit;
	DBL		len1, len2;
	int		ret;

	limit = m_dFureLimit*2.0;
	pcnt = pcVect->GetPointCount();
	if(pp1 == pcnt - 1
	|| pp1 == pcnt - 2){
		return(OFF);
	}

	for(pp3 = pp1 + 3; pp3 < pcnt; pp3++){
		pp2 = (pp1 + pp3) / 2;
		pcVect->GetDXY(pp1, &x1, &y1);
		pcVect->GetDXY(pp2, &x2, &y2);
		pcVect->GetDXY(pp3, &x3, &y3);
		if(curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0) == FAIL){
			return(OFF);
		}
		if(curve3pragrget(x1,y1,x2,y2,x3,y3,x0,y0,&rag01,&rag03,&r) == FAIL){
			return(OFF);
		}
		subrag = rag03 - rag01;
		if(M_PI_4 < fabs(subrag)){
			break;
		}

		ret = 0;
		pcVect->GetDXY(pp1, &x1, &y1);
		for(tpp = pp1+1; tpp < pp3; tpp++){
			pcVect->GetDXY(tpp, &tx, &ty);
			if(PlgnCrclDastance(tx, ty, x0, y0, r, limit) == FAIL){
				ret = FAIL;
				break;
			}
			thx = (x1 + tx) / 2;
			thy = (y1 + ty) / 2;
			if(PlgnCrclDastance(thx, thy, x0, y0, r, limit*2) == FAIL){
				ret = FAIL;
				break;
			}
			x1 = tx;
			y1 = ty;
		}
		thx = (x1 + x3) / 2;
		thy = (y1 + y3) / 2;
		if(PlgnCrclDastance(thx, thy, x0, y0, r, limit*2) == FAIL){
			ret = FAIL;
		}
		pcVect->GetDXY(pp3, &x3, &y3);

		if(ret == FAIL){
			break;
		}
	}
	if(pp3 >= pcnt){
		return(OFF);
	}
	pp3 = pp3-1;
	if(pp3 < pp1+3){
		return(OFF);
	}
	pp2 = (pp1+pp3)/2;
	pcVect->GetDXY(pp1, &x1, &y1);
	pcVect->GetDXY(pp2, &x2, &y2);
	pcVect->GetDXY(pp3, &x3, &y3);
	len1 = hypot(x1-x2, y1-y2);
	len2 = hypot(x2-x3, y2-y3);
	if(len1 < limit*2 && len2 < limit*2){
		return(OFF);
	}
	*retx = x2;
	*rety = y2;
	*retpp = pp3;
	return(ON);
	/*
	if(curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0) == FAIL){
		return(OFF);
	}
	if(curve3pragrget(x1,y1,x2,y2,x3,y3,x0,y0,&rag01,&rag03,&r) == FAIL){
		return(OFF);
	}
	subrag = rag03-rag01;

	rag02 = subrag/2.0 + rag01;
	*retx = cos(rag02)*r+x0;
	*rety = sin(rag02)*r+y0;
	*retpp = pp3;
	return(ON);
	*/
}

int CCmdDibC::PlgnCrclDastance(DBL tx, DBL ty, DBL x0, DBL y0, DBL r, DBL flimit)
{
	DBL		frx, fry, r2, deltr;

	frx = tx - x0;
	fry = ty - y0;
	r2 = sqrt(frx*frx + fry*fry);
	deltr = fabs(r2 - r);
	if(deltr > flimit){
		return(FAIL);
	}
	return(0);
}


BOOL CCmdDibC::SaveImageFile()
{
	CBmpFile	*pcBmpFile;
	TCHAR	jpgfname[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	int		mode;
	int		len;
	DBL		xsize, ysize;
	int		movex, movey;
	COLORREF	clr;

	scrngetmmsize(&xsize, &ysize);
	if(m_szJpgFname[0] == _T('\0')){
		_stprintf_s(fname, _T("%s\\%s"), m_pcApp->m_szSavePath,  m_pcApp->m_szSaveName);
	}else{
		_tcscpy_s(fname, m_szJpgFname);
	}

	len = _tcslen(fname);
	if((fname[len-3] == _T('j') || fname[len-3] == _T('J'))
	&& (fname[len-2] == _T('p') || fname[len-2] == _T('P'))
	&& (fname[len-1] == _T('g') || fname[len-1] == _T('G'))){
		_tcscpy_s(jpgfname, fname);
		diblibcreateimgfilename(fname);
	}
	else{
		jpgfname[0] = _T('\0');
	}

	clr = m_pcApp->m_clrBack;
	basechangesafix(fname, _T("bmp"));
	pcBmpFile = new CBmpFile(m_pcView);

	if(m_nRotate == 0){
		mode = -1;
		pcBmpFile->CreateBmpInit(m_nImageXSize*3, m_nImageYSize*3);
	}else if(m_nRotate == 1){
		mode = 1;
		pcBmpFile->CreateBmpInit(m_nImageYSize*3, m_nImageXSize*3);
	}else if(m_nRotate == 2){
		mode = 0;
		pcBmpFile->CreateBmpInit(m_nImageXSize*3, m_nImageYSize*3);
	}else if(m_nRotate == 3){
		mode = 2;
		pcBmpFile->CreateBmpInit(m_nImageYSize*3, m_nImageXSize*3);
	}
	pcBmpFile->GetMoveXY(&movex, &movey);
	pcBmpFile->CreateBmpImageHalf(movex, movey, clr, fname);
	pcBmpFile->CreateBmpEnd();
	delete(pcBmpFile);

	/*
	if(mode != -1){
		HANDLE	hndle;
		HANDLE	nhndle;
		int		ret;
		hndle = ImgKitFileLoad(fname, _T(""), _T(""));
		nhndle = ImgKitRotation(hndle, _T(""), _T(""), mode);
		ret = ImgKitBmpFileSave(fname, nhndle, _T(""), _T(""), 0);
		ret = ImgKitMemFree(hndle);
		ret = ImgKitMemFree(nhndle);
	}

	if(jpgfname[0] != _T('\0')){
		hndle = ImgKitFileLoad(fname, _T(""), _T(""));
		ret = ImgKitJpegFileSave(jpgfname, hndle, _T(""), _T(""), m_nQuality, 0);
	}
	*/
	return(TRUE);
}

void CCmdDibC::ChangeCopyItiFile()
{
	TCHAR	fname[FNAMEMAX];
	TCHAR	cpyfname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX8+16];
	LPTSTR	csvsp[RECMAX];

	if(m_pcApp->m_szArgv1[0] == _T('\0')){
		return;
	}
	_tcscpy_s(fname, m_pcApp->m_szArgv1);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	_fgetts(buf, RECMAX8, fp);
	int len = _tcslen(buf);
	buf[len-1] = _T('\0');
	fclose(fp);
	basedivcsvdata(csvsp, buf);

	_tfopen_s(&fp, fname, _T("w"));
	if(fp == NULL){
		return;
	}
	_tcscpy_s(csvsp[17], RECMAX, _T("済"));
	_stprintf_s(buf, _T("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s"),
		csvsp[0], csvsp[1], csvsp[2], csvsp[3], csvsp[4],
		csvsp[5], csvsp[6], csvsp[7], csvsp[8], csvsp[9],
		csvsp[10], csvsp[11], csvsp[12], csvsp[13], csvsp[14],
		csvsp[15], csvsp[16], csvsp[17], csvsp[18], csvsp[19]);	
	baselibFputs(buf, fp);
	fclose(fp);

	_stprintf_s(cpyfname, _T("%s\\%s"), m_pcApp->m_szSavePath,  m_pcApp->m_szSaveName);
	basechangesafix(cpyfname, _T("txt"));
	CopyFile(fname, cpyfname, FALSE);
}

void CCmdDibC::SaveDocument()
{
	TCHAR	datpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	CFile	file;

	senvgetdatpath(datpath);
	_stprintf_s(fname, _T("%s\\%s"), datpath,  m_pcApp->m_szSaveName);
	basechangesafix(fname, _T("iti"));

	file.Open(fname,  CFile::modeCreate|CFile::modeWrite, NULL);
	CArchive ar(&file, CArchive::store);
	m_pcDoc->Serialize(ar);
	ar.Close();
	file.Close();
	basechangesafix(fname, _T("env"));
	SaveCrtEnv(fname);

	basechangesafix(fname, _T("bin"));
	m_pcView->PrintEnvSave(fname);
}

void CCmdDibC::SaveCrtEnv(LPTSTR fname)
{
	FILE	*fp;
	DBL		xmmsize, ymmsize;
	TCHAR	buf[RECMAX];

	scrngetmmsize(&xmmsize, &ymmsize);
	_tfopen_s(&fp, fname, _T("w"));
	if(fp == NULL){
		return;
	}
	_stprintf_s(buf, _T("%1.1lf %1.1lf\n"), xmmsize, ymmsize);
	baselibFputs(buf, fp);
	fclose(fp);
}

void CCmdDibC::DrawAreaBoxs(CDC *pcDC)
{
	DBL		sx, sy;
	DBL		ex, ey;

	sx = m_dSX; sy = m_dSY;
	ex = m_dEX; ey = m_dEY;

	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);

	CPen *pcPen = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen *pOldPen = pcDC->SelectObject(pcPen);

	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);
	pcDC->LineTo((int)sx, (int)ey);
	pcDC->LineTo((int)sx, (int)sy);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);
	pcDC->MoveTo((int)ex, (int)sy);
	pcDC->LineTo((int)sx, (int)ey);

	pcDC->SelectObject(pOldPen);
	delete(pcPen);
}
