
function fncTableTextSetFunction()
{
    var txtSit1L1 = document.getElementById("txtSit1L1");
    txtSit1L1.onclick = fncOnClickSit1L1;
    var txtSit1L2 = document.getElementById("txtSit1L2");
    txtSit1L2.onclick = fncOnClickSit1L2;
    var txtSit1L3 = document.getElementById("txtSit1L3");
    txtSit1L3.onclick = fncOnClickSit1L3;
    var txtSit1L4 = document.getElementById("txtSit1L4");
    txtSit1L4.onclick = fncOnClickSit1L4;
    var txtSit1L5 = document.getElementById("txtSit1L5");
    txtSit1L5.onclick = fncOnClickSit1L5;
    var txtSit1L6 = document.getElementById("txtSit1L6");
    txtSit1L6.onclick = fncOnClickSit1L6;
    var txtSit1L7 = document.getElementById("txtSit1L7");
    txtSit1L7.onclick = fncOnClickSit1L7;

    var txtSit1R1 = document.getElementById("txtSit1R1");
    txtSit1R1.onclick = fncOnClickSit1R1;
    var txtSit1R2 = document.getElementById("txtSit1R2");
    txtSit1R2.onclick = fncOnClickSit1R2;
    var txtSit1R3 = document.getElementById("txtSit1R3");
    txtSit1R3.onclick = fncOnClickSit1R3;
    var txtSit1R4 = document.getElementById("txtSit1R4");
    txtSit1R4.onclick = fncOnClickSit1R4;
    var txtSit1R5 = document.getElementById("txtSit1R5");
    txtSit1R5.onclick = fncOnClickSit1R5;
    var txtSit1R6 = document.getElementById("txtSit1R6");
    txtSit1R6.onclick = fncOnClickSit1R6;
    var txtSit1R7 = document.getElementById("txtSit1R7");
    txtSit1R7.onclick = fncOnClickSit1R7;

    var txtSit2L1 = document.getElementById("txtSit2L1");
    txtSit2L1.onclick = fncOnClickSit2L1;
    var txtSit2L2 = document.getElementById("txtSit2L2");
    txtSit2L2.onclick = fncOnClickSit2L2;
    var txtSit2L3 = document.getElementById("txtSit2L3");
    txtSit2L3.onclick = fncOnClickSit2L3;
    var txtSit2L4 = document.getElementById("txtSit2L4");
    txtSit2L4.onclick = fncOnClickSit2L4;
    var txtSit2L5 = document.getElementById("txtSit2L5");
    txtSit2L5.onclick = fncOnClickSit2L5;
    var txtSit2L6 = document.getElementById("txtSit2L6");
    txtSit2L6.onclick = fncOnClickSit2L6;
    var txtSit2L7 = document.getElementById("txtSit2L7");
    txtSit2L7.onclick = fncOnClickSit2L7;

    var txtSit2R1 = document.getElementById("txtSit2R1");
    txtSit2R1.onclick = fncOnClickSit2R1;
    var txtSit2R2 = document.getElementById("txtSit2R2");
    txtSit2R2.onclick = fncOnClickSit2R2;
    var txtSit2R3 = document.getElementById("txtSit2R3");
    txtSit2R3.onclick = fncOnClickSit2R3;
    var txtSit2R4 = document.getElementById("txtSit2R4");
    txtSit2R4.onclick = fncOnClickSit2R4;
    var txtSit2R5 = document.getElementById("txtSit2R5");
    txtSit2R5.onclick = fncOnClickSit2R5;
    var txtSit2R6 = document.getElementById("txtSit2R6");
    txtSit2R6.onclick = fncOnClickSit2R6;
    var txtSit2R7 = document.getElementById("txtSit2R7");
    txtSit2R7.onclick = fncOnClickSit2R7;
}
function fncOnClickSit1L1(){ fncOnClickSitText(1,1,1); }
function fncOnClickSit1L2(){ fncOnClickSitText(1,1,2); }
function fncOnClickSit1L3(){ fncOnClickSitText(1,1,3); }
function fncOnClickSit1L4(){ fncOnClickSitText(1,1,4); }
function fncOnClickSit1L5(){ fncOnClickSitText(1,1,5); }
function fncOnClickSit1L6(){ fncOnClickSitText(1,1,6); }
function fncOnClickSit1L7(){ fncOnClickSitText(1,1,7); }

function fncOnClickSit1R1(){ fncOnClickSitText(1,2,1); }
function fncOnClickSit1R2(){ fncOnClickSitText(1,2,2); }
function fncOnClickSit1R3(){ fncOnClickSitText(1,2,3); }
function fncOnClickSit1R4(){ fncOnClickSitText(1,2,4); }
function fncOnClickSit1R5(){ fncOnClickSitText(1,2,5); }
function fncOnClickSit1R6(){ fncOnClickSitText(1,2,6); }
function fncOnClickSit1R7(){ fncOnClickSitText(1,2,7); }

function fncOnClickSit2L1(){ fncOnClickSitText(2,1,1); }
function fncOnClickSit2L2(){ fncOnClickSitText(2,1,2); }
function fncOnClickSit2L3(){ fncOnClickSitText(2,1,3); }
function fncOnClickSit2L4(){ fncOnClickSitText(2,1,4); }
function fncOnClickSit2L5(){ fncOnClickSitText(2,1,5); }
function fncOnClickSit2L6(){ fncOnClickSitText(2,1,6); }
function fncOnClickSit2L7(){ fncOnClickSitText(2,1,7); }

function fncOnClickSit2R1(){ fncOnClickSitText(2,2,1); }
function fncOnClickSit2R2(){ fncOnClickSitText(2,2,2); }
function fncOnClickSit2R3(){ fncOnClickSitText(2,2,3); }
function fncOnClickSit2R4(){ fncOnClickSitText(2,2,4); }
function fncOnClickSit2R5(){ fncOnClickSitText(2,2,5); }
function fncOnClickSit2R6(){ fncOnClickSitText(2,2,6); }
function fncOnClickSit2R7(){ fncOnClickSitText(2,2,7); }

function fncOnClickSitText(blkno,lrkind,sitno)
{
    var ta = null;
    var clsTbl;
    var nGestId, nSitGestId;
    var max, idx;

    var sitidx = sitno - 1;
    if(blkno == 1){
        ta = m_taLeft;
    }else if(blkno == 2){
        ta = m_taRight;
    }
    if(ta == null){
        return;
    }
    clsTbl = m_clsLayout.tlines[ta.nLine].tables[ta.nClum];
    if(lrkind == 1){
        aryGestId = clsTbl.left;
    }else if(lrkind == 2){
        aryGestId = clsTbl.right;
    }
    nSitGestId = aryGestId[sitidx];
    if(m_nSlctGestId != 0){
        tno = fncGetTNo(clsTbl);
        if(lrkind == 1){
            sno = sitidx + 1;
        }else{
            sno = 14 - sitidx; // idx=0->14 7->7
        }
        fncUpdateGestSit(m_nSlctGestId, tno, sno);
    }
    aryGestId[sitidx] = m_nSlctGestId;
    var chkInsert = document.getElementById("chkInsert");
    if(chkInsert.checked == true){
        if(m_nSlctGestId == 0){ // 上につめる
            fncUpdateGestSit(nSitGestId, 0, 0);
            nSitGestId = 0;
            max = aryGestId.length;
            for(idx = max-1; idx >= sitidx; idx--){
                nGestId = nSitGestId;
                nSitGestId = aryGestId[idx];
                aryGestId[idx] = nGestId;
            }
        }else{ // 下にづらす
            max = aryGestId.length;
            for(idx = sitidx+1; idx < max; idx++){
                nGestId = nSitGestId;
                nSitGestId = aryGestId[idx];
                aryGestId[idx] = nGestId;
            }
            fncUpdateGestSit(nSitGestId, 0, 0);
        }
    }else{
        fncUpdateGestSit(nSitGestId, 0, 0);
    }
    if(lrkind == 1){
        clsTbl.left = aryGestId;
    }else if(lrkind == 2){
        clsTbl.right = aryGestId;
    }

    m_nSlctGestId = 0;
    
    if(chkInsert.checked == true){
        max = aryGestId.length;
        for(idx = sitidx; idx < max; idx++){
            nGestId = aryGestId[idx];
            if(lrkind == 1){
                sno = idx + 1;
            }else{
                sno = 14 - idx; // idx=0->14 7->7
            }
            fncUpdateGestSit(nGestId, tno, sno);
        }
    }
    fncDispSitNameTable();
    fncInitGestList();
}

function fncUpdateGestSit(id, tno, sno)
{
    if(id == 0){
        return;
    }
    var idx = fncGetGestSitIndex(id);
    m_aryGestSit[idx].tno = tno;
    m_aryGestSit[idx].sno = sno;

    var dbnm = m_szHotelDB;
    sSql = "UPDATE ge"+m_sKonreiNo+" SET ";
    sSql = sSql+"tno="+tno+",sno="+sno;
    sSql = sSql+" WHERE (id="+id+");";
	var data = "dbnm="+dbnm+"&sql="+sSql;
	var fnc = fncUpdateGestSitCallback;
    sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncUpdateGestSitCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("配席のレコードの更新に失敗しました");
		return;
	}
}
function fncCmdExec(event, dx, dy)
{
    var max, idx;
    var clsTA;

    if(event == SETEVENT){
        fncCmdDebugPos(dx, dy);
        max = m_aryTableArea.length;
        for(idx = 0; idx < max; idx++){
            clsTA = m_aryTableArea[idx];
            if(clsTA.sx < dx && dx < clsTA.ex 
            && clsTA.sy < dy && dy < clsTA.ey){
                fncResetSitNames(clsTA);
                break;
             }
        }
    }
}
function fncResetSitNames(clsTA)
{
    if(m_taLeft == null){
        m_taLeft = clsTA;
    }else if(m_taRight == null){
        if(m_taLeft != clsTA){
             m_taRight = clsTA;
        }
    }else{
        if(m_taRight != clsTA){
            m_taLeft = m_taRight;
            m_taRight = clsTA;
        }
    }
    fncDispSitNameTable();
}
function fncDispSitNameTable()
{
    if(m_taLeft == null){
        return;
    }
    var clsTbl = m_clsLayout.tlines[m_taLeft.nLine].tables[m_taLeft.nClum];
    var txtTableName1 = document.getElementById("txtTableName1");
    txtTableName1.value = clsTbl.name;
    fncSitGestName("txtSit1L1", clsTbl.left[0]);
    fncSitGestName("txtSit1L2", clsTbl.left[1]);
    fncSitGestName("txtSit1L3", clsTbl.left[2]);
    fncSitGestName("txtSit1L4", clsTbl.left[3]);
    fncSitGestName("txtSit1L5", clsTbl.left[4]);
    fncSitGestName("txtSit1L6", clsTbl.left[5]);
    fncSitGestName("txtSit1L7", clsTbl.left[6]);

    fncSitGestName("txtSit1R1", clsTbl.right[0]);
    fncSitGestName("txtSit1R2", clsTbl.right[1]);
    fncSitGestName("txtSit1R3", clsTbl.right[2]);
    fncSitGestName("txtSit1R4", clsTbl.right[3]);
    fncSitGestName("txtSit1R5", clsTbl.right[4]);
    fncSitGestName("txtSit1R6", clsTbl.right[5]);
    fncSitGestName("txtSit1R7", clsTbl.right[6]);
    if(m_taRight == null){
        return;
    }
    var clsTbl = m_clsLayout.tlines[m_taRight.nLine].tables[m_taRight.nClum];
    var txtTableName2 = document.getElementById("txtTableName2");
    txtTableName2.value = clsTbl.name;
    fncSitGestName("txtSit2L1", clsTbl.left[0]);
    fncSitGestName("txtSit2L2", clsTbl.left[1]);
    fncSitGestName("txtSit2L3", clsTbl.left[2]);
    fncSitGestName("txtSit2L4", clsTbl.left[3]);
    fncSitGestName("txtSit2L5", clsTbl.left[4]);
    fncSitGestName("txtSit2L6", clsTbl.left[5]);
    fncSitGestName("txtSit2L7", clsTbl.left[6]);

    fncSitGestName("txtSit2R1", clsTbl.right[0]);
    fncSitGestName("txtSit2R2", clsTbl.right[1]);
    fncSitGestName("txtSit2R3", clsTbl.right[2]);
    fncSitGestName("txtSit2R4", clsTbl.right[3]);
    fncSitGestName("txtSit2R5", clsTbl.right[4]);
    fncSitGestName("txtSit2R6", clsTbl.right[5]);
    fncSitGestName("txtSit2R7", clsTbl.right[6]);
}
function fncSitGestName(sCid, nGestId)
{
    var txtCnt = document.getElementById(sCid);
    if(nGestId == 0){
        txtCnt.value = "";
    }else{
        var idx = fncGetGestSitIndex(nGestId);
        var name = m_aryGestSit[idx].name;
        var ary = name.split(" ");
        txtCnt.value = ary[0];
    }
}
function fncGetGestSitIndex(nGestId)
{
    var max, idx;

    max = m_aryGestSit.length;
    for(idx = 0; idx < max; idx++){
        if(m_aryGestSit[idx].id == nGestId){
            return(idx);
        }
    }
    return(-1);
}
function fncCmdDebugPos(dx, dy)
{
	var sx, sy, ex, ey;

	sx = dx - m_dLimit;
	sy = dy - m_dLimit;
	ex = dx + m_dLimit;
	ey = dy + m_dLimit;
	sx = fncScrnTrnsMRX(sx);
	sy = fncScrnTrnsMRY(sy);
	ex = fncScrnTrnsMRX(ex);
	ey = fncScrnTrnsMRY(ey);
	fncCtxSetPen(m_ctxPaper, "#000000", 1);
	fncCtxLine(m_ctxPaper, sx, sy, ex, ey);
	fncCtxLine(m_ctxPaper, sx, ey, ex, sy);
}

function fncCtxSetPen(ctx, color, thick)
{
	ctx_sStrokeColor = color
	ctx_nStrokeThick = thick;
	ctx.strokeStyle = ctx_sStrokeColor;
	ctx.lineWidth = ctx_nStrokeThick;
}

function fncCtxLine(ctx, sx, sy, ex, ey)
{
	ctx.beginPath();
	ctx.moveTo(sx,sy);
	ctx.lineTo(ex,ey);
	ctx.stroke();
}
