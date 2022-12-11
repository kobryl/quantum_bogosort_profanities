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
By otworzyć program w trybie tekstowym, należy uruchomić plik wykonywalny `profanities.exe`. Program przyjmuje tekst do przefiltrowania na standardowym wejściu. W konsoli należy wpisać interesujący nas tekst, a następnie zasygnalizować koniec tekstu do filtrowania znakiem końca pliku (Ctrl+Z). Wynik działania zostanie wysłany na standardowe wyjście.

### Tryb graficzny
Aby uruchomić program w trybie graficznym należy uruchomić plik wykonywalny `gui.exe`. Tryb graficzny pozwala na wprowadzanie tekstu do przefiltrowania oraz dodawanie słów do wyjątków i do listy wulgaryzmów - *dodane w ten sposób słowa będą wykrywane tylko z dokładnym dopasowaniem*.
<br/>
<br/>

## :closed_lock_with_key: Filtrowanie słów
### Dodawanie do wyjątków
Istnieje możliwość dodania słów, które naszym zdaniem nie powinny być cenzurowane. Aby dodać nowe słowo należy dodać na koniec pliku `whitelist.txt` nowe słowo w formacie:

    słowo p z

gdzie <br>
`p` oznacza liczbę dowolnych znaków dozwolonych przed słowem; <br>
`z` oznacza liczbę dowolnych znaków dozwolonych za słowem.

Plik `whitelist.txt` musi być zakończony pustą linią.

### Dodawanie do listy słów filtrowanych
Opcja pozwalająca dodawać do cenzury słowa, które są dla nas obraźliwe działa podobnie jak dodawanie wyjątków. Na koniec pliku `profanity_list.txt` należy dodać wpis w formacie:

    słowo o p z
gdzie <br>
`o` oznacza <br>
`p` oznacza <br>
`z` oznacza <br>
<br/>
<br/>

## :gear: Użyte technologie
- Python
- C++ 
- Qt6
- Unidecode
- PyInstaller (do utworzenia pliku gui.exe)
