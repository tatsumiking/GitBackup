
#pragma once

// Mente.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMente ウィンドウ

class CMente
{
// コンストラクション
public:
	CMente();
	virtual ~CMente();

// アトリビュート
public:

// オペレーション
public:

// インプリメンテーション
public:
	void SrchBackUpFolder(LPTSTR lpBackupPath);
	void CopyFolder(LPTSTR lpSrcPath, LPTSTR lpDstPath);

};

