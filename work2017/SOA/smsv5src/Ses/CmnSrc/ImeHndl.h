

#pragma once

class AFX_CLASS_EXPORT CImeHndl
{
public:
	HIMC	m_hImc;
	HWND	m_hWnd;

	CImeHndl();
	virtual ~CImeHndl();
	void ImeHndlOn(HWND hWnd);
	void ImeHndlOff(HWND hWnd);
	void ImeHndlKanji(HWND hWnd);
	void ImeHndlKana(HWND hWnd);
	void ImeHndlNon(HWND hWnd);
};


