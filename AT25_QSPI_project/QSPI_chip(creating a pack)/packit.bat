echo off
color 0A
del /Q Renesas.DaleWare.1.6.0.pack
cd Renesas.DaleWare.1.6.0
c:\progra~1\7-Zip\7z a -r -tzip Renesas.DaleWare.1.6.0.pack @../filelist.txt
move Renesas.DaleWare.1.6.0.pack ..
cd ..
echo  Pack created. 
echo   Press space bar to delete any existing packs or ctrl-C to abort now
pause
del /Q C:\Renesas\e2_studio\internal\projectgen\arm\Packs\Renesas.DaleWare.1.6.0.pack
echo Old pack removed,
echo Ckeck to see that xml is removed from .module_descriptions
echo If not then delete it now 
color 0E
echo press space bar to copy pack to packs folder or ctrl-C to abort now
pause
copy Renesas.DaleWare.1.6.0.pack C:\Renesas\e2_studio\internal\projectgen\arm\Packs
color 0B

