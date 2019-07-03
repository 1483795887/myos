@ECHO OFF
if not exist %cd%\myos.vhd (start /wait MakeVHD.bat)
set wrkarch=%1
set wrkconfig=%2
nmake -nologo %wrkarch%= %wrkconfig%=