:BUILD
echo off
echo Compiling...
echo.

if %2 == clean  goto BUILD_CLEAN
if %2 == all    goto BUILD_ALL
goto DIRECTIONS

:BUILD_CLEAN
if %1 == Debug   goto CLEAN_DEBUG
if %1 == debug   goto CLEAN_DEBUG
if %1 == Release goto CLEAN_RELEASE
if %1 == release goto CLEAN_RELEASE
goto DIRECTIONS
:CLEAN_DEBUG
cs-make -f makefile CFG=Debug clean 
goto EXIT
:CLEAN_RELEASE
cs-make -f makefile CFG=Release clean
goto EXIT
:BUILD_ALL
if %1 == Debug   goto BUILD_DEBUG
if %1 == debug   goto BUILD_DEBUG
if %1 == Release goto BUILD_RELEASE
if %1 == release goto BUILD_RELEASE
goto DIRECTIONS
:BUILD_DEBUG
cs-make -f makefile CFG=Debug clean all 
goto OUTPUT
:BUILD_RELEASE
cs-make -f makefile CFG=Release clean all 
goto OUTPUT

:OUTPUT
echo.
echo ************************* Done *************************
echo.

goto EXIT

:DIRECTIONS
echo  Invalid parameters %1 %2
echo  Invoke as follows: rebuild config buildtype
echo       where config    = current config mode of IDE (Release/Debug) 
echo             buildtype = all or clean
echo           
echo.
pause

:EXIT
echo.