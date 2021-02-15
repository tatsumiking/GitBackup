using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Printing;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace kden
{
    public partial class SougiSeikyuWin : Window
    {
        private double m_dXTime, m_dYTime;
        private string[] m_sKubunLine;

        private void SeikyusyoPrintOut()
        {
            string[] sAry;
            PrintDialog dPrt;
            PageImageableArea area;
            Canvas canvas;
            FixedPage page;
            PageContent cont;
            FixedDocument doc;

            dPrt = new PrintDialog();
            if (dPrt.ShowDialog() == true)
            {
                area = dPrt.PrintQueue.GetPrintCapabilities().PageImageableArea;
                m_dXTime = (area.OriginWidth + area.ExtentWidth) / 210.0;
                m_dYTime = (area.OriginWidth + area.ExtentWidth) / 297.0;

                doc = new FixedDocument();
                canvas = new Canvas();
                canvas.Width = area.OriginWidth + area.ExtentWidth;
                canvas.Height = area.OriginHeight + area.ExtentHeight;

                m_libCanvas.setFontSize(25.0);
                m_libCanvas.setFontName("ＭＳ 明朝");
                m_libCanvas.setFontNomal();
                m_libCanvas.setBrushFill(-1, -1, -1);
                m_libCanvas.setBrushStroke(0, 0, 0, 1, 0);
                m_libCanvas.setBrushText(0, 0, 0);

                mainSeikyusyoPrintOut(canvas);

                page = new FixedPage();
                page.Children.Add(canvas);
                cont = new PageContent();
                cont.Child = page;
                doc.Pages.Add(cont);

                dPrt.PrintDocument(doc.DocumentPaginator, "請求書");
            }
        }

        private void mainSeikyusyoPrintOut(Canvas canvas)
        {
            double sx, sy, ex, ey;
            double wd, hi;
            double space, time;
            double dFs;
            string sStr;
            string[] sAry;
            int max, idx;
            double m_nCrtY;
            int nSumPrice;
            string sKokubetu;

            space = 0.0;
            time = 1.0;
            dFs = 10.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = 80.0 * m_dXTime;
            sy = 20.0 * m_dYTime;
            sStr = "請　求　書";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            sAry = m_aryRecLine[0].Split(',');
            sKokubetu = sAry[6];
            dFs = 6.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = 20.0 * m_dXTime;
            sy = 40.0 * m_dYTime;
            sStr = sAry[17];
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = 110.0 * m_dXTime;
            sStr = "様";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = 20.0 * m_dXTime;
            sy = 46.0 * m_dYTime;
            ex = 120.0 * m_dXTime;
            ey = 46.0 * m_dYTime;
            m_libCanvas.drawLine(canvas, sx, sy, ex, ey);
            dFs = 4.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = 25.0 * m_dXTime;
            sy = 48.0 * m_dYTime;
            sStr = "下記のとおりご請求申し上げます。";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            dFs = 4.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = 130.0 * m_dXTime;
            sy = 40.0 * m_dYTime;
            sStr = "株式会社京阪御所センター";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            dFs = 6.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = 130.0 * m_dXTime;
            sy = 45.0 * m_dYTime;
            sStr = "寝屋川玉泉院";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            dFs = 3.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = 130.0 * m_dXTime;
            sy = 51.0 * m_dYTime;
            sStr = "〒572-0066 寝屋川市仁和寺町12-34";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            dFs = 3.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = 150.0 * m_dXTime;
            sy = 54.0 * m_dYTime;
            sStr = "電話　072-828-4242";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sy = 57.0 * m_dYTime;
            sStr = "FAX 　072-828-3432";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            sx = 20.0 * m_dXTime;
            sy = 60.0 * m_dYTime;
            wd = 130.0 * m_dXTime;
            hi = 20.0 * m_dYTime;
            m_libCanvas.drawBoxs(canvas, sx, sy, wd, hi);
            dFs = 3.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = 21.0 * m_dXTime;
            sy = 61.0 * m_dYTime;
            sStr = "指定コード番号　お振込名";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            dFs = 4.0 * m_dXTime;
            m_libCanvas.setFontSize(dFs);
            sx = 40.0 * m_dXTime;
            sy = 68.0 * m_dYTime;
            sStr = sAry[24];
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            dFs = 3.5 * m_dXTime;
            m_libCanvas.setFontSize(dFs);
            sx = 20.0 * m_dXTime;
            sy = 80.0 * m_dYTime;
            // 複数行に対応する必要あり
            sStr = txtBunsyou1.Text;
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            dFs = 6.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = 80.0 * m_dXTime;
            sy = 100.0 * m_dYTime;
            sStr = sAry[5];
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            dFs = 5.5 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = (80.0 + 6.0 * sAry[5].Length) * m_dXTime;
            sy = 100.0 * m_dYTime;
            sStr = "供花代金";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            max = m_aryRecLine.Length;
            sx = 20.0 * m_dXTime;
            sy = 106.0 * m_dYTime;
            wd = 170.0 * m_dXTime;
            hi = 70.0 * m_dYTime;
            m_libCanvas.drawBoxs(canvas, sx, sy, wd, hi);
            ex = 190.0 * m_dXTime;
            sy = 116.0 * m_dYTime;
            ey = 116.0 * m_dYTime;
            m_libCanvas.drawLine(canvas, sx, sy, ex, ey);
            sy = 120.0 * m_dYTime;
            ey = 120.0 * m_dYTime;
            m_libCanvas.drawLine(canvas, sx, sy, ex, ey);
            sx = 55.0 * m_dXTime;
            ex = 55.0 * m_dXTime;
            sy = 116.0 * m_dYTime;
            ey = 176.0 * m_dYTime;
            m_libCanvas.drawLine(canvas, sx, sy, ex, ey);

            dFs = 3.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = 21.0 * m_dXTime;
            sy = 107.0 * m_dYTime;
            sStr = "通夜";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = 56.0 * m_dXTime;
            sStr = "告別式";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = 91.0 * m_dXTime;
            sStr = "施工場所";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            dFs = 4.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = 21.0 * m_dXTime;
            sy = 110.0 * m_dYTime;
            sStr = txtTuyaYY.Text + "/" + txtTuyaMM.Text + "/" + txtTuyaDD.Text;
            sStr = m_libCmn.convWakeki(sStr);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = 56.0 * m_dXTime;
            sStr = m_libCmn.convWakeki(sKokubetu);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = 91.0 * m_dXTime;
            sStr = txtSekoubasyo.Text;
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            dFs = 3.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            sx = 30.0 * m_dXTime;
            sy = 117.0 * m_dYTime;
            sStr = "品名・個数・金額";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = 120.0 * m_dXTime;
            sy = 117.0 * m_dYTime;
            sStr = "供花名札";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            dFs = 4.0 * m_dYTime;
            m_libCanvas.setFontSize(dFs);
            m_nCrtY = 121.0;
            nSumPrice = 0;
            max = m_aryRecLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                sAry = m_aryRecLine[idx].Split(',');
                sx = 21.0 * m_dXTime;
                sy = m_nCrtY * m_dYTime;
                sStr = sAry[7];
                m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

                sx = 56.0 * m_dXTime;
                sStr = getFudaStr(sAry);
                m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

                nSumPrice = nSumPrice + m_libCmn.StrToInt(sAry[13]);
                m_nCrtY = m_nCrtY + 5.0;
            }

            sx = 40.0 * m_dXTime;
            sy = 180.0 * m_dYTime;
            sStr = "御請求合計金額";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            sx = 80.0 * m_dXTime;
            sStr = nSumPrice.ToString("C")+"也　(消費税込)";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);


            sx = 20.0 * m_dXTime;
            sy = 200.0 * m_dYTime;
            sStr = "お振込みされるお客様へ";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sy = 205.0 * m_dYTime;
            sStr = "誠に勝手ではございますが、振込手数料はお客様にてご負担くださいませ。";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sy = 210.0 * m_dYTime;
            sStr = "上記番号・上記名　以外で振込をされますと処理に行き違いが生じますのでお手数ですが";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sy = 215.0 * m_dYTime;
            sStr = "必ず指定の振込番号・お名前をご入力いただきますようお願い申し上げます。";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            sAry = txtBunsyou2.Text.Split('\n');
            m_nCrtY = 225.0;
            max = sAry.Length;
            for (idx = 0; idx < max; idx++)
            {
                sy = m_nCrtY * m_dYTime;
                m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sAry[idx]);
                m_nCrtY = m_nCrtY + 5.0;
            }
        }
        private string getFudaStr(string[] sAry)
        {
            string sFuda;

            sFuda = sAry[28] + " " + sAry[29] + " " + sAry[30] + " " + sAry[31] + " ";
            sFuda = sFuda + sAry[32] + " " + sAry[33] + " " + sAry[34] + " " + sAry[35];
            return (sFuda);
        }
    }
}
