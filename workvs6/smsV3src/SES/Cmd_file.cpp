
/////////////////////////////////////////////////////////////////////////////
// CCmdFile

#include "stdafx.h"
#include "math.h"
#include "ctype.h"
#include "string.h"
#include "direct.h"

#include "..\cmnsrc\cmd_base.h"
#include "..\cmnsrc\bmpfile.h"
#include "..\ses\cmd_file.h"
#include "..\cmnsrc\ViewFDlg.h"
#include "..\cmnsrc\filevect.h"
#include "..\ses\fileeps.h"
#include "..\cmnsrc\filedxf.h"
#include "..\cmnsrc\filearc.h"

#include "..\imgkit\Imagekit.h"

extern "C" {
	void senvgetdatpath(LPSTR path);
	void senvgettmppath(LPSTR path);
	void senvgetexepath(LPSTR path);
	void scrngetmmsize(DBL *scrnxsize, DBL *scrnysize);
	void scrnsetmmsize(DBL scrnxsize, DBL scrnysize);
	void scrnonnoactflag();
	void scrnoffnoactflag();
	void diblibcreatebmpfile(LPSTR bmpfname);
	void baseaddsafix(LPSTR FileName, LPSTR Safix);
	void basechangesafix(LPSTR FileName, LPSTR Safix);
	int baseiskanji(UINT far hc);
	int basedivcsvdata(LPSTR csvsp[], LPSTR csvbuf);
}

//static char BASED_CODE szFilter[] = "Chart Files (*.xlc)|*.xlc|Worksheet Files (*.xls)|*.xls|Data Files (*.xlc;*.xls)|*.xlc; *.xls|All Files (*.*)|*.*||";
//static char BASED_CODE szFilter[] = "Vect Files (*.vct;*.log;*.wak)|*.vct;*.log;*.wak|Bitmap File (*.tiff;*.jpg)|*.tiff;*.jpg||";
static char BASED_CODE szExportFilter[] = "Eps Files (*.eps)|*.eps|Dxf Files (*.dxf)|*.dxf||";
static char BASED_CODE szFilter[RECMAX*2];
static char BASED_CODE szSafixs[8];

CCmdFile::CCmdFile(CScrollView* pcview)
:CCmdBase(pcview)
{
	char	exepath[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	fname[FNAMEMAX];
	char	buf[HRECMAX];
	FILE	*fp;

	strcpy(m_szInpSafixs, "vct");
	strcpy(m_szExpSafixs, "eps");
	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmd_file.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", m_szInpSafixs);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", m_szExpSafixs);
		fclose(fp);
	}
	senvgetdatpath(datpath);
	strcpy(n_szInpPath, datpath);
	strcpy(n_szExpPath, datpath);
	strcpy(n_szDatPath, datpath);
}

CCmdFile::~CCmdFile()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmd_file.env", exepath);
	if((fp = fopen(fname, "w")) != NULL){
		fprintf(fp, "%s\n", m_szInpSafixs);
		fprintf(fp, "%s\n", m_szExpSafixs);
		fclose(fp);
	}
}

void CCmdFile::SetFilter(LPSTR filter, LPSTR safixs)
{
	strcpy(szFilter, filter); 
	strcpy(szSafixs, safixs);
}

UINT CCmdFile::ExecMyFileLoad(UINT event, DBL dx, DBL dy)
{
	CViewFileDialog	*pcViewFileDialog;
	char	fname[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	safixs[QRECMAX];
	char	szSrch[RECMAX];
	CTonDoc*	pcDoc;
	int		nVerNo;
	CString		csMsg;
	int		nResave;
	char	szResaveBuf[RECMAX];
	int		i;

	if(event != INITEVENT){
		return(0);
	}
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	csMsg = pcDoc->GetTitle();
	strcpy(safixs, "ess");
	sprintf(szSrch, "*.%s", safixs);
	sprintf(szFilter, "%s Files (*.%s)|*.%s||", pcDoc->m_szSystemName, safixs, safixs);

	strcpy(datpath, n_szDatPath);
	SetCurrentDirectory(datpath);
	pcViewFileDialog = new CViewFileDialog(TRUE, NULL, szSrch, NULL, szFilter, m_pcView);
	pcViewFileDialog->SetTitle("ファイル読込");
	if(pcViewFileDialog->DoModal( ) == IDOK){
		CString cpStr = pcViewFileDialog->GetPathName();
		strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
		baseaddsafix(fname, safixs);
		GetDirName(fname, n_szDatPath);
		pcDoc->SetTitle(fname);	
		CFile file(fname, CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		ar >> nVerNo;
		BitMapImageLoad(ar);
		ar >> csMsg;
		//strcpy(pcFileDialog->m_szGyousya, csMsg.GetBuffer(QRECMAX));
		ar >> csMsg;
		//strcpy(pcFileDialog->m_szKouji, csMsg.GetBuffer(QRECMAX));
		ar >> csMsg;
		//strcpy(pcFileDialog->m_szComent1, csMsg.GetBuffer(QRECMAX));
		ar >> csMsg;
		//strcpy(pcFileDialog->m_szComent2, csMsg.GetBuffer(QRECMAX));
		ar >> csMsg;
		//strcpy(pcFileDialog->m_szComent3, csMsg.GetBuffer(QRECMAX));
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
	char	fname[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	safixs[QRECMAX];
	char	szSrch[RECMAX];
	CTonDoc*	pcDoc;
	int		nVerNo;
	char	msg[RECMAX+QRECMAX];
	CString		csMsg;
	CString		cpStr;
	int		nResave;
	char	szResaveBuf[RECMAX];
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
	strcpy(safixs, "ess");
	//sprintf(szSrch, "*.%s", safixs);
	sprintf(szSrch, "%s", csMsg.GetBuffer(FNAMEMAX));
	sprintf(szFilter, "%s Files (*.%s)|*.%s||", pcDoc->m_szSystemName, safixs, safixs);

	strcpy(datpath, n_szDatPath);
	SetCurrentDirectory(datpath);
	pcViewFileDialog = new CViewFileDialog(FALSE, NULL, szSrch, NULL, szFilter, m_pcView);
	pcViewFileDialog->SetTitle("ファイル保存");
	ret = pcViewFileDialog->DoModal( );
	if(ret != IDOK){
		return(ENDEVENT);
		//goto ENDLABEL;
	}

	cpStr = pcViewFileDialog->GetPathName();
	strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
	baseaddsafix(fname, safixs);
	GetDirName(fname, n_szDatPath);
	if(file.Open(fname, CFile::modeRead, NULL)){
		file.Close();
		sprintf(msg, "%sに上書きしてもよろしいですか", fname);
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
		::AfxMessageBox("システムエラーが発生しました", MB_OK);
		pcException->Delete();
	}
	*/
//ENDLABEL:
	delete(pcViewFileDialog);
	senvgetexepath(exepath);
	SetCurrentDirectory(exepath);
	if(pcDoc->m_nError == TRUE){
		unlink(fname);
	}
	return(ENDEVENT);
}

UINT CCmdFile::ExecAddLoad(UINT event, DBL dx, DBL dy)
{
	char	fname[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	srchptn[QRECMAX];
	char	safixs[QRECMAX];

	if(event == INITEVENT){
		InitClass();
		sprintf(srchptn, "*.%s", szSafixs);
		strcpy(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("追加用ファイル読込");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
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
	char	fname[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	srchptn[QRECMAX];
	char	safixs[QRECMAX];
	CTonDoc*	pcDoc;
	char	szBuf[16];
	short		nBmpMark;
	long		lImageSize;

	if(event == INITEVENT){
		sprintf(srchptn, "*.%s", szSafixs);
		strcpy(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("追加用ファイル読込");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
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

			basechangesafix(fname, "env");
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
	char	fname[FNAMEMAX];
	char	tmpfname[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	srchptn[QRECMAX];
	char	safixs[QRECMAX];
	CTonDoc*	pcDoc;

	if(event == INITEVENT){
		sprintf(srchptn, "*.%s", szSafixs);
		strcpy(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(FALSE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("追加用ファイル保存");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			baseaddsafix(fname, szSafixs);

			pcDoc->SaveFile(fname);

			basechangesafix(fname, "env");
			SaveCrtEnv(fname);

			basechangesafix(tmpfname, szSafixs);

		}
		senvgetexepath(exepath);
		SetCurrentDirectory(exepath);


		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdFile::ExecAddDel(UINT event, DBL dx, DBL dy)
{
	char	fname[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	srchptn[QRECMAX];
	char	safixs[QRECMAX];
	char	msgbuf[RECMAX];
	CTonDoc*	pcDoc;
	int		ret;

	if(event == INITEVENT){
		sprintf(srchptn, "*.%s", szSafixs);
		strcpy(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("追加用ファイル削除");
		pcFileDialog->SetBtnText("削除");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			sprintf(msgbuf, "<<%s>>ファイルを削除してよろしいですか", fname);
			ret = AfxMessageBox(msgbuf, MB_OKCANCEL);
			if(ret == IDOK){
				unlink(fname);
				basechangesafix(fname, "env");
				unlink(fname);
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
	char	fname[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	srchptn[QRECMAX];
	char	msgbuf[RECMAX];
	CTonDoc*	pcDoc;
	int		ret;

	if(event == INITEVENT){
		sprintf(srchptn, "*.*");
		strcpy(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("ファイル削除");
		pcFileDialog->SetBtnText("削除");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			sprintf(msgbuf, "<<%s>>ファイルを削除してよろしいですか", fname);
			ret = AfxMessageBox(msgbuf, MB_OKCANCEL);
			if(ret == IDOK){
				unlink(fname);
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
	char	fname[FNAMEMAX];
	char	crctfname[FNAMEMAX];
	char	backfname[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	srchptn[QRECMAX];
	char	safixs[QRECMAX];
	CTonDoc*	pcDoc;
	int		ret;
	LPVOID	lpMsgBuf;

	if(event == INITEVENT){
		sprintf(srchptn, "*.%s", szSafixs);
		strcpy(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("貯め込みファイル読込");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());

			senvgetexepath(exepath);
			sprintf(crctfname, "%s\\outcrect.dat", exepath);

			senvgetexepath(exepath);
			sprintf(backfname, "%s\\outcrect.bak", exepath);

			CopyFile(crctfname, backfname, FALSE);
			ret = CopyFile(fname, crctfname, FALSE);
			if(ret == 0){
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
				AfxMessageBox((LPSTR)lpMsgBuf);
			}
		}
		senvgetexepath(exepath);
		SetCurrentDirectory(exepath);

		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdFile::ExecCrctSave(UINT event, DBL dx, DBL dy)
{
	char	msg[HRECMAX];
	char	fname[FNAMEMAX];
	char	crctfname[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	srchptn[QRECMAX];
	char	safixs[QRECMAX];
	CTonDoc*	pcDoc;

	if(event == INITEVENT){
		sprintf(srchptn, "*.%s", szSafixs);
		strcpy(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(FALSE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("貯め込みファイル保存");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			baseaddsafix(fname, szSafixs);
			CFile file;
			if(file.Open(fname,  CFile::modeRead, NULL)){
				file.Close();
				sprintf(msg, "%sに上書きしてもよろしいですか", fname);
				if(AfxMessageBox(msg, MB_OKCANCEL) == IDCANCEL){
					return(ENDEVENT);
				}
			}

			senvgetexepath(exepath);
			sprintf(crctfname, "%s\\outcrect.dat", exepath);
			SaveCrctFile(crctfname, fname);
		}
		senvgetexepath(exepath);
		SetCurrentDirectory(exepath);

		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdFile::ExecCrctDel(UINT event, DBL dx, DBL dy)
{
	char	fname[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	srchptn[QRECMAX];
	char	safixs[QRECMAX];
	char	msgbuf[RECMAX];
	CTonDoc*	pcDoc;
	int		ret;

	if(event == INITEVENT){
		sprintf(srchptn, "*.%s", szSafixs);
		strcpy(datpath, n_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("貯め込みファイル削除");
		pcFileDialog->SetBtnText("削除");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, n_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());

			sprintf(msgbuf, "<<%s>>ファイルを削除してよろしいですか", fname);
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
	char	msg[RECMAX2];
	char	fname[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	safix[QRECMAX];
	char	epsfilter[QRECMAX];
	char	dxffilter[QRECMAX];
	int		filteridx;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();

	strcpy(epsfilter, "Eps Files (*.eps)|*.eps|");
	sprintf(safix, "*.%s", m_szExpSafixs);
	strcpy(dxffilter, "DXF Files (*.dxf)|*.dxf|");

	//sprintf(safix, "*.%s", m_szExpSafixs);
	//strcpy(safix, "*.eps");
	//sprintf(szFilter, "%s|", epsfilter);

	if(strcmp(m_szExpSafixs, "DXF") == 0){
		strcpy(safix, "*.dxf");
		sprintf(szFilter, "%s%s|", dxffilter, epsfilter);
	}
	else{
		strcpy(safix, "*.eps");
		sprintf(szFilter, "%s%s|", epsfilter, dxffilter);
	}

	strcpy(datpath, n_szExpPath);
	SetCurrentDirectory(datpath);
	CMyFileDialog *pcFileDialog = new CMyFileDialog(FALSE, NULL, safix, NULL, szFilter, m_pcView);
	pcFileDialog->SetTitle("外部ファイル保存");
	if(pcFileDialog->DoModal( ) == IDOK){
		CString cpExt = pcFileDialog->GetFileExt();
		filteridx = (pcFileDialog->m_ofn).nFilterIndex;
		CString cpStr = pcFileDialog->GetPathName();
		strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
		GetDirName(fname, n_szExpPath);
		if(filteridx == 1){
			baseaddsafix(fname, "eps");	
		}
		else if(filteridx == 2){
			baseaddsafix(fname, "dxf");
		}
		CFile file;
		if(file.Open(fname,  CFile::modeRead, NULL)){
			file.Close();
			sprintf(msg, "%sに上書きしてもよろしいですか", fname);
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

void CCmdFile::SaveCrtEnv(LPSTR fname)
{
	FILE	*fp;
	DBL		xmmsize, ymmsize;

	scrngetmmsize(&xmmsize, &ymmsize);
	if((fp = fopen(fname, "w")) == NULL){
		return;
	}
	fprintf(fp, "%1.1lf %1.1lf\n", xmmsize, ymmsize);
	fclose(fp);
}

void CCmdFile::LoadCrtEnv(LPSTR fname)
{
	FILE	*fp;
	char	buf[RECMAX];
	DBL		xmmsize, ymmsize;

	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%lf %lf\n", &xmmsize, &ymmsize);
	if(fgets(buf, RECMAX, fp) != NULL){
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		if(fgets(buf, RECMAX, fp) == NULL){
			fclose(fp);
			return;
		}
		sscanf(buf, "%lf %lf\n", &xmmsize, &ymmsize);
	}
	scrnsetmmsize(xmmsize, ymmsize);
	fclose(fp);
	((CTonView*)m_pcView)->WorkSizeReset();
	((CTonView*)m_pcView)->WorkAreaReset();
}

BOOL CCmdFile::LoadInportFile()
{
	char	fname[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	safix[RECMAX];
	char	vctfilter[QRECMAX];
	char	epsfilter[QRECMAX];
	char	sksifilter[QRECMAX];
	char	arcfilter[QRECMAX];
	char	rasterfilter[QRECMAX];
	char	metafilter[QRECMAX];

	strcpy(vctfilter, "Vect Files (*.vct;*.log;*.wak)|*.vct; *.log; *.wak|");
	strcpy(epsfilter, "Eps&DXF Files (*.eps;*.ai;*.dxf)|*.eps;*.ai;*.dxf|");
	strcpy(rasterfilter, "Raster Files (*.bmp;*.jpg;*.tif)|*.bmp;*.jpg;*.tif|");
#ifdef	SOA
	strcpy(metafilter, "WinMeta Files (*.wmf;*.emf)|*.wmf;*.emf|");
#else
	strcpy(metafilter, "");
#endif
	strcpy(arcfilter, "ARCPART Files (*.;*.ed)|*.; *.ed|");
	strcpy(sksifilter, "SKSI Files (*.;*.wrk)|*.; *.wrk|");

	strcpy(datpath, n_szInpPath);
	SetCurrentDirectory(datpath);
	if(strcmp(m_szInpSafixs, "VCT") == 0
	|| strcmp(m_szInpSafixs, "LOG") == 0
	|| strcmp(m_szInpSafixs, "WAK") == 0){
		strcpy(safix, "*.vct;*.log;*.wak");
		sprintf(szFilter, "%s%s%s%s%s%s|"
			, vctfilter, epsfilter, metafilter
			, rasterfilter
			, sksifilter, arcfilter);
	}
	else if(strcmp(m_szInpSafixs, "EPS") == 0
		 ||strcmp(m_szInpSafixs, "AI") == 0
		 ||strcmp(m_szInpSafixs, "DXF") == 0){
		strcpy(safix, "*.eps;*.ai;*.dxf");
		sprintf(szFilter, "%s%s%s%s%s%s|"
			, epsfilter, metafilter, vctfilter
			, rasterfilter
			, sksifilter, arcfilter);
	}
	else if(strcmp(m_szInpSafixs, "WMF") == 0
		 ||strcmp(m_szInpSafixs, "EMF") == 0){
		strcpy(safix, "*.wmf;*.emf");
		sprintf(szFilter, "%s%s%s%s%s%s|"
			, metafilter, epsfilter, vctfilter
			, rasterfilter
			, sksifilter, arcfilter);
	}
	else if(strcmp(m_szInpSafixs, "BMP") == 0
		 ||strcmp(m_szInpSafixs, "JPG") == 0
		 ||strcmp(m_szInpSafixs, "TIF") == 0){
		strcpy(safix, "*.bmp;*.jpg;*.tif");
		sprintf(szFilter, "%s%s%s%s%s%s|"
			, rasterfilter
			, metafilter, epsfilter, vctfilter
			, sksifilter, arcfilter);
	}
	else if(strcmp(m_szInpSafixs, "ED") == 0){
		strcpy(safix, "*.ed");
		sprintf(szFilter, "%s%s%s%s%s%s|"
			, arcfilter, sksifilter
			, vctfilter, epsfilter, metafilter
			, rasterfilter);
	}
	else if(strcmp(m_szInpSafixs, "WRK") == 0){
		strcpy(safix, "*.wrk");
		sprintf(szFilter, "%s%s%s%s%s%s|"
			, sksifilter, arcfilter
			, vctfilter, epsfilter, metafilter
			, rasterfilter);
	}
	else{
		strcpy(safix, "*.");
		sprintf(szFilter, "%s%s%s%s%s%s|"
			, sksifilter, arcfilter
			, vctfilter, epsfilter, metafilter
			, rasterfilter);
	}

	CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, safix, NULL, szFilter, m_pcView);
	pcFileDialog->SetTitle("外部ファイル読込");
	if(pcFileDialog->DoModal( ) == IDOK){
		CString cpExt = pcFileDialog->GetFileExt();
		strcpy(m_szInpSafixs, cpExt.GetBuffer(QRECMAX));
		CString cpStr = pcFileDialog->GetPathName();
		strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
		GetDirName(fname, n_szInpPath);
		strupr(m_szInpSafixs);

		SaveUndo();
		if(strcmp(m_szInpSafixs, "VCT") == 0){
			LoadVectFile(fname);			
		}
		else if(strcmp(m_szInpSafixs, "LOG") == 0){
			LoadLogoFile(fname);			
		}
		else if(strcmp(m_szInpSafixs, "WAK") == 0){
			LoadVectFile(fname);			
		}
		else if(strcmp(m_szInpSafixs, "EPS") == 0){
			LoadEpsFile(fname);			
		}
		else if(strcmp(m_szInpSafixs, "AI") == 0){
			LoadEpsFile(fname);			
		}
		else if(strcmp(m_szInpSafixs, "DXF") == 0){
			LoadDxfFile(fname);			
		}
		else if(strcmp(m_szInpSafixs, "BMP") == 0){
			LoadBmpFile(fname);			
		}
		else if(strcmp(m_szInpSafixs, "JPG") == 0){
			LoadJPGFile(fname);			
		}
		else if(strcmp(m_szInpSafixs, "TIF") == 0){
			LoadTIFFFile(fname);			
		}
		else if(strcmp(m_szInpSafixs, "WMF") == 0){
			LoadWMFFile(fname);			
		}
		else if(strcmp(m_szInpSafixs, "EMF") == 0){
			LoadEMFFile(fname);			
		}
		else if(strcmp(m_szInpSafixs, "ED") == 0){
			LoadArcFile(fname);			
		}
		else{
			return(FALSE);
		}

		if(strcmp(m_szInpSafixs, "VCT") != 0){
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

void CCmdFile::LoadVectFile(LPSTR filename)
{
	CFileVect*	pcFileVect;
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	pcFileVect = new CFileVect(m_pcView);
	pcFileVect->LoadVectFile(filename);
	delete(pcFileVect);
}

void CCmdFile::LoadLogoFile(LPSTR filename)
{
	CFileVect*	pcFileVect;
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	pcFileVect = new CFileVect(m_pcView);
	pcFileVect->LoadLogoFile(filename);
	delete(pcFileVect);

}

void CCmdFile::LoadEpsFile(LPSTR filename)
{
	CFileEps*	pcFileEps;
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	pcFileEps = new CFileEps(m_pcView);
	pcFileEps->LoadData(filename);
	delete(pcFileEps);
}

void CCmdFile::SaveEpsFile(LPSTR fname)
{
	SaveUndo();
	ActiveDataChange(SWAKVECTID);
	CFileEps*	pcFileEps;
	pcFileEps = new CFileEps(m_pcView);
	pcFileEps->SaveData(fname);
	delete(pcFileEps);
	LoadUndo();
}

void CCmdFile::LoadBmpFile(LPSTR fname)
{
	CTonDoc*	pcDoc;
	CDibCData*	pcDibCData;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDibCData = (CDibCData*)(pcDoc->NewData(DIBCDATAID));
	if(pcDibCData == NULL){
		return;
	}
	pcDibCData->SetBmpFname(fname);
	pcDibCData->DibLoad();
	pcDibCData->RenewMiniMax();
	ScrnRedraw();
	ActiveDataAllOff();
	AddLinkDoc(pcDibCData);
}

void CCmdFile::LoadJPGFile(LPSTR fname)
{
	HANDLE		hndle;
	CTonDoc*	pcDoc;
	CDibCData*	pcDibCData;

	hndle = ImgKitJpegFileLoad(fname, "ＪＰＥＧファイル", "読込中", 0);
	diblibcreatebmpfile(fname);
	ImgKitBmpFileSave(fname, hndle, "ＪＰＥＧファイル",  "変換中", FALSE);
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDibCData = (CDibCData*)(pcDoc->NewData(DIBCDATAID));
	if(pcDibCData == NULL){
		return;
	}
	pcDibCData->SetBmpFname(fname);
	pcDibCData->DibLoad();
	pcDibCData->RenewMiniMax();
	ScrnRedraw();
	ActiveDataAllOff();
	AddLinkDoc(pcDibCData);
}

void CCmdFile::LoadTIFFFile(LPSTR fname)
{
	HANDLE		hndle;
	CTonDoc*	pcDoc;
	CDibCData*	pcDibCData;

	hndle = ImgKitTiffFileLoad(fname, "ＴＩＦＦファイル", "読込中", 0);
	diblibcreatebmpfile(fname);
	ImgKitBmpFileSave(fname, hndle, "ＴＩＦＦファイル",  "変換中", FALSE);
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDibCData = (CDibCData*)(pcDoc->NewData(DIBCDATAID));
	if(pcDibCData == NULL){
		return;
	}
	pcDibCData->SetBmpFname(fname);
	pcDibCData->DibLoad();
	pcDibCData->RenewMiniMax();
	ScrnRedraw();
	ActiveDataAllOff();
	AddLinkDoc(pcDibCData);
}

void CCmdFile::LoadEMFFile(LPSTR loadfname)
{
	CFile	cpFile;
	HGLOBAL	ph;
	LPBYTE	p;
	DWORD	s;
	CMetaData	*pcMetaData;
	DBL		dstsx, dstsy, dstxsize, dstysize;
	DBL		xsize, ysize, xtime, ytime;

	if((cpFile.Open(loadfname, CFile::modeRead)) == FALSE){
		return;
	}
	s = cpFile.GetLength();
	ph = ::GlobalAlloc(GHND,s);
	p = (LPBYTE)::GlobalLock(ph);
	cpFile.ReadHuge(p, s);
	cpFile.Close();
	scrngetmmsize(&dstxsize, &dstysize);
	dstxsize /= 3; dstysize /= 3;
	dstsx = dstxsize; dstsy = dstysize;

	pcMetaData = (CMetaData *)m_pcDoc->NewData(METADATAID);
	pcMetaData->m_hEmf = ::SetEnhMetaFileBits(s,p);
	ENHMETAHEADER *enh = (ENHMETAHEADER *)p;
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

void CCmdFile::LoadWMFFile(LPSTR loadfname)
{
	CFile	cpFile;
	HGLOBAL	ph;
	LPBYTE	p;
	DWORD	s;
	CMetaData	*pcMetaData;
	DBL		dstsx, dstsy, dstxsize, dstysize;
	DBL		xsize, ysize, xtime, ytime;

	if((cpFile.Open(loadfname, CFile::modeRead)) == FALSE){
		return;
	}
	s = cpFile.GetLength();
	ph = ::GlobalAlloc(GHND,s);
	p = (LPBYTE)::GlobalLock(ph);
	cpFile.ReadHuge(p, s);
	cpFile.Close();
	scrngetmmsize(&dstxsize, &dstysize);
	dstxsize /= 3; dstysize /= 3;
	dstsx = dstxsize; dstsy = dstysize;

	pcMetaData = (CMetaData *)m_pcDoc->NewData(METADATAID);
	pcMetaData->m_hEmf = ::SetWinMetaFileBits(s-APMSIZE, p+APMSIZE, NULL, NULL);
	APMFILEHEADER *apm = (APMFILEHEADER*)p;
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
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	UCHAR	uch;
	CFile	cpFile;
	long	lFileSize;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\crt$$img.bmp", exepath);
	pcBmpFile = new CBmpFile(m_pcView);
	pcBmpFile->CreateBmpInit(128, 128);
	pcBmpFile->CreateBmpImage(fname);
	pcBmpFile->CreateBmpEnd();
	delete(pcBmpFile);

	lFileSize = 0;
	if((cpFile.Open(fname, CFile::modeRead, NULL)) == FALSE){
		return(FALSE);
	}
	//try
	//{
		lFileSize = cpFile.GetLength();
		ar << lFileSize;
		while(lFileSize--){
			cpFile.Read(&uch, 1);
			ar << uch;
			if(!(lFileSize % 1000)){
				exepath[0] = uch;
			}
		}
		cpFile.Close();
	/*
	}
	catch(CFileException *pcFileException)
	{
		pcFileException->Delete();
		return(FALSE);
	}
	catch(CException *pcException)
	{
		pcException->Delete();
		return(FALSE);
	}
	*/
	return(TRUE);
}

void CCmdFile::BitMapImageLoad(CArchive& ar)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	UCHAR	uch;
	CFile	cpFile;
	long	lFileSize;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\crt$$img.bmp", exepath);

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

void CCmdFile::LoadArcFile(LPSTR filename)
{
	CFileArc*	pcFileArc;
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	pcFileArc = new CFileArc(m_pcView);
	pcFileArc->LoadData(filename);
	delete(pcFileArc);
}

void CCmdFile::SaveDxfFile(LPSTR filename)
{
	CFileDxf*	pcFileDxf;

	pcFileDxf = new CFileDxf(m_pcView);
	pcFileDxf->SaveData(filename);
	delete(pcFileDxf);
}

void CCmdFile::LoadDxfFile(LPSTR filename)
{
	CFileDxf*	pcFileDxf;
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	pcFileDxf = new CFileDxf(m_pcView);
	pcFileDxf->LoadData(filename);
	delete(pcFileDxf);
}

void CCmdFile::GetDirName(LPSTR filename, LPSTR dirname)
{
	int		len, i, pt;
	UINT	hc;

	len = strlen(filename);
	i = 0;
	pt = 0;
	while(1){
		hc = filename[i] & 0x00ff;
		if(hc == '\0'){
			break;
		}
		if(baseiskanji(hc) == ON){
			i += 2;
		}
		else{
			if(filename[i] == ':'){
				pt = i;
			}
			else if(filename[i] == '\\'){
				pt = i;
			}
			i++;
		}
	}
	strcpy(dirname, filename);
	dirname[pt] = '\0';
}

void CCmdFile::SaveGyousya(LPSTR lpGyousya)
{
	char	exepath[FNAMEMAX];
	char	lstfname[FNAMEMAX];
	char	bakfname[FNAMEMAX];
	FILE	*lstfp;
	FILE	*bakfp;
	char	buf[RECMAX];
	int		addflag;
	int		len;

	senvgetexepath(exepath);
	sprintf(lstfname, "%s\\fgyousya.lst", exepath);
	sprintf(bakfname, "%s\\fgyousya.bak", exepath);
	CopyFile(lstfname, bakfname, FALSE);
	
	if((lstfp = fopen(lstfname, "w")) == NULL){
		return;
	}
	addflag = ON;
	if((bakfp = fopen(bakfname, "r")) != NULL){
		while(1){
			if(fgets(buf, RECMAX, bakfp) == NULL){
				break;
			}
			len = strlen(buf);
			buf[len-1] = '\0';
			if(strcmp(buf, lpGyousya) == 0){
				addflag = OFF;
			}
			fprintf(lstfp, "%s\n", buf);
		}
		fclose(bakfp);
	}
	if(addflag == ON){
		fprintf(lstfp, "%s\n", lpGyousya);
	}
	fclose(lstfp);
}
	
void CCmdFile::SaveKouji(LPSTR lpKouji)
{
	char	exepath[FNAMEMAX];
	char	lstfname[FNAMEMAX];
	char	bakfname[FNAMEMAX];
	FILE	*lstfp;
	FILE	*bakfp;
	char	buf[RECMAX];
	int		addflag;
	int		len;

	senvgetexepath(exepath);
	sprintf(lstfname, "%s\\fkouji.lst", exepath);
	sprintf(bakfname, "%s\\fkouji.bak", exepath);
	CopyFile(lstfname, bakfname, FALSE);
	
	if((lstfp = fopen(lstfname, "w")) == NULL){
		return;
	}
	addflag = ON;
	if((bakfp = fopen(bakfname, "r")) != NULL){
		while(1){
			if(fgets(buf, RECMAX, bakfp) == NULL){
				break;
			}
			len = strlen(buf);
			buf[len-1] = '\0';
			if(strcmp(buf, lpKouji) == 0){
				addflag = OFF;
			}
			fprintf(lstfp, "%s\n", buf);
		}
		fclose(bakfp);
	}
	if(addflag == ON){
		fprintf(lstfp, "%s\n", lpKouji);
	}
	fclose(lstfp);
}

long CCmdFile::CheckFreeSpace(LPSTR path)
{
	ULONG	lSPC, lBPS, lFC, lTC;
	ULONG	freesize;

	::GetDiskFreeSpace(path, &lSPC, &lBPS, &lFC, &lTC);
	freesize = lBPS * lSPC * lFC;
	return(freesize);
}

void CCmdFile::ErrorMessageOut(CFileException *e)
{
	char	msg[RECMAX];

	switch(e->m_cause){
	case CFileException::fileNotFound:
		sprintf(msg, "ファイル名が正しくありません");
		::AfxMessageBox(msg, MB_OK);
		break;
	case CFileException::badPath:
		sprintf(msg, "パス名が正しくありません");
		::AfxMessageBox(msg, MB_OK);
		break;
	case CFileException::tooManyOpenFiles:
		sprintf(msg, "これ以上ファイルを開けません");
		::AfxMessageBox(msg, MB_OK);
		break;
	case CFileException::accessDenied:
		sprintf(msg, "書込み禁止状態のファイルで開けません");
		::AfxMessageBox(msg, MB_OK);
		break;
	case CFileException::directoryFull:
	case CFileException::diskFull:
		sprintf(msg, "ディスクがいっぱいです");
		::AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
		break;
	default:
		sprintf(msg, "ファイルが作成できませんでした");
		::AfxMessageBox(msg, MB_OK);
		break;
	}
}

void CCmdFile::SaveCrctFile(LPSTR lpSrcFname, LPSTR lpDstFname)
{
	FILE	*srcfp;
	FILE	*dstfp;
	char	szSrcBuf[RECMAX8+16];
	char	szDstBuf[RECMAX8+16];
	char	szSrcBaseName[FNAMEMAX];
	char	szDstBaseName[FNAMEMAX];
	char	szSrcFileName[FNAMEMAX];
	char	szDstFileName[FNAMEMAX];
	LPSTR	csvsp[RECMAX];
	int		idx, cnt;

	strcpy(szDstBaseName, lpDstFname);
	szDstBaseName[strlen(szDstBaseName)-4] = '\0';
	srcfp = fopen(lpSrcFname, "r");
	dstfp = fopen(lpDstFname, "w");
	idx = 0;
	while(1){
		if(fgets(szSrcBuf, RECMAX8, srcfp) == NULL){
			break;
		}
		cnt = basedivcsvdata(csvsp, szSrcBuf);
		strcpy(szSrcBaseName, csvsp[3]);
		szSrcBaseName[strlen(szSrcBaseName)-4] = '\0';

		sprintf(szSrcFileName, "%s.iti", szSrcBaseName);
		sprintf(szDstFileName, "%s$%03d.iti", szDstBaseName, idx);
		CopyFile(szSrcFileName, szDstFileName, FALSE);
		sprintf(szSrcFileName, "%s.bin", szSrcBaseName);
		sprintf(szDstFileName, "%s$%03d.bin", szDstBaseName, idx);
		CopyFile(szSrcFileName, szDstFileName, FALSE);
		sprintf(szSrcFileName, "%s.env", szSrcBaseName);
		sprintf(szDstFileName, "%s$%03d.env", szDstBaseName, idx);
		CopyFile(szSrcFileName, szDstFileName, FALSE);
		sprintf(szSrcFileName, "%s.wak", szSrcBaseName);
		sprintf(szDstFileName, "%s$%03d.wak", szDstBaseName, idx);
		CopyFile(szSrcFileName, szDstFileName, FALSE);

		sprintf(szDstBuf, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
			csvsp[0], csvsp[1], csvsp[2], szDstFileName, csvsp[4], csvsp[5]);
		fputs(szDstBuf, dstfp);
		idx++;
	}
	fclose(srcfp);
	fclose(dstfp);
}

void CCmdFile::DeleteCrctFile(LPSTR lpFileName)
{
	FILE	*fp;
	char	szBuf[RECMAX8+16];
	char	szBaseName[FNAMEMAX];
	char	szDelFileName[FNAMEMAX];
	LPSTR	csvsp[RECMAX];
	int		cnt;

	fp = fopen(lpFileName, "r");
	while(1){
		if(fgets(szBuf, RECMAX8, fp) == NULL){
			break;
		}
		cnt = basedivcsvdata(csvsp, szBuf);
		strcpy(szBaseName, csvsp[3]);
		szBaseName[strlen(szBaseName)-4] = '\0';
		sprintf(szDelFileName, "%s.iti", szBaseName);
		unlink(szDelFileName);
		sprintf(szDelFileName, "%s.bin", szBaseName);
		unlink(szDelFileName);
		sprintf(szDelFileName, "%s.env", szBaseName);
		unlink(szDelFileName);
		sprintf(szDelFileName, "%s.wak", szBaseName);
		unlink(szDelFileName);
	}
	fclose(fp);
	unlink(lpFileName);
}

