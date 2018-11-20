@echo off
 
set BuildDir=%~dp0..\build
 
if exist %BuildDir% (  
  pushd %BuildDir%
  del /q /s *.exe *.pdb *.ilk *.dll *.obj
  ::rd /s /q .vs
  popd
)