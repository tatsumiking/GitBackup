// wr32.h : WR32 アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_WR32_H__5CDF5605_823E_11D3_BF1B_00A0B0086478__INCLUDED_)
#define AFX_WR32_H__5CDF5605_823E_11D3_BF1B_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CWr32App:
// このクラスの動作の定義に関しては wr32.cpp ファイルを参照してください。
//

class CWr32App : public CWinApp
{
public:
	CWr32App();
	~CWr32App();

public:
	CSysEnv	*m_pcSysEnv;
	CFuncLib	*m_pcFuncLib;
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CWr32App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CWr32App)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_WR32_H__5CDF5605_823E_11D3_BF1B_00A0B0086478__INCLUDED_)
