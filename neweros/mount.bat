@ECHO OFF
echo select vdisk file=%cd%\myos.vhd>vhdsel
echo attach vdisk>>vhdsel
echo exit>>vhdsel
diskpart /s %cd%\vhdsel
del /f /q vhdsel