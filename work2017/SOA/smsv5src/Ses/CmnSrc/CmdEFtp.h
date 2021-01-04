
#pragma once


//C ägëÂèkè¨ä÷åWÉRÉ}ÉìÉhèàóùÉNÉâÉX
class CCmdEFtp : public CCmdBaseBmp
{
public:
	CCmdEFtp(CScrollView* pcview);
public:
	UINT ExecMltprnFtp(UINT event, DBL dx, DBL dy);

public:
	TCHAR	m_strFtpPW[QRECMAX];
	TCHAR	m_strFtpRoot[RECMAX];
	TCHAR	m_strFtpPath[RECMAX];
	TCHAR	m_strFtpID[QRECMAX];
	TCHAR	m_nCrtNo;
	TCHAR	m_szFtpAdrs[QRECMAX];
	TCHAR	m_szBaseName[QRECMAX];
public:
	void SmsoutCrctFileEnd();
	void SmsoutSendData();
	void SmsoutCrctFileInit();
	void SaveBmpFile(LPTSTR fname, DBL dpi);
	void SaveBmpFileRv(LPTSTR fname, DBL dpi);
	void SaveDspBmpFile(LPTSTR fname, DBL dpi);
	void SaveDspBmpFileRv(LPTSTR fname, DBL dpi);
	void BmpToCmpBytelen(LPTSTR lpBmpFName, LPTSTR lpCmpFName);
	void CmpToBmpBytelen(LPTSTR lpCmpFName, LPTSTR lpBmpFName);
	void SrcZipConv(LPTSTR lpWorkPath, LPTSTR lpSrcFile, LPTSTR lpZipFile);
	void SendFileForFtp(LPTSTR lpFileName, LPTSTR lpFtpFolder);
	void Setint32(BYTE buf[], int offset, int num);
	void Setint16(BYTE buf[], int offset, int num);
private:


};

