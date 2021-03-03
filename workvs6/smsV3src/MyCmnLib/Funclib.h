// funclib.h : Bk32 �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(FUNCLIB_H)
#define FUNCLIB_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CFuncLib:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� funclib.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class AFX_CLASS_EXPORT CFuncLib
{
public:
	CFuncLib();
	~CFuncLib();

public:

public:
	int GetLastCharPos(char ch, LPSTR str);
	void ChangeSafix(LPSTR fname, LPSTR safixs);
	void AddSafix(LPSTR fname, LPSTR safixs);
	BOOL DelCRfgets(LPSTR lpBuf, int max, FILE *fp);
	void GetCrtDateStr(LPSTR lpDateStr);
	void GetCrtDate(int *yy, int *mm, int *dd, int *ww);
	void GetCrtTime(int *hh, int *min, int *ss);
	int IsKanji(int hc);
	int DivCsvData(LPSTR csvsp[], LPSTR csvbuf);
	void CreateDateStr(LPSTR lpDateStr, int yyyy, int mm, int dd, int hh, int min);
	void ConverteDateStr(LPSTR lpDateStr, int *yyyy, int *mm, int *dd, int *hh, int *min);
	void NameSetSapce(LPSTR name1, LPSTR name2, LPSTR name);
};

#endif // !defined(FUNCLIB_H)

