; CLW ファイルは MFC ClassWizard の情報を含んでいます。

[General Info]
Version=1
LastClass=CDialogEditData
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ah32.h"

ClassCount=3
Class1=CAh32App
Class2=CAh32Dlg

ResourceCount=3
Resource2=IDD_EDIT_DATA
Resource1=IDR_MAINFRAME
Class3=CDialogEditData
Resource3=IDD_AH32_DIALOG

[CLS:CAh32App]
Type=0
HeaderFile=ah32.h
ImplementationFile=ah32.cpp
Filter=N
LastObject=CAh32App

[CLS:CAh32Dlg]
Type=0
HeaderFile=ah32Dlg.h
ImplementationFile=ah32Dlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAh32Dlg



[DLG:IDD_AH32_DIALOG]
Type=1
Class=CAh32Dlg
ControlCount=12
Control1=IDC_BTN_INPUTEDIT,button,1342242816
Control2=IDC_BTN_OUTPUT1,button,1342242816
Control3=IDC_BTN_OUTPUT2,button,1342242816
Control4=IDC_BTN_OUTPUT3,button,1342242816
Control5=IDC_BTN_OUTPUT4,button,1342242816
Control6=IDC_BTN_OUTPUT5,button,1342242816
Control7=IDC_BTN_OUTPUT6,button,1342242816
Control8=IDC_BTN_OUTPUT7,button,1342242816
Control9=IDC_BTN_OUTPUT8,button,1342242816
Control10=IDC_BTN_OUTPUT9,button,1342242816
Control11=IDC_BTN_OUTPUT10,button,1342242816
Control12=IDC_BTN_EXIT,button,1342242816

[DLG:IDD_EDIT_DATA]
Type=1
Class=CDialogEditData
ControlCount=3
Control1=IDC_EDIT_DATA,edit,1350635716
Control2=IDCANCEL,button,1342242816
Control3=IDOK,button,1342242816

[CLS:CDialogEditData]
Type=0
HeaderFile=DlEdData.h
ImplementationFile=DlEdData.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialogEditData

