# Microsoft Developer Studio Project File - Name="plugin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=plugin - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "TDEmitterv1.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "TDEmitterv1.mak" CFG="plugin - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "plugin - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plugin - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "plugin - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "./obj/sdk_rel"
# PROP Intermediate_Dir "./obj/sdk_rel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".\API" /I ".\API\c4d_preview" /I ".\res" /D "WIN32" /D "__PC" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_EXPORTS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"TDEmitterv1.cdl"

!ELSEIF  "$(CFG)" == "plugin - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "./obj/sdk_deb"
# PROP Intermediate_Dir "./obj/sdk_deb"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /Od /I ".\API" /I ".\API\c4d_preview" /I ".\res" /D "WIN32" /D "__PC" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_EXPORTS" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"TDEmitterv1.cdl" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "plugin - Win32 Release"
# Name "plugin - Win32 Debug"
# Begin Group "Source Code"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "api"

# PROP Default_Filter ""
# Begin Group "c4d_scaling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\api\c4d_scaling\mergelin.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_scaling\miniblt.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_scaling\scale.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_scaling\scaleitf.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_scaling\scalelin.cpp
# End Source File
# End Group
# Begin Group "c4d_preview"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\api\c4d_preview\filter_prefs.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_preview\filterpreview.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_preview\filterthread.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_preview\linefetchutil.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_preview\matrixrotate.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_preview\matrixutil.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_preview\matrixwrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_preview\pixelfetchutil.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\api\c4d_basebitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_basechannel.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_basecontainer.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_basedocument.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_basedraw.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_basekey.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_baselist.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_basematerial.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_baseobject.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_baseselect.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_basesequence.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_basesound.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_basetag.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_basetime.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_basetrack.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_bitmapfilter.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_file.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_general.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_gui.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_listview.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_memory.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_painter.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_particleobject.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_pmain.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_resource.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_string.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_thread.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_toolplugin.cpp
# End Source File
# Begin Source File

SOURCE=.\api\c4d_tools.cpp
# End Source File
# Begin Source File

SOURCE=.\api\ge_sort.cpp
# End Source File
# Begin Source File

SOURCE=.\api\OperatingSystem.cpp
# End Source File
# End Group
# Begin Group "PolyEm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\source\polyem\PolyEm.cpp
# End Source File
# End Group
# Begin Group "HELP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\source\BitmapPreview.cpp
# End Source File
# Begin Source File

SOURCE=.\source\ChSampler.cpp
# End Source File
# Begin Source File

SOURCE=.\source\mytools.cpp
# End Source File
# End Group
# Begin Group "MAIN"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\source\Main.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\source\ESPTag.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PTPTag.cpp
# End Source File
# Begin Source File

SOURCE=.\source\TDEmitter.cpp

!IF  "$(CFG)" == "plugin - Win32 Release"

!ELSEIF  "$(CFG)" == "plugin - Win32 Debug"

# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\source\TDMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\source\VDSampler.cpp
# End Source File
# End Group
# Begin Group "Res"

# PROP Default_Filter ""
# Begin Group "dialogs"

# PROP Default_Filter "res"
# Begin Source File

SOURCE=.\res\dialogs\dlg_esp.res
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\dialogs\dlg_ptp.res
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\dialogs\DLG_TDEMITTER.res
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "strings_de"

# PROP Default_Filter "str"
# Begin Source File

SOURCE=.\res\strings_de\c4d_strings.str
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\strings_de\dialogs\dlg_esp.str
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\strings_de\dialogs\dlg_ptp.str
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\strings_de\dialogs\DLG_TDEMITTER.str
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "strings_us"

# PROP Default_Filter "str"
# Begin Source File

SOURCE=.\res\strings_us\c4d_strings.str
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\strings_us\dialogs\dlg_esp.str
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\strings_us\dialogs\dlg_ptp.str
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\strings_us\dialogs\DLG_TDEMITTER.str
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\c4d_symbols.h
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
