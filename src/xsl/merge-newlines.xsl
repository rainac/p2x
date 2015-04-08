<?xml version="1.0"?>
<!-- 
Copyright © 2014 Johannes Willkomm
-->
<xsl:stylesheet version="1.0"
    xmlns:cx='http://johannes-willkomm.de/xml/code-xml/'
    xmlns:ca='http://johannes-willkomm.de/xml/code-xml/attributes/'
    xmlns:x='http://www.w3.org/1999/xhtml'
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:import href="copy.xsl"/>

  <xsl:output method="xml"/>

  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="cx:op[@type='NEWLINE']">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:apply-templates mode="strip-newline"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="cx:op[@type='NEWLINE']" mode="strip-newline">
    <xsl:apply-templates mode="strip-newline"/>
  </xsl:template>

  <xsl:template match="*" mode="strip-newline">
    <xsl:apply-templates select="."/>
  </xsl:template>

</xsl:stylesheet>
