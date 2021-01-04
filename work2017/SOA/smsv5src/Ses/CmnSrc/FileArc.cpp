
/////////////////////////////////////////////////////////////////////////////
// CFileArc

#include "stdio.h"
#include "ctype.h"

#include "stdafx.h"
#include "FileArc.h"

#define	RENGE(a, b, c)	if((a) < (b)){(a) = (b);} else if((c) < (a)){(a) = (c);}

CFileArc::CFileArc(CScrollView* pcview)
{
	m_pcView = (CTonView*)pcview;
	m_pcDoc = (CTonDoc*)(m_pcView->GetDocument());
}

void CFileArc::LoadData(LPTSTR filename)
{
	FILE		*fp;
	TCHAR		buf[RECMAX];
	short		atr;
	float		fx;
	float		fy;
	DBL			dx, dy;
	int			sflag;
	int			pcnt;

	_tfopen_s(&fp, filename, _T("rb"));
	if(fp == NULL){
		return;
	}
	m_pcDataList = (CDataList*)(m_pcDoc->NewData(DATALISTID));
	fread(buf, 256, 1, fp);
	sflag = TRUE;
	while(1){
		fread(&atr, sizeof(short), 1, fp);
		fread(&fx, sizeof(float), 1, fp);
		fread(&fy, sizeof(float), 1, fp);
		if(atr == 33){
			if(pcnt != 0){
				m_pcVect->SetPointCount(pcnt);
				m_pcDataList->SetData(m_pcVect);
				pcnt = 0;
			}
			break;
		}
		switch(atr){
		case 0:
			dx = fx; dy = fy;
			sflag = TRUE;
			pcnt = 0;
			break;
		case 1:
			if(sflag == TRUE){
				sflag = FALSE;
				m_pcVect = (CVect*)(m_pcDoc->NewData(BASEVECTID));
				pcnt = 0;
			}
			m_pcVect->SetAtrDXY(pcnt, 0, dx, dy);
			pcnt++;
			dx = fx; dy = fy;
			break;
		case 3:
			m_pcVect->SetAtrDXY(pcnt, 0, dx, dy);
			pcnt++;
			dx = fx; dy = fy;
			m_pcVect->SetAtrDXY(pcnt, 0, dx, dy);
			pcnt++;
			break;
		case 2:
			if(sflag == TRUE){
				sflag = FALSE;
				m_pcVect = (CVect*)(m_pcDoc->NewData(BASEVECTID));
				pcnt = 0;
			}
			m_pcVect->SetAtrDXY(pcnt, CURVATR, dx, dy);
			pcnt++;
			dx = fx; dy = fy;
			break;
		case 12:
			m_pcVect->SetAtrDXY(pcnt, CURVATR|CONTATR, dx, dy);
			pcnt++;
			dx = fx; dy = fy;
			break;
		case 13:
			m_pcVect->SetAtrDXY(pcnt, CURVATR|CONTATR, dx, dy);
			pcnt++;
			dx = fx; dy = fy;
			m_pcVect->SetAtrDXY(pcnt, 0, dx, dy);
			pcnt++;
			break;
		case 33:
			break;
		}
		if(atr == 3 || atr == 13){
			if(pcnt != 0){
				m_pcVect->SetPointCount(pcnt);
				m_pcDataList->SetData(m_pcVect);
				pcnt = 0;
			}
		}
	}
	fclose(fp);

	m_pcDataList->RenewMiniMax();
	m_pcDoc->DataAddTail((CData*)m_pcDataList);
}

