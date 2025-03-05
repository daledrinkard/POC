echo off
color 0A
set packs7="C:\Users\daled\.eclipse\com.renesas.platform_351115138\internal\projectgen\ra\packs\"
set modules7="C:\Users\daled\.eclipse\com.renesas.platform_351115138\internal\projectgen\ra\modules\
set xml7="Renesas##HAL Drivers##all##r_at25####5.7.0.xml"
set sp7="%modules7%%xml7%"

cls
echo "press space to remove all the *.pack files from all support folders."
pause
del /Q %packs7%"Renesas.AT25.5.7.0.pack"
echo "press space to remove the *.xml files in the support folders."
pause
del /Q "%modules7%%xml7%"

echo "press space to remove the pack files from the e2studio install folders"
pause
del /Q C:\Renesas\e2_studio_v2024-01\internal\projectgen\ra\packs\Renesas.AT25.5.6.0.pack
del /Q C:\Renesas\e2_studio_v2024-01\internal\projectgen\ra\packs\Renesas.AT25.5.7.0.pack

echo "press space to remove installedpacks.xml from the support folder"
pause
del /Q %modules7%"installedpacks.xml"

echo "press space to rebuild packs"
pause
echo "Removing pack files from local folder"
del /Q Renesas.AT25.5.7.0.pack
echo "rebuilding 5.7.0"
cd Renesas.AT25.5.7.0
c:\progra~1\7-Zip\7z a -r -tzip Renesas.AT25.5.7.0.pack @../filelist.txt
move Renesas.AT25.5.7.0.pack ..
cd ..
echo  "5.7.0 created"
pause
echo "rebuilding BSP pack"
cd Rocket.I2C.5.7.0
c:\progra~1\7-Zip\7z a -r -tzip Rocket.I2C.5.7.0.pack @../filelist_BSP.txt
move Rocket.I2C.5.7.0.pack ..
cd ..
echo  "Rocket BSP created"


