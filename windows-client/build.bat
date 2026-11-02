@echo off
setlocal enabledelayedexpansion

echo ========================================================
echo   Linux Server Helper - Windows Client C++ Compiler
echo ========================================================

set "VS_PATH=C:\Program Files\Microsoft Visual Studio\18\Enterprise"
if not exist "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat" (
    echo [!] vcvars64.bat bulunamadi! Lutfen Visual Studio kurulum yolunu kontrol edin.
    exit /b 1
)

echo [*] Visual Studio 64-bit Gelistirici Ortami Hazirlaniyor...
call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat"

set "ROOT_DIR=%~dp0"
cd /d "%ROOT_DIR%"

if not exist "bin" mkdir "bin"
if not exist "bin\ui" mkdir "bin\ui"
if not exist "bin\resources" mkdir "bin\resources"

echo [*] UI ve kaynak dosyalari kopyalaniyor...
copy /y "src\ui\index.html" "bin\ui\" >nul
copy /y "src\ui\style.css" "bin\ui\" >nul
copy /y "src\ui\app.js" "bin\ui\" >nul
copy /y "resources\app.ico" "bin\resources\" >nul
copy /y "packages\WebView2\build\native\x64\WebView2Loader.dll" "bin\" >nul

echo [*] C++ kaynak kodlari derleniyor (cl.exe /O2 /std:c++17 /utf-8)...

cl.exe /nologo /O2 /W3 /EHsc /std:c++17 /utf-8 ^
    /I "include" ^
    /I "packages\WebView2\build\native\include" ^
    "src\main.cpp" ^
    /Fe:"bin\LinuxServerHelper.exe" ^
    /link ^
    /LIBPATH:"packages\WebView2\build\native\x64" ^
    "WebView2Loader.dll.lib" ^
    "user32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "advapi32.lib" "gdi32.lib" ^
    /SUBSYSTEM:WINDOWS

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================================
    echo   [OK] DERLEME BASARILI!
    echo   Uygulama: %ROOT_DIR%bin\LinuxServerHelper.exe
    echo ========================================================
    del *.obj 2>nul
) else (
    echo.
    echo [!] DERLEME HATASI OLUSTU!
    exit /b %ERRORLEVEL%
)

endlocal
