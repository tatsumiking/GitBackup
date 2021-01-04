

#pragma once

class CDibLib
{
	int		m_nXDotSize, m_nYDotSize, m_nXByte;
	HANDLE	m_hDIB;
	long	m_dwFileSize;
	BYTE	*m_lpBits;
	CDC		*m_pcMemDC;
	CBitmap	*m_pcBitmap, *m_pcoldBitmap;
	
private:
	CTonView	*m_pcView;

public:
	CDibLib(CScrollView* pcview);
	~CDibLib();
	CDC 	*CreateDIBDC(int xsize, int ysize);
	void 	DeleteDIBDC();
	void 	AreaCls(CDC *pcDC);
	void	GetBitImage();
	int		PreFirstXOnBit(int y);
	int		PostFirstXOnBit(int y);
	int		PreFirstYOnBit(int x);
	int		PostFirstYOnBit(int x);
	void	BmpFileOut(LPTSTR fname, BYTE far *m_lpBits, int xbyte, int xsize, int ysize);
private:
};


