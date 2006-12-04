<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
        "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<title>NetSurf | Gebruikersinformatie</title>
<link rel="stylesheet" type="text/css" href="netsurf.css">
</head>

<body>
<p class="banner"><a href="/"><img src="netsurfnl.png" alt="NetSurf"></a></p>
<p class="languages"><a href="info.de">Deutsch</a> · <a href="info.en">English</a> · <a href="info.fr">Français</a> · Nederlands</p>

<h1>1. Gebruikersinformatie</h1>

<ul>
<li><a href="#GettingStarted">Starten</a>
<ul>
<li><a href="#GettingStartedInstallation">Installatie</a></li>
<li><a href="#GettingStartedRequirements">Verzoeken</a></li>
<li><a href="#GettingStartedAdditionalResources">Aanvullingen</a></li>
</ul></li>
<li><a href="#Upgrading">Opwaardering</a></li>
<li><a href="#ContactDevelopers">Contact maken met de ontwikkelaars</a></li>
<li><a href="#Bugs">Foutmelding</a></li>
<li><a href="#FeatureRequests">Toekomstwensen</a></li>
<li><a href="#Unicode">Lettertypes</a>
<ul>
<li><a href="#UnicodeInstallingFonts">Installeren van meer lettertypes</a></li>
<li><a href="#UnicodeProblems">Problemen en niet beschikbare mogelijkheden</a></li>
</ul></li>
<li><a href="#JavaScript">JavaScript</a></li>
</ul>

<p class="updated">Laatste aanpassing: <font color=red>19 mei 2005</font></p>

<h2 id="GettingStarted">Starten</h2>

<h3 id="GettingStartedInstallation">Installatie</h3>

<p>NetSurf vereist RISC OS 3.5 of later. NetSurf is verkrijgbaar bij de <a href="http://netsurf.sourceforge.net"><strong>webstek</strong></a> van het project. Er zijn drie typen beschikbaar. Voor de meeste gebruikers wordt &quot;NetSurf&quot; aanbevolen. &quot;NetSurf small&quot; is bedoeld voor systemen met minder geheugen en daarom ontbreken sommige mogelijkheden.</p>

<p>Voor installatie van &quot;NetSurf&quot; of &quot;NetSurf small&quot; kan de gewenste !NetSurf-map naar de gewenste plaats worden gesleept en van daaruit worden gestart.</p>

<h3 id="GettingStartedRequirements">Verzoeken</h3>

<p>Wanneer NetSurf is gestart, wordt automatisch contact gemaakt met alle bronnen die nodig zijn voor een correcte werking. Wanneer iets niet wordt gevonden, verschijnt er een melding van hetgeen niet kan worden gevonden. In dat geval gaat het waarschijnlijk om &eacute;&eacute;n van de volgende zaken:</p>

<dl>
<dt><a href="http://sudden.recoil.org/others/">Acorn URI</a></dt>
<dd>De Acorn URI-module is nodig <font color=red>[to pass]</font> URIs (bijvoorbeeld URL's) heen en weer tussen verschillende applicaties.</dd>
<dt><a href="http://netsurf-browser.org/iconv/">Iconv</a></dt>
<dd>Deze module zorgt voor een verbeterde karakterondersteuning.</dd>
<dt><a href="http://www.riscos.info/downloads/gccsdk/sharedunixlib/system.zip">SharedUnixLibrary</a></dt>
<dd>Dit is een ondersteunende module voor UnixLib-programma's.</dd>
<dt><a href="http://www.tinct.net/tinct.asp">Tinct</a></dt>
<dd>Tinct is nodig voor het <font color=green>plotten</font> van afbeeldingen. Het maakt het mogelijk om <font color=red>[sprites]</font> te plotten met alpha-kanalen.</dd>
<dt>Window Manager 3.80 of later</dt>
<dd>NetSurf gebruikt de Nested Wimp, dat een onderdeel is van Window Manager. Deze hoort bij RISC OS 4 of latere versies. RISC OS 3 gebruikers moeten <a href="http://acorn.riscos.com/riscos/releases/UniBoot/">Universal Boot Sequence</a> installeren.</dd>
</dl>

<p>Wanneer NetSurf zonder problemen start, is dat een teken dat aan alle vereisten voor het gebruik is voldaan.</p>

<h3 id="GettingStartedAdditionalResources">Aanvullende bronnen</h3>

<p>De volgende bronnen zijn niet vereist voor de goede werking van NetSurf, maar voegen wel meer mogelijkheden toe.</p>

<dl>
<dt><a href="http://www.ecs.soton.ac.uk/~jmb202/">Flash plugin</a></dt>
<dd>Deze programma maakt het mogelijk Flash-bestanden op webstekken te bekijken.</dd>
</dl>

<h2 id="Upgrading">Opwaardering</h2>

<p>Om een nieuwere versie van Netsurf te gaan gebruiken, is het voldoende om de te vervangen versie te verwijderen en de nieuwere versie naar de gewenste plaats te slepen. NetSurf bewaard de keuzen, <font color=red>[cookies]</font>, favorietenlijst en onderwerpen in <em>Choices:WWW.NetSurf</em>. Daardoor kan !NetSurf worden verwijderd zonder verlies van de ingestelde NetSurf-configuratie.</p>

<p>Het is mogelijk om een nieuwere NetSurf-versie over een oudere versie te kopi&euml;ren. Daardoor blijven bestanden die niet meer nodig zijn, achter in NetSurf. Dit kan echter verder geen kwaad.</p>

<h2 id="ContactDevelopers">Contact maken met de ontwikkelaars</h2>
<p>Voor foutmeldingen en vragen over NetSurf kun je <font color=red>(in het engels!!!)</font> terecht bij de volgende forums:</p>

<h3>Verzendlijst</h3>
<p>De <a href="http://lists.sourceforge.net/lists/listinfo/netsurf-develop"><strong>netsurf-ontwikkeling</strong></a>-verzendlijst is bedoeld voor discussie over NetSurf. Wanneer je ge&iuml;nteresseerd bent in de verdere ontwikkeling van NetSurf, geef je dan op voor deze lijst. Alle informatie voor deze lijst wordt gelezen en bewaard door het ontwikkelteam.</p>

<h3>IRC-kanaal</h3>
<p>Het #netsurf-ontwikkel IRC-kanaal op het <a href="http://freenode.net/"><strong>Freenode</strong></a>-netwerk is bedoeld voor discussie over NetSurf-ontwikkeling.</p>

<h3>Foutzoeker</h3>
<p>Geef elke fout door aan de <a href="http://sourceforge.net/tracker/?group_id=51719&amp;atid=464312"><strong>foutzoeker</strong></a>.</p>

<p>Wanneer je een bijdrage wilt leveren aan het NetSurf-project, neem dan <font color=red>(in het engels!!!)</font> contact op met de ontwikkelaars op de verzendlijst of IRC.</p>

<h2 id="Bugs">Foutmelding</h2>
<p>De NetSurf-ontwikkelaars zijn zeer blij met foutmeldingen. Volg daarbij wel de volgende richtlijnen:</p>

<ol>
<li>Controleer dat de fout voorkomt in de <a href="http://netsurf.strcprstskrzkrk.co.uk/"><strong>laatst beschikbare versie</strong></a>.</li>
<li>Probeer een manier te vinden om de fout structureel te verhelpen.</li>
<li>Controleer het <a href="progress">voortgang</a>hoofdstuk en de <a href="http://source.netsurf-browser.org/trunk/netsurf/Docs/"><strong>Te doen</strong></a>-lijsten om na te gaan of de fout kortgeleden reeds is gemeld en/of verholpen.</li>
</ol>

<p>Voeg bij het melden van fouten de volgende informatie toe:</p>

<ol>
<li>De NetSurf-versie die word gebruikt.</li>
<li>Welke hardware en welk OS word gebruikt.</li>
<li>Zo precies en duidelijk mogelijke omschrijving van het probleem. (bijv. loopt de computer vast, loopt NetSurf vast, komt er een foutmelding, is er een layout-probleem met een bepaalde netstek? Wanneer het laatste het geval is, leg dan uit wat er niet goed is.)</li>
<li>Wanneer een bepaalde netstek faalt, geef dan het volledige URL-adres op.</li>
<li>Geef een lijst met de preciese stappen om de fout te laten optreden.</li>
<li>Wanneer je de fout terug kunt brengen in een zo klein mogelijke testsituatie, is dat fantastisch. Waarschijnlijk wordt daardoor het vinden van de oplossing versneld. Maak je geen zorgen wanneer je dat niet kunt.</li>
</ol>

<p>Het volgen van deze richtlijnen spaart de ontwikkelaars tijd en versnelt de ontwikkeling NetSurf.</p>

<h2 id="FeatureRequests">Toekomstwensen</h2>

<p>Het NetSurf-team heeft veel belangstelling voor toekomstwensen en vernieuwende idee&euml;n ter verbetering van blader[brow]experimenten. Kijk eerst in het <a href="progress">voortgang</a>hoofdstuk en de <a href="http://source.netsurf-browser.org/trunk/netsurf/Docs/"><strong>Te doen</strong></a>-lijsten om te zien wat er al staat gepland. Suggesties kunnen verzonden worden naar de <a href="info#Devel">verzendlijst</a>. Alle ingezonden informatie wordt gelezen en beoordeeld door de NetSurf-ontwikkelaars.</p>

<h2 id="Unicode">Lettertypes</h2>

<p>NetSurf ondersteunt het gebruik van Unicodelettertypes die normaal niet beschikbaar zijn voor RISC OS, bij voorbeeld Latijns met accenten, Grieks, Cyrillisch, Japans en verschillende symbolen.</p>

<p>De <a href="guide.en#ConfigurationGlobalFonts">lettertype-keuzes</a> maken het mogelijk een letttertype te kiezen voor elk van de vijf standaardfamilies die beschikbaar zijn webontwikkelaars (in CSS). De keuzes specificeren het gewenste lettertype. Wanneer een letter niet beschikbaar is in het gekozen lettertype maar wel aanwezig is in een ander geinstalleerd lettertype dan zal NetSurf dit automatisch gebruiken. Het is dus niet nodig om de lettertypekeuze te veranderen om pagina's te kunnen lezen met een letter(type) dat niet beschikbaar is in het gekozen lettertype.</p>

<p>N.b. Er kan alleen een lettertype gekozen worden. NetSurf gebruikt automatisch de bijpassende zwaarte voor vetgedrukte tekst en schuingedrukte tekst, indien beschikbaar.</p>

<h3 id="UnicodeInstallingFonts">Lettertypes installeren</h3>

<p>De beschikbare lettertypes voor RISC OS zijn Latijns (Homerton, Trinity, Corpus), Grieks (Sidney) en verschillende symbolen (Selwyn, Sidney). (Met RISC OS 3-4, zijn alleen de &quot;Latin 1&quot; letters van de standaardlettertypes die gebruikt worden in de West-Europese talen beschikbaar met NetSurf).</p>

<p>Om pagina's met andere lettertypes correct te kunnen tonen moeten die lettertypes geinstalleerd zijn. Wanneer een letter niet beschikbaar is in een welk beschikbaar lettertype ook, dan zal de letter in Unicode getoond worden.</p>

<p>Codes beginnend met 009, geven aan dat de pagina het gebruikte lettertype niet juist specificeert. Installeren van lettertypes helpt niet. We hebben nog geen oplossing gevonden om dit probleem zo goed mogelijk te voorkomen/verhelpen.</p>

<p>Elk lettertype dat ondersteund wordt door een goed ontworpen coderingbestand zal geen enkel probleem geven. In de praktijk komen zelden lettertypes voor die niet gedekt worden door Latin 1. De oplossing is om TrueType-lettertypes om te zetten met <a href="http://moose.mine.nu/ttf2f_latest.zip"><strong>TTF2f</strong></a> (momenteel zijn alleen lettertypes voor RISC OS 5 beschikbaar).</p>

<p>N.b. Na het installeren van lettertypes moet NetSurf opnieuw gestart worden om die lettertypes te kunnen detecteren.</p>

<h3 id="UnicodeProblems">Problemen en niet beschikbare mogelijkheden</h3>

<ul>
<li>Het standaardlettertype is altijd sans-serif.</li>
<li>Afdrukken met RISC OS 5 werkt niet vanwege het gebrek aan ondersteuning van de Font Manager en printerdrivers. Afdrukken met Postscript-printers met RISC OS 3-4 is niet correct ingevoerd in NetSurf.</li>
<li>Vervangende lettertypes komen van het eerst beschikbare lettertype, zelfs wanneer elders een lettertype beschikbaar is waarvan de zwaarte van de vetgedrukte letter of de schuinte beter overeenkomt.</li>
<li>Alleen twee zwaarten (normaal en vet) worden ondersteund, zelfs al heeft een lettertype andere zwaarten. Het algoritme dat zwaarten zoekt moet beter worden, bij voorbeeld door gebruik te maken van de methoden die voorgesteld worden in <a href="http://www.w3.org/TR/CSS21/"><strong>CSS 2.1</strong></a>, deel <a href="http://www.w3.org/TR/CSS21/fonts.html#font-boldness"><strong>15.6</strong></a>.</li>
<li>Drawfile-export is <font color=red>[broken]</font>.</li>
<li>Tekst die van rechts naar links leest (Arabisch, Hebreeuws) is niet ingevoerd.</li>
</ul>

<h2 id="JavaScript">JavaScript</h2>

<p>Momenteel, beschikt NetSurf niet over de mogelijkheid om het gebruik van JavaScript te ondersteunen. Zonder deze mogelijkheid geeft NetSurf toegang tot de meeste stekken op het Wereld Wijde Net<font color=green>[Web]</font>. Sommige stekken echter, kunnen niet correct worden getoond vanwege de sterke afhankelijkheid van deze standaard.</p>

<p>Zoals blijkt in het <a href="progress">voortgang</a>hoofdstuk, staat JavaScript momenteel &quot;niet gepland&quot;. Dit is hoofdzakelijk vanwege het vele werk dat daarvoor nodig zou zijn en de beperkte tijd van de ontwikkelaars. Onnodig om te zeggen, dat wanneer iemand de tijd en motivatie zou hebben om ondersteuning van JavaScript mogelijk te maken, de huidige NetSurf-ontwikkelaars alle hulp zullen geven die mogelijk is.</p>

<div class="footer">
<p class="breadcrumbs"><a href="/">NetSurf</a> » <a href="docs">Documentatie</a> » Gebruikersinformatie</p>
</div>

</body>
</html>
