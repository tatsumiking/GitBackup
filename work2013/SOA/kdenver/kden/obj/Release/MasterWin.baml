   M S B A M L   `   `   `  ���� ?  ;kden, Version=1.0.0.4, Culture=neutral, PublicKeyToken=nullclr-namespace:kdenkden      kden.MasterWin   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     !exclr-namespace:kden     -     � Title$  	MasterWin��$	��740��$
��1024��.��� 
������� � RowDefinitions �� $��50 ��� $��50 ��� $��* ��� $��50 �.+�� $��0��.+��� -    ��Name$ 
btnSyouhin��$��商品マスター��$
��Left=�$	��Top=�$��	10,10,0,0q�$	��100���� -   $ btnSeikyusaki��$��請求先マスター��$
��Left=�$	��Top=�$��
120,10,0,0q�$	��100���� -   $ btnSeikyukubun��$��請求先区分��$
��Left=�$	��Top=�$��
230,10,0,0q�$	��100���� -   $ btnSasidasinin��$��差出人マスター��$
��Left=�$	��Top=�$��
340,10,0,0q�$	��100���� -   $ btnFurikomisaki��$��振込先マスター��$
��Left=�$	��Top=�$��
450,10,0,0q�$	��100���� -   $ btnSougi��$��葬家テーブル��$
��Left=�$	��Top=�$��
670,10,0,0q�$	��100���� -   $ 	btnSeikyu��$��請求テーブル��$
��Left=�$	��Top=�$��
780,10,0,0q�$	��100���� -	   $ 	btnHanbai��$��販売テーブル��$
��Left=�$	��Top=�$��
890,10,0,0q�$	��100��� $��1��.+��� $��更新開始日��$	��Top=�$��26��$��
0,10,500,0q�$��14(�$��Right=�$��80���� -
    ��Name$ txtSearchStartDate��$�� ��$��
0,10,400,0q�$��26��$��12(�$	��Top=�$��Right=�$��95���� -   $ btnSelectStartDate��$��
0,10,360,0q�$��26��$	��Top=�$��Right=�$��30��.���� $��pic/calender.png���� $	��～��$	��Top=�$��26��$��
0,10,330,0q�$��14(�$��Right=�$��24���� $��更新終了日��$	��Top=�$��26��$��
0,10,250,0q�$��14(�$��Right=�$��80���� -   $ txtSearchEndDate��$�� ��$��
0,10,150,0q�$��26��$��12(�$	��Top=�$��Right=�$��95���� -   $ btnSelectEndDate��$��
0,10,120,0q�$��26��$	��Top=�$��Right=�$��30��.���� $��pic/calender.png���� -   $ 
btnExtract��$��絞込��$��Right=�$	��Top=�$��	0,10,10,0q�$	��100��� $��2��.+�  kden.DataGridEx  -     Name$ dgTable��   kden.DataGridEx   BeginningEdit$ dgTable_BeginningEdit��   CellEditEnding$ dgTable_CellEditEnding��   CurrentCellChanged$  dgTable_CurrentCellChanged��$��10q�&   System.Windows.Controls.DataGrid   AutoGenerateColumns .  	 � DataContext	 �� ���� � $��3��.+��� -   $ 
btnCsvLoad��$��	CSV読込��$
��Left=�$	��Top=�$��	10,10,0,0q�$	��100���� -   $ 
btnCsvSave��$��	CSV保存��$
��Left=�$	��Top=�$��
120,10,0,0q�$	��100���� -   $ 	btnDelete��$��削除��$
��Left=�$	��Top=�$��
230,10,0,0q�$	��100���� -   $ 	btnUpdate��$��更新��$
��Left=�$	��Top=�$��
340,10,0,0q�$	��100���� -   $ 	btnInitDB��$��テーブル初期化��$	��Top=�$��
0,10,290,0q�$��Right=�$	��100���� -   $ btnTest��$��テストデータ登録��$��Right=�$	��Top=�$��
0,10,150,0q�$	��100���� -   $ 	btnReturn��$��戻る��$��Right=�$	��Top=�$��	0,10,10,0q�$	��100��