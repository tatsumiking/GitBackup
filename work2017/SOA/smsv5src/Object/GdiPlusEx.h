
#pragma once

class DllExport CGdiPlusEx
{
public:
	CGdiPlusEx();
	~CGdiPlusEx();
public:
	SolidBrush *m_brushFill;
	Pen *m_pen;
	Pen *m_penLine;
	Pen *m_penCntrl;
	Pen *m_penBezier;
	Graphics *m_graphics;
	GraphicsPath *m_path;
	Point	m_ptCrt;

public:
	void InitGraphics(CDC *pcDC);
	void InitPath();
	void DeletePath();
	void TransCoodinate(LPDBL dlen);
	void TransCoodinate(LPDBL dx, LPDBL dy);
	void SetFillBrush(int rval, int gval, int bval);
	void SetFillBrush(int aval, int rval, int gval, int bval);
	void SetPen(int rval, int gval, int bval, int width);
	void SetPen(int aval, int rval, int gval, int bval, int width);
	void SetLinePen(int rval, int gval, int bval, int width);
	void SetLinePen(int aval, int rval, int gval, int bval, int width);
	void SetBezierPen(int rval, int gval, int bval, int width);
	void SetBezierPen(int aval, int rval, int gval, int bval, int width);
	void AddPathMoveTo(DBL dx, DBL dy);
	void AddPathLineTo(DBL x2, DBL y2);
	void AddPathBezierTo(DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void PathCloseFigure();
	void CloseAllFigures();
	void DrawFillPath();
	void DrawPath();
	void DrawLPoint(DBL sx, DBL sy, DBL ex, DBL ey);
	void DrawCPoint(DBL sx, DBL sy, DBL ex, DBL ey);
	void DrawCLine(DBL x1, DBL y1, DBL x2, DBL y2);
	void DrawLine(DBL x1, DBL y1, DBL x2, DBL y2);
	void DrawBezier(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void DrawImage(Bitmap *bmp, DBL sx, DBL sy, DBL wd, DBL hi);
};

