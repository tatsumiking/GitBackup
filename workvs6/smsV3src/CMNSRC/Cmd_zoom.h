#ifndef __CMD_ZOOM_H
#define __CMD_ZOOM_H

//C 拡大縮小関係コマンド処理クラス
class CCmdZoom : public CCmdBase
{
public:
	CCmdZoom(CScrollView* pcview);
	UINT ExecZoomAll();
	UINT ExecZoomIn(UINT event, DBL dx, DBL dy);
	UINT ExecZoomOut(UINT event, DBL dx, DBL dy);
public:
	void AreaInTimeLegal();
private:
	void ScrollAbsCntrSet(DBL dx, DBL dy);

};

#endif
