   M S B A M L   `   `   `  ���� @  <instr, Version=1.0.0.4, Culture=neutral, PublicKeyToken=nullclr-namespace:instrinstr      instr.MainWindow   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     "exclr-namespace:instr     -     � Title$  
MainWindow��$	��740��$
��1024�� � Icon$ 	instr.ico��.���  � ColumnDefinitions �� $	��200 ��� $��* �.+�� $��0�� � RowDefinitions �� $��80 ��� $��* ��� $	��140 �.+�� $��0��.+��� $��フォルダ名��$
��Left=�$��30��$��	10,10,0,0q�$	��Top=�$��77���� -    ��Name$ btnFolderSelect��$��枠フォルダ��$
��Left=�$��
115,10,0,0q�$	��Top=�$��75���� -    ��Name$ lblFolderName��$��名札フォルダ��$
��Left=�$��30��$��	10,45,0,0q�$	��Top=�$	��180��� $��1��.+��� -    ��Name$ canvasImage��$��10,10,10,10q����	#FFD1D1D1� $��2��.+��� -   $ btnSave��$��保存��$
��Left=�$��
10,0,0,106q�$��Bottom=�$��75���� -   $ btnLoad��$��呼出��$
��Left=�$��
115,10,0,0q�$	��Top=�$��75���� -   $ btnList��$��一覧印刷��$
��Left=�$��	10,39,0,0q�$	��Top=�$��75���� -   $ btnOut��$��
出力[F1]��$
��Left=�$��
10,100,0,0q�$	��Top=�$��75���� -	   $ btnExit��$��終了[F12]��$
��Left=�$��115,100,0,0q�$	��Top=�$��75��� $��1��.+�   instr.DataGridEx  -
     Name$ dgNafuda��   instr.DataGridEx   CurrentCellChanged$! dgNafuda_CurrentCellChanged��	   BeginningEdit$	 dgNafuda_BeginningEdit��
   CellEditEnding$
 dgNafuda_CellEditEnding��$��10,10,10,10q�&   System.Windows.Controls.DataGrid   CanUserSortColumns .     AutoGenerateColumns .   � DataContext �� ���� 