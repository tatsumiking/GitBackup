// RdsOutCrct.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "RdsOutCrct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRdsOutCrct

IMPLEMENT_DYNAMIC(CRdsOutCrct, CDaoRecordset)

CRdsOutCrct::CRdsOutCrct(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRdsOutCrct)
	m_column1 = 0;
	m_column2 = _T("");
	m_column3 = _T("");
	m_column4 = _T("");
	m_column5 = _T("");
	m_column6 = _T("");
	m_column7 = _T("");
	m_column8 = _T("");
	m_nFields = 8;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CRdsOutCrct::GetDefaultDBName()
{
	return _T("OUTCRECT.MDB");
}

CString CRdsOutCrct::GetDefaultSQL()
{
	return _T("[ソートクエリー]");
}

void CRdsOutCrct::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRdsOutCrct)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Long(pFX, _T("[カウンタ]"), m_column1);
	DFX_Text(pFX, _T("[グループ名]"), m_column2);
	DFX_Text(pFX, _T("[用紙名]"), m_column3);
	DFX_Text(pFX, _T("[枠名]"), m_column4);
	DFX_Text(pFX, _T("[文字列]"), m_column5);
	DFX_Text(pFX, _T("[出力未済]"), m_column6);
	DFX_Text(pFX, _T("[コピー枚数]"), m_column7);
	DFX_Text(pFX, _T("[出力チェック]"), m_column8);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRdsOutCrct 診断

#ifdef _DEBUG
void CRdsOutCrct::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CRdsOutCrct::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
