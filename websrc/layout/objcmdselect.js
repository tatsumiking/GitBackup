
// Edit object branch function 

function fncCommandSelect(ev, nx, ny)
{
	var	idx;
	
	switch (ev)
	{
		case DOWNEVENT :
			idx = fncSrchObject(nx, ny);
			if(idx == -1){ 
				m_nObjCrtIdx = -1;
				m_objspCrt = null;
				if(!(m_nRectSX == 0 && m_nRectEX == 0 && m_nRectSY == 0 && m_nRectEY == 0)){
					fncSetCrtCommand(CMDNEWIMAGE);
					fncCommandExec(DOWNEVENT, nx, ny, 0);
				}
			}else{
				m_nObjCrtIdx = idx;
				m_objspCrt = m_aryObject[idx];
				if(m_objspCrt.m_nObjType == OBJFIGURE){
					fncSetCrtCommand(CMDSELECTFIGURE);
				}else{ // OBJTEXT;
					fncSetCrtCommand(CMDSELECTTEXT);
				}
				fncCommandExec(INITEVENT, nx, ny, 0);
				fncCommandExec(DOWNEVENT, nx, ny, 0);
			}
			break;
		case CLICKEVENT :
			idx = fncSrchObject(nx, ny);
			if(idx == -1){
				m_nObjCrtIdx = -1;
				m_objspCrt = null;
				if(!(m_nRectSX == 0 && m_nRectEX == 0 && m_nRectSY == 0 && m_nRectEY == 0)){
					fncSetCrtCommand(CMDSELECTPHOTO);
				}
			}else{
				m_nObjCrtIdx = idx;
				m_objspCrt = m_aryObject[idx];
				if(m_objspCrt.m_nObjType == OBJFIGURE){
					fncSetCrtCommand(CMDSELECTFIGURE);
				}else{ // OBJTEXT;
					fncSetCrtCommand(CMDSELECTTEXT);
				}
			}
			break;
	}
}

