// wr32Dlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "wr32.h"
#include "wr32Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWr32Dlg �_�C�A���O

CWr32Dlg::CWr32Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWr32Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWr32Dlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nFirstFlag = ON;
}

void CWr32Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWr32Dlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWr32Dlg, CDialog)
	//{{AFX_MSG_MAP(CWr32Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWr32Dlg ���b�Z�[�W �n���h��

BOOL CWr32Dlg::OnInitDialog()
{
	CWr32App*	theApp;

	CDialog::OnInitDialog();

	theApp = (CWr32App*)AfxGetApp();
	InitExePath(m_szExePath);


	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B
	
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CWr32Dlg::OnPaint() 
{
	BOOL	ret;

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

	if(m_nFirstFlag == ON){
		m_nFirstFlag = OFF;
		ret = ConvertWroutRec();
		if(ret == TRUE){
			ProgramExec();
		}
		CDialog::OnOK();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CWr32Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWr32Dlg::InitExePath(LPSTR lpExePath)
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

BOOL CWr32Dlg::ConvertWroutRec()
{
	char	envfname[FNAMEMAX];
	char	recfname[FNAMEMAX];
	char	crctfname[FNAMEMAX];
	FILE	*envfp;
	FILE	*recfp;
	FILE	*crctfp;
	char	buf[RECMAX];
	char	wakunum[QRECMAX];
	int		pos;
	char	recbuf[RECMAX4];
	char	head[QRECMAX];
	int		len;

	LoadSeting();
	if(m_szWakuPath[0] == '\0'){
		return(FALSE);
	}

	strcpy(recfname, "wrout.rec");

	sprintf(envfname, "%s\\wr32.env", m_szExePath);
	envfp = fopen(envfname, "r");
	if(envfp == NULL){
		return(FALSE);
	}
	fgets(buf, RECMAX, envfp);
	sscanf(buf, "%s", recfname);
	fclose(envfp);

	sprintf(crctfname, "%s\\outcrect.dat", m_szExePath);
	crctfp = fopen(crctfname, "w");
	if(crctfp == NULL){
		return(FALSE);
	}

	recfp = fopen(recfname, "r");
	if(recfp == NULL){
		fclose(crctfp);
		return(FALSE);
	}
	while(1){
		if(fgets(buf, RECMAX, recfp) == NULL){
			break;
		}
		strncpy(wakunum, buf, 5);
		wakunum[5] = '\0';
		strcpy(recbuf, &(buf[5]));
		len = strlen(recbuf);
		recbuf[len-1] = '\0';
		CreateHead(wakunum, head);
		if(head[0] != '\0'){
			fprintf(crctfp, "%s,\"%s\"\n", head, recbuf);
		}
	}
	fclose(crctfp);
	fclose(recfp);
	return(TRUE);
}

void CWr32Dlg::ProgramExec()
{
	char	szprog[RECMAX];
	char	szExecProg[RECMAX];

	GetProgranName(1, szprog);
	if(szprog[1] == ':' || szprog[1] == '.'){
		strcpy(szExecProg, szprog);		
	}
	else{
		sprintf(szExecProg, "%s\\%s", m_szExePath, szprog);
	}
	::WinExec(szExecProg, SW_SHOW);
}

void CWr32Dlg::GetProgranName(int no, char *lpprog)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		i, len;

	*lpprog = '\0';
	switch(no){
	case 0:
		strcpy(lpprog, "sms.exe");
		break;
	case 1:
		strcpy(lpprog, "smsout.exe");
		break;
	}

	sprintf(fname, "%s\\prntprog.env", m_szExePath);
	if((fp = fopen(fname, "r")) != NULL){
		for(i = 0; i < no; i++){
			fgets(lpprog, RECMAX, fp);
		}
		fgets(lpprog, RECMAX, fp);
		len = strlen(lpprog);
		lpprog[len-1] = '\0';
		fclose(fp);
	}
}

void CWr32Dlg::CreateHead(LPSTR wakunum, LPSTR head)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	waku[FNAMEMAX];

	SrchWakuName(wakunum, waku);
	if(waku[0] == '\0'){
		head[0] == '\0';
		return;
	}
	sprintf(head, "\"��\",\"%s\",\"%s\",\"%s\",\"1\""
		, m_szGrup, m_szPaper, waku);
}

void CWr32Dlg::LoadSeting()
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	*csvsp[10];
	char	szDmy1[FNAMEMAX];
	char	szDmy2[FNAMEMAX];
	char	szDmy3[FNAMEMAX];

	strcpy(m_szGrup, "�\�A");
	strcpy(m_szPaper, "");
	strcpy(m_szWakuPath, "");
	sprintf(fname, "%s\\soainfo.env", m_szExePath);
	fp = fopen(fname, "r");
	if(fp == NULL){
		return;
	}
	fgets(buf, RECMAX, fp);	//��Ж�
	fgets(buf, RECMAX, fp);	//���̈ʒu
	fgets(buf, RECMAX, fp);	//�O���[�v��
	DivCsvData(csvsp, buf);
	strcpy(m_szGrup, csvsp[0]);
	//sscanf(buf, "\"%s\"", m_szGrup); 
	fgets(buf, RECMAX, fp);	//�g�ʒu�i�O���[�v���j
	fgets(buf, RECMAX, fp);	//�W������
	fgets(buf, RECMAX, fp);	//�O���v���O������
	fgets(buf, RECMAX, fp);	//�g�쐬�v���O������
	fgets(buf, RECMAX, fp);	//�W���o�͋@
	fgets(buf, RECMAX, fp);	//�p����
	DivCsvData(csvsp, buf);
	strcpy(m_szPaper, csvsp[0]);
	//sscanf(buf, "\"%s\"", m_szPaper); 
	fgets(buf, RECMAX, fp);	//�c�����̉�]�p�x
	fgets(buf, RECMAX, fp);	//�W���o�͋@
	fgets(buf, RECMAX, fp);	//
	fgets(buf, RECMAX, fp);	//
	fgets(buf, RECMAX, fp);	//
	fgets(buf, RECMAX, fp);	//
	fgets(buf, RECMAX, fp);	//
	fgets(buf, RECMAX, fp);	//��ʕ\���{��
	fclose(fp);

	sprintf(fname, "%s\\waku.env", m_szExePath);
	fp = fopen(fname, "r");
	if(fp == NULL){
		return;
	}
	fgets(buf, RECMAX, fp);	//�p����
	sscanf(buf, "%s", m_szPaper);
	fclose(fp);

	sprintf(fname, "%s\\ses_sys.env", m_szExePath);
	fp = fopen(fname, "r");
	if(fp == NULL){
		return;
	}
	fgets(buf, RECMAX, fp);	//��Ж�
	fgets(buf, RECMAX, fp);	//��}�̈�
	fgets(buf, RECMAX, fp);	//�o�͕���
	fgets(buf, RECMAX, fp);	//�|�[�g
	fgets(buf, RECMAX, fp);	//�o�͋@�ݒ�
	fgets(buf, RECMAX, fp);	//�o�͋@�t���O
	fgets(buf, RECMAX, fp);	//�H�H�H
	fgets(buf, RECMAX, fp);	//���̈ʒu
	fgets(buf, RECMAX, fp);	//�f�[�^�ۑ��ʒu
	fgets(buf, RECMAX, fp);	//���ߍ��݈ʒu
	fgets(buf, RECMAX, fp);	//�Ɩ�ʒu
	fgets(buf, RECMAX, fp);	//�g�ʒu
	fgets(buf, RECMAX, fp);	//���i�ʒu
	fgets(buf, RECMAX, fp);	//��ƈʒu
	fgets(buf, RECMAX, fp);	//�a�l�o��ƈʒu
	fclose(fp);
}

void CWr32Dlg::SrchWakuName(LPSTR wakunum, LPSTR waku)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	*csvsp[20];
	char	wakudir[FNAMEMAX];
	int		len;

	waku[0] = '\0';
	sprintf(fname, "%s\\paperlst.lst", m_szWakuPath);
	fp = fopen(fname, "r");
	if(fp == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		DivCsvData(csvsp, buf);
		if(strcmp(m_szPaper, csvsp[0]) == 0){
			strcpy(wakudir, csvsp[9]);
			break;
		}
	}
	fclose(fp);

	sprintf(fname, "%s\\%s\\wakuname.lst", m_szWakuPath, wakudir);
	fp = fopen(fname, "r");
	if(fp == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		DivCsvData(csvsp, buf);
		len = strlen(csvsp[0]);
		if(strncmp(wakunum, &(csvsp[0][len-9]), 5) == 0){
			sprintf(waku, "%s\\%s\\%s", m_szWakuPath, wakudir, csvsp[0]);
			break;
		}
	}
	fclose(fp);
	
}

int CWr32Dlg::DivCsvData(LPSTR csvsp[], LPSTR csvbuf)
{
	int		i;
	UINT	hc;
	LPSTR	sp;
	int		nDoubleQ;

	sp = csvbuf;
	i = 0;
	while(1){
		while(*sp){
			hc = *sp;
			if(!isspace(hc)){
				break;
			}
			sp++;
		}

		if(*sp == '\0'){
			break;
		}

		if(*sp == '\"'){
			nDoubleQ = TRUE;
			sp++;
		}
		else{
			nDoubleQ = FALSE;
		}

		csvsp[i] = sp;
		while(*sp){
			hc = *sp;
			if(IsKanji(hc) == ON){
				sp += 2;
			}
			else{
				if(nDoubleQ == TRUE){
					if(hc == '\"'){
						*sp = '\0';
						sp++;
						while(*sp){
							hc = *sp;
							if(IsKanji(hc) == ON){
								sp += 2;
							}
							else{
								if(hc == ','){
									*sp = '\0';
									sp++;
									break;
								}
								sp++;
							}
						}
						break;
					}
				}
				else if(hc == ','){
					*sp = '\0';
					sp++;
					break;
				}
				sp++;
			}
		}
		i++;
	}
	return(i);
}

int CWr32Dlg::IsKanji(int hc)
{
	hc = hc & 0x00ff;
	if((0x0080 <= hc && hc <= 0x009f)
	|| (0x00e0 <= hc && hc <= 0x00ff)){
		return(ON);
	}
	else{
		return(OFF);
	}
}


