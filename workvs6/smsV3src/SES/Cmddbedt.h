#ifndef __CMDDBEDT_H
#define __CMDDBEDT_H

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
	void FileMDBLoadMDBSet(LPSTR lpFileName);
	void FileXLSLoadMDBSet(LPSTR lpFileName);
	void FileCSVLoadMDBSet(LPSTR lpFileName);
	void SelectInitDB();

private:


};

#endif
