#pragma once

class CCrctWaku :
	public CObject
{
public:
	CCrctWaku();
	~CCrctWaku();
public:
	long m_nCode; //1 レコード番号
	CString	m_csFlag; //8 "0"　"1"
	CString m_csMiSumi; //6 "未"　"済"
	CString m_csGroupName; //2 枠グループ名
	CString m_csYousiName; //3 用紙名
	CString m_csFileName; //4 枠フルパス名
	CString m_csCount; //7 枚数
	CString m_csStrings; //5 /で区切った枠文字列
};

