
// Photograph upload reading processing

function fncPhotoLoad()
{
	var flaFilter:FileFilter;
	
	flaFilter = new FileFilter("Photo File", "*.jpg");
	m_frPhotoUpload.browse([flaFilter]);
}

function onSelectPhotoUpload(event)
{
	var strUrl;

	var urlReq:URLRequest;
	var vars:URLVariables = new URLVariables();

	vars.com = m_nSlctRecNo + ",,,";
	urlReq = new URLRequest("uploadphoto.php");
	urlReq.method = URLRequestMethod.POST;
	urlReq.data = vars;

	try
	{
		txtInfo.text = m_frPhotoUpload.name + " uploading";
		m_frPhotoUpload.upload(urlReq);
	}
	catch (e:Error)
	{
		txtInfo.text = e.message;
	}
}

function onCompletePhotoUpload(event)
{
	m_szPhotoName = m_frPhotoUpload.name;
	txtInfo.text = m_frPhotoUpload.name + "Finished";
	fncLoadLYPhotoImage();
}

function onIOErrorPhotoUpload(event:IOErrorEvent)
{
	txtInfo.text = "Upload of the ERROR photograph went wrong.";
}

function fncLoadLYPhotoImage()
{
	var szFileName;
	var loaderBmp:Loader = new Loader();
	szFileName = "photo/" + m_nSlctRecNo + "/" + m_szPhotoName;
	loaderBmp.contentLoaderInfo.addEventListener(Event.COMPLETE,onCompleteLYPhotoImg);
	loaderBmp.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR,onIOErrorLYPhotoImg);
	var urlImage:URLRequest = new URLRequest(szFileName);
	loaderBmp.load(urlImage);
	txtInfo.text = m_szPhotoName + "Importing";
}

function onIOErrorLYPhotoImg(event:IOErrorEvent)
{
	txtInfo.text = "Taking in of an ERROR photograph went wrong. ";
}

function onCompleteLYPhotoImg(event)
{
	var szFileName;
	var szName;
	var nIdx;
	var loaderBmp:Loader;
	var bmpdata;
	var bmp;
	var sprite;
	var timex,timey;
	var areax,areay;

	if (m_sprPhoto.numChildren == 1)
	{
		var display_obj:DisplayObject = m_sprPhoto.getChildAt(0);
		m_sprPhoto.removeChild(display_obj);
	}
	loaderBmp = event.target.content.parent;
	areax = m_nEditAreaXSize / 2;
	timex = areax / Number(loaderBmp.width);
	areay = m_nEditAreaYSize / 2;
	timey = areay / Number(loaderBmp.height);
	if (timex < timey)
	{
		m_sprPhoto.scaleX = timex;
		m_sprPhoto.scaleY = timex;
	}
	else
	{
		m_sprPhoto.scaleX = timey;
		m_sprPhoto.scaleY = timey;
	}
	m_bmpdataPhoto = new BitmapData(loaderBmp.width,loaderBmp.height);
	m_bmpdataPhoto.draw(loaderBmp);
	bmp = new Bitmap(m_bmpdataPhoto);
	m_sprPhoto.x = m_nEditAreaXSize / 4;
	m_sprPhoto.y = m_nEditAreaYSize / 4;
	m_sprPhoto.addChild(bmp);
	txtInfo.text = "";
	fncSetCrtCommand(CMDSELECT);
}

function fncLoadLYPhotoData(filename)
{
	var szFileName;

	m_szPhotoName = filename
	var loaderBmp:Loader = new Loader();
	szFileName = "photo/" + m_nSlctRecNo + "/" + m_szPhotoName;
	loaderBmp.contentLoaderInfo.addEventListener(Event.COMPLETE,onCompleteLYPhotoData);
	loaderBmp.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR,onIOErrorLYPhotoData);
	var urlImage:URLRequest = new URLRequest(szFileName);
	loaderBmp.load(urlImage);
	txtInfo.text = m_szPhotoName + "Importing";
}

function onIOErrorLYPhotoData(event:IOErrorEvent)
{
	txtInfo.text = "Taking in of an ERROR photograph went wrong. ";
}

function onCompleteLYPhotoData(event)
{
	var loaderBmp:Loader;
	var bmpdata;
	var bmp;

	if (m_sprPhoto.numChildren == 1)
	{
		var display_obj:DisplayObject = m_sprPhoto.getChildAt(0);
		m_sprPhoto.removeChild(display_obj);
	}
	loaderBmp = event.target.content.parent;
	m_sprPhoto.scaleX = m_dLoadPhotoScaloeX;
	m_sprPhoto.scaleY = m_dLoadPhotoScaloeX;
	m_bmpdataPhoto = new BitmapData(loaderBmp.width,loaderBmp.height);
	m_bmpdataPhoto.draw(loaderBmp);
	bmp = new Bitmap(m_bmpdataPhoto);
	m_sprPhoto.x = m_nLoadPhotoSX;
	m_sprPhoto.y = m_nLoadPhotoSY;
	m_sprPhoto.addChild(bmp);
	loaderBmp.contentLoaderInfo.removeEventListener(Event.COMPLETE,onCompleteLYPhotoData);
	loaderBmp.contentLoaderInfo.removeEventListener(IOErrorEvent.IO_ERROR,onIOErrorLYPhotoData);
}

