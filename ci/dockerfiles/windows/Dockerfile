# escape=`

ARG FROM_IMAGE=mcr.microsoft.com/dotnet/framework/sdk:4.8
FROM ${FROM_IMAGE}

SHELL ["powershell", "-Command", "$ErrorActionPreference = 'Stop';"]

ARG CHOCOLATEY_SOURCE=https://chocolatey.org/api/v2
ARG ChocolateyEnvironmentDebug=false
ADD ci/dockerfiles/windows/chocolatey/install_choco.ps1 choco/install_choco.ps1
RUN Set-ExecutionPolicy Bypass -Scope Process -Force; `
    ./choco/install_choco.ps1 -packageRepo $env:CHOCOLATEY_SOURCE ; `
    if( -not ([string]::IsNullOrEmpty($Env:CHOCOLATEY_SOURCE ))) `
    { `
        Write-Host "Adding $env:CHOCOLATEY_SOURCE to chocolatey sources" ; `
        choco source add -n=CHOCOLATEY_SOURCE --priority=1 -s=\"$env:CHOCOLATEY_SOURCE\"; `
        choco source remove -n=chocolatey ;`
    }; `
    Remove-Item C:\ProgramData\chocolatey\bin\cpack.exe ; `
    Remove-Item -Force -Recurse ${Env:TEMP}\*


ADD ci/dockerfiles/windows/chocolatey/visual_studio.config choco/visual_studio.config
RUN $CocoPackageInstallConfig = \"choco\\visual_studio.config\" ; `
    C:\ProgramData\chocolatey\bin\choco.exe sources ; `
    Get-Content -Path $CocoPackageInstallConfig ; `
    C:\ProgramData\chocolatey\bin\choco.exe install -y --stoponfirstfailure --no-progress --verbose $CocoPackageInstallConfig; `
    Remove-Item C:\Users\ContainerAdministrator\AppData\Local\Temp\chocolatey -Recurse ; `
    refreshenv ; `
    Remove-Item -Force -Recurse ${Env:TEMP}\* ; `
    if (! (Test-Path 'C:\BuildTools\Common7\Tools\VsDevCmd.bat')) `
    { `
        throw 'VsDevCmd.bat not found' ; `
    }

ADD ci/dockerfiles/windows/chocolatey/packages.config choco/packages.config
RUN C:\ProgramData\chocolatey\bin\choco.exe sources ; `
    C:\ProgramData\chocolatey\bin\choco.exe install -y --stoponfirstfailure --no-progress --verbose choco/packages.config ;`
    Remove-Item C:\Users\ContainerAdministrator\AppData\Local\Temp\chocolatey -Recurse ; `
    refreshenv ; `
    Remove-Item -Force -Recurse ${Env:TEMP}\* ; `
    if (!(Test-Path 'C:\Program Files\Git\cmd\git.exe')) `
    { `
        throw 'git.exe not found' ; `
    }; `
    Write-Host "Finished install packages with Chocolatey"

ADD ci/dockerfiles/windows/startup.bat c:\temp\startup.bat
RUN Set-ItemProperty -Path 'HKLM:\Software\Microsoft\Command Processor' -Name 'AutoRun' -Value "c:\temp\startup.bat"
ARG PYTHON_VERSION=3.8.5
RUN choco install -y python --no-progress --version=$env:PYTHON_VERSION
SHELL ["cmd", "/S", "/C" ]
ADD requirements-dev.txt ci/dockerfiles/python/shared/requirements-ci.txt  c:/requirements/
RUN python -m pip install --upgrade pip &&`
    pip install --upgrade setuptools && `
    pip install wheel && `
    pip install conan &&`
    pip wheel --no-cache --wheel-dir c:/wheels/ `
        -r c:/requirements/requirements-dev.txt  `
        -r c:/requirements/requirements-ci.txt  && `
    pip install  --find-links=c:/wheels/ `
        -r c:/requirements/requirements-dev.txt `
        -r c:/requirements/requirements-ci.txt

ADD conanfile.py c:/temp/conanfile.py
ADD ci/dockerfiles/python/shared/remotes.json C:/Users/ContainerAdministrator/.conan/remotes.json

WORKDIR C:\\BuildTools

RUN conan config init
RUN type C:\Users\ContainerAdministrator\.conan\profiles\default
RUN conan install C:/TEMP/ --build  missing --no-import && `
    conan remove * -b --src -f
