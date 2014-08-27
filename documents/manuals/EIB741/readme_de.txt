DR. JOHANNES HEIDENHAIN GmbH
Datei: Readme_de.txt für EIB 741/742 driver CD
Datum: 30.09.2013



EIB 741/742 Software
================

1. Inhalt
---------

/EIB_741_742
   /doc
      /de                Dokumentation und Lizenzbedingungen in Deutsch
      /en                Dokumentation und Lizenzbedingungen in Englisch

   /labview
      /examples          Beispiele
      /subvi             allgemeine VIs
      /typedefs          Typ-Definitionen für die VIs
      /VIs               LabView VIs für die EIB 741/742
      eib7_top.mnu       Funktionspalette aller VIs für die EIB 741/742

   /linux
      /examples          Beispiele
         /c_cpp          Beispiele für Programmiersprache C und C++
      /include           Headerdatei für die Funktionen der EIB 741/742
      /lib               Funktions-Bibliothek für die EIB 741/742 (Linux x32)
      /lib64             Funktions-Bibliothek für die EIB 741/742 (Linux x64)
      /tools             Hilfsprogramme für die EIB 741/742

   /windows
      /bin               DLL für die EIB 741/742 (Windows x32)
      /bin64             DLL für die EIB 741/742 (Windows x64)
      /examples          Beispiele
         /c_cpp          Beispiele für Programmiersprache C und C++
      /include           Headerdatei für die Funktionen der EIB 741/742
      /lib               Bibliothek für die EIB 741/742 (Windows x32)
      /lib64             Bibliothek für die EIB 741/742 (Windows x64)
      /tools             Hilfsprogramme für die EIB 741/742



2. Anforderungen
----------------

LabView:
	Version 8.0 oder höher

Linux:
	Bibliotheken: pthread
	Die SO-Bibliothek wurde erstellt mit
	  - Kernel 3.1.0
	  - gcc 4.6.2
	  - libc 2.14.1

Windows:
	Betriebssystem: Windows 2000, Windows XP, Windows Vista, Windows 7
	Bibliotheken: user32.lib, ws2_32.lib, kernel32.lib



3. Erstinbetriebnahme
---------------------

Software installieren
---------------------
Die Treiberdateien (eib7.dll/eib7_64.dll bzw. libeib7.so/libeib7_64.so) müssen,
wie im Handbuch beschrieben, von der CD in das Systemverzeichnis kopiert 
werden. Anschließend muss der Inhalt des Ordners "EIB_741_742" von der CD in ein
beliebiges Verzeichnis auf einer lokalen Festplatte kopiert werden. Danach sind
die Beispiele und Tools verwendbar.

Netzwerkverbindung zur EIB 741/742 herstellen
-----------------------------------------
Bei der ersten Inbetriebnahme muss die EIB 741/742 mittels einer Punkt-zu-Punkt-
Netzwerkverbindung mit dem Rechner verbunden und konfiguriert werden. Dazu wird 
vor dem Anschließen des Netzwerkkabels auf dem Rechner folgende 
TCP/IP-Konfiguration für die verwendete Netzwerkkarte eingestellt:

- Feste IP-Adresse verwenden (nicht automatisch beziehen)
- IP-Adresse:   192.168.1.1
- Subnetzmaske: 255.255.255.0

Wurden diese Einstellungen vorgenommen, muss das Netzwerkkabel am Rechner und 
an der EIB 741/742 eingesteckt und die EIB 741/742 eingeschaltet werden. Sobald 
die STATUS-LED leuchtet, ist die EIB 741/742 betriebsbereit.

Testen der Verbindung
---------------------
Es wird empfohlen, als erstes mit dem ping-Befehl (DOS-Konsole bzw.Linux-Shell) 
zu prüfen, ob die EIB 741/742 per Netzwerk ansprechbar ist. Dies sollte der Fall 
sein, wenn alle beschriebenen Schritte ordnungsgemäß durchgeführt wurden.

Befehl:
ping 192.168.1.2

Auf der Konsole wird gemeldet, ob die EIB 741/742 ansprechbar ist oder nicht. 
Im zuerst genannten Fall können die Tools sowie die Beispielprogramme verwendet 
werden. Bei einigen dieser Programme muss die IP-Adresse der EIB 741/742 
eingegeben werden. In Auslieferungszustand der EIB 741/742 hat sie den 
Wert 192.168.1.2.

Falls die Netzwerkparameter der EIB 741/742 bereits verändert wurden, ist die 
EIB 741/742 nicht unter der Standard-IP-Adresse 192.168.1.2 ansprechbar. In diesem 
Fall wird ein Bootvorgang mit Standard-Netzwerkeinstellungen empfohlen.
(Siehe Installationsanleitung, Abschnitt "Reset-Modi")

Konfiguration der Netzwerkparameter der EIB 741/742
-----------------------------------------------
Dies erfolgt mit dem Tool "networksettings" (siehe unten).
Die EIB 741/742 kann damit für eine individuelle Netzwerkumgebung konfiguriert 
werden.

Ausführen der Beispiele und Tools
---------------------------------
Die mitgelieferten Programme für Windows und Linux werden direkt von der 
Konsole (Shell) ausgeführt. Die LabView-Programme liegen als VI's vor.



4. Beispiele
------------

C/C++ Beispiele für Window und Linux
------------------------------------

AuxAxis:
--------
Die Positionswerte des Messgeräts an Achse 1 können durch zwei verschiedenen
Triggerquellen gespeichert werden. Dazu wird die Hilfsachse verwendet.
An der Achse 1 muss ein Messgerät mit Inkrementalsignalen angeschlossen
sein. Die Positionswerte werden periodisch über den internen Timer Trigger
gespeichert. Parallel dazu kann die Position mit einem zweiten asynchronen
Triggersignal gespeichert werden.

easy:
-----
Die Achse 1 der EIB 741/742 wird für Messgeräte mit Inkrementalsignalen
initialisiert. Von dieser Achse wird der Positionswert und das Statuswort
abgefragt und angezeigt. Die EIB 741/742 muss die IP-Adresse 192.168.1.2
besitzen.

eib7info:
---------
Die Gerätedaten der EIB 741/742 werden ausgelesen. Hierzu zählen die
Seriennummer und MAC-Adresse sowie die Netzwerk-Parameter. Alle Daten
werden auf der Konsole ausgegeben. Die IP-Adresse der EIB 741/742 wird 
nach dem Programmstart abgefragt.

IntervalCounter:
----------------
Das Messgerät an Achse 1 generiert Triggerimpulse abhängig von der
aktuellen Position. Die Position der Achse 2 wird durch diese Triggerquelle
gespeichert und angezeigt.

MatchIncrEnDat:
---------------
Die absolute und die inkrementale Position eines EnDat01 Messgeräts wird
zeitgleich ausgelesen und angezeigt.

polling:
--------
Der Positionswert und das Statuswort einer Achse der EIB 741/742 wird ausgelesen
und angezeigt. Dazu wird die EIB 741/742 im Polling Modus betrieben. Die Achse,
von der die Positionswerte gelesen werden, ist wählbar. Ebenso wird der Typ
der Messgeräteschnittstelle vom Programm abgefragt. Der Anwender kann
zwischen Inkremental, EnDat 2.1 und EnDat 2.2 wählen. Die Positionswerte
werden in einer Schleife fortlaufend ausgelesen. Die IP-Adresse der EIB 741/742
muss nach dem Programmstart eingegeben werden.

PulsCounter:
------------
Die EIB 741/742 wird im Recording Modus betrieben und eine wählbare Achse
für inkrementale Messgeräte konfiguriert. Die Triggerung erfolgt über den
internen Timer Trigger. Die Triggerimpulse werden per Software Trigger
gestartet und durch den PulsCounter auf eine bestimmte Anzahl begrenzt.

softrealtime:
-------------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden für eine Achse der EIB 741/742 ausgelesen und angezeigt. Die EIB 741/742 
wird dazu im Soft Realtime Modus betrieben. Die Daten werden für vier Achsen
übertragen. Der Anwender kann wählen, welche der vier Achsen angezeigt wird.
Weiterhin kann der Anwender zwischen externer und interner Triggerung wählen.
Die Positionswerte werden in einer Schleife fortlaufend ausgelesen. Die
IP-Adresse der EIB 741/742 wird nach dem Programmstart abgefragt.

SoftrealtimeEnDat:
------------------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden für eine Achse der EIB 741/742 ausgelesen und angezeigt. Die EIB 741/742 
wird dazu im Soft Realtime Modus betrieben. Die Achse wird für EnDat 2.2 Messgeräte
konfiguriert. Der Anwender kann wählen, welche der vier Achsen angezeigt wird.
Weiterhin kann der Anwender zwischen externer und interner Triggerung wählen.
Die Positionswerte werden in einer Schleife fortlaufend ausgelesen. Die
IP-Adresse der EIB 741/742 wird nach dem Programmstart abgefragt.

streaming:
----------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden für eine Achse der EIB 741/742 ausgelesen und angezeigt. Die EIB 741/742 
wird dazu im Streaming Modus betrieben. Die Daten werden für vier Achsen
übertragen. Der Anwender kann wählen, welche der vier Achsen angezeigt wird.
Weiterhin kann der Anwender zwischen externer und interner Triggerung wählen.
Die Positionswerte werden in einer Schleife fortlaufend ausgelesen. Die
IP-Adresse der EIB 741/742 wird nach dem Programmstart abgefragt.

recording:
----------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden für eine wählbare Achse in der EIB 741/742 aufgezeichnet. Die EIB 741/742 
wird dazu im Recording Modus betrieben. Das Datenpaket wird so konfiguriert, dass
nur die Daten für die entsprechende Achse aufgezeichnet werden. Nach der
Aufzeichnungsphase werden die Daten zum PC übertragen und angezeigt. Die
IP-Adresse der EIB 741/742 wird nach dem Programmstart abgefragt. Der Anwender
kann zwischen externer und interner Triggerung wählen.

TriggerIO:
-----------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden für eine Achse der EIB 741/742 ausgelesen und angezeigt. Die EIB 741/742 
wird dazu im Soft Realtime Modus betrieben. Alle Achsen werden für inkrementale
Messgeräte konfiguriert. Der Anwender kann wählen, welche der vier Achsen
angezeigt wird. Die Triggerung erfolgt über den internen Timer Trigger.
Darüber hinaus wird das Triggersignal am Triggerausgang 1 ausgegeben. Die
IP-Adresse der EIB 741/742 wird nach dem Programmstart abgefragt.

TriggerRI:
----------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden für eine wählbare Achse der EIB 741/742 ausgelesen und angezeigt. Die
EIB 741/742 wird dazu im Soft Realtime Modus betrieben. Die Triggerung erfolgt
über die Referenzmarke des Messgeräts. Die IP-Adresse der EIB 741/742 wird nach
dem Programmstart abgefragt.


LabView Beispiele
-----------------

/EIB7Info:
----------

AuxAxis:
--------
Die Positionswerte des Messgeräts an Achse 1 können durch zwei verschiedenen
Triggerquellen gespeichert werden. Dazu wird die Hilfsachse verwendet.
An der Achse 1 muss ein Messgerät mit Inkrementalsignalen angeschlossen
sein. Die Positionswerte werden periodisch über den internen Timer Trigger
gespeichert. Parallel dazu kann die Position mit einem zweiten asynchronen
Triggersignal, dem Software Trigger 1, gespeichert werden. Beide
Positionswerte werden angezeigt.

/EIB7Info:
----------

GetConnInfo:
------------
Die Daten für alle derzeit geöffneten Verbindungen zur EIB 741/742 werden
ausgegeben. Hierzu zählen die IP-Adresse und Port-Nummer für die
TCP-Verbindungen jeweils auf der Seite der EIB 741/742 und des Hosts.

GetNetworkSettings:
-------------------
Die Einstellungen für die Netzwerkschnittstelle der EIB 741/742 werden 
ausgelesen und angezeigt.

SetNetworkSettings:
-------------------
Die Einstellungen für die Netzwerkschnittstelle der EIB 741/742 können verändert
werden. Bei der Ausführung dieses Programms werden die Einstellungen auf die
angegebenen Werte in den Eingabefeldern geändert. Diese sind nach dem
nächsten Reset wirksam.

/Polling:
---------

PollEncoderData:
----------------
Für eine Achse der EIB 741/742 wird der Trigger Counter, der Positionswert,
das Statuswort und der Timestamp ausgegeben. Die Achse wird für inkrementale
Messgeräte konfiguriert. Pro Programmdurchlauf wird ein Postionswert
abgefragt. Die EIB 741/742 wird im Polling Modus betrieben.

PollIncrEnDatPostition:
-----------------------
Eine Achse der EIB 741/742 wird für EnDat 01 konfiguriert. Die EnDat- und die
Inkrementalposition werden gleichzeitig ausgelesen.

PollPosition:
-------------
Die Positionswerte und das Statuswort der Achsen 1-4 werden zyklisch
abgerufen. Die Achsen können individuell für inkrementale Messgeräte,
oder für EnDat 2.1 bzw. EnDat 2.2 konfiguriert werden. Die EIB 741/742 wird
im Polling Modus betrieben.

/SoftRealtime:
--------------

SoftRealtime:
-------------
Der Positionswert, das Statuswort, der Timestamp und die Referenzpositionen
werden für die Achsen 1 und 2 der EIB 741/742 ausgelesen und angezeigt. Die
EIB 741/742 wird dazu im Soft Realtime Modus betrieben. Es kann zwischen
externer und interner Triggerung gewählt werden. Bei interner Triggerung
ist das Intervall des Timer Triggers einstellbar. Die Achsen werden für
inkrementale Messgeräte konfiguriert und die Positionswerte in einer Schleife
fortlaufend ausgelesen. Zusätzlich wird zu Beginn die Speicherung der
Referenzmarke aktiviert.

SoftRealtime_EnDat:
-------------------
Die Achse 1 wird für ein inkrementales Messgerät und die Achse 2 für ein
EnDat 2.2 Messgerät konfiguriert. Für das inkrementale Messgerät werden
zusätzlich zwei Referenzmarken gespeichert und deren Werte übertragen.
Bei dem EnDat Messgerät werden zyklisch verschiedene EnDat Zusatzinformationen
abgefragt. Die EIB 741/742 wird im Soft Realtime Modus betrieben. Es kann 
zwischen externer und interner Triggerung gewählt werden. Bei interner 
Triggerung ist das Intervall des Timer Triggers einstellbar.

/Streaming:
-----------

Streaming:
----------
Der Positionswert und das Statuswort werden für die Achsen 1 und 2 der
EIB 741/742 ausgelesen und angezeigt. Die EIB 741/742 wird dazu im Streaming 
Modus betrieben. Es kann zwischen externer und interner Triggerung gewählt 
werden. Bei interner Triggerung ist das Intervall des Timer Triggers 
einstellbar. Die Achsen werden für inkrementale Messgeräte konfiguriert und die
Positionswerte in einer Schleife fortlaufend ausgelesen. Parallel dazu wird
der FIFO für die Zwischenspeicherung der Daten in der EIB überwacht und
dessen Status angezeigt.

/Recording:
-----------

Recording:
----------
Der Positionswert, das Statuswort und der Timestamp werden für die Achsen
1 und 2 in der EIB 741/742 gespeichert. Die EIB 741/742 wird dazu im Recording 
Modus betrieben. Es kann zwischen externer und interner Triggerung gewählt 
werden. Bei interner Triggerung ist das Intervall des Timer Triggers einstellbar.
Parallel dazu wird der Speicher in der EIB 741/742 überwacht und angezeigt, wie
viele Samples bereits gespeichert wurden. Die Aufzeichnungsphase kann vom
Anwender beendet werden. Anschließend werden die Daten zum PC übertragen
und der Verlauf der Positionswerte in einem Diagramm graphisch dargestellt.

/Trigger:
---------

ExtTriggerOutput:
-----------------
Der Positionswert, das Statuswort und der Timestamp werden für Achse 1 und 2
der EIB 741/742 ausgelesen und angezeigt. Die EIB 741/742 wird dazu im 
Soft Realtime Modus betrieben. Alle Achsen werden für inkrementale Messgeräte 
konfiguriert. Die Triggerung erfolgt über den internen Timer Trigger. Darüber 
hinaus wird das Triggersignal des internen Timers am Triggerausgang 1 ausgegeben.

IntervalCounter:
----------------
Das Messgerät an Achse 1 generiert Triggerimpulse abhängig von der
aktuellen Position. Die Position der Achse 2 wird durch diese Triggerquelle
gespeichert und angezeigt. Der Abstand der Triggerimpulse ist einstellbar.

PulsCounter:
------------
Die EIB 741/742 wird im Recording Modus betrieben und die Achsen 1 und 2
für inkrementale Messgeräte konfiguriert. Die Triggerung erfolgt über den
internen Timer Trigger. Der erste Triggerimpuls wird durch den
Software Trigger freigegeben und die Anzahl der Triggerimpulse durch den
PulsCounter auf eine bestimmte Anzahl begrenzt. Die aufgezeichneten
Positionswerte werden nach der Messung in einem Diagramm dargestellt.

RI_Trigger:
-----------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden für eine Achse der EIB 741/742 ausgelesen und angezeigt. Die 
EIB 741/742 wird dazu im Soft Realtime Modus betrieben. Die Triggerung 
erfolgt über die Referenzmarke des Messgeräts.



5. Tools
--------

networksettings:
----------------
Die Einstellungen für die Netzwerkschnittstelle der EIB 741/742 können
verändert werden. Hierzu zählen die IP-Adresse, die Netzwerkmaske,
das Standard Gateway, die Einstellungen für den DHCP Client und
der Hostname.



6. Kontakt
----------

HEIDENHAIN
------------------------------------------
DR. JOHANNES HEIDENHAIN GmbH
Dr.-Johannes-Heidenhain-Straße 5
83301 Traunreut, Germany

Tel:    +49 8669 31-0
E-Mail: info@heidenhain.de
------------------------------------------
Messsysteme (EIB741/742 Support)
Tel:    +49 8669 31-3104
E-Mail: service.ms-support@heidenhain.de
------------------------------------------
www.heidenhain.de

