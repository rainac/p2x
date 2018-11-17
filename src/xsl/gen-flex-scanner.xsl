<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output encoding="latin1" method="text"/>

  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="text()"/>

  <xsl:template match="q/text()|action/text()|c-code/text()|comment/text()|options/text()|def/text()|re/text()">
    <xsl:copy/>
  </xsl:template>

  <xsl:template match="options">
    <xsl:apply-templates/>
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>
  
  <xsl:template match="body/rules">
    <xsl:text>%%&#xa;</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>%%&#xa;</xsl:text>
  </xsl:template>

  <xsl:template match="def">
    <xsl:value-of select="@name"/>
    <xsl:text> </xsl:text>
    <xsl:apply-templates/>
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

  <xsl:template match="q">
    <xsl:text>"</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>"</xsl:text>
  </xsl:template>

  <xsl:template match="ref">
    <xsl:text>{</xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>}</xsl:text>
  </xsl:template>

  <xsl:template match="action">
    <xsl:text> return </xsl:text>
    <xsl:apply-templates/>
    <xsl:text>;</xsl:text>
  </xsl:template>

  <xsl:template match="rule">
    <xsl:apply-templates select="re"/>
    <xsl:text>&#x9;&#x9; </xsl:text>
    <xsl:apply-templates select="action"/>
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

</xsl:stylesheet>
<!-- $Id: reproduce.xsl 75 2014-10-24 16:21:34Z johannes.willkomm@googlemail.com $ -->
