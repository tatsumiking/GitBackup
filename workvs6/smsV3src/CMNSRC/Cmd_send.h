#ifndef __CMD_SEND_H
#define __CMD_SEND_H

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
	void BmpZipConv(LPSTR lpBmpFile, LPSTR lpZipFile);
	void BmpTarConv(LPSTR lpBmpFile, LPSTR lpTarFile);
	int BmpGzConv(LPSTR lpBmpFile, LPSTR lpGzFile);
	void SavePrnFile(LPSTR fname);
	void SaveBmpFile(LPSTR fname, DBL dpi);
	void SaveEpsFile(LPSTR fname);
	void BmpCmpConvBytelen(LPSTR lpBmpFName, LPSTR lpCmpFName);
	void CmpBmpConvBytelen(LPSTR lpCmpFName, LPSTR lpBmpFName);
	void BmpCmpConvBitlen(LPSTR lpBmpFName, LPSTR lpCmpFName);
	void CmpBmpConvBitlen(LPSTR lpCmpFName, LPSTR lpBmpFName);

private:


};

#endif
