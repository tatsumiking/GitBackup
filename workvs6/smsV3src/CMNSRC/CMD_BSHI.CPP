
/////////////////////////////////////////////////////////////////////////////
// CCmdBoshi

#include "stdafx.h"
#include "math.h"

#include "dlinbshi.h"
#include "cmd_base.h"
#include "cmd_bshi.h"

extern "C" {
	void senvgetexepath(LPSTR path);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	void mojienvgetfontname(LPSTR fontname);
}

CCmdBoshi::CCmdBoshi(CScrollView* pcview)
:CCmdBase(pcview)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	char	buf[HRECMAX];
	FILE	*fp;

	m_nLineCount = 1;
	m_dLineSpace = 100;
	m_dSpace = 20;
	m_dWidth = 80;
	m_dHight = 300;
	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmd_bshi.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &m_nLineCount);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf", &m_dLineSpace);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf", &m_dSpace);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf", &m_dWidth);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf", &m_dHight);
		fclose(fp);
	}
}

CCmdBoshi::~CCmdBoshi()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmd_bshi.env", exepath);
	if((fp = fopen(fname, "w")) != NULL){
		fprintf(fp, "%d\n", m_nLineCount);
		fprintf(fp, "%4.1lf\n", m_dLineSpace);
		fprintf(fp, "%4.1lf\n", m_dSpace);
		fprintf(fp, "%4.1lf\n", m_dWidth);
		fprintf(fp, "%4.1lf\n", m_dHight);
		fclose(fp);
	}
}

UINT CCmdBoshi::ExecBoshiType1(UINT event, DBL dx, DBL dy)
{
	CDialogInnumBoshi	*pcDialogInnumBoshi;
	int		nfirst;
	DBL		xsize, ysize;
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	CTonDoc *pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	m_cPtrList.RemoveAll( );

	pcDialogInnumBoshi = new CDialogInnumBoshi(m_pcView);
	nfirst = ON;
	scrngetmmsize(&xsize, &ysize);
	m_dSY = ysize - 100;
	m_nSide = VCNTR;
	//m_nSide = LEFT;
	//m_nSide = RIGHT;

STEP1:
	pcDialogInnumBoshi->SetDialogTitle("•æŽ˜gƒ^ƒCƒv‚P");
	pcDialogInnumBoshi->SetInfoText("s”Ý’è");
	pcDialogInnumBoshi->SetTitleText("s”");
	pcDialogInnumBoshi->SetUnitText("s");
	pcDialogInnumBoshi->SetRetryText("I—¹");
	pcDialogInnumBoshi->m_dInputNum = m_nLineCount;
	ret = pcDialogInnumBoshi->DoModal();
	if(ret == IDOK){
		m_nLineCount = (int)(pcDialogInnumBoshi->m_dInputNum);
	}
	else if(ret == IDRETRY){
		goto ENDLABEL;
	}
	else if(ret == IDCANCEL){
		goto ENDLABEL;
	}

STEP2:
	pcDialogInnumBoshi->SetDialogTitle("•æŽ˜gƒ^ƒCƒv‚P");
	pcDialogInnumBoshi->SetInfoText("s‚Æs‚ÌŠÔŠu");
	pcDialogInnumBoshi->SetTitleText("ŠÔŠu");
	pcDialogInnumBoshi->SetUnitText("mm");
	pcDialogInnumBoshi->SetRetryText("–ß‚é");
	pcDialogInnumBoshi->m_dInputNum = m_dLineSpace;
	ret = pcDialogInnumBoshi->DoModal();
	if(ret == IDOK){
		m_dLineSpace = pcDialogInnumBoshi->m_dInputNum;
	}
	else if(ret == IDRETRY){
		goto STEP1;
	}
	else if(ret == IDCANCEL){
		goto ENDLABEL;
	}

STEP3:
	pcDialogInnumBoshi->SetDialogTitle("•æŽ˜gƒ^ƒCƒv‚P");
	pcDialogInnumBoshi->SetInfoText("•¶Žš•,˜g‚Ì•");
	pcDialogInnumBoshi->SetTitleText("•");
	pcDialogInnumBoshi->SetUnitText("mm");
	pcDialogInnumBoshi->SetRetryText("–ß‚é");
	pcDialogInnumBoshi->m_dInputNum = m_dWidth;
	ret = pcDialogInnumBoshi->DoModal();
	if(ret == IDOK){
		m_dWidth = pcDialogInnumBoshi->m_dInputNum;
	}
	else if(ret == IDRETRY){
		if(nfirst == ON){
			goto STEP2;
		}
		else{
			m_dSY += m_dSpace;
			goto STEP5;
		}
	}
	else if(ret == IDCANCEL){
		goto ENDLABEL;
	}
	if(nfirst == ON){
		m_dCX = (m_dLineSpace + m_dWidth) / 2;  
	}

	pcDialogInnumBoshi->SetDialogTitle("•æŽ˜gƒ^ƒCƒv‚P");
	pcDialogInnumBoshi->SetInfoText("˜g‚Ì‚‚³");
	pcDialogInnumBoshi->SetTitleText("‚‚³");
	pcDialogInnumBoshi->SetUnitText("mm");
	pcDialogInnumBoshi->SetRetryText("–ß‚é");
	pcDialogInnumBoshi->m_dInputNum = m_dHight;
	ret = pcDialogInnumBoshi->DoModal();
	if(ret == IDOK){
		m_dHight = pcDialogInnumBoshi->m_dInputNum;
	}
	else if(ret == IDRETRY){
		goto STEP3;
	}
	else if(ret == IDCANCEL){
		goto ENDLABEL;
	}

	CreateWaku();
	nfirst = OFF;
	m_dSY -= m_dHight;

STEP5:
	pcDialogInnumBoshi->SetDialogTitle("•æŽ˜gƒ^ƒCƒv‚P");
	pcDialogInnumBoshi->SetInfoText("‘O‚Ì˜g‚Æ‚ÌŠÔŠuÝ’è");
	pcDialogInnumBoshi->SetTitleText("ŠÔŠu");
	pcDialogInnumBoshi->SetUnitText("mm");
	pcDialogInnumBoshi->SetRetryText("I—¹");
	pcDialogInnumBoshi->m_dInputNum = m_dSpace;
	ret = pcDialogInnumBoshi->DoModal();
	if(ret == IDOK){
		m_dSpace = pcDialogInnumBoshi->m_dInputNum;
		m_dSY -= m_dSpace;
		goto STEP3;
	}

ENDLABEL:
	ActiveDataRemoveAt();
	CreateWakuSort();
	ScrnRedraw();
	delete(pcDialogInnumBoshi);
	return(ENDEVENT);
}

UINT CCmdBoshi::ExecBoshiType2(UINT event, DBL dx, DBL dy)
{
	CDialogInnumBoshi	*pcDialogInnumBoshi;
	char	szInfo[QRECMAX];
	int		nfirst;
	DBL		xsize, ysize;
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	CTonDoc *pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	m_cPtrList.RemoveAll( );

	pcDialogInnumBoshi = new CDialogInnumBoshi(m_pcView);
	nfirst = ON;
	scrngetmmsize(&xsize, &ysize);
	m_dSY = ysize - 100;
	m_dCY = ysize - 100;
	m_nSide = VCNTR;
	//m_nSide = LEFT;
	//m_nSide = RIGHT;

STEP1:
	pcDialogInnumBoshi->SetDialogTitle("•æŽ˜gƒ^ƒCƒv‚Q");
	pcDialogInnumBoshi->SetInfoText("s”Ý’è");
	pcDialogInnumBoshi->SetTitleText("s”");
	pcDialogInnumBoshi->SetUnitText("s");
	pcDialogInnumBoshi->SetRetryText("I—¹");
	pcDialogInnumBoshi->m_dInputNum = m_nLineCount;
	ret = pcDialogInnumBoshi->DoModal();
	if(ret == IDOK){
		m_nLineCount = (int)(pcDialogInnumBoshi->m_dInputNum);
	}
	else if(ret == IDRETRY){
		goto ENDLABEL;
	}
	else if(ret == IDCANCEL){
		goto ENDLABEL;
	}

STEP2:
	pcDialogInnumBoshi->SetDialogTitle("•æŽ˜gƒ^ƒCƒv‚Q");
	pcDialogInnumBoshi->SetInfoText("s‚Æs‚ÌŠÔŠu");
	pcDialogInnumBoshi->SetTitleText("ŠÔŠu");
	pcDialogInnumBoshi->SetUnitText("mm");
	pcDialogInnumBoshi->SetRetryText("–ß‚é");
	pcDialogInnumBoshi->m_dInputNum = m_dLineSpace;
	ret = pcDialogInnumBoshi->DoModal();
	if(ret == IDOK){
		m_dLineSpace = pcDialogInnumBoshi->m_dInputNum;
	}
	else if(ret == IDRETRY){
		goto STEP1;
	}
	else if(ret == IDCANCEL){
		goto ENDLABEL;
	}

STEP3:
	pcDialogInnumBoshi->SetDialogTitle("•æŽ˜gƒ^ƒCƒv‚Q");
	if(m_nSide == VCNTR){
		strcpy(szInfo, "•¶Žš•,˜g‚Ì•");
	}
	else if(m_nSide == LEFT){
		strcpy(szInfo, "¶‘¤‚Ì•¶Žš•,˜g‚Ì•");
	}
	else if(m_nSide == RIGHT){
		strcpy(szInfo, "‰E‘¤‚Ì•¶Žš•,˜g‚Ì•");
	}
	pcDialogInnumBoshi->SetInfoText(szInfo);
	pcDialogInnumBoshi->SetTitleText("•");
	pcDialogInnumBoshi->SetUnitText("mm");
	pcDialogInnumBoshi->SetRetryText("–ß‚é");
	pcDialogInnumBoshi->m_dInputNum = m_dWidth;
	ret = pcDialogInnumBoshi->DoModal();
	if(ret == IDOK){
		m_dWidth = pcDialogInnumBoshi->m_dInputNum;
	}
	else if(ret == IDRETRY){
		if(nfirst == ON){
			goto STEP2;
		}
		else{
			m_dSY += m_dSpace;
			goto STEP5;
		}
	}
	else if(ret == IDCANCEL){
		goto ENDLABEL;
	}
	if(nfirst == ON){
		m_dCX = (m_dLineSpace + m_dWidth) / 2;  
	}

	pcDialogInnumBoshi->SetDialogTitle("•æŽ˜gƒ^ƒCƒv‚Q");
	if(m_nSide == VCNTR){
		strcpy(szInfo, "˜g‚Ì‚‚³");
	}
	else if(m_nSide == LEFT){
		strcpy(szInfo, "¶‘¤‚Ì˜g‚Ì‚‚³");
	}
	else if(m_nSide == RIGHT){
		strcpy(szInfo, "‰E‘¤‚Ì˜g‚Ì‚‚³");
	}
	pcDialogInnumBoshi->SetInfoText(szInfo);
	pcDialogInnumBoshi->SetTitleText("‚‚³");
	pcDialogInnumBoshi->SetUnitText("mm");
	pcDialogInnumBoshi->SetRetryText("–ß‚é");
	pcDialogInnumBoshi->m_dInputNum = m_dHight;
	ret = pcDialogInnumBoshi->DoModal();
	if(ret == IDOK){
		m_dHight = pcDialogInnumBoshi->m_dInputNum;
	}
	else if(ret == IDRETRY){
		goto STEP3;
	}
	else if(ret == IDCANCEL){
		goto ENDLABEL;
	}

	CreateWaku();
	if(m_nSide == VCNTR){
		m_nSide = LEFT;
		m_dSX = m_dCX - m_dWidth/2;
		m_dEX = m_dCX + m_dWidth/2;
		m_dCY -= m_dHight;
	}
	nfirst = OFF;
	m_dSY -= m_dHight;

STEP5:
	pcDialogInnumBoshi->SetDialogTitle("•æŽ˜gƒ^ƒCƒv‚Q");
	if(m_nSide == VCNTR){
		strcpy(szInfo, "‘O‚Ì˜g‚Æ‚ÌŠÔŠuÝ’è");
		pcDialogInnumBoshi->SetRetryText("I—¹");
	}
	else if(m_nSide == LEFT){
		strcpy(szInfo, "¶‘¤‚Ì‘O‚Ì˜g‚Æ‚ÌŠÔŠuÝ’è");
		pcDialogInnumBoshi->SetRetryText("‰EÝ’è");
	}
	else if(m_nSide == RIGHT){
		strcpy(szInfo, "‰E‘¤‚Ì‘O‚Ì˜g‚Æ‚ÌŠÔŠuÝ’è");
		pcDialogInnumBoshi->SetRetryText("I—¹");
	}
	pcDialogInnumBoshi->SetInfoText(szInfo);
	pcDialogInnumBoshi->SetTitleText("ŠÔŠu");
	pcDialogInnumBoshi->SetUnitText("mm");
	pcDialogInnumBoshi->m_dInputNum = m_dSpace;
	ret = pcDialogInnumBoshi->DoModal();
	if(ret == IDOK){
		m_dSpace = pcDialogInnumBoshi->m_dInputNum;
		m_dSY -= m_dSpace;
		goto STEP3;
	}
	else if(ret == IDRETRY){
		if(m_nSide == LEFT){
			m_nSide = RIGHT;
			m_dSY = m_dCY - m_dSpace;
			goto STEP3;
		}
	}

ENDLABEL:
	ActiveDataRemoveAt();
	CreateWakuSort();
	ScrnRedraw();
	delete(pcDialogInnumBoshi);
	return(ENDEVENT);
}

void CCmdBoshi::CreateWaku()
{
	CTonDoc		*pcDoc;
	CWakuVect	*pcWakuVect;
	char	fontname[FNAMEMAX];
	int		i;
	DBL		sx, sy;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	mojienvgetfontname(fontname);
	for(i = 0; i < m_nLineCount; i++){
		pcWakuVect = (CWakuVect*)pcDoc->NewData(WAKUVECTID);
		if(pcWakuVect == NULL){
			return;
		}
		pcWakuVect->SetFontSize(m_dWidth, m_dWidth);
		pcWakuVect->SetStrLen(m_dHight);
		pcWakuVect->SetTateGaki();
		sy = m_dSY;
		if(m_nSide == VCNTR){
			sx = m_dCX+m_dLineSpace*i-m_dWidth/2;
		}
		else if(m_nSide == LEFT){
			sx = m_dSX+m_dLineSpace*i;
		}
		else if(m_nSide == RIGHT){
			sx = m_dEX+m_dLineSpace*i-m_dWidth;
		}
		pcWakuVect->SetStrStartXY(sx, sy);
		pcWakuVect->SetFontName(fontname);
		m_cPtrList.AddTail(pcWakuVect);
		AddLinkDoc(pcWakuVect);
	}
	ScrnRedraw();
}

void CCmdBoshi::CreateWakuSort()
{
	POSITION	pos, ppos, tpos;
	CData	*pcData;
	DBL		minx,miny,maxx,maxy;
	DBL		tx, ty;
	CData	*pcTData;


	while(1){
		pos = m_cPtrList.GetHeadPosition();
		if(pos == NULL){
			break;
		}
		tx = 0;
		ty = 0;
		pcTData = NULL;
		while (pos != NULL)
		{
			ppos = pos;
			pcData = m_cPtrList.GetNext(pos);
			if(pcData == NULL){
				break;
			}
			pcData->GetDMiniMax(&minx, &miny, &maxx, &maxy);
			if(tx < maxx){
				tx = maxx;
				ty = maxy;
				pcTData = pcData;
				tpos = ppos;
			}
			else if(tx == maxx && ty < maxy){
				tx = maxx;
				ty = maxy;
				pcTData = pcData;
				tpos = ppos;
			}
		}
		m_cPtrList.RemoveAt(tpos);
		AddLinkDoc(pcTData);
	}	
}

void CCmdBoshi::ActiveDataRemoveAt()
{
	CData*		pcData;
	POSITION	pos, prepos;

	CTonDoc *pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pos = pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		prepos = pos;
		pcData = pcDoc->m_pcObjDataList->GetNext(pos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			pcDoc->m_pcObjDataList->RemoveAt(prepos);
		}
	}
}
