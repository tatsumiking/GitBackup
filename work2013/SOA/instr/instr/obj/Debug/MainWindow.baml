   M S B A M L   `   `   `  ����@  <instr, Version=1.0.0.3, Culture=neutral, PublicKeyToken=nullclr-namespace:instrinstr  5   	       instr.MainWindow   5   	   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   "exclr-namespace:instr     5   	   -   5          � Title$  
MainWindow��5   	   $	��740��6   $
��1024��6)    � Icon$ 	instr.ico��6t   .��5      �  � ColumnDefinitions 5   
   �� 5      $	��200 �6   �� 5	      $��* �6   .+�5   
   � $��0��6    � RowDefinitions 5      �� 5      $��80 �6    �� 5      $��* �6    �� 5      $	��140 �6    .+�5      � $��0��6   .+�5      �� $��フォルダ名��6   $
��Left=�6(   $��30��6C   $��	10,10,0,0q�6O   $	��Top=�6b   $��77��6z   �� 5      -   5         ��Name$ btnFolderSelect��5      $��枠フォルダ��62   $
��Left=�6B   $��
115,10,0,0q�6]   $	��Top=�6q   $��75��6�   6�   �� 5      -   5         ��Name$ lblFolderName��5      $��名札フォルダ��6/   $
��Left=�6@   $��30��6[   $��	10,45,0,0q�6g   $	��Top=�6z   $	��180��6�   5      � 5      $��1��6   .+�5      �� -   5         ��Name$ canvasImage��5      $��10,10,10,10q�6.   ���	#FFD1D1D16D   5      � 5      $��2��6   .+�5      �� -   5        $ btnSave��5      $��保存��6*   $
��Left=�67   $��
10,0,0,106q�6R   $��Bottom=�6f   $��75��6�   6�   �� 5      -   5        $ btnLoad��5      $��呼出��6*   $
��Left=�67   $��
115,10,0,0q�6R   $	��Top=�6f   $��75��6~   6�   �� 5      -   5        $ btnList��5      $��一覧印刷��6*   $
��Left=�69   $��	10,39,0,0q�6T   $	��Top=�6g   $��75��6   6�   �� 5      -   5        $ btnOut��5      $��
出力[F1]��6)   $
��Left=�6:   $��
10,100,0,0q�6U   $	��Top=�6i   $��75��6�   6�   �� 5      -	   5        $ btnExit��5      $��終了[F12]��6*   $
��Left=�6<   $��115,100,0,0q�6W   $	��Top=�6l   $��75��6�   6�   5      5       � 5!   
   $��1��6   .+�5"         instr.DataGridEx  -
   5          Name$ dgNafuda��5"         instr.DataGridEx   CurrentCellChanged$! dgNafuda_CurrentCellChanged��5&      	   BeginningEdit$	 dgNafuda_BeginningEdit��5'      
   CellEditEnding$
 dgNafuda_CellEditEnding��5(      $��10,10,10,10q�5"   .   &   System.Windows.Controls.DataGrid   CanUserSortColumns .     AutoGenerateColumns .   � DataContext 5#      �� ��6/   �� 5(      5*      5+      5,      