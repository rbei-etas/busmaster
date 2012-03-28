@echo off

set PATH=%PATH%;%ProgramFiles\Cppcheck
cppcheck --xml ../../Sources 2> cppcheck.xml
