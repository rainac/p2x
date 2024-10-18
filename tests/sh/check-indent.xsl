<?xml version="1.0"?>
<!--
This file is part of P2X.
Copyright © 2014 Johannes Willkomm
See the file p2x.cc for copying conditions.
-->
<xsl:stylesheet version="1.0"
    xmlns:cx='http://johannes-willkomm.de/xml/code-xml/'
    xmlns:ca='http://johannes-willkomm.de/xml/code-xml/attributes/'
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="text"/>

  <xsl:variable name="nl">&#xa;</xsl:variable>

  <xsl:template match="text()"/>

  <xsl:template match="*">
    <xsl:variable name="i" select="count(preceding::*)"/>
    <xsl:variable name="d" select="count(ancestor::*)"/>
    <xsl:variable name="pn" select="preceding::node()[1]"/>
    <xsl:variable name="apn" select="../preceding::node()[1]"/>
    <xsl:variable name="pws" select="$pn/self::text()"/>
<!--    <xsl:message>
      Element <xsl:value-of select="$i"/> (<xsl:value-of select="name()"/>), prec: <xsl:value-of
	select="local-name($pn)"/>: depth=<xsl:value-of select="$d"/>, indent ws=<xsl:value-of select="string-length($pn)"/>
    </xsl:message> -->
    <xsl:if test="$d > 0 and $pws and generate-id($pn) != generate-id($apn)">
      <xsl:if test="substring($pws, 1, 1) != '&#xa;'">
	<xsl:message terminate="yes">Error: must start with \n</xsl:message>
      </xsl:if>
      <xsl:if test="translate(substring($pws, 2), ' ', '')">
	<xsl:message terminate="yes">Error: must consist of \n followed by spaces</xsl:message>
      </xsl:if>
      <xsl:if test="string-length($pws) - 1 != $d">
	<xsl:message terminate="yes">Error: must be indented according to depth</xsl:message>
      </xsl:if>
    </xsl:if>
    <xsl:apply-templates/>
  </xsl:template>

</xsl:stylesheet>
