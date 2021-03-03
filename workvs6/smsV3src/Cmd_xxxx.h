#ifndef __CMD_TEXT_H
#define __CMD_TEXT_H

//C 拡大縮小関係コマンド処理クラス
class CCmdText : public CCmdBase
{
public:
	CCmdText(CScrollView* pcview);
	UINT Exec(UINT event, DBL dx, DBL dy);
public:

private:


};

#endif
