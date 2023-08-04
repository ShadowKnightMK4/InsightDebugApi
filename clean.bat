@ECHO OFF
echo Deleting contents of BuildFolder  that acts at the temp storage for obj files/ect...
rd /q /s "BuildFolder\"
echo Deleting contents of Code folder that receives the exe/dll/lib files on build
rd /q /s "Code\"
echo recreating the folders
md Code
md BuildFolder

