; CLW ファイルは MFC ClassWizard の情報を含んでいます。

[General Info]
Version=1
LastClass=CTonDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mps.h"

ClassCount=3
Class1=CTonApp
Class2=CTonDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MPS_DIALOG

[CLS:CTonApp]
Type=0
HeaderFile=mps.h
ImplementationFile=mps.cpp
Filter=N

[CLS:CTonDlg]
Type=0
HeaderFile=mpsDlg.h
ImplementationFile=mpsDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_BTNRELOAD

[CLS:CAboutDlg]
Type=0
HeaderFile=mpsDlg.h
ImplementationFile=mpsDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MPS_DIALOG]
Type=1
Class=CTonDlg
ControlCount=22
Control1=IDC_STTOP,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDFILENAME,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BTNREF,button,1342242816
Control6=IDC_EDSRCXDOT,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDSRCYDOT,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDDSTXMM,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDDSTYMM,edit,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_BTNSEND,button,1342242816
Control20=IDC_BTNEND,button,1342242816
Control21=IDC_STIMG,static,1342177287
Control22=IDC_BTNRELOAD,button,1342242816

