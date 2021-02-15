
// 家名関係変数
var m_dPrnKmiSX = 0.0;
var m_dPrnKmiSY = 0.0;
var m_dPrnKmiKKSp = 0.0;
var m_dPrnKmiNSSp = 0.0;
var m_dPrnKmiSize = 0.0;
var m_nPrnKmiFlag = 0;
var m_nPrnKmiKind = 0;

// 高砂関係変数
var m_dPrnTksTopSp = 0.0;
var m_dPrnTksHeight = 0.0;
var m_dPrnTksWidth = 0.0;
var m_dPrnTksSinTopSp = 0.0;
var m_dPrnTksSinKSize = 0.0;
var m_dPrnTksSinKSp = 0.0;
var m_dPrnTksSinKNSp = 0.0;
var m_dPrnTksSinNSize = 0.0;
var m_dPrnTksSinNLen = 0.0;
var m_dPrnTksBskTopSp = 0.0;
var m_dPrnTksBskKSize = 0.0;
var m_dPrnTksBskKKSp = 0.0;
var m_dPrnTksBskKNSp = 0.0;
var m_dPrnTksBskNSize = 0.0;
var m_dPrnTksBskNLen = 0.0;
var m_dPrnTksBskNSSp = 0.0;

// テーブル関係変数
var m_dPrnTblTopSp = 0.0;
var m_dPrnTblBottomSp = 0.0;
var m_dPrnTblLeftSp = 0.0;
var m_dPrnTblRightSp = 0.0;
var m_dPrnTblXSize = 0.0;
var m_dPrnTblYSize = 0.0;
var m_dPrnTblTextSize = 0.0;
var m_dPrnSitAreaHeight = 0.0;
var m_dPrnSitAreaWidth = 0.0;
var m_dPrnSitTblSp = 0.0;
var m_dPrnSitNametextSize = 0.0;
var m_dPrnSitSamatextSize = 0.0;
var m_dPrnSitKtgktextSize = 0.0;
var m_dPrnSitNSSpace = 0.0;

var m_dPrnDGAreaXSize = 0.0;
var m_dPrnDGTextArea = 0.0;
var m_dPrnDGTextSize = 0.0;

var m_dPrnCommentRSp = 0.0;
var m_dPrnCommentBSp = 0.0;
var m_dPrnCommentLeftSize = 0.0;
var m_dPrnCommentRightSize = 0.0;

var m_nTextSize = 0;
var m_nRyoukeKind = 0;
var m_nTakasagoKind = 0;

function fncInitPrintElement()
{
	var data = "";

	if(m_sTextSize == "小"){ m_nTextSize = 1;
	}else if(m_sTextSize == "大"){ m_nTextSize = 3;
	}else{ m_nTextSize = "2"; }

	if (m_sRyoukeKind == "なし") { m_nRyoukeKind = 0;
	}else if(m_sRyoukeKind == "結婚披露宴御席次表"){ m_nRyoukeKind = 1;
	}else if(m_sRyoukeKind == "両家結婚披露宴御席次"){ m_nRyoukeKind = 2;
	}else if(m_sRyoukeKind == "両家結婚披露宴御臨席表"){ m_nRyoukeKind = 3;
	}else if(m_sRyoukeKind == "両家結婚披露宴御案内"){ m_nRyoukeKind = 4;
	}else if(m_sRyoukeKind == "2行家"){ m_nRyoukeKind = 5;
	}else if(m_sRyoukeKind == "1行家"){ m_nRyoukeKind = 6;
	}else if(m_sRyoukeKind == "2行様"){ m_nRyoukeKind = 7;
	}else if(m_sRyoukeKind == "1行様"){ m_nRyoukeKind = 8;
	}else{ m_nRyoukeKind = 6; }

	if(m_strTakasagoKind == "横書高砂上(高砂名無し)"){ m_nTakasagoKind = 1;
	}else if(m_strTakasagoKind == "横書高砂上(高砂名有)"){ m_nTakasagoKind = 2;
	}else if(m_strTakasagoKind == "縦書高砂上(高砂名無し)"){ m_nTakasagoKind = 3;
	}else if(m_strTakasagoKind == "縦書高砂上(高砂名有)"){ m_nTakasagoKind = 4;
	}else if(m_strTakasagoKind == "横書媒酌有枠有"){ m_nTakasagoKind = 5;
	}else if(m_strTakasagoKind == "横書媒酌有高砂上(高砂名無し)"){ m_nTakasagoKind = 6;
	}else if(m_strTakasagoKind == "横書媒酌有高砂上(高砂名有)"){ m_nTakasagoKind = 7;
	}else if(m_strTakasagoKind == "縦書媒酌有高砂上(高砂名無し)"){ m_nTakasagoKind = 8;
	}else if(m_strTakasagoKind == "縦書媒酌有高砂上(高砂名有)"){ m_nTakasagoKind = 9;
	}else{ m_nTakasagoKind = 2; }

	data = "file=../list/layout"+m_nTextSize+"0"+m_nTakasagoKind+".lct";
	var fnc = fncInitPrintElementCallback;
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncInitPrintElementCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var aryLine = new Array();
	var aryCsv = new Array();

	m_strKubunFile = "kubun.txt";
	m_nSitSeatMax = 7;
	m_nSitSeatUseMax =0;

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fncInitPrintElementDefault();
		return;
	}
	aryLine = data.split("\r\n");
	// 家名関係変数
	aryCsv = m_aryLine[5].split(" ");
	m_dPrnKmiSX = parseFloat(aryCsv[1]);	// 両家名領域左余白
	m_dPrnKmiSY =  parseFloat(aryCsv[2]);	// 両家名領域上余白
	m_dPrnKmiKKSp = parseFloat(aryCsv[3]);	// 両家の間隔
	m_dPrnKmiNSSp = parseFloat(aryCsv[4]);	 // 名前と様の間隔
	m_dPrnKmiSize = parseFloat(aryCsv[5]);	 // 文字サイズ
	m_nPrnKmiFlag = parseInt(aryCsv[6]);	 // 両家名印刷(する,しない)
	m_nPrnKmiKind = parseInt(aryCsv[7]);	 // 家・様の選択(家,様)
	// 高砂関係変数
	aryCsv = m_aryLine[7].split(" ");
	m_dPrnTksHeight =  parseFloat(aryCsv[1]);	// 高砂領域の高さ
	m_dPrnTksWidth =  parseFloat(aryCsv[2]);	// 高砂領域の横幅
	m_dPrnTksTopSp =  parseFloat(aryCsv[3]);	// 高砂領域上余白

	aryCsv = m_aryLine[8].split(" ");
	m_dPrnTksSinTopSp =  parseFloat(aryCsv[1]);	// 新郎新婦肩書上余白
	m_dPrnTksSinKSize =  parseFloat(aryCsv[2]);	// 新郎新婦肩書文字サイズ
	m_dPrnTksSinKSp =  parseFloat(aryCsv[3]);	// 新郎新婦肩書文字間隔
	//m_dPrnTksSinKMv =  parseFloat(aryCsv[4]);	// 新郎新婦肩書移動量
	m_dPrnTksSinKNSp =  parseFloat(aryCsv[5]);	// 新郎新婦肩書と名前の間隔
	m_dPrnTksSinNSize =  parseFloat(aryCsv[6]);	// 新郎新婦名前文字サイズ
	m_dPrnTksSinNLen =  parseFloat(aryCsv[7]);	// 新郎新婦名前長さ

	aryCsv = m_aryLine[9].split(" ");
	m_dPrnTksBskTopSp =  parseFloat(aryCsv[1]);	// 媒妁人上余白
	m_dPrnTksBskKSize =  parseFloat(aryCsv[2]);	// 媒妁人肩書文字サイズ
	m_dPrnTksBskKKSp =  parseFloat(aryCsv[3]);	// 媒妁人肩書文字間隔
	//m_dPrnTksBskKMv =  parseFloat(aryCsv[4]);	// 媒妁人肩書移動量
	m_dPrnTksBskKNSp =  parseFloat(aryCsv[5]);	// 媒妁人の名前と肩書きの間隔
	m_dPrnTksBskNSize =  parseFloat(aryCsv[6]);	// 媒妁人文字サイズ
	m_dPrnTksBskNLen =  parseFloat(aryCsv[7]);	// 媒妁人名前長さ
	m_dPrnTksBskNSSp =  parseFloat(aryCsv[8]);	// 媒妁人の名前と様の間隔

	// テーブル関係変数
	aryCsv = m_aryLine[12].split(" ");
	m_dPrnTblRightSp =  parseFloat(aryCsv[1]);		// 食卓テーブル領域全体の右余白
	m_dPrnTblLeftSp =  parseFloat(aryCsv[2]);	// 食卓テーブル領域全体の左余白
	m_dPrnTblTopSp =  parseFloat(aryCsv[3]);	// 高砂下から食卓テーブル領域全体の上余白
	m_dPrnTblBottomSp =  parseFloat(aryCsv[4]);	// 食卓テーブル領域全体の下余白
	m_dPrnTblXSize =  parseFloat(aryCsv[5]);		// 食卓テーブルの横幅
	m_dPrnTblYSize =  parseFloat(aryCsv[6]);		// 食卓テーブルの縦長さ
	m_dPrnTblTextSize =  parseFloat(aryCsv[7]);		// 食卓名文字サイズ

	aryCsv = m_aryLine[14].split(" ");
	m_dPrnSitAreaHeight =  parseFloat(aryCsv[1]);		// 食卓テーブル領域の高さ
	m_dPrnSitTblSp =  parseFloat(aryCsv[2]);		// 食卓と招待者の間隔
	m_dPrnSitAreaWidth =  parseFloat(aryCsv[4]);		// 名前領域のＸサイズ
	m_dPrnSitNametextSize =  parseFloat(aryCsv[5]);		// 最大名前領域のＹサイズ
	m_dPrnSitSamatextSize =  parseFloat(aryCsv[6]);		// 敬称の文字サイズ

	aryCsv = m_aryLine[15].split(" ");
	m_dPrnSitKtgktextSize =  parseFloat(aryCsv[3]);		// 肩書Ｙサイズ
	m_dPrnSitNSSpace =  parseFloat(aryCsv[7]);		// 名前と敬称の間隔
	
	aryCsv = m_aryLine[17].split(" ");
	m_dPrnCommentRSp =  parseFloat(aryCsv[1]);		// 右下コメント領域右余白
	m_dPrnCommentBSp =  parseFloat(aryCsv[2]);		// 右下コメント領域右余白
	m_dPrnCommentLeftSize =  m_dPrnSitKtgktextSize; 
	m_dPrnCommentRightSize =  m_dPrnSitNametextSize; 

	m_dPrnDGAreaXSize = 30;
	if(m_sTextSize == "大"){
		m_dPrnDGTextSize = 3.8;
		m_dPrnDGTextArea = 4.5;
	}else{
		m_dPrnDGTextSize = m_dPrnSitNametextSize;
		m_dPrnDGTextArea = m_dPrnDGTextSize+1;
	}
}
function fncInitPrintElementDefault()
{
	m_strKubunFile = "kubun.txt";
	m_nSitSeatMax = 7;
	m_nSitSeatUseMax =0;

	// 家名関係変数
	m_dPrnKmiSX = 30.0;
	m_dPrnKmiSY = 15.0;
	m_dPrnKmiKKSp = 6.0;
	m_dPrnKmiNSSp = 6.0;
	m_dPrnKmiSize = 6.0;
	m_nPrnKmiFlag = 0;
	m_nPrnKmiKind = 0;
	// 高砂関係変数
	m_dPrnTksTopSp = 10.0;
	m_dPrnTksHeight = 50.0;
	m_dPrnTksWidth = 100.0;
	m_dPrnTksSinTopSp = 10.0;
	m_dPrnTksSinKSize = 2.5;
	m_dPrnTksSinKSp = 2.5;
	m_dPrnTksSinKNSp = 4.0;
	m_dPrnTksSinNSize = 4.0;
	m_dPrnTksSinNLen = 12.0;
	m_dPrnTksBskTopSp = 0.0;
	m_dPrnTksBskKSize = 2.5;
	m_dPrnTksBskKKSp = 0.0;
	m_dPrnTksBskKNSp = 0.0;
	m_dPrnTksBskNSize = 4.0;
	m_dPrnTksBskNLen = 24.0;
	m_dPrnTksBskNSSp = 2.0;
	// テーブル関係変数
	m_dPrnTblTopSp = 10.0;
	m_dPrnTblBottomSp = 20.0;
	m_dPrnTblLeftSp = 10.0;
	m_dPrnTblRightSp = 10.0;
	m_dPrnTblXSize = 15.0;
	m_dPrnTblYSize = 15.0;
	m_dPrnTblTextSize = 10.0;
	m_dPrnSitAreaHeight = 48.0;
	m_dPrnSitAreaWidth = 12.0;
	//m_dPrnSitAreaWidth = 24;
	m_dPrnSitTblSp = 2.0;
	m_dPrnSitNametextSize = 3.0;
	m_dPrnSitSamatextSize = 3.0;
	m_dPrnSitKtgktextSize = 2.5;
	m_dPrnSitNSSpace = 2.0;
	
	m_dPrnCommentRSp = 10.0;
	m_dPrnCommentBSp = 5.0;
	m_dPrnCommentLeftSize = m_dPrnSitKtgktextSize;
	m_dPrnCommentRightSize = m_dPrnSitNametextSize;

	m_dPrnDGAreaXSize = 30.0;
	m_dPrnDGTextArea = 4.0;
	m_dPrnDGTextSize = 3.0;
	
	m_nFontName = "ＭＳ ゴシック";
}

