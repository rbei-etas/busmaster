#!/bin/sh

# Usage
if [ $# -ne 1 ]; then
  echo "Usage: update-version-information.sh <X.Y.Z>"
  exit 1
fi

# Set version information
VERSION=$1
VERSION_MAJOR=`echo $VERSION | awk 'BEGIN { FS = "\." } ; { print $1 }'`
VERSION_MINOR=`echo $VERSION | awk 'BEGIN { FS = "\." } ; { print $2 }'`
VERSION_BUILD=`echo $VERSION | awk 'BEGIN { FS = "\." } ; { print $3 }'`
VERSION="$VERSION_MAJOR.$VERSION_MINOR.$VERSION_BUILD"
echo "Setting to Version $VERSION"

# Sources/BIN/Release/Changelog.txt
git log > Sources/BIN/Release/Changelog.txt
dos2unix Sources/BIN/Release/Changelog.txt

# Sources/Application/BUSMASTER.rc
echo "  Sources/Application/BUSMASTER.rc"
sed -i.orig \
  -e 's/^            VALUE "FileVersion", "Ver .*"$/            VALUE "FileVersion", "Ver '$VERSION'"/' \
  -e 's/^            VALUE "ProductVersion", "Ver .*"$/            VALUE "ProductVersion", "Ver '$VERSION'"/' \
  -e 's/^    IDS_VERSION             "Ver .*"$/    IDS_VERSION             "Ver '$VERSION'"/' \
  Sources/Application/BUSMASTER.rc

# Sources/Application/StdAfx.h
echo "  Sources/Application/StdAfx.h"
sed -i.orig \
  -e 's/^const BYTE VERSION_MAJOR.*$/const BYTE VERSION_MAJOR    = '$VERSION_MAJOR';/' \
  -e 's/^const BYTE VERSION_MINOR.*$/const BYTE VERSION_MINOR    = '$VERSION_MINOR';/' \
  -e 's/^const BYTE VERSION_BUILD.*$/const BYTE VERSION_BUILD    = '$VERSION_BUILD';/' \
  Sources/Application/StdAfx.h

# Sources/FrameProcessor/LogObjectCAN.cpp
echo "  Sources/FrameProcessor/LogObjectCAN.cpp"
sed -i.orig \
  -e 's/^#define CAN_VERSION           _T("\*\*\*BUSMASTER Ver .*\*\*\*")$/#define CAN_VERSION           _T("***BUSMASTER Ver '$VERSION'***")/' \
  Sources/FrameProcessor/LogObjectCAN.cpp

# Installation Setup/BUSMASTER INSTALL SCRIPT.nsi
echo "  Installation Setup/BUSMASTER INSTALL SCRIPT.nsi"
sed -i.orig \
  -e 's/^Outfile "BUSMASTER_Installer_Ver_.*\.exe"$/Outfile "BUSMASTER_Installer_Ver_'$VERSION'\.exe"/' \
  "Installation Setup/BUSMASTER INSTALL SCRIPT.nsi"


# Some additional ideas, which could be interesting for debug purposes:

# Git describe
#DESCRIBE=`git describe`
#VERSION_MAJOR=`echo $DESCRIBE | awk 'BEGIN { FS = "[v\.-]" } ; { print $2 }'`
#VERSION_MINOR=`echo $DESCRIBE | awk 'BEGIN { FS = "[v\.-]" } ; { print $3 }'`
#VERSION_BUILD=`echo $DESCRIBE | awk 'BEGIN { FS = "[v\.-]" } ; { print $4 }'`

# Build date (or just look for the installer date)
# `date`

# Build environment (or just parse the binary files)
# Visual C++ version, e.g. Express 2008
# MFC version, e.g. 4.2
# ATL version, e.g. 7.1

# Git commit & tag & push
# git commit -m "Version $VERSION"
# git tag -a v$VERSION -m "Version $VERSION"
# git push --tags

