<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
        "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<title>NetSurf | Voortgang</title>
<link rel="stylesheet" type="text/css" href="netsurf.css">
</head>
<body>
<p class="banner"><a href="/"><img src="netsurfnl.png" alt="NetSurf"></a></p>
<p class="languages"><a href="progress.de">Deutsch</a> · <a href="progress.en">English</a> · <a href="progress.fr">Français</a> · Nederlands</p>

<h1>Van de vertaler</h1>

<p>Vanaf 21 oktober 2004 ben ik begonnen om bestanden waarvan (nog) geen nederlandstalige versie was, te vertalen naar het nederlands.</p>

<p>Delen waarvan ik (nog) niet zeker ben hoe deze juist vertaald moeten worden heb ik in de oorspronkelijke taal (Engels) laten staan maar wel tussen rechthoekige haken en in <font color=red>rood</font> gezet.</p>

<p>Waar ik vrij algemeen gebruikte engelstalige begrippen heb vervangen door nederlandstalige begrippen, heb ik die engelstalige begrippen er de eerste keer (ook tussen rechthoekige haken en in <font color=green>groen</font>) achter laten staan.</p>

<p>Wanneer ik een engelstalig begrip handhaafde, heb ik dit de eerste keer in groen gezet en er de eerste keer (ook tussen rechthoekige haken) een mogelijke vertaling vlak daarna gezet. Uiteraard zijn de keuzes persoonlijk en dus betwistbaar.

<p>Alle <font color=green>links</font> [verbindingsmogelijkheden] waarvoor verbinding met het net nodig is, staan <strong><font color=blue>vet</font></strong> afgebeeld.</p>

<p>Eerste vertaling: 21 oktober 2004<br>
Eerste aanpassing: 1 januari 2005<br>
Laatste aanpassing: 24 april 2005<br>
Laatste vertaling: 17 juni 2005</p>

<p class="headnote">Gerard van Katwijk (BBC; Regio Den Haag)</p>



<h1>Ontwikkelingsvoortgang</h1>

<p>Dit hoofdstuk dient om de gebruikers en (mede)ontwikkelaars een eerste indruk te geven van de geboden mogelijkheden en die waaraan nog gewerkt word. Voor meer gedetaileerde informatie; selecteer de <a href="http://cvs.semichrome.net/netsurf/Docs/"><strong>Te&nbsp;doen</strong></a>-lijsten. De mogelijkheden zijn alsvolgt gegroepeerd:</p>

<ul>
<li><a href="progress#Web">Web-standaarden</a>
<li><a href="progress#CSSandHTML">Belangrijke CSS- en HTML-kenmerken</a>
<li><a href="progress#Images">Beeldondersteuning</a>
<li><a href="progress#ROStandards">RISC OS-standaarden</a>
<li><a href="progress#NSF">NetSurf-kenmerken</a>
<li><a href="progress#NlVertalingen">Nederlandse vertalingen</a>
</ul>

<p class="updated">Laatste opwaardering: <font color=red>15 april 2005</font></p>

<h2 id="Web">Web-standaarden</h2>

<table class="progress">
  <tr><th>Titel</th><th>Status</th><th>Toelichting</th></tr>
  <tr class="inprogress"><td>Cookies</td><td>In ontwikkeling </td><td>Geldigheidsduur verloopt niet</td></tr>
  <tr class="nearlydone"><td>CSS 1</td><td>Bijna klaar</td><td>Zie: <a href="http://svn.semichrome.net/trunk/netsurf/Docs/TODO-CSS?view=markup"><strong>Te doen</strong></a>-lijst.</td></tr>
<tr class="inprogress"><td>CSS 2</td><td>In ontwikkeling</td><td>Zie: <a href="http://svn.semichrome.net/trunk/netsurf/Docs/TODO-CSS?view=markup"><strong>Te doen</strong></a>-lijst.</td></tr>
<tr class="nearlydone"><td>HTML 4</td><td>Bijna klaar</td><td>Zie: <a href="http://svn.semichrome.net/trunk/netsurf/Docs/TODO-HTML?view=log"><strong>Te doen</strong></a>-lijst. Nog geen ondersteuning voor geordende lijsten.</td></tr>
<tr class="complete"><td>HTTP 1.1</td><td>Klaar</td><td></td></tr>
<tr class="inprogress"><td>HTTP-controle</td><td>In ontwikkeling</td><td></td></tr>
<tr class="nearlydone"><td>HTTPS</td><td>Bijna klaar</td><td>Gaat niet goed om met ongeldige certificaten.</td></tr>
<tr class="notplanned"><td>JavaScript</td><td>Niet gepland</td><td>Details in het hoofdstuk <a href="info#JavaScript">Gebruikersinformatie</a>.</td></tr>
</table>

<h2 id="CSSandHTML">Belangrijke CSS- en  HTML-kenmerken</h2>

<table class="progress">
<tr><th>Titel</th><th>Status</th><th>Toelichting</th></tr>
<tr class="nearlydone"><td>Achtergrondafbeeldingen</td><td>Bijna klaar</td><td><tt>background-attachment</tt> vereist meer werk.</td></tr>
<tr class="nearlydone"><td>Randen / Marges / <tt>Padding</tt></td><td>Bijna klaar</td><td></td></tr>
<tr class="nearlydone"><td>Formulieren</td><td>Bijna klaar</td><td>Werkt nog niet helemaal met Scale View.</td></tr>
<tr class="juststarted"><td>Frame's</td><td>Net begonnen</td><td>Experimentele inpassing frame's is nog lang niet klaar.</td></tr>
<tr class="complete"><td>Beeld<font color=green>[Image]</font>mappen</td><td>Klaar</td><td></td></tr>
<tr class="juststarted"><td>Lijsten</td><td>Net begonnen</td><td>Lijsten nog niet goed hanteerbaar.</td></tr>
<tr class="notstarted"><td>Elementen positioneren (CSS2-attribuut)</td><td>Nog niet begonnen</td><td></td></tr>
<tr class="nearlydone"><td>Tabellen</td><td>Bijna klaar</td><td><tt>Elements col</tt>, <tt>colgroup</tt>, <tt>rowgroup</tt> en <tt>caption</tt> zijn nog niet beschikbaar.<br>Extra ruimte moet gelijk verdeeld worden tussen kolommen inplaats van in verhouding met de breedte van de inhoud.</td></tr>
<tr class="inprogress"><td>Vertikaal uitlijnen</td><td>In ontwikkeling</td><td>Alleen ingepast voor tabellen.</td></tr>
</table>

<h2 id="Images">Beeld-ondersteuning</h2>

<table class="progress">
<tr><th>Titel</th><th>Status</th><th>Toelichting</th></tr>
<tr class="complete"><td>Drawfile</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td>GIF</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td>JNG</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td>JPEG</td><td>Klaar</td><td></td></tr>
<tr class="nearlydone"><td>MNG</td><td>Bijna klaar</td><td>Interactieve mogelijkheden zijn niet ondersteund.</td></tr>
<tr class="nearlydone"><td>PNG</td><td>Bijna klaar</td><td>Gamma-correctie is niet correct voor PNG's met alpha-kanalen.</td></tr>
<tr class="complete"><td>Sprite</td><td>Klaar</td><td></td></tr>
</table>

<h2 id="ROStandards">RISC OS-standaarden</h2>

<table class="progress">
<tr><th>Title</th><th>Status</th><th>Toelichting</th></tr>
<tr class="complete"><td>Acorn URI Protocol</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td>Ant URL Protocol</td><td>Klaar</td><td></td></tr>
<tr class="inprogress"><td>Plugin Protocol</td><td>In ontwikkeling</td><td>Onvolledige inpassing en verschillende problemen.</td></tr>
</table>

<h2 id="NSF">NetSurf-kenmerken</h2>

<table class="progress">
<tr><th>Titel</th><th>Status</th><th>Toelichting</th></tr>
<tr class="complete"><td>Advertentie-/reclameblokkering</td><td>Klaar</td><td></td></tr>
<tr class="complete"><td><font color=red>[Buffered rendering]</font></td><td>Klaar</td><td>Bereid het tonen van de inhoud voor.<br>Zie de <a href="guide_en#BrowserWindowMenuDisplayRender">handleiding</a> voor meer details.</td></tr>
<tr class="inprogress"><td>Instelling</td><td>In ontwikkeling</td><td></td></tr>
<tr class="notstarted"><td></td><td>Nog niet begonnen.</td><td></td></tr>
<tr class="complete"><td>Dithering</td><td>Klaar</td><td>Verbetert de weergave als er weinig kleuren beschikbaar zijn.<br>Zowel eenvoudige <font color=red>[dithering]</font> en <font color=red>[Error diffusion]</font> zijn beschikbaar.</td></tr>
<tr class="inprogress"><td>Documentatie</td><td>In ontwikkeling</td><td></td></tr>
<tr class="nearlydone"><td>Vind tekst</td><td>Bijna klaar</td><td>Kan nog geen uitdrukkingen vinden wanneer ze opgesplitst zijn over twee regels.</td></tr>
<tr class="nearlydone"><td>Historie (globaal)</td><td>Bijna klaar</td><td></td></tr>
<tr class="complete"><td>Historie (lokaal)</td><td>Klaar</td><td><font color=red>[Local window history is not yet cloned when adjust clicking a link.]</font></td></tr>
<tr class="nearlydone"><td>Favorietenlijst</td><td>Bijna klaar</td><td></td></tr>
<tr class="inprogress"><td>Interactieve hulp</td><td>In ontwikkeling</td><td></td></tr>
<tr class="complete"><td>Geheugen <font color=red>[Cache]</font></td><td>Klaar</td><td></td></tr>
<tr class="inprogress"><td>Afdrukken</td><td>In ontwikkeling</td><td></td></tr>
<tr class="inprogress"><td><font color=red>[Proxy]</font>-ondersteuning</td><td>In ontwikkeling</td><td>Protocols anders dan HTTP worden onjuist <font color=red>[proxied]</font>.</td></tr>
<tr class="nearlydone"><td>Bewaar alles</td><td>Bijna klaar</td><td>Bewaard pagina's met CSS-bestanden en afbeeldingen. <font color=red>[Doesn't rewrite]</font> URL-adressen voor CSS-achtergrondafbeeldingen.</td></tr>
<tr class="nearlydone"><td>Bewaar als Draw-bestand</td><td>Bijna klaar</td><td><font color=red>[Embedded]</font> Draw-bestanden <font color=red>[aren't exported]</font> bij bewaren van een pagina als Draw-bestand.</td></tr>
<tr class="complete"><td>Bewaar locatie</td><td>Klaar</td><td>Bewaart de plaats van de pagina / object in Acorn URI, Ant URL of platte tekst-formaten.</td></tr>
<tr class="inprogress"><td>Bewaar als tekst</td><td>In ontwikkeling</td><td>Sommige <font color=red>[issues with spacing out of text]</font>.</td></tr>
<tr class="complete"><td>Schaal <font color=red>[View]</font></td><td>Klaar.</td><td>Webpagina's en afbeeldingen kunnen worden vergroot en verkleind.</td></tr>
<tr class="inprogress"><td>Tekstselectie</td><td>In ontwikkeling</td><td></td></tr>
<tr class="complete"><td>Onderwerpen</td><td>Klaar</td><td>Onderwerpen zijn beschikbaar op de NetSurf-<a href="htpp://netsurf.sourceforge.net/themes/"><strong>onderwerpen</strong></a>-pagina.</td></tr>
<tr class="complete"><td>Gereedschapbalkaanpassing</td><td>Klaar</td><td>Gereedschapbalken kunnen aangepast worden met gewenste symbolen in elke gewenste volgorde.</td></tr>
<tr class="inprogress"><td>Unicode-karakter-ondersteuning</td><td>In ontwikkeling</td><td></td></tr>
<tr class="complete"><td>URL-aanvulling</td><td>Klaar</td><td>Bij intypen van een URL geeft NetSurf suggesties voor het volledige adres.</td></tr>
<tr class="complete"><td>Broncode tonen</td><td>Klaar</td><td>Gebruik F8 of <em>Page> Vieuw source</em> in het hoofdmenu.</td></tr>
</table>

<h2 id="NlVertalingen">Nederlandse vertalingen</h2>
<p>Per: <font color=red>17-06-2005</font></p>
<table class="progress">
<tr><th>Bestand</th><th>Status</th><th>Toelichting</th></tr>
<tr class="complete"><td>!Sprites22</td><td>Aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.docs_de</td><td>Aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.docs_en</td><td>Aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.docs_fr</td><td>Aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.docs_nl</td><td>Aangemaakt en vertaald </td><td>19-05-'05</td></tr>
<tr class="inprogress"><td>Docs.guide_nl</td><td>Aangemaakt en deels aangepast</td><td>27-05-'05 Moet nog getoetst worden.</td></tr>
<tr class="complete"><td>Docs.images.bwin_nl</td><td>Aangemaakt</td><td>21-05-'05</td></tr>
<tr class="nearlydone"><td>Docs.info_nl</td><td>Aangemaakt en aangepast </td><td>19-05-'05</td></tr>
<tr class="complete"><td>Docs.intro_de</td><td>Aangepast</td><td>21-05-'05</td></tr>
<tr class="complete"><td>Docs.intro_en</td><td>Aangepast</td><td>21-05-'05</td></tr>
<tr class="complete"><td>Docs.intro_fr</td><td>Aangepast</td><td>21-05-'05</td></tr>
<tr class="complete"><td>Docs.intro_nl</td><td>Aangemaakt, vertaald en aangepast</td><td>21-05-'05</td></tr>
<tr class="complete"><td>Docs.netsurfnl_png</td><td>Aangemaakt en aangepast</td><td>Rood, wit en blauwe balletjes.</td></tr>
<tr class="complete"><td>Docs.overNS_nl</td><td>Aangemaakt en aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.progress_de</td><td>Aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.progress_en</td><td>Aangepast</td><td></td></tr>
<tr class="complete"><td>Docs.progress_fr</td><td>Aangepast</td><td></td></tr>
<tr class="inprogress"><td>Docs.progress_nl</td><td>Aangemaakt</td><td>Deels vertaald (21-05-'05)</td></tr>
<tr class="inprogress"><td>Resources.nl</td><td>Aangemaakt</td><td>Deels vertaald (27-05-'05)</td></tr>
</table>

<div class="footer">
<p class="breadcrumbs"><a href="/">NetSurf</a> » <a href="docs">Inhoudsopgave</a> » Voortgang</p>
</div>

</body>
</html>
