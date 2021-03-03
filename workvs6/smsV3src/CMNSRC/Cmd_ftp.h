#ifndef __CMD_FTP_H
#define __CMD_FTP_H

#include "..\\cmnsrc\\cmdbsbmp.h"

//C 拡大縮小関係コマンド処理クラス
class CCmdFtp : public CCmdBaseBmp
{
public:
	CCmdFtp(CScrollView* pcview);
public:
	UINT ExecMltprnFtp(UINT event, DBL dx, DBL dy);

public:
	char	m_strFtpPW[QRECMAX];
	char	m_strFtpRoot[RECMAX];
	char	m_strFtpPath[RECMAX];
	char	m_strFtpID[QRECMAX];
	char	m_nCrtNo;
	char	m_szFtpAdrs[QRECMAX];
	char	m_szBaseName[QRECMAX];
public:
	void SmsoutCrctFileEnd();
	void SmsoutSendData();
	void SmsoutCrctFileInit();
	void SaveBmpFile(LPSTR fname, DBL dpi);
	void SaveDspBmpFile(LPSTR fname, DBL dpi);
	void BmpToCmpBytelen(LPSTR lpBmpFName, LPSTR lpCmpFName);
	void CmpToBmpBytelen(LPSTR lpCmpFName, LPSTR lpBmpFName);
	void SrcZipConv(LPSTR lpWorkPath, LPSTR lpSrcFile, LPSTR lpZipFile);
	void SendFileForFtp(LPSTR lpFileName, LPSTR lpFtpFolder);
	void Setint32(BYTE buf[], int offset, int num);
	void Setint16(BYTE buf[], int offset, int num);
private:


};

#endif
