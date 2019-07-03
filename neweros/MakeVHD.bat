@ECHO OFF
echo create vdisk file=%cd%\myos.vhd maximum=100 type=fixed>vhdsel
echo select vdisk file=%cd%\myos.vhd>>vhdsel
echo attach vdisk>>vhdsel
echo create partition primary size=50>>vhdsel
echo active>>vhdsel
echo format fs=fat32 label="OS" quick>>vhdsel
echo assign letter=O>>vhdsel
echo create partition extended>>vhdsel
echo create partition logical>>vhdsel
echo format fs=fat32 label="OSDATA" quick>>vhdsel
echo assign letter=P>>vhdsel
echo detach vdisk>>vhdsel
echo exit>>vhdsel
diskpart /s %cd%\vhdsel
del /f /q vhdsel
exit