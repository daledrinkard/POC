echo off
echo "Pack 5.8.0  e2studio 2025-1"
color 0A
set packs7="C:\Users\daled\.eclipse\com.renesas.platform_1832973377\internal\projectgen\ra\packs\"
set modules7="C:\Users\daled\.eclipse\com.renesas.platform_1832973377\internal\projectgen\ra\modules\"
set xml8="Renesas##HAL Drivers##all##r_at25####5.8.0.xml"
set mdf8="%modules7%%xml8%"

cls
echo "press space to remove 5.8.0 pack from e2studio 2025-1 support area pack folder"
pause
del /Q %packs7%"Renesas.AT25.5.8.0.pack"
echo "press space to remove the module description from the from e2studio 2025-1 support area modules folder"
pause
rem del /Q "%modules7%%xml8%"
del /Q "%mdf8%"

echo "press space to remove the pack file from the e2studio 2025-1 install folders"
pause
del /Q C:\Renesas\e2_studio_v2025-01\internal\projectgen\ra\packs\Renesas.AT25.5.8.0.pack

echo "press space to remove installedpacks.xml from the support folder"
pause
del /Q %modules7%"installedpacks.xml"

echo "press space to rebuild pack"
pause
echo "Removing pack files from local folder"
del /Q Renesas.AT25.5.8.0.pack
echo "rebuilding 5.8.0"
cd Renesas.AT25.5.8.0
c:\progra~1\7-Zip\7z a -r -tzip Renesas.AT25.5.8.0.pack @../filelist.txt
move Renesas.AT25.5.8.0.pack ..
cd ..
echo  "5.8.0 created"
pause


