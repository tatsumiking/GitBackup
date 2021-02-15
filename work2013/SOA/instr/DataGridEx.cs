using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;

namespace instr
{
    public class DataGridEx : DataGrid
    {
        public DataGridEx() : base()
        {
        }
 
        protected override void OnKeyDown(KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                this.MoveNextCell();
                e.Handled = true;
            }
            else
            {
                base.OnKeyDown(e);
            }
        }
 
        private void MoveNextCell()
        {
            DataGridColumn currentcol = this.CurrentColumn;
            // 現在のカラムが最大かどうか
            bool isLastCol = (currentcol.DisplayIndex == this.Columns.Count - 1);
            if (!isLastCol)
            {
                // 編集を終了して次のカラムへ
                this.CommitEdit();
                this.CurrentColumn = this.Columns[currentcol.DisplayIndex + 1];
                this.BeginEdit();
            }
            else
            {
                // 現在行取得
                int currentrow = this.Items.IndexOf(this.SelectedItem);
                // 最大行数
                int rowMax = this.Items.Count;
 
                if ((currentrow + 1) != rowMax)
                {
                    // 編集を終了して次行の先頭へ
                    this.CommitEdit();
                    this.SelectedIndex = currentrow + 1;
                    this.CurrentCell = new DataGridCellInfo(this.Items[currentrow + 1], this.Columns[0]);
                    this.BeginEdit();
                }
            }
        }
    }
}
