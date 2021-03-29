
function fncColorTableDraw()
{

}
function fncToolSampleSelect(x, y)
{
    var imgex;

    imgex = fncToolSampleSearch(y);
    if(imgex == null){
        return;
    }
    fncToolSampleCreateObject(imgex);
    fncCommandSetCrt(CMDSELECT);
}
function fncToolSampleSearch(y)
{
    var max, idx;
    var imgex;

    var area = document.getElementById("divSampleArea");
    fncCommandInfoTextSet("y " + y + "area.offsetTop " + area.offsetTop + "area.scrollTop " + area.scrollTop);
    y = y - area.offsetTop + area.scrollTop;
 	max = m_arySampleImage.length;
    for(idx = 0; idx < max; idx++){
        imgex = m_arySampleImage[idx];
        sy = imgex.sy;
        ey = sy + imgex.hi;
        if(sy < y && y < ey){
            return(imgex);
        }
    }
    return(null);
}
function fncToolSampleCreateObject(imgex)
{
    var obj;
    var sx, sy, ex, ey;
    var filename;
    var lock;
    var img;

    if(m_nSelectKind == YOKOFRAME){
        sx = 0;
        sy = 0;
        ex = SIZEA4LONG;
        ey = SIZEA4SHORT;
        lock = true;
        filename = "ykframe/"+imgex.basename+".png";
    }else if(m_nSelectKind == TATEFRAME){
        sx = 0;
        sy = 0;
        ex = SIZEA4SHORT;
        ey = SIZEA4LONG;
        lock = true;
        filename = "ttframe/"+imgex.basename+".png";
    }else if(m_nSelectKind == PARTS){
        if(imgex.width < imgex.height){
            time = (SIZEA4SHORT / 2) / imgex.height;
        }else{
            time = (SIZEA4SHORT / 2) / imgex.width;
        }
        sx = SIZEA4SHORT / 4;
        sy = SIZEA4SHORT / 4;
        ex = sx + imgex.width * time;
        ey = sy + imgex.height * time;
        lock = false;
        filename = "parts/"+imgex.basename+".png";
    }else{
        return;
    }
    obj = new clsObjImage();
    obj.kind = OBJIMAGE;
    obj.lock = lock;
    obj.fncSetAtrXY(0, 0, sx, sy);
    obj.fncSetAtrXY(1, 1, ex, sy);
    obj.fncSetAtrXY(2, 1, ex, ey);
    obj.fncSetAtrXY(3, 1, sx, ey);

    fncCommandSetCrtObject(null);
    m_aryObject.push(obj);
    obj.image.src = filename;
    obj.image.onload = fncCommandImageLoad;
    if(m_nSelectKind == PARTS){
        fncCommandSetCrtObject(obj);
    }
}
function fncToolSampleDraw()
{
    var divSample;
    var divSampleArea
	var max, idx;
    var image;
    var barsize, base;
	var sx, sy, wd, hi;
    var dx, dy, dw, dh;
	var time;

    barsize = 20;
	base = m_cnvsSampleWidth-barsize;
	max = m_arySampleImage.length;
	sx = 0;
	sy = 0;
 	for(idx = 0; idx < max; idx++){
		image = m_arySampleImage[idx];
        if(image.width > image.height){
            wd = base;
            time = wd / image.width;
            hi = image.height * time;
            sx = 0;
        }else{
            hi = base;
            time = hi / image.height;
            wd = image.width * time;
            sx = (base - wd) / 2;
        }
        image.sx = parseInt(sx);
        image.sy = parseInt(sy);
        image.wd = parseInt(wd);
        image.hi = parseInt(hi);
		sy = sy + hi + 5;
    }
    m_cnvsSampleHeight = sy + 10;
    divSample = document.getElementById("divSample");
    divSample.height = m_cnvsSampleHeight;
    cnvsSample = document.getElementById("cnvsSample");
    cnvsSample.width = base;
    cnvsSample.height = m_cnvsSampleHeight;
    //m_cnvsSample.scrollHeight = m_cnvsSampleHeight;
    //m_divSampleArea.height = m_cnvsSampleHeight;
    ctxSample = cnvsSample.getContext('2d');

    for(idx = 0; idx < max; idx++){
		image = m_arySampleImage[idx];
        dx = image.sx;
        dy = image.sy;
        dw = image.wd;
        dh = image.hi;
        ctxSample.drawImage(image, dx, dy, dw, dh);
	}
}
