using System.Windows.Controls;
using System.Windows.Input;

namespace kden
{
    class DataGridEx : DataGrid
    {
        public DataGridEx()
            : base()
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
 
        public void MoveNextCell()
        {
            DataGridColumn currentcol = this.CurrentColumn;
            // 現在のカラムが最大かどうか
            if (currentcol.DisplayIndex < (this.Columns.Count -1))
            {
                // 編集を終了して次のカラムへ
                //this.CommitEdit();
                this.CurrentColumn = this.Columns[currentcol.DisplayIndex + 1];
                this.BeginEdit();
            }
        }
        public void SetSelect(int nLine, int nColumn)
        {
            //this.CommitEdit();
            this.SelectedIndex = nLine;
            this.CurrentCell = new DataGridCellInfo(this.Items[nLine], this.Columns[nColumn]);
            if (nLine != 0)
            {
                this.CurrentColumn = this.Columns[nColumn + 1];
            }
            this.BeginEdit();
        }
    }
}
