<?xml version="1.0"?>
<!--
This file is part of P2X.
Copyright © 2014 Johannes Willkomm 
See the file p2x.cc for copying conditions.  
-->
<xsl:stylesheet version="1.0"
    xmlns:cx='http://ai-and-it.de/xml/code-xml/'
    xmlns:ca='http://ai-and-it.de/xml/code-xml/attributes/'
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="xml"/>

  <xsl:template match="node()">
    <xsl:copy>
      <xsl:copy-of select="attribute::node()"/>
      <xsl:apply-templates select="node()"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="/">
    <xsl:apply-templates select="(/*/cx:root|/*/cx:ROOT)/cx:*[local-name() != 'null'][1]"/>
  </xsl:template>

</xsl:stylesheet>
