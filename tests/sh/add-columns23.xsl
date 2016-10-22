<?xml version="1.0"?>
<!--
This file is part of P2X.
Copyright © 2014 Johannes Willkomm 
See the file p2x.cc for copying conditions.  
-->
<xsl:stylesheet version="1.0"
    xmlns:cx='http://johannes-willkomm.de/xml/code-xml/'
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="text"/>

  <xsl:template match="cx:op[@type = 'COMMA']">
    <xsl:value-of select="cx:*[2]+cx:*[3]"/>
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

  <xsl:template match="cx:COMMA">
    <xsl:value-of select="cx:*[2]+cx:*[3]"/>
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

</xsl:stylesheet>
