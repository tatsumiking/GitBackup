// DlCrctDsp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogCrctDisp.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogCrctDisp ダイアログ

CDialogCrctDisp::CDialogCrctDisp(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCrctDisp::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	m_pcDoc = m_pcView->GetDocument();
	//{{AFX_DATA_INIT(CDialogCrctDisp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}
void CDialogCrctDisp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogCrctDisp)
	DDX_Control(pDX, IDC_LST_CRCT, m_lstCrct);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CDialogCrctDisp, CDialog)
	//{{AFX_MSG_MAP(CDialogCrctDisp)
	ON_BN_CLICKED(IDC_REC_DEL, OnRecDel)
	ON_BN_CLICKED(IDC_REC_UP, OnRecUp)
	ON_BN_CLICKED(IDC_REC_DOWN, OnRecDown)
	ON_LBN_SELCHANGE(IDC_LST_CRCT, OnSelchangeLstCrct)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CDialogCrctDisp メッセージ ハンドラ
BOOL CDialogCrctDisp::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitLstCrct();

	return TRUE;
}
void CDialogCrctDisp::InitLstCrct()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	crctfname[FNAMEMAX];
	FILE	*crctfp;
	TCHAR	csvbuf[RECMAX8];
	int 	len;
	LPTSTR	csvsp[RECMAX];
	TCHAR	buf[RECMAX];

	SetWindowText(m_pcDoc->m_szCrctFileName);
	senvgetexepath(exepath);
	_stprintf_s(crctfname, _T("%s\\outcrect.dat"), exepath);
	_tfopen_s(&crctfp, crctfname, _T("r+,ccs=UNICODE"));
	if(crctfp == NULL){
		return;
	}
	m_lstCrct.ResetContent();
	m_lstCrct.SetHorizontalExtent(1000);
	while(1){
		if(_fgetts(csvbuf, RECMAX8, crctfp) == NULL){
			break;
		}
		len = _tcslen(csvbuf);
		csvbuf[len-1] = _T('\0');
		basedivcsvdata(csvsp, csvbuf);
		_stprintf_s(buf, _T("%s %s"), csvsp[2], csvsp[5]);
		m_lstCrct.AddString(buf);
	}
	fclose(crctfp);
	m_nSlctIdx = -1;
}
void CDialogCrctDisp::OnRecDel() 
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	backfname[FNAMEMAX];
	TCHAR	crctfname[FNAMEMAX];
	FILE	*backfp;
	FILE	*crctfp;
	TCHAR	szSrcBuf[RECMAX8+16];
	TCHAR	szDstBuf[RECMAX8+16];
	int		idx, len, cnt;
	LPTSTR	csvsp[RECMAX];
	TCHAR	szBaseName[FNAMEMAX];
	TCHAR	szSrcBaseName[FNAMEMAX];
	TCHAR	szDstBaseName[FNAMEMAX];

	if(m_nSlctIdx == -1){
		return;
	}
	senvgetexepath(exepath);
	_stprintf_s(backfname, _T("%s\\outcrect.bak"), exepath);
	_stprintf_s(crctfname, _T("%s\\outcrect.dat"), exepath);
	CopyFile(crctfname, backfname, FALSE);
	_tfopen_s(&backfp, backfname, _T("r,ccs=UNICODE"));
	_tfopen_s(&crctfp, crctfname, _T("w,ccs=UNICODE"));
	idx = 0;
	while(1){
		if(_fgetts(szSrcBuf, RECMAX8, backfp) == NULL){
			break;
		}
		len = _tcslen(szSrcBuf);
		szSrcBuf[len-1] = _T('\0');
		if(idx == m_nSlctIdx){
			break;
		}
		_fputts(szSrcBuf, crctfp);
		idx++;
	}
	while(1){
		if(_fgetts(szSrcBuf, RECMAX8, backfp) == NULL){
			break;
		}
		len = _tcslen(szSrcBuf);
		szSrcBuf[len-1] = _T('\0');
		cnt = basedivcsvdata(csvsp, szSrcBuf);
		_tcscpy_s(szBaseName, csvsp[3]);
		szBaseName[_tcslen(szBaseName)-8] = _T('\0');
		_stprintf_s(szSrcBaseName, _T("%s$%03d"), szBaseName, idx+1);
		_stprintf_s(szDstBaseName, _T("%s$%03d"), szBaseName, idx);
		m_pcDoc->CopyWakuFileBlock(szSrcBaseName, szDstBaseName);
		_stprintf_s(szDstBuf, _T("\"%s\",\"%s\",\"%s\",\"%s.wak\",\"%s\",\"%s\"\n"),
			csvsp[0], csvsp[1], csvsp[2], szDstBaseName, csvsp[4], csvsp[5]);
		_fputts(szDstBuf, crctfp);
	}
	fclose(backfp);
	fclose(crctfp);
	InitLstCrct();
}
void CDialogCrctDisp::OnRecUp() 
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	backfname[FNAMEMAX];
	TCHAR	crctfname[FNAMEMAX];
	FILE	*backfp;
	FILE	*crctfp;
	TCHAR	szBuf1[RECMAX8+16];
	TCHAR	szBuf2[RECMAX8+16];
	TCHAR	szDstBuf[RECMAX8+16];
	int		idx, len, cnt;
	LPTSTR	csvsp1[RECMAX];
	LPTSTR	csvsp2[RECMAX];
	TCHAR	szBaseName[FNAMEMAX];
	TCHAR	szBaseName1[FNAMEMAX];
	TCHAR	szBaseName2[FNAMEMAX];
	TCHAR	szTmpBaseName[FNAMEMAX];
	TCHAR	szDstBaseName[FNAMEMAX];

	if(m_nSlctIdx == -1 || m_nSlctIdx == 0){
		return;
	}
	senvgetexepath(exepath);
	senvgettmppath(tmppath);
	_stprintf_s(backfname, _T("%s\\outcrect.bak"), exepath);
	_stprintf_s(crctfname, _T("%s\\outcrect.dat"), exepath);
	CopyFile(crctfname, backfname, FALSE);
	_tfopen_s(&backfp, backfname, _T("r,ccs=UNICODE"));
	_tfopen_s(&crctfp, crctfname, _T("w,ccs=UNICODE"));
	idx = 0;
	while(1){
		if(_fgetts(szBuf1, RECMAX8, backfp) == NULL){
			break;
		}
		len = _tcslen(szBuf1);
		szBuf1[len-1] = _T('\0');
		if(idx == m_nSlctIdx-1){
			break;
		}
		_fputts(szBuf1, crctfp);
		idx++;
	}
	if(_fgetts(szBuf2, RECMAX8, backfp) == NULL){
		_fputts(szBuf1, crctfp);
		fclose(backfp);
		fclose(crctfp);
		return;
	}
	len = _tcslen(szBuf2);
	szBuf2[len-1] = _T('\0');
	cnt = basedivcsvdata(csvsp1, szBuf1);
	cnt = basedivcsvdata(csvsp2, szBuf2);

	_tcscpy_s(szBaseName1, csvsp1[3]);
	szBaseName1[_tcslen(szBaseName1)-4] = _T('\0'); // .wakを削除
	_stprintf_s(szTmpBaseName, _T("%s$tmp"), tmppath);
	m_pcDoc->CopyWakuFileBlock(szBaseName1, szTmpBaseName);

	_tcscpy_s(szBaseName2, csvsp2[3]);
	szBaseName2[_tcslen(szBaseName2)-4] = _T('\0'); // .wakを削除
	_tcscpy_s(szBaseName, szBaseName2);
	szBaseName[_tcslen(szBaseName)-4] = _T('\0'); // $xxxを削除
	_stprintf_s(szDstBaseName, _T("%s$%03d"), szBaseName, idx-1);
	m_pcDoc->CopyWakuFileBlock(szBaseName2, szDstBaseName);
	_tcscpy_s(szBaseName2, szDstBaseName);

	_tcscpy_s(szBaseName, csvsp1[3]);
	szBaseName[_tcslen(szBaseName)-8] = _T('\0'); // $xxx.wakを削除
	_stprintf_s(szBaseName1, _T("%s$%03d"), szBaseName, idx);
	m_pcDoc->CopyWakuFileBlock(szTmpBaseName, szBaseName1);

	_stprintf_s(szDstBuf, _T("\"%s\",\"%s\",\"%s\",\"%s.wak\",\"%s\",\"%s\"\n"),
			csvsp2[0], csvsp2[1], csvsp2[2], szBaseName2, csvsp2[4], csvsp2[5]);
	_fputts(szDstBuf, crctfp);
	_stprintf_s(szDstBuf, _T("\"%s\",\"%s\",\"%s\",\"%s.wak\",\"%s\",\"%s\"\n"),
			csvsp1[0], csvsp1[1], csvsp1[2], szBaseName1, csvsp1[4], csvsp1[5]);
	_fputts(szDstBuf, crctfp);

	while(1){
		if(_fgetts(szBuf1, RECMAX8, backfp) == NULL){
			break;
		}
		len = _tcslen(szBuf1);
		szBuf1[len-1] = _T('\0');
		_fputts(szBuf1, crctfp);
	}
	fclose(backfp);
	fclose(crctfp);
	InitLstCrct();
}
void CDialogCrctDisp::OnRecDown() 
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	backfname[FNAMEMAX];
	TCHAR	crctfname[FNAMEMAX];
	FILE	*backfp;
	FILE	*crctfp;
	TCHAR	szBuf1[RECMAX8+16];
	TCHAR	szBuf2[RECMAX8+16];
	TCHAR	szDstBuf[RECMAX8+16];
	int		len, idx, cnt;
	LPTSTR	csvsp1[RECMAX];
	LPTSTR	csvsp2[RECMAX];
	TCHAR	szBaseName[FNAMEMAX];
	TCHAR	szBaseName1[FNAMEMAX];
	TCHAR	szBaseName2[FNAMEMAX];
	TCHAR	szTmpBaseName[FNAMEMAX];
	TCHAR	szDstBaseName[FNAMEMAX];

	if(m_nSlctIdx == -1){
		return;
	}
	senvgetexepath(exepath);
	senvgettmppath(tmppath);
	_stprintf_s(backfname, _T("%s\\outcrect.bak"), exepath);
	_stprintf_s(crctfname, _T("%s\\outcrect.dat"), exepath);
	CopyFile(crctfname, backfname, FALSE);
	_tfopen_s(&backfp, backfname, _T("r,ccs=UNICODE"));
	_tfopen_s(&crctfp, crctfname, _T("w,ccs=UNICODE"));
	idx = 0;
	while(1){
		if(_fgetts(szBuf1, RECMAX8, backfp) == NULL){
			break;
		}
		len = _tcslen(szBuf1);
		szBuf1[len-1] = _T('\0');
		if(idx == m_nSlctIdx){
			break;
		}
		_fputts(szBuf1, crctfp);
		idx++;
	}
	_fgetts(szBuf2, RECMAX8, backfp);
	len = _tcslen(szBuf2);
	szBuf2[len-1] = _T('\0');
	
	cnt = basedivcsvdata(csvsp1, szBuf1);
	cnt = basedivcsvdata(csvsp2, szBuf2);

	_tcscpy_s(szBaseName1, csvsp1[3]);
	szBaseName1[_tcslen(szBaseName1)-4] = _T('\0'); // .wakを削除
	_stprintf_s(szTmpBaseName, _T("%s$tmp"), tmppath);
	m_pcDoc->CopyWakuFileBlock(szBaseName1, szTmpBaseName);

	_tcscpy_s(szBaseName2, csvsp2[3]);
	szBaseName2[_tcslen(szBaseName2)-4] = _T('\0'); // .wakを削除
	_tcscpy_s(szBaseName, szBaseName2);
	szBaseName[_tcslen(szBaseName)-4] = _T('\0'); // $xxxを削除
	_stprintf_s(szDstBaseName, _T("%s$%03d"), szBaseName, idx);
	m_pcDoc->CopyWakuFileBlock(szBaseName2, szDstBaseName);
	_tcscpy_s(szBaseName2, szDstBaseName);

	_tcscpy_s(szBaseName, csvsp1[3]);
	szBaseName[_tcslen(szBaseName)-8] = _T('\0'); // $xxx.wakを削除
	_stprintf_s(szBaseName1, _T("%s$%03d"), szBaseName, idx+1);
	m_pcDoc->CopyWakuFileBlock(szTmpBaseName, szBaseName1);

	_stprintf_s(szDstBuf, _T("\"%s\",\"%s\",\"%s\",\"%s.wak\",\"%s\",\"%s\"\n"),
			csvsp2[0], csvsp2[1], csvsp2[2], szBaseName2, csvsp2[4], csvsp2[5]);
	_fputts(szDstBuf, crctfp);
	_stprintf_s(szDstBuf, _T("\"%s\",\"%s\",\"%s\",\"%s.wak\",\"%s\",\"%s\"\n"),
			csvsp1[0], csvsp1[1], csvsp1[2], szBaseName1, csvsp1[4], csvsp1[5]);
	_fputts(szDstBuf, crctfp);

	while(1){
		if(_fgetts(szBuf1, RECMAX8, backfp) == NULL){
			break;
		}
		len = _tcslen(szBuf1);
		szBuf1[len-1] = _T('\0');
		_fputts(szBuf1, crctfp);
	}
	fclose(backfp);
	fclose(crctfp);
	InitLstCrct();
}
void CDialogCrctDisp::OnSelchangeLstCrct() 
{
	m_nSlctIdx = m_lstCrct.GetCurSel();
}
