# quantum_bogosort_profanities

Zadanie eliminacyjne na Hackaton 2023. Program służy do wykrywania słów wulgarnych i zastępowania ich gwiazdkami. Program działa w trybie tekstowym oraz graficznym. Istnieje możliwość dodawania słów do list słów filtrowanych.
<br>
<br>

## :writing_hand: Autorzy 

- Konrad Bryłowski
- Aleksander Czerwionka
- Michał Krause
- Łukasz Nowakowski
<br>

## Przewidziane sytuacje

- kilka tych samych liter obok siebie
- podobne znaki (np. $ -> s, u -> v, vv -> w itp.)
- uwzględnianie w powyższych zamian dla kilku przypadków (np. ! może być i lub l)
- rozdzielenie słowa spacją
<br>

## :wrench: Tryby działania programu

### Tryb tekstowy

By otworzyć program w trybie tekstowym, należy uruchomić plik wykonywalny `profanities.exe`. Program przyjmuje tekst do przefiltrowania na standardowym wejściu. W konsoli należy wpisać interesujący nas tekst, a następnie zasygnalizować koniec tekstu do filtrowania znakiem końca pliku (Ctrl+Z). Wynik działania zostanie wysłany na standardowe wyjście.

### Tryb graficzny

Aby uruchomić program w trybie graficznym należy uruchomić plik wykonywalny `gui.exe`. Tryb graficzny pozwala na wprowadzanie tekstu do przefiltrowania oraz dodawanie słów do wyjątków i do listy wulgaryzmów - *dodane w ten sposób słowa będą wykrywane tylko z dokładnym dopasowaniem*.
<br>
<br>

## :closed_lock_with_key: Filtrowanie słów

### Dodawanie do wyjątków

Istnieje możliwość dodania słów, które naszym zdaniem nie powinny być cenzurowane. Aby dodać nowe słowo należy dodać na koniec pliku `whitelist.txt` nowe słowo w formacie:

    slowo p z

gdzie: <br>
`slowo` to dodawane słowo pozbawione polskich znaków diakratycznych (np. "słów" -> "slow");<br>
`p` oznacza liczbę dowolnych znaków dozwolonych przed słowem; <br>
`z` oznacza liczbę dowolnych znaków dozwolonych za słowem.

Plik `whitelist.txt` musi być zakończony pustą linią.

### Dodawanie do listy słów filtrowanych

Opcja pozwalająca dodawać do cenzury słowa, które są dla nas obraźliwe działa podobnie jak dodawanie wyjątków. Na koniec pliku `profanity_list.txt` należy dodać wpis w formacie:

    slowo m p z

gdzie: <br>
`slowo` to dodawane słowo pozbawione polskich znaków diakratycznych (np. "słów" -> "slow");<br>
`m` oznacza liczbę dowolnych znaków, które mogą się znajdować pomiędzy literami słowa, aby było ono wykryte jako wulgaryzm;<br>
`p` oznacza liczbę dowolnych znaków umieszczonych przed wulgaryzmem, które będą uzwględnione w przekleństwie;<br>
`z` oznacza liczbę dowolnych znaków umieszczonych po wulgaryzmie, które będą uwzględnione w przekleństwie.<br>

Plik `profanity_list.txt` musi być zakończony pustą linią.

### Przywracanie domyślnych list

Domyślne listy znajdują się w katalogu `default_lists/`. Aby przywrócić domyślne działanie programu wystarczy skopiować je do głównego katalogu.
<br>
<br>

## :gear: Użyte technologie

- Python
- C++ 
- Qt6 (PySide6)
- Unidecode
