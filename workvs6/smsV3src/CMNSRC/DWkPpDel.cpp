// DWkPpDel.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DWkPpDel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWkPaperDel �_�C�A���O
extern "C" {
void senvgetwakpath(LPSTR wakpath);
int baseiskanji(UINT ch);
LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR pname);
void wakuenvinit(LPSTR envfile);
void wakuenvsetpaperelement();
void wakuenvgetpaperelement();
void wakuenvend();
void wakuenvsetpapername(LPSTR name);
void wakuenvgetpapername(LPSTR name);
void wakuenvsetpaperpath(LPSTR path);
void wakuenvgetpaperpath(LPSTR path);
void wakuenvgetfullpaperpath(LPSTR path);
void wakuenvdelpaper(LPSTR delpapername);
}

CDialogWkPaperDel::CDialogWkPaperDel(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWkPaperDel::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	//{{AFX_DATA_INIT(CDialogWkPaperDel)
	m_csPaperPath = _T("");
	m_csPaperName = _T("");
	//}}AFX_DATA_INIT
}


void CDialogWkPaperDel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWkPaperDel)
	DDX_Text(pDX, IDC_PATH, m_csPaperPath);
	DDX_CBString(pDX, IDC_PAPERNAME, m_csPaperName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWkPaperDel, CDialog)
	//{{AFX_MSG_MAP(CDialogWkPaperDel)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_CBN_SELCHANGE(IDC_PAPERNAME, OnSelchangePapername)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWkPaperDel ���b�Z�[�W �n���h��

BOOL CDialogWkPaperDel::OnInitDialog() 
{
	CRect	rect;
	int		sx, sy, sizex, sizey;
	char	papername[RECMAX];
	char	paperpath[RECMAX];

	CDialog::OnInitDialog();

	GetWindowRect(&rect);
	sx = rect.left;
	sy = rect.top;
	sizex = rect.Width();
	sizey = rect.Height();
	::SetWindowPos(m_hWnd, HWND_TOPMOST, sx, sy, sizex, sizey, 0);

	CBInitPaperName();
	wakuenvgetpaperelement();
	wakuenvgetpapername(papername);
	m_csPaperName = _T(papername);
	wakuenvgetfullpaperpath(paperpath);
	m_csPaperPath = _T(paperpath);
	UpdateData(FALSE);
	
	return TRUE;
}

void CDialogWkPaperDel::CBInitPaperName()
{
	char	wakpath[FNAMEMAX];
	char	paperlistfname[FNAMEMAX];
	char	buf[RECMAX];
	char	*sp;
	char	pname[HRECMAX];
	FILE	*fp;

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PAPERNAME);
	senvgetwakpath(wakpath);
	sprintf(paperlistfname, "%s\\paperlst.lst", wakpath);
	if((fp = fopen(paperlistfname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, pname);
		pCB->AddString(pname);
	}
	fclose(fp);
}

void CDialogWkPaperDel::OnSelchangePapername() 
{
	char	buf[RECMAX];
	int		idx;
	char	pname[RECMAX];
	char	paperpath[RECMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PAPERNAME);
	idx = pCB->GetCurSel();
	pCB->GetLBText(idx, pname);
	UpdateData(TRUE);
	wakuenvsetpapername(pname);
	wakuenvgetpaperelement();
	wakuenvgetpapername(buf);
	m_csPaperName = _T(buf);
	wakuenvgetfullpaperpath(paperpath);
	m_csPaperPath = _T(paperpath);
	UpdateData(FALSE);
}

void CDialogWkPaperDel::OnDelete() 
{
	char	papername[RECMAX];
	char	buf[RECMAX];
	int		ret;

	UpdateData(TRUE);
	strcpy(papername, m_csPaperName.GetBuffer(RECMAX));
	sprintf(buf, "[%s]���폜���Ă�낵���ł���\n�폜���s���Ƙg���[%s]�̃��C�A�E�g�͂��ׂč폜����܂�", papername, papername);
	ret = MessageBox(buf, "�g��ލ폜", MB_OKCANCEL);
	if(ret == IDOK){
		wakuenvdelpaper(papername);
	}
	CDialog::OnOK();
}

