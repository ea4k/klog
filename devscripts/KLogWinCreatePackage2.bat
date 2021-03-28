@echo off
echo Setting up environment for Qt usage...
set PATH=C:/Qt/5.12.3/mingw73_32/bin;C:/Qt/Tools/mingw730_32/bin;%PATH%
cd ../src/
rmdir /S /Q build
rmdir /S /Q release
rmdir /S /Q debug
mingw32-make clean

qmake src.pro CONFIG=release
mingw32-make
rem mkdir release
rem xcopy /Y /S /F build\target release
rem windeployqt release\klog.exe
rem copy ..\..\paquete\hamlibDLL\*.dll release
:: The SSL DLLs must be included and must match the version that were used to build Qt.
:: Check in main.cpp and uncomment the SSL line to see what is the version that was used.
:: After knowing the version, the package can be obtained from: https://indy.fulgan.com/SSL/Archive/
rem copy ..\..\paquete\openssl\*.dll release
rem copy *.ico release
rem copy AUTHORS release
rem copy Changelog release
rem copy NEWS release
rem copy TODO release
rem copy README release
rem copy COPYING release
rem copy gpl-3.0-standalone.html release
rem cd ..\..



