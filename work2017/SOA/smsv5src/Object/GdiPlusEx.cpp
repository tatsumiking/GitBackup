

#include "stdafx.h"
#include "GdiPlusEx.h"

#include	"../ObjLib/Scrnlib.h"

CGdiPlusEx::CGdiPlusEx()
{
	m_brushFill = new SolidBrush(Color(255, 0, 0, 0));
	m_pen = new Pen(Color(255, 0, 0, 0), 1);
	m_penLine = new Pen(Color(255, 0, 0, 0), 1);
	m_penCntrl = new Pen(Color(255, 0, 255, 255), 1);
	m_penBezier = new Pen(Color(255, 255, 0, 0), 1);
	m_graphics = NULL;
	m_path = NULL;
}
CGdiPlusEx::~CGdiPlusEx()
{
	if (m_graphics != NULL) {
		delete(m_graphics);
	}
	if (m_path != NULL) {
		delete(m_path);
	}
}
void CGdiPlusEx::InitGraphics(CDC *pcDC)
{
	m_graphics = new Graphics(pcDC->m_hDC);
	// UnitWorld, UnitDisplay, UnitPixel, UnitPoint, UnitInch, UnitDocument, UnitMillimeter
	m_graphics->SetPageUnit(UnitPixel);
}
void CGdiPlusEx::InitPath()
{
	// FillModeAlternate(default), FillModeWinding
	m_path = new GraphicsPath();
}
void CGdiPlusEx::DeletePath()
{
	delete(m_path);
	m_path = NULL;
}
void CGdiPlusEx::TransCoodinate(LPDBL dlen)
{

}
void CGdiPlusEx::TransCoodinate(LPDBL dx, LPDBL dy)
{
	scrndxymmtorltv(dx, dy);
	/*
	DBL dtime;
	DBL dxtime, dytime;
	DBL dcdx, dcdy;

	dtime = scrngetbasetime();
	scrngetxytime(&dxtime, &dytime);
	dcdx = *dx / dxtime;
	dcdy = *dy / dxtime;
	*dx = dcdx;
	*dy = dcdy;
	*/
}
void CGdiPlusEx::SetFillBrush(int rval, int gval, int bval)
{
	delete(m_brushFill);
	m_brushFill = new SolidBrush(Color(255, rval, gval, bval));
}
void CGdiPlusEx::SetFillBrush(int aval, int rval, int gval, int bval)
{
	delete(m_brushFill);
	m_brushFill = new SolidBrush(Color(aval, rval, gval, bval));
}
void CGdiPlusEx::SetPen(int rval, int gval, int bval, int width)
{
	delete(m_pen);
	m_pen = new Pen(Color(255, rval, gval, bval), width);
}
void CGdiPlusEx::SetPen(int aval, int rval, int gval, int bval, int width)
{
	delete(m_pen);
	m_pen = new Pen(Color(aval, rval, gval, bval), width);
}
void CGdiPlusEx::SetLinePen(int rval, int gval, int bval, int width)
{
	delete(m_penLine);
	m_penLine = new Pen(Color(255, rval, gval, bval), width);
}
void CGdiPlusEx::SetLinePen(int aval, int rval, int gval, int bval, int width)
{
	delete(m_penLine);
	m_penLine = new Pen(Color(aval, rval, gval, bval), width);
}
void CGdiPlusEx::SetBezierPen(int rval, int gval, int bval, int width)
{
	delete(m_penBezier);
	m_penBezier = new Pen(Color(255, rval, gval, bval), width);
}
void CGdiPlusEx::SetBezierPen(int aval, int rval, int gval, int bval, int width)
{
	delete(m_penBezier);
	m_penBezier = new Pen(Color(aval, rval, gval, bval), width);
}
void CGdiPlusEx::AddPathMoveTo(DBL dx, DBL dy)
{
	m_path->StartFigure();
	m_ptCrt.X = dx;
	m_ptCrt.Y = dy;
}
void CGdiPlusEx::AddPathLineTo(DBL x2, DBL y2)
{
	Point pt;

	pt.X = x2;
	pt.Y = y2;
	m_path->AddLine(m_ptCrt, pt);
	m_ptCrt = pt;
}
void CGdiPlusEx::AddPathBezierTo(DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4) {
	Point pt2, pt3, pt4;

	pt2.X = x2; pt2.Y = y2;
	pt3.X = x3; pt3.Y = y3;
	pt4.X = x4; pt4.Y = y4;
	m_path->AddBezier(m_ptCrt, pt2, pt3, pt4);
	m_ptCrt = pt4;

}
void CGdiPlusEx::PathCloseFigure()
{
	m_path->CloseFigure();
}
void CGdiPlusEx::CloseAllFigures()
{
	m_path->CloseAllFigures();
}
void CGdiPlusEx::DrawFillPath()
{
	m_graphics->FillPath(m_brushFill, m_path);
}
void CGdiPlusEx::DrawPath()
{
	m_graphics->DrawPath(m_pen, m_path);
}
void CGdiPlusEx::DrawLPoint(DBL sx, DBL sy, DBL ex, DBL ey)
{
	m_graphics->DrawRectangle(m_penLine, (REAL)sx, (REAL)sy, (REAL)(ex - sx), (REAL)(ey - sy));
}
void CGdiPlusEx::DrawCPoint(DBL sx, DBL sy, DBL ex, DBL ey)
{
	m_graphics->DrawRectangle(m_penBezier, (REAL)sx, (REAL)sy, (REAL)(ex - sx), (REAL)(ey - sy));
}
void CGdiPlusEx::DrawLine(DBL x1, DBL y1, DBL x2, DBL y2)
{
	Point pt1, pt2;

	pt1.X = x1; pt1.Y = y1;
	pt2.X = x2; pt2.Y = y2;
	m_graphics->DrawLine(m_penLine, pt1, pt2);
}
void CGdiPlusEx::DrawCLine(DBL x1, DBL y1, DBL x2, DBL y2)
{
	Point pt1, pt2;

	pt1.X = x1; pt1.Y = y1;
	pt2.X = x2; pt2.Y = y2;
	m_graphics->DrawLine(m_penCntrl, pt1, pt2);
}
void CGdiPlusEx::DrawBezier(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	Point pt1, pt2, pt3, pt4;

	pt1.X = x1; pt1.Y = y1;
	pt2.X = x2; pt2.Y = y2;
	pt3.X = x3; pt3.Y = y3;
	pt4.X = x4; pt4.Y = y4;
	m_graphics->DrawBezier(m_penBezier, pt1, pt2, pt3, pt4);
}
void CGdiPlusEx::DrawImage(Bitmap *bmp, DBL sx, DBL sy, DBL wd, DBL hi)
{
	m_graphics->DrawImage(bmp, (REAL)sx, (REAL)sy, (REAL)wd, (REAL)hi);
}


