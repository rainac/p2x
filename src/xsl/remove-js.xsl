<?xml version="1.0"?>
<!--
This file is part of P2X.
Copyright © 2024 Johannes Willkomm
See the file p2x.cc for copying conditions.
-->
<xsl:stylesheet version="1.0"
                xmlns:html="http://www.w3.org/1999/xhtml"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="html"/>

  <xsl:template match="node()">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:apply-templates select="node()"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="html:script[@src]">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:attribute name="src">/usr/share/javascript/mathjax/MathJax.js</xsl:attribute>
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>

</xsl:stylesheet>
