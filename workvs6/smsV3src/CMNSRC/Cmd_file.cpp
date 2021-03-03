
/////////////////////////////////////////////////////////////////////////////
// CCmdFile

#include "stdafx.h"
#include "math.h"
#include "ctype.h"
#include "string.h"
#include "direct.h"

#include "dlfileld.h"
#include "dlfilesv.h"
#include "cmd_base.h"
#include "cmd_file.h"
#include "ViewFDlg.h"
#include "TitlFDlg.h"
#include "filevect.h"
#include "fileeps.h"
#include "filedxf.h"
#include "filesksi.h"
#include "filearc.h"
#include "bmpfile.h"

#include "Imagekit.h"

extern "C" {
	void senvgetdatpath(LPSTR path);
	void senvgetexepath(LPSTR path);
	void scrngetmmsize(DBL *scrnxsize, DBL *scrnysize);
	void scrnsetmmsize(DBL scrnxsize, DBL scrnysize);
	DBL scrnsetxytime(DBL timex, DBL timey);
	void scrngetxytime(LPDBL timex, LPDBL timey);
	void scrnsetrltvelement();
	void scrnsetrltvtop(DBL xTop, DBL yTop);
	void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void scrnsetdisprltvmove(DBL xmove, DBL ymove);
	void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove);
	int senvsetdrawmode(int mode);
	void diblibcreatebmpfile(LPSTR bmpfname);
	void baseaddsafix(LPSTR FileName, LPSTR Safix);
	void basechangesafix(LPSTR FileName, LPSTR Safix);
	int baseiskanji(UINT far hc);
}

//static char BASED_CODE szFilter[] = "Chart Files (*.xlc)|*.xlc|Worksheet Files (*.xls)|*.xls|Data Files (*.xlc;*.xls)|*.xlc; *.xls|All Files (*.*)|*.*||";
static char BASED_CODE szFilterVect[] = "Vect Files (*.vct;*.log;*.wak)|*.vct;*.log;*.wak||";
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
	strcpy(m_szInpPath, datpath);
	strcpy(m_szExpPath, datpath);
	strcpy(m_szDatPath, datpath);
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
	char	safixs1[QRECMAX];
	char	safixs2[QRECMAX];
	char	szSrch[RECMAX];

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	strcpy(safixs1, "ess");
	strcpy(safixs2, "d98");
	sprintf(szSrch, "*.%s;*.%s", safixs1, safixs2);
	sprintf(szFilter, "%s Files (*.%s;*.%s)|*.%s;*.%s||", m_pcDoc->m_szSystemName, safixs1, safixs2, safixs1, safixs2);

	strcpy(datpath, m_szDatPath);
	SetCurrentDirectory(datpath);
	pcViewFileDialog = new CViewFileDialog(TRUE, NULL, szSrch, NULL, szFilter, m_pcView);
	pcViewFileDialog->SetTitle("ファイル読込");
	if(pcViewFileDialog->DoModal( ) == IDOK){
		CString cpExt = pcViewFileDialog->GetFileExt();
		strcpy(safixs1, cpExt.GetBuffer(QRECMAX));
		CString cpStr = pcViewFileDialog->GetPathName();
		strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
		baseaddsafix(fname, safixs1);
		GetDirName(fname, m_szDatPath);
		m_pcDoc->SetTitle(fname);
		ToupperString(safixs1);
		if(strncmp(safixs1, "ESS", 3) == 0){
			m_pcDoc->LoadEssFile(fname);
		}
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
	char	msg[RECMAX+QRECMAX];
	CString		csMsg;
	CFile	file;
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	m_pcDoc->m_nError = FALSE;
	csMsg = m_pcDoc->GetTitle();
	strcpy(safixs, "ess");
	sprintf(szSrch, "%s", csMsg.GetBuffer(FNAMEMAX));
	sprintf(szFilter, "%s Files (*.%s)|*.%s||", m_pcDoc->m_szSystemName, safixs, safixs);

	strcpy(datpath, m_szDatPath);
	SetCurrentDirectory(datpath);
	pcViewFileDialog = new CViewFileDialog(FALSE, NULL, szSrch, NULL, szFilter, m_pcView);
	pcViewFileDialog->SetTitle("ファイル保存");
	pcViewFileDialog->m_csGyousya = m_pcDoc->m_csGyousya;
	pcViewFileDialog->m_csKouji = m_pcDoc->m_csKouji;
	pcViewFileDialog->m_csComent1 = m_pcDoc->m_csComent1;
	pcViewFileDialog->m_csComent2 = m_pcDoc->m_csComent2;
	pcViewFileDialog->m_csComent3 = m_pcDoc->m_csComent3;
	ret = pcViewFileDialog->DoModal( );
	if(ret != IDOK){
		return(ENDEVENT);
	}

	CString csStr = pcViewFileDialog->GetPathName();
	strcpy(fname, csStr.GetBuffer(FNAMEMAX));
	baseaddsafix(fname, safixs);
	GetDirName(fname, m_szDatPath);

	if(file.Open(fname, CFile::modeRead, NULL)){
		file.Close();
		sprintf(msg, "%sに上書きしてもよろしいですか", fname);
		if(AfxMessageBox(msg, MB_OKCANCEL) == IDCANCEL){
			return(ENDEVENT);
		}
	}
	SaveGyousya(pcViewFileDialog->m_csGyousya.GetBuffer(RECMAX));
	SaveKouji(pcViewFileDialog->m_csKouji.GetBuffer(RECMAX));

	m_pcDoc->m_csGyousya = pcViewFileDialog->m_csGyousya;
	m_pcDoc->m_csKouji = pcViewFileDialog->m_csKouji;
	m_pcDoc->m_csComent1 = pcViewFileDialog->m_csComent1;
	m_pcDoc->m_csComent2 = pcViewFileDialog->m_csComent2;
	m_pcDoc->m_csComent3 = pcViewFileDialog->m_csComent3;

	m_pcDoc->SaveEssFile(fname);
	
	delete(pcViewFileDialog);
	senvgetexepath(exepath);
	SetCurrentDirectory(exepath);
	if(m_pcDoc->m_nError == TRUE){
		AfxMessageBox("ファイルが作成できません", MB_OK);
		unlink(fname);
	}

	DBL	xtop, ytop;
	CPoint	point;
	scrngetrltvtop(&xtop, &ytop);
	point.x = xtop;
	point.y = ytop;
	m_pcView->ScrollToPosition(point);
	return(ENDEVENT);
}

UINT CCmdFile::ExecLoadVect(UINT event, DBL dx, DBL dy)
{
	CTitlFileDialog	*pcTitlFileDialog;
	char	fname[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	safixs[QRECMAX];
	char	szSrch[RECMAX];
	CTonDoc*	pcDoc;
	CString		csMsg;

	if(event != INITEVENT){
		return(0);
	}
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	csMsg = pcDoc->GetTitle();
	strcpy(szSrch, "*.vct;*.log;*.wak");
	strcpy(datpath, m_szDatPath);
	SetCurrentDirectory(datpath);
	pcTitlFileDialog = new CTitlFileDialog(TRUE, NULL, szSrch, NULL, szFilterVect, m_pcView);
	pcTitlFileDialog->SetTitle("ファイル読込");
	if(pcTitlFileDialog->DoModal( ) == IDOK){
		CString cpExt = pcTitlFileDialog->GetFileExt();
		strcpy(safixs, cpExt.GetBuffer(QRECMAX));
		CString cpStr = pcTitlFileDialog->GetPathName();
		strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
		ToupperString(safixs);
		SaveUndo();
		if(strcmp(safixs, "VCT") == 0){
			LoadVectFile(fname);			
		}
		else if(strcmp(safixs, "LOG") == 0){
			LoadLogoFile(fname);			
		}
		else if(strcmp(safixs, "WAK") == 0){
			LoadVectFile(fname);			
		}

		if(strcmp(safixs, "VCT") != 0){
			TrnsOverScreenData();
		}
	}
	delete(pcTitlFileDialog);
	senvgetexepath(exepath);
	SetCurrentDirectory(exepath);

	return(ENDEVENT);
}

UINT CCmdFile::ExecAddLoad(UINT event, DBL dx, DBL dy)
{
	char	fname[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	srchptn[QRECMAX];
	char	safixs[QRECMAX];
	CTonDoc*	pcDoc;

	if(event == INITEVENT){
		sprintf(srchptn, "*.%s", szSafixs);
		strcpy(datpath, m_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("追加用ファイル読込");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, m_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());

			CFile file(fname, CFile::modeRead);
			CArchive ar(&file, CArchive::load);
			pcDoc->Serialize(ar);
			ar.Close();
			file.Close();

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

	if(event == INITEVENT){
		sprintf(srchptn, "*.%s", szSafixs);
		strcpy(datpath, m_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("追加用ファイル読込");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, m_szDatPath);
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());

			CFile file(fname, CFile::modeRead);
			CArchive ar(&file, CArchive::load);
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
	char	msg[RECMAX2];
	char	fname[FNAMEMAX];
	char	datpath[FNAMEMAX];
	char	exepath[FNAMEMAX];
	char	srchptn[QRECMAX];
	char	safixs[QRECMAX];
	long	filesize;

	if(event == INITEVENT){
		InitClass();
		sprintf(srchptn, "*.%s", szSafixs);
		strcpy(datpath, m_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(FALSE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("追加用ファイル保存");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, m_szDatPath);
			baseaddsafix(fname, szSafixs);

			CFile file;
			CFileException fe;

			if(file.Open(fname,  CFile::modeRead, NULL)){
				file.Close();
				sprintf(msg, "%sに上書きしてもよろしいですか", fname);
				if(AfxMessageBox(msg, MB_OKCANCEL) == IDCANCEL){
					return(ENDEVENT);
				}
			}

			senvgetexepath(exepath);
			if(file.Open(fname,  CFile::modeCreate|CFile::modeWrite, &fe) == FALSE){
				AfxThrowFileException(fe.m_cause, fe.m_lOsError);
				//AfxMessageBox("ファイルが作成できません", MB_OK);
				return(ENDEVENT);
			}
			try
			{
				CArchive ar(&file, CArchive::store | CArchive::bNoFlushOnDelete);
				ar.m_pDocument = m_pcDoc;
				ar.m_bForceFlat = FALSE;

				m_pcDoc->Serialize(ar);

				filesize = file.GetLength();
				ar.Close();
				file.Close();
				basechangesafix(fname, "env");
				SaveCrtEnv(fname);
			}
			catch (CException* e)
			{
				AfxMessageBox("ファイルが作成できません", MB_OK);
				e->Delete();
				file.Close();
				basechangesafix(fname, "e98");
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
		strcpy(datpath, m_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("追加用ファイル削除");
		pcFileDialog->SetBtnText("削除");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, m_szDatPath);
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
		strcpy(datpath, m_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("ファイル削除");
		pcFileDialog->SetBtnText("削除");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, m_szDatPath);
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
		strcpy(datpath, m_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("貯め込みファイル読込");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, m_szDatPath);
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
		strcpy(datpath, m_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(FALSE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("貯め込みファイル保存");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, m_szDatPath);
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
			CopyFile(crctfname, fname, FALSE);
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
		strcpy(datpath, m_szDatPath);
		SetCurrentDirectory(datpath);
		CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, srchptn, NULL, szFilter, m_pcView);
		pcFileDialog->SetTitle("貯め込みファイル削除");
		pcFileDialog->SetBtnText("削除");
		if(pcFileDialog->DoModal( ) == IDOK){
			CString cpExt = pcFileDialog->GetFileExt();
			strcpy(safixs, cpExt.GetBuffer(QRECMAX));
			CString cpStr = pcFileDialog->GetPathName();
			strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
			GetDirName(fname, m_szDatPath);
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

UINT CCmdFile::ExecInport(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		InitClass();
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
	char	bmpfilter[QRECMAX];
	char	emffilter[QRECMAX];
	int		filteridx;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();

	strcpy(epsfilter, "Eps Files (*.eps)|*.eps|");
	strcpy(dxffilter, "DXF Files (*.dxf)|*.dxf|");
	strcpy(bmpfilter, "BMP Files (*.bmp)|*.bmp|");
	strcpy(emffilter, "EMF Files (*.emf)|*.emf|");

	sprintf(szFilter, "%s%s|", epsfilter, dxffilter);
	//sprintf(szFilter, "%s%s%s|", epsfilter, dxffilter, bmpfilter);
	//sprintf(szFilter, "%s%s%s%s|", epsfilter, dxffilter, bmpfilter, emffilter);

	strcpy(datpath, m_szExpPath);
	SetCurrentDirectory(datpath);
	if(strcmp(m_szExpSafixs, "eps") == 0){
		strcpy(safix, "*.eps");
		filteridx = 1;
	}
	if(strcmp(m_szExpSafixs, "dxf") == 0){
		strcpy(safix, "*.dxf");
		filteridx = 2;
	}
	if(strcmp(m_szExpSafixs, "bmp") == 0){
		strcpy(safix, "*.bmp");
		filteridx = 3;
	}
	if(strcmp(m_szExpSafixs, "emf") == 0){
		strcpy(safix, "*.emf");
		filteridx = 4;
	}
	else{
		strcpy(safix, "*.eps");
		filteridx = 1;
	}
	CMyFileDialog *pcFileDialog = new CMyFileDialog(FALSE, NULL, safix, NULL, szFilter, m_pcView);
	(pcFileDialog->m_ofn).nFilterIndex = filteridx;
	pcFileDialog->SetTitle("外部ファイル保存");
	if(pcFileDialog->DoModal( ) == IDOK){
		CString cpExt = pcFileDialog->GetFileExt();
		filteridx = (pcFileDialog->m_ofn).nFilterIndex;
		CString cpStr = pcFileDialog->GetPathName();
		strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
		GetDirName(fname, m_szExpPath);
		if(filteridx == 1){
			strcpy(m_szExpSafixs, "eps");
			baseaddsafix(fname, "eps");	
		}
		else if(filteridx == 2){
			strcpy(m_szExpSafixs, "dxf");
			baseaddsafix(fname, "dxf");
		}
		else if(filteridx == 3){
			strcpy(m_szExpSafixs, "bmp");
			baseaddsafix(fname, "bmp");
		}
		else{
			strcpy(m_szExpSafixs, "emf");
			baseaddsafix(fname, "emf");
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
		else if(filteridx == 2){
			SaveBmpFile(fname);
		}
		else{
			SaveEmfFile(fname);
		}
	}
	senvgetexepath(exepath);
	SetCurrentDirectory(exepath);
	ScrnAllRedraw();
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
	char	aifilter[QRECMAX];
	char	dxffilter[QRECMAX];
	char	sksifilter[QRECMAX];
	char	arcfilter[QRECMAX];
	char	bmpfilter[QRECMAX];
	char	jpegfilter[QRECMAX];
	char	tifffilter[QRECMAX];
	int		len, i;

	strcpy(vctfilter, "Vect Files (*.vct;*.log;*.wak)|*.vct; *.log; *.wak|");
	strcpy(epsfilter, "Eps Files (*.eps)|*.eps|");
	strcpy(aifilter, "AI Files (*.ai)|*.ai|");
	strcpy(dxffilter, "DXF Files (*.dxf)|*.dxf|");
	strcpy(sksifilter, "SKSI Files (*.;*.wrk)|*.; *.wrk|");
	strcpy(arcfilter, "ARCPART Files (*.;*.ed)|*.; *.ed|");
	strcpy(bmpfilter, "Bmp Files (*.bmp)|*.bmp|");
	strcpy(jpegfilter, "JPEG Files (*.jpg)|*.jpg|");
	strcpy(tifffilter, "TIFF Files (*.tif)|*.tif|");

	strcpy(datpath, m_szInpPath);
	SetCurrentDirectory(datpath);
	if(strcmp(m_szInpSafixs, "VCT") == 0
	|| strcmp(m_szInpSafixs, "LOG") == 0
	|| strcmp(m_szInpSafixs, "WAK") == 0){
		strcpy(safix, "*.vct;*.log;*.wak");
		sprintf(szFilter, "%s%s%s%s%s%s%s%s%s|"
			, vctfilter, epsfilter, aifilter, dxffilter
			, sksifilter, arcfilter
			, bmpfilter, jpegfilter, tifffilter);
	}
	else if(strcmp(m_szInpSafixs, "EPS") == 0){
		strcpy(safix, "*.eps");
		sprintf(szFilter, "%s%s%s%s%s%s%s%s%s|"
			, epsfilter, aifilter, vctfilter, dxffilter
			, sksifilter, arcfilter
			, bmpfilter, jpegfilter, tifffilter);
	}
	else if(strcmp(m_szInpSafixs, "AI") == 0){
		strcpy(safix, "*.ai");
		sprintf(szFilter, "%s%s%s%s%s%s%s%s%s|"
			, aifilter, epsfilter, vctfilter, dxffilter
			, sksifilter, arcfilter
			, bmpfilter, jpegfilter, tifffilter);
	}
	else if(strcmp(m_szInpSafixs, "DXF") == 0){
		strcpy(safix, "*.dxf");
		sprintf(szFilter, "%s%s%s%s%s%s%s%s%s|"
			, dxffilter, epsfilter, aifilter, vctfilter
			, sksifilter, arcfilter
			, bmpfilter, jpegfilter, tifffilter);
	}
	else if(strcmp(m_szInpSafixs, "ED") == 0){
		strcpy(safix, "*.ed");
		sprintf(szFilter, "%s%s%s%s%s%s%s%s%s|"
			, arcfilter, sksifilter
			, vctfilter, epsfilter, aifilter, dxffilter
			, bmpfilter, jpegfilter, tifffilter);
	}
	else if(strcmp(m_szInpSafixs, "BMP") == 0){
		strcpy(safix, "*.bmp");
		sprintf(szFilter, "%s%s%s%s%s%s%s%s%s|"
			, bmpfilter, jpegfilter, tifffilter
			, vctfilter, epsfilter, aifilter, dxffilter
			, sksifilter, arcfilter);
	}
	else if(strcmp(m_szInpSafixs, "JPG") == 0){
		strcpy(safix, "*.jpg");
		sprintf(szFilter, "%s%s%s%s%s%s%s%s%s|"
			, jpegfilter, bmpfilter, tifffilter
			, vctfilter, epsfilter, aifilter, dxffilter
			, sksifilter, arcfilter);
	}
	else if(strcmp(m_szInpSafixs, "TIF") == 0){
		strcpy(safix, "*.tif");
		sprintf(szFilter, "%s%s%s%s%s%s%s%s%s|"
		, tifffilter, jpegfilter, bmpfilter
		, vctfilter, epsfilter, aifilter, dxffilter
		, sksifilter, arcfilter);
	}
	else if(strcmp(m_szInpSafixs, "WRK") == 0){
		strcpy(safix, "*.wrk");
		sprintf(szFilter, "%s%s%s%s%s%s%s%s%s|"
			, sksifilter, arcfilter
			, vctfilter, epsfilter, aifilter, dxffilter
			, bmpfilter, jpegfilter, tifffilter);
	}
	else{
		strcpy(safix, "*.");
		sprintf(szFilter, "%s%s%s%s%s%s%s%s%s|"
			, sksifilter, arcfilter
			, vctfilter, epsfilter, aifilter, dxffilter
			, bmpfilter, jpegfilter, tifffilter);
	}

	CMyFileDialog	*pcFileDialog = new CMyFileDialog(TRUE, NULL, safix, NULL, szFilter, m_pcView);
	pcFileDialog->SetTitle("外部ファイル読込");
	if(pcFileDialog->DoModal( ) == IDOK){
		CString cpExt = pcFileDialog->GetFileExt();
		strcpy(m_szInpSafixs, cpExt.GetBuffer(QRECMAX));
		CString cpStr = pcFileDialog->GetPathName();
		strcpy(fname, cpStr.GetBuffer(FNAMEMAX));
		GetDirName(fname, m_szInpPath);
		len = strlen(m_szInpSafixs);
		for(i = 0; i < len; i++){
			m_szInpSafixs[i] = toupper(m_szInpSafixs[i]);
		}

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
		else if(strcmp(m_szInpSafixs, "ED") == 0){
			LoadArcFile(fname);			
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
	CFileEps*	pcFileEps;
	pcFileEps = new CFileEps(m_pcView);
	pcFileEps->SaveData(fname);
	delete(pcFileEps);
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

void CCmdFile::ToupperString(LPSTR safixs)
{
	int		len;
	int		i;

	len = strlen(safixs);
	for(i = 0; i < len; i++){
		safixs[i] = toupper(safixs[i]);
	}
}

void CCmdFile::SaveBmpFile(LPSTR fname)
{
	CDialogInputNum	*pcDialogInputNum;
	int		ret;
	DBL		xmm, ymm;
	int		dpi, xdot, ydot;
	CBmpFile	*pcBmpFile;
	
	scrngetmmsize(&xmm, &ymm);
	if(xmm > ymm){
		dpi = (int)floor(1024 / (xmm / 25.4));
	}
	else{
		dpi = (int)floor(1024 / (xmm / 25.4));
	}
	pcDialogInputNum = new CDialogInputNum(m_pcView);
	pcDialogInputNum->SetTitleText("画像解像度");
	pcDialogInputNum->SetUnitText("dpi");
	pcDialogInputNum->SetDialogTitle("画像の解像度設定");
	pcDialogInputNum->m_dInputNum = dpi;
	dpi = 0;
	ret = pcDialogInputNum->DoModal();
	if(ret == IDOK){
		dpi = (int)pcDialogInputNum->m_dInputNum;
	}
	delete(pcDialogInputNum);
	if(dpi == 0){
		return;
	}
	xdot = (int)(xmm / 25.4 * dpi + 0.5);  
	ydot = (int)(ymm / 25.4 * dpi + 0.5);  
	pcBmpFile = new CBmpFile(m_pcView);
	ret = pcBmpFile->CreateBmpInit(xdot, ydot);
	if(ret == FALSE){
		pcBmpFile->CreateBmpEnd();
		delete(pcBmpFile);
		::AfxMessageBox("解像度が大きすぎて画像が作成できません", MB_OK);
		return;
	}
	pcBmpFile->CreateBmpImage(fname);
	pcBmpFile->CreateBmpEnd();
	delete(pcBmpFile);
}

void CCmdFile::SaveEmfFile(LPSTR fname)
{
	CMetaFileDC	dcMeta;
	CRect	rect;
	DBL		xmm, ymm;
	DBL		time, ttime;
	int		bmode;
	DBL		btime;
	DBL		bmvx, bmvy;

	scrngetmmsize(&xmm, &ymm);
	time = 25.4 / 720.0;
	ttime = xmm / 10000.0;
	if(time < ttime){
		time = ttime;
	}
	ttime = ymm / 10000.0;
	if(time < ttime){
		time = ttime;
	}

	rect.top = 0;
	rect.left = 0;
	rect.right = (long)(xmm / time);
	rect.bottom = (long)(ymm / time);
	dcMeta.CreateEnhanced(NULL, fname, rect, NULL);

	scrngetdisprltvmove(&bmvx, &bmvy);
	bmode = senvsetdrawmode(METAFVIEW);
	btime = scrnsetxytime(time, time);
	scrnsetrltvelement();
	scrnsetrltvtop(0, 0);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetdisprltvmove((DBL)0.0, (DBL)0.0);

	POSITION pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pData = m_pcDoc->m_pcObjDataList->GetNext(pos);
		pData->OnDraw(&dcMeta);
	}

	scrnsetxytime(btime, btime);
	senvsetdrawmode(bmode);
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetdisprltvmove(bmvx, bmvy);

	::DeleteEnhMetaFile(dcMeta.CloseEnhanced());
}
