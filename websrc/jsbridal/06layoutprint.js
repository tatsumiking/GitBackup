
var m_cnvsWidth;
var m_cnvsHeight;
var m_fs;
var m_dTime;

function fncInit()
{
	var cnvsPaper = document.getElementById("cnvsPaper");
	var ctxPaper = cnvsPaper.getContext('2d');

	m_cnvsWidth = cnvsPaper.offsetWidth;
	m_cnvsHeight = cnvsPaper.offsetHeight;
	m_dTime = 
	m_fs = parseInt(m_cnvsHeight / 12);
	ctx.beginPath();
	ctx.arc(x, y, m_fs/4*3, 0, 2 * Math.PI);
	ctx.closePath();
	ctx.strokeStyle = "black";
	ctx.lineWidth = 2;
	ctx.stroke();

	ctxPaper
	print();
}