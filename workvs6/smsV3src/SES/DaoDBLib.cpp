// DaoDBLib.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "DaoDBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	EXCELCLMMAX		256

extern "C" {
	int baselastspacedel(char *lpstr);
	int basedivcsvdata(LPSTR csvsp[], LPSTR csvbuf);
	int baseiskanji(UINT far hc);
}

IMPLEMENT_DYNCREATE(CDaoDBLib, CObject)

/////////////////////////////////////////////////////////////////////////////
// CDaoDBLib

CDaoDBLib::CDaoDBLib()
{
	m_lpDBBuf = (LPTSTR)new char[TEMPBUFMAX2];
}

CDaoDBLib::~CDaoDBLib()
{
	if(m_lpDBBuf){
		delete []m_lpDBBuf;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDaoDBLib 診断

#ifdef _DEBUG
void CDaoDBLib::AssertValid() const
{
	CObject::AssertValid();
}

void CDaoDBLib::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDaoDBLib メッセージ ハンドラ

void CDaoDBLib::Open()
{
	CString		csStr;

	switch(m_nType){
	case DBCSV:
		if(CheckCsvFile(m_csFileName.GetBuffer(FNAMEMAX)) == FALSE){
			m_nDBOpen = FALSE;
			m_fpCSV = NULL;
			break;
		}
		m_fpCSV = fopen(m_csFileName.GetBuffer(FNAMEMAX), "r");
		if(m_fpCSV == NULL){
			m_nDBOpen = FALSE;
		}
		m_nDBOpen = TRUE;
		break;
	case DBMDBT:
		try{
			m_pcDaoDatabase = new CDaoDatabase();
			m_pcDaoDatabase->Open(m_csFileName);
		}
		catch (CDaoException* e1){
			e1->Delete();
			m_nDBOpen = FALSE;
			return;
		}
		m_nDBOpen = TRUE;
		break;
	case DBMDBQ:
		try{
			m_pcDaoDatabase = new CDaoDatabase();
			m_pcDaoDatabase->Open(m_csFileName);
		}
		catch (CDaoException* e1){
			e1->Delete();
			m_nDBOpen = FALSE;
			return;
		}
		m_nDBOpen = TRUE;
		break;
	case DBXLS:
		try{
			if(m_cExcelLib.Open(&m_csFileName) == FALSE){
				m_nDBOpen = FALSE;
				return;
			}
		}
		catch (CException* e1){
			e1->Delete();
			m_nDBOpen = FALSE;
			return;
		}

		try{
			if(m_cExcelLib.OpenSheet(&m_csTableName) == FALSE){
				m_nDBOpen = FALSE;
				return;
			}
		}
		catch (CException* e2){
			m_cExcelLib.Close();
			e2->Delete();
			m_nDBOpen = FALSE;
			return;
		}
		m_nDBOpen = TRUE;
		break;
	}
}

void CDaoDBLib::Close()
{
	if(m_nDBOpen == FALSE){
		return;
	}
	switch(m_nType){
	case DBCSV:
		if(m_nDBOpen == FALSE){
			return;
		}
		fclose(m_fpCSV);
		break;
	case DBMDBT:
		if(m_nDBOpen == FALSE){
			return;
		}
		try{
			m_pcDaoDatabase->Close();
			delete(m_pcDaoDatabase);
		}
		catch (CDaoException* e){
			e->Delete();
		}
		break;
	case DBMDBQ:
		if(m_nDBOpen == FALSE){
			return;
		}
		try{
			m_pcDaoDatabase->Close();
			delete(m_pcDaoDatabase);
		}
		catch (CDaoException* e){
			e->Delete();
		}
		break;
	case DBXLS:
		m_cExcelLib.CloseSheet();
		m_cExcelLib.Close();
		break;
	}
}

int CDaoDBLib::InitGetFild()
{
	int			len;
//	int			idx;
	CString		csStr;
	int			fildno;
	COleVariant		var;
	CString			strRet;
	CDaoFieldInfo	cDaoFieldInfo;

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	if(m_lpDBBuf == NULL){
		return(FALSE);
	}
	switch(m_nType){
	case DBCSV:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		if(fgets(m_lpDBBuf, TEMPBUFMAX2, m_fpCSV) == NULL){
			m_nDBOpen = FALSE;
			fclose(m_fpCSV);
			return(FALSE);
		}
		len = strlen(m_lpDBBuf);
		if(len >= TEMPBUFMAX2){
			::AfxMessageBox("１行が長すぎるため処理できませんでした", MB_OK);
			m_nDBOpen = FALSE;
			fclose(m_fpCSV);
			return(FALSE);
		}
		m_nFildMax = DivCsvSetSp(m_lpCsvsp, m_lpDBBuf);
		break;
	case DBMDBT:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		csStr.Format("[%s]", m_csTableName);
		try{
			m_pcTableDef = new CDaoTableDef(m_pcDaoDatabase);
			m_pcTableDef->Open(csStr);
			m_nFildMax = m_pcTableDef->GetFieldCount();
		}
		catch (CDaoException* e1){
			e1->Delete();
			m_nDBOpen = FALSE;
			m_pcDaoDatabase->Close();
			delete(m_pcDaoDatabase);
			m_pcDaoDatabase = NULL;
			return(FALSE);
		}
		m_nDBOpen = TRUE;
		break;
	case DBMDBQ:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		csStr.Format("%s", m_csTableName);
		try{
			m_pcQueryDef = new CDaoQueryDef(m_pcDaoDatabase);
			m_pcQueryDef->Open(csStr);
			m_nFildMax = m_pcQueryDef->GetFieldCount();
		}
		catch (CDaoException* e){
			e->Delete();
			m_nDBOpen = FALSE;
			m_pcDaoDatabase->Close();
			delete(m_pcDaoDatabase);
			m_pcDaoDatabase = NULL;
			return(FALSE);
		}
		m_nDBOpen = TRUE;
		break;
	case DBXLS:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		for(fildno = 0; fildno < EXCELCLMMAX; fildno++){
			m_cExcelLib.GetCell(fildno+1, 1, pcApp->m_lpTempStr);
			if(pcApp->m_lpTempStr[0] != '\0'){
				m_nFildMax = fildno+1;
			}
		}
		break;
	}
	return(TRUE);
}

void CDaoDBLib::EndGetFild()
{
	if(m_nDBOpen == FALSE){
		return;
	}
	switch(m_nType){
	case DBCSV:
		if(m_nDBOpen == FALSE){
			return;
		}
		break;
	case DBMDBT:
		if(m_nDBOpen == FALSE){
			return;
		}
		try{
			m_pcTableDef->Close();
			delete(m_pcTableDef);
		}
		catch (CDaoException* e){
			e->Delete();
		}
		break;
	case DBMDBQ:
		if(m_nDBOpen == FALSE){
			return;
		}
		try{
			m_pcQueryDef->Close();
			delete(m_pcQueryDef);
		}
		catch (CDaoException* e){
			e->Delete();
		}
		break;
	case DBXLS:
		break;
	}
}

int CDaoDBLib::InitGetRecord()
{
	CString		csStr;

	if(m_lpDBBuf == NULL){
		return(FALSE);
	}
	switch(m_nType){
	case DBCSV:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		if(fgets(m_lpDBBuf, TEMPBUFMAX2, m_fpCSV) == NULL){
			m_nDBOpen = FALSE;
			fclose(m_fpCSV);
			return(FALSE);
		}
		m_nFildMax = DivCsvSetSp(m_lpCsvsp, m_lpDBBuf);
		break;
	case DBMDBT:
	case DBMDBQ:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		csStr.Format("select * from [%s]", m_csTableName);
		try{
			m_pcDaoRecordset = new CDaoRecordset(m_pcDaoDatabase);
			m_pcDaoRecordset->Open(dbOpenDynaset, csStr);
		}
		catch (CDaoException* e2){
			e2->Delete();
			m_nDBOpen = FALSE;
			m_pcDaoDatabase->Close();
			delete(m_pcDaoDatabase);
			return(FALSE);
		}
		if(m_pcDaoRecordset->IsBOF() && m_pcDaoRecordset->IsEOF()){
			m_nDBOpen = FALSE;
			return(FALSE);
		}
		m_pcDaoRecordset->MoveFirst();
		break;
	case DBXLS:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		m_nCrtLine = 2;
		break;
	}
	return(TRUE);
}

void CDaoDBLib::EndGetRecord()
{
	if(m_nDBOpen == FALSE){
		return;
	}
	switch(m_nType){
	case DBCSV:
		if(m_nDBOpen == FALSE){
			return;
		}
		break;
	case DBMDBT:
	case DBMDBQ:
		if(m_nDBOpen == FALSE){
			return;
		}
		try{
			m_pcDaoRecordset->Close();
			delete(m_pcDaoRecordset);
		}
		catch (CDaoException* e){
			e->Delete();
		}
		break;
	case DBXLS:
		break;
	}
}

int CDaoDBLib::GetFildName(int fildno, LPSTR lpFildName)
{
	CDaoFieldInfo	fi;
	COleVariant		var;
	CString			strRet;

	lpFildName[0] = '\0';
	if(fildno < 0 || m_nFildMax <= fildno){
		return(FALSE);
	}
	switch(m_nType){
	case DBCSV:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		strcpy(lpFildName, m_lpCsvsp[fildno]);
		break;
	case DBMDBT:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		try{
			m_pcTableDef->GetFieldInfo(fildno, fi);
		}
		catch (CDaoException* e){
			e->Delete();
		}
		strcpy(lpFildName, fi.m_strName.GetBuffer(DBNAMEMAX));
		break;
	case DBMDBQ:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		try{
			m_pcQueryDef->GetFieldInfo(fildno, fi);
		}
		catch (CDaoException* e){
			e->Delete();
		}
		strcpy(lpFildName, fi.m_strName.GetBuffer(DBNAMEMAX));
		break;
	case DBXLS:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		m_cExcelLib.GetCell(fildno+1, 1, lpFildName);
		break;
	}
	if(lpFildName[0] == '\0'){
		sprintf(lpFildName, "項目%d", fildno);
	}
	return(TRUE);
}

int CDaoDBLib::SetNextRecord()
{
	int		len;
//	char	msg[QRECMAX];

	if(m_lpDBBuf == NULL){
		return(FALSE);
	}
	switch(m_nType){
	case DBCSV:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		if(fgets(m_lpDBBuf, TEMPBUFMAX2, m_fpCSV) == NULL){
			return(FALSE);
		}
		len = strlen(m_lpDBBuf);
		if(len >= TEMPBUFMAX2-16){
			::AfxMessageBox("１行が長すぎるため処理できません\n読込みを中断します", MB_OK);
			return(FALSE);
		}
		m_nFildMax = DivCsvSetSp(m_lpCsvsp, m_lpDBBuf);
		break;
	case DBMDBT:
	case DBMDBQ:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		try{
			m_pcDaoRecordset->MoveNext();
		}
		catch (CDaoException* e){
			e->Delete();
			return(FALSE);
		}
		if(m_pcDaoRecordset->IsEOF()){
			return(FALSE);
		}
		break;
	case DBXLS:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		m_nCrtLine++;
		break;
	}
	return(TRUE);
}

int CDaoDBLib::GetItemStr(int fildno, LPSTR lpStr)
{
	COleVariant		var;
	CString			strRet;

	switch(m_nType){
	case DBCSV:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		strncpy(lpStr, m_lpCsvsp[fildno], RECMAX8);
		break;
	case DBMDBT:
	case DBMDBQ:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		try{
			var = m_pcDaoRecordset->GetFieldValue(fildno);
		}
		catch (CDaoException* e){
			e->Delete();
			return(FALSE);
		}
		strRet = ConversionVerToCStr(var);
		strncpy(lpStr, strRet.GetBuffer(RECMAX8), RECMAX8);
		break;
	case DBXLS:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		m_cExcelLib.GetCell(fildno+1, m_nCrtLine, lpStr);
		break;
	}
	return(TRUE);
}

int CDaoDBLib::DivCsvSetSp(LPSTR csvsp[], LPSTR csvbuf)
{
	int		len;
	int		i, max;
	UINT	hc;
	LPSTR	sp;
	LPSTR	dstsp;
	int		nDoubleQ;

	len = strlen(csvbuf);
	if(csvbuf[len-1] == '\n'){
		csvbuf[len-1] = '\0';
	}

	sp = csvbuf;
	i = 0;
	while(1){
		while(*sp){
			hc = (*sp) & 0x00ff;
			if(IsKanji(hc) == ON){
				break;
			}else if(hc != 0x20 && hc != 0x09){
				break;
			}
			sp++;
		}

		if(*sp == '\0'){
			break;
		}

		if(*sp == '\"'){
			nDoubleQ = TRUE;
			sp++;
		}else{
			nDoubleQ = FALSE;
		}

		csvsp[i] = sp;
		dstsp = sp;
		while(*sp){
			hc = (*sp) & 0x00ff;
			if(IsKanji(hc) == ON){
				*dstsp++ = *sp++;
				*dstsp++ = *sp++;
			}else{
				if(nDoubleQ == TRUE){
					if(hc == '\"'){
						if(*(sp+1) == '\"'){
							*dstsp++ = *sp++;
							sp++;
						}
						else{
							*dstsp++ = '\0';
							sp++;
							while(*sp){
								hc = (*sp) & 0x00ff;
								if(IsKanji(hc) == ON){
									sp += 2;
								}
								else{
									if(hc == ','){
										*sp = '\0';
										sp++;
										break;
									}
									sp++;
								}
							}
							break;
						}
					}else{
						*dstsp++ = *sp++;
					}
				}
				else if(hc == ','){
					*dstsp = '\0';
					sp++;
					break;
				}
				else{
					*dstsp++ = *sp++;
				}
			}
			//::AfxMessageBox(sp, MB_OK);
		}
		i++;
	}
	max = i;
	for(i = 0; i < max; i++){
		baselastspacedel(csvsp[i]);
	}
	return(i);
}

CString CDaoDBLib::ConversionVerToCStr(COleVariant var)
{
	CString	strRet;
	float	fnum;
	DBL		dnum;

	switch(var.vt){
	case VT_EMPTY:
	case VT_NULL:
		strRet = _T("");
		break;
	case VT_I2:
		strRet.Format(_T("%hd"),V_I2(&var));
		break;
	case VT_I4:
		strRet.Format(_T("%d"),V_I4(&var));
		break;
	case VT_R4:
		fnum = (float)V_R4(&var);
		if(fnum >= 0){
			fnum = fnum + (float)0.000001;
		}else{
			fnum = fnum - (float)0.000001;
		}
		strRet.Format(_T("%1.1f"), fnum);
		break;
	case VT_R8:
		dnum = (double)V_R8(&var);
		if(dnum >= 0){
			dnum += 0.00000001;
		}else{
			dnum -= 0.00000001;
		}
		strRet.Format(_T("%1.1lf"), dnum);
		break;
	case VT_CY:
		strRet = COleCurrency(var).Format();
		break;
	case VT_DATE:
		strRet = COleDateTime(var).Format(_T("%m %d %y"));
		break;
	case VT_BSTR:
		strRet = V_BSTRT(&var);
		break;
	case VT_DISPATCH:
		strRet = _T("VT_DISPATCH");
		break;
	case VT_ERROR:
		strRet = _T("VT_ERROR");
		break;
	case VT_BOOL:
		strRet.Format(_T("%d"), V_BOOL(&var));
		break;
	case VT_VARIANT:
		strRet = _T("VT_VARIANT");
		break;
	case VT_UNKNOWN:
		strRet = _T("VT_UNKNOWN");
		break;
	case VT_I1:
		strRet = _T("VT_I1");
		break;
	case VT_UI1:
		strRet.Format(_T("0x%02hX"),(unsigned short)V_UI1(&var));
		break;
	case VT_UI2:
		strRet = _T("VT_UI2");
		break;
	case VT_UI4:
		strRet = _T("VT_UI4");
		break;
	case VT_I8:
		strRet = _T("VT_I8");
		break;
	case VT_UI8:
		strRet = _T("VT_UI8");
		break;
	case VT_INT:
		strRet = _T("VT_INT");
		break;
	case VT_UINT:
		strRet = _T("VT_UINT");
		break;
	case VT_VOID:
		strRet = _T("VT_VOID");
		break;
	case VT_HRESULT:
		strRet = _T("VT_HRESULT");
		break;
	case VT_PTR:
		strRet = _T("VT_PTR");
		break;
	case VT_SAFEARRAY:
		strRet = _T("VT_SAFEARRAY");
		break;
	case VT_CARRAY:
		strRet = _T("VT_CARRAY");
		break;
	case VT_USERDEFINED:
		strRet = _T("VT_USERDEFINED");
		break;
	case VT_LPSTR:
		strRet = _T("VT_LPSTR");
		break;
	case VT_LPWSTR:
		strRet = _T("VT_LPWSTR");
		break;
	case VT_FILETIME:
		strRet = _T("VT_FILETIME");
		break;
	case VT_BLOB:
		strRet = _T("VT_BLOB");
		break;
	case VT_STREAM:
		strRet = _T("VT_STREAM");
		break;
	case VT_STORAGE:
		strRet = _T("VT_STORAGE");
		break;
	case VT_STREAMED_OBJECT:
		strRet = _T("VT_STREAMED_OBJECT");
		break;
	case VT_STORED_OBJECT:
		strRet = _T("VT_STORED_OBJECT");
		break;
	case VT_BLOB_OBJECT:
		strRet = _T("VT_BLOB_OBJECT");
		break;
	case VT_CF:
		strRet = _T("VT_CF");
		break;
	case VT_CLSID:
		strRet = _T("VT_CLSID");
		break;
	default:
		strRet = _T("");
		break;
	}
	return(strRet);
}

int CDaoDBLib::IsKanji(UINT hc)
{
	if((0x0080 <= hc && hc <= 0x009f)
	|| (0x00e0 <= hc && hc <= 0x00ff)){
		return(ON);
	}else{
		return(OFF);
	}
}

int CDaoDBLib::CheckCsvFile(LPSTR lpFileName)
{
	FILE	*fp;
	char	buf[RECMAX];
	int		ret;
	UINT	ch;
	int		i;

	fp = fopen(lpFileName, "r");
	if(fp == NULL){
		return(FALSE);
	}
	while(1){
		ret = fread(buf, 1, RECMAX, fp);
		if(ret == 0){
			break;
		}
		for(i = 0; i < ret; i++){
			ch = (buf[i] & 0x00ff);
			if(ch < 0x0020){
				if(ch != TAB && ch != CR && ch != LF){
					fclose(fp);
					return(FALSE);
				}
			}
		}
		if(ret != RECMAX){
			break;
		}
	}
	fclose(fp);
	return(TRUE);
}
