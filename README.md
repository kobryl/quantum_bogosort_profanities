# quantum_bogosort_profanities

Zadanie eliminacyjne na Hackaton 2023. Program służy do wykrywania słów wulgarnych i zastępowania ich gwiazdkami. Program działa w trybie tekstowym oraz graficznym. Istnieje możliwość dodawania słów do list słów filtrowanych.
<br/>
<br/>

## :writing_hand: Autorzy 
- Konrad Bryłowski
- Aleksander Czerwionka
- Michał Krause
- Łukasz Nowakowski
<br/>
<br/>

## :wrench: Tryby działania programu
### Tryb tekstowy
By otworzyć program w trybie tekstowym, należy uruchomić plik wykonywalny "profanities.exe". W konsoli należy wpisać interesujący nas tekst, a następnie należy zakończyć proces poprzez wciśnięcie ctrl+Z. 

### Tryb graficzny
Aby uruchomić program w trybie graficznym należy uruchomić plik wykonywalny "gui.exe". Tryb graficzny pozwala na wprowadzanie tekstu do przefiltrowania oraz dodawanie słów do wyjątków i do listy wulgaryzmów - *dodane w ten sposób słowa będą wykrywane tylko z dokładnym dopasowaniem*.
<br/>
<br/>

## :closed_lock_with_key: Filtrowanie słów
### Dodawanie do wyjątków
Istnieje możliwość dodania słów, które naszym zdaniem nie powinny być cenzurowane. Filtr ten działa dla słów z dokładnym dopasowaniem.

### Dodawanie do listy słów filtrowanych
Opcja pozwala nam dodawać słowa do cenzury, które są dla nas obraźliwe. Filtr ten działa dla słów z dokładnym dopasowaniem.
<br/>
<br/>

## :gear: Użyte technologie
- Python
- C++ 
- Qt6
- Unidecode
- PyInstaller (do utworzenia pliku gui.exe)
