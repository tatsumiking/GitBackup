   M S B A M L   `   `   `  ����   
WpfUdpTest    WpfUdpTest.MainWindow   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     -     �Name$  
windowMain�� � Title$ UDP送受信テスト��$	��420��$	��650�� � Icon$ WpfUdpTest.ico��.��� .+��� -    ��Name$ 
txtMessage��$	��230��$��	10,10,0,0q�$	��590��$��TextBox��$
��Left=�$	��Top=� �� AcceptsReturn . " �� VerticalScrollBarVisibility$
 Auto=�$ �� HorizontalScrollBarVisibility$
 Auto=��� -    ��Name$ lblIpAddress��$��IPアドレス��$��
10,245,0,0q�$��77��$��28��$
��Left=�$	��Top=��� -   $ txtIpAddress��$��192.168.1.20��$��
90,247,0,0q�$	��120��$��23��$
��Left=�$	��Top=��� -   $ lblPort��$��ポート番号��$��210,245,0,0q�$��77��$��28��$
��Left=�$	��Top=��� -   $ txtPort��$��12345��$��290,247,0,0q�$��60��$��23��$
��Left=�$	��Top=��� $��自分のIPアドレス��$��360,245,0,0q�$	��115��$��28��$
��Left=�$	��Top=��� -    ��Name$ cmbSelfIpAddress��$��480,245,0,0q�$	��120��$
��Left=�$	��Top=�	� -   	 	�Name$	 rdoSend��$��	UDP送信��$��
10,285,0,0q�$
��Left=�$	��Top=�$��80��
 	� 	GroupName$
 groupUdpMode�� d� 	IsChecked$
 TrueY�	� -	   $	 
rdoReceive��$��	UDP受信��$��100,285,0,0q�$
��Left=�$	��Top=�$��80��$
 groupUdpMode���� -
    ��Name$ btnUdpStart��$��送信開始��$	��100��$��30��$��380,285,0,0q�$
��Left=�$	��Top=��� -   $ 
btnUdpStop��$��送信停止��$	��100��$��30��$��500,285,0,0q�$
��Left=�$	��Top=��� -   $ lblInformation��$�� ��$��
10,330,0,0q�$	��470��$��28��$
��Left=�$	��Top=��� -   $ btnExit��$��終了��$	��100��$��30��$��500,330,0,0q�$
��Left=�$	��Top=�