
// Photograph operation relation function 

function fncCommandSelectImage(ev, nx, ny)
{
	var ddlt;
	var cx, cy;

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
			m_nRectSX = m_sprPhoto.x;
			m_nRectSY = m_sprPhoto.y;
			m_nMouseAddX = m_nRectSX - nx;
			m_nMouseAddY = m_nRectSY - ny;
			m_wdRect = m_sprPhoto.width;
			m_hiRect = m_sprPhoto.height;
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
			m_sprPhoto.x = nx + m_nMouseAddX;
			m_sprPhoto.y = ny + m_nMouseAddY;
			break;
		case UPEVENT :
			m_nRectSX = nx + m_nMouseAddX;
			m_nRectSY = ny + m_nMouseAddY;
			m_nRectEX = m_nRectSX + m_wdRect;
			m_nRectEY = m_nRectSY + m_hiRect;
			fncDrawRect();
			m_sprPhoto.x = nx + m_nMouseAddX;
			m_sprPhoto.y = ny + m_nMouseAddY;
			break;
		case WHEELEVENT :
			ddlt = dlt;
			ddlt *=  0.01;
			
			m_sprPhoto.scaleX = m_sprPhoto.scaleX + ddlt;
			m_sprPhoto.scaleY = m_sprPhoto.scaleY + ddlt;
			if (m_sprPhoto.scaleX <= 0.01 || m_sprPhoto.scaleY <= 0.01)
			{
				m_sprPhoto.scaleX = m_sprPhoto.scaleX - ddlt;
				m_sprPhoto.scaleY = m_sprPhoto.scaleY - ddlt;
			}
			m_wdRect = m_sprPhoto.width;
			m_hiRect = m_sprPhoto.height;
			cx = (m_nRectSX + m_nRectEX) / 2;
			cy = (m_nRectSY + m_nRectEY) / 2;
			m_nRectSX = cx - m_wdRect / 2;
			m_nRectSY = cy - m_hiRect / 2;
			m_nRectEX = m_nRectSX + m_wdRect;
			m_nRectEY = m_nRectSY + m_hiRect;
			fncDrawRect();
			m_sprPhoto.x = m_nRectSX;
			m_sprPhoto.y = m_nRectSY;
			break;
		case ENDEVENT:
			m_objspCrt = null;
			m_nRectSX = 0; m_nRectSY = 0;
			m_nRectEX = 0; m_nRectEY = 0;
			m_spriteSelectRect.graphics.clear();
			break;
	}
}

