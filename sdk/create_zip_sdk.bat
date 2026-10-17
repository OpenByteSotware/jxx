@echo off
REM ============================================================
REM Create sdk.c (actually a ZIP archive with .c extension)
REM ============================================================

set "outFile=sdk.c"
set "tempZip=sdk.zip"

REM Remove old files
if exist "%tempZip%" del "%tempZip%"
if exist "%outFile%" del "%outFile%"

REM Create ZIP
powershell -NoProfile -Command ^
    "Compress-Archive -Path '%cd%\*' -DestinationPath '%tempZip%' -Force"

REM Verify creation
if not exist "%tempZip%" (
    echo ERROR: Archive creation failed.
    exit /b 1
)

REM Rename ZIP to .c
ren "%tempZip%" "%outFile%"

echo Created %outFile%
