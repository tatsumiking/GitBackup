   M S B A M L   `   `   `  ����  kden    kden.SyouhinSearchWin   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   -   5          � Title$  SyouhinSearchWin��5   	   $	��600��6"   $	��500��6/    � Icon$ kden.ico��6R   .��5      � 
������� � RowDefinitions 5   
   �� 5      $��60 �6   �� 5      $��1* �6   �� 5	      $��50 �6   .+�5   
   � $��0��6   .+�5      �� $
��Left=�6   $��10,5,0,0q�63   $
��Wrap=�6E   $��検索文字列��6Y   $	��Top=�6f   $��71��6~   �� 5      $
��Left=�6   $��	315,5,0,0q�63   $
��Wrap=�6F   $��分類��6Z   $	��Top=�6d   $��80��6|   �� 5      -   5         ��Name$ txtSearchName��5      $
��Left=�6-   $��23��6H   $��	10,27,0,0q�6T    �� TextWrapping$
 Wrap=�6g   $�� ��6{   $	��Top=�6�   $	��300��6�   �� 5      -   5         ��Name$ cmbSearchKind��5      $��
315,26,0,0q�6.   $	��Top=�6B   $
��Left=�6Z   $��80��6u   �� 5      -   5         ��Name$ 	btnSearch��5      $��検索��6(   $��	0,26,10,0q�65   $��24��6H   $	��Top=�6T   $��Right=�6l   $��75��6�   6�   5      � 5   
   $��1��6   .+�5      &   System.Windows.Controls.DataGrid  -   5          Name$ 	dgSyouhin��5      $��
10,10,0,10q�6*      AutoGenerateColumns .  $
��Left=�6�   $	��472��6�   	 � DataContext	 6>   �� ��6V   �� 6�   5      � 5   
   $��2��6   .+�5      �� -   5        $ btnOk��5      $��確定��6$   $��	0,0,99,16q�61   $��24��6D   $��Bottom=�6P   $��Right=�6k   $��75��6�   6�   �� 5      -   5        $ 	btnCancel��5      $��キャンセル��6(   $��	0,0,10,16q�68   $��24��6K   $��Bottom=�6W   $��Right=�6r   $��75��6�   6�   5      5      5      