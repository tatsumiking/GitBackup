#pragma once

//C ���i�֌W�R�}���h�����N���X
class CCmdPart : public CCmdBaseBmp
{
public:
	CCmdPart(CScrollView* pcview);
	UINT ExecPartLoad(UINT event, DBL dx, DBL dy);
	UINT ExecPartSave(UINT event, DBL dx, DBL dy);
public:
	void LoadPart(LPTSTR lpName);
	void AddPartList(LPTSTR lpName);
	void SavePart(LPTSTR lpName);

private:


};

