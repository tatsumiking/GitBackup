
// 用紙全体をLayout領域に表示するための変換倍率
var m_dScrnBaseTime = 1.0;
// Canvas領域の左上座標(px)
var m_dScrnRltvSX = 0.0;
var m_dScrnRltvSY = 0.0;
var m_dScrnZoomTime = 1.0;

function fncScrnTrnsRMX(rltvx)
{
	var absx = 0;

    rltvx = rltvx - m_dScrnRltvSX;
    rltvx = rltvx / m_dScrnZoomTime;
    absx = rltvx / m_dScrnBaseTime;
	return(absx);
}
function fncScrnTrnsRMY(rltvy)
{
	var absy = 0;

    rltvy = rltvy - m_dScrnRltvSX;
    rltvy = rltvy / m_dScrnZoomTime;
    absy = rltvy / m_dScrnBaseTime;
	return(absy);
}
function fncScrnTrnsRMLen(rltvlen)
{
	var abslen = 0;

    rltvlen = rltvlen / m_dScrnZoomTime;
    abslen = rltvlen / m_dScrnBaseTime;
	return(abslen);
}

function fncScrnTrnsMRX(absx)
{
	var rltvx = 0;
    
    rltvx = absx * m_dScrnBaseTime;
    rltvx = rltvx * m_dScrnZoomTime;
    rltvx = rltvx + m_dScrnRltvSX;
	return(rltvx);
}
function fncScrnTrnsMRY(absy)
{
	var rltvy = 0;
    
    rltvy = absy * m_dScrnBaseTime;
    rltvy = rltvy * m_dScrnZoomTime;
    rltvy = rltvy + m_dScrnRltvSY;
	return(rltvy);
}
function fncScrnTrnsMRLen(abslen)
{
	var rltvlen = 0;

    rltvlen = abslen * m_dScrnBaseTime;
    rltvlen = rltvlen * m_dScrnZoomTime;
	return(rltvlen);
}

