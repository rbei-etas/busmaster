<?xml version='1.0'?>

<!DOCTYPE xsl:stylesheet [
<!-- entities for use in the generated output (Unicode typographic glyphs) -->
  <!ENTITY gt            "&gt;"> 
  <!ENTITY lt            "&lt;"> 
  <!ENTITY rbl           "&#160;">
  <!ENTITY nbsp          "&#160;">
  <!ENTITY quot          "&#34;">
  <!ENTITY quotedblleft  "&#x201C;">
  <!ENTITY quotedblright "&#x201D;">
  <!ENTITY sqbull        "[]">
  <!ENTITY middot        "&#x00A5;">

  <!ENTITY section       "&#xA7;">
  <!ENTITY endash        "&#x2013;">
  <!ENTITY emdash        "&#x2014;">

  <!ENTITY copyr         "&#xA9;">
  <!ENTITY trademark     "&#x2122;">
  <!ENTITY registered    "&#xAE;">
<!-- create some fixed values for now for customizing later -->
  <!ENTITY copyrowner    "Robert Bosch Engineering and Business Solutions Limited">
  <!ENTITY copyrdate     "2011">

]>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:fo="http://www.w3.org/1999/XSL/Format"
    version="1.1">
  
    <xsl:template name="createFrontMatter">
        <fo:page-sequence master-reference="front-matter" format="i" xsl:use-attribute-sets="__force__page__count">
            <xsl:call-template name="insertFrontMatterStaticContents"/>
            <fo:flow flow-name="xsl-region-body">
                <fo:block xsl:use-attribute-sets="__frontmatter">
		    <!-- set the logo -->
		    <fo:block xsl:use-attribute-sets="__frontmatter__logo">
			<fo:external-graphic src="url(../../../Logo/BUSMASTER_Logo.svg)" content-width="scale-to-fit" content-height="scale-to-fit" width="100%" scaling="uniform"/>
                    </fo:block>

                    <!-- set the title -->
                    <fo:block xsl:use-attribute-sets="__frontmatter__title">
                        <xsl:value-of select="//*/title"/>
                        <!-- <xsl:choose>
                            <xsl:when test="//*[contains(@class,' bkinfo/bkinfo ')][1]">
                                <xsl:apply-templates select="//*[contains(@class,' bkinfo/bkinfo ')][1]/*[contains(@class,' topic/title ')]/node()"/>
                            </xsl:when>
                            <xsl:when test="//*[contains(@class, ' map/map ')]/@title">
                                <xsl:value-of select="//*[contains(@class, ' map/map ')]/@title"/>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:value-of select="/descendant::*[contains(@class, ' topic/topic ')][1]/*[contains(@class, ' topic/title ')]"/>
                            </xsl:otherwise>
                        </xsl:choose/> -->
                    </fo:block>

                    <!-- set the subtitle -->
                    <xsl:apply-templates select="//*[contains(@class,' bkinfo/bkinfo ')][1]/*[contains(@class,' bkinfo/bktitlealts ')]/*[contains(@class,' bkinfo/bksubtitle ')]"/>

                    <!-- set the owners -->
                    <fo:block xsl:use-attribute-sets="__frontmatter__owner">
                        <xsl:choose>
                            <xsl:when test="//*[contains(@class,' bkinfo/bkowner ')]">
                                <xsl:apply-templates select="//*[contains(@class,' bkinfo/bkowner ')]"/>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:apply-templates select="$map/*[contains(@class, ' map/topicmeta ')]"/>
                            </xsl:otherwise>
                        </xsl:choose>
                    </fo:block>

		</fo:block>

                <!-- set the copyright -->
                <xsl:call-template name="processCopyrigth"/>

            </fo:flow>
        </fo:page-sequence>
    </xsl:template>

    <xsl:template match="*[contains(@class, ' topic/author ')]">
	<fo:block xsl:use-attribute-sets="author"/>
    </xsl:template>
    
    <xsl:template match="*[contains(@class, ' topic/copyright ')]">
	<fo:block xsl:use-attribute-sets="copyright">&copyr; Copyright &copyrdate; &copyrowner;</fo:block>
    </xsl:template>

</xsl:stylesheet>
