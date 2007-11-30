<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
        "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<title>NetSurf | Voortgang</title>
<link rel="stylesheet" type="text/css" href="netsurf.css">
</head>
<body>
<p class="banner"><a href="/"><img src="netsurf.png" alt="NetSurf: web bladeraar voor RISC OS"></a></p>
<p class="languages"><a href="progress.de">Deutsch</a> · <a href="progress.en">English</a> · <a href="progress.fr">Français</a> · Nederlands</p>

<h1>Van de vertalers</h1>

<p>Vanaf 21 oktober 2004 is begonnen om bestanden waarvan (nog) geen nederlandstalige versie was, te vertalen naar het nederlands.</p>

<p>Delen waarvan (nog) niet zeker is, hoe deze juist vertaald moeten worden bleven in de oorspronkelijke taal (Engels) staan maar wel tussen rechthoekige haken en in <font color=red>rood</font>.</p>

<p>Waar vrij algemeen gebruikte engelstalige begrippen zijn vervangen door nederlandstalige begrippen, zijn die engelstalige begrippen er de eerste keer (ook tussen rechthoekige haken en in <font color=green>groen</font>) achter geplaatst.</p>

<p>Wanneer een engelstalig begrip bleef gehandhaafd, is dit de eerste keer in groen gezet en is er de eerste keer (ook tussen rechthoekige haken) een mogelijke vertaling vlak daarna gezet. Uiteraard zijn de keuzes persoonlijk en dus betwistbaar.</p>

<p>Alle <font color=green>links</font> [verbindingsmogelijkheden] waarvoor verbinding met het net nodig is, staan <strong><font color=blue>vet</font></strong> afgebeeld.</p>

<p>Eerste vertaling: 21 oktober 2004<br>
Eerste aanpassing: 1 januari 2005<br>
Laatste aanpassing: 19 april 2007<br>
Laatste vertaling: 15 november 2007</p>

<p class="headnote">Gerard van Katwijk & Simon Voortman (BBC; Regio Den Haag)</p>


<h1>Ontwikkelingsvoortgang</h1>

<p>Dit hoofdstuk dient om de gebruikers en (mede)ontwikkelaars een eerste indruk te geven van de geboden mogelijkheden en die waaraan nog gewerkt word. Voor meer gedetaileerde informatie: selecteer de <a href="http://source.netsurf-browser.org/trunk/netsurf/Docs/"><strong>Te&nbsp;doen</strong></a>-lijsten.Het <a href="http://netsurf-browser.org/plan">projectontwikkelingsplan</a> toont de huidige prioriteiten. De mogelijkheden zijn alsvolgt gegroepeerd:</p>

<ul>
<li><a href="#Web">Web-standaarden</a>
<li><a href="#CSSandHTML">Belangrijke CSS- en HTML-kenmerken</a>
<li><a href="#Images">Beeldondersteuning</a>
<li><a href="#ROStandards">RISC OS-standaarden</a>
<li><a href="#NSF">NetSurf-kenmerken</a>
<li><a href="#NlVertalingen">Nederlandse vertalingen</a>
</ul>

<p class="updated">Laatste opwaardering: <font color=red>15 november 2007</font></p>

<h2 id="Web">Web-standaarden</h2>

<table class="progress">
  <tr><th>Titel</th><th>Status</th><th>Toelichting</th></tr>
  <tr class="nearlydone"><td>Cookies</td><td>Bijna klaar</td><td>Gebruikersinterface niet beschikbaar<br>Niet-gevalideerde transacties niet gedaan</td></tr>
  <tr class="nearlydone"><td>CSS 1</td><td>Bijna klaar</td><td>Zie: <a href="http://source.netsurf-browser.org/*checkout*/trunk/netsurf/Docs/TODO-CSS"><strong>Te doen</strong></a>-lijst.</td></tr>
<tr class="inprogress"><td>CSS 2</td><td>In ontwikkeling</td><td>Zie: <a href="http://source.netsurf-browser.org/*checkout*/trunk/netsurf/Docs/TODO-CSS"><strong>Te doen</strong></a>-lijst.</td></tr>
<tr class="nearlydone"><td>HTML 4</td><td>Bijna klaar</td><td>Zie: <a href="http://source.netsurf-browser.org/*checkout*/trunk/netsurf/Docs/TODO-HTML"><strong>Te doen</strong></a>-lijst. Nog geen ondersteuning voor geordende lijsten.</td></tr>
<tr class="complete"><td>HTTP 1.1</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td>HTTP-authentificatie</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td>HTTPS</td><td>Klaar</td><td></td></tr>
<tr class="notplanned"><td>JavaScript</td><td>Niet gepland</td><td>Details in het hoofdstuk <a href="info#JavaScript">Gebruikersinformatie</a>.</td></tr>
</table>

<h2 id="CSSandHTML">Belangrijke CSS- en  HTML-kenmerken</h2>

<table class="progress">
<tr><th>Titel</th><th>Status</th><th>Toelichting</th></tr>
<tr class="nearlydone"><td>Achtergrondafbeeldingen</td><td>Bijna klaar</td><td><tt>background-attachment</tt> vereist meer werk.</td></tr>
<tr class="nearlydone"><td>Randen / Marges / <tt>Padding</tt></td><td>Bijna klaar</td><td></td></tr>
<tr class="nearlydone"><td>Formulieren</td><td>Bijna klaar</td><td>Werkt nog niet helemaal met <tt>Scale View</tt>.</td></tr>
<tr class="nearlydone"><td>Frame's</td><td>Bijna klaar</td><td>Marge-attributen werken niet.<br>Frames hebben invloed op vorige/volgende-navigatie en historie-duimnagels.</td></tr>
<tr class="complete"><td>Beeld<font color=green>[Image]</font>mappen</td><td>Klaar</td><td></td></tr>
<tr class="nearlydone"><td>Lijsten</td><td>Bijna klaar</td><td></td></tr>
<tr class="nearlydone"><td>Positie (CSS2-attribuut)</td><td>Bijna klaar</td><td>Relatieve en absolute positionering werkt.<br>Vaste positionering werkt niet.<br>Stapelvolgorde is niet correct.</td></tr>
<tr class="nearlydone"><td>Tabellen</td><td>Bijna klaar</td><td>Elementen <tt>col</tt>, <tt>colgroup</tt>, <tt>rowgroup</tt> en <tt>caption</tt> werken niet.<br>Extra ruimte wordt gelijk verdeeld tussen kolommen inplaats van in verhouding met de breedte van de inhoud.</td></tr>
<tr class="inprogress"><td>Vertikaal uitlijnen</td><td>In ontwikkeling</td><td>Werkt alleen voor tabellen.</td></tr>
</table>

<h2 id="Images">Beeld-ondersteuning</h2>

<table class="progress">
<tr><th>Titel</th><th>Status</th><th>Toelichting</th></tr>
<tr class="complete"><td>Artworks</td><td>Klaar</td><td>Vereist <a href="http://www.mw-software.com/software/awmodules/awrender.html"><strong>AWRender</strong></td></tr>
<tr class="complete"><td>BMP</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td>Drawfile</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td>GIF</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td>ICO</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td>JNG</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td>JPEG</td><td>Klaar</td><td></td></tr>
<tr class="nearlydone"><td>MNG</td><td>Bijna klaar</td><td>Interactieve mogelijkheden worden niet ondersteund.</td></tr>
<tr class="nearlydone"><td>PNG</td><td>Bijna klaar</td><td>Gamma-correctie is niet correct voor PNG's met alpha-kanalen.</td></tr>
<tr class="complete"><td>Sprite</td><td>Klaar</td><td></td></tr>
</table>

<h2 id="ROStandards">RISC OS-standaarden</h2>

<table class="progress">
<tr><th>Title</th><th>Status</th><th>Toelichting</th></tr>
<tr class="complete"><td>Acorn URI Protocol</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td>Ant URL Protocol</td><td>Klaar</td><td></td></tr>
<tr class="nearlydone"><td>Plugin Protocol</td><td>Bijna klaar</td><td>Niet volledig uitgevoerd.</td></tr>
</table>

<h2 id="NSF">NetSurf-kenmerken</h2>

<table class="progress">
<tr><th>Titel</th><th>Status</th><th>Toelichting</th></tr>
<tr class="complete"><td>Advertentieblokkering</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td><font color=red>[Buffered rendering]</font></td><td>Klaar</td><td>Verbeterd het tonen van de inhoud substantieel.<br>Details in de <a href="guide.nl#BrowserWindowMenuDisplayRender">handleiding</a>.</td></tr>
<tr class="complete"><td>Configuratie</td><td>Klaar</td><td></td></tr>
<tr class="notstarted"><td>Schijfvoorraad</td><td>Nog niet begonnen.</td><td></td></tr>
<tr class="complete"><td>Dithering</td><td>Klaar</td><td>Verbetert de weergave als er te weinig kleuren beschikbaar zijn.<br>Zowel eenvoudige <font color=red>[dithering]</font> als foutenspreiding <font color=green>[Error diffusion]</font> zijn beschikbaar.</td></tr>
<tr class="nearlydone"><td>Documentatie</td><td>Bijna klaar</td><td>Zie de <a href="docs.nl">inhoudspagina</a>.</td></tr>
<tr class="nearlydone"><td>Tekst zoeken</td><td>Bijna klaar</td><td>Kan nog geen uitdrukkingen vinden wanneer ze over twee regels opgesplitst zijn.</td></tr>
<tr class="nearlydone"><td>Full Save</td><td>Bijna klaar</td><td>Pagina's met CSS-bestanden en afbeeldingen opslaan.<br>URL's voor CSS achtergrondafbeeldingen worden niet herschreven.</td></tr>
<tr class="nearlydone"><td>Historie (globaal)</td><td>Bijna klaar</td><td>Details in de <a href="guide_nl#History">handleiding</a>.</td></tr>
<tr class="complete"><td>Historie (lokaal)</td><td>Klaar</td><td>Details in de <a href="guide.nl#History">handleiding</a>.</td></tr>
<tr class="nearlydone"><td>Bladwijzers</td><td>Bijna klaar</td><td>Details in de <a href="guide.nl#Hotlist">handleiding</a>.</td></tr>
<tr class="nearlydone"><td>Interactieve hulp</td><td>Bijna klaar</td><td></td></tr>
<tr class="complete"><td>Geheugen <font color=red>[Cache]</font></td><td>Klaar</td><td></td></tr>
<tr class="inprogress"><td>Afdrukken</td><td>In ontwikkeling</td><td>Afdrukken met RISC OS 5 werkt niet, wegens het ontbreken van ondersteuning in de Font Manager en de printerdrivers.</td></tr>
<tr class="inprogress"><td><font color=red>[Proxy]</font>-ondersteuning</td><td>In ontwikkeling</td><td>Protocollen anders dan HTTP worden onjuist afgehandeld.</td></tr>
<tr class="nearlydone"><td>Bewaar als Draw-bestand</td><td>Bijna klaar</td><td>Ingebedde Draw-bestanden worden niet geexporteerd bij bewaren van een pagina als Draw-bestand.</td></tr>
<tr class="complete"><td>Bewaar locatie</td><td>Klaar</td><td>Bewaart de plaats van de pagina / object in Acorn URI, Ant URL of platte tekst-formaten.</td></tr>
<tr class="inprogress"><td>Bewaar als tekst</td><td>In ontwikkeling</td><td>Spati&euml;ring van tekst gaat soms fout.</td></tr>
<tr class="complete"><td>Schaal <font color=red>[View]</font></td><td>Klaar.</td><td>Webpagina's en afbeeldingen kunnen vergroot en verkleind worden.</td></tr>
<tr class="inprogress"><td>Tekstselectie</td><td>In ontwikkeling</td><td>Tekstselectie is niet compleet in tekstvelden.</td></tr>
<tr class="complete"><td>Thema's</td><td>Klaar</td><td>Er zijn thema's beschikbaar op de <a href="htpp://netsurf-browser.org/themes/"><strong>thema</strong></a>-pagina.</td></tr>
<tr class="complete"><td>Gereedschapbalkaanpassing</td><td>Klaar</td><td>Gereedschapbalken kunnen aangepast worden met gewenste symbolen in elke gewenste volgorde.<br>Details in de <a href="guide#ConfigurationToolbar">handleiding</a>.</td></tr>
<tr class="inprogress"><td>Unicode-lettertypes</td><td>In ontwikkeling</td><td>Details in de <a href="info.nl#Unicode"> gebruikers informatie</a> pagina.</td></tr>
<tr class="complete"><td>URL-aanvulling</td><td>Klaar</td><td>Bij het intypen van een URL geeft NetSurf suggesties voor het volledige adres.</td></tr>
<tr class="complete"><td>Broncode tonen</td><td>Klaar</td><td>Druk op F8 of <i>Pagina &gt; Bekijk HTML</i> in het hoofdmenu.</td></tr>
</table>

<h2 id="NlVertalingen">Nederlandse vertalingen</h2>
<p>Vanaf: <font color=red>17-06-2005</font></p>
<table class="progress">
<tr><th>Bestand</th><th>Status</th><th>Toelichting</th></tr>
<tr class="complete"><td>!Sprites22</td><td>Aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.docs_de</td><td>Aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.docs_en</td><td>Aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.docs_fr</td><td>Aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.docs_nl</td><td>Aangemaakt en vertaald </td><td>09-11-'07</td></tr>
<tr class="inprogress"><td>Docs.guide_nl</td><td>Aangemaakt en deels aangepast</td><td>27-05-'05 Moet nog getoetst worden.</td></tr>
<tr class="complete"><td>Docs.images.bwin_nl</td><td>Aangemaakt</td><td>21-05-'05</td></tr>
<tr class="nearlydone"><td>Docs.info_nl</td><td>Aangemaakt en aangepast </td><td>19-05-'05</td></tr>
<tr class="complete"><td>Docs.intro_de</td><td>Aangepast</td><td>21-05-'05</td></tr>
<tr class="complete"><td>Docs.intro_en</td><td>Aangepast</td><td>21-05-'05</td></tr>
<tr class="complete"><td>Docs.intro_fr</td><td>Aangepast</td><td>21-05-'05</td></tr>
<tr class="complete"><td>Docs.intro_nl</td><td>Aangemaakt, vertaald en aangepast</td><td>21-05-'05</td></tr>
<tr class="complete"><td>Docs.plan_nl</td><td>Aangemaakt en deels vertaald</td><td>26-04-'07</td></tr>
<tr class="complete"><td>Docs.netsurfnl_png</td><td>Aangemaakt en aangepast</td><td>Rood, wit en blauwe balletjes.</td></tr>
<tr class="complete"><td>Docs.overNS_nl</td><td>Aangemaakt en aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.progress_de</td><td>Aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.progress_en</td><td>Aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.progress_fr</td><td>Aangepast</td><td></td></tr>
<tr class="inprogress"><td>Docs.progress_nl</td><td>Aangemaakt</td><td>Deels vertaald (05-11-'07) en aangepast (15-11-'07)</td></tr>
<tr class="inprogress"><td>Resources.nl</td><td>Aangemaakt</td><td>Deels vertaald (27-05-'05)</td></tr>
</table>

<div class="footer">
<p class="breadcrumbs"><a href="/">NetSurf</a> » <a href="docs">Documentatie</a> » Voortgang</p>
</div>

</body>
</html>
