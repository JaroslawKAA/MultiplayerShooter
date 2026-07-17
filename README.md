\# MultiplayerShooter



Sieciowa gra typu arena shooter zbudowana w \*\*Unreal Engine 5.8 (C++)\*\*, stworzona jako projekt portfolio/rekrutacyjny demonstrujący architekturę sieciową i wzorce projektowe w UE5.



\## Tech stack



\- \*\*Silnik:\*\* Unreal Engine 5.8

\- \*\*Język:\*\* C++ (hybrydowa architektura C++/Blueprint)

\- \*\*System input:\*\* Enhanced Input System

\- \*\*VFX:\*\* Niagara

\- \*\*Sieć:\*\* Online Subsystem (LAN, replikacja Actor/RPC)

\- \*\*IDE:\*\* JetBrains Rider



\## Architektura



Projekt konsekwentnie stosuje wzorzec \*\*C++ jako logika, Blueprint jako konfiguracja\*\*:



\- \*\*C++\*\* odpowiada za całą logikę gameplay, replikację sieciową i strukturę klas (`CPP\_` prefix).

\- \*\*Blueprint\*\* (`BP\_` prefix) konfiguruje assety, wartości domyślne i prezentację wizualną (AnimGraph, VFX assets, UI layout) przez `EditDefaultsOnly` i eventy.



Kluczowe klasy:



| Klasa | Odpowiedzialność |

|---|---|

| `CPP\_ArenaGameInstance` | Zarządzanie sesją sieciową (LAN), zapisami gracza (SaveGame), przekazywanie danych do UI przez delegaty |

| `CPP\_PlayerCharacter` | Ruch gracza, kamera, input, broń |

| `CPP\_Weapon` | Logika strzału, VFX (muzzle flash, impact) jako właściwości broni |

| `CPP\_AnimInstance` | Zmienne i logika animacji (Speed, IsInAir) na potrzeby AnimGraph w BP |

| `CPP\_PlayerController` | Lifecycle UI (menu główne), input mapping context |



\### Wybrane decyzje projektowe



\- \*\*VFX broni jako właściwość klasy `CPP\_Weapon`\*\* (nie Factory Pattern) – każda broń zna swój efekt statycznie, dodatkowa warstwa fabryki nie wnosiła realnej elastyczności.

\- \*\*Komunikacja C++ → UI przez `DynamicMulticastDelegate`\*\* (np. wyniki wyszukiwania sesji) zamiast bezpośrednich wywołań – pozwala wielu widgetom niezależnie subskrybować te same dane bez sprzężenia z GameInstance.

\- \*\*Wrapper structs (`BlueprintType`) dla danych Online Subsystem\*\* – `FOnlineSessionSearchResult` nie jest Blueprint-friendly, więc dane do UI są konwertowane na dedykowane, proste structy.



\## Uruchomienie projektu



\### Wymagania



\- Unreal Engine 5.8

\- Visual Studio 2022 lub JetBrains Rider z pluginem RiderLink



\### Kroki



1\. Sklonuj repozytorium.

2\. Kliknij prawym przyciskiem na `MultiplayerShooter.uproject` → \*\*Generate Visual Studio project files\*\*.

3\. Otwórz projekt w Rider/Visual Studio i zbuduj konfigurację \*\*Development Editor\*\*.

4\. Uruchom edytor UE5, otwórz projekt.



\### Testowanie multiplayer (LAN)



1\. W \*\*Editor Preferences → Level Editor → Play → Multiplayer Options\*\* ustaw liczbę graczy (min. 2) i \*\*odznacz "Run Under One Process"\*\* — wymagane, żeby wykrywanie sesji LAN (UDP broadcast) działało poprawnie między instancjami.

2\. Uruchom PIE — jedna instancja hostuje sesję, druga wyszukuje i dołącza przez ekran Menu Głównego.



\## Status projektu



Projekt w aktywnym rozwoju. Zaimplementowane: ruch i kamera gracza, system broni z VFX, animacje, menu główne z UI, wyszukiwanie i dołączanie do sesji LAN, zapisywanie profilu gracza. W toku: pełna replikacja rozgrywki, cykl życia meczu (State Pattern w GameMode).



