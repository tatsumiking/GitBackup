
#pragma once

#include "CmdBase.h"
#include "BmpFile.h"

// DSetJpgEle.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSetJpgElement ダイアログ

class CDialogSetJpgElement : public CDialog
{
// コンストラクション
public:
	CDialogSetJpgElement(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CDialogSetJpgElement();
// ダイアログ データ
public:
	CTonView	*m_pcView;
	CTonDoc		*m_pcDoc;
	CBmpFile	*m_pcBmpFile;
	int			m_nInSort;
	int			m_nImageFitSizeX;
	int			m_nImageFitSizeY;
	int			m_nDotSizeX;
	int			m_nDotSizeY;
	//{{AFX_DATA(CDialogSetJpgElement)
	enum { IDD = IDD_SETJPGELEMENT };
	CString	m_csBaseName;
	int		m_nJpgSizeX;
	int		m_nJpgSizeY;
	int		m_nJpgSX;
	int		m_nJpgSY;
	int		m_nImageSizeX;
	int		m_nImageSizeY;
	int		m_nImageSPX;
	int		m_nImageSPY;
	BOOL	m_nChkNoWakuOut;
	int		m_nJpgQuality;
	int		m_nRoundRag;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSetJpgElement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void SaveJpgoutElement();
	void CreateJpgFile(int nPageNo);
	void SetFitingElement();
	void JpgOutMain(CDC *pcDC);

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSetJpgElement)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

