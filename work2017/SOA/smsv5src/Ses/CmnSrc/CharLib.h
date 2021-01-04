// CCharLib ÉNÉâÉXíËã`
// ï∂éöóÒä÷åWä÷êî

#pragma once

class CCharLib
{
public:
	CCharLib(void);
	~CCharLib(void);
public:
	int TCHARStr_CharStr(TCHAR *lpSrcStr, char *lpDstStr, int nBufMax);
	int CharStr_TCHARStr(char *lpDstStr, TCHAR *lpSrcStr, int nBufMax);
	int CharStr_CString(char *lpSrcStr, CString *csDstStr);
	int CString_CharStr(CString csSrcStr, char *lpDstStr, int nBufMax);
	int WCharStr_CString(WCHAR *lpSrcStr, CString *csDstStr);
	int CString_WCharStr(CString csSrcStr, WCHAR *lpDstStr, int nBufMax);
	int WCharStr_CharStr(WCHAR *lpSrcStr, char *lpDstStr, int nBufMax);
	int CharStr_WCharStr(char *lpSrcStr, WCHAR *lpDstStr, int nBufMax);
	int	DivCsvStr(char *lpSrcStr, char *lpDstStr[]);
	LPSTR DivCsvStr(char *lpSrcStr);
	LPSTR DelDoubleQuartStr(char *lpSrcStr);
	LPSTR SkipSpace(char *lpSrcStr);
	int	TrnsCodeTable(char *lpSrcStr, UINT nCodeTbl[]);
	int IsKanji(char ch);
	int AddSafix(LPSTR FileName, LPSTR Safix);
	int ChangeSafix(LPSTR FileName, LPSTR Safix);
	int CmpSafixs(LPSTR lpFileName, LPSTR lpOrgSafixs);
	UINT GetCharCode(LPSTR strg);
	int SJisToJis(int far sjis);
	int JisToSJis(int far jis);
	void SJisToKuten(int far sjis, int far *ku, int far *ten);
	int KutenToJis(int kuten);
public:
};


