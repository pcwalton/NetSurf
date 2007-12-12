<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
        "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<title>NetSurf | Ontwikkelversies</title>
<link rel="stylesheet" type="text/css" href="/netsurf.css">
</head>

<body>
<p class="banner"><a href="/"><img src="/netsurf.png" alt="NetSurf: Web Browser for RISC OS"></a></p>
<p class="languages"><a href="index.de">Deutsch</a> · <a href="index.en">English</a> · <a href="index.fr">Français</a> · Nederlands</p>

<h1>NetSurf-ontwikkelversies</h1>

<p>Deze archieven worden automatisch van SVN-broncode gemaakt.
<strong>Ze kunnen onstabiel zijn en uw machine laten vastlopen.</strong></p>

<p id="newtonetsurf"><strong>Nieuw bij NetSurf?</strong> Haal
<span class="highlight">NetSurf</span> hier op en lees de <a
href="/info#GettingStartedInstallation">installatie-instructies</a>.</p>

<table class="download">
<tr><th class="highlight"><a href="netsurf.zip"><img src="netsurf.png" alt=""><br>NetSurf</a></th>
<td>SIZE_NETSURF</td>
<td>DATE_NETSURF</td>
<td>NetSurf-browser voor RISC OS, volledige versie.
<strong>Aanbevolen voor de meeste gebruikers.</strong></td>
</tr>

<tr><th><a href="unetsurf.zip"><img src="unetsurf.png" alt=""><br>Kleine NetSurf</a></th>
<td>SIZE_UNETSURF</td>
<td>DATE_UNETSURF</td>
<td>De kleinere versie van NetSurf voor RISC OS. Ondersteund alleen http: en file: (geen https:). Te gebruiken voor computers met weinig geheugen (8MB of minder).</td>
</tr>

<tr><th><a href="nstheme.zip"><img src="nstheme.png" alt=""><br>NSTheme</a></th>
<td>SIZE_NSTHEME</td>
<td>DATE_NSTHEME</td>
<td>Thema-verpakker voor NetSurf. Alleen nodig voor ontwerpers van thema's.</td>
</tr>
</table>

<p>Een <a href="http://www.riscpkg.org/">RiscPkg</a>-pakket van NetSurf is beschikbaar. Om dit te gebruiken moet de regel <code>pkg http://www.netsurf-browser.org/builds/riscpkg/packages</code> toegevoegd worden aan <code>Choices:RiscPkg.Sources</code>. Lees de RiscPkg documentatie voor meer informatie.</p>

<p><a href="netsurf.log">Verslag van de laatste bouwpoging</a> (alleen interessant voor ontwikkelaars).</p>

<h2>Vereisten</h2>

<p>NetSurf heeft een aantal voorzieningen nodig, die aanwezig moeten zijn op uw systeem om te kunnen starten.</p>

<ul>
<li>WindowManager 3.80 of later. Dit zit in RISC OS 4 en hoger. RISC OS 3 gebruikers moeten hiervoor de <a href="http://acorn.riscos.com/riscos/releases/UniBoot/">Universal Boot Sequence</a> installeren.</li>
</ul>

<p>De volgende voorzieningen worden bij NetSurf meegeleverd:</p>

<ul>
<li><a href="http://sudden.recoil.org/others/">AcornURI</a> v1.04, Christian Ludlam.</li>
<li><a href="/iconv/">Iconv</a> v0.08, John-Mark Bell.</li>
<li><a href="http://www.riscos.info/downloads/gccsdk/sharedunixlib/system.zip">SharedUnixLibrary</a> v1.10, GCCSDK team.</li>
<li><a href="http://www.tinct.net/tinct.asp">Tinct</a> v0.13, Richard Wilson.</li>
</ul>

<p><strong>Optioneel:</strong> installeer dit <a href="http://www.beebware.com/software/mappings/">MimeMap databestand</a> als u er nog geen heeft en NetSurf daar over klaagt.</p>

<!-- <p><strong>Optional:</strong> download and install the <a href="nsfonts.zip">fonts (360k)</a> to get improved font-family and font-variant support.</p> -->

<h2>Recente SVN-activiteit</h2>
<p>Alle tijden zijn in UTC.</p>
