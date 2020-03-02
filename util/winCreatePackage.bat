cd klog
rmdir /S /Q build
rmdir /S /Q release
rmdir /S /Q debug
qmake -config release KLog.pro
mingw32-make
xcopy /Y /S /F build\target release
windeployqt release\klog.exe
copy 
copy ..\paquete\hamlibDLL\*.dll release
:: The SSL DLLs must be included and must match the version that were used to build Qt.
:: Check in main.cpp and uncomment the SSL line to see what is the version that was used.
:: After knowing the version, the package can be obtained from: https://indy.fulgan.com/SSL/Archive/
copy ..\paquete\openssl\*.dll release
copy *.ico release
copy AUTHORS release
copy Changelog release
copy NEWS release
copy TODO release
copy README release
copy COPYING release
copy gpl-3.0-standalone.html release
