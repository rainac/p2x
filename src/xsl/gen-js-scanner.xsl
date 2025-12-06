<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output encoding="utf-8" method="text"/>

  <xsl:template match="/">
    if (typeof window == 'undefined') {
      var P2X = require('./scanner.js');
    }
    var scanner = P2X.scanner;
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="text()">
    <xsl:copy/>
  </xsl:template>

  <xsl:template name="replace">
    <xsl:param name="str"/>
    <xsl:param name="by"/>
    <xsl:param name="pattern"/>
    <xsl:choose>
      <xsl:when test="contains($str, $pattern)">
        <xsl:value-of select="substring-before($str, $pattern)"/>
        <xsl:value-of select="$by"/>
        <xsl:value-of select="substring-after($str, $pattern)"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$str"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="text()" mode="quote">
    <xsl:call-template name="replace">
      <xsl:with-param name="pattern" select="'/'"/>
      <xsl:with-param name="by" select="'\/'"/>
      <xsl:with-param name="str">
    <xsl:call-template name="replace">
      <xsl:with-param name="pattern" select="'.'"/>
      <xsl:with-param name="by" select="'\.'"/>
      <xsl:with-param name="str">
    <xsl:call-template name="replace">
      <xsl:with-param name="pattern" select="')'"/>
      <xsl:with-param name="by" select="'\)'"/>
      <xsl:with-param name="str">
    <xsl:call-template name="replace">
      <xsl:with-param name="pattern" select="'('"/>
      <xsl:with-param name="by" select="'\('"/>
      <xsl:with-param name="str">
    <xsl:call-template name="replace">
      <xsl:with-param name="pattern" select="']'"/>
      <xsl:with-param name="by" select="'\]'"/>
      <xsl:with-param name="str">
    <xsl:call-template name="replace">
      <xsl:with-param name="pattern" select="'['"/>
      <xsl:with-param name="by" select="'\['"/>
      <xsl:with-param name="str">
    <xsl:call-template name="replace">
      <xsl:with-param name="pattern" select="'^'"/>
      <xsl:with-param name="by" select="'\^'"/>
      <xsl:with-param name="str">
    <xsl:call-template name="replace">
      <xsl:with-param name="pattern" select="'|'"/>
      <xsl:with-param name="by" select="'\|'"/>
      <xsl:with-param name="str">
    <xsl:call-template name="replace">
      <xsl:with-param name="pattern" select="'+'"/>
      <xsl:with-param name="by" select="'\+'"/>
      <xsl:with-param name="str">
    <xsl:call-template name="replace">
      <xsl:with-param name="pattern" select="'?'"/>
      <xsl:with-param name="by" select="'\?'"/>
      <xsl:with-param name="str">
        <xsl:call-template name="replace">
          <xsl:with-param name="by" select="'\*'"/>
          <xsl:with-param name="pattern" select="'*'"/>
          <xsl:with-param name="str" select="."/>
        </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="head/text()"/>
  <xsl:template match="body/text()"/>
  <xsl:template match="c-code/text()"/>
  <xsl:template match="options/text()"/>
  <xsl:template match="definitions/text()"/>
  <xsl:template match="rules/text()"/>

  <xsl:template match="ref">
    <xsl:text>(</xsl:text>
    <xsl:apply-templates select="//def[@name = current()/@name]/node()"/>
    <xsl:text>)</xsl:text>
    <!--
    <xsl:variable name="ref-txt">
      <xsl:apply-templates select="//def[@name = current()/@name]/node()"/>
    </xsl:variable>
    <xsl:if test="not(starts-with($ref-txt, '[') or starts-with($ref-txt, '('))">
      <xsl:text>(</xsl:text>
    </xsl:if>
    <xsl:value-of select="$ref-txt"/>
    <xsl:if test="not(starts-with($ref-txt, '[') or starts-with($ref-txt, '('))">
      <xsl:text>)</xsl:text>
    </xsl:if>
    -->
  </xsl:template>

  <xsl:template match="def"/>
  
<!--  <xsl:template match="def">
    var def_<xsl:value-of select="@name"/>
    <xsl:text> = /</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>/</xsl:text>
  </xsl:template>
-->
  <xsl:template match="re">
    <xsl:text>/</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>/</xsl:text>
  </xsl:template>

  <xsl:template match="q">
    <xsl:apply-templates mode="quote"/>
  </xsl:template>

  <xsl:template match="rule">
    scanner.add(<xsl:apply-templates select="re"/>, <xsl:apply-templates select="action"/>)
  </xsl:template>

</xsl:stylesheet>
