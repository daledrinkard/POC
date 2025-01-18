echo off
color 0A
cls
echo press space to remove Renesas.AT25.5.6.0.pack from the support folder
pause
del /Q C:\Users\daled\.eclipse\com.renesas.platform_351115138\internal\projectgen\ra\packs\Renesas.AT25.5.6.0.pack
echo press space to remove Renesas##HAL Drivers##all##r_at25####5.6.0.xml from the support folder modules folder
pause
del /Q "C:\Users\daled\.eclipse\com.renesas.platform_351115138\internal\projectgen\ra\modules\Renesas##HAL Drivers##all##r_at25####5.6.0.xml"
echo press space to remove Renesas.AT25.5.6.0.pack from the install folder
pause
del /Q C:\Renesas\e2_studio_v2024-10\internal\projectgen\ra\packs\Renesas.AT25.5.6.0.pack
echo press space to remove installedpacks.xml from the support folder
pause
del /Q "C:\Users\daled\.eclipse\com.renesas.platform_351115138\internal\projectgen\ra\modules\installedpacks.xml"
echo press space to rebuild the pack file or ctrl C to exit now
pause
echo Removing Renesas.AT25.5.6.0 from local folder
del /Q Renesas.AT25.5.6.0.pack
echo rebuilding pack file
cd Renesas.AT25.5.6.0
c:\progra~1\7-Zip\7z a -r -tzip Renesas.AT25.5.6.0.pack @../filelist.txt
move Renesas.AT25.5.6.0.pack ..
cd ..
echo  Pack created. 
pause


