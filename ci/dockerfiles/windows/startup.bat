@echo off
if not defined DevEnvDir (
    CALL c:/BuildTools/Common7/Tools/VsDevCmd.bat -arch=amd64 -host_arch=amd64
    )
