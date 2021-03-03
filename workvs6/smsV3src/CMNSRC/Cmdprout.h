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
	// 以下ダイアログで設定する内容
	char	m_szSaverPath[FNAMEMAX];
	char	m_szSendPath[FNAMEMAX];
	CDataList	*m_pcDataList;
	int	m_nCmdKind;				// 出力機種別
	char	m_szPaperName[RECMAX];
	int	m_nXdpi, m_nYdpi;		// 出力機の解像度
	DBL	m_dStep;
	BOOL m_bChkX, m_bChkY;		// 縦横固定フラグ
	DBL	m_dXMMOut, m_dYMMOut;	// 出力データーサイズ
	DBL	m_dXMMPaper, m_dYMMPaper;	// 出力用紙サイズ
	DBL	m_dXMMNori, m_dYMMNori;	// のりしろサイズ
	DBL	m_dSendTime;	// 用紙送り補正倍率
	int	m_nCopyCount;	// 割付カウント
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
	DBL	m_dXDotNori, m_dYDotNori;	// のりしろドットサイズ
	DBL	m_dXDotOut, m_dYDotOut;	// 出力ドットデーターサイズ
	DBL m_dXTime, m_dYTime;			// データ変換倍率
	DBL m_dXDotMax, m_dYDotMax;		// 全体データサイズ
	DBL m_dXBlockSize, m_dYBlockSize;	//　一枚のデータサイズ
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

