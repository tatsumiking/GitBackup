#if !defined(_MENTE_H_)
#define _MENTE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
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
	void SrchBackUpFolder(LPSTR lpBackupPath);
	void CopyFolder(LPSTR lpSrcPath, LPSTR lpDstPath);

};

#endif // !defined(_MENTE_H_)
