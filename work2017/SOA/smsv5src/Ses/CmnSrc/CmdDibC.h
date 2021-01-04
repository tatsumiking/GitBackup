
#pragma once


//G　概要
//　ビットマップオブジェクトを操作する機能
//　ビットマップオブジェクトの生成、回転、
//　色を指定するベクトルデータへの変換
//　画像を変更する外部プログラムの呼び出しなどの
//　機能を提供する
//

#include "../../Object/DibCData.h"
#include "../../Object/DataLogo.h"
#include "../CmnSrc/CmdCrdnt.h"
#include "../CmnSrc/DialogTraceImage.h"

//C ＢＭＰ関係コマンド処理クラスの基本クラス
class CCmdDibC : public CCmdBaseBmp
{
public:
	CCmdDibC(CScrollView* pcview);
	~CCmdDibC();
	UINT ExecBmpLoad(UINT event, DBL dx, DBL dy);
	UINT ExecSave(UINT event, DBL dx, DBL dy);
	UINT ExecJpgSave(UINT event, DBL dx, DBL dy);
	UINT ExecRound(UINT event, DBL dx, DBL dy);
	UINT ExecTrace(UINT event, DBL dx, DBL dy);
	UINT ExecDotedit(UINT event, DBL dx, DBL dy);
	UINT ExecBmping(UINT event, DBL dx, DBL dy);
	UINT ExecAreaDel(UINT event, DBL dx, DBL dy);
	UINT ExecClip(UINT event, DBL dx, DBL dy);
	UINT ExecBkLoad(UINT event, DBL dx, DBL dy);
	UINT ExecBkReset(UINT event, DBL dx, DBL dy);
public:
	virtual void DrawAreaBoxs(CDC *pcDC);
	void SaveCrtEnv(LPTSTR fname);
	void SaveDocument();
	void ChangeCopyItiFile();
	BOOL SaveImageFile();
	int m_nTrace;
	void TraceMain();
	
	CCmdCrdnt	*m_pcCmdCrdnt;
	CDataLogo*	m_pcDataLogo;
	CDataList*	m_pcDataList;
	CVect*		m_pcVect;
	int			m_nPcnt;
	CDibCData	*m_pcDibCData;
	COLORREF	m_clrPicup;
	TCHAR		m_szEditProg[FNAMEMAX];
	TCHAR		m_szBmpFname[FNAMEMAX];
	int			m_nImageXSize;
	int			m_nImageYSize;
	int			m_nRotate;
	int			m_nQuality;
	TCHAR		m_szJpgInitPath[FNAMEMAX];
	TCHAR		m_szJpgFname[FNAMEMAX];
	int			m_nJpgXSize;
	int			m_nJpgYSize;
	int			m_nJpgRotate;
	int			m_nJpgQuality;
	HANDLE	m_hDib1;
	HANDLE	m_hDib2;
	HANDLE	m_hCusTbl;
	HANDLE	m_hXYTbl1;
	HANDLE	m_hXYTbl2;
	LPBYTE	m_lpDib1;
	LPBYTE	m_lpDib2;
	LPBYTE	m_lpBits1;
	LPBYTE	m_lpBits2;
	LPBYTE	m_lpCusTbl;
	XYType	*m_lpXYTbl1;
	XYType	*m_lpXYTbl2;
	int		m_nXSize, m_nYSize, m_nXByte;
	DBL		m_dFureLimit;

	void LocateTraceImgeDlg();
	void SetTraceClr();
	void SetDibCDataSize();
	BOOL LoadDibCData(LPTSTR lpPath);
	void SizeTrnsMain();
	virtual void InitDataAreaBoxs();

private:
	CDialogTraceImage*	m_pcDialogTraceImage;

private:
	void BmpFileOut(LPTSTR fname, BYTE far *lpBits, int xbyte, int xsize, int ysize);
	void BitMapOutLineSet(BYTE *lpBits, BYTE *lpTBits, int xbyte, int xsize, int ysize);
	void BitMapTrace(BYTE *lpBits, BYTE *lpTBits, int xbyte, int xsize, int ysize);
	void BitMapTarceSetPlgn(BYTE *lpBits, BYTE *lpTBits, int xbyte, int x, int y);
	void NextCusGet(BYTE *lpTBits, int xbyte, int *retx, int *rety, int *retcus);
	int FirstCusGet(BYTE *lpBits, int scus, int xbyte, int x, int y);
	void CusAddGet(int cus, int *addx, int *addy);
	int CusLegal(int cus);
	int BitOnOffCheck(BYTE *lpBits, int xbyte, int x, int y);
	void BitOnSet(BYTE *lpBits, int xbyte, int x, int y);
	void BitOffSet(BYTE *lpBits, int xbyte, int x, int y);
	void SetDataLogoPolygon(XYType *lpXYTbl1, XYType *lpXYTbl2, int points);
	void SetXYTblSetPlgn(BYTE *lpTBits, int xbyte, int idx, int tsx, int tsy);
	void SetPlgnEnd();
	void SetPlgnInit();
	void PlgnPointSet(XYType *lpXYTbl1, int points, int tx, int ty);
	void PlgnLineToArcRejionSet(CVect *pcVect);
	int PlgnCurveChk(CVect *pcVect, int pp1, DBL *retx, DBL *rety, int *retpp);
	int PlgnCrclDastance(DBL tx, DBL ty, DBL x0, DBL y0, DBL r, DBL flimit);
};

