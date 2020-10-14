@echo off
if not defined DevEnvDir (
    CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"  -arch=amd64
    )
