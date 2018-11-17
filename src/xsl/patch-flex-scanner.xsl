<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="xml"/>

  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="*">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="text()">
    <xsl:copy/>
  </xsl:template>

  <xsl:template match="def[@name = 'HIGHLET']"/>
  
  <xsl:template match="def[@name = 'HIGHLET_C']">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:attribute name="name">HIGHLET</xsl:attribute>
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>

</xsl:stylesheet>
<!-- $Id: reproduce.xsl 75 2014-10-24 16:21:34Z johannes.willkomm@googlemail.com $ -->
