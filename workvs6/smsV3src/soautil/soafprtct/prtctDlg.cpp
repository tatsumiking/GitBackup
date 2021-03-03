// prtctDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <io.h>
#include "prtct.h"
#include "prtctDlg.h"
#include "WINIOCTL.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	DEFSTR	"KEYSTRINGSOA"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// ���b�Z�[�W �n���h��������܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrtctDlg �_�C�A���O

CPrtctDlg::CPrtctDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrtctDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrtctDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPrtctDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrtctDlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPrtctDlg, CDialog)
	//{{AFX_MSG_MAP(CPrtctDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNPRTCTSET, OnBtnprtctset)
	ON_BN_CLICKED(IDC_BTNPRTCTRESET, OnBtnprtctreset)
	ON_BN_CLICKED(IDC_BTNSET, OnBtnset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrtctDlg ���b�Z�[�W �n���h��

BOOL CPrtctDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	strcpy(m_szSetLabel, "��������");
	strcpy(m_szDelLabel, "��������");

	InitExePath(m_szExePath);
	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B
	
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

void CPrtctDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CPrtctDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CPrtctDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPrtctDlg::OnBtnprtctset() 
{
	char	fname[FNAMEMAX];
	char	szWinDir[FNAMEMAX];
	char	buf[RECMAX+16];
	FILE	*fp;

	SetDriveNo();
	ReadLabel(buf);
	if(strcmp(DEFSTR, buf) == 0){
		//ObjectCopy();
		memset(buf, '\0', RECMAX);
		WriteLabel(m_szDelLabel);
		::GetSystemDirectory(szWinDir, FNAMEMAX);
		sprintf(fname, "%s\\winsleep.sys", szWinDir);
		fp = fopen(fname, "wb");
		memset(buf, '\0', 32);
		fwrite(buf, 1, 158, fp);
		fclose(fp);
	}
	else{
		MessageBox("���̃t���b�s�[�͂��g�p�ɂȂ�܂���", "�m�F", MB_OK);
	}
	CDialog::OnOK();
}

void CPrtctDlg::OnBtnprtctreset() 
{
	char	fname[FNAMEMAX];
	char	szWinDir[FNAMEMAX];
	char	buf[RECMAX+16];
	FILE	*fp;

	SetDriveNo();
	::GetSystemDirectory(szWinDir, FNAMEMAX);
	sprintf(fname, "%s\\winsleep.sys", szWinDir);
	fp = fopen(fname, "r");
	if(fp != NULL){
		fclose(fp);
		unlink(fname);
		memset(buf, '\0', RECMAX);
		strcpy(buf, DEFSTR); 
		WriteLabel(m_szSetLabel);
	}
	else{
		MessageBox("�L�[��߂����Ƃ��o���܂���", "�m�F", MB_OK);
	}
	CDialog::OnOK();
}

void CPrtctDlg::OnBtnset() 
{
	char	buf[RECMAX+16];

	SetDriveNo();
	memset(buf, '\0', RECMAX);
	strcpy(buf, DEFSTR); 
	WriteLabel(m_szSetLabel);
}

void CPrtctDlg::InitExePath(LPSTR lpExePath)
{
	int		len;

	len = ::GetModuleFileName(AfxGetInstanceHandle( ), lpExePath, FNAMEMAX);
	while(1){
		if(len <= 0){
			lpExePath[0] = '\0';
			break;
		}
		if(lpExePath[len] == ':' || lpExePath[len] == '\\'){
			lpExePath[len] = '\0';
			break;
		}
		len--;
	}
}

void CPrtctDlg::ReadLabel(LPSTR buf)
{
	char	szlabel[129];
	char	szsysname[129];
	DWORD	nmaxlen;
	DWORD	nvolno;
	DWORD	flag;

	memset(buf, '\0', RECMAX);
	GetVolumeInformation(m_szDriveName, szlabel, sizeof(szlabel)-1, &nvolno, &nmaxlen, &flag, szsysname, sizeof(szsysname)-1);
	if(strcmp(szlabel, m_szSetLabel) == 0){
		strcpy(buf, DEFSTR);
	}
}

void CPrtctDlg::WriteLabel(LPSTR buf)
{
	int		ret;

	ret = SetVolumeLabel(m_szDriveName, buf);
}

void CPrtctDlg::SetDriveNo()
{
	int		ret;
	int		no;
	char	fname[FNAMEMAX];

	m_nDriveNo = 0;
	strcpy(m_szDriveName,"A:\\");
	for(no = 1; no < 26; no++){
		/*
		ret = GetDriveType(m_szDriveName);
		if(ret == DRIVE_REMOVABLE){
			m_nDriveNo = no;
			break;
		}
		*/
		sprintf(fname, "%sprtct.exe", m_szDriveName);
		if(_access(fname, 0) == 0){
			break;
		}
		m_szDriveName[0]++;
	}
}

void CPrtctDlg::ObjectCopy()
{
}
