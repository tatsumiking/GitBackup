//
// Easy Scanner System Ver1.0
//
// ess  disp.c	プロッター出力データコマンド処理関数
//
//  Copyright (c) 1998 by THEON

#include	"stdafx.h"
#include	"Curvelib.h"
#include	"Plot.h"
#include	"Scrnlib.h"
#include	"Disp.h"

HDC		m_hDC;
HPEN	m_hPen, m_oldhPen;

DllExport void dispsethdc(HDC hDC)
{
	m_hDC = hDC;
}

DllExport void dispinit()
{
	m_hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	m_oldhPen = (HPEN)SelectObject(m_hDC, m_hPen);
}

DllExport void dispend()
{
	SelectObject(m_hDC, m_oldhPen);
	DeleteObject(m_hPen);
}

void dispmove(DBL dx, DBL dy)
{
	POINT	point;

	plotdxysteptomm(&dx, &dy);
	scrndxymmtorltv(&dx, &dy);
	MoveToEx(m_hDC, (int)dx, (int)dy, &point);
}

void dispdrawline(DBL x1, DBL y1, DBL x2, DBL y2)
{
	plotdxysteptomm(&x2, &y2);
	scrndxymmtorltv(&x2, &y2);
	LineTo(m_hDC, (int)x2, (int)y2);
}

void dispdraw3penko(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	int	stat;
	DBL	x0, y0, rag01, rag03, dr;

	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	}
	if(stat == FAIL){
		dispdrawline(x1, y1, x2, y2);
		dispdrawline(x2, y2, x3, y3);
		return;
	}
	dispdrawenko(x0, y0, dr, rag01, rag03, x1, y1, x3, y3);
}

void dispdrawenko(DBL x0, DBL y0, DBL dr, DBL rag1, DBL rag2, DBL x1, DBL y1, DBL x2, DBL y2)
{
	int		idx, stepcnt;
	DBL		steprag, rag;
	DBL		sx, sy, ex, ey;

	stepcnt = 10;
	steprag = (rag2 - rag1) / stepcnt;
	sx = x1; sy = y1;
	for(idx = 1, rag = rag1+steprag; idx < stepcnt; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		dispdrawline(sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
	dispdrawline(sx, sy, x2, y2);
}

