
function fncLayoutSaveData()
{
}
function fncUndoSave()
{
}
function fncRedoSave()
{
}
function fncGetLayoutData()
{
	var max, idx;
	var obj;

	max = m_aryObject.length;
	for (idx = 0; idx < max; idx++){
		obj = m_aryObject[idx];
		if(objsp.m_nObjType == OBJFIGURE){
			if(objsp.m_nSrcIdx == -1){
				filename = "";
			}else{
				filename = m_aryLYFigureName[objsp.m_nSrcIdx];
			}
			txtdata = txtdata + "FIGU," + filename;
			txtdata = txtdata + "," + objsp.x;
			txtdata = txtdata + "," + objsp.y;
			txtdata = txtdata + "," + objsp.scaleX;
			txtdata = txtdata + "," + objsp.scaleY;
			txtdata = txtdata + ",,,,,,,\r\n";
		}else{ // OBJTEXT;
			size = objsp.GetFontSize();
			tf = objsp.GetBaseTextFild();
			tfmt = tf.defaultTextFormat;
			par = objsp.m_dArchPar;
			txtdata = txtdata + "TEXT," + tf.text;
			txtdata = txtdata + "," + objsp.x;
			txtdata = txtdata + "," + objsp.y;
			txtdata = txtdata + "," + size;
			txtdata = txtdata + "," + tfmt.font;
			txtdata = txtdata + "," + tfmt.color;
			txtdata = txtdata + "," + objsp.m_bArch;
			txtdata = txtdata + "," + objsp.m_bShadow;
			txtdata = txtdata + "," + par;
			txtdata = txtdata + "," + objsp.m_dRound;
			txtdata = txtdata + ",\r\n";
		}
	}
	return(txtdata);
}

function onSelectedLayoutSave(evt){
	txtInfo.text = "Layout preservation is started.";
}

function onProgressLayoutSave(evt:ProgressEvent){
	txtInfo.text = "Under layout preservation";
}

function onCompleteLayoutSave(evt){
	txtInfo.text = "";
}
function onCancelLayoutSave(evt){
	txtInfo.text = "Layout preservation was canceled.";
}
function onIOErrorLayoutSave(evt){
	txtInfo.text = "Layout preservation was interrupted for the error.";
}

function onSecurityErrorLayoutSave(evt){
	txtInfo.text = "Layout preservation is forbidden.";
}

