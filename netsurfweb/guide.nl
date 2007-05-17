<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
        "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<title>NetSurf | Handleiding</title>
<link rel="stylesheet" type="text/css" href="netsurf.css">
</head>

<body>
<p class="banner"><a href="/"><img src="netsurf.png" alt="NetSurf"></a></p>
<p class="languages"><a href="guide.de">Deutsch</a> · <a href="guide.en">English</a> · Nederlands</p>

<p class="headnote">NetSurf wordt snel verder ontwikkeld maar bevind zich nog steeds in het beginstadium. Daarom kunnen de navolgende mogelijkheden zijn veranderd zonder waarschuwing.</p>

<h1>2. Handleiding</h1>

<ul>
<li><a href="#UsingNetSurf">Gebruik</a></li>
<li><a href="#Help">Interactieve hulp</a></li>
<li><a href="#BrowserWindow">Bladervenster</a>
<ul>
<li><a href="#BrowserWindowToolbar">Gereedschapbalk</a></li>
<li><a href="#BrowserWindowURLBar">Adresvenster</a></li>
<li><a href="#BrowserWindowStatusBar">Statusbalk</a></li>
<li><a href="#BrowserWindowContent">Inhoudvenster</a></li>
<li><a href="#BrowserWindowMenus">Menu's</a></li>
</ul></li>
<li><a href="#FileTypes">Bestandstypen</a></li>
<li><a href="#DownloadWindow">Ophaalvenster</a></li>
<li><a href="#History">Historie</a>
<ul>
<li><a href="#HistoryLocal">Lokaal historievenster</a></li>
<li><a href="#HistoryGlobal">Globaal historievenster</a>
<ul>
<li><a href="#HistoryGlobalToolbar">Gereedschapbalk</a></li>
<li><a href="#HistoryGlobalMenus">Menu's</a></li>
</ul></li>
</ul></li>
<li><a href="#Hotlist">Favorietenvenster</a>
<ul>
<li><a href="#HotlistToolbar">Gereedschapbalk</a></li>
<li><a href="#HotlistMenus">Menu's</a></li>
</ul></li>
<li><a href="#Configuration">Instelling</a>
<ul>
<li><a href="#ConfigurationGlobal"><font color=red>[Global]</font> keuzes</a></li>
<li><a href="#ConfigurationToolbar">Gereedschapbalkaanpassing</a></li>
</ul></li>
<li><a href="#Keys">Toetscombinaties</a></li>
</ul>

<p class="updated">Laatste versie: <font color=red>26 april 2005</font>.</p>

<h2 id="UsingNetSurf">Gebruik</h2>

<p>Om NetSurf te starten, volstaat het om te dubbelklikken op het NetSurf-symbool. Daardoor verschijnt het NetSurf-symbool op de programma- of symbolenbalk. Door daar dat symbool te selecteren wordt een <a href="#BrowserWindow">bladervenster</a> geopend. Door het NetSurf-symbool met de PasAan-muisknop te selecteren, verschijnt het <a href="#Hotlist">favorietenvenster</a>.</p>

<h3>Symbolenbalkmenu</h3>

<p>Het synbolenbalkmenu biedt mogelijkheden <font color=red>[which concern the application as a whole]</font>.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">NetSurf</th></tr>
<tr><td><a href="#IconbarMenuInfo">&nbsp;&nbsp;Informatie</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#IconbarMenuHelp">&nbsp;&nbsp;Hulp</a></td><td></td></tr>
<tr><td><a href="#IconbarMenuOpen">&nbsp;&nbsp;Open</a></td><td class="menuarrow">&gt;</td></tr>
<tr><td><a href="#IconbarMenuChoices">&nbsp;&nbsp;Keuzes</a></td><td></td></tr>
<tr><td><a href="#IconbarMenuQuit">&nbsp;&nbsp;Stop</a></td><td></td></tr>
</table>

<p class="menupath" id="IconbarMenuInfo">NetSurf &gt; Informatie</p>
<p class="menuitem">Deze mogelijkheid geeft basisinformatie over NetSurf. Meer informatie kan gevonden worden in het <a href="#BrowserWindowMenuHelpAbout">Over NS</a>-hoofdstuk.</p>

<p class="menupath" id="IconbarMenuHelp">NetSurf &gt; Hulp</p>
<p class="menuitem">Deze mogelijkheid opent het <a href="docs">documentatie</a>hoofdstuk in een nieuw venster.</p>

<p class="menupath" id="IconbarMenuOpen">NetSurf &gt; Open</p>
<p class="menuitem">Met deze optie kunnen verschillende vensters worden geopend:</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Open</th></tr>
<tr><td><a href="#IconbarMenuOpenURL">&nbsp;&nbsp;Open URL</a></td><td class="menuarrow">&gt;</td></tr>
<tr><td><a href="#IconbarMenuOpenHotlist">&nbsp;&nbsp;Toon favorietenlijst</a></td><td></td></tr>
<tr><td><a href="#IconbarMenuOpenHistory">&nbsp;&nbsp;Toon <font color=red>[global]</font> historie</a></td><td></td></tr>
</table>

<p class="menupath" id="IconbarMenuOpenURL">NetSurf &gt; Open &gt; Open URL</p>
<p class="menuitem">Dit opent een dialoogvenster, dat toegang geeft tot een adres (URL).</p>

<p class="menupath" id="IconbarMenuOpenHotlist">NetSurf &gt; Open &gt; Toon favorietenlijst</p>
<p class="menuitem">Dit opent het <a href="#Hotlist">managementvenster</a> van de favorietenlijst.</p>

<p class="menupath" id="IconbarMenuOpenHistory">NetSurf &gt; Open &gt; Toon <font color=red>[global]</font> historie</p>
<p class="menuitem">Dit opent het <a href="#HistoryGlobal"><font color=red>[global]</font> historievenster</a>.</p>

<p class="menupath" id="IconbarMenuChoices">NetSurf &gt; Keuzes</p>
<p class="menuitem">Deze mogelijkheid opent het <a href="#Configuration">instelling</a>venster.</p>

<p class="menupath" id="IconbarMenuQuit">NetSurf &gt; Stop</p>
<p class="menuitem">Deze mogelijkheid be&euml;indigt het programma, waarbij alle programmavensters worden gesloten en het ophalen van informatie word gestopt. NetSurf biedt geen mogelijkheden voor het afhandelen van nog niet geheel opgehaalde of afgebroken informatie. Wanneer NetSurf is be&euml;indigd, zijn alle wijzigingen van de <a href="#HistoryGlobal"><font color=red>[global]</font> historie</a> en <a href="#BrowserWindowURLCompletion">URL <font color=red>[complete]</font></a>bewaard.</p>

<h2 id="Help">Interactieve hulp</h2>

<p>Onmiddelijke hulp is altijd verkrijgbaar in de vorm van interactieve hulp. Deze geeft snelle informatie over hetgeen door de muispijl word aangewezen.</p>

<p>Om gebruik te kunnen maken van interactieve hulp, is een passend programma gewenst. Het programma &rdquo;Help&rdquo; wordt standaard bij elke RISC OS-computer geleverd en kan geactiveerd worden vanuit NetSurf's menukeuze <a href="#BrowserWindowMenuHelpInteractive">Hulp &gt; Interactieve hulp</a>.</p>

<h2 id="BrowserWindow">Bladervenster</h2>

<p>Het bladervenster dient voor het tonen van webstekdelen en andere zaken die het web biedt. Het kan ook bestanden tonen van de eigen computer. De <a href="#FileTypes">bestandstypen</a> waarmee NetSurf kan omgaan zijn, onder andere, HTML, CSS en verschillende soorten beeldtypen.</p>

<p class="screenshot"><img src="images/bwin.en" title="Browser Window" alt=""></p>

<p>Het deel aan de bovenkant van het bladervenster is de <a href="#BrowserWindowToolbar">gereedschapbalk</a>. Onderaan het bladervenster valt te zien, dat de horizontale schuifbalk niet de hele breedte van het bladervenster in beslag neemt. Links er van bevind zich de <a href="#BrowserWindowStatusBar">statusbalk</a>. Het grote deel in het midden van het bladervenster is het belangrijkste deel. Hier wordt de inhoud van opgehaalde bestanden getoond.</p>

<p>Getoonde informatie kan verbindingsmogelijkheden (vaak &rdquo;<font color=green>links</font>&rdquo; genoemd) bevatten. Door deze te selecteren wordt daarmee verbonden informatie opgehaald en getoond. Dit kan een ander deel van de webstek zijn, een andere webstek maar ook ieder ander soort bestand. Wanneer het op te halen soort bestand voor NetSurf niet bekend is, word een <a href="#DownloadWindow">ophaalvenster</a> geopend. Zodra de muispijl zich op zo'n <font color=green>link</font> bevind, veranderd de muispijl in een hand met een wijzende vinger, ten teken dat dit een verbinding is die geselecteerd kan worden.</p>

<h3 id="BrowserWindowToolbar">Gereedschapbalk</h3>

<p>Het deel bovenaan het bladervenster is de gereedshapbalk. Deze maakt het snel beschikbaar zijn van diverse, vaak gebruikte toepassingen mogelijk. Veel andere mogelijkheden zijn beschikbaar via <a href="#BrowserWindowMenus">menu's</a>. De gereedschapbalk kan worden aangepast zodat alleen de gewenste symbolen in de gewenste volgorde worden getoond. Dit wordt verder toegelicht in het deel <a href="#ConfigurationToolbar">gereedschapbalk aanpassen</a>. Beschikbare symbolen staan hierna.</p>

<hr>
<font color=red>Hierna nog controleren!!!!!</font>

<h3>Gereedschapbalksymbolen</h3>

<p>Omdat NetSurf nog in ontwikkeling is, werken sommige symbolen nog niet of ontbreken zelfs. Hierna volgt de beschrijving van de functie van deze symbolen. Voor zover dit nog niet het geval is, worden symbolen actief zodra de bijbehorende invoeging heeft plaatsgevonden.</p>
<!-- Hierna de informatie in tabelvorm gegoten zodat het netter wordt vormgegeven. -->
<p class="iconinfo">
<table>
<tr><td><strong><img src="images/back" alt="Terug" title="Terug-symbool" class="icon"></strong></td><td>&nbsp;&nbsp;</td><td>Selectie van dit symbool, zorgt dat de inhoud van het daarvoor getoonde venster weer wordt getoond. Met de PasAan-muisknop wordt die inhoud in een nieuw bladervenster getoond. <font color=red>[Form information is not resubmitted]</font>.<br>&nbsp;</td></tr></p>

<p class="iconinfo">
<tr><td><strong><img src="images/forward" class="icon" alt="Volgende" title="Volgende-symbool"></strong></td><td>&nbsp;&nbsp;</td><td>Selectie van dit symbool, zorgt dat de inhoud van een navolgend bladervenster wordt getoond. Met de PasAan-muisknop wordt die inhoud in een nieuw bladervenster getoond. <font color=red>[Form information is not resubmitted]</font>.<br>&nbsp;</td></tr></p>

<p class="iconinfo">
<tr><td><strong><img src="images/history" class="icon" alt="Historie" title="Historie-symbool"></strong></td><td>&nbsp;&nbsp;</td><td>Selectie van dit symbool opent een venster met een historieschema. Daarin staan alle eerder geopende bladervensters waarbij het openstaande bladervenster in rood is aangegeven. Door een bladervenster uit dit schema te selecteren, wordt het openstaande bladervenster vervangen door het geselecteerde. Meer gedetailleerde informatie is te vinden in het hoofdstuk <a href="#HistoryWindow">historievenster</a>.<br>&nbsp;</td></tr></p>

<p class="iconinfo">
<tr><td><strong><img src="images/home" class="icon" alt="Start" title="Start-symbool"></strong></td><td>&nbsp;&nbsp;</td><td>Selectie van dit symbool, zorgt dat de inhoud van de start- of <font color=green>home</font>pagina wordt getoond.<br>&nbsp;</td></tr></p>

<p class="iconinfo">
<tr><td><strong><img src="images/hotlist" class="icon" alt="Favorieten" title="Favorieten-symbool"></strong></td><td>&nbsp;&nbsp;</td><td>Selectie van dit symbool, zorgt dat de inhoud van het favorietenbestand wordt getoond. Met de PasAan-muisknop wordt het adres van de getoonde informatie aan het favorietenbestand toegevoegd.<br>&nbsp;</td></tr></p>

<p class="iconinfo">
<tr><td><strong><img src="images/print" class="icon" alt="DrukAf" title="DrukAf-symbool"></strong></td><td>&nbsp;&nbsp;</td><td>Door selectie van dit symbool, verschijnt het afdruk- of <font color=green>print</font>dialoogvenster.<br>&nbsp;</td></tr></p>

<p class="iconinfo">
<tr><td><strong><img src="images/reload" class="icon" alt="Herlaad" title="Herlaad-symbool"></strong></td><td>&nbsp;&nbsp;</td><td>Selectie van dit symbool, zorgt voor het opnieuw samenstellen en tonen van de opgevraagde informatie. Met de PasAan-muisknop worden ook alle objecten, zoals afbeeldingen en stijlbladen opnieuw samengesteld.<br>&nbsp;</td></tr></p>

<p class="iconinfo">
<tr><td><strong><img src="images/save" class="icon" alt="Bewaar" title="Bewaar-symbool"></strong></td><td>&nbsp;&nbsp;</td><td>Door selectie van dit symbool, verschijnt een bewaar- of <font color=green>safe</font>dialoogvenster. Daarmee kan de informatie worden opgeslagen in de computer voor later gebruik.<br>&nbsp;</td></tr></p>

<p class="iconinfo">
<tr><td><strong><img src="images/scale" class="icon" alt="Schaal" title="Schaal-symbool"></strong></td><td>&nbsp;&nbsp;</td><td>Door selectie van dit symbool, verschijnt een schaalvenster. Daarmee kan de inhoud (zowel tekst als afbeeldingen) van het bladervenster kleiner of groter worden getoond.<br>&nbsp;</td></tr></p>

<p class="iconinfo">
<tr><td><strong><img src="images/search" class="icon" alt="Zoek" title="Zoek-symbool"></strong></td><td>&nbsp;&nbsp;</td><td>Door selectie van dit symbool, verschijnt het tekstzoekvenster. Daarmee kan gezocht worden naar een tekstdeel in het bestand.<br>&nbsp;</td></tr></p>

<p class="iconinfo">
<tr><td><strong><img src="images/stop" class="icon" alt="Stop" title="Stop-symbool"></strong></td><td>&nbsp;&nbsp;</td><td>Door selectie van dit symbool wordt elk proces onmiddelijk gestopt.<br>&nbsp;</td></tr></p>

<p class="iconinfo">
<tr><td><strong><img src="images/up" class="icon" alt="Omhoog" title="Omhoog-symbool"></strong></td><td>&nbsp;&nbsp;</td><td>Selectie van dit symbool, zorgt dat omhoog gegaan wordt in het directoriepad. Bij voorbeeld; wanneer je kijkt naar de inhoud van <em>http://www.domain.com/directory/page.html</em>, zal selectie van dit symbool de inhoud tonen van <em>http://www.domain.com/directory/</em> en een volgende selectie voert naar <em>http://www.domain.com/</em>.</td></tr></p>
</table>

<p>Symbolen waarvan de omtrek grijs is, zijn nog niet werkend. Dit is bij voorbeeld het geval wanneer een nieuw bladervenster voor het eerst wordt geopend. Immers, omdat het venster voor het eerst een bestand toont is er nog geen vorig bestand en is de omtrek van het terug-symbool dus nog grijs. Hetzelfde geldt in dat geval ook voor het volgende-symbool.</p>

<h3 id="BrowserWindowURLBar">Adresbalk</h3>

<p>De witte balk in de gereedschapbalk is de adresbalk (ook wel URL-balk genoemd) en toont het adres van het getoonde bestand. Hier kan ook het adres van een gewenst bestand worden getypt.</p>

<p><strong>Throbber:</strong> Dit NetSurf-symbool is actief tijdens het ophalen en toonbaar maken van een bestand. Het is een indicatie (net als de zandloper van ROSC OS of iets dergelijks), dat NetSurf iets aan het doen is.</p>

<h3 id="BrowserWindowStatusBar">Statusbalk</h3>

<p>De statusbalk, onderaan het bladervenster, geeft informatie voor de gebruiker. Wanneer het aanwijssymbool over een <font color=green>link</font> komt, wordt daar het <font color=green>URL</font>-adres getoond waarnaar die <font color=green>link</font> verwijst. De statusbalk geeft ook informatie over de voortgang van NetSurf bij het uitvoeren van taken.</p>

<p>Tussen de statusbalk en de horizontale schuifbalk zit een schuifknopje. Zodra het aanwijssymbool op de schuifknopje komt, veranderd het aanwijssymbool in een dubbel pijl. Daarna kan, door dit schuifknopje naar links of rechts te slepen de statusbalk smaller of breder gemaakt worden.</p>

<h3 id="BrowserWindowMenus">Menu's</h3>

<p>Het bladervenstermenu bevat mogelijkheden voor het geopende venster. Sommige mogelijkheden zijn nog niet beschikbaar omdat NetSurf nog niet compleet is. De volgende mogelijkheden zijn beschikbaar:</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">NetSurf</th></tr>
<tr><td><a href="#BrowserWindowMenuPage">&nbsp;&nbsp;Pagina</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuObject">&nbsp;&nbsp;Object</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuNavigate">&nbsp;&nbsp;Navigatie</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuDisplay">&nbsp;&nbsp;Toon</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuUtilities">&nbsp;&nbsp;<font color=red>[Utilities]</font></a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuHelp">&nbsp;&nbsp;Hulp</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
</table>

<h4 id="BrowserWindowMenuPage" class="submenu">Pagina-submenu</h4>

<p>Het pagina-submenu bevat de volgende mogelijkheden voor de geopende pagina:</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Pagina</th></tr>
<tr><td><a href="#BrowserWindowMenuPageInfo">&nbsp;&nbsp;Informatie</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuPageSave">&nbsp;&nbsp;Bewaar</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuPageFullSave">&nbsp;&nbsp;Bewaar alles</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuPageExport">&nbsp;&nbsp;Exporteer</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuPageSaveLocation">&nbsp;&nbsp;Bewaar adres</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuPagePrint">&nbsp;&nbsp;Druk af...</a></td><td></td></tr>
<tr><td class="menusplit"><a href="#BrowserWindowMenuPageNewwindow">&nbsp;&nbsp;Nieuw venster</a></td><td class="menusplit"></td></tr>
<tr><td><a href="#BrowserWindowMenuPageViewSource">&nbsp;&nbsp;Toon bron...</a></td><td class="menuarrow"></td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuPageInfo">Pagina &gt; Informatie</p>
<p class="menuitem">Deze menukeuze toont informatie over de getoonde pagina.</p>

<p class="menupath" id="BrowserWindowMenuPageSave">Pagina &gt; Bewaar</p>
<p class="menuitem">Deze menukeuze opent een bewaardialoogvenster, waardoor de broncode van de geopende pagina kan worden bewaard.</p>

<p class="menupath" id="BrowserWindowMenuPageFullSave">Pagina &gt; Bewaar alles</p>
<p class="menuitem">Deze menukeuze opent een bewaardialoogvenster, waardoor de geopende pagina, inclusief CSS-bestanden en afbeeldingen, kan worden bewaard.</p>

<p class="menupath" id="BrowserWindowMenuPageExport">Pagina &gt; Exporteer</p>
<p class="menuitem">Deze menukeuze biedt verschillende exportkeuzes voor de geopende pagina. Die mogelijkheden zijn:</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">&nbsp;Exporteer als</th></tr>
<tr><td><a href="#BrowserWindowMenuPageExportDraw">&nbsp;&nbsp;Draw</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuPageExportText">&nbsp;&nbsp;Tekst</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuPageExportDraw">Pagina &gt; Exporteer &gt; Draw</p>
<p class="menuitem">Deze menukeuze opent een bewaardialoogvenster. Daarmee kan de geopende pagina als Draw-bestand bewaard worden.</p>

<p class="menupath" id="BrowserWindowMenuPageExportText">Pagina &gt; Exporteer &gt; Tekst</p>
<p class="menuitem">Deze menukeuze opent een bewaardialoogvenster. Daarmee kan de geopende pagina als tekst-bestand bewaard worden.</p>

<p class="menupath" id="BrowserWindowMenuPageSaveLocation">Pagina &gt; Bewaar adres</p>
<p class="menuitem">Deze menukeuze opent de mogelijkheid om het adres van de geopende pagina in verschillende vormen te bewaren. Die verschillende vormen zijn:</p>

<table class="menu">
<tr><th class="menutitle" colspan="2"><font color=green>Link</font></th></tr>
<tr><td><a href="#BrowserWindowMenuPageSaveLocationAcorn">&nbsp;&nbsp;Acorn URI</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuPageSaveLocationAnt">&nbsp;&nbsp;Ant URL</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuPageSaveLocationText">&nbsp;&nbsp;Tekst</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuPageSaveLocationAcorn">Pagina &gt; Bewaar adres &gt; Acorn URI</p>
<p class="menuitem">Deze menukeuze opent een bewaardialoogvenster. Daarmee kan het adres van de geopende pagina, in Acorn URI <font color=red>[format]</font>, bewaard worden op diskette.</p>

<p class="menupath" id="BrowserWindowMenuPageSaveLocationAnt">Pagina &gt; Bewaar adres &gt; Ant URL</p>
<p class="menuitem">Deze menukeuze opent een bewaardialoogvenster. Daarmee kan het adres van de geopende pagina, in Acorn URL <font color=red>[format]</font>, bewaard worden op diskette.</p>

<p class="menupath" id="BrowserWindowMenuPageSaveLocationText">Pagina &gt; Bewaar adres &gt; Tekst</p>
<p class="menuitem">Deze menukeuze opent een bewaardialoogvenster. Daarmee kan het adres van de geopende pagina, in platte tekst, bewaard worden op diskette.</p>

<p class="menupath" id="BrowserWindowMenuPagePrint">Pagina &gt; Druk af...</p>
<p class="menuitem">Deze menukeuze opent een afdrukdialoogvenster.</p>

<p class="menupath" id="BrowserWindowMenuPageNewwindow">Pagina &gt; Nieuw venster</p>
<p class="menuitem">Deze menukeuze opent de huidige pagina in een nieuw venster.</p>

<p class="menupath" id="BrowserWindowMenuPageViewSource">Pagina &gt; Toon bron...</p>
<p class="menuitem">Deze menukeuze opent de broncode van de geopende pagina in een tekst<font color=green>editor</font>venster.</p>

<h4 id="BrowserWindowMenuObject" class="submenu">Object-submenu</h4>

<p>Het object-submenu is alleen beschikbaar wanneer het aanwijssymbool op een object, zoals een afbeelding, staat. Het biedt de volgende mogelijkheden voor objecten:</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Object</th></tr>
<tr><td><a href="#BrowserWindowMenuObjectInfo">&nbsp;&nbsp;Informatie</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuObjectSave">&nbsp;&nbsp;Bewaar</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuObjectExport">&nbsp;&nbsp;Exporteer</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuObjectSaveLocation">&nbsp;&nbsp;Bewaar adres</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td class="menusplit"><a href="#BrowserWindowMenuObjectReload">&nbsp;&nbsp;Herlaad</a></td><td class="menusplit"></td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuObjectInfo">Object &gt; Informatie</p>
<p class="menuitem">Deze menukeuze toont informatie over het betreffende object.</p>

<p class="menupath" id="BrowserWindowMenuObjectSave">Object &gt; Bewaar</p>
<p class="menuitem">Deze menukeuze opent een bewaardialoogvenster. Daarmee kan het object in originele vorm worden bewaard.</p>

<p class="menupath" id="BrowserWindowMenuObjectExport">Object &gt; Exporteer</p>
<p class="menuitem">Deze menukeuze biedt verschillende exporteermogelijkheden voor het object, zijnde:</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">&nbsp;Exporteer als</th></tr>
<tr><td><a href="#BrowserWindowMenuObjectExportSprite">&nbsp;&nbsp;<font color=green>Sprite</font></a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
</table>
<!-- Momenteel dus nog maar &eacute;&eacute;n mogelijkheid -->

<p class="menupath" id="BrowserWindowMenuObjectExportSprite">Object &gt; Exporteer &gt; <font color=green>Sprite</font></p>
<p class="menuitem">Deze menukeuze opent een bewaardialoogvenster. Daarmee kan het object als <font color=green>sprite</font> bewaard worden.</p>

<p class="menupath" id="BrowserWindowMenuObjectSaveLocation">Object &gt; Bewaar adres</p>
<p class="menuitem">Deze menukeuze biedt verschillende mogelijkheden om het adres van het object te bewaren, zijnde:</p>

<table class="menu">
<tr><th class="menutitle" colspan="2"><font color=green>Link</font></th></tr>
<tr><td><a href="#BrowserWindowMenuObjectSaveLocationAcorn">&nbsp;&nbsp;Acorn URI</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuObjectSaveLocationAnt">&nbsp;&nbsp;Ant URL</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuObjectSaveLocationText">&nbsp;&nbsp;Tekst</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuObjectSaveLocationAcorn">Object &gt; Bewaar adres &gt; Acorn URI</p>
<p class="menuitem">Deze menukeuze opent een bewaardialoogvenster. Daarmee kan het adres van het object, in Acorn URI <font color=red>[format]</font>, bewaard worden op diskette.</p>

<p class="menupath" id="BrowserWindowMenuObjectSaveLocationAnt">Object &gt; Bewaar adres &gt; Ant URL</p>
<p class="menuitem">Deze menukeuze opent een bewaardialoogvenster. Daarmee kan het adres van het object, in Acorn URL <font color=red>[format]</font>, bewaard worden op diskette.</p>

<p class="menupath" id="BrowserWindowMenuObjectSaveLocationText">Object &gt; Bewaar adres &gt; Tekst</p>
<p class="menuitem">Deze menukeuze opent een bewaardialoogvenster. Daarmee kan het adres van het object, in platte tekst, bewaard worden op diskette.</p>

<p class="menupath" id="BrowserWindowMenuObjectReload">Object &gt; Herlaad</p>
<p class="menuitem">Deze menukeuze herlaad de geopende pagina inclusief alle objecten an die pagina.</p>

<h4 id="BrowserWindowMenuNavigate" class="submenu">Navigatie-submenu</h4>

<p>Het navigatie-submenu is nuttig voor het navigeren in het net en biedt de volgende mogelijkheden:</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Navigatie</th></tr>
<tr><td><a href="#BrowserWindowMenuNavigateHome">&nbsp;&nbsp;Startpagina</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#BrowserWindowMenuNavigateBack">&nbsp;&nbsp;Vorige pagina</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#BrowserWindowMenuNavigateForward">&nbsp;&nbsp;Volgende pagina</a></td><td class="menuarrow"></td></tr>
<tr><td class="menusplit"><a href="#BrowserWindowMenuNavigateReload">&nbsp;&nbsp;Herlaad deze pagina</a></td><td></td></tr>
<tr><td><a href="#BrowserWindowMenuNavigateStop">&nbsp;&nbsp;Stop pagina ophalen</a></td><td class="menuarrow"></td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuNavigateHome">Navigate &gt; Startpagina</p>
<p class="menuitem">Deze menukeuze toont de NetSurf-startpagina. Dezelfde functie is te vinden op de <a href="#BrowserWindowToolbar">gereedschapbalk</a>.</p>

<p class="menupath" id="BrowserWindowMenuNavigateBack">Navigate &gt; Vorige pagina</p>
<p class="menuitem">Deze menukeuze toont de hiervoor bekeken pagina. <font color=red>[Any form information will not be resubmitted]</font>. Dezelfde functie is te vinden op de <a href="#BrowserWindowToolbar">gereedschapbalk</a>.</p>

<p class="menupath" id="BrowserWindowMenuNavigateForward">Navigate &gt; Volgende pagina</p>
<p class="menuitem">Deze menukeuze toont de hierna bekeken pagina. <font color=red>[Any form information will not be resubmitted]</font>. Dezelfde functie is te vinden op de <a href="#BrowserWindowToolbar">gereedschapbalk</a>.</p>

<p class="menupath" id="BrowserWindowMenuNavigateReload">Navigate &gt; Herlaad pagina</p>
<p class="menuitem"> Deze menukeuze toont de geopende pagina opnieuw. Elke eerdere versie in de <font color=red>[local cache]</font> wordt daardoor vervangen. Dezelfde functie is te vinden op de <a href="#BrowserWindowToolbar">gereedschapbalk</a>.</p>

<p class="menupath" id="BrowserWindowMenuNavigateStop">Navigate &gt; Stop pagina ophalen</p>
<p class="menuitem"> Deze menukeuze stopt het ophalen van een pagina onmiddelijk. Dezelfde functie is te vinden op de <a href="#BrowserWindowToolbar">gereedschapbalk</a>.</p>

<h4 id="BrowserWindowMenuDisplay" class="submenu">Display submenu</h4>

<p>Het Toon-submenu biedt de volgende mogelijkheden om <font color=red>[rendering]</font> een geopende pagina:</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Toon</th></tr>
<tr><td><a href="#BrowserWindowMenuDisplayScale">&nbsp;&nbsp;Schaal</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuDisplayImages">&nbsp;&nbsp;Afbeeldingen</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuDisplayToolbars">&nbsp;&nbsp;Gereedschapbalken</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuDisplayRender">&nbsp;&nbsp;<font color=red>[Render]</font></a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td class="menusplit"><a href="#BrowserWindowMenuDisplaySetDefault">&nbsp;&nbsp;Standaard</a></td><td></td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuDisplayScale">Display &gt; Schaal</p>
<p class="menuitem">Deze menukeuze biedt de mogelijkheid de geopende pagina te vergroten of te verkleinen. De schaal wordt aangegeven door een percentage waarbij 100% gelijk is aan de ongewijzigde schaalgrootte van de pagina. Dezelfde functie is te vinden op de <a href="#BrowserWindowToolbar">gereedschapbalk</a>.</p>

<p class="menupath" id="BrowserWindowMenuDisplayImages">Toon &gt; Afbeeldingen</p>
<p class="menuitem">Het Afbeeldingen<font color=green>[Images]</font>-submenu biedt de mogelijkheid om afbeeldingen op verschillende manieren te tonen. Deze mogelijkheden zijn alleen zichtbaar in het eigen bladervenster.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Afbeeldingen</th></tr>
<tr><td><a href="#BrowserWindowMenuDisplayImagesForeground">&nbsp;&nbsp;Voorgrondafbeeldingen</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#BrowserWindowMenuDisplayImagesBackground">&nbsp;&nbsp;Achtergrondafbeeldingen</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#BrowserWindowMenuDisplayImagesAnimations">&nbsp;&nbsp;Animaties</a></td><td class="menuarrow"></td></tr>
<tr><td class="menusplit"><a href="#BrowserWindowMenuDisplayImagesDither">&nbsp;&nbsp;<font color=red>[Dither]</font> images</a></td><td></td></tr>
<tr><td><a href="#BrowserWindowMenuDisplayImagesSmooth">&nbsp;&nbsp;<font color=red>[Smooth]</font> afbeeldingen</a></td><td class="menuarrow"></td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuDisplayImagesForeground">Toon &gt; Afbeeldingen &gt; Voorgrondafbeeldingen</p>
<p class="menuitem">Deze menukeuze biedt de mogelijkheid om al dan niet voorgrondafbeeldingen te tonen. Wanneer voorgrondafbeeldingen worden getoond staat er een &rdquo;vinkje&rdquo; rechts van deze menukeuze. Wanneer voorgrondafbeeldingen niet staan geactiveerd,<font color=red>[ any alternate text that the page provides will be shown instead]</font>.</p>

<p class="menupath" id="BrowserWindowMenuDisplayImagesBackground">Toon &gt; Afbeeldingen &gt; Achtergrondafbeeldingen</p>
<p class="menuitem">Deze menukeuze biedt de mogelijkheid om al dan niet achtergrondafbeeldingen te tonen. Wanneer Achtergrondafbeeldingen worden getoond staat er een &rdquo;vinkje&rdquo; rechts van deze menukeuze. Wanneer achtergrondafbeeldingen niet staan geactiveerd, zal de achtergrondkleur zoals gespecificeerd door het document, worden getoond.</p>

<p class="menupath" id="BrowserWindowMenuDisplayImagesAnimations">Toon &gt; Afbeeldingen &gt; Animaties</p>
<p class="menuitem">Deze mogelijkheid allows you to toggle the display of animations images. When animations are enabled, a tick appears to the left of the menu entry. If background images are disabled, the first frame of the animation will be shown as a static image.</p>

<p class="menupath" id="BrowserWindowMenuDisplayImagesDither">Toon &gt; Afbeeldingen &gt; <font color=red>[Dither]</font> afbeeldingen</p>
<p class="menuitem">This option allows you to toggle image dithering. When dithering is enabled, a tick appears to the left of the menu entry. Dithering gives an improved display in sub true colour screen modes.</p>

<p class="menupath" id="BrowserWindowMenuDisplayImagesSmooth">Toon &gt; Afbeeldingen &gt; <font color=red>[Smooth]</font> afbeeldingen</p>
<p class="menuitem">This option allows you to toggle image smoothing. When smoothing is enabled, a tick appears to the left of the menu entry. Image smoothing uses bi-linear filtering to give an improved display of scaled images.</p>

<p class="menupath" id="BrowserWindowMenuDisplayToolbars">Toon &gt; Gereedschapbalken</p>
<p class="menuitem">The Toolbars submenu allows you to control the way NetSurf's toolbars are displayed. The options available here only affect the local browser window.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">&nbsp;Gereedschapbalken</th></tr>
<tr><td><a href="#BrowserWindowMenuDisplayToolbarsButtons">&nbsp;&nbsp;Symbolen</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#BrowserWindowMenuDisplayToolbarsAddress">&nbsp;&nbsp;Adresbalk</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#BrowserWindowMenuDisplayToolbarsThrobber">&nbsp;&nbsp;Throbber</a></td><td class="menuarrow"></td></tr>
<tr><td class="menusplit"><a href="#BrowserWindowMenuDisplayToolbarsStatus">&nbsp;&nbsp;Status bar</a></td><td></td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuDisplayToolbarsButtons">Toon &gt; Gereedschapbalken &gt; Symbolen</p>
<p class="menuitem">This option toggles the display of the buttons on NetSurf's toolbar at the top of the window. When the buttons are turned off, the URL bar (if enabled) stretches to fill the extra space.</p>

<p class="menupath" id="BrowserWindowMenuDisplayToolbarsAddress">Toon &gt; Gereedschapbalken &gt; Adresbalk</p>
<p class="menuitem">This option toggles the display of the address (URL) bar on NetSurf's toolbar at the top of the window.</p>

<p class="menupath" id="BrowserWindowMenuDisplayToolbarsThrobber">Toon &gt; Gereedschapbalken &gt; Throbber</p>
<p class="menuitem">This option toggles the display of the throbber on NetSurf's toolbar at the top of the window. When the throbber is turned off, the URL bar (if enabled) stretches to fill the extra space.</p>

<p>If the buttons, URL bar and throbber are all disabled, the toolbar at the top of the window is removed, leaving more space for the main content area.</p>

<p class="menupath" id="BrowserWindowMenuDisplayToolbarsStatus">Toon &gt; Gereedschapbalken &gt; Statusbalk</p>
<p class="menuitem">This option toggles the display of the status bar at the bottom of the NetSurf window. When the status bar is turned off, the horizontal scroll bar stretches to the full width of the window.</p>

<p class="menupath" id="BrowserWindowMenuDisplayRender">Toon &gt; <font color=red>[Render]</font></p>
<p class="menuitem">The Render submenu allows you to control the way NetSurf draws content to the screen.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2"><font color=red>[Render]</font></th></tr>
<tr><td><a href="#BrowserWindowMenuDisplayRenderBlend">&nbsp;&nbsp;<font color=red>[Blend]</font> tekst naar achtergrond</a></td><td class="menuarrow"></td></tr>
<tr><td class="menusplit"><a href="#BrowserWindowMenuDisplayRenderBufferAnim">&nbsp;&nbsp;<font color=red>[Buffer]</font> animaties</a></td><td></td></tr>
<tr><td><a href="#BrowserWindowMenuDisplayRenderBufferAll">&nbsp;&nbsp;<font color=red>[Buffer all rendering]</font></a></td><td class="menuarrow"></td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuDisplayRenderBlend">Toon &gt; <font color=red>[Render]</font> &gt; <font color=red>[Blend]</font> tekst naar achtergrond</p>
<p class="menuitem">This option improves the edges of any text that is rendered over a background image. This facility is only available on RISC OS 3.7 or later.</p>

<p class="menupath" id="BrowserWindowMenuDisplayRenderBufferAnim">Toon &gt; <font color=red>[Render]</font> &gt; <font color=red>[Buffer]</font> animaties</p>
<p class="menuitem">This option buffers anything that NetSurf forces to be redrawn, other than redrawing a part of the window which has been obscured. Things like text areas, which are redrawn after typing, and animations are buffered. This has the effect of stopping these items from flickering.</p>

<p class="menupath" id="BrowserWindowMenuDisplayRenderBufferAll">Toon &gt; <font color=red>[Render]</font> &gt; <font color=red>[Buffer all rendering]</font></p>
<p class="menuitem">This option buffers everything, from the moment NetSurf starts drawing to the moment it finishes. Buffering all rendering is advantageous when pages consist of many boxes with different background colours on top of each other, as it only draws the page once, rather than building it up in layers. In addition, the display of pages which make heavy use of background images can be improved by enabling this option. Buffer all rendering is configured off by default as it slows down rendering, which makes scrolling pages much slower.</p>

<p class="menupath" id="BrowserWindowMenuDisplaySetDefault">Toon &gt; Standaard</p>
<p class="menuitem">This option makes NetSurf use the display options you have set up for the current window as the default options.</p>

<h4 id="BrowserWindowMenuUtilities" class="submenu"><font color=red>[Utilities]</font> Submenu</h4>

<p>The Utilities submenu contains access to some of NetSurf's extra features.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2"><font color=red>Utilities</font></th></tr>
<tr><td><a href="#BrowserWindowMenuUtilitiesHotlist">&nbsp;&nbsp;Favorieten</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuUtilitiesHistory">&nbsp;&nbsp;Historie</a></td><td class="menuarrow"></td><td></td></tr>
<tr><td><a href="#BrowserWindowMenuUtilitiesWindow">&nbsp;&nbsp;Venster</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#BrowserWindowMenuUtilitiesFindText">&nbsp;&nbsp;Zoek tekst</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuUtilitiesHotlist"><font color=red>[Utilities]</font> &gt; Favorieten</p>
<p class="menuitem">The Hotlist submenu allows you to manage the hotlist.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Favorieten</th></tr>
<tr><td><a href="#BrowserWindowMenuUtilitiesHotlistAdd">&nbsp;&nbsp;Bij favorieten</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#BrowserWindowMenuUtilitiesHotlistShow">&nbsp;&nbsp;Toon favorieten</a></td><td class="menuarrow"></td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuUtilitiesHotlistAdd"><font color=red>[Utilities]</font> &gt; Favorieten &gt; Bij favorieten</p>
<p class="menuitem">This option adds the current page to the bottom of your hotlist. The same function is available on the <a href="#BrowserWindowToolbar">gereedschapbalk</a>.</p>

<p class="menupath" id="BrowserWindowMenuUtilitiesHotlistShow"><font color=red>[Utilities]</font> &gt; Favorieten &gt; Toon favorieten</p>
<p class="menuitem">This option opens the <a href="#HotlistWindow">hotlist management window</a>. The same function is available on the <a href="#BrowserWindowToolbar">gereedschapbalk</a>.</p>

<p class="menupath" id="BrowserWindowMenuUtilitiesHistory"><font color=red>[Utilities]</font> &gt; Historie</p>
<p class="menuitem">This option opens the <a href="#HistoryWindow">local history window</a>. The same function is available on the <a href="#BrowserWindowToolbar">toolbar</a>.</p>

<p class="menupath" id="BrowserWindowMenuUtilitiesWindow"><font color=red>[Utilities]</font> &gt; Venster</p>
<p class="menuitem">The Window submenu allows you to control the size and positioning of new windows.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Venster</th></tr>
<tr><td><a href="#BrowserWindowMenuUtilitiesWindowSetdefault">&nbsp;&nbsp;Standaard</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#BrowserWindowMenuUtilitiesWindowStagger">&nbsp;&nbsp;<font color=red>[Stagger]</font> vensterpositie</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#BrowserWindowMenuUtilitiesWindowMaintainsize">&nbsp;&nbsp;<font color=red>[Maintain]</font> venstermaat</a></td><td class="menuarrow"></td></tr>
<tr><td class="menusplit"><a href="#BrowserWindowMenuUtilitiesWindowResetdefault">&nbsp;&nbsp;Herstel standaardpositie</a></td><td></td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuUtilitiesWindowSetdefault">[Utilities] &gt; Venster &gt; Standaard</p>
<p class="menuitem">This option sets the position of the current window to be the default position for new windows.</p>

<p class="menupath" id="BrowserWindowMenuUtilitiesWindowStagger"><font color=red>[Utilities]</font> &gt; Venster &gt; <font color=red>[Stagger]</font> vensterpositie</p>
<p class="menuitem">This option toggles staggered window opening positions for new windows. When turned on, this option means that new windows will open with a small offset from the position of the previously opened window. This option is only available when a user defined default window position has been set.</p>

<p class="menupath" id="BrowserWindowMenuUtilitiesWindowMaintainsize">[Utilities] &gt; Venster &gt; [Maintain] venstermaat</p>
<p class="menuitem">This option toggles the maintain window size feature. When the feature is turned off, NetSurf behaves in the same manner as other RISC OS browsers - newly opened windows all open at the same size in the default position. With this feature enabled, browser windows opened from other browser windows (eg. by adjust clicking on a link), will inherit the size of the first window and open above the first window, rather than at the default position.</p>

<p class="menupath" id="BrowserWindowMenuUtilitiesWindowResetdefault"><font color=red>[Utilities]</font> &gt; Venster &gt; Herstel standaardpositie</p>
<p class="menuitem">This option removes any user defined default window position. The option is only available when a user defined default window position has been set.</p>

<p class="menupath" id="BrowserWindowMenuUtilitiesFindText">[Utilities] &gt; Zoek tekst</p>
<p class="menuitem">This option allows you to search the current page for a text string. The same function is available on the <a href="#BrowserWindowToolbar">toolbar</a>.</p>

<h4 id="BrowserWindowMenuHelp" class="submenu">Hulp-submenu</h4>

<p>The Help submenu contains options for fast access to help and information while you're using NetSurf.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Hulp</th></tr>
<tr><td><a href="#BrowserWindowMenuHelpContents">&nbsp;&nbsp;Inhoud</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#BrowserWindowMenuHelpGuide">&nbsp;&nbsp;Handleiding</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#BrowserWindowMenuHelpInfo">&nbsp;&nbsp;Gebruikersinformatie</a></td><td class="menuarrow"></td></tr>
<tr><td class="menusplit"><a href="#BrowserWindowMenuHelpAbout">&nbsp;&nbsp;Over NetSurf</a></td><td></td></tr>
<tr><td class="menusplit"><a href="#BrowserWindowMenuHelpInteractive">&nbsp;&nbsp;Interactieve hulp</a></td><td></td></tr>
</table>

<p class="menupath" id="BrowserWindowMenuHelpContents">Hulp &gt; Inhoud</p>
<p class="menuitem">This option opens the <a href="docs.en">documentation contents page</a> in a new window.</p>

<p class="menupath" id="BrowserWindowMenuHelpGuide">Hulp &gt; Handleiding</p>
<p class="menuitem">This option opens this page in a new window!</p>

<p class="menupath" id="BrowserWindowMenuHelpInfo">Hulp &gt; Gebruikersinformatie</p>
<p class="menuitem">This option opens the <a href="info.en">user information page</a> in a new window.</p>

<p class="menupath" id="BrowserWindowMenuHelpAbout">Hulp &gt; Over NetSurf</p>
<p class="menuitem">This opens NetSurf's about page in a new window. The about page contains a list of people who have contributed to the NetSurf project and a credits for the libraries NetSurf uses.</p>

<p class="menupath" id="BrowserWindowMenuHelpInteractive">Hulp &gt; Interactieve hulp</p>
<p class="menuitem">This option loads Help, if it hasn't already been loaded, to display interactive help messages.</p>

<h2 id="BrowserWindowFileTypes">Bestandtypen</h2>

<p>Wanneer NetSurf een bestandtype tegenkomt dat het niet kent, wordt een <a href="#DownloadWindow">laadvenster</a> geopend dat het mogelijk maakt om het bestand op de <font color=red>[disc]</font> te bewaren. De bestandtypen die NetSurf kent, staan hierna met het RISC OS-bestandtype en MIME type(n).</p>

<table>
<tr><th>Naam</th><th>Type</th><th>MIME type(n)</th></tr>
<tr><td>CSS</td><td>&amp;f79</td><td>text/css</td></tr>
<tr><td>Drawfile</td><td>&amp;aff</td><td>application/drawfile<br>application/x-drawfile<br>image/drawfile<br>image/x-drawfile</td></tr>
<tr><td>Flash</td><td>&amp;188</td><td>application/x-shockwave-flash</td></tr>
<tr><td>GIF</td><td>&amp;695</td><td>image/gif</td></tr>
<tr><td>HTML</td><td>&amp;faf</td><td>text/html</td></tr>
<tr><td>JNG</td><td>&amp;f78</td><td>image/jng<br>image/x-jng</td></tr>
<tr><td>JPEG</td><td>&amp;c85</td><td>image/jpeg<br>image/pjpeg</td></tr>
<tr><td>MNG</td><td>&amp;f83</td><td>image/mng<br>image/x-mng<br>video/mng<br>video/x-mng</td></tr>
<tr><td>PNG</td><td>&amp;b60</td><td>image/png</td></tr>
<tr><td>Sprite</td><td>&amp;ff9</td><td>image/x-riscos-sprite</td></tr>
<tr><td>Text</td><td>&amp;fff</td><td>text/plain</td></tr>
</table>

<p>Flash-bestanden <font color=red>[are not handled by NetSurf directly and are displayed by an]</font> <a href="info.en#GettingStartedRequirements">external plugin</a>.</p>

<p>NetSurf recognises different file types by their RISC OS file type for local files and by their content-type header for files sourced from the internet. Files are not identified by their extension or content. This means that for a file to be displayed, servers must be configured to send the correct content-type header and local files must have the correct RISC OS file type set.</p>

<h2 id="DownloadWindow">Ophaalvenster</h2>

<p>Whenever you follow a link to a file that NetSurf doesn't know how to handle, NetSurf opens the download window. This allows you to save a local copy of the file. The download window is shown below.</p>

<p class="screenshot"><img src="images/dwnld.en" title="Download Window" alt=""></p>

<p>As soon as the download window opens, NetSurf starts downloading the file to your Scrap directory. At any time, either as the file downloads or after it has finished, it is possible drag the file icon at the top of the window to the directory where you wish to keep the file. This action causes the file to be moved to your desired location from Scrap and if the download is incomplete, the remainder of the file will also go to the new location.</p>

<p>The Source field indicates the location of the file being downloaded.</p>

<p>Initially, the Destination field allows the local filename of the file being downloaded to be set. Once the local destination of the file has been set, by dragging the icon to a filer window, the local path is shown here. Clicking on it will open the associated directory.</p>

<p>The bottom part of the download window displays information about the download's progress. On the left, the amount already downloaded and the total size are displayed. In the middle, the download speed is indicated and that the right, and estimated time remaining is shown. A beige progress bar indicates the download's progress visually.</p>

<h2 id="History">History</h2>

<p>While browsing the World Wide Web, you may come across interesting pages and some time later, you may wish to return to them. NetSurf's history stores the addresses (URLs) of the web pages you visit so that you can return to a page without having to remember the URL or how you first came across it.</p>

<p>NetSurf supports two distinct types of history; local and global. Local history contains only the pages that are visited in any particular window. Global history contains all the pages that have been visited in NetSurf and is remembered when NetSurf is restarted.</p>

<h3 id="HistoryLocal">Local History Window</h3>

<p>NetSurf's local history window provides a diagrammatical overview of the browsing history of a particular window. Each web page that is visited is represented by a thumbnail in a tree diagram. Beneath each thumbnail the page title is shown. Clicking on a thumbnail will cause the NetSurf browser window to return to that particular page.</p>

<h3 id="HistoryGlobal">Global History Window</h3>

<p>NetSurf's global history stores the addresses (URLs) of all the web pages you visit. To open the global history window, you can adjust click on the history toolbar button on a <a href="#BrowserWindow">browser window</a>, use the browser window <a href="#BrowserWindowMenuUtilitiesHistoryGlobal">menu option</a> or use the <a href="#IconbarMenu">iconbar menu</a>. The global history is made up from &quot;addresses&quot; and &quot;directories&quot;.</p>

<p>An address consists of a name and a URL. Addresses in the global history can point to any kind of file and double clicking on one will launch the URL in NetSurf. Addresses also store the date the URL was visited. By default, addresses are listed in the order they were visited and they are grouped by date within directories.</p>

<p>Directories store groups of addresses. In the default global history view, there are directories for each day up to the beginning of last week and further directories for previous weeks. The global history is expired after a maximum of four weeks.</p>

<p>The global history display works in a similar way to a filer window, although there are some important differences. Directories are not opened in new windows. Instead, the whole of the history is displayed in one window and directories represented by branches in a tree diagram. Double clicking on a closed directory will open it, displaying its contents as a new branch. Double clicking on an open directory will close it. When addresses are double clicked on, their URL is launched by NetSurf.</p>

<p>Next to the address and directory icons there are toggle boxes, which can be used to expand and collapse branches in the history tree. Clicking on the branch toggle boxes next to directories has exactly the same effect as double clicking on a directory. Expanding an address with the branch toggle box displays the other information it contains. (The URL and the time it was visited.) The space bar can also be used to expand or collapse selected addresses.</p>

<p>As with filer windows, selections can be made by dragging selection boxes and adjust clicking can be used to add or remove items from the selection.</p>

<h4 id="HistoryGlobalToolbar">Toolbar</h4>

<p>The global history toolbar allows quick access to various commonly used features. More functions are available through the <a href="#HistoryGlobalMenus">menus</a>. The toolbar can be customized to contain only the icons you want, in the order you want, in toolbar edit mode. This is covered in the <a href="#ConfigurationToolbar">toolbar customization</a> section. Available icons are listed below.</p>

<p class="iconinfo"><strong><img src="images/delete" class="icon" alt="Delete Selection" title="Delete Selection Button"></strong> Clicking on this button deletes any selected directories or addresses from the global history.</p>

<p class="iconinfo"><strong><img src="images/expand" class="icon" alt="Expand Addresses" title="Expand Addresses Button"></strong> Select clicking on this button expands any addresses that are being shown, to display additional information about each one. Adjust clicking hides the additional information for any expanded addresses.</p>

<p class="iconinfo"><strong><img src="images/launch" class="icon" alt="Launch Selection" title="Launch Selection Button"></strong> Clicking on this button launches all selected addresses in separate <a href="#BrowserWindow">browser windows</a>. If a directory is selected any addresses it contains will be launched.</p>

<p class="iconinfo"><strong><img src="images/open" class="icon" alt="Open Directories" title="Open Directories Button"></strong> Select clicking on this button opens every directory in the global history. Adjust clicking closes any open directories.</p>

<h4 id="HistoryGlobalMenus">Menus</h4>

<p>The global history menu contains options which allow you to manage the content of your global history. The options available are:</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">History</th></tr>
<tr><td><a href="#HistoryGlobalMenuHistory">History</a></td><td class="menuarrow">&gt;</td></tr>
<tr><td><a href="#HistoryGlobalMenuSelection">Selection</a></td><td class="menuarrow">&gt;</td></tr>
<tr><td><a href="#HistoryGlobalMenuSelectAll">Select all</a></td><td></td></tr>
<tr><td><a href="#HistoryGlobalMenuClearSelection">Clear selection</a></td><td></td></tr>
</table>

<h5 id="HistoryGlobalMenuHistory" class="submenu">History Submenu</h5>

<p>The Hotlist submenu contains the following options which apply to the hotlist in general.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">History</th></tr>
<tr><td><a href="#HistoryGlobalMenuHistoryExport">Export</a></td><td class="menuarrow">&gt;</td></tr>
<tr><td class="menusplit"><a href="#HistoryGlobalMenuHistoryExpand">Expand</a></td><td class="menuarrow">&gt;</td></tr>
<tr><td><a href="#HistoryGlobalMenuHistoryCollapse">Collapse</a></td><td class="menuarrow">&gt;</td></tr>
<tr><td><a href="#HistoryGlobalMenuHistoryToolbars">Toolbars</a></td><td class="menuarrow">&gt;</td></tr>
</table>

<p class="menupath" id="HistoryGlobalMenuHistoryExport">Hotlist &gt; Export</p>
<p class="menuitem">This option allows you to export the global history as an HTML file. The directory structure is represented by nested unordered lists and addresses are converted to clickable links.</p>

<p class="menupath" id="HistoryGlobalMenuHistoryExpand">Hotlist &gt; Expand</p>
<p class="menuitem">This option allows various parts of the global history to be shown.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Expand</th></tr>
<tr><td><a href="#HistoryGlobalMenuHistoryExpandAll">All</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#HistoryGlobalMenuHistoryExpandDirectories">Directories</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#HistoryGlobalMenuHistoryEdpandAddresses">Addresses</a></td><td class="menuarrow"></td></tr>
</table>

<p class="menupath" id="HistoryGlobalMenuHistoryExpandAll">History &gt; Expand &gt; All</p>
<p class="menuitem">This option opens all directories and expands all addresses in the global history, making the entire directory structure and all address information visible.</p>

<p class="menupath" id="HistoryGlobalMenuHistoryExpandDirectories">History &gt; Expand &gt; Directories</p>
<p class="menuitem">This option opens all of the directories in the global history, making the entire directory structure visible.</p>

<p class="menupath" id="HistoryGlobalMenuHistoryExpandAddresses">History &gt; Expand &gt; Addresses</p>
<p class="menuitem">This option expands all of the addresses which are presently being displayed in the global history. Their URLs and additional usage information are made visible.</p>

<p class="menupath" id="HistoryGlobalMenuHistoryCollapse">History &gt; Collapse</p>
<p class="menuitem">This option allows various parts of the global history to be hidden.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Collapse</th></tr>
<tr><td><a href="#HistoryGlobalMenuHistoryCollapseAll">All</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#HistoryGlobalMenuHistoryCollapseDirectories">Directories</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#HistoryGlobalMenuHistoryCollapseAddresses">Addresses</a></td><td class="menuarrow"></td></tr>
</table>

<p class="menupath" id="HistoryGlobalMenuHistoryCollapseAll">History &gt; Collapse &gt; All</p>
<p class="menuitem">This option closes all directories and collapses all addresses in the global history, reducing the display to the contents of the root directory.</p>

<p class="menupath" id="HistoryGlobalMenuHistoryCollapseDirectories">History &gt; Collapse &gt; Directories</p>
<p class="menuitem">This option closes all of the directories in the global history, hiding the directory structure.</p>

<p class="menupath" id="HistoryGlobalMenuHistoryCollapseAddresses">History &gt; Collapse &gt; Addresses</p>
<p class="menuitem">This option reduces all of the displayed addresses to just their name, hiding their URLs and additional usage information.</p>

<p class="menupath" id="HistoryGlobalMenuHistoryToolbars">Display &gt; Toolbars</p>
<p class="menuitem">The Toolbars submenu allows you to control the way NetSurf's global history toolbar is displayed.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Toolbars</th></tr>
<tr><td><a href="#HistoryGlobalMenuHistoryToolbarsButtons">Buttons</a></td><td class="menuarrow"></td></tr>
</table>

<p class="menupath" id="HistoryGlobalMenuHistoryToolbarsButtons">Display &gt; Toolbars &gt; Buttons</p>
<p class="menuitem">This option toggles the display of the buttons on NetSurf's global history toolbar at the top of the window.</p>

<h5 id="HistoryGlobalMenuSelection" class="submenu">Selection Submenu</h5>

<p>The selection submenu, which is only available when a selection has been made in the global history window, contains the following options.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Selection</th></tr>
<tr><td><a href="#HistoryGlobalMenuSelectionLaunch">Launch</a></td><td></td></tr>
<tr><td><a href="#HistoryGlobalMenuSelectionDelete">Delete</a></td><td></td></tr>
</table>

<p class="menupath" id="HistoryGlobalMenuSelectionLaunch">Selection &gt; Launch</p>
<p class="menuitem">This option allows you to launch all of the selected addresses in separate NetSurf browser windows. This option is also available on the global history window <a href="#HistoryGlobalToolbar">toolbar</a>.</p>

<p class="menupath" id="HistoryGlobalMenuSelectionDelete">Selection &gt; Delete</p>
<p class="menuitem">This option deletes the current selection from the global history. This option is also available on the global history window <a href="#HistoryGlobalToolbar">toolbar</a>.</p>

<p class="menupath" id="HistoryGlobalMenuSelectAll">Select all</p>
<p class="menuitem">This option allows you to select all the addresses and directories in the global history window.</p>

<p class="menupath" id="HistoryGlobalMenuClearSelection">Clear selection</p>
<p class="menuitem">This option allows you to deselect any selected items.</p>

<h2 id="Hotlist">Favorietenvenster</h2>

<p>While browsing the World Wide Web, you may come across pages that you wish to return to. NetSurf's hotlist provides a convenient way of storing these addresses (URLs) so that you can return to the page without having to remember the URL or how you first came across the page. Hotlists are made up from &quot;entries&quot; and &quot;directories&quot;. There can be as many of each of these as you like.</p>

<p>An entry consists of a name and a URL. Hotlist entries can point to any kind of file and double clicking on one will launch the URL in NetSurf. The name can be anything you want and, for web pages, is set to the page title by default. Entries also store other usage information, such as the date the entry was added to the hotlist, when it was last visited and how many times it has been visited in total.</p>

<p>Directories are used to group related entries to allow for easy location of the entry you want. They can be nested and contain a mixture of entries and other directories.</p>

<p>To open the hotlist management window, you can adjust click on NetSurf's iconbar icon or select click on the Hotlist toolbar button on a <a href="#BrowserWindow">browser window</a>. The area at the top of the hotlist management window is the hotlist toolbar and the main section below that is the hotlist display.</p>

<p>The hotlist display works in a similar way to a filer window, although there are some important differences. Directories are not opened in new windows. Instead, the whole hotlist is displayed in one window and directories represented by branches in a tree diagram. Double clicking on a closed directory will open it, displaying its contents as a new branch. Double clicking on an open directory will close it. When entries are double clicked on, their URL is launched by NetSurf. Next to the entry and directory icons there are toggle boxes, which can be used to expand and collapse branches in the hotlist tree. Clicking on the branch toggle boxes next to directories has exactly the same effect as double clicking on a directory. Expanding an entry with the branch toggle box displays the other information it contains, such as the URL and it's usage statistics. The space bar can also be used to expand or collapse selected entries.</p>

<p>As with filer windows, selections can be made by dragging selection boxes and adjust clicking can be used to add or remove items from the selection. Selections can then be dragged and dropped within the hotlist management window to move them around and you can drop a selection over a closed directory by releasing it there.</p>

<p>NetSurf's hotlist is stored as an HTML file which can be found in <em>Choices:WWW.NetSurf</em>. This means the hotlist file can be loaded into a NetSurf browser window and used as a links page or set to your default home page. The hotlist file is saved when you quit NetSurf and a save can be forced from the hotlist management window <a href="#HotlistMenuHotlistSave">menu</a>.</p>

<h3 id="HotlistToolbar">Gereedschapbalk</h3>

<p>Nog niet beschreven.</p>

<h3 id="HotlistMenus">Menu's</h3>

<p>The hotlist menu contains options which allow you to manage the content of your hotlist. The options available are:</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Favorieten</th></tr>
<tr><td><a href="#HotlistMenuHotlist">&nbsp;&nbsp;Favorieten</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#HotlistMenuSelection">&nbsp;&nbsp;Selectie</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#HotlistMenuSelectAll">&nbsp;&nbsp;Selecteer alles</a></td><td></td></tr>
<tr><td><a href="#HotlistMenuClearSelection">&nbsp;&nbsp;Wis<font color=red>(?)</font>selectie</a></td><td></td></tr>
</table>

<h4 id="HotlistMenuHotlist" class="submenu">Favoriete-submenu</h4>

<p>The Hotlist submenu contains the following options which apply to the hotlist in general.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Favorieten</th></tr>
<tr><td><a href="#HotlistMenuHotlistNew">&nbsp;&nbsp;Nieuw</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#HotlistMenuHotlistSave">&nbsp;&nbsp;Bewaar</a></td><td></td></tr>
<tr><td><a href="#HotlistMenuHotlistExport">&nbsp;&nbsp;Exporteer</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td class="menusplit"><a href="#HotlistMenuHotlistExpand">&nbsp;&nbsp;Vergroot</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#HotlistMenuHotlistCollapse">&nbsp;&nbsp;Verbergen</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
</table>

<p class="menupath" id="HotlistMenuHotlistNew">Favorieten &gt; Nieuw</p>
<p class="menuitem">This option allows the creation of new entries or directories.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Nieuw</th></tr>
<tr><td><a href="#HotlistMenuHotlistNewDirectory">&nbsp;&nbsp;Directorie</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#HotlistMenuHotlistNewEntry">&nbsp;&nbsp;Toegang</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
</table>

<p class="menupath" id="HotlistMenuHotlistNewDirectory">Favoriete &gt; Nieuw &gt; Directorie</p>
<p class="menuitem">This option allows you to create a new directory, which is added to the bottom of the hotlist. The name of the directory can be set and once created, it can be dragged to a different location in the hotlist and <a href="#HotlistMenuSelectionEdit">renamed</a>. Directories can be nested and there is no limit to the number of sub directories. This option is also available on the hotlist management window <a href="#HotlistToolbar">toolbar</a>.</p>

<p class="menupath" id="HotlistMenuHotlistNewEntry">Favorieten &gt; Nieuw &gt; Toegang</p>
<p class="menuitem">This option allows you to create a new entry. Its name and URL must be set before it is added to the bottom of the hotlist. The entry can later be moved around in the hotlist and <a href="#HotlistMenuSelectionEdit">edited</a>. This option is also available on the hotlist management window <a href="#HotlistToolbar">toolbar</a>.</p>

<p class="menupath" id="HotlistMenuHotlistSave">Favorieten &gt; Bewaar</p>
<p class="menuitem">This option allows you to save the hotlist. The hotlist is also saved automatically when NetSurf is <a href="#IconbarMenuQuit">quitted</a>.</p>

<p class="menupath" id="HotlistMenuHotlistExport">Favorieten &gt; Exporteer</p>
<p class="menuitem">This option allows you to export the hotlist as an HTML file. The directory structure is represented by nested unordered lists and entries are converted to clickable links.</p>

<p class="menupath" id="HotlistMenuHotlistExpand">Favorieten &gt; Vergroot</p>
<p class="menuitem">This option allows various parts of the hotlist to be shown.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Vergroot</th></tr>
<tr><td><a href="#HotlistMenuHotlistExpandAll">&nbsp;&nbsp;Alles</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#HotlistMenuHotlistExpandDirectories">&nbsp;&nbsp;Directories</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#HotlistMenuHotlistEdpandEntries">&nbsp;&nbsp;Toegangen</a></td><td class="menuarrow"></td></tr>
</table>

<p class="menupath" id="HotlistMenuHotlistExpandAll">Favorieten &gt; Vergroot &gt; Alles</p>
<p class="menuitem">This option opens all directories and expands all entries in the hotlist, making the entire directory structure and all entry information visible.</p>

<p class="menupath" id="HotlistMenuHotlistExpandDirectories">Favorieten &gt; Vergroot &gt; Directories</p>
<p class="menuitem">This option opens all of the directories in the hotlist, making the entire directory structure visible.</p>

<p class="menupath" id="HotlistMenuHotlistExpandEntries">Favorieten &gt; Vergroot &gt; Toegangen</p>
<p class="menuitem">This option expands all of the entries which are presently being displayed in the hotlist. Their URLs and additional usage information are made visible.</p>

<p class="menupath" id="HotlistMenuHotlistCollapse">Favorieten &gt; Verbergen</p>
<p class="menuitem">This option allows various parts of the hotlist to be hidden.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Verbergen</th></tr>
<tr><td><a href="#HotlistMenuHotlistCollapseAll">&nbsp;&nbsp;Alles</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#HotlistMenuHotlistCollapseDirectories">&nbsp;&nbsp;Directories</a></td><td class="menuarrow"></td></tr>
<tr><td><a href="#HotlistMenuHotlistCollapseEntries">&nbsp;&nbsp;Toegangen</a></td><td class="menuarrow"></td></tr>
</table>

<p class="menupath" id="HotlistMenuHotlistCollapseAll">Favorieten &gt; Verbergen &gt; Alles</p>
<p class="menuitem">This option closes all directories and collapses all entries in the hotlist, reducing the display to the contents of the hotlist root directory.</p>

<p class="menupath" id="HotlistMenuHotlistCollapseDirectories">Favorieten &gt; Verbergen &gt; Directories</p>
<p class="menuitem">This option closes all of the directories in the hotlist, hiding the directory structure.</p>

<p class="menupath" id="HotlistMenuHotlistCollapseEntries">Favorieten &gt; Verbergen &gt; Toegangen</p>
<p class="menuitem">This option reduces all of the hotlist entries to just their name, hiding their URLs and additional usage information.</p>

<h4 id="HotlistMenuSelection" class="submenu">Selectie-submenu</h4>

<p>The selection submenu, which is only available when a selection has been made in the hotlist management window, contains the following options.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Selectie</th></tr>
<tr><td><a href="#HotlistMenuSelectionSave">&nbsp;&nbsp;Bewaar</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#HotlistMenuSelectionEdit">&nbsp;&nbsp;<font color=red>[Edit]</font></a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#HotlistMenuSelectionLaunch">&nbsp;&nbsp;<font color=red>[Launch]</font></a></td><td></td></tr>
<tr><td><a href="#HotlistMenuSelectionDelete">&nbsp;&nbsp;Wis</a></td><td></td></tr>
<tr><td><a href="#HotlistMenuSelectionResetStatistics">&nbsp;&nbsp;Herstel <font color=red>[statistics]</font></a></td><td></td></tr>
</table>

<p class="menupath" id="HotlistMenuSelectionSave">Selectie &gt; Bewaar</p>
<p class="menuitem">This option allows you to save the current selection. Different options are available depending on what has been selected.</p>

<table class="menu">
<tr><th class="menutitle" colspan="2">Bewaar</th></tr>
<tr><td><a href="#HotlistMenuSelectionSaveAcorn">&nbsp;&nbsp;Acorn URI</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#HotlistMenuSelectionSaveAnt">&nbsp;&nbsp;Ant URL</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
<tr><td><a href="#HotlistMenuSelectionSaveHTML">&nbsp;&nbsp;HTML</a></td><td class="menuarrow">&gt;&nbsp;</td></tr>
</table>

<p class="menupath" id="HotlistMenuSelectionSaveAcorn">Selectie &gt; Bewaar &gt; Acorn URI</p>
<p class="menuitem">This option allows you to save the address of the selected entry in Acorn URI format. This option is only available when one entry is selected.</p>

<p class="menupath" id="HotlistMenuSelectionSaveAnt">Selectie &gt; Bewaar &gt; Ant URL</p>
<p class="menuitem">This option allows you to save the address of the selected entry in Ant URL format. This option is only available when one entry is selected.</p>

<p class="menupath" id="HotlistMenuSelectionSaveHTML">Selectie &gt; Bewaar &gt; HTML</p>
<p class="menuitem">This option allows you to save the current selection in HTML format. Any directories are represented by nested unordered lists while entries are converted to clickable links.</p>

<p class="menupath" id="HotlistMenuSelectionEdit">Selectie &gt; <font color=red>[Edit]</font></p>
<p class="menuitem">This option allows you to edit the currently selected item and is only available when either one directory or one item is selected. The names of directories can entries can be altered. Entries' URLs can also be changed.</p>

<p>Entries added to the hotlist from a NetSurf browser window have a default name set. For HTML files, this is the page title.</p>

<p class="menupath" id="HotlistMenuSelectionLaunch">Selectie &gt; <font color=red>[Launch]</font></p>
<p class="menuitem">This option allows you to launch all of the selected entries in separate NetSurf browser windows. This option is also available on the hotlist management window <a href="#HotlistToolbar">toolbar</a>.</p>

<p class="menupath" id="HotlistMenuSelectionDelete">Selectie &gt; Wis</p>
<p class="menuitem">This option deletes the current selection from the hotlist. This option is also available on the hotlist management window <a href="#HotlistToolbar">toolbar</a>.</p>

<p class="menupath" id="HotlistMenuSelectionResetStatistics">Selectie &gt; Herstel <font color=red>[statistics]</font></p>
<p class="menuitem">This option resets the usage information for all selected entries. The hotlist's last visited record and total visit counter are removed. The URL and the date the entry was added to the hotlist are left untouched.</p>

<p class="menupath" id="HotlistMenuSelectAll">Selecteer alles</p>
<p class="menuitem">This option allows you to select all the entries and directories in the hotlist.</p>

<p class="menupath" id="HotlistMenuClearSelection"><font color=red>[Clear]</font> selectie</p>
<p class="menuitem">This option allows you to deselect any selected items.</p>

<h2 id="Configuration">Instellen NetSurf</h2>

<p>Not written yet. NetSurf's configuration system and options are due for an update soon.</p>

<h2 id="Keys">Toets(enbord)combinaties</h2>

<p>NetSurf supports keyboard shortcuts, which allow quick access to features and make NetSurf easier to use. The supported keypresses and corresponding functions are listed below.</p>

<h3>Inhoud</h3>
<!-- De kolommen hierna zijn verwisseld omdat iemand meestal niet op zoek is naar welke toetsencombinatie een bepaalde functie heeft. Meestal zal het zo zijn dat iemand op zoek is naar een bepaalde functie en vervolgens pas wil weten welke toetsencombinatie daarvoor gebruikt kan worden. -->
<table>
<tr><th>Functie</th><th>Toetscombinatie</th></tr>
<tr><td>Pagina openen in een nieuw venster</td><td>Ctrl+N</td></tr>
<tr><td>Pagina-broncode tonen in tekst<font color=red>[editor]</font></td><td>F8</td></tr>
<tr><td>Pagina-broncode bewaren</td><td>F3</td></tr>
<tr><td>Pagina bewaren als tekst</td><td>Ctrl+F3</td></tr>
<tr><td>Pagina bewaren als Drawbestand</td><td>Ctrl+Shift+F3</td></tr>
<tr><td>Netstek, inclusief afbeeldingen e.d., bewaren</td><td>Shift+F3</td></tr>
</table>

<h3>Toon</h3>

<table>
<tr><th>Functie</th><th>Toetscombinatie</th></tr>
<tr><td>Pagina omlaag</td><td>Page Up</td></tr>
<tr><td>Pagina omhoog</td><td>Page Down</td></tr>
<tr><td>Regel omlaag</td><td>Cursor Down</td></tr>
<tr><td>Pagina-onderkant</td><td>Ctrl+Cursor Down</td></tr>
<tr><td>Regel omhoog</td><td>Cursor Up</td></tr>
<tr><td>Pagina-bovenkant</td><td>Ctrl+Cursor Up</td></tr>
<tr><td>Schaalvenster openen</td><td>F11</td></tr>
<tr><td>Schaal verkleinen met 10%</td><td>Ctrl+Q</td></tr>
<tr><td>Schaal vergroten met 10%</td><td>Ctrl+W</td></tr>
<tr><td>Randen tonen. Dit kan handig zijn bij het maken van een netstek. De inhoud heeft een <font color=red>[Cyan]</font> rand, de <font color=red>[padding]</font> heeft een <font color=red>[magenta]</font> rand en de marge heeft een gele rand.</td><td>Shift+F11</td></tr>
</table>

<h3>Navigatie</h3>

<table>
<tr><th>Functie</th><th>Toetscombinatie</th></tr>
<tr><td>Pagina herladen</td><td>F5, Ctrl+R</td></tr>
<tr><td>Paginaladen stoppen</td><td>Esc</td></tr>
</table>

<h3>Algemeen</h3>

<table>
<tr><td><a href="docs.nl">Pagina-inhouddocumentatie</a> openen in een nieuw venster</td><td>F1</td></tr>
<tr><td>Move the caret to URL bar and clear it to &quot;www.&quot;</td><td>F2</td></tr>
<tr><td>Tekstzoekvenster openen</td><td>F4</td></tr>
<tr><td>Favorietenvenster openen</td><td>F6</td></tr>
<tr><td>Venster sluiten</td><td>Ctrl+F2</td></tr>
</table>

<div class="footer">
<p class="breadcrumbs"><a href="/">NetSurf</a> » <a href="docs">Documentatie</a> » Handleiding</p>
</div>

</body>
</html>
