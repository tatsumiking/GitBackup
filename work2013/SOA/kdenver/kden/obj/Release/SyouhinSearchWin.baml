   M S B A M L   `   `   `  ����   kden    kden.SyouhinSearchWin   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     -     � Title$  SyouhinSearchWin��$	��600��$	��500�� � Icon$ kden.ico��.��� 
������� � RowDefinitions �� $��60 ��� $��1* ��� $��50 �.+�� $��0��.+��� $
��Left=�$��10,5,0,0q�$
��Wrap=�$��検索文字列��$	��Top=�$��71���� $
��Left=�$��	315,5,0,0q�$
��Wrap=�$��分類��$	��Top=�$��80���� -    ��Name$ txtSearchName��$
��Left=�$��23��$��	10,27,0,0q� �� TextWrapping$
 Wrap=�$�� ��$	��Top=�$	��300���� -    ��Name$ cmbSearchKind��$��
315,26,0,0q�$	��Top=�$
��Left=�$��80���� -    ��Name$ 	btnSearch��$��検索��$��	0,26,10,0q�$��24��$	��Top=�$��Right=�$��75��� $��1��.+�&   System.Windows.Controls.DataGrid  -     Name$ 	dgSyouhin��$��
10,10,0,10q�   AutoGenerateColumns .  $
��Left=�$	��472��	 � DataContext	 �� ���� � $��2��.+��� -   $ btnOk��$��確定��$��	0,0,99,16q�$��24��$��Bottom=�$��Right=�$��75���� -   $ 	btnCancel��$��キャンセル��$��	0,0,10,16q�$��24��$��Bottom=�$��Right=�$��75��