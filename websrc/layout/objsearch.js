
// オブジェクトサーチ関係関数

function fncSrchObject(nx, ny)
{
	var str;
	var max,i;
	var objsp;
	var ret = Boolean;
	var sx,sy,ex,ey;
	var wd,hi;

	max = m_aryObject.length;
	for (i = 0; i < max; i++)
	{
		objsp = m_aryObject[i];
		sx = objsp.GetSX();
		sy = objsp.GetSY();
		hi = objsp.GetDHeight();
		wd = objsp.GetDWidth();
		ex = sx + wd;
		ey = sy + hi;
		
		if(objsp.m_nObjType ==  OBJTEXT){
			ret = objsp.CheckAreaSelect(nx, ny);
			if(ret == true){
				m_nRectSX = sx; m_nRectSY = sy;
				m_nRectEX = ex; m_nRectEY = ey;
				return (i);
			}
		}else{
			if (sx < nx && nx < ex && sy < ny && ny < ey)
			{
				m_nRectSX = sx; m_nRectSY = sy;
				m_nRectEX = ex; m_nRectEY = ey;
				return (i);
			}
		}
	}
	if (m_sprPhoto.numChildren == 1){
		sx = m_sprPhoto.x;
		sy = m_sprPhoto.y;
		hi = m_sprPhoto.height;
		wd = m_sprPhoto.width;
		ex = sx + wd;
		ey = sy + hi;
		if (sx < nx && nx < ex && sy < ny && ny < ey)
		{
			m_nRectSX = sx; m_nRectSY = sy;
			m_nRectEX = ex; m_nRectEY = ey;
			return (-1);
		}
	}
	m_nRectSX = 0; m_nRectSY = 0;
	m_nRectEX = 0; m_nRectEY = 0;
	m_spriteSelectRect.graphics.clear();
	return (-1);
}

function fncSrchTextObject(nx, ny)
{
	var str;
	var max,i;
	var objsp;
	var sx,sy,ex,ey;
	var wd,hi;

	m_nObjCrtIdx = -1;
	max = m_aryObject.length;
	for (i = 0; i < max; i++)
	{
		objsp = m_aryObject[i];
		if(objsp.m_nObjType == OBJTEXT && objsp.m_dRound == 0){
			sx = objsp.GetSX();
			sy = objsp.GetSY();
			hi = objsp.GetDHeight();
			wd = objsp.GetDWidth();
			ex = sx + wd;
			ey = sy + hi;
			if (sx < nx && nx < ex && sy < ny && ny < ey)
			{
				m_nObjCrtIdx = i;
				m_objspCrt = objsp;
				return (i);
			}
		}
	}
	return(-1);
}

function fncSrchLYTemplateIdx(szName){
	var max,i;

	max = m_aryLYTemplateName.length;
	for(i = 0; i < max; i++){
		if(m_aryLYTemplateName[i] == szName){
			return(i);
		}
	}
	return(-1);
}

function fncSrchLYFigureIdx(szName){
	var max,i;

	max = m_aryLYFigureName.length;
	for(i = 0; i < max; i++){
		if(m_aryLYFigureName[i] == szName){
			return(i);
		}
	}
	return(-1);
}

function fncSrchColorIdx(clr){
	var max,i;

	max = m_aryClrTable.length;
	for(i = 0; i < max; i++){
		if(m_aryClrTable[i] == clr){
			return(i);
		}
	}
	return(-1);
}

