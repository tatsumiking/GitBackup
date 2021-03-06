
/////////////////////////////////////////////////////////////////////////////
// CCmdFile

#include "stdafx.h"
#include "ctype.h"
#include "string.h"
#include "direct.h"

#include "CmdBase.h"
#include "CmdFile.h"

#include "BmpFile.h"
#include "FileEps.h"
#include "FileVect.h"
#include "FileDxf.h"
#include "FileArc.h"
#include "ViewFileDialog.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Diblib.h"
//}


//static TCHAR BASED_CODE szFilter[] = _T("TCHARt Files (*.xlc)|*.xlc|Worksheet Files (*.xls)|*.xls|Data Files (*.xlc;*.xls)|*.xlc; *.xls|All Files (*.*)|*.*||");
//static TCHAR BASED_CODE szFilter[] = _T("Vect Files (*.vct;*.log;*.wak)|*.vct;*.log;*.wak|Bitmap File (*.tiff;*.jpg)|*.tiff;*.jpg||");
static TCHAR BASED_CODE szExportFilter[] = _T("Eps Files (*.eps)|*.eps|Dxf Files (*.dxf)|*.dxf||");
static TCHAR BASED_CODE szFilter[RECMAX*2];
static TCHAR BASED_CODE szSafixs[8];

CCmdFile::CCmdFile(CScrollView* pcview)
:CCmdBase(pcview)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	FILE	*fp;

	_tcscpy_s(m_szInpSafixs, _T("vct"));
	_tcscpy_s(m_szExpSafixs, _T("eps"));
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_file.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), m_szInpSafixs, RECMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), m_szExpSafixs, RECMAX);
		fclose(fp);
	}
	senvgetdatpath(datpath);
	_tcscpy_s(n_szInpPath, datpath);
	_tcscpy_s(n_szExpPath, datpath);
	_tcscpy_s(n_szDatPath, datpath);
}

CCmdFile::~CCmdFile()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_file.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if(fp != NULL){
		_stprintf_s(buf, _T("%s\n"), m_szInpSafixs);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), m_szExpSafixs);
		baselibFputs(buf, fp);
		fclose(fp);
	}
}

void CCmdFile::SetFilter(LPTSTR filter, LPTSTR safixs)
{
	_tcscpy_s(szFilter, filter);
	_tcscpy_s(szSafixs, safixs);
}

UINT CCmdFile::ExecMyFileLoad(UINT event, DBL dx, DBL dy)
{
	CViewFileDialog	*pcViewFileDialog;
	TCHAR	fname[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	safixs[QRECMAX];
	TCHAR	szSrch[RECMAX];
	CTonDoc*	pcDoc;
	int		nVerNo;
	CString		csMsg;
	int		nResave;
	TCHAR	szResaveBuf[RECMAX];
	int		i;

	if(event != INITEVENT){
		return(0);
	}
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	csMsg = pcDoc->GetTitle();
	_tcscpy_s(safixs, _T("ess"));
	_stprintf_s(szSrch, _T("*.%s"), safixs);
	_stprintf_s(szFilter, _T("%s Files (*.%s)|*.%s||"), pcDoc->m_szSystemName, safixs, safixs);

	_tcscpy_s(datpath, n_szDatPath);
	SetCurrentDirectory(datpath);
	pcViewFileDialog = new CViewFileDialog(TRUE, NULL, szSrch, NULL, szFilter, m_pcView);
	pcViewFileDialog->SetTitle(_T("ファイル読込"));
	if(pcViewFileDialog->DoModal( ) == IDOK){
		CString cpStr = pcViewFileDialog->GetPathName();
		_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
		baseaddsafix(fname, safixs);
		GetDirName(fname, n_szDatPath);
		pcDoc->SetTitle(fname);	
		CFile file(fname, CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		ar >> nVerNo;
		BitMapImageLoad(ar);
		ar >> csMsg;
		//_tcscpy_s(pcFileDialog->m_szGyousya, csMsg.GetBuffer(QRECMAX));
		ar >> csMsg;
		//_tcscpy_s(pcFileDialog->m_szKouji, csMsg.GetBuffer(QRECMAX));
		ar >> csMsg;
		//_tcscpy_s(pcFileDialog->m_szComent1, csMsg.GetBuffer(QRECMAX));
		ar >> csMsg;
		//_tcscpy_s(pcFileDialog->m_szComent2, csMsg.GetBuffer(QRECMAX));
		ar >> csMsg;
		//_tcscpy_s(pcFileDialog->m_szComent3, csMsg.GetBuffer(QRECMAX));
		ar >> nResave;
		for(i = 0; i < nResave; i++){
			ar >> szResaveBuf[i];
		}
		pcDoc->Serialize(ar);
		ar.Close();
		file.Close();

		ScrnSizeReset();

	}
	delete(pcViewFileDialog);
	senvgetexepath(exepath);
	SetCurrentDirectory(exepath);

	return(ENDEVENT);
}

UINT CCmdFile::ExecMyFileSave(UINT event, DBL dx, DBL dy)
{
	CViewFileDialog	*pcViewFileDialog;
	TCHAR	fname[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	safixs[QRECMAX];
	TCHAR	szSrch[RECMAX];
	CTonDoc*	pcDoc;
	int		nVerNo;
	TCHAR	msg[RECMAX+QRECMAX];
	CString		csMsg;
	CString		cpStr;
	int		nResave;
	TCHAR	szResaveBuf[RECMAX];
	CFile	file;
	CFileException e;
	int		i, ret;

	nVerNo = 105;
	nResave = 32;
	memset(szResaveBuf, 0, RECMAX);
	if(event != INITEVENT){
		return(0);
	}
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->m_nError = FALSE;
	csMsg = pcDoc->GetTitle();
	_tcscpy_s(safixs, _T("ess"));
	//_stprintf_s(szSrch, _T("*.%s"), safixs);
	_stprintf_s(szSrch, _T("%s"), csMsg.GetBuffer(FNAMEMAX));
	_stprintf_s(szFilter, _T("%s Files (*.%s)|*.%s||"), pcDoc->m_szSystemName, safixs, safixs);

	_tcscpy_s(datpath, n_szDatPath);
	SetCurrentDirectory(datpath);
	pcViewFileDialog = new CViewFileDialog(FALSE, NULL, szSrch, NULL, szFilter, m_pcView);
	pcViewFileDialog->SetTitle(_T("ファイル保存"));
	ret = pcViewFileDialog->DoModal( );
	if(ret != IDOK){
		return(ENDEVENT);
		//goto ENDLABEL;
	}

	cpStr = pcViewFileDialog->GetPathName();
	_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
	baseaddsafix(fname, safixs);
	GetDirName(fname, n_szDatPath);
	if(file.Open(fname, CFile::modeRead, NULL)){
		file.Close();
		_stprintf_s(msg, _T("%sに上書きしてもよろしいですか"), fname);
		if(AfxMessageBox(msg, MB_OKCANCEL) == IDCANCEL){
			return(ENDEVENT);
		}
	}
	SaveGyousya(pcViewFileDialog->m_csGyousya.GetBuffer(RECMAX));
	SaveKouji(pcViewFileDialog->m_csKouji.GetBuffer(RECMAX));
	//try{
		if(file.Open(fname,  CFile::modeCreate|CFile::modeWrite, &e) == FALSE){
			/*
			ErrorMessageOut(&e);
			goto ENDLABEL;
			*/
			return(ENDEVENT);
		}
		CArchive ar(&file, CArchive::store);
		scrnonnoactflag();
		ar << nVerNo;
		ret = BitMapImageSave(ar);
		/*
		if(ret == FALSE){
			pcDoc->m_nError = TRUE;
			goto ENDLABEL;
		}
		*/
		ar << pcViewFileDialog->m_csGyousya;
		ar << pcViewFileDialog->m_csKouji;
		ar << pcViewFileDialog->m_csComent1;
		ar << pcViewFileDialog->m_csComent2;
		ar << pcViewFileDialog->m_csComent3;
		// 環境などの保存用処理
		ar << nResave;
		for(i = 0; i < nResave; i++){
			ar << szResaveBuf[i];
		}
		pcDoc->Serialize(ar);
		/*
		if(pcDoc->m_nError == TRUE){
			goto ENDLABEL;
		}
		*/
		scrnoffnoactflag();
		ar.Close();
		file.Close();
		pcDoc->SetTitle(fname);
	/*
	}
	catch(CFileException *pcFileException)
	{
		ErrorMessageOut(pcFileException);
		pcFileException->Delete();
	}
	catch(CException *pcException)
	{
		::AfxMessageBox(_T("システムエラーが発生しました"), MB_OK);
		pcException->Delete();
	}
	*/
//ENDLABEL:
	delete(pcViewFileDialog);
	senvgetexepath(exepath);
	SetCurrentDirectory(exepath);
	if(pcDoc->m_nError == TRUE){
		_tremove(fname);
	}
	return(ENDEVENT);
}

UINT CCmdFile::ExecAddLoad(UINT event, DBL dx, DBL dy)
{
	TCHAR	fname[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	srchptn[QRECMAX];
	TCHAR	safixs[QRECMAX];

	if(event == INITEVENT){
		InitClass();
		_stprintf_s(srchptn, _T("*.%s"), szSafixs);
		_tcscpy_s(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle(_T("追加用ファイル読込"));
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			_tcscpy_s(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);

			m_pcDoc->LoadFile(fname);

			ScrnSizeReset();
		}
		senvgetexepath(exepath);
		SetCurrentDirectory(exepath);

		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdFile::ExecAddEnvLoad(UINT event, DBL dx, DBL dy)
{
	TCHAR	fname[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	srchptn[QRECMAX];
	TCHAR	safixs[QRECMAX];
	CTonDoc*	pcDoc;
	BYTE	szBuf[16];
	short		nBmpMark;
	long		lImageSize;

	if(event == INITEVENT){
		_stprintf_s(srchptn, _T("*.%s"), szSafixs);
		_tcscpy_s(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle(_T("追加用ファイル読込"));
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			_tcscpy_s(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());

			CFile file(fname, CFile::modeRead);
			file.Read(szBuf, 2);
			file.Seek(0L, CFile::begin);
			CArchive ar(&file, CArchive::load);
			if(szBuf[0] == 0x42 && szBuf[1] == 0x4d){
				ar >> nBmpMark;
				ar >> lImageSize;
				lImageSize -= 6;
				while(lImageSize--){
					ar >> szBuf[0];
				}
			}
			pcDoc->Serialize(ar);
			ar.Close();
			file.Close();

			basechangesafix(fname, _T("env"));
			LoadCrtEnv(fname);

			ScrnSizeReset();

		}
		senvgetexepath(exepath);
		SetCurrentDirectory(exepath);

		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdFile::ExecAddSave(UINT event, DBL dx, DBL dy)
{
	TCHAR	fname[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	srchptn[QRECMAX];
	TCHAR	safixs[QRECMAX];
	CTonDoc*	pcDoc;

	if(event == INITEVENT){
		_stprintf_s(srchptn, _T("*.%s"), szSafixs);
		_tcscpy_s(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(FALSE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle(_T("追加用ファイル保存"));
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			_tcscpy_s(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			baseaddsafix(fname, szSafixs);

			pcDoc->SaveFile(fname);

			basechangesafix(fname, _T("env"));
			SaveCrtEnv(fname);

			basechangesafix(fname, szSafixs);

		}
		senvgetexepath(exepath);
		SetCurrentDirectory(exepath);


		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdFile::ExecAddDel(UINT event, DBL dx, DBL dy)
{
	TCHAR	fname[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	srchptn[QRECMAX];
	TCHAR	safixs[QRECMAX];
	TCHAR	msgbuf[RECMAX];
	CTonDoc*	pcDoc;
	int		ret;

	if(event == INITEVENT){
		_stprintf_s(srchptn, _T("*.%s"), szSafixs);
		_tcscpy_s(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle(_T("追加用ファイル削除"));
		pcFileDialog->SetBtnText(_T("削除"));
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			_tcscpy_s(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			_stprintf_s(msgbuf, _T("<<%s>>ファイルを削除してよろしいですか"), fname);
			ret = AfxMessageBox(msgbuf, MB_OKCANCEL);
			if(ret == IDOK){
				_tremove(fname);
				basechangesafix(fname, _T("env"));
				_tremove(fname);
			}
		}
		senvgetexepath(exepath);
		SetCurrentDirectory(exepath);
		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdFile::ExecDelete(UINT event, DBL dx, DBL dy)
{
	TCHAR	fname[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	srchptn[QRECMAX];
	TCHAR	msgbuf[RECMAX];
	CTonDoc*	pcDoc;
	int		ret;

	if(event == INITEVENT){
		_stprintf_s(srchptn, _T("*.*"));
		_tcscpy_s(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle(_T("ファイル削除"));
		pcFileDialog->SetBtnText(_T("削除"));
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpStr = pcFileDialog->GetPathName();
			_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			_stprintf_s(msgbuf, _T("<<%s>>ファイルを削除してよろしいですか"), fname);
			ret = AfxMessageBox(msgbuf, MB_OKCANCEL);
			if(ret == IDOK){
				_tremove(fname);
			}
		}
		senvgetexepath(exepath);
		SetCurrentDirectory(exepath);

		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdFile::ExecCrctLoad(UINT event, DBL dx, DBL dy)
{
	TCHAR	fname[FNAMEMAX];
	TCHAR	crctfname[FNAMEMAX];
	TCHAR	backfname[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	srchptn[QRECMAX];
	TCHAR	safixs[QRECMAX];
	CTonDoc*	pcDoc;

	if(event == INITEVENT){
		_stprintf_s(srchptn, _T("*.%s"), szSafixs);
		_tcscpy_s(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle(_T("貯め込みファイル読込"));
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			_tcscpy_s(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			cpStr = pcFileDialog->GetFileName();
			_tcscpy_s(pcDoc->m_szCrctFileName, cpStr.GetBuffer(FNAMEMAX));

			senvgetexepath(exepath);
			_stprintf_s(crctfname, _T("%s\\outcrect.dat"), exepath);

			senvgetexepath(exepath);
			_stprintf_s(backfname, _T("%s\\outcrect.bak"), exepath);

			CopyFile(crctfname, backfname, FALSE);
			CopyCrctFile(fname, crctfname);
		}
		senvgetexepath(exepath);
		SetCurrentDirectory(exepath);

		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdFile::ExecCrctSave(UINT event, DBL dx, DBL dy)
{
	TCHAR	msg[HRECMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	crctfname[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	srchptn[QRECMAX];
	TCHAR	safixs[QRECMAX];
	CTonDoc*	pcDoc;

	if(event == INITEVENT){
		_stprintf_s(srchptn, _T("*.%s"), szSafixs);
		_tcscpy_s(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(FALSE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle(_T("貯め込みファイル保存"));
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			_tcscpy_s(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			baseaddsafix(fname, szSafixs);
			CFile file;
			if(file.Open(fname,  CFile::modeRead, NULL)){
				file.Close();
				_stprintf_s(msg, _T("%sに上書きしてもよろしいですか"), fname);
				if(AfxMessageBox(msg, MB_OKCANCEL) == IDCANCEL){
					return(ENDEVENT);
				}
			}

			senvgetexepath(exepath);
			_stprintf_s(crctfname, _T("%s\\outcrect.dat"), exepath);
			CopyCrctFile(crctfname, fname);
		}
		senvgetexepath(exepath);
		SetCurrentDirectory(exepath);

		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdFile::ExecCrctDel(UINT event, DBL dx, DBL dy)
{
	TCHAR	fname[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	srchptn[QRECMAX];
	TCHAR	safixs[QRECMAX];
	TCHAR	msgbuf[RECMAX];
	CTonDoc*	pcDoc;
	int		ret;

	if(event == INITEVENT){
		_stprintf_s(srchptn, _T("*.%s"), szSafixs);
		_tcscpy_s(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle(_T("貯め込みファイル削除"));
		pcFileDialog->SetBtnText(_T("削除"));
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			_tcscpy_s(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());

			_stprintf_s(msgbuf, _T("<<%s>>ファイルを削除してよろしいですか"), fname);
			ret = AfxMessageBox(msgbuf, MB_OKCANCEL);
			if(ret == IDOK){
				DeleteCrctFile(fname);
			}
		}
		senvgetexepath(exepath);
		SetCurrentDirectory(exepath);

		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdFile::ExecInport(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		SaveUndo();
		if(LoadInportFile() == FALSE){
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
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			MoveTrnsMain();
			m_nMouseSetCount = 0;
			ScrnRedraw();
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
			ScrnRedraw();
		}
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdFile::ExecExport(UINT event, DBL dx, DBL dy)
{
	TCHAR	msg[RECMAX2];
	TCHAR	fname[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	safix[QRECMAX];
	TCHAR	epsfilter[QRECMAX];
	TCHAR	dxffilter[QRECMAX];
	int		filteridx;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();

	_tcscpy_s(epsfilter, _T("Eps Files (*.eps)|*.eps|"));
	_stprintf_s(safix, _T("*.%s"), m_szExpSafixs);
	_tcscpy_s(dxffilter, _T("DXF Files (*.dxf)|*.dxf|"));

	//_stprintf_s(safix, _T("*.%s"), m_szExpSafixs);
	//_tcscpy_s(safix, _T("*.eps"));
	//_stprintf_s(szFilter, _T("%s|"), epsfilter);

	if(_tcscmp(m_szExpSafixs, _T("DXF")) == 0){
		_tcscpy_s(safix, _T("*.dxf"));
		_stprintf_s(szFilter, _T("%s%s|"), dxffilter, epsfilter);
	}
	else{
		_tcscpy_s(safix, _T("*.eps"));
		_stprintf_s(szFilter, _T("%s%s|"), epsfilter, dxffilter);
	}

	_tcscpy_s(datpath, n_szExpPath);
	SetCurrentDirectory(datpath);
	CMyFileDialog *pcFileDialog = new CMyFileDialog(FALSE, NULL, safix, NULL, szFilter, m_pcView);
	pcFileDialog->SetTitle(_T("外部ファイル保存"));
	if(pcFileDialog->DoModal( ) == IDOK){
		CString cpExt = pcFileDialog->GetFileExt();
		filteridx = (pcFileDialog->m_ofn).nFilterIndex;
		CString cpStr = pcFileDialog->GetPathName();
		_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
		GetDirName(fname, n_szExpPath);
		if(filteridx == 1){
			baseaddsafix(fname, _T("eps"));	
		}
		else if(filteridx == 2){
			baseaddsafix(fname, _T("dxf"));
		}
		CFile file;
		if(file.Open(fname,  CFile::modeRead, NULL)){
			file.Close();
			_stprintf_s(msg, _T("%sに上書きしてもよろしいですか"), fname);
			if(AfxMessageBox(msg, MB_OKCANCEL) == IDCANCEL){
				return(ENDEVENT);
			}
		}
		if(filteridx == 1){
			SaveEpsFile(fname);
		}
		else if(filteridx == 2){
			SaveDxfFile(fname);
		}
	}
	senvgetexepath(exepath);
	SetCurrentDirectory(exepath);

	return(ENDEVENT);
}

void CCmdFile::SaveCrtEnv(LPTSTR fname)
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

void CCmdFile::LoadCrtEnv(LPTSTR fname)
{
	FILE	*fp;
	TCHAR	buf[RECMAX];
	DBL		xmmsize, ymmsize;

	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%lf %lf\n"), &xmmsize, &ymmsize);
	if(baselibCrDelFgets(buf, RECMAX, fp) != NULL){
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			fclose(fp);
			return;
		}
		_stscanf_s(buf, _T("%lf %lf\n"), &xmmsize, &ymmsize);
	}
	scrnsetmmsize(xmmsize, ymmsize);
	fclose(fp);
	((CTonView*)m_pcView)->WorkSizeReset();
	((CTonView*)m_pcView)->WorkAreaReset();
}

BOOL CCmdFile::LoadInportFile()
{
	TCHAR	fname[FNAMEMAX];
	TCHAR	datpath[FNAMEMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	safix[RECMAX];
	TCHAR	vctfilter[QRECMAX];
	TCHAR	epsfilter[QRECMAX];
	TCHAR	sksifilter[QRECMAX];
	TCHAR	arcfilter[QRECMAX];
	TCHAR	rasterfilter[QRECMAX];
	TCHAR	metafilter[QRECMAX];

	_tcscpy_s(vctfilter, _T("Vect Files (*.vct;*.log;*.wak)|*.vct; *.log; *.wak|"));
	_tcscpy_s(epsfilter, _T("Eps&DXF Files (*.eps;*.ai;*.dxf)|*.eps;*.ai;*.dxf|"));
	_tcscpy_s(rasterfilter, _T("Raster Files (*.bmp;*.jpg;*.tif)|*.bmp;*.jpg;*.tif|"));
#ifdef	SOA
	_tcscpy_s(metafilter, _T("WinMeta Files (*.wmf;*.emf)|*.wmf;*.emf|"));
#else
	_tcscpy_s(metafilter, _T(""));
#endif
	_tcscpy_s(arcfilter, _T("ARCPART Files (*.;*.ed)|*.; *.ed|"));
	_tcscpy_s(sksifilter, _T("SKSI Files (*.;*.wrk)|*.; *.wrk|"));

	_tcscpy_s(datpath, n_szInpPath);
	SetCurrentDirectory(datpath);
	if(_tcscmp(m_szInpSafixs, _T("VCT")) == 0
	|| _tcscmp(m_szInpSafixs, _T("LOG")) == 0
	|| _tcscmp(m_szInpSafixs, _T("WAK")) == 0){
		_tcscpy_s(safix, _T("*.vct;*.log;*.wak"));
		_stprintf_s(szFilter, _T("%s%s%s%s%s%s|")
			, vctfilter, epsfilter, metafilter
			, rasterfilter
			, sksifilter, arcfilter);
	}
	else if(_tcscmp(m_szInpSafixs, _T("EPS")) == 0
		 || _tcscmp(m_szInpSafixs, _T("AI")) == 0
		 || _tcscmp(m_szInpSafixs, _T("DXF")) == 0){
		_tcscpy_s(safix, _T("*.eps;*.ai;*.dxf"));
		_stprintf_s(szFilter, _T("%s%s%s%s%s%s|")
			, epsfilter, metafilter, vctfilter
			, rasterfilter
			, sksifilter, arcfilter);
	}
	else if(_tcscmp(m_szInpSafixs, _T("WMF")) == 0
		 || _tcscmp(m_szInpSafixs, _T("EMF")) == 0){
		_tcscpy_s(safix, _T("*.wmf;*.emf"));
		_stprintf_s(szFilter, _T("%s%s%s%s%s%s|")
			, metafilter, epsfilter, vctfilter
			, rasterfilter
			, sksifilter, arcfilter);
	}
	else if(_tcscmp(m_szInpSafixs, _T("BMP")) == 0
		 || _tcscmp(m_szInpSafixs, _T("JPG")) == 0
		 || _tcscmp(m_szInpSafixs, _T("TIF")) == 0){
		_tcscpy_s(safix, _T("*.bmp;*.jpg;*.tif"));
		_stprintf_s(szFilter, _T("%s%s%s%s%s%s|")
			, rasterfilter
			, metafilter, epsfilter, vctfilter
			, sksifilter, arcfilter);
	}
	else if(_tcscmp(m_szInpSafixs, _T("ED")) == 0){
		_tcscpy_s(safix, _T("*.ed"));
		_stprintf_s(szFilter, _T("%s%s%s%s%s%s|")
			, arcfilter, sksifilter
			, vctfilter, epsfilter, metafilter
			, rasterfilter);
	}
	else if(_tcscmp(m_szInpSafixs, _T("WRK")) == 0){
		_tcscpy_s(safix, _T("*.wrk"));
		_stprintf_s(szFilter, _T("%s%s%s%s%s%s|")
			, sksifilter, arcfilter
			, vctfilter, epsfilter, metafilter
			, rasterfilter);
	}
	else{
		_tcscpy_s(safix, _T("*."));
		_stprintf_s(szFilter, _T("%s%s%s%s%s%s|")
			, sksifilter, arcfilter
			, vctfilter, epsfilter, metafilter
			, rasterfilter);
	}

	CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, safix, NULL, szFilter, m_pcView);
	pcFileDialog->SetTitle(_T("外部ファイル読込"));
	if(pcFileDialog->DoModal( ) == IDOK){
		CString cpExt = pcFileDialog->GetFileExt();
		_tcscpy_s(m_szInpSafixs, cpExt.GetBuffer(QRECMAX));
		CString cpStr = pcFileDialog->GetPathName();
		_tcscpy_s(fname, cpStr.GetBuffer(FNAMEMAX));
		GetDirName(fname, n_szInpPath);
		_tcsupr_s(m_szInpSafixs, NRECMAX);

		SaveUndo();
		if(_tcscmp(m_szInpSafixs, _T("VCT")) == 0){
			LoadVectFile(fname);			
		}
		else if(_tcscmp(m_szInpSafixs, _T("LOG")) == 0){
			LoadLogoFile(fname);			
		}
		else if(_tcscmp(m_szInpSafixs, _T("WAK")) == 0){
			LoadVectFile(fname);			
		}
		else if(_tcscmp(m_szInpSafixs, _T("EPS")) == 0){
			LoadEpsFile(fname);			
		}
		else if(_tcscmp(m_szInpSafixs, _T("AI")) == 0){
			LoadEpsFile(fname);			
		}
		else if(_tcscmp(m_szInpSafixs, _T("DXF")) == 0){
			LoadDxfFile(fname);			
		}
		else if(_tcscmp(m_szInpSafixs, _T("BMP")) == 0){
			LoadImageFile(fname);			
		}
		else if(_tcscmp(m_szInpSafixs, _T("JPG")) == 0){
			LoadImageFile(fname);
		}
		else if(_tcscmp(m_szInpSafixs, _T("TIF")) == 0){
			LoadImageFile(fname);
		}
		else if(_tcscmp(m_szInpSafixs, _T("WMF")) == 0){
			LoadWMFFile(fname);			
		}
		else if(_tcscmp(m_szInpSafixs, _T("EMF")) == 0){
			LoadEMFFile(fname);			
		}
		else if(_tcscmp(m_szInpSafixs, _T("ED")) == 0){
			LoadArcFile(fname);			
		}
		else{
			return(FALSE);
		}

		if(_tcscmp(m_szInpSafixs, _T("VCT")) != 0){
			TrnsOverScreenData();
		}
		ScrnSizeReset();

		senvgetexepath(exepath);
		SetCurrentDirectory(exepath);

		return(TRUE);
	}
	senvgetexepath(exepath);
	SetCurrentDirectory(exepath);

	return(FALSE);
}

void CCmdFile::LoadVectFile(LPTSTR filename)
{
	CFileVect*	pcFileVect;
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	pcFileVect = new CFileVect(m_pcView);
	pcFileVect->LoadVectFile(filename);
	delete(pcFileVect);
}

void CCmdFile::LoadLogoFile(LPTSTR filename)
{
	CFileVect*	pcFileVect;
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	pcFileVect = new CFileVect(m_pcView);
	pcFileVect->LoadLogoFile(filename);
	delete(pcFileVect);

}

void CCmdFile::LoadEpsFile(LPTSTR filename)
{
	CFileEps*	pcFileEps;
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	pcFileEps = new CFileEps(m_pcView);
	pcFileEps->LoadData(filename);
	delete(pcFileEps);
}

void CCmdFile::SaveEpsFile(LPTSTR fname)
{
	SaveUndo();
	ActiveDataChange(SWAKVECTID);
	CFileEps*	pcFileEps;
	pcFileEps = new CFileEps(m_pcView);
	pcFileEps->SaveData(fname);
	delete(pcFileEps);
	LoadUndo();
}

void CCmdFile::LoadImageFile(LPTSTR fname)
{
	CTonDoc*	pcDoc;
	CDibCData*	pcDibCData;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDibCData = (CDibCData*)(pcDoc->NewData(DIBCDATAID));
	if(pcDibCData == NULL){
		return;
	}
	pcDibCData->SetFileName(fname);
	pcDibCData->InitElement();
	pcDibCData->RenewMiniMax();
	ScrnRedraw();
	ActiveDataAllOff();
	AddLinkDoc(pcDibCData);
}
void CCmdFile::LoadEMFFile(LPTSTR loadfname)
{
	CFile	cpFile;
	HGLOBAL	ph;
	LPBYTE	lpBuf;
	DWORD	size;
	CMetaData	*pcMetaData;
	DBL		dstsx, dstsy, dstxsize, dstysize;
	DBL		xsize, ysize, xtime, ytime;

	if((cpFile.Open(loadfname, CFile::modeRead)) == FALSE){
		return;
	}
	size = (DWORD)cpFile.GetLength();
	ph = ::GlobalAlloc(GHND, size);
	lpBuf = (LPBYTE)::GlobalLock(ph);
	cpFile.Read(lpBuf, size);
	cpFile.Close();
	scrngetmmsize(&dstxsize, &dstysize);
	dstxsize /= 3; dstysize /= 3;
	dstsx = dstxsize; dstsy = dstysize;

	pcMetaData = (CMetaData *)m_pcDoc->NewData(METADATAID);
	pcMetaData->m_hEmf = ::SetEnhMetaFileBits(size, lpBuf);
	ENHMETAHEADER *enh = (ENHMETAHEADER *)lpBuf;
	xsize = (enh->rclFrame.right - enh->rclFrame.left) / 100;
	ysize = (enh->rclFrame.bottom - enh->rclFrame.top) / 100;
	xtime = dstxsize / xsize;
	ytime = dstysize / ysize;
	if(xtime < ytime){
		dstysize = ysize * xtime;
	}
	else{
		dstxsize = xsize * ytime;
	}
	pcMetaData->SetDstElement(dstsx, dstsy, dstxsize, dstysize);
	::GlobalUnlock(ph);
	::GlobalFree(ph);
	pcMetaData->RenewMiniMax();
	ScrnDataRedraw(pcMetaData);
	ActiveDataAllOff();
	AddLinkDoc(pcMetaData);
}

void CCmdFile::LoadWMFFile(LPTSTR loadfname)
{
	CFile	cpFile;
	HGLOBAL	ph;
	LPBYTE	lpBuf;
	DWORD	size;
	CMetaData	*pcMetaData;
	DBL		dstsx, dstsy, dstxsize, dstysize;
	DBL		xsize, ysize, xtime, ytime;

	if((cpFile.Open(loadfname, CFile::modeRead)) == FALSE){
		return;
	}
	size = (DWORD)cpFile.GetLength();
	ph = ::GlobalAlloc(GHND, size);
	lpBuf = (LPBYTE)::GlobalLock(ph);
	cpFile.Read(lpBuf, size);
	cpFile.Close();
	scrngetmmsize(&dstxsize, &dstysize);
	dstxsize /= 3; dstysize /= 3;
	dstsx = dstxsize; dstsy = dstysize;

	pcMetaData = (CMetaData *)m_pcDoc->NewData(METADATAID);
	pcMetaData->m_hEmf = ::SetWinMetaFileBits(size-APMSIZE, lpBuf+APMSIZE, NULL, NULL);
	APMFILEHEADER *apm = (APMFILEHEADER*)lpBuf;
	xsize = (apm->bbox.Right - apm->bbox.Left) / apm->inch * 25.4;
	ysize = (apm->bbox.Bottom - apm->bbox.Top) / apm->inch * 25.4;
	xtime = dstxsize / xsize;
	ytime = dstysize / ysize;
	if(xtime < ytime){
		dstysize = ysize * xtime;
	}
	else{
		dstxsize = xsize * ytime;
	}
	pcMetaData->SetDstElement(dstsx, dstsy, dstxsize, dstysize);
	::GlobalUnlock(ph);
	::GlobalFree(ph);
	pcMetaData->RenewMiniMax();
	ScrnDataRedraw(pcMetaData);
	ActiveDataAllOff();
	AddLinkDoc(pcMetaData);
}

void CCmdFile::MoveTrnsMain()
{
	CTonDoc*	pcDoc;
	DBL		movex, movey;

	movex = m_dSX - m_dBSX;
	movey = m_dSY - m_dBSY;
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->MoveTrns(movex, movey);
}

void CCmdFile::TrnsOverScreenData()
{
	CTonDoc*	pcDoc;
	DBL		minix, miniy, maxx, maxy;
	DBL		dataxsize, dataysize;
	DBL		scrnxsize, scrnysize;
	DBL		timex, timey, time;
	DBL		movex, movey;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	dataxsize = maxx - minix;
	dataysize = maxy - miniy;

	scrngetmmsize(&scrnxsize, &scrnysize);
	if(scrnxsize < maxx || scrnysize < maxy){
		scrnxsize -= ADDMM*2;
		scrnysize -= ADDMM*2;
		movex = ADDMM - minix;
		movey = ADDMM - miniy;
		pcDoc->MoveTrns(movex, movey);
	}
	if(scrnxsize < dataxsize || scrnysize < dataysize){
		timex = scrnxsize / dataxsize;
		timey = scrnysize / dataysize;
		if(timex < timey){
			time = timex;
		}
		else{
			time = timey;
		}
		pcDoc->SizeTrns(ADDMM, ADDMM, time, time);
	}
}

int CCmdFile::BitMapImageSave(CArchive& ar)
{
	CBmpFile *pcBmpFile;
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	BYTE	uch;
	CFile	cpFile;
	ULONG	lFileSize;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\crt$$img.bmp"), exepath);
	pcBmpFile = new CBmpFile(m_pcView);
	pcBmpFile->CreateBmpInit(128, 128);
	pcBmpFile->CreateBmpImage(fname);
	pcBmpFile->CreateBmpEnd();
	delete(pcBmpFile);

	lFileSize = 0;
	if((cpFile.Open(fname, CFile::modeRead, NULL)) == FALSE){
		return(FALSE);
	}
	lFileSize = (ULONG)cpFile.GetLength();
	ar << lFileSize;
	while (lFileSize--) {
		cpFile.Read(&uch, 1);
		ar << uch;
		if (!(lFileSize % 1000)) {
			exepath[0] = uch;
		}
	}
	cpFile.Close();
	return(TRUE);
}

void CCmdFile::BitMapImageLoad(CArchive& ar)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	uch;
	CFile	cpFile;
	long	lFileSize;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\crt$$img.bmp"), exepath);

	if((cpFile.Open(fname, CFile::modeCreate|CFile::modeWrite, NULL)) == FALSE){
		return;
	}
	ar >> lFileSize;
	while(lFileSize--){
		ar >> uch;
		cpFile.Write(&uch, 1);
	}
	cpFile.Close();
}

void CCmdFile::LoadArcFile(LPTSTR filename)
{
	CFileArc*	pcFileArc;
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	pcFileArc = new CFileArc(m_pcView);
	pcFileArc->LoadData(filename);
	delete(pcFileArc);
}

void CCmdFile::SaveDxfFile(LPTSTR filename)
{
	CFileDxf*	pcFileDxf;

	pcFileDxf = new CFileDxf(m_pcView);
	pcFileDxf->SaveData(filename);
	delete(pcFileDxf);
}

void CCmdFile::LoadDxfFile(LPTSTR filename)
{
	CFileDxf*	pcFileDxf;
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	pcFileDxf = new CFileDxf(m_pcView);
	pcFileDxf->LoadData(filename);
	delete(pcFileDxf);
}

void CCmdFile::GetDirName(LPTSTR filename, LPTSTR dirname)
{
	int		len, i, pt;
	int		hc;

	len = _tcslen(filename);
	i = 0;
	pt = 0;
	while(1){
		hc = filename[i] & 0x00ff;
		if(hc == _T('\0')){
			break;
		}
		if(baseiskanji(hc) == ON){
			i += 2;
		}
		else{
			if(filename[i] == _T(':')){
				pt = i;
			}
			else if(filename[i] == _T('\\')){
				pt = i;
			}
			i++;
		}
	}
	len = _tcslen(filename)+1;
	_tcscpy_s(dirname, len, filename);
	dirname[pt] = _T('\0');
}

void CCmdFile::SaveGyousya(LPTSTR lpGyousya)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	lstfname[FNAMEMAX];
	TCHAR	bakfname[FNAMEMAX];
	FILE	*lstfp;
	FILE	*bakfp;
	TCHAR	buf[RECMAX];
	int		addflag;
	int		len;

	senvgetexepath(exepath);
	_stprintf_s(lstfname, _T("%s\\fgyousya.lst"), exepath);
	_stprintf_s(bakfname, _T("%s\\fgyousya.bak"), exepath);
	CopyFile(lstfname, bakfname, FALSE);
	
	_tfopen_s(&lstfp, lstfname, _T("w"));
	if(lstfp == NULL){
		return;
	}
	addflag = ON;
	_tfopen_s(&bakfp, bakfname, _T("r"));
	if(bakfp != NULL){
		while(1){
			if(baselibCrDelFgets(buf, RECMAX, bakfp) == NULL){
				break;
			}
			if(_tcscmp(buf, lpGyousya) == 0){
				addflag = OFF;
			}
			_stprintf_s(buf, _T("%s\n"), buf);
			baselibFputs(buf, lstfp);
		}
		
		fclose(bakfp);
	}
	if(addflag == ON){
		_stprintf_s(buf, _T("%s\n"), lpGyousya);
		baselibFputs(buf, lstfp);
	}
	
	fclose(lstfp);
}
	
void CCmdFile::SaveKouji(LPTSTR lpKouji)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	lstfname[FNAMEMAX];
	TCHAR	bakfname[FNAMEMAX];
	FILE	*lstfp;
	FILE	*bakfp;
	TCHAR	buf[RECMAX];
	int		addflag;
	int		len;

	senvgetexepath(exepath);
	_stprintf_s(lstfname, _T("%s\\fkouji.lst"), exepath);
	_stprintf_s(bakfname, _T("%s\\fkouji.bak"), exepath);
	CopyFile(lstfname, bakfname, FALSE);
	
	_tfopen_s(&lstfp, lstfname, _T("w"));
	if(lstfp == NULL){
		return;
	}
	addflag = ON;
	_tfopen_s(&bakfp, bakfname, _T("r"));
	if(bakfp != NULL){
		while(1){
			if(baselibCrDelFgets(buf, RECMAX, bakfp) == NULL){
				break;
			}
			if(_tcscmp(buf, lpKouji) == 0){
				addflag = OFF;
			}
			_stprintf_s(buf, _T("%s\n"), buf);
			baselibFputs(buf, lstfp);
		}
		
		fclose(bakfp);
	}
	if(addflag == ON){
		_stprintf_s(buf, _T("%s\n"), lpKouji);
		baselibFputs(buf, lstfp);
	}
	
	fclose(lstfp);
}

long CCmdFile::CheckFreeSpace(LPTSTR path)
{
	ULONG	lSPC, lBPS, lFC, lTC;
	ULONG	freesize;

	::GetDiskFreeSpace(path, &lSPC, &lBPS, &lFC, &lTC);
	freesize = lBPS * lSPC * lFC;
	return(freesize);
}

void CCmdFile::ErrorMessageOut(CFileException *e)
{
	TCHAR	msg[RECMAX];

	switch(e->m_cause){
	case CFileException::fileNotFound:
		_stprintf_s(msg, _T("ファイル名が正しくありません"));
		::AfxMessageBox(msg, MB_OK);
		break;
	case CFileException::badPath:
		_stprintf_s(msg, _T("パス名が正しくありません"));
		::AfxMessageBox(msg, MB_OK);
		break;
	case CFileException::tooManyOpenFiles:
		_stprintf_s(msg, _T("これ以上ファイルを開けません"));
		::AfxMessageBox(msg, MB_OK);
		break;
	case CFileException::accessDenied:
		_stprintf_s(msg, _T("書込み禁止状態のファイルで開けません"));
		::AfxMessageBox(msg, MB_OK);
		break;
	case CFileException::directoryFull:
	case CFileException::diskFull:
		_stprintf_s(msg, _T("ディスクがいっぱいです"));
		::AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
		break;
	default:
		_stprintf_s(msg, _T("ファイルが作成できませんでした"));
		::AfxMessageBox(msg, MB_OK);
		break;
	}
}

void CCmdFile::CopyCrctFile(LPTSTR lpSrcFname, LPTSTR lpDstFname)
{
	FILE	*srcfp;
	FILE	*dstfp;
	TCHAR	szSrcBuf[RECMAX8+16];
	TCHAR	szDstBuf[RECMAX8+16];
	TCHAR	szSrcBaseName[FNAMEMAX];
	TCHAR	szDstBaseName[FNAMEMAX];
	TCHAR	szDstBaseNoName[FNAMEMAX];
	LPTSTR	csvsp[RECMAX];
	int		idx, cnt;
	int		len;

	_tcscpy_s(szDstBaseName, lpDstFname);
	szDstBaseName[_tcslen(szDstBaseName)-4] = _T('\0');
	_tfopen_s(&srcfp, lpSrcFname, _T("r,ccs=UNICODE"));
	_tfopen_s(&dstfp, lpDstFname, _T("w,ccs=UNICODE"));
	idx = 0;
	while(1){
		if(_fgetts(szSrcBuf, RECMAX8, srcfp) == NULL){
			break;
		}
		len = _tcslen(szSrcBuf);
		szSrcBuf[len-1] = _T('\0');
		cnt = basedivcsvdata(csvsp, szSrcBuf);
		_tcscpy_s(szSrcBaseName, csvsp[3]);
		szSrcBaseName[_tcslen(szSrcBaseName)-4] = _T('\0');

		_stprintf_s(szDstBaseNoName, _T("%s$%03d"), szDstBaseName, idx);

		m_pcDoc->CopyWakuFileBlock(szSrcBaseName, szDstBaseNoName);
		_stprintf_s(szDstBuf, _T("\"%s\",\"%s\",\"%s\",\"%s.wak\",\"%s\",\"%s\"\n"),
			csvsp[0], csvsp[1], csvsp[2], szDstBaseNoName, csvsp[4], csvsp[5]);
		baselibFputs(szDstBuf, dstfp);
		idx++;
	}
	fclose(srcfp);
	fclose(dstfp);
}

void CCmdFile::DeleteCrctFile(LPTSTR lpFileName)
{
	FILE	*fp;
	TCHAR	szBuf[RECMAX8+16];
	TCHAR	szBaseName[FNAMEMAX];
	TCHAR	szDelFileName[FNAMEMAX];
	LPTSTR	csvsp[RECMAX];
	int		cnt;
	int		len;

	_tfopen_s(&fp, lpFileName, _T("r"));
	while(1){
		if(_fgetts(szBuf, RECMAX8, fp) == NULL){
			break;
		}
		len = _tcslen(szBuf);
		szBuf[len-1] = _T('\0');
		cnt = basedivcsvdata(csvsp, szBuf);
		_tcscpy_s(szBaseName, csvsp[3]);
		szBaseName[_tcslen(szBaseName)-4] = _T('\0');
		_stprintf_s(szDelFileName, _T("%s.iti"), szBaseName);
		_tremove(szDelFileName);
		_stprintf_s(szDelFileName, _T("%s.bin"), szBaseName);
		_tremove(szDelFileName);
		_stprintf_s(szDelFileName, _T("%s.env"), szBaseName);
		_tremove(szDelFileName);
		_stprintf_s(szDelFileName, _T("%s.wak"), szBaseName);
		_tremove(szDelFileName);
	}
	fclose(fp);
	_tremove(lpFileName);
}

