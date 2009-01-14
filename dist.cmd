@SET DSTDIR=randomtools
@SET ZIPFILE=randomtools-win32.zip
@SET TOOLDIR=win32-tools
@IF NOT EXIST %DSTDIR% MKDIR %DSTDIR%
@ECHO Kopieren der EXE-Dateien ...
@FOR %%F IN (ct-rng ct-lotto ct-randcam ct-rng-assessor ct-rng-analyzer) DO @COPY /B /Y /V Release\%%F.exe %DSTDIR%\
@COPY /Y LICENSE.txt %DSTDIR%
@ECHO Zip'pen der EXE-Dateien in %DSTDIR% ...
@%TOOLDIR%\zip.exe -9 %ZIPFILE% %DSTDIR%\*
@%TOOLDIR%\unzip.exe -t -q %ZIPFILE%
