echo on
rmdir /S/Q Renesas.DaleWare.1.3.3
c:\progra~1\7-Zip\7z x Renesas.DaleWare.1.3.3.pack -oRenesas.DaleWare.1.3.3 -r
md Renesas.DaleWare.1.3.3\.module_descriptions
move Renesas.DaleWare.1.3.3\Renesas##* Renesas.DaleWare.1.3.3\.module_descriptions
cd Renesas.DaleWare.1.3.3\synergy\DaleWare
move ssp ..
cd ..\..\..
pause