
#ifndef __GRADDATA_H
#define __GRADDATA_H

#include "dibcdata.h"
// 内部使用予定クラス宣言

#define	CLRMAX	128

class AFX_CLASS_EXPORT CGradData : public CDibCData
{
protected:
	CGradData();
	DECLARE_SERIAL(CGradData)
public:
	CGradData(int nDummy);
	~CGradData();
public:
	int	m_nKind;	// 直線0,円形1
	DBL	m_dSX, m_dSY;	// 開始位置
	DBL	m_dLen;		// 長さ
	DBL	m_dRag;		// 方向角度
	int	m_nClrTblCount;	// 色テーブルの個数
	DBL	m_dClrPos[CLRMAX];		// 色の位置(0～100)
	DBL	m_dClrCtrlPos[CLRMAX];	// 前の色との重み位置(0～100)
	COLORREF	m_clrRGB[CLRMAX];	// 色情報
public:

private:
	
public:
	virtual int CheckID();
	virtual void Serialize(CArchive& ar);
};

#endif

