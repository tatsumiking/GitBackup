using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace kden
{
    public class clsHanZenCount
    {
        public int nZen;
        public int nHan;

        public clsHanZenCount()
        {
            nZen = 0;
            nHan = 0;
        }
    }
    public class LibCanvas
    {

        private double m_dFontSize;
        private string m_sFontName;
        private FontWeight m_fwFont;
        private HorizontalAlignment m_haText;
        private Brush m_brushFill;
        private Brush m_brushStroke;
        private double m_dStrokeThick;
        private int m_nLineType;
        private Brush m_brushText;
        private double m_dTextAreaXSize;
        private double m_dTextAreaYSize;

        public LibCanvas()
        {
        }
        public void DrawElement(Canvas canvas){
            Line line;
            Rectangle rect;
            Ellipse elli;
            Image img;

            m_nLineType = 0;
            m_brushFill =  Brushes.Black;
            m_brushStroke =  Brushes.Black;

            for (int i = 0; i < 10; i++){
                m_dStrokeThick = 1.0d + (i * 0.5d);
                line = CreateLine(200.0d, 100.0d + (i * 10.0d), 300.0d, 100.0d + (i * 10.0d));
                canvas.Children.Add(line);
            }

            // 矩形の描画
            m_brushStroke = Brushes.Black;
            m_dStrokeThick = 1.0d;
            m_brushFill = Brushes.Blue;
            rect = CreateRect(350.0d, 100.0d, 100.0d, 100.0d);
            canvas.Children.Add(rect);

            // 円の描画
            m_brushFill = Brushes.Green;
            elli = CreateEllipse(500.0d, 100.0d, 100.0d, 100.0d);
            canvas.Children.Add(elli);

            img = CreateImage(10.0d, 10.0d, 100.0d, 100.0d, "c:\\UsesProgram\\camera\\name.bmp");
            canvas.Children.Add(img);

            // テキストの描画
            m_sFontName = "HG教科書体";
            m_dFontSize = 24.0;
            m_brushText = Brushes.Black;
            m_dTextAreaXSize = 500;
            m_dTextAreaYSize = 30;

            drawYokoStrg(canvas, 200.0d, 250.0, 0.0, 1.0, "横文字列");
            drawYokoStrg(canvas, 200.0d, 280.0, 0.0, 0.5, "横文字列");
            drawTateStrg(canvas, 120.0d, 250.0, 0.0, 1.0, "縦文字列");
            drawTateStrg(canvas, 160.0d, 250.0, 0.0, 0.5, "縦文字列");
            m_brushFill = null;
            rect = CreateRect(99.0d, 399.0d, 462.0d, 102.0d);
            canvas.Children.Add(rect);

            m_haText = HorizontalAlignment.Left;
            ContentControl cont = CreateTextContent(120.0d, 400.0d, "左寄せ文字列");
            Matrix matrix = ((MatrixTransform)cont.RenderTransform).Matrix;
            matrix.ScaleAt(0.5, 1.0, 0,0);
            cont.RenderTransform = new MatrixTransform(matrix);
            canvas.Children.Add(cont);
            m_haText = HorizontalAlignment.Center;
            canvas.Children.Add(CreateText(100.0d, 430.0d, "中央寄せ文字列"));
            m_haText = HorizontalAlignment.Right;
            canvas.Children.Add(CreateText(100.0d, 460.0d, "右寄せ文字列"));
        }
        public void setFontSize(double size)
        {
            m_haText = HorizontalAlignment.Left;
            m_dFontSize = size;
        }
        public double getFontSize()
        {
            return(m_dFontSize);
        }
        public void setFontName(string sFontName)
        {
            m_sFontName = sFontName;
        }
        public void setFontNomal()
        {
            m_fwFont = FontWeights.Normal;
        }
        public void setFontBold()
        {
            m_fwFont = FontWeights.Bold;
        }
        public void setBrushFill(int r, int g, int b)
        {
            Color clr;
            if (r == -1 && g == -1 && b == -1)
            {
                m_brushFill = null;
            }
            else
            {
                clr = Color.FromArgb(255, (byte)r, (byte)g, (byte)b);
                m_brushFill = new SolidColorBrush(clr);
            }
        }
        public void setBrushStroke(int r, int g, int b, double thick, int type)
        {
            Color clr;
            if (r == -1 && g == -1 && b == -1)
            {
                m_brushStroke = null;
            }
            else
            {
                clr = Color.FromArgb(255, (byte)r, (byte)g, (byte)b);
                m_brushStroke = new SolidColorBrush(clr);
                m_dStrokeThick = thick;
                m_nLineType = type;
                //l1.StrokeDashArray = new DoubleCollection { 2, 2 };
                //l1.StrokeDashCap = PenLineCap.Round;
                //l1.SnapsToDevicePixels = true;
            }

        }
        public void setBrushText(int r, int g, int b)
        {
            Color clr;
            clr = Color.FromArgb(255, (byte)r, (byte)g, (byte)b);
            m_brushText = new SolidColorBrush(clr);
        }
        public void drawImage(Canvas canvas, double x, double y, double w, double h, string sFileName)
        {
            Image img;

            img = CreateImage(x, y, w, h, sFileName);
            canvas.Children.Add(img);
        }
        public void drawLeftEqualStrg(Canvas canvas, double sx, double sy, double ex, double ey, string sStr)
        {

            double len, lenmax, dFs;
            double space, time;
            clsHanZenCount clsHZ;

            dFs = ey - sy;
            lenmax = ex - sx;
            space = 0.0;
            clsHZ = getZenHanCount(sStr);

            len = clsHZ.nZen * dFs + clsHZ.nHan * dFs / 2;
            if (len < lenmax)
            {
                time = 1.0;
            }
            else
            {
                time = lenmax / len;
            }
            drawYokoStrg(canvas, sx, sy, space, time, sStr);
        }
        public void drawRightEqualStrg(Canvas canvas, double sx, double sy, double ex, double ey, string sStr)
        {

            double len, lenmax, dFs;
            double space, time;
            clsHanZenCount clsHZ;

            dFs = ey - sy;
            lenmax = ex - sx;
            space = 0.0;
            clsHZ = getZenHanCount(sStr);

            len = clsHZ.nZen * dFs + clsHZ.nHan * dFs / 2;
            if (len < lenmax)
            {
                time = 1.0;
                sx = ex - len;
            }
            else
            {
                time = lenmax / len;
            }
            drawYokoStrg(canvas, sx, sy, space, time, sStr);
        }
        public clsHanZenCount getZenHanCount(string str)
        {
            int nByte;
            int nCh;
            clsHanZenCount clsHZ;

            clsHZ = new clsHanZenCount();
            Encoding sjisEnc = Encoding.GetEncoding("Shift_JIS");
            nByte = sjisEnc.GetByteCount(str);
            nCh = str.Length;
            clsHZ.nZen = nByte - nCh;
            clsHZ.nHan = nCh - clsHZ.nZen;
            return (clsHZ);
        }
        public void drawYokoStrg(Canvas canvas, double sx, double sy, double space, double time, string sStr)
        {
            int max, idx;
            string sText;
            double crtx, crty;
            TextBlock tb;

            Canvas cvText = new Canvas();
            Canvas.SetLeft(cvText, sx);
            Canvas.SetTop(cvText, sy);
            max = sStr.Length;
            crtx = 0; crty = 0;
            for (idx = 0; idx < max; idx++)
            {
                sText = sStr.Substring(idx, 1);
                tb = CreateText(crtx, crty, sText);
                cvText.Children.Add(tb);
                if (isHankaku(sText))
                {
                    crtx = crtx + m_dFontSize/2 + space / time;
                }
                else
                {
                    crtx = crtx + m_dFontSize + space / time;
                }
            }
            Matrix matrix = ((MatrixTransform)cvText.RenderTransform).Matrix;
            matrix.ScaleAt(time, 1.0, 0, 0);
            cvText.RenderTransform = new MatrixTransform(matrix);
            canvas.Children.Add(cvText);
        }
        public void drawTateStrg(Canvas canvas, double sx, double sy, double space, double time, string sStr)
        {
            int max, idx;
            string sText;
            double crtx, crty;
            TextBlock tb;
            Matrix matrix;

            Canvas cvText = new Canvas();
            Canvas.SetLeft(cvText, sx);
            Canvas.SetTop(cvText, sy);
            max = sStr.Length;
            crtx = 0; crty = 0;
            for (idx = 0; idx < max; idx++)
            {
                sText = sStr.Substring(idx, 1);
                tb = CreateText(crtx, crty, sText);
                if (checkTateRoundChar(sText) == true)
                {
                    matrix = new Matrix(0, 1, 1, 0, 0, 0);
                    tb.RenderTransform = new MatrixTransform(matrix);
                }
                cvText.Children.Add(tb);
                crty = crty + m_dFontSize + space / time;
            }
            matrix = ((MatrixTransform)cvText.RenderTransform).Matrix;
            matrix.ScaleAt(1.0, time, 0, 0);
            cvText.RenderTransform = new MatrixTransform(matrix);
            canvas.Children.Add(cvText);
        }
        public void drawLine(Canvas canvas, double sx, double sy, double ex, double ey)
        {
            Line line;

            line = CreateLine(sx, sy, ex, ey);
            canvas.Children.Add(line);
        }
        public void drawBoxs(Canvas canvas, double sx, double sy, double wd, double hi)
        {
            Rectangle rect;

            rect = CreateRect(sx, sy, wd, hi);
            canvas.Children.Add(rect);
        }
        public void drawEllipse(Canvas canvas, double sx, double sy, double wd, double hi)
        {
            Ellipse elli;

            elli = CreateEllipse(sx, sy, wd, hi);
            canvas.Children.Add(elli);
        }
        private Line CreateLine(double x1, double y1, double x2, double y2)
        {
            Line line = new Line();
            line.X1 = x1;
            line.Y1 = y1;
            line.X2 = x2;
            line.Y2 = y2;
            line.Stroke = m_brushStroke;
            line.StrokeThickness = m_dStrokeThick;
            if (m_nLineType == 1)
            {
                line.StrokeDashArray = new DoubleCollection { 2, 2 };
                line.StrokeDashCap = PenLineCap.Round;
                line.SnapsToDevicePixels = true;
            }
            return line;
        }

        // Rectangleオブジェクトの生成
        private Rectangle CreateRect(double x, double y, double width, double height)
        {
            Rectangle rect = new Rectangle();
            Canvas.SetLeft(rect, x);
            Canvas.SetTop(rect, y);
            rect.Width = width;
            rect.Height = height;
            rect.Stroke = m_brushStroke;
            rect.StrokeThickness = m_dStrokeThick;
            rect.Fill = m_brushFill;
            return rect;
        }

        // Ellipseオブジェクトの生成
        private Ellipse CreateEllipse(double x, double y, double width, double height)
        {
            Ellipse ellipse = new Ellipse();
            Canvas.SetLeft(ellipse, x);
            Canvas.SetTop(ellipse, y);
            ellipse.Width = width;
            ellipse.Height = height;
            ellipse.Stroke = m_brushStroke;
            ellipse.StrokeThickness = m_dStrokeThick;
            ellipse.Fill = m_brushFill;
            return ellipse;

        }

        // Polygonオブジェクトの生成
        private Polygon CreatePolygon(double x, double y, double width, double height, IEnumerable<Point> points)
        {
            Polygon polygon = new Polygon();
            Canvas.SetLeft(polygon, x);
            Canvas.SetTop(polygon, y);
            polygon.Width = width;
            polygon.Height = height;
            polygon.Stretch = Stretch.None;
            polygon.Stroke = m_brushStroke;
            polygon.StrokeThickness = m_dStrokeThick;
            polygon.Fill = m_brushFill;

            foreach (var point in points)
                polygon.Points.Add(point);

            return polygon;
        }

        // Imageオブジェクトの生成
        private Image CreateImage(double x, double y, double width, double height, string sFileName)
        {
            FileStream fs;
            long size;
            byte[] data;

            fs = new FileStream(sFileName, FileMode.Open, FileAccess.Read);
            size = fs.Seek(0L, SeekOrigin.End);
            fs.Seek(0L, SeekOrigin.Begin);
            data = new byte[size];
            fs.Read(data, 0, (int)size);
            fs.Close();

            MemoryStream stream = new MemoryStream();
            stream.Write(data, 0, data.Length);
            stream.Seek(0, SeekOrigin.Begin);
            Image image = new Image();
            image.Source = BitmapFrame.Create(stream, BitmapCreateOptions.None, BitmapCacheOption.OnLoad);
            stream.Close();

            Canvas.SetLeft(image, x);
            Canvas.SetTop(image, y);
            image.Width = width;
            //image.Height = height;
            image.Stretch = Stretch.Fill;
            return image;
        }

        // TextBlockオブジェクトの生成(配置を指定するためのContentControlでラップする）
        private ContentControl CreateTextContent(double x, double y, string text)
        {
            ContentControl content = new ContentControl();
            Canvas.SetLeft(content, x);
            Canvas.SetTop(content, y);
            content.Width = m_dTextAreaXSize;
            content.Height = m_dTextAreaYSize;
            // ActualSize;
            TextBlock tb = new TextBlock();
            tb.Text = text;
            tb.FontFamily = new FontFamily(m_sFontName);
            tb.FontSize = m_dFontSize;
            tb.FontWeight = m_fwFont;
            tb.Foreground = m_brushText;
            tb.HorizontalAlignment = m_haText;
            content.Content = tb;
            return content;
        }
        private TextBlock CreateText(double x, double y, string text)
        {
            TextBlock tb = new TextBlock();
            Canvas.SetLeft(tb, x);
            Canvas.SetTop(tb, y);
            tb.Text = text;
            tb.FontFamily = new FontFamily(m_sFontName);
            tb.FontSize = m_dFontSize;
            tb.FontWeight = m_fwFont;
            tb.Foreground = m_brushText;
            return tb;
        }
        public static bool isHankaku(string str)
        {
            Encoding sjisEnc = Encoding.GetEncoding("Shift_JIS");
            int num = sjisEnc.GetByteCount(str);
            return num == str.Length;
        }
        public Boolean checkTateRoundChar(string str)
        {
            if (str == "ー" || str == "―" || str == "‐" || str == "～"
             || str == "（" || str == "）" || str == "〔" || str == "〕"
             || str == "［" || str == "］" || str == "｛" || str == "｝"
             || str == "〈" || str == "〉" || str == "《" || str == "》"
             || str == "《" || str == "》" || str == "「" || str == "」"
             || str == "『" || str == "』" || str == "【" || str == "】")
            {
                return (true);
            }
            return (false);
        }
    }
}
