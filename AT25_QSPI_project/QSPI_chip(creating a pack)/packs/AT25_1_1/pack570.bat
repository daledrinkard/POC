echo off
echo "Pack 5.7.0  e2studio 2024-10"
color 0A
set packs6="C:\Users\daled\.eclipse\com.renesas.platform_351115138\internal\projectgen\ra\packs\"
set modules6="C:\Users\daled\.eclipse\com.renesas.platform_351115138\internal\projectgen\ra\modules\
set xml7="Renesas##HAL Drivers##all##r_at25####5.7.0.xml"

cls
echo "press space to remove 5.7.0 pack from e2studio 2024-10 support area pack folder"
pause
del /Q %packs6%"Renesas.AT25.5.7.0.pack"
echo "press space to remove the module description from the from e2studio 2024-10 support area modules folder"
pause
del /Q "%modules6%%xml7%"

echo "press space to remove the pack files from the e2studio 2024-10 install folders"
pause
del /Q C:\Renesas\e2_studio_v2024-01\internal\projectgen\ra\packs\Renesas.AT25.5.7.0.pack

echo "press space to remove installedpacks.xml from the e2studio 2024-10 support folder"
pause
del /Q %modules6%"installedpacks.xml"

echo "press space to rebuild pack"
pause
echo "Removing pack file from local folder"
del /Q Renesas.AT25.5.7.0.pack
echo "rebuilding 5.7.0"
cd Renesas.AT25.5.7.0
c:\progra~1\7-Zip\7z a -r -tzip Renesas.AT25.5.7.0.pack @../filelist.txt
move Renesas.AT25.5.7.0.pack ..
cd ..
echo  "5.7.0 created"
pause


