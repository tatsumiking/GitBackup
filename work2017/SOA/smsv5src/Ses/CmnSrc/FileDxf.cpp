
/////////////////////////////////////////////////////////////////////////////
// CFileDxf

#include "stdio.h"
#include "ctype.h"

#include "stdafx.h"
#include "FileDxf.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Fdxflib.h"
#include "../../ObjLib/Curvelib.h"
#include "../../ObjLib/Mojienv.h"
//}

#define	RENGE(a, b, c)	if((a) < (b)){(a) = (b);} else if((c) < (a)){(a) = (c);}

#define	LIMITMM		2.0

CFileDxf::CFileDxf(CScrollView* pcview)
{
	m_pcView = (CTonView*)pcview;
	m_pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_dLimit = LIMITMM;
}

void CFileDxf::SaveData(LPTSTR filename)
{
	fdxffileopen(filename);
	fdxfstartfile();

	CTypedPtrList<CObList,CData*> *pcObjDataList = m_pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pcData = pcObjDataList->GetNext(pos);
		pcData->SaveDxfFile();
		
	}
	fdxfendfile();
	fdxffileclose();
}

void CFileDxf::LoadData(LPTSTR filename)
{
	FILE	*fp;
	TCHAR	buffer[RECMAX];
	int		len, len1, len2, len3, len4, len5, len6;

	_tfopen_s(&fp, filename, _T("r"));
	if(fp == NULL){
		return;
	}
	m_pcVect = NULL;
	m_nPointCount = 0;
	m_pcDataGrup = (CDataGrup*)(m_pcDoc->NewData(DATAGRUPID));
	// ヘッダー部をスキップ
	while(1){
		if(FDxfFGets(buffer, RECMAX, fp) == NULL){
			break;
		}
		len = _tcslen(_T("ENTITIES"));
		if (_tcsnccmp(_T("ENTITIES"), buffer, len) == 0) {
			break;
		}
	}
	m_pcDataList = (CDataList*)(m_pcDoc->NewData(DATALISTID));
	m_pcDataGrup->SetData(m_pcDataList);
	while(1){
		if(FDxfFGets(buffer, RECMAX, fp) == NULL){
			break;
		}
		len1 = _tcslen(_T("ENDSEC"));
		len2 = _tcslen(_T("LINE"));
		len3 = _tcslen(_T("ARC"));
		len4 = _tcslen(_T("CIRCLE"));
		len5 = _tcslen(_T("POLYLINE"));
		len6 = _tcslen(_T("TEXT"));
		if (_tcsnccmp(_T("ENDSEC"), buffer, len1) == 0) {
			break;
		}
		else if(_tcsnccmp(_T("LINE"), buffer, len2) == 0) {
			if(LineLoad(fp) == FAIL){
				break;
			}
		}
		else if (_tcsnccmp(_T("ARC"), buffer, len3) == 0) {
			if(ArcLoad(fp) == FAIL){
				break;
			}
		}
		else if (_tcsnccmp(_T("CIRCLE"), buffer, len4) == 0) {
			if(CircleLoad(fp) == FAIL){
				break;
			}
		}
		else if (_tcsnccmp(_T("POLYLINE"), buffer, len5) == 0) {
			if(PolyLineLoad(fp) == FAIL){
				break;
			}
		}
		else if (_tcsnccmp(_T("TEXT"), buffer, len6) == 0) {
			if(TextLoad(fp) == FAIL){
				break;
			}
		}
	}
	if(m_pcVect != NULL){
		if(m_nPointCount == 0){
			delete(m_pcVect);
		}
		else{
			SetVect();
		}
	}
	m_pcDataGrup->RenewMiniMax();
	fclose(fp);

	m_pcDoc->DataAddTail((CData*)m_pcDataGrup);
}

void CFileDxf::SetVect()
{
	DBL		sx, sy;
	DBL		ex, ey;

	if(m_pcVect == NULL){
		return;
	}
	if(m_nPointCount >= 1){
		m_pcVect->GetDXY(0, &sx, &sy);
		if(PointContinueChk(sx, sy) == ON){
			m_pcVect->OnFlag(REJIONMASK);
			m_pcVect->SetPointCount(m_nPointCount-1);
		}
		else{
			if(m_nPointCount == 2){
				m_pcVect->GetDXY(0, &sx, &sy);
				m_pcVect->GetDXY(1, &ex, &ey);
				m_pcVect->GetDXY(0, &sx, &sy);
			}
			m_pcVect->OffFlag(REJIONMASK);
			m_pcVect->SetPointCount(m_nPointCount);
		}
		m_pcVect->RenewMiniMax();
		m_pcDataList->SetData(m_pcVect);
		m_pcVect = NULL;
		m_nPointCount = -1;
	}
}

int CFileDxf::PointContinueChk(DBL x, DBL y)
{
	DBL	len;

	if(m_pcVect == NULL){
		return(OFF);
	}
	len = hypot(m_dCrtX - x, m_dCrtY - y);
	if(len <= m_dLimit){
		return(ON);
	}
	return(OFF);
}

int CFileDxf::CreateVect()
{
	m_pcVect = (CVect*)(m_pcDoc->NewData(BASEVECTID));
	if(m_pcVect == 0){
		return(FAIL);
	}
	m_nPointCount = 0;
	m_pcVect->SetPointCount(m_nPointCount);
	return(0);
}

int CFileDxf::LineLoad(FILE *fp)
{
	TCHAR	buf[RECMAX];
	int		atr;
	DBL		sx, sy, ex, ey;
	int		len1, len2, len3, len4, len5;
	int		len6, len7, len8, len9, lenA;

	if(FDxfFGets(buf, RECMAX, fp) == NULL){
		return(0);
	}
	if(FDxfFGets(buf, RECMAX, fp) == NULL){
		return(0);
	}
	len1 = _tcslen(_T("10"));
	len2 = _tcslen(_T("20"));
	len3 = _tcslen(_T("30"));
	len4 = _tcslen(_T("11"));
	len5 = _tcslen(_T("21"));
	len6 = _tcslen(_T("31"));
	len7 = _tcslen(_T("62"));
	len8 = _tcslen(_T("8"));
	len9 = _tcslen(_T("6"));
	lenA = _tcslen(_T("0"));
	while(1){
		if(FDxfFGets(buf, RECMAX, fp) == FALSE){
			break;
		}
		if(_tcsnccmp(_T("10"), buf, len1) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &sx);
		}
		else if (_tcsnccmp(_T("20"), buf, len2) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &sy);
		}
		else if (_tcsnccmp(_T("30"), buf, len3) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if (_tcsnccmp(_T("11"), buf, len4) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &ex);
		}
		else if (_tcsnccmp(_T("21"), buf, len5) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &ey);
		}
		else if (_tcsnccmp(_T("31"), buf, len6) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if (_tcsnccmp(_T("62"), buf, len7) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if (_tcsnccmp(_T("8"), buf, len8) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if (_tcsnccmp(_T("6"), buf, len9) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if (_tcsnccmp(_T("0"), buf, lenA) == 0) {
			break;
		}
	}
	if(PointContinueChk(sx, sy) == ON){
		atr = 0;
		m_pcVect->SetAtrDXY(m_nPointCount, atr, ex, ey);
		m_nPointCount++;
	}
	else{
		SetVect();
		if(CreateVect() == FAIL){
			return(FAIL);
		}
		atr = 0;
		m_pcVect->SetAtrDXY(m_nPointCount, atr, sx, sy);
		m_nPointCount++;
		m_pcVect->SetAtrDXY(m_nPointCount, atr, ex, ey);
		m_nPointCount++;
	}
	m_dCrtX = ex; m_dCrtY = ey;
	return(0);
}

int CFileDxf::ArcLoad(FILE *fp)
{
	TCHAR	buf[RECMAX];
	DBL		x0, y0, r, srag, erag, trag;
	DBL		sx, sy, tx, ty, ex, ey;
	int		atr;
	DBL		dlen1, dlen2;
	int		len1, len2, len3, len4, len5, len6, len7;

	if(FDxfFGets(buf, RECMAX, fp) == NULL){
		return(0);
	}
	if(FDxfFGets(buf, RECMAX, fp) == NULL){
		return(0);
	}
	while(1){
		if(FDxfFGets(buf, RECMAX, fp) == NULL){
			return(0);
		}
		len1 = _tcslen(_T("10"));
		len2 = _tcslen(_T("20"));
		len3 = _tcslen(_T("30"));
		len4 = _tcslen(_T("40"));
		len5 = _tcslen(_T("50"));
		len6 = _tcslen(_T("51"));
		len7 = _tcslen(_T("0"));
		if (_tcsnccmp(_T("10"), buf, len1) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &x0);
		}
		else if (_tcsnccmp(_T("20"), buf, len2) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &y0);
		}
		else if (_tcsnccmp(_T("30"), buf, len3) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if (_tcsnccmp(_T("40"), buf, len4) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &r);
		}
		else if (_tcsnccmp(_T("50"), buf, len5) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &srag);
		}
		else if (_tcsnccmp(_T("51"), buf, len6) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &erag);
			break;
		}
		else if (_tcsnccmp(_T("0"), buf, len7) == 0) {
			break;
		}
	}
	if(srag > erag){
		erag += 360;
	}
	srag = (srag) * curvedegree1get();
	erag = (erag) * curvedegree1get();

	sx = cos(srag)*r + x0;
	sy = sin(srag)*r + y0;
	ex = cos(erag)*r + x0;
	ey = sin(erag)*r + y0;

	dlen1 = hypot(m_dCrtX - sx, m_dCrtY - sy);
	dlen2 = hypot(m_dCrtX - ex, m_dCrtY - ey);
	if(dlen1 < dlen2){
		trag = (erag + srag) / 2.0;
		tx = cos(trag)*r + x0;
		ty = sin(trag)*r + y0;
		if(PointContinueChk(sx, sy) == ON){
			atr = CURVATR;
			m_pcVect->SetAtrDXY(m_nPointCount-1, atr, sx, sy);
			atr = CURVATR | CONTATR;
			m_pcVect->SetAtrDXY(m_nPointCount, atr, tx, ty);
			m_nPointCount++;
			atr = 0;
			m_pcVect->SetAtrDXY(m_nPointCount, atr, ex, ey);
			m_nPointCount++;
			m_dCrtX = ex; m_dCrtY = ey;
		}
		else{
			SetVect();
			if(CreateVect() == FAIL){
				return(FAIL);
			}
			// 最初の始まりが３点円弧のとき回転方向が分からないため
			// 誤動作する可能性あり
			// 回避のため保存時に始点と同じ座標を直線で出力
			atr = CURVATR;
			m_pcVect->SetAtrDXY(m_nPointCount, atr, sx, sy);
			m_nPointCount++;
			atr = CURVATR | CONTATR;
			m_pcVect->SetAtrDXY(m_nPointCount, atr, tx, ty);
			m_nPointCount++;
			atr = 0;
			m_pcVect->SetAtrDXY(m_nPointCount, atr, ex, ey);
			m_nPointCount++;
			m_dCrtX = ex; m_dCrtY = ey;
		}
	}
	else{
		trag = (srag + erag) / 2.0;
		tx = cos(trag)*r + x0;
		ty = sin(trag)*r + y0;
		if(PointContinueChk(ex, ey) == ON){
			atr = CURVATR;
			m_pcVect->SetAtrDXY(m_nPointCount-1, atr, ex, ey);
			atr = CURVATR | CONTATR;
			m_pcVect->SetAtrDXY(m_nPointCount, atr, tx, ty);
			m_nPointCount++;
			atr = 0;
			m_pcVect->SetAtrDXY(m_nPointCount, atr, sx, sy);
			m_nPointCount++;
			m_dCrtX = sx; m_dCrtY = sy;
		}
		else{
			SetVect();
			if(CreateVect() == FAIL){
				return(FAIL);
			}
			// 最初の始まりが３点円弧のとき回転方向が分からないため
			// 誤動作する可能性あり
			// 回避のため保存時に始点と同じ座標を直線で出力
			atr = CURVATR;
			m_pcVect->SetAtrDXY(m_nPointCount, atr, ex, ey);
			m_nPointCount++;
			atr = CURVATR | CONTATR;
			m_pcVect->SetAtrDXY(m_nPointCount, atr, tx, ty);
			m_nPointCount++;
			atr = 0;
			m_pcVect->SetAtrDXY(m_nPointCount, atr, sx, sy);
			m_nPointCount++;
			m_dCrtX = sx; m_dCrtY = sy;
		}
	}
	return(0);
}

int CFileDxf::CircleLoad(FILE *fp)
{
	TCHAR	buf[RECMAX];
	DBL		x0, y0, r, srag, erag, trag;
	DBL		sx, sy, tx, ty, ex, ey;
	int		atr;
	int		len1, len2, len3, len4, len5;

	if(FDxfFGets(buf, RECMAX, fp) == NULL){
		return(0);
	}
	if(FDxfFGets(buf, RECMAX, fp) == NULL){
		return(0);
	}
	while(1){
		if(FDxfFGets(buf, RECMAX, fp) == NULL){
			return(0);
		}
		len1 = _tcslen(_T("10"));
		len2 = _tcslen(_T("20"));
		len3 = _tcslen(_T("30"));
		len4 = _tcslen(_T("40"));
		len5 = _tcslen(_T("0"));
		if (_tcsnccmp(_T("10"), buf, len1) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &x0);
		}
		else if (_tcsnccmp(_T("20"), buf, len2) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &y0);
		}
		else if (_tcsnccmp(_T("30"), buf, len3) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if (_tcsnccmp(_T("40"), buf, len4) == 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &r);
		}
		else if (_tcsnccmp(_T("0"), buf, len5) == 0) {
			break;
		}
	}
	srag = 0; trag = 90; erag = 180;
	srag = (-srag) * curvedegree1get();
	erag = (-erag) * curvedegree1get();
	trag = (-trag) * curvedegree1get();
	sx = cos(srag)*r + x0;
	sy = sin(srag)*r + y0;
	tx = cos(trag)*r + x0;
	ty = sin(trag)*r + y0;
	ex = cos(erag)*r + x0;
	ey = sin(erag)*r + y0;
	SetVect();
	if(CreateVect() == FAIL){
		return(FAIL);
	}
	atr = CURVATR;
	m_pcVect->SetAtrDXY(m_nPointCount, atr, sx, sy);
	m_nPointCount++;
	atr = CURVATR | CONTATR;
	m_pcVect->SetAtrDXY(m_nPointCount, atr, tx, ty);
	m_nPointCount++;
	atr = CURVATR;
	m_pcVect->SetAtrDXY(m_nPointCount, atr, ex, ey);
	m_nPointCount++;
	tx = ex;	ty = ey;
	ex = sx;	ey = sy;
	sx = tx;	sy = ty;
	trag = 270;
	trag = (-trag) * curvedegree1get();
	tx = cos(trag)*r+x0;
	ty = sin(trag)*r+y0;

	atr = CURVATR | CONTATR;
	m_pcVect->SetAtrDXY(m_nPointCount, atr, tx, ty);
	m_nPointCount++;
	atr = CURVATR;
	m_pcVect->SetAtrDXY(m_nPointCount, atr, ex, ey);
	m_nPointCount++;
	m_dCrtX = ex; m_dCrtY = ey;
	return(0);
}

int CFileDxf::PolyLineLoad(FILE *fp)
{
	TCHAR	buf[RECMAX];
	DBL		vertx, verty;
	int		firstflag;
	int		len, len0, len1, len2;

	firstflag = ON;
	if(FDxfFGets(buf, RECMAX, fp) == NULL){
		return(0);
	}
	if(FDxfFGets(buf, RECMAX, fp) == NULL){
		return(0);
	}
	while(1) {
		if(FDxfFGets(buf, RECMAX, fp) == NULL){
			return(0);
		}
		len0 = _tcslen(_T("0"));
		if (_tcsnccmp(_T("0"),buf,len0) != 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else{
			while (_tcsnccmp(_T("0"), buf, len0) == 0){
				if(FDxfFGets(buf, RECMAX, fp) == NULL){
					return(0);
				}
				len = _tcslen(_T("VERTEX"));
				if(_tcsnccmp(_T("VERTEX"), buf, len) == 0){
					if(FDxfFGets(buf, RECMAX, fp) == NULL){
						return(0);
					}
					while (_tcsnccmp(_T("0"), buf, len0) != 0){
						len1 = _tcslen(_T("10"));
						len2 = _tcslen(_T("20"));
						if (_tcsnccmp(_T("10"), buf, len1) == 0){
							if(FDxfFGets(buf, RECMAX, fp) == NULL){
								return(0);
							}
							_stscanf_s(buf, _T("%lf\n"), &vertx);
						}
						else
						if (_tcsnccmp(_T("20"), buf, len2) == 0){
							if(FDxfFGets(buf, RECMAX, fp) == NULL){
								return(0);
							}
							_stscanf_s(buf, _T("%lf\n"), &verty);
						}
						else {
							if(FDxfFGets(buf, RECMAX, fp) == NULL){
								return(0);
							}
						}
						if(FDxfFGets(buf, RECMAX, fp) == NULL){
							return(0);
						}
					}
					if(firstflag == ON){
						if(PointContinueChk(vertx, verty) == OFF){
							SetVect();
							if(CreateVect() == FAIL){
								return(FAIL);
							}
						}
						firstflag = OFF;
					}
					m_pcVect->SetAtrDXY(m_nPointCount, 0, vertx, verty);
					m_nPointCount++;
				}
				else{
			 		break;
				}
			}
		}
		len = _tcslen(_T("SEQEND"));
		if(_tcsnccmp(_T("SEQEND"), buf, len) == 0){
			break; 
		}
	}
	m_dCrtX = vertx; m_dCrtY = verty;
	return(0);
}

int CFileDxf::TextLoad(FILE *fp)
{
	TCHAR		buf[RECMAX];
	DBL			sx, sy, slen;
	DBL			heigth, space;
	TCHAR		strg[RECMAX];
	CStrgVect	*pcStrgVect;
	TCHAR		sFontName[FACEMAX];
	int		len1, len2, len3, len4, len5;
	int		len6, len7, len8, len9;

	len1 = _tcslen(_T("10"));
	len2 = _tcslen(_T("20"));
	len3 = _tcslen(_T("40"));
	len4 = _tcslen(_T("41"));
	len5 = _tcslen(_T("1"));
	len6 = _tcslen(_T("7"));
	len7 = _tcslen(_T("8"));
	len8 = _tcslen(_T("62"));
	len9 = _tcslen(_T("0"));
	while(1){
		if(FDxfFGets(buf, RECMAX, fp) == NULL){
			return(0);
		}
		if(_tcsnccmp(_T("10"), buf, len1) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &sx);
		}
		else if(_tcsnccmp(_T("20"), buf, len2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &sy);
		}
		else if(_tcsnccmp(_T("40"), buf, len3) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &heigth);
		}
		else if(_tcsnccmp(_T("41"), buf, len4) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%lf\n"), &space);
		}
		else if(_tcsnccmp(_T("1"), buf, len5) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			_stscanf_s(buf, _T("%s\n"), strg, RECMAX);
		}
		else if(_tcsnccmp(_T("7"), buf, len6) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(_tcsnccmp(_T("8"), buf, len7) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(_tcsnccmp(_T("62"), buf, len8) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(_tcsnccmp(_T("0"), buf, len9) == 0){
			break;
		}
	}

	slen = 0.0;
	pcStrgVect = (CStrgVect*)(m_pcDoc->NewData(STRGVECTID));
	if(pcStrgVect == NULL){
		return(FAIL);
	}
	mojienvgetfontname(sFontName);
	pcStrgVect->SetFontName(sFontName);
	pcStrgVect->SetTateYoko(YOKOGAKI);
	pcStrgVect->SetFontSize(heigth, heigth);
	pcStrgVect->SetSpaceEnv(space, space);
	pcStrgVect->SetStrStartXY(sx, sy);
	pcStrgVect->SetStrLen(slen);
	pcStrgVect->SetString(strg);
	m_pcDataGrup->SetData(pcStrgVect);
	return(0);
}

LPTSTR CFileDxf::FDxfFGets(LPTSTR retbuf, int maxlen, FILE *fp)
{
	LPTSTR	ret;
	TCHAR	buffer[RECMAX];
	int		len;
	LPTSTR	sp;	

	ret = baselibCrDelFgets(buffer, maxlen, fp);
	if(ret == NULL){
		return(ret);
	}
	sp = buffer;

	while(*sp){
		if(!isspace(*sp)){
			break;
		}
		sp++;
	}
	len = _tcslen(sp) + 1;
	_tcscpy_s(retbuf, len, sp);
	return(retbuf);
}

