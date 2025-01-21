To use the development environment follow these steps:

Navigate to the packs/latest working pack folder.
edit things in the Renesas.AT25.5.6.0 folder  (or whatever things may have changed to)
be sure to edit the toolingsupport.xml if you cange names or add stuff to module descriptions.

edit the packit.bat file and make sure the paths to the support folder and install folder are correct.

run packit.  It deletes stuff and repacks the RA.AT25.5.6.0.pack file.

At this point, there is no AT25 in the system.  If you restart e2studios it will be as if AT25 does not exist.

In e2studio, do a import->CMSIS pack  and import it.

Now restart e2studios.



