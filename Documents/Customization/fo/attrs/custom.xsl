<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:fo="http://www.w3.org/1999/XSL/Format"
    version="1.0">

  <xsl:variable name="page-width">210mm</xsl:variable>
  <xsl:variable name="page-height">297mm</xsl:variable>

  <xsl:attribute-set name="__frontmatter__logo">
    <xsl:attribute name="margin-top">50mm</xsl:attribute>
  </xsl:attribute-set>

  <xsl:attribute-set name="__frontmatter__title">
    <xsl:attribute name="margin-top">20mm</xsl:attribute>
    <xsl:attribute name="font-family">Sans</xsl:attribute>
    <xsl:attribute name="font-size">22pt</xsl:attribute>
    <xsl:attribute name="font-weight">bold</xsl:attribute>
    <xsl:attribute name="line-height">140%</xsl:attribute>
  </xsl:attribute-set>

  <xsl:attribute-set name="image">
    <xsl:attribute name="content-width">scale-down-to-fit</xsl:attribute>
    <xsl:attribute name="content-height">scale-down-to-fit</xsl:attribute>
    <xsl:attribute name="width">100%</xsl:attribute>
    <xsl:attribute name="scaling">uniform</xsl:attribute>
  </xsl:attribute-set>
</xsl:stylesheet>