
#pragma once


//G　概要
//　オブジェクトの描画順序の変更
//	オブジェクトのグループ化（操作、選択単位）
//	複数のベクトルを複合化（塗潰し単位）するの機能を提供する
//
class CMinMaxStruct
{
public:
	DBL m_dMin;
	DBL m_dMax;
	CData	*m_pcData;
};

typedef CTypedPtrList<CPtrList, CMinMaxStruct*> CMinMaxStructList;

class CCmdCrdnt : public CCmdBase
{
public:
public:
	CCmdCrdnt(CScrollView* pcview);
public:
	CData*	m_pcData;
	CMinMaxStructList	*m_pcPtrList;

public:
	void SortAddList(CMinMaxStruct *pcSetMinMax);
	void DeleteAllList();
	UINT ExecSortHead(UINT event, DBL dx, DBL dy);
	UINT ExecSortTail(UINT event, DBL dx, DBL dy);
	UINT ExecSortNext(UINT event, DBL dx, DBL dy);
	UINT ExecSortPrev(UINT event, DBL dx, DBL dy);
	UINT ExecSortNextOne(UINT event, DBL dx, DBL dy);
	UINT ExecSortPrevOne(UINT event, DBL dx, DBL dy);
	UINT ExecGrupSet(UINT event, DBL dx, DBL dy);
	UINT ExecGrupReset(UINT event, DBL dx, DBL dy);
	UINT ExecListSet(UINT event, DBL dx, DBL dy);
	UINT ExecListReset(UINT event, DBL dx, DBL dy);
	UINT ExecCrdntTop(UINT event, DBL dx, DBL dy);
	UINT ExecCrdntVCenter(UINT event, DBL dx, DBL dy);
	UINT ExecCrdntBottom(UINT event, DBL dx, DBL dy);
	UINT ExecCrdntLeft(UINT event, DBL dx, DBL dy);
	UINT ExecCrdntHCenter(UINT event, DBL dx, DBL dy);
	UINT ExecCrdntRight(UINT event, DBL dx, DBL dy);
	UINT ExecMoveHCenter(UINT event, DBL dx, DBL dy);
	UINT ExecMoveVCenter(UINT event, DBL dx, DBL dy);
	UINT ExecMoveHVCenter(UINT event, DBL dx, DBL dy);
	UINT ExecSizeHSame(UINT event, DBL dx, DBL dy);
	UINT ExecSizeVSame(UINT event, DBL dx, DBL dy);
	UINT ExecSizeHVSame(UINT event, DBL dx, DBL dy);
	UINT ExecSpaseHEqual(UINT event, DBL dx, DBL dy);
	UINT ExecSpaseVEqual(UINT event, DBL dx, DBL dy);
private:
	int ListSetLoop(CDataList* pcSetDataList, CData* pcData);
	void ListResetLoop(CDataList *pcDataList);
	void SetCuserKind(DBL dx, DBL dy);
	void SortHeadMain();
	void SortTailMain();
	void SortNextMain();
	void SortPrevMain();
	void SortActiveNextMain();
	void SortActivePrevMain();
};

