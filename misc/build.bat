@echo off

 :: Uncomment to debug the vsvarsall call
::set VSCMD_DEBUG=1
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

 :: %~dp0 Cool way of getting the d = drive and p = path name of the current batch file.
set BuildDir=%~dp0..\build
 
if not exist %BuildDir% mkdir %BuildDir%
 
pushd %BuildDir%

:: delete pdb file and get rid of output errors
:: del *.pdb > NUL 2> NUL
 
:: compiler flags:
:: /Zi enable debugging information
:: /FC use full path in diagnostics
set CommonCompilerFlags=-Zi -FC 
set CommonLinkerFlags=user32.lib gdi32.lib winmm.lib opengl32.lib

:: Code directory path
set CodePath=..\code\main.cpp
 
cl %CommonCompilerFlags% %CodePath% /link %CommonLinkerFlags%
 
:: Copy dependencies if needed
 
popd