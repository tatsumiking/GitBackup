   M S B A M L   `   `   `  ����  kden    kden.SelectRecWin   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   -   5          � Title$  SelectRecWin��5   	   $	��700��6   $	��700��6+   .��5      �  � RowDefinitions 5   
   �� 5      $��50 �6   �� 5      $��* �6   �� 5	      $��50 �6   .+�5   
   � $��0��6   
�������.+�5      �� $��30��6   $��10,10,110,10q�6"   $�� ��68   �� 5      -   5         ��Name$ 	btnSearch��5      $��検索��6(   $��
0,10,10,10q�65   $��Right=�6I   $	��100��6e   6q   5      � 5   
   $��1��6   
�������.+�5      &   System.Windows.Controls.DataGrid  -   5          Name$ dgTable��5      $��10q�6(    � DataContext 5      �� ��6/   �� 5      5      � 5   
   $��2��6   
�������.+�5      �� -   5        $ 	btnSelect��5      $��選択��6(   $��
0,10,130,0q�65   $	��Top=�6I   $��Right=�6a   $	��100��6}   6�   �� 5      -   5        $ 	btnCancel��5      $��Cancel��6(   $��	0,10,10,0q�69   $	��Top=�6L   $��Right=�6d   $	��100��6�   6�   5      5      5      