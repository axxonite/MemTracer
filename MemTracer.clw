; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMemTracerView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MemTracer.h"
LastPage=0

ClassCount=5
Class1=CMemTracerApp
Class2=CMemTracerDoc
Class3=CMemTracerView
Class4=CMainFrame

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDD_MEMTRACER_FORM

[CLS:CMemTracerApp]
Type=0
HeaderFile=MemTracer.h
ImplementationFile=MemTracer.cpp
Filter=N

[CLS:CMemTracerDoc]
Type=0
HeaderFile=MemTracerDoc.h
ImplementationFile=MemTracerDoc.cpp
Filter=N

[CLS:CMemTracerView]
Type=0
HeaderFile=MemTracerView.h
ImplementationFile=MemTracerView.cpp
Filter=D
LastObject=IDC_MAINTREE
BaseClass=CFormView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=MemTracer.cpp
ImplementationFile=MemTracer.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_APP_EXIT
Command2=ID_EDIT_CUT
Command3=ID_EDIT_COPY
Command4=ID_EDIT_PASTE
Command5=ID_APP_ABOUT
CommandCount=5

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[DLG:IDD_MEMTRACER_FORM]
Type=1
Class=CMemTracerView
ControlCount=3
Control1=IDC_MAINTREE,SysTreeView32,1350631447
Control2=IDC_SEP,button,1476460544
Control3=IDC_LOG,edit,1352669380

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

