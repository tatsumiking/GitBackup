// DaoDBLib.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DaoDBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	EXCELCLMMAX		256

//extern "C" {
#include "../../ObjLib/Baselib.h"
//}

IMPLEMENT_DYNCREATE(CDaoDBLib, CObject)

/////////////////////////////////////////////////////////////////////////////
// CDaoDBLib

CDaoDBLib::CDaoDBLib()
{
	m_lpDBBuf = (LPTSTR)new TCHAR[TEMPBUFMAX2];
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
		_tfopen_s(&m_fpCSV, m_csFileName.GetBuffer(FNAMEMAX), _T("r"));
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
		if(_fgetts(m_lpDBBuf, TEMPBUFMAX2, m_fpCSV) == NULL){
			m_nDBOpen = FALSE;
			fclose(m_fpCSV);
			return(FALSE);
		}
		len = _tcslen(m_lpDBBuf);
		if(len >= TEMPBUFMAX2){
			::AfxMessageBox(_T("１行が長すぎるため処理できませんでした"), MB_OK);
			m_nDBOpen = FALSE;
			fclose(m_fpCSV);
			return(FALSE);
		}
		m_lpDBBuf[len-1] = _T('\0');
		m_nFildMax = DivCsvSetSp(m_lpCsvsp, m_lpDBBuf);
		break;
	case DBMDBT:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		csStr.Format(_T("[%s]"), m_csTableName);
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
		csStr.Format(_T("%s"), m_csTableName);
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
			if(pcApp->m_lpTempStr[0] != _T('\0')){
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
	int			len;

	if(m_lpDBBuf == NULL){
		return(FALSE);
	}
	switch(m_nType){
	case DBCSV:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		if(_fgetts(m_lpDBBuf, TEMPBUFMAX2, m_fpCSV) == NULL){
			m_nDBOpen = FALSE;
			fclose(m_fpCSV);
			return(FALSE);
		}
		len = _tcslen(m_lpDBBuf);
		m_lpDBBuf[len-1] = _T('\0');
		
		m_nFildMax = DivCsvSetSp(m_lpCsvsp, m_lpDBBuf);
		break;
	case DBMDBT:
	case DBMDBQ:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		csStr.Format(_T("select * from [%s]"), m_csTableName);
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

int CDaoDBLib::GetFildName(int fildno, LPTSTR lpFildName)
{
	CDaoFieldInfo	fi;
	COleVariant		var;
	CString			strRet;
	int				len;

	lpFildName[0] = _T('\0');
	if(fildno < 0 || m_nFildMax <= fildno){
		return(FALSE);
	}
	switch(m_nType){
	case DBCSV:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		len = _tcslen(m_lpCsvsp[fildno]) + 1;
		_tcscpy_s(lpFildName, len, m_lpCsvsp[fildno]);
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
		len = fi.m_strName.GetLength()+1;
		_tcscpy_s(lpFildName, len, fi.m_strName.GetBuffer(DBNAMEMAX));
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
		len = fi.m_strName.GetLength()+1;
		_tcscpy_s(lpFildName, len, fi.m_strName.GetBuffer(DBNAMEMAX));
		break;
	case DBXLS:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		m_cExcelLib.GetCell(fildno+1, 1, lpFildName);
		break;
	}
	if(lpFildName[0] == _T('\0')){
		len = _tcslen(_T("項目00"))+1;
		_stprintf_s(lpFildName, len, _T("項目%d"), fildno);
	}
	return(TRUE);
}

int CDaoDBLib::SetNextRecord()
{
	int		len;
//	TCHAR	msg[QRECMAX];

	if(m_lpDBBuf == NULL){
		return(FALSE);
	}
	switch(m_nType){
	case DBCSV:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		if(_fgetts(m_lpDBBuf, TEMPBUFMAX2, m_fpCSV) == NULL){
			return(FALSE);
		}
		len = _tcslen(m_lpDBBuf);
		if(len >= TEMPBUFMAX2-16){
			::AfxMessageBox(_T("１行が長すぎるため処理できません\n読込みを中断します"), MB_OK);
			return(FALSE);
		}
		m_lpDBBuf[len-1] = _T('\0');
		
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

int CDaoDBLib::GetItemStr(int fildno, LPTSTR lpStr)
{
	COleVariant		var;
	CString			strRet;
	int				len;

	switch(m_nType){
	case DBCSV:
		if(m_nDBOpen == FALSE){
			return(FALSE);
		}
		len = _tcslen(m_lpCsvsp[fildno])+1;
		if(RECMAX8 < len){
			len = RECMAX8;
		}
		_tcscpy_s(lpStr, len, m_lpCsvsp[fildno]);
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
		len = strRet.GetLength()+1;
		if(RECMAX8 < len){
			len = RECMAX8;
		}
		_tcscpy_s(lpStr, len, strRet.GetBuffer(RECMAX8));
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

int CDaoDBLib::DivCsvSetSp(LPTSTR csvsp[], LPTSTR csvbuf)
{
	int		len;
	int		i, max;
	UINT	hc;
	LPTSTR	sp;
	LPTSTR	dstsp;
	int		nDoubleQ;

	len = _tcslen(csvbuf);
	if(csvbuf[len-1] == _T('\n')){
		csvbuf[len-1] = _T('\0');
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

		if(*sp == _T('\0')){
			break;
		}

		if(*sp == _T('\"')){
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
					if(hc == _T('\"')){
						if(*(sp+1) == _T('\"')){
							*dstsp++ = *sp++;
							sp++;
						}
						else{
							*dstsp++ = _T('\0');
							sp++;
							while(*sp){
								hc = (*sp) & 0x00ff;
								if(IsKanji(hc) == ON){
									sp += 2;
								}
								else{
									if(hc == _T(',')){
										*sp = _T('\0');
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
				else if(hc == _T(',')){
					*dstsp = _T('\0');
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

int CDaoDBLib::CheckCsvFile(LPTSTR lpFileName)
{
	FILE	*fp;
	TCHAR	buf[RECMAX];
	int		ret;
	UINT	ch;
	int		i;

	_tfopen_s(&fp, lpFileName, _T("r"));
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
