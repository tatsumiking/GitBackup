
var m_inputtext;
// Text object generation operation processing function 

function fncCommandNewText(ev, nx, ny)
{
	var str;
	var sxy, exy;

	switch (ev)
	{
		case CLICKEVENT :
			fncUndoSave();
			if(m_CrtObject == null){
				m_CrtObject = 
				str = m_CrtObject.text;
				sxy = m_CrtObject.fncObjectGetAtrXY(0);
				exy = m_CrtObject.fncObjectGetAtrXY(1);

				sxy = fncTrnsXYMMToRltv(sxy);
				exy = fncTrnsXYMMToRltv(exy);
				if(m_CrtObject.yktt == YOKO){	
					hi = exy.y - sxy.y;
					wd = hi * 10;
				}else{
					hi = exy.x - sxy.x;
					wd = hi * 10;
				}
				m_inputtext = document.createElement("text");
				m_inputtext.type = "text";
				m_inputtext.innerHTML = str;						
				m_inputtext.style.font = hi + "px"+"'"+m_CrtObject.font+"'";
				m_inputtext.style.left = sxy.x+"px";
				m_inputtext.style.top = sxy.y+"px";
				m_inputtext.style.width = wd+"px";
				m_inputtext.style.height = hi+"px";
				cnvsLayout = document.getElementById("cnvsLayout");
				cnvsLayout.appendChild(m_inputtext);		
						
			}else if(fncSrchTextObject(nx, ny) != -1){

			}
			break;
		case KEYINEVENT :
			if(m_keyeventCrt.keyCode == KEYCODE_ENTER){

			}
			fncSetCrtCommand(CMDSELECT);
			break;
		case ENDEVENT:
			fncDeleteNullText();
			break;
	}
}

function fncCommandSelectText(ev, nx, ny)
{
	var objsp;
	var size, clr, sclr;
	var tf;
	var tfmt;
	var idx;
	var font, str;
	var cx,cy;
	var tx,ty;

	switch (ev)
	{
		case INITEVENT :
			m_objspCrt.DrawGraphicsClear();
			chkShadow.selected = m_objspCrt.m_bShadow;
			chkArch.selected = m_objspCrt.m_bArch;
			tf = m_objspCrt.GetBaseTextFild();
			tfmt = tf.defaultTextFormat;
			font = tfmt.font;
			cmbFontSelect.prompt = font;
			cmbFontSelect.invalidate();
			m_nAddX = m_objspCrt.x -nx;
			m_nAddY = m_objspCrt.y -ny;
			m_nRectSX = m_objspCrt.GetSX();
			m_nRectSY = m_objspCrt.GetSY();
			m_wdRect = m_objspCrt.GetDWidth();
			m_hiRect = m_objspCrt.GetDHeight();
			m_nRectEX = m_nRectSX + m_wdRect;
			m_nRectEY = m_nRectSY + m_hiRect;
			
			m_objspCrt.DrawRect();
			m_objspCrt.DrawRoundAnchor();
			m_objspCrt.DrawArchAnchor();
			break;
		case DOWNEVENT :
			tx = nx; ty = ny;
			m_bSelectRound = m_objspCrt.CheckRoundSelect(tx, ty);
			if(m_bSelectRound == false){
				if(m_objspCrt.CheckAreaSelect(tx, ty) == false){
					fncSetCrtCommand(CMDSELECT);
					fncCommandSelect(DOWNEVENT, nx, ny, 0);
					return;	
				}
				m_bSelectArch = m_objspCrt.CheckArchSelect(tx, ty);
			}
			fncUndoSave();
			if(m_bSelectRound == false && m_bSelectArch == false){
				m_nAddX = m_objspCrt.x - nx;
				m_nAddY = m_objspCrt.y - ny;
				m_nRectSX = m_objspCrt.GetSX();
				m_nRectSY = m_objspCrt.GetSY();
				m_wdRect = m_objspCrt.GetDWidth();
				m_hiRect = m_objspCrt.GetDHeight();
				m_nRectEX = m_nRectSX + m_wdRect;
				m_nRectEY = m_nRectSY + m_hiRect;
			}
			break;
		case MOVEEVENT :
			m_objspCrt.DrawGraphicsClear();
			if(m_bSelectArch == true){
				m_objspCrt.SetArchAnchor(nx, ny);
			}else if(m_bSelectRound == true){
				m_objspCrt.SetRoundAnchor(nx, ny);
			}else{
				m_objspCrt.x = nx + m_nAddX;
				m_objspCrt.y = ny + m_nAddY;
			}
			font = cmbFontSelect.prompt;
			clr = m_aryClrTable[m_nCrtClrIdx];
			sclr = m_aryShClrTable[m_nCrtClrIdx];
			tf = m_objspCrt.GetBaseTextFild();
			size = m_objspCrt.GetFontSize();
			str = tf.text;
			m_objspCrt.ResetTextField(font, size, str, clr, sclr);
			m_nRectSX = m_objspCrt.GetSX();
			m_nRectSY = m_objspCrt.GetSY();
			m_wdRect = m_objspCrt.GetDWidth();
			m_hiRect = m_objspCrt.GetDHeight();
			m_nRectEX = m_nRectSX + m_wdRect;
			m_nRectEY = m_nRectSY + m_hiRect;
			
			m_objspCrt.DrawRect();
			m_objspCrt.DrawRoundAnchor();
			m_objspCrt.DrawArchAnchor();
			break;
		case UPEVENT :
			m_objspCrt.DrawGraphicsClear();
			if(m_bSelectArch == true){
				m_objspCrt.SetArchAnchor(nx, ny);
			}else if(m_bSelectRound == true){
				m_objspCrt.SetRoundAnchor(nx, ny);
			}else{
				m_objspCrt.x = nx + m_nAddX;
				m_objspCrt.y = ny + m_nAddY;
			}
			font = cmbFontSelect.prompt;
			clr = m_aryClrTable[m_nCrtClrIdx];
			sclr = m_aryShClrTable[m_nCrtClrIdx];
			tf = m_objspCrt.GetBaseTextFild();
			size = m_objspCrt.GetFontSize();
			str = tf.text;
			m_objspCrt.ResetTextField(font, size, str, clr, sclr);
			m_nRectSX = m_objspCrt.GetSX();
			m_nRectSY = m_objspCrt.GetSY();
			m_wdRect = m_objspCrt.GetDWidth();
			m_hiRect = m_objspCrt.GetDHeight();
			m_nRectEX = m_nRectSX + m_wdRect;
			m_nRectEY = m_nRectSY + m_hiRect;
			
			m_objspCrt.DrawGraphicsClear();
			m_objspCrt.DrawRect();
			m_objspCrt.DrawRoundAnchor();
			m_objspCrt.DrawArchAnchor();
			break;
		case WHEELEVENT :
			m_objspCrt.DrawGraphicsClear();
			m_objspCrt.m_bShadow = chkShadow.selected;
			m_objspCrt.m_bArch = chkArch.selected;
			font = cmbFontSelect.prompt;
			clr = m_aryClrTable[m_nCrtClrIdx];
			sclr = m_aryShClrTable[m_nCrtClrIdx];
			tf = m_objspCrt.GetBaseTextFild();
			size = m_objspCrt.GetFontSize();
			str = tf.text;
			
			cx = (m_nRectSX + m_nRectEX) / 2;
			cy = (m_nRectSY + m_nRectEY) / 2;
			size = size + dlt;
			// m_objspCrt.m_dRound = m_objspCrt.m_dRound + dlt;
			m_objspCrt.ResetTextField(font, size, str, clr, sclr);
			// m_objspCrt.rotation = m_objspCrt.m_dRound;
			m_nRectSX = m_objspCrt.GetSX();
			m_nRectSY = m_objspCrt.GetSY();
			m_wdRect = m_objspCrt.GetDWidth();
			m_hiRect = m_objspCrt.GetDHeight();
			m_nRectEX = m_nRectSX + m_wdRect;
			m_nRectEY = m_nRectSY + m_hiRect;
			m_nRectSX = cx - m_wdRect / 2;
			m_nRectSY = cy - m_hiRect / 2;
			m_nRectEX = m_nRectSX + m_wdRect;
			m_nRectEY = m_nRectSY + m_hiRect;
			m_objspCrt.SetSX(m_nRectSX);
			m_objspCrt.SetSY(m_nRectSY);

			m_objspCrt.DrawRect();
			m_objspCrt.DrawRoundAnchor();
			m_objspCrt.DrawArchAnchor();
			break;
		case ENDEVENT:
			if(m_objspCrt != null){
				m_objspCrt.DrawGraphicsClear();
			}
			m_objspCrt = null;
			m_nRectSX = 0; m_nRectSY = 0;
			m_nRectEX = 0; m_nRectEY = 0;
			m_spriteSelectRect.graphics.clear();
			break;
	}
}

function fncDeleteNullText()
{
	var max, i;
	var objsp;
	
	max = m_aryObject.length;
	for(i = max-1; i >= 0; i--){
		objsp = m_aryObject[i];
		if(objsp.m_nObjType == OBJTEXT){
			tf = objsp.GetBaseTextFild();
			if(tf.text == ""){
				m_aryObject.splice(i, 1);
				m_sprEditArea.removeChild(objsp);
			}
		}
	}
}
