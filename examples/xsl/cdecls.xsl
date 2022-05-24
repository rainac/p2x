<?xml version="1.0" encoding="utf-8"?>
<!--
Copyright © 2014,2016 Johannes Willkomm
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:cx='http://johannes-willkomm.de/xml/code-xml/'
    xmlns:c='http://johannes-willkomm.de/xml/code-xml/attributes/'
    xmlns:ci='http://johannes-willkomm.de/xml/code-xml/ignore/'
    version="1.0">

  <xsl:output method="text"/>

  <xsl:template match="text()"/>

  <xsl:template match="cx:JUXTA[count(cx:*) = 2]">
    <xsl:param name="type"/>
    <xsl:apply-templates select="cx:*[2]">
      <xsl:with-param name="type" select="cx:*[1]/c:t"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="cx:JUXTA[count(cx:*) = 3]">
    <xsl:variable name="type-decl">
      <xsl:apply-templates select="cx:*[1]"/>
    </xsl:variable>
    <xsl:variable name="param-decls">
      <xsl:apply-templates select="cx:*[3]"/>
    </xsl:variable>
    <xsl:apply-templates select="cx:*[2]">
      <xsl:with-param name="type">Funktion(<xsl:value-of select="$param-decls"/>) -> <xsl:value-of select="$type-decl"/></xsl:with-param>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="cx:*" mode="in-ptr-decl">
    <xsl:param name="type"/>
    <xsl:apply-templates select=".">
      <xsl:with-param name="type" select="$type"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="cx:JUXTA" mode="in-ptr-decl">
    <xsl:param name="type"/>
    <xsl:variable name="param-decls">
      <xsl:apply-templates select="cx:*[2]"/>
    </xsl:variable>
    <xsl:apply-templates select="cx:*[1]">
      <xsl:with-param name="type">Funktion(<xsl:value-of select="$param-decls"/>) -> <xsl:value-of select="$type"/></xsl:with-param>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="cx:COMMA">
    <xsl:for-each select="cx:*">
      <xsl:if test="position() > 1">, </xsl:if>
      <xsl:apply-templates select="."/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="cx:L_PAREN">
  </xsl:template>

  <xsl:template match="cx:L_PAREN[cx:*[1]/self::cx:null]">
    <xsl:param name="type"/>
    <xsl:apply-templates select="cx:*[2]/cx:*[1]">
      <xsl:with-param name="type" select="$type"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="cx:L_BRACKET">
    <xsl:param name="type"/>
    <xsl:apply-templates select="cx:*[1]">
      <xsl:with-param name="type" select="concat('Array(', cx:*[2]/cx:*[1], ') von ', $type)"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="cx:MULT[cx:*[1]/self::cx:null]">
    <xsl:param name="type"/>
    <xsl:apply-templates select="cx:*[2]" mode="in-ptr-decl">
      <xsl:with-param name="type" select="concat('Zeiger auf ', $type)"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="cx:MULT">
    <xsl:variable name="type-decl">
      <xsl:apply-templates select="cx:*[1]"/>
    </xsl:variable>
    <xsl:apply-templates select="cx:*[2]" mode="in-ptr-decl">
      <xsl:with-param name="type">Zeiger auf <xsl:copy-of select="$type-decl"/></xsl:with-param>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="cx:SEMICOLON">
    <xsl:variable name="spacer">
      <xsl:text>                                                 </xsl:text>
    </xsl:variable>
    <xsl:for-each select="cx:*">
      <xsl:variable name="text">
        <xsl:copy-of select=".//c:t|.//ci:*"/>
      </xsl:variable>
      <xsl:value-of select="$text"/>
      <xsl:value-of select="substring($spacer, string-length($text))"/>
      <xsl:text>:   </xsl:text>
      <xsl:apply-templates select="."/>
      <xsl:text>.&#xa;</xsl:text>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="cx:ID">
    <xsl:param name="type"/>
    <xsl:if test="not($type)">
      <xsl:value-of select="c:t"/>
    </xsl:if>
    <xsl:copy-of select="$type"/>
  </xsl:template>

</xsl:stylesheet>
