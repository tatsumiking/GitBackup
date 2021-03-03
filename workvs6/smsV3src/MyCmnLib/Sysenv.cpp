// sysenv.cpp : アプリケーション用環境定義
//

#include "stdafx.h"

#include "sysenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
}

CSysEnv::CSysEnv()
{
	int		i;
	DBL		dx, dy;

	m_dCntrX = 0;
	m_dCntrY = 0;
	m_dSubX = 0;
	m_dSubY = 0;
	m_dSubRag = 0;
	m_nSubFlag = OFF;
	m_dGrid = 0;
	m_nCntrlKey = OFF;
	m_nShiftKey = OFF;
}

CSysEnv::~CSysEnv()
{
}

void CSysEnv::GetTableListFname(int idx, LPSTR listfname)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	tfname[FNAMEMAX];
	char	dummy[QRECMAX];
	int		max;
	int		i;

	sprintf(listfname, "%s\\LIST\\tabledef.lst", m_szExePath);

	sprintf(fname, "%s\\LIST\\テーブル.lst", m_szExePath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	i = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(i == idx){
			sscanf(buf, "%s %s %d\n", dummy, tfname, &max);
			sprintf(listfname, "%s\\%s", m_szExePath, tfname);
			break;
		}
		i++;
	}
	fclose(fp);
}

void CSysEnv::GetTableMax(int idx, int *max)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		i;
	char	buf[RECMAX];
	char	dummy1[QRECMAX];
	char	dummy2[FNAMEMAX];

	*max = 7;
	sprintf(fname, "%s\\LIST\\テーブル.lst", m_szExePath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	i = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(i == idx){
			sscanf(buf, "%s %s %d\n", dummy1, dummy2, max);
		}
		i++;
	}
	fclose(fp);
}

BOOL CSysEnv::GetTablePartFname(LPSTR listfname, LPSTR tablename, LPSTR partfname)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	ttablename[QRECMAX];
	char	tpartfname[QRECMAX];
	int		i;
	BOOL	ret;

	if((fp = fopen(listfname, "r")) == NULL){
		sprintf(partfname, "%s\\PART\\tabledef.lay", m_szExePath);
		return(FALSE);
	}
	if(fgets(buf, RECMAX, fp) == NULL){
		fclose(fp);
		return(FALSE);
	}
	sscanf(buf, "%s\n", tpartfname);
	sprintf(partfname, "%s\\PART\\%s", m_szExePath, tpartfname);
	ret = FALSE;
	i = 1;
	while(1){
		memset(tpartfname, 0, RECMAX);
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s %s\n", ttablename, tpartfname);
		if(strcmp(tablename, ttablename) == 0 && tpartfname[0] != '\0'){
			sprintf(partfname, "%s\\PART\\%s", m_szExePath, tpartfname);
			ret = TRUE;
			break;
		}
		i++;
	}
	fclose(fp);
	return(ret);
}

void CSysEnv::GetTksgoFname(int idx, LPSTR partfname)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	tfname[FNAMEMAX];
	char	dummy[QRECMAX];
	int		i;

	sprintf(partfname, "%s\\PART\\tksgdef.lay", m_szExePath);

	sprintf(fname, "%s\\LIST\\高砂種類.lst", m_szExePath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	i = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(i == idx){
			sscanf(buf, "%s %s", dummy, tfname);
			sprintf(partfname, "%s\\PART\\%s", m_szExePath, tfname);
			break;
		}
		i++;
	}
	fclose(fp);
}

void CSysEnv::GetCmntFname(int idx, LPSTR pfffname1, LPSTR pfffname2)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	tfname1[FNAMEMAX];
	char	tfname2[FNAMEMAX];
	char	dummy[QRECMAX];
	int		i;

	sprintf(pfffname1, "%s\\PFF\\cmntdef.pff", m_szExePath);
	strcpy(pfffname2, "");

	sprintf(fname, "%s\\LIST\\コメント.lst", m_szExePath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	i = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(i == idx){
			sscanf(buf, "%s %s %s", dummy, tfname1, tfname2);
			sprintf(pfffname1, "%s\\%s", m_szExePath, tfname1);
			if(strncmp(tfname2, "dummy", 5) == 0){
				strcpy(tfname2, "");
			}
			else{
				sprintf(pfffname2, "%s\\%s", m_szExePath, tfname2);
			}
			break;
		}
		i++;
	}
	fclose(fp);
}

void CSysEnv::GetPaperSize(int idx, LPDBL xsize, LPDBL ysize)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		i;
	char	buf[RECMAX];
	char	name[QRECMAX];

	sprintf(fname, "%s\\LIST\\用紙種類.lst", m_szExePath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	i = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(i == idx){
			sscanf(buf, "%s %lf %lf\n", name, xsize, ysize);
			break;
		}
		i++;
	}
	fclose(fp);
}

void CSysEnv::SetExePath(LPSTR pcExePath)
{
	strcpy(m_szExePath, pcExePath);
}

void CSysEnv::GetExePath(LPSTR pcExePath)
{
	strcpy(pcExePath, m_szExePath);
}

void CSysEnv::LoadEnv()
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	int		len;

	strcpy(m_szUserName, "標準");
	strcpy(m_szCrtHotel, "標準");
	m_nCrtKihonNo = 0;
	sprintf(fname, "%s\\bk32_sys.env", m_szExePath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}

	fgets(m_szUserName, RECMAX, fp);		// ユーザー名
	len = strlen(m_szUserName);
	m_szUserName[len-1] = '\0';

	fgets(m_szCrtHotel, RECMAX, fp);		// ホテル名
	len = strlen(m_szCrtHotel);
	m_szCrtHotel[len-1] = '\0';

	fgets(buf, RECMAX, fp);		// 婚礼基本項目番号
	sscanf(buf, "%d", &m_nCrtKihonNo);	

	fclose(fp);
}

void CSysEnv::SaveEnv()
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];

	sprintf(fname, "%s\\bk32_sys.env", m_szExePath);
	if((fp = fopen(fname, "w")) == NULL){
		return;
	}
	fprintf(fp, "%s\n", m_szUserName);
	fprintf(fp, "%s\n", m_szCrtHotel);
	fprintf(fp, "%d\n", m_nCrtKihonNo);
	fclose(fp);
}

void CSysEnv::SetCrtHotel(LPSTR lpszHotel)
{
	strcpy(m_szCrtHotel, lpszHotel);
}

void CSysEnv::GetCrtHotel(LPSTR lpszHotel)
{
	strcpy(lpszHotel, m_szCrtHotel);
}

void CSysEnv::GetCrtKihonNo(int *nKihonNo)
{
	*nKihonNo = m_nCrtKihonNo;
}

void CSysEnv::SetCrtKihonNo(int nKihonNo)
{
	m_nCrtKihonNo = nKihonNo;
}

void CSysEnv::ScrnGetCntrXY(LPDBL cntrx, LPDBL cntry)
{
	*cntrx = m_dCntrX;
	*cntry = m_dCntrY;
}

void CSysEnv::ScrnSetCntrXY(DBL cntrx, DBL cntry)
{
	m_dCntrX = cntrx;
	m_dCntrY = cntry;
}

int CSysEnv::ScrnGetSubXY(LPDBL subx, LPDBL suby)
{
	*subx = m_dSubX;
	*suby = m_dSubY;
	return(m_nSubFlag);
}

void CSysEnv::ScrnSetSubXY(DBL subx, DBL suby)
{
	m_dSubX = subx;
	m_dSubY = suby;
	m_nSubFlag = ON;
}

void CSysEnv::ScrnGetSubRag(LPDBL subrag)
{
	*subrag = m_dSubRag;
}

void CSysEnv::ScrnSetSubRag(DBL subrag)
{
	m_dSubRag = subrag;
	m_nSubFlag = ON;
}

int	CSysEnv::ScrnOffSubFlag()
{
	int		flag;

	flag = m_nSubFlag;
	m_nSubFlag = OFF;
	return(flag);
}

void CSysEnv::ScrnGetGrid(LPDBL grid)
{
	*grid = m_dGrid;
}

void CSysEnv::ScrnSetGrid(DBL grid)
{
	m_dGrid = grid;
}

void CSysEnv::ScrnAdjustGrid(LPDBL retdx, LPDBL retdy)
{
	DBL		dx, dy;
	long	cntx, cnty;

	dx = (*retdx);
	dy = (*retdy);
	dx = dx - m_dCntrX;
	dy = dy - m_dCntrY;
	cntx = (int)(dx / m_dGrid + 0.5);
	cnty = (int)(dy / m_dGrid + 0.5);
	dx = (DBL)cntx * m_dGrid + m_dCntrX;
	dy = (DBL)cnty * m_dGrid + m_dCntrY;
	(*retdx) = dx;
	(*retdy) = dy;
}

int CSysEnv::ScrnCheckCntrlKey()
{
	return(m_nCntrlKey);
}

void CSysEnv::ScrnSetCntrlKey(int flag)
{
	m_nCntrlKey = flag;
}

int CSysEnv::ScrnCheckShiftKey()
{
	return(m_nShiftKey);
}

void CSysEnv::ScrnSetShiftKey(int flag)
{
	m_nShiftKey = flag;
}

