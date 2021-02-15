
var m_cnvsPaper;
var m_ctxPaper;

var m_cnvsWidth = 0;
var m_cnvsHeight = 0;
var m_fs = 0;
var m_dTime = 0.0;

var m_dPrnMMWidth = 0.0;
var m_dPrnMMHeight = 0.0;

var m_dPrnTblLeft = 0.0;
var m_dPrnTblTop = 0.0;
var m_dPrnTblAreaXSize = 0.0;
var m_dPrnTblAreaYSize = 0.0;

var A3XSIZE=420.0;
var A3YSIZE=297.0;

var A4XSIZE=297.0;
var A4YSIZE=210.0;

var B4XSIZE=364.0;
var B4YSIZE=257.0;

var TABLEAREASIZE = 1000.0;

function fncDrawPaperCanvas()
{
	var xsize = 0;
	var ysize = 0;

	m_cnvsPaper = document.getElementById("cnvsPaper");
	m_ctxPaper = cnvsPaper.getContext('2d');

	if(m_sPaperSize == "Ｂ４"){
		xsize = B4XSIZE;
		ysize = B4YSIZE;
	}else if(m_sPaperSize == "Ａ３"){
		xsize = A3XSIZE;
		ysize = A3YSIZE;
	}else{ // Ａ４
		xsize = A4XSIZE;
		ysize = A4YSIZE;
	}
	if(m_sPaperLocate == "横置き"){
		m_dPrnMMWidth = xsize;
		m_dPrnMMHeight = ysize;
	}else{ // 縦置き
		m_dPrnMMWidth = ysize;
		m_dPrnMMHeight = xsize;
	}
	m_dTime = 10.0;
	m_cnvsWidth = m_dPrnMMWidth * m_dTime;
	m_cnvsHeight = m_dPrnMMHeight * m_dTime;
	m_cnvsPaper.width = m_cnvsWidth;
	m_cnvsPaper.height = m_cnvsHeight;

	m_dPrnTblLeft = m_dPrnTblLeftSp;
	m_dPrnTblTop = m_dPrnTksTopSp + m_dPrnTksHeight + m_dPrnTblTopSp;
	m_dPrnTblAreaXSize = m_dPrnMMWidth - m_dPrnTblLeftSp - m_dPrnTblRightSp;
	m_dPrnTblAreaYSize = m_dPrnMMHeight - m_dPrnTblTop - m_dPrnTblBottomSp;

	fncSekijiKameiArea(m_ctxPaper);
	fncSekijiTakasagoArea(m_ctxPaper);
	fncSekijiTableArea(m_ctxPaper);
	fncSekijiCommentArea(m_ctxPaper);

	print();
}
function fncSekijiKameiArea(ctx)
{
	var sx, sy, ex, ey;
	var strStr;

	if ((m_nRyoukeKind == 1) || (m_nRyoukeKind == 2) || (m_nRyoukeKind == 5)) {
		strStr = "家";
	} else if ((m_nRyoukeKind == 3) || (m_nRyoukeKind == 4)) {
		strStr = "様";
	} else{
		strStr = "";
	}

	sx = m_dPrnKmiSX;
	sy = m_dPrnKmiSY;
	ex = sx + m_dPrnKmiSize * 3;
	ey = sy + m_dPrnKmiSize;
	fncYStrInBox(spr, sx, sy, ex, ey, m_strSinroName1);
	if(strStr != ""){
		sx = ex + m_dPrnKmiNSSp;
		ex = sx + m_dPrnKmiSize;
		fncYStrInBox(spr, sx, sy, ex, ey, strStr);
	}
	if ((m_nRyoukeKind == 2) || (m_nRyoukeKind == 4)) {
		sx = ex + m_dPrnKmiKKSp;
		sy = m_dPrnKmiSY
	}else{
		sx = m_dPrnKmiSX;
		sy = m_dPrnKmiSY+m_dPrnKmiSize+m_dPrnKmiKKSp;
	}
	ex = sx + m_dPrnKmiSize * 3;
	ey = sy + m_dPrnKmiSize;
	fncYStrInBox(spr, sx, sy, ex, ey, m_strSinpuName1);
	if(strStr != ""){
		sx = ex + m_dPrnKmiNSSp;
		ex = sx + m_dPrnKmiSize;
		fncYStrInBox(spr, sx, sy, ex, ey, strStr);
	}
	if(4 < m_nRyoukeKind){
		sx = ex + m_dPrnKmiKKSp;
		sy = m_dPrnKmiSY + (m_dPrnKmiSize + m_dPrnKmiNSSp) / 2;
		fncYStrOut(spr, sx, sy, m_dPrnKmiSize, m_sRyoukeKind);
	}

}
function fncSekijiTakasagoArea(ctx)
{
	switch(m_nTakasagoKind){
	case 0:
	case 1:
	case 2:
	case 5:
	case 6:
	case 7:
		// 横書き
		fncSekijiTakasagoAreaType1(ctx);
		break;
	case 3:
	case 4:
	case 8:
	case 9:
		// 縦書き
		fncSekijiTakasagoAreaType0(ctx);
		break;
	}
}

// 縦書き
function fncSekijiTakasagoAreaType0(ctx)
{
	var idx, cnt, i;
	var sx, sy, ex, ey, cx, cy;
	var strStr, strTStr;
	var aryStr = new Array();
	var divx, cntrx;

	divx = m_dPrnTksWidth / 8;
	cntrx = m_dPrnPaperWidth / 2.0;
	sx = (m_dPrnPaperWidth - m_dPrnTksWidth) / 2.0;
	ey = m_dPrnTksTopSp + m_dPrnTksHeight;
	ex = sx + m_dPrnTksWidth;
	sy = ey - 10.0;
	fncBoxOut(ctx, sx, sy, ex, ey);

	if(m_nTakasagoKind == 4
	|| m_nTakasagoKind == 9){
		ex = sx + m_dPrnTksWidth * 2.0 / 3.0;
		sx = sx + m_dPrnTksWidth / 3.0;
		sy = sy + 1.5;
		ey = ey - 1.5;
		fncYStrInBox(ctx, sx, sy, ex, ey, "高砂");
	}

	// 新郎
	cx = cntrx - divx;
	sx = cx - m_dPrnTksSinKSize / 2,0;
	ex = sx + m_dPrnTksSinKSize;
	sy = m_dPrnTksTopSp + m_dPrnTksSinTopSp;
	ey = sy + m_dPrnTksSinKSp + m_dPrnTksSinKSize * 2.0;
	fncTStrInBox(ctx, sx, sy, ex, ey, "新郎");

	sx = cx - m_dPrnTksSinNSize / 2,0;
	ex = sx + m_dPrnTksSinNSize;
	sy = ey + m_dPrnTksSinKNSp;
	ey = sy + m_dPrnTksSinNLen;
	fncTStrInBox(ctx, sx, sy, ex, ey, m_strSinroName2);

	// 新婦
	cx = cntrx + divx;
	sx = cx - m_dPrnTksSinKSize / 2,0;
	ex = sx + m_dPrnTksSinKSize;
	sy = m_dPrnTksTopSp + m_dPrnTksSinTopSp;
	ey = sy + m_dPrnTksSinKSp + m_dPrnTksSinKSize * 2.0;
	fncTStrInBox(ctx, sx, sy, ex, ey, "新婦");

	sx = cx - m_dPrnTksSinNSize / 2,0;
	ex = sx + m_dPrnTksSinNSize;
	sy = ey + m_dPrnTksSinKNSp;
	ey = sy + m_dPrnTksSinNLen;
	fncTStrInBox(ctx, sx, sy, ex, ey, m_strSinpuName2);

	if(m_nTakasagoKind == 8
	|| m_nTakasagoKind == 9){
		idx = m_arystrSitGestKubun.indexOf(m_strBsyk1Kubun, 0);
		cx = cntrx - divx*3;
		sx = cx - m_dPrnTksBskNSize / 2,0;
		ex = sx + m_dPrnTksBskNSize;
		sy = m_dPrnTksTopSp + m_dPrnTksBskTopSp;
		ey = sy + m_dPrnTksBskNLen;
		strStr = m_arystrSitGestName1[idx]+m_arystrSitGestName2[idx];
		fncTStrInBox(ctx, sx, sy, ex, ey, strStr);
		sy = ey + m_dPrnTksBskNSSp;
		ey = sy + m_dPrnTksBskNSize;
		fncTStrInBox(ctx, sx, sy, ex, ey, "様");
		// 肩書きを処理
		strStr = fncGetGestKtgk(idx);
		cnt = fncGetKtgkCnt(idx);
		aryStr = strStr.split(",");
		sx = cx + m_dPrnTksBskNSize / 2,0 +  m_dPrnTksBskKNSp;
		sx = sx + (m_dPrnTksBskKSize * Number(cnt-1));
		for (i = 0; i < cnt; i++) {
			sy = m_dPrnTksTopSp + m_dPrnTksBskTopSp;
			fncTStrOut(ctx, sx, sy, m_dPrnTksBskKSize, aryStr[i]);
			sx = sx - m_dPrnTksBskKSize;
		}
		// 御媒妁人夫人(御媒酌令夫人)
		idx = m_arystrSitGestKubun.indexOf(m_strBsyk2Kubun, 0);
		cx = cntrx + divx*3;
		sx = cx - m_dPrnTksBskNSize / 2,0;
		ex = sx + m_dPrnTksBskNSize;
		sy = m_dPrnTksTopSp + m_dPrnTksBskTopSp;
		ey = sy + m_dPrnTksBskNLen;
		strStr = m_arystrSitGestName1[idx]+m_arystrSitGestName2[idx];
		fncTStrInBox(ctx, sx, sy, ex, ey, strStr);
		sy = ey + m_dPrnTksBskNSSp;
		ey = sy + m_dPrnTksBskNSize;
		fncTStrInBox(ctx, sx, sy, ex, ey, "様");
		// 肩書きを処理
		strStr = fncGetGestKtgk(idx);
		cnt = fncGetKtgkCnt(idx);
		aryStr = strStr.split(",");
		sx = cx + m_dPrnTksBskNSize / 2,0 +  m_dPrnTksBskKNSp;
		sx = sx + (m_dPrnTksBskKSize * Number(cnt-1));
		for (i = 0; i < cnt; i++) {
			sy = m_dPrnTksTopSp + m_dPrnTksBskTopSp;
			fncTStrOut(ctx, sx, sy, m_dPrnTksBskKSize, aryStr[i]);
			sx = sx - m_dPrnTksBskKSize;
		}
	}
}
// 横書き
function fncSekijiTakasagoAreaType1(ctx)
{
	var idx, cnt, i;
	var sx, sy, ex, ey, cx, cy;
	var tsx, tex;
	var strStr, strTStr;
	var aryStr = new Array();
	var divx, cntrx;

	divx = m_dPrnTksWidth / 8;
	cntrx = m_dPrnPaperWidth / 2.0;
	
	if(m_nTakasagoKind == 1
	|| m_nTakasagoKind == 2
	|| m_nTakasagoKind == 6
	|| m_nTakasagoKind == 7){
		sx = (m_dPrnPaperWidth - m_dPrnTksWidth) / 2.0;
		ex = sx + m_dPrnTksWidth;
		ey = m_dPrnTksTopSp + m_dPrnTksHeight;
		sy = ey - 10.0;
		fncBoxOut(ctx, sx, sy, ex, ey);
	
		if(m_nTakasagoKind == 2
		|| m_nTakasagoKind == 7){
			ex = sx + m_dPrnTksWidth * 2.0 / 3.0;
			sx = sx + m_dPrnTksWidth / 3.0;
			sy = sy + 1.5;
			ey = ey - 1.5;
			fncYStrInBox(ctx, sx, sy, ex, ey, "高砂");
		}
	}else{	//m_nTakasagoKind == 0 m_nTakasagoKind == 5
		sx = cntrx;
		ex = cntrx;
		sy = m_dPrnTksTopSp;
		ey = sy + m_dPrnTksHeight; 
		fncLineOut(ctx, sx, sy, ex, ey);
		sx = cntrx - divx * 2;
		ex = cntrx + divx * 2;
		if(m_nTakasagoKind == 5){
			fncLineOut(ctx, sx, sy, sx, ey);
			fncLineOut(ctx, ex, sy, ex, ey);
			sx = cntrx - divx * 4;
			ex = cntrx + divx * 4;
		}
		fncBoxOut(ctx, sx, sy, ex, ey);
	}
	
	// 新郎
	sx = cntrx - divx - m_dPrnTksSinKSize;
	ex = sx + m_dPrnTksSinKSize*2;
	sy = m_dPrnTksTopSp + m_dPrnTksSinTopSp-m_dPrnTksSinNSize;
	ey = sy + m_dPrnTksSinKSize;
	fncYStrInBox(ctx, sx, sy, ex, ey, "新郎");

	sx = cntrx - divx - m_dPrnTksSinNLen / 2;
	ex = sx + m_dPrnTksSinNLen;
	sy = ey + m_dPrnTksSinKNSp;
	ey = sy + m_dPrnTksSinNSize;
	fncYStrInBox(ctx, sx, sy, ex, ey, m_strSinroName2);

	// 新婦
	sx = cntrx + divx - m_dPrnTksSinKSize;
	ex = sx + m_dPrnTksSinKSize * 2;
	sy = m_dPrnTksTopSp + m_dPrnTksSinTopSp-m_dPrnTksSinNSize;
	ey = sy + m_dPrnTksSinKSize;
	fncYStrInBox(ctx, sx, sy, ex, ey, "新婦");

	sx = cntrx + divx - m_dPrnTksSinNLen/2;
	ex = sx + m_dPrnTksSinNLen;
	sy = ey + m_dPrnTksSinKNSp;
	ey = sy + m_dPrnTksSinNSize;
	fncYStrInBox(ctx, sx, sy, ex, ey, m_strSinpuName2);

	if(m_nTakasagoKind == 5
	|| m_nTakasagoKind == 6
	|| m_nTakasagoKind == 7){
		// 御媒妁人(御媒酌人)
		idx = m_arystrSitGestKubun.indexOf(m_strBsyk1Kubun, 0);
		sx = cntrx -  divx*3 - (m_dPrnTksBskNLen+m_dPrnTksBskNSSp+m_dPrnTksBskNSize)/2;
		//sx = cntrx - divx*3 - m_dPrnTksBskNLen/2;
		ex = sx + m_dPrnTksBskNLen;
		sy = m_dPrnTksTopSp + m_dPrnTksSinTopSp + m_dPrnTksSinKSize + m_dPrnTksSinKNSp-m_dPrnTksSinNSize;
		ey = sy + m_dPrnTksBskNSize;
		strStr = m_arystrSitGestName1[idx]+m_arystrSitGestName2[idx];
		fncYStrInBox(ctx, sx, sy, ex, ey, strStr);
		tsx = ex + m_dPrnTksBskNSSp;
		tex = tsx + m_dPrnTksBskNSize;
		fncYStrInBox(ctx, tsx, sy, tex, ey, "様");
		
		strStr = fncGetGestKtgk(idx);
		cnt = fncGetKtgkCnt(idx);
		aryStr = strStr.split(",");
		sy = sy-m_dPrnTksBskKSize*(4-cnt);
		for (i = 0; i < cnt; i++) {
			fncYStrOut(ctx, sx, sy, m_dPrnTksBskKSize, aryStr[i]);
			sy = sy + m_dPrnTksBskKSize;
		}

		// 御媒妁人夫人(御媒酌令夫人)
		idx = m_arystrSitGestKubun.indexOf(m_strBsyk2Kubun, 0);
		sx = cntrx +  divx*3 - (m_dPrnTksBskNLen+m_dPrnTksBskNSSp+m_dPrnTksBskNSize)/2;
		//sx = cntrx +  divx*3 - m_dPrnTksBskNLen/2;
		ex = sx + m_dPrnTksBskNLen;
		sy = m_dPrnTksTopSp + m_dPrnTksSinTopSp + m_dPrnTksSinKSize + m_dPrnTksSinKNSp-m_dPrnTksSinNSize;
		ey = sy + m_dPrnTksBskNSize;
		strStr = m_arystrSitGestName1[idx]+m_arystrSitGestName2[idx];
		fncYStrInBox(ctx, sx, sy, ex, ey, strStr);
		tsx = ex + m_dPrnTksBskNSSp;
		tex = tsx + m_dPrnTksBskNSize;
		fncYStrInBox(ctx, tsx, sy, tex, ey, "様");
		// 肩書きを処理
		strStr = fncGetGestKtgk(idx);
		cnt = fncGetKtgkCnt(idx);
		aryStr = strStr.split(",");
		sy = sy-m_dPrnTksBskKSize*(4-cnt);
		for (i = 0; i < cnt; i++) {
			fncYStrOut(ctx, sx, sy, m_dPrnTksBskKSize, aryStr[i]);
			sy = sy + m_dPrnTksBskKSize;
		}
	}
}
function fncSekijiTableArea(ctx)
{
	var dTableX, dTableY;
	var tidx, tno;
	var sx, sy, ex, ey, cx, cy;
	var tx, ty;
	var pt = new Point();
	var hx, hy;
	var r;

	hx = m_dPrnTblXSize/2.0;
	hx = fncTrnsPrintLen(hx);
	hy = m_dPrnTblYSize/2.0;
	hy = fncTrnsPrintLen(hy);
	for (tidx = 0; tidx < m_nSitTableMax; tidx++) {
		dTableX = m_arydTableX[tidx] * m_dTableXTime + m_dPrnTblLeft;
		dTableY = m_arydTableY[tidx] * m_dTableYTime + m_dPrnTblTop;

		pt.x = dTableX;
		pt.y = dTableY;
		pt = fncTrnsPrintPoint(pt);
		sprTable.x = pt.x-hx;
		sprTable.y = pt.y-hy;
		if (m_nPrnTblKind == 0) {
			r = hx;
			sprTable.graphics.lineStyle(0.1, 0);
			sprTable.graphics.beginFill(0xffffff);
			sprTable.graphics.drawCircle(r, r, r);
			sprTable.graphics.endFill();
		} else {
			cx = hx;
			cy = hy;
			sx = 0;
			sy = 0;
			ex = hx*2;
			ey = hy*2;
			sprTable.graphics.lineStyle(0.1, 0);
			sprTable.graphics.beginFill(0xffffff);
			sprTable.graphics.moveTo(sx, cy);
			sprTable.graphics.lineTo(cx, sy);
			sprTable.graphics.lineTo(ex, cy);
			sprTable.graphics.lineTo(cx, ey);
			sprTable.graphics.lineTo(sx, cy);
			sprTable.graphics.endFill();
		}
		tx = dTableX;
		ty = dTableY;
		sx = tx - m_dPrnTblTextSize / 2;
		sy = ty - m_dPrnTblTextSize / 2;
		fncYStrOut(spr, sx, sy, m_dPrnTblTextSize, m_aryTableName[tidx]);
		fncSekijiOneTableDraw(spr, tx, ty, tidx);
	}
}

function fncSekijiCommentArea(ctx)
{
	var sx, sy;
	var len;
	
	sx = m_dPrnCommentRSp;
	sy = m_dPrnPaperHeight - m_dPrnCommentBSp - m_dPrnCommentLeftSize;
	fncYStrOut(ctx, sx, sy, m_dPrnCommentLeftSize, m_strLeftComment);

	len = m_strRightComment.length;
	sx = m_dPrnPaperWidth - m_dPrnCommentRSp -( len * m_dPrnCommentRightSize * 1.1+6);
	sy = m_dPrnPaperHeight - m_dPrnCommentBSp - m_dPrnCommentRightSize;
	fncYStrOut(ctx, sx, sy, m_dPrnCommentRightSize, m_strRightComment);
}

function fncSekijiOneTableDraw(ctx, cx, cy, tidx)
{
	//m_dPrnTblXSize
	var dAreaWidth;
	var sidx, aryno, idx;
	var sumcnt, cnt, i;
	var div, len;
	var sumsp, space;
	var sx, sy, ex, ey;
	var tsx, tsy, tex, tey;
	var dsx, dsy, dex, dey;
	var ttsy, ttsize;
	var strStr, strTStr;
	var aryStr = new Array();
	var nLastGest;

	dAreaWidth = 	m_dPrnSitAreaWidth + m_dPrnSitNSSpace + m_dPrnSitSamatextSize;
	div = dAreaWidth / m_dPrnSitKtgktextSize;
	sx = cx - m_dPrnTblXSize/2.0 - m_dPrnSitTblSp - dAreaWidth;
	ex = sx + dAreaWidth;
	sy = cy - m_dPrnSitAreaHeight/2.0;
	ey = sy + m_dPrnSitAreaHeight;

	//dsx = sx; dsy = sy;
	//dex = ex; dey = ey;
	//fncBoxOut(spr, dsx, dsy, dex, dey);

	for (sidx = 0; sidx < m_nSitSeatMax; sidx++) {
		aryno = m_ary2nSitLeftGest[tidx][sidx];
		if (aryno != 0) {
			nLastGest = sidx+1;
		}
	}
	sumcnt = 0;
	for (sidx = 0; sidx < nLastGest; sidx++) {
		aryno = m_ary2nSitLeftGest[tidx][sidx];
		if (aryno == 0) {
			sumcnt += 1;
		} else {
			idx = aryno - 1;
			cnt = fncGetKtgkCnt(idx);
			if(cnt == 0){
				cnt=1;
			}
			sumcnt += cnt;
		}
	}
	//sumsp = m_dPrnSitAreaHeight - (m_dPrnSitKtgktextSize * sumcnt +m_dPrnSitNametextSize * m_nSitSeatMax);
	//space = sumsp / (m_nSitSeatMax-1);
	//sumsp = m_dPrnSitAreaHeight - (m_dPrnSitKtgktextSize * sumcnt +m_dPrnSitNametextSize * m_nSitSeatUseMax);
	//space = sumsp / (m_nSitSeatUseMax-1);
	sumsp = m_dPrnSitAreaHeight - (m_dPrnSitKtgktextSize * sumcnt +m_dPrnSitNametextSize * nLastGest);
	space = sumsp / (nLastGest-1);
	tsx = sx;
	tsy = sy;
	for (sidx = 0; sidx < m_nSitSeatMax; sidx++) {
		aryno = m_ary2nSitLeftGest[tidx][sidx];
		if (aryno == 0) {
			tsy = tsy + m_dPrnSitKtgktextSize + m_dPrnSitNametextSize + space;
		} else {
			idx = aryno - 1;
			tsx = sx;
			strStr = fncGetGestKtgk(idx);
			aryStr = strStr.split(",");
			cnt = fncGetKtgkCnt(idx);
			for (i = 0; i < cnt; i++) {
				if (i != 0) {
					tsy = tsy + m_dPrnSitKtgktextSize;
				}
				len = aryStr[i].length;
				if (len < div) {
					dsx = tsx; dsy = tsy;
					fncYStrOut(ctx, dsx, dsy, m_dPrnSitKtgktextSize, aryStr[i]);
				} else {
					tex = tsx + dAreaWidth;
					tey = tsy + m_dPrnSitKtgktextSize;
					dsx = tsx; dsy = tsy;
					dex = tex; dey = tey;
					fncYStrInBox(ctx, dsx, dsy, dex, dey, aryStr[i]);
				}
			}
			tsy = tsy+m_dPrnSitKtgktextSize;
			tey = tsy+m_dPrnSitNametextSize;
			tex = tsx+m_dPrnSitAreaWidth;
			strStr = fncGetGestName(idx);
			dsx = tsx; dsy = tsy;
			dex = tex; dey = tey;
			fncYStrInBox(ctx, dsx, dsy, dex, dey, strStr);
			tsx = tex + m_dPrnSitNSSpace;
			tex = tsx + m_dPrnSitSamatextSize;
			ttsy = tsy+m_dPrnSitSamatextSize/2;
			ttsize = m_dPrnSitSamatextSize/2;
			len = m_arystrSitGestSama[idx].length;
			if (m_arystrSitGestSama[idx] != "") {
				if(len != 1){
					dsx = tsx; dsy = ttsy;
					fncYStrOut(ctx, dsx, dsy, ttsize, m_arystrSitGestSama[idx]);
				}else{
					dsx = tsx; dsy = tsy;
					fncYStrOut(ctx, dsx, dsy, m_dPrnSitSamatextSize, m_arystrSitGestSama[idx]);
				}
			}
			tsy = tey + space;
		}
	}
	sx = cx + m_dPrnTblXSize/2.0 + m_dPrnSitTblSp;
	ex = sx + dAreaWidth;
	sy = cy - m_dPrnSitAreaHeight/2.0;
	ey = sy + m_dPrnSitAreaHeight;

	//dsx = sx; dsy = sy;
	//dex = ex; dey = ey;
	//fncBoxOut(spr, dsx, dsy, dex, dey);

	for (sidx = 0; sidx < m_nSitSeatMax; sidx++) {
		aryno = m_ary2nSitRightGest[tidx][sidx];
		if (aryno != 0) {
			nLastGest = sidx+1;
		}
	}
	sumcnt = 0;
	for (sidx = 0; sidx < nLastGest; sidx++) {
		aryno = m_ary2nSitRightGest[tidx][sidx];
		if (aryno == 0) {
			sumcnt += 1;
		} else {
			idx = aryno - 1;
			cnt = fncGetKtgkCnt(idx);
			if(cnt == 0){
				cnt = 1;
			}
			sumcnt += cnt;
		}
	}
	//sumsp = m_dPrnSitAreaHeight - (m_dPrnSitKtgktextSize * sumcnt +m_dPrnSitNametextSize * m_nSitSeatMax);
	//space = sumsp / (m_nSitSeatMax-1);
	//sumsp = m_dPrnSitAreaHeight - (m_dPrnSitKtgktextSize * sumcnt +m_dPrnSitNametextSize * m_nSitSeatUseMax);
	//space = sumsp / (m_nSitSeatUseMax-1);
	sumsp = m_dPrnSitAreaHeight - (m_dPrnSitKtgktextSize * sumcnt +m_dPrnSitNametextSize * nLastGest);
	space = sumsp / (nLastGest-1);
	tsx = sx;
	tsy = sy;
	for (sidx = 0; sidx < m_nSitSeatMax; sidx++) {
		aryno = m_ary2nSitRightGest[tidx][sidx];
		if (aryno == 0) {
			tsy = tsy + m_dPrnSitKtgktextSize + m_dPrnSitNametextSize + space;
		} else {
			idx = aryno - 1;
			tsx = sx;
			strStr = fncGetGestKtgk(idx);
			aryStr = strStr.split(",");
			cnt = fncGetKtgkCnt(idx);
			for (i = 0; i < cnt; i++) {
				if (i != 0) {
					tsy = tsy + m_dPrnSitKtgktextSize;
				}
				len = aryStr[i].length;
				if (len < div) {
					dsx = tsx; dsy = tsy;
					fncYStrOut(ctx, tsx, tsy, m_dPrnSitKtgktextSize, aryStr[i]);
				} else {
					tex = tsx + dAreaWidth;
					tey = tsy + m_dPrnSitKtgktextSize;
					dsx = tsx; dsy = tsy;
					dex = tex; dey = tey;
					fncYStrInBox(ctx, tsx, tsy, tex, tey, aryStr[i]);
				}
			}
			tsy = tsy+m_dPrnSitKtgktextSize;
			tey = tsy+m_dPrnSitNametextSize;
			tex = tsx+m_dPrnSitAreaWidth;
			strStr = fncGetGestName(idx);
			dsx = tsx; dsy = tsy;
			dex = tex; dey = tey;
			fncYStrInBox(spr, dsx, dsy, dex, dey, strStr);
			tsx = tex + m_dPrnSitNSSpace;
			tex = tsx + m_dPrnSitSamatextSize;
			ttsy = tsy+m_dPrnSitSamatextSize/2;
			ttsize = m_dPrnSitSamatextSize/2;
			len = m_arystrSitGestSama[idx].length;
			if (m_arystrSitGestSama[idx] != "") {
				if(len != 1){
					dsx = tsx; dsy = ttsy;
					fncYStrOut(ctx, dsx, dsy, ttsize, m_arystrSitGestSama[idx]);
				}else{
					dsx = tsx; dsy = tsy;
					fncYStrOut(ctx, dsx, dsy, m_dPrnSitSamatextSize, m_arystrSitGestSama[idx]);
				}
			}
			tsy = tey + space;
		}
	}
}
function fncDrawTable(ctx, cTbl)
{
	m_fs = parseInt(m_cnvsHeight / 12);
	ctx.beginPath();
	ctx.arc(x, y, m_fs/4*3, 0, 2 * Math.PI);
	ctx.closePath();
	ctx.strokeStyle = "black";
	ctx.lineWidth = 2;
	ctx.stroke();
}