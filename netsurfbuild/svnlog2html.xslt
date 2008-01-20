<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" encoding="iso-8859-1" media-type="text/html"/>

  <xsl:template match="log">
    <div class="changelog">
      <xsl:for-each select="logentry">
	<xsl:sort select="@revision" data-type="number" order="descending"/>
	<div class="entry">
	  <p class="meta">
	    <span class="date">
	      <xsl:value-of select="substring(date, 1, 10)"/>
	      <xsl:text> </xsl:text>
	      <xsl:value-of select="substring(date, 12, 5)"/>
	    </span>
	    <xsl:text> - </xsl:text>
	    <em class="author"><xsl:value-of select="author"/></em>
	    <xsl:text> - </xsl:text>
	    <xsl:variable name="rev">
	      <xsl:value-of select="@revision"/>
	    </xsl:variable>
	    <a href="http://source.netsurf-browser.org/?rev={$rev}&amp;view=rev">
	      <xsl:text>r</xsl:text>
	      <xsl:value-of select="@revision"/>
	    </a>
	  </p>
	  <p class="msg">
	    <xsl:value-of select="msg"/>
	  </p>
	</div>
      </xsl:for-each>
    </div>
  </xsl:template>

</xsl:stylesheet>

