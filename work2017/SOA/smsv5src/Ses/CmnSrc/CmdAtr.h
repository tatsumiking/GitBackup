
#pragma once


//G　概要
//	移動、複写、変形用のオブジェクトに対する
//　支点を設定する機能を提供するクラス
//

//C 支点設定コマンド処理クラス
class CCmdAtr : public CCmdBase
{
public:
	CCmdAtr(CScrollView* pcview);
	UINT ExecAtrConer(UINT event, DBL dx, DBL dy);
	UINT ExecAtrWakuConer(UINT event, DBL dx, DBL dy);
	UINT ExecAtrEditConer(UINT event, DBL dx, DBL dy);
	UINT ExecAtrMojiConer(UINT event, DBL dx, DBL dy);
public:

private:

};

