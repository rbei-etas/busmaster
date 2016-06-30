del libWrapper_J1939.a
g++ -DNO_INCLUDE_TEMPSTRUCT -c BMJ1939Defines.cpp BMUtility.cpp Wrapper_Common.cpp
ar -rcs libWrapper_J1939.a BMJ1939Defines.o  BMUtility.o Wrapper_Common.o
copy /Y libWrapper_J1939.a ..\OBJ\
del BMJ1939Defines.o  BMUtility.o Wrapper_Common.o