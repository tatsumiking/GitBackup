
/////////////////////////////////////////////////////////////////////////////
// CCmdText

#include "stdafx.h"
#include "math.h"

#include "cmd_base.h"
#include "cmd_text.h"

extern "C" {
}

CCmdText::CCmdText(CScrollView* pcview)
:CCmdBase(pcview)
{
	//m_pcView = (CTonView*)pcview;
}

UINT CCmdText::ExecCreate(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		InitClass();
		break;
	case SETEVENT:
	case UPSETEVENT:
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdText::ExecType2(UINT event, DBL dx, DBL dy)
{
	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	return(ENDEVENT);
}

