
function fncCommandNewFigure(ev, nx, ny)
{
	var bmpdata;
	var bmp;
	var objsp;
	var idx;

	switch (ev)
	{
	case INITEVENT:
		fncUndoSave();
		m_nRectSX = 0; m_nRectSY = 0;
		m_nRectEX = 0; m_nRectEY = 0;
	case SETEVENT :
		break;
	case ENDEVENT :
		fncSetCrtCommand(CMDSELECT);
		break;
	}
}

function fncCommandSelectFigure(ev, nx, ny)
{
	var cx, cy;
	var idx;

	switch (ev)
	{
		case DOWNEVENT :
			if(nx < m_nRectSX || m_nRectEX < nx
			|| ny < m_nRectSY || m_nRectEY < ny){
				fncSetCrtCommand(CMDSELECT);
				fncCommandSelect(DOWNEVENT, nx, ny, 0);
				return;	
			}
			fncUndoSave();
			m_nRectSX = m_objspCrt.x;
			m_nRectSY = m_objspCrt.y;
			m_nMouseAddX = m_nRectSX - nx;
			m_nMouseAddY = m_nRectSY - ny;
			m_wdRect = m_objspCrt.width;
			m_hiRect = m_objspCrt.height;
			m_nRectEX = m_nRectSX + m_wdRect;
			m_nRectEY = m_nRectSY + m_hiRect;
			fncDrawRect();
			break;
		case MOVEEVENT :
			m_nRectSX = nx + m_nMouseAddX;
			m_nRectSY = ny + m_nMouseAddY;
			m_nRectEX = m_nRectSX + m_wdRect;
			m_nRectEY = m_nRectSY + m_hiRect;
			fncDrawRect();
			m_objspCrt.x = nx + m_nMouseAddX;
			m_objspCrt.y = ny + m_nMouseAddY;
			break;
		case UPEVENT :
			m_nRectSX = nx + m_nMouseAddX;
			m_nRectSY = ny + m_nMouseAddY;
			m_nRectEX = m_nRectSX + m_wdRect;
			m_nRectEY = m_nRectSY + m_hiRect;
			fncDrawRect();
			m_objspCrt.x = nx + m_nMouseAddX;
			m_objspCrt.y = ny + m_nMouseAddY;
			break;
		case ENDEVENT:
			m_objspCrt = null;
			m_nRectSX = 0; m_nRectSY = 0;
			m_nRectEX = 0; m_nRectEY = 0;
			fncMainDraw();
			break;
	}

}

function fncSetFigureBitmap(idx)
{
	m_nSelectLYFigure = idx;
	fncDrawLYFigureImage();
}


