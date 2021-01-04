
#pragma once


//C �g��k���֌W�R�}���h�����N���X
class CCmdDBEdit : public CCmdBase
{
public:
	CCmdDBEdit(CScrollView* pcview);
	UINT ExecDBLoad(UINT event, DBL dx, DBL dy);
	UINT ExecDBDelete(UINT event, DBL dx, DBL dy);

public:
	int			m_nType;
	CString		m_csFileName;
	CString		m_csTableName;

public:
	void FileMDBLoadMDBSet(LPTSTR lpFileName);
	void FileXLSLoadMDBSet(LPTSTR lpFileName);
	void FileCSVLoadMDBSet(LPTSTR lpFileName);
	void SelectInitDB();

private:


};

