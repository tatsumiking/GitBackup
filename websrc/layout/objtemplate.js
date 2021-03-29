
// Template picture initialization processing 

function fncLoadLYTemplateList()
{
	var request:URLRequest = new URLRequest();
	var header:URLRequestHeader;
	
	header = new URLRequestHeader("pragma", "no-cache");
	request.requestHeaders.push(header);
	if(m_nOrientation == YOUSIYOKO){
		request.url = "list/yktemplate.txt";
	}else{
		request.url = "list/tttemplate.txt";
	}
	m_loaderLYTemplateList.dataFormat = URLLoaderDataFormat.BINARY;
	m_loaderLYTemplateList.load(request);
}

function onCompleteLYTemplateList(event)
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
	m_nLYTemplateMax = aryCsv.length;
	if (aryCsv[m_nLYTemplateMax - 1] == "")
	{
		m_nLYTemplateMax--;
	}
	m_nLYTemplateCrt = 0;
	for (i = 0; i < m_nLYTemplateMax; i++)
	{
		sprite = new Sprite();
		szName = new String(aryCsv[i]);
		ptAdd = new Point(0,0);
		m_aryLYTemplateSprite.push(sprite);
		m_aryLYTemplateName.push(szName);
		m_aryLYTemplateAdd.push(ptAdd);
		fncLoadLYTemplateImage(szName);
	}
}

function fncLoadLYTemplateImage(szName)
{
	var szFileName;
	var loaderBmp:Loader = new Loader();
	var header:URLRequestHeader;

	if(m_nOrientation == YOUSIYOKO){
		szFileName = "yktemplate/S"+szName;
	}else{
		szFileName = "tttemplate/S"+szName;
	}
	loaderBmp.contentLoaderInfo.addEventListener(Event.COMPLETE,onCompleteLYTemplateImg);
	loaderBmp.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR,onIOErrorLYTemplateImg);
	var urlImage:URLRequest = new URLRequest(szFileName);
	header = new URLRequestHeader("pragma", "no-cache");
	urlImage.requestHeaders.push(header);
	loaderBmp.load(urlImage);
}

function onIOErrorLYTemplateImg(event:IOErrorEvent)
{
	m_nLYTemplateCrt++;
	txtInfo.text = "ERROR <" + event.text+">";
	if (m_nLYTemplateMax <= m_nLYTemplateCrt)
	{
		m_nLYTemplateCrtTop = 0;
		if (m_nLYFigureCrtTop == 0)
		{
			fncInitScrlTemplate();
			fncDrawLYTemplateImage();
		}
	}
}

function onCompleteLYTemplateImg(event)
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
	szName = szFileName.slice(szFileName.lastIndexOf("/") + 2,szFileName.length);
	nIdx = fncSrchLYTemplateIdx(szName);
	sprite = m_aryLYTemplateSprite[nIdx];
	ptAdd = m_aryLYTemplateAdd[nIdx];
	areax = LYTMONEXSIZE - LYTMSPACE;
	timex = areax / Number(loaderBmp.width);
	areay = LYTMONEYSIZE - LYTMSPACE;
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
	bmp = new Bitmap(bmpdata);
	m_aryLYTemplateBmpData[nIdx] = bmpdata;
	sprite.addChild(bmp);
	m_nLYTemplateCrt++;
	if (m_nLYTemplateMax <= m_nLYTemplateCrt)
	{
		m_nLYTemplateCrtTop = 0;
		if (m_nLYFigureCrtTop == 0)
		{
			fncInitScrlTemplate();
			fncDrawLYTemplateImage();
		}
	}
}

function fncInitScrlTemplate()
{
	var nXCnt,nYCnt;
	var nMin,nMax;

	m_nSelectAreaCommand = CMDTEMPLATE;
	nXCnt = SLCTAREAXSIZE / LYTMONEXSIZE;
	nYCnt = SLCTAREAYSIZE / LYTMONEYSIZE;
	nMin = 0;
	nMax = m_nLYTemplateMax / nXCnt;
	scrlSelectArea.setScrollProperties(nYCnt, nMin, nMax);
}

