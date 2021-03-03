#ifndef __CMDPROUT_H
#define __CMDPROUT_H

#define	BMPDIVMAX	10

class CCmdPrint : public CCmdBase
{
public:
	CCmdPrint(CScrollView* pcview);
	~CCmdPrint();
public:
	void CreateCrcl(CVect *pcVect, DBL sx, DBL sy, DBL ex, DBL ey);
	void CreateMark(DBL xsize, DBL ysize);
	BOOL SaveEnvData(LPSTR fname);
	BOOL SaveXXXData(LPSTR fname);
	BOOL SaveEssData(LPSTR fname);
	BOOL SetDialogElement();
	void FileDataOut(LPBYTE m_lpOutLine, int idx);
	void TrnsDXYMMToStep(LPDBL x, LPDBL y);
	void PlotOutVect(int nComID, CVect* pcVect);
	void PlotOutData(int nComID);
	void CutingDataOut(int cpycnt);
	UINT ExecPrintOut(UINT event, DBL dx, DBL dy);
	UINT ExecSendData(UINT event, DBL dx, DBL dy);
	UINT ExecStopData(UINT event, DBL dx, DBL dy);
	UINT ExecOutData(UINT event, DBL dx, DBL dy);
private:
	void CopyPrintOut();
	void DivPrintOut();
	int OutBlockData(DBL dotsx, DBL dotsy, DBL dotex, DBL dotey, int xcpycnt);
	void MemDCInit(int xdot, int ydot);
	void MemDCEnd();
	void AreaPaintData(DBL sx, DBL sy, DBL ex, DBL ey);
	int OutPaintBlock(DBL sx, DBL sy, DBL ex, DBL ey, int cpycnt);
	void BmpFileOut(int ysize, int i);

private:
	// �ȉ��_�C�A���O�Őݒ肷����e
	char	m_szSaverPath[FNAMEMAX];
	char	m_szSendPath[FNAMEMAX];
	CDataList	*m_pcDataList;
	int	m_nCmdKind;				// �o�͋@���
	char	m_szPaperName[RECMAX];
	int	m_nXdpi, m_nYdpi;		// �o�͋@�̉𑜓x
	DBL	m_dStep;
	BOOL m_bChkX, m_bChkY;		// �c���Œ�t���O
	DBL	m_dXMMOut, m_dYMMOut;	// �o�̓f�[�^�[�T�C�Y
	DBL	m_dXMMPaper, m_dYMMPaper;	// �o�͗p���T�C�Y
	DBL	m_dXMMNori, m_dYMMNori;	// �̂肵��T�C�Y
	DBL	m_dSendTime;	// �p������␳�{��
	int	m_nCopyCount;	// ���t�J�E���g
	DBL m_dRgnum, m_dGgnum,	m_dBgnum;
	DBL m_dRadd, m_dGadd, m_dBadd;
	DBL m_dRcntrst, m_dGcntrst, m_dBcntrst;
	int	m_nProcess;
	DBL	m_dPlotXTime, m_dPlotYTime;
	DBL	m_dPlotAddX, m_dPlotAddY;
	int	m_nOutFlag;
	int	m_bChkMark;
	int	m_nPrintCut;
	DBL	m_dMarkAddX, m_dMarkAddY;
	DBL	m_dMarkDot;

private:
	DBL	m_dXDotNori, m_dYDotNori;	// �̂肵��h�b�g�T�C�Y
	DBL	m_dXDotOut, m_dYDotOut;	// �o�̓h�b�g�f�[�^�[�T�C�Y
	DBL m_dXTime, m_dYTime;			// �f�[�^�ϊ��{��
	DBL m_dXDotMax, m_dYDotMax;		// �S�̃f�[�^�T�C�Y
	DBL m_dXBlockSize, m_dYBlockSize;	//�@�ꖇ�̃f�[�^�T�C�Y
	int	m_nBmpDivMax;
	int	m_nCrtDotY;
	int	m_nBMPXSize[BMPDIVMAX];
	int	m_nBMPYSize;
	int	m_nBMPXByte;
	int	m_nBmpStat;
private:
	CRgn		m_crgnClip;
	CBitmap		m_cbmpCBitmap;
	CBitmap		*m_pcbackbmp;
	CDC			m_cdcMemCDC;
	HANDLE		m_hOneLine;
	LPBYTE		m_lpOneLine;
	HANDLE		m_hBits[BMPDIVMAX];
	LPBYTE		m_lpBits[BMPDIVMAX];
	long		m_lXByte[BMPDIVMAX];
	long		m_lBitsSize[BMPDIVMAX];
};

#endif

