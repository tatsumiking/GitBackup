using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;

namespace CsvUtil
{
    public partial class MainWindow : Window
    {
        private void lblbtnUnisCsv_MouseLeave(object sender, MouseEventArgs e)
        {
            try
            {
                lblbtnUnisCsv.BorderBrush = Brushes.Black;
                lblbtnUnisCsv.Background = Brushes.LightGray;
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("lblbtnUnisCsv_MouseLeave");
                App.LogOut(ex.ToString());
            }
        }
        private void lblbtnUnisCsv_MouseDown(object sender, MouseButtonEventArgs e)
        {
            try
            {
                lblbtnUnisCsv.BorderBrush = Brushes.LightGray;
                lblbtnUnisCsv.Background = Brushes.LightCyan;
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("lblbtnUnisCsv_MouseDown");
                App.LogOut(ex.ToString());
            }
        }
        private void lblbtnUnisCsv_MouseUp(object sender, MouseButtonEventArgs e)
        {
            try
            {
                lblbtnUnisCsv.BorderBrush = Brushes.Black;
                lblbtnUnisCsv.Background = Brushes.LightGray;
                selectUnisCsvFileName();
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("lblbtnUnisCsv_MouseUp");
                App.LogOut(ex.ToString());
            }
        }

        private void lblbtnTarget_MouseLeave(object sender, MouseEventArgs e)
        {
            try
            {
                lblbtnTarget.BorderBrush = Brushes.Black;
                lblbtnTarget.Background = Brushes.LightGray;
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("lblbtnTarget_MouseLeave");
                App.LogOut(ex.ToString());
            }
        }
        private void lblbtnTarget_MouseDown(object sender, MouseButtonEventArgs e)
        {
            try
            {
                lblbtnTarget.BorderBrush = Brushes.LightGray;
                lblbtnTarget.Background = Brushes.LightCyan;
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("lblbtnSlct_PreviewMouseDown");
                App.LogOut(ex.ToString());
            }
        }
        private void lblbtnTarget_MouseUp(object sender, MouseButtonEventArgs e)
        {
            try
            {
                lblbtnTarget.BorderBrush = Brushes.Black;
                lblbtnTarget.Background = Brushes.LightGray;
                selectTargetFileName();
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("lblbtnSlct_PreviewMouseUp");
                App.LogOut(ex.ToString());
            }
        }

        private void lblbtnMerge_MouseLeave(object sender, MouseEventArgs e)
        {
            try
            {
                lblbtnMerge.BorderBrush = Brushes.Black;
                lblbtnMerge.Background = Brushes.LightGray;
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("lblbtnMerge_MouseLeave");
                App.LogOut(ex.ToString());
            }
        }
        private void lblbtnMerge_MouseDown(object sender, MouseButtonEventArgs e)
        {
            try
            {
                lblbtnMerge.BorderBrush = Brushes.LightGray;
                lblbtnMerge.Background = Brushes.LightCyan;
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("lblbtnMerge_MouseDown");
                App.LogOut(ex.ToString());
            }
        }
        private void lblbtnMerge_MouseUp(object sender, MouseButtonEventArgs e)
        {
            try
            {
                lblbtnMerge.BorderBrush = Brushes.Black;
                lblbtnMerge.Background = Brushes.LightGray;
                margeRecodes();
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("lblbtnMerge_MouseUp");
                App.LogOut(ex.ToString());
            }
        }

        private void lblbtnExit_MouseLeave(object sender, MouseEventArgs e)
        {
            try
            {
                lblbtnExit.BorderBrush = Brushes.Black;
                lblbtnExit.Background = Brushes.LightGray;
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("lblbtnExit_MouseLeave");
                App.LogOut(ex.ToString());
            }
        }
        private void lblbtnExit_MouseDown(object sender, MouseButtonEventArgs e)
        {
            try
            {
                lblbtnExit.BorderBrush = Brushes.LightGray;
                lblbtnExit.Background = Brushes.LightCyan;
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("lblbtnExit_MouseDown");
                App.LogOut(ex.ToString());
            }
        }
        private void lblbtnExit_MouseUp(object sender, MouseButtonEventArgs e)
        {
            try
            {
                lblbtnExit.BorderBrush = Brushes.Black;
                lblbtnExit.Background = Brushes.LightGray;
                exitProgram();
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("lblbtnExit_MouseUp");
                App.LogOut(ex.ToString());
            }
        }
    }
}
