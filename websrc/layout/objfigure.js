
//  Figure picture initialization processing

function fncLoadLYFigureList()
{
	var request:URLRequest = new URLRequest();
	var header:URLRequestHeader;
	
	header = new URLRequestHeader("pragma", "no-cache");
	request.requestHeaders.push(header);
	request.url = "list/figure.txt";
	m_loaderLYFigureList.dataFormat = URLLoaderDataFormat.BINARY;
	m_loaderLYFigureList.load(request);
}

function onCompleteLYFigureList(event)
{
	var loader:URLLoader = URLLoader(event.target);
	var aryCsv;
	var i,max;
	var szName;
	var sprite;
	var ptAdd:Point;

	var byteArray:ByteArray = loader.data;
	var str = byteArray.readMultiByte(byteArray.length,"utf-8");
	aryCsv = str.split("\r\n");
	m_nLYFigureMax = aryCsv.length;
	if (aryCsv[m_nLYFigureMax - 1] == "")
	{
		m_nLYFigureMax--;
	}
	m_nSelectLYFigure = -1;
	m_nLYFigureCrt = 0;
	for (i = 0; i < m_nLYFigureMax; i++)
	{
		sprite = new Sprite();
		szName = new String(aryCsv[i]);
		ptAdd = new Point(0,0);
		m_aryLYFigureSprite.push(sprite);
		m_aryLYFigureName.push(szName);
		m_aryLYFigureAdd.push(ptAdd);
		fncLoadLYFigureImage(szName);
	}
}

function fncLoadLYFigureImage(szName)
{
	var szFileName;
	var loaderBmp:Loader = new Loader();
	var header:URLRequestHeader;

	szFileName = "figure/" + szName;
	loaderBmp.contentLoaderInfo.addEventListener(Event.COMPLETE,onCompleteLYFigureImg);
	loaderBmp.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR,onIOErrorLYFigureImg);
	var urlImage:URLRequest = new URLRequest(szFileName);
	header = new URLRequestHeader("pragma", "no-cache");
	urlImage.requestHeaders.push(header);
	loaderBmp.load(urlImage);
}

function onIOErrorLYFigureImg(event:IOErrorEvent)
{
	m_nLYFigureCrt++;
	txtInfo.text = "ERROR" + m_nLYFigureMax + "," + m_nLYFigureCrt;
	if (m_nLYFigureMax <= m_nLYFigureCrt)
	{
		m_nLYFigureCrtTop = 0;
		if (m_nLYTemplateCrtTop == 0)
		{
			fncInitScrlTemplate();
			fncDrawLYTemplateImage();
		}
	}
}

function onCompleteLYFigureImg(event)
{
	var szFileName;
	var szName;
	var nIdx;
	var loaderBmp:Loader;
	var bmpdata;
	var bmp;
	var sprite;
	var ptAdd:Point;
	var timex,timey;
	var areax,areay;

	loaderBmp = event.target.content.parent;
	szFileName = loaderBmp.contentLoaderInfo.url;
	szName = szFileName.slice(szFileName.lastIndexOf("/") + 1,szFileName.length);
	nIdx = fncSrchLYFigureIdx(szName);
	sprite = m_aryLYFigureSprite[nIdx];
	ptAdd = m_aryLYFigureAdd[nIdx];
	areax = LYFGONEXSIZE - LYFGSPACE;
	timex = areax / Number(loaderBmp.width);
	areay = LYFGONEYSIZE - LYFGSPACE;
	timey = areay / Number(loaderBmp.height);
	if (timex < timey)
	{
		sprite.scaleX = timex;
		sprite.scaleY = timex;
		ptAdd.y = (areay - Number(loaderBmp.height) * timex) / 2;
	}
	else
	{
		sprite.scaleX = timey;
		sprite.scaleY = timey;
		ptAdd.x = (areax - Number(loaderBmp.width) * timey) / 2;
	}
	bmpdata = new BitmapData(loaderBmp.width,loaderBmp.height,true,0x00FFFFFF);
	bmpdata.draw(loaderBmp);
	m_aryLYFigureBmpData[nIdx] = bmpdata;
	bmp = new Bitmap(bmpdata);
	sprite.addChild(bmp);
	m_nLYFigureCrt++;
	if (m_nLYFigureMax <= m_nLYFigureCrt)
	{
		m_nLYFigureCrtTop = 0;
		if (m_nLYTemplateCrtTop == 0)
		{
			fncInitScrlTemplate();
			fncDrawLYTemplateImage();
		}
	}
}
function fncInitScrlFigure()
{
	var nXCnt,nYCnt;
	var nMin,nMax;

	m_nSelectAreaCommand = CMDNEWFIGURE;
	nXCnt = SLCTAREAXSIZE / LYFGONEXSIZE;
	nYCnt = SLCTAREAYSIZE / LYFGONEYSIZE;
	nMin = 0;
	nMax = m_nLYFigureMax / int(nXCnt)-(nYCnt-2);
	scrlSelectArea.setScrollProperties(nYCnt, nMin, nMax);
}

