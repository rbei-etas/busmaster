del libWrapper_LIN.a
g++ -DNO_INCLUDE_TEMPSTRUCT -c  BMLINDefines.cpp BMUtility.cpp Wrapper_Common.cpp 
ar -rcs libWrapper_LIN.a BMLINDefines.O BMUtility.o Wrapper_Common.o
copy /Y libWrapper_LIN.a ..\OBJ\
del BMLINDefines.O BMUtility.o Wrapper_Common.o
