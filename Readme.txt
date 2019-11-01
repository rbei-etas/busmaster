1. Development environment: To configure a computer for developing BUSMASTER code, refer to file "1. BUSMASTER OSS - Development Environment.doc" in the 'Downloads' folder.

2. BUSMASTER application is cross platform - runs on many different operating systems such as Windows and Linux. The emulation mode does not require any CAN interface hardware. These are also documented in the aforementioned file. The application source code is in the 'Sources' folder. 

3. Connecting BUSMASTER to physical CAN bus - acquire hardware and install device driver software from the respective vendor. For example:
  3.1 PCAN_USB: Peak GmbH.
  3.2 ES 581  : ETAS GmbH. Use the HSP (Hardware Service Pack) driver.

4. Instructions in the aforementioned file includes steps to build an installer program. The install script and the related artifacts are in the 'Installation Setup' folder.

5. Testing: Documentation and scripts are in the 'Test Artifacts' folder.

6. BUSMASTER has component tools (dependencies). Their binaries will be sufficient to build BUSMASTER. Since these are opensource, complete source code package is available for the discerning developer. The corresponding URLs are listed on the right. 
	6.1. MinGW, GCC       : http://sourceforge.net/projects/mingw/files/MinGW/
	6.2. Bison.exe        : http://downloads.sourceforge.net/gnuwin32/bison-2.4.1-src-setup.exe
	6.3. Flex.exe         : http://flex.sourceforge.net/
	6.4. Libxml2          : ftp://xmlsoft.org/libxml2/win32/
	6.5. GetText          : http://www.gnu.org/software/gettext/	and http://ftp.gnu.org/gnu/gettext/ and http://gnuwin32.sourceforge.net/packages/gettext.htm	
	6.6. Oxygen Icon Set  : http://www.oxygen-icons.org/. For Licensing terms, refer the legal information page http://www.oxygen-icons.org/?page_id=4

7. BUSMASTER uses Codeproject component tools (dependencies). Their binaries will be sufficient to build BUSMASTER. Since these are opensource, complete source code package is available for the discerning developer. The URLs are listed on the right. For Codeproject license visit http://www.codeproject.com/info/cpol10.aspx. 
	7.1. Dm Graph         : http://www.codeproject.com/Articles/310494/2D-Graph-ActiveX-control-in-Cplusplus-with-ATL-no
	7.2. Header Control   : http://www.sothink.com
	7.3. MFC Tree Control : http://doc.mimec.org/articles/mfc/mctree/index.html
    	7.4. Qt 5.3.0         : Installer or sources for LGPL http://download.qt-project.org/archive/qt/5.3/5.3.0/qt-opensource-windows-x86-msvc2012_opengl-5.3.0.exe
	7.5. Free Type Library: license http://www.freetype.org/
    	7.6. Antlr3.5.2       : http://www.antlr3.org/download.html
	
