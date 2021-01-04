
#pragma once


//C 拡大縮小関係コマンド処理クラス
class CCmdSend : public CCmdBaseBmp
{
public:
	CCmdSend(CScrollView* pcview);
public:
	UINT ExecEpsSend(UINT event, DBL dx, DBL dy);
	UINT ExecMprnSend(UINT event, DBL dx, DBL dy);
	UINT ExecPrnSend(UINT event, DBL dx, DBL dy);

public:

public:
	void BmpZipConv(LPTSTR lpBmpFile, LPTSTR lpZipFile);
	void BmpTarConv(LPTSTR lpBmpFile, LPTSTR lpTarFile);
	int BmpGzConv(LPTSTR lpBmpFile, LPTSTR lpGzFile);
	void SavePrnFile(LPTSTR fname);
	void SaveBmpFile(LPTSTR fname, DBL dpi);
	void SaveEpsFile(LPTSTR fname);
	void BmpCmpConvBytelen(LPTSTR lpBmpFName, LPTSTR lpCmpFName);
	void CmpBmpConvBytelen(LPTSTR lpCmpFName, LPTSTR lpBmpFName);
	void BmpCmpConvBitlen(LPTSTR lpBmpFName, LPTSTR lpCmpFName);
	void CmpBmpConvBitlen(LPTSTR lpCmpFName, LPTSTR lpBmpFName);

private:


};

