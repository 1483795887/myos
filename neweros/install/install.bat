@ECHO OFF
cd install
set masterdisk=O
set logicdisk=P
set toptarget=..\build\target
if not exist %cd%\myos.vhd (start /wait MakeVHD.bat %masterdisk%= %logicdisk%=)
start /wait mount
mkdir %masterdisk%:\root
copy /b /y %toptarget%\bootmgr %masterdisk%:\bootmgr
copy /b /y %toptarget%\setup.exe %masterdisk%:\root\setup.exe
copy /b /y %toptarget%\kernel.exe %masterdisk%:\root\kernel.exe
copy /b /y boot.fnt %masterdisk%:\root\boot.fnt
start /wait unmount