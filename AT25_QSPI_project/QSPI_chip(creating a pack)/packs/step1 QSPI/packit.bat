echo off
color 0A
echo Removing Renesas.AT25.5.6.0 from the support folder
del /Q C:\Users\daled\.eclipse\com.renesas.platform_351115138\internal\projectgen\ra\packs\Renesas.AT25.5.6.0.pack
echo Removing Renesas##HAL Drivers##all##r_qspi####5.6.0.xml from the support folder modules folder
del /Q C:\Users\daled\.eclipse\com.renesas.platform_351115138\internal\projectgen\ra\modules\Renesas##HAL Drivers##all##r_qsspi####5.6.0.xml
echo Removing Renesas.AT25.5.6.0 from local folder
del /Q Renesas.AT25.5.6.0.pack
echo rebuilding pack file
cd Renesas.AT25.5.6.0
c:\progra~1\7-Zip\7z a -r -tzip Renesas.AT25.5.6.0.pack @../filelist.txt
move Renesas.AT25.5.6.0.pack ..
cd ..
echo  Pack created. 


