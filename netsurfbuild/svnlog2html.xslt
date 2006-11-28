<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" encoding="iso-8859-1" media-type="text/html"/>

  <xsl:template match="log">
    <div class="changelog">
      <xsl:for-each select="logentry">
        <xsl:sort select="@revision" date-type="number" order="descending"/>
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
            <small class="files">
			  <xsl:for-each select="paths/path">
			    <a href="http://svn.semichrome.net{.}">
				  <xsl:value-of select="substring(., 2)"/>
				</a>
				<xsl:if test="position() != last()">, </xsl:if>
			  </xsl:for-each>
			</small>
		  </p>
          <p class="msg">
            <xsl:value-of select="msg"/>
          </p>
        </div>
      </xsl:for-each>
    </div>
  </xsl:template>

</xsl:stylesheet>

