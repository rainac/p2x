<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
    xmlns:cx='http://johannes-willkomm.de/xml/code-xml/'
    xmlns:ca='http://johannes-willkomm.de/xml/code-xml/attributes/'
    xmlns:ci='http://johannes-willkomm.de/xml/code-xml/ignore'
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="text"/>

  <xsl:template match="/">
    <xsl:apply-templates mode="reproduce"/>
  </xsl:template>

  <xsl:template match="text()" mode="reproduce"/>

  <xsl:template match="ca:text|ca:t|ci:*" mode="reproduce">
    <xsl:value-of select="."/>
  </xsl:template>

  <xsl:template match="ca:br" mode="reproduce">
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

  <xsl:template match="ca:cr" mode="reproduce">
    <xsl:text>&#xd;</xsl:text>
  </xsl:template>


  <xsl:template match="text()" mode="reproduce-cx-text">
    <xsl:copy/>
  </xsl:template>
  <xsl:template match="*" mode="reproduce-cx-text">
    <xsl:apply-templates select="." mode="reproduce-text"/>
  </xsl:template>

  <xsl:template match="text()" mode="reproduce-text"/>

  <xsl:template match="*" mode="reproduce-text">
    <xsl:apply-templates mode="reproduce-text"/>
  </xsl:template>

  <xsl:template match="cx:*" mode="reproduce-text">
    <xsl:apply-templates mode="reproduce-cx-text"/>
  </xsl:template>

  <xsl:template match="ca:text" mode="reproduce-text">
    <xsl:value-of select="."/>
  </xsl:template>

  <xsl:template match="ca:br" mode="reproduce-text">
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

</xsl:stylesheet>
