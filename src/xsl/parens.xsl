<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
    xmlns:cx='http://johannes-willkomm.de/xml/code-xml/'
    xmlns:ca='http://johannes-willkomm.de/xml/code-xml/attributes/'
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="text"/>

  <xsl:template match="/">
    <xsl:apply-templates mode="parens"/>
  </xsl:template>

  <xsl:template match="text()" mode="parens"/>

  <xsl:template match="cx:root" mode="parens">
    <xsl:apply-templates select="cx:*[2]" mode="parens"/>
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

  <xsl:template match="cx:op|cx:paren" mode="parens">
    <xsl:text>[</xsl:text>
    <xsl:choose>
      <xsl:when test="@type = 'IDENTIFIER'">
        <xsl:value-of select="@repr"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="@type"/>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:text>]</xsl:text>
    <xsl:text>(</xsl:text>
    <xsl:for-each select="cx:*">
      <xsl:if test="position() > 1">, </xsl:if>
      <xsl:apply-templates select="." mode="parens"/>
    </xsl:for-each>
    <xsl:text>)</xsl:text>
  </xsl:template>

  <xsl:template match="cx:null" mode="parens">
    <xsl:text>.</xsl:text>
  </xsl:template>

  <xsl:template match="cx:id|cx:int|cx:integer|cx:float|cx:string" mode="parens">
    <xsl:value-of select="ca:text"/>
  </xsl:template>

</xsl:stylesheet>
<!-- $Id: parens.xsl 89 2014-10-25 13:50:49Z johannes.willkomm@googlemail.com $ -->
