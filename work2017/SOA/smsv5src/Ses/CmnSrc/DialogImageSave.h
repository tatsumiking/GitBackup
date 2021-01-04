
#pragma once

// DImgSave.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogImageSave ダイアログ

class CDialogImageSave : public CFileDialog
{
	DECLARE_DYNAMIC(CDialogImageSave)

public:
	CDialogImageSave(LPTSTR lpInitPath);
	~CDialogImageSave();

// ダイアログ データ
public:
	int	m_nXSize;
	int	m_nYSize;
	int	m_nQuality;
	int	m_nRotate;

// オーバーライド
	virtual BOOL OnFileNameOK();

// インプリメンテーション
public:
protected:
	//{{AFX_MSG(CDialogImageSave)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

