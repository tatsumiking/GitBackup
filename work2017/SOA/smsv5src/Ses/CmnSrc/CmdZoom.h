
#pragma once


//C �g��k���֌W�R�}���h�����N���X
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

