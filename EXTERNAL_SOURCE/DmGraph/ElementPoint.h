/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		ElementPoint.h: Declaration of the CElementPoint class
//
// CLASS NAME
//		CElementPoint
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila created from old CElementPoint
//
#ifndef __ELEMENT_POINT__H__
#define __ELEMENT_POINT__H__

///////////////////////////////////////////////////////////
// Declaration of the CElementPoint class.
class CElementPoint
{

public:
   double	x, y;

   CElementPoint () { x=y=0; }
   CElementPoint (double c1, double c2)
   {
      x = c1;      y = c2;    
   }
   CElementPoint& operator=(const CElementPoint& pt)
   {
      x = pt.x;  y = pt.y;
      return *this;
   }
   CElementPoint (const CElementPoint& pt)
   {
      *this = pt;
   }
};

#endif //__ELEMENT_POINT__H__