
/////////////////////////////////////////////////////////////////////////////
// CFileDxf

#include "stdafx.h"
#include "stdio.h"
#include "math.h"
#include "ctype.h"

#include "filedxf.h"

#define	RENGE(a, b, c)	if((a) < (b)){(a) = (b);} else if((c) < (a)){(a) = (c);}

extern "C" {
int fdxffileopen(char *filename);
int fdxffileclose();
void fdxfstartfile();
void fdxfendfile();
void fdxflinesave(DBL x1, DBL y1, DBL x2, DBL y2);
void fdxfarcsave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
void fdxfbezesave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
void fdxffreecurvesave(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
DBL curvedegree1get();
void mojienvgetfontname(LPSTR fontname);
}

#define	LIMITMM		2.0

CFileDxf::CFileDxf(CScrollView* pcview)
{
	m_pcView = (CTonView*)pcview;
	m_pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_dLimit = LIMITMM;
}

void CFileDxf::SaveData(LPSTR filename)
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

void CFileDxf::LoadData(LPSTR filename)
{
	FILE	*fp;
	char	buffer[RECMAX];

	if((fp = fopen(filename, "r")) == NULL){
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
		if(strncmp("ENTITIES", buffer, 8) == 0){
			break;
		}
	}
	m_pcDataList = (CDataList*)(m_pcDoc->NewData(DATALISTID));
	m_pcDataGrup->SetData(m_pcDataList);
	while(1){
		if(FDxfFGets(buffer, RECMAX, fp) == NULL){
			break;
		}
		if(strncmp("ENDSEC", buffer, 6) == 0){
			break;
		}
		else if(strncmp("LINE", buffer, 4) == 0){
			if(LineLoad(fp) == FAIL){
				break;
			}
		}
		else if(strncmp("ARC", buffer, 3) == 0){
			if(ArcLoad(fp) == FAIL){
				break;
			}
		}
		else if(strncmp("CIRCLE", buffer, 6) == 0){
			if(CircleLoad(fp) == FAIL){
				break;
			}
		}
		else if(strncmp("POLYLINE", buffer, 8) == 0){
			if(PolyLineLoad(fp) == FAIL){
				break;
			}
		}
		else if(strncmp("TEXT", buffer, 4) == 0){
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
	char	buf[RECMAX];
	int		atr;
	DBL		sx, sy, ex, ey;

	if(FDxfFGets(buf, RECMAX, fp) == NULL){
		return(0);
	}
	if(FDxfFGets(buf, RECMAX, fp) == NULL){
		return(0);
	}
	while(1){
		if(FDxfFGets(buf, RECMAX, fp) == FALSE){
			break;
		}
		if(strncmp("10", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &sx);
		}
		else if(strncmp("20", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &sy);
		}
		else if(strncmp("30", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(strncmp("11", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &ex);
		}
		else if(strncmp("21", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &ey);
		}
		else if(strncmp("31", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(strncmp("62", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(strncmp("8", buf, 1) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(strncmp("6", buf, 1) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(strncmp("0", buf, 1) == 0){
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
	char	buf[RECMAX];
	DBL		x0, y0, r, srag, erag, trag;
	DBL		sx, sy, tx, ty, ex, ey;
	int		atr;
	DBL		len1, len2;

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
		if(strncmp("10", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &x0);
		}
		else if(strncmp("20", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &y0);
		}
		else if(strncmp("30", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(strncmp("40", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &r);
		}
		else if(strncmp("50", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &srag);
		}
		else if(strncmp("51", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &erag);
			break;
		}
		else if(strncmp("0", buf, 1) == 0){
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

	len1 = hypot(m_dCrtX - sx, m_dCrtY - sy);
	len2 = hypot(m_dCrtX - ex, m_dCrtY - ey);
	if(len1 < len2){
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
	char	buf[RECMAX];
	DBL		x0, y0, r, srag, erag, trag;
	DBL		sx, sy, tx, ty, ex, ey;
	int		atr;

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
		if(strncmp("10", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &x0);
		}
		else if(strncmp("20", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &y0);
		}
		else if(strncmp("30", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(strncmp("40", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &r);
		}
		else if(strncmp("0", buf, 1) == 0){
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
	char	buf[RECMAX];
	DBL		vertx, verty;
	int		firstflag;

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
		if (strncmp("0",buf,1) != 0) {
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else{
			while (strncmp("0", buf, 1) == 0){
				if(FDxfFGets(buf, RECMAX, fp) == NULL){
					return(0);
				}
				if(strncmp("VERTEX", buf, 6) == 0){
					if(FDxfFGets(buf, RECMAX, fp) == NULL){
						return(0);
					}
					while (strncmp("0", buf, 1) != 0){
						if (strncmp("10", buf, 2) == 0){
							if(FDxfFGets(buf, RECMAX, fp) == NULL){
								return(0);
							}
							sscanf(buf, "%lf\n", &vertx);
						}
						else
						if (strncmp("20", buf, 2) == 0){
							if(FDxfFGets(buf, RECMAX, fp) == NULL){
								return(0);
							}
							sscanf(buf, "%lf\n", &verty);
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
		if(strncmp("SEQEND", buf, 6) == 0){
			break; 
		}
	}
	m_dCrtX = vertx; m_dCrtY = verty;
	return(0);
}

int CFileDxf::TextLoad(FILE *fp)
{
	char		buf[RECMAX];
	DBL			sx, sy, slen;
	DBL			heigth, space;
	char		strg[RECMAX];
	CStrgVect	*pcStrgVect;
	char		sFontName[FACEMAX];

	while(1){
		if(FDxfFGets(buf, RECMAX, fp) == NULL){
			return(0);
		}
		if(strncmp("10", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &sx);
		}
		else if(strncmp("20", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &sy);
		}
		else if(strncmp("40", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &heigth);
		}
		else if(strncmp("41", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%lf\n", &space);
		}
		else if(strncmp("1", buf, 1) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
			sscanf(buf, "%s\n", strg);
		}
		else if(strncmp("7", buf, 1) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(strncmp("8", buf, 1) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(strncmp("62", buf, 2) == 0){
			if(FDxfFGets(buf, RECMAX, fp) == NULL){
				return(0);
			}
		}
		else if(strncmp("0", buf, 1) == 0){
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

LPSTR CFileDxf::FDxfFGets(LPSTR retbuf, int maxlen, FILE *fp)
{
	LPSTR	ret;
	char	buffer[RECMAX];
	int		len;
	LPSTR	sp;	

	ret = fgets(buffer, maxlen, fp);
	if(ret == NULL){
		return(ret);
	}
	len = strlen(buffer);
	buffer[len-1] = '\0';
	sp = buffer;

	while(*sp){
		if(!isspace(*sp)){
			break;
		}
		sp++;
	}

	strcpy(retbuf, sp);
	return(retbuf);
}

