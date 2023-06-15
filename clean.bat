@ECHO OFF
echo Easy Clean is delete the contents of the BuildFolder and Code folder without deleting said folders. This deletes the directory contents
rd /q /s "BuildFolder\"
rd /q /s "Code\"
md Code
md 

