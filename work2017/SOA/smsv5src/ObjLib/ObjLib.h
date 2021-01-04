// ObjLib.h : ObjLib DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

// CObjLibApp
// このクラスの実装に関しては ObjLib.cpp をご覧ください
//

class CObjLibApp : public CWinApp
{
public:
	CObjLibApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
