# P1 projekt

Projekt 1 for gruppe `cs-22-dat-1-p1-05` på datalogiuddannelsen, Aalborg Universitet.

Projektet omhandler optimering af tilbudsjagt på dagligvarer, og er skrevet i C.


## Kompilering

Projektet er skrevet i CLion, og er kompileret med [cmake](https://cmake.org/install/).

```bash
git clone https://github.com/Tinggaard/P1
cd P1
cmake . # initialiser cmake
```

Herefter kan selve projektet kompileres enten med `cmake`, eller `make`.

```bash
cmake --build .
# eller
make
```

Nu kan det kompilerede program findes i [src/](src/) mappen.

```bash
cd src
./P1 # kør programmet
# på Windows hedder den eksekverbare fil 'P1.exe'
```

For at fjerne de kompilerede filer igen, bruges `clean` argumentet til enten `cmake` eller `make`.

```bash
cmake --build . --target clean
# eller
make clean
```

## Brug af programmet

Når programmet køres, spørger det først om en radius at finde tilbud inden for.
Denne radius angives i meter, og afsluttes med en newline.

Derefter spørger programmet om du er i bil, eller ej, hvor `y` angiver ja, og `n` er nej.

Såfrem man er i bil, beder programmet derefter om en pris pr km kørt i bilen, hvilket kan angives som et decimaltal. 
Denne pris medregnes nu i de samlede omkostninger for indkøbet, alt efter hvor langt der er til de respektive butikker.

**OBS**: Hvis der angives en radius, inden for hvilken, der ikke ligger nogen butikker, vil programmet ikke give et brugbart output.

### Filstruktur og ændring af standarder

Filen [src/files/shopping_list.csv](src/files/shopping_list.csv) betår af de varer som ønskes handlet - én vare pr. linje.
Disse varer *skal* fremgå i filen [src/files/normal_prices.csv](src/files/normal_prices.csv),
ellers terminerer programmet, idet der ønskes en vare som ikke findes i datasættet.

[src/files/shopping_list.csv](src/files/shopping_list.csv) har format som følger:
```
cheese
milk
chicken breast
vare 4
vare 5
```

Hver vare kan maskimalt have 14 karakterer, idet størrelsen af de structs som holder på vareinformationen er begrænset af dette.
Alternativt skal `MAX_NAME_SIZE` ændres i [src/lib.h](src/lib.h).

---

Lokationen der tages udgangspunkt i for brugeren, ligger i filen [src/files/user_location.csv](src/files/user_location.csv).

Denne fil indeholder udelukkende to decimaltal, for længdegrad og breddegrad, adskilt med et komma og et mellemrum.
Der er ingen newlines i filen.

Lige nu er der følgende koordinater i filen:

```csv
57.0139045715332, 9.986823081970215
```

Hvilket svarer til adressen `Fredrik Bajers Vej 7A, 9220 Aalborg`

## Tidstagning

Inde i mappen [compare/](compare/) ligger en modificeret version af programmet, hvor funktionen `brute_force()` er implementeret,
som finder den absolut korteste vej, ved at bruteforce alle n! kombinationer.

Når programmet køres printer det automatisk tiden brugt på hver af de to algoritmer. 
Det er også muligt at tage tid på den samlede eksekvering, ved brug af kommandolinjen i bash:

```bash
make 
cd compare
{ time ./P1_compare > /dev/null ; } 2>&1 | cut -d ' ' -f 4-
```

Som udgangspunkt er der 10 butikker, som alle skal besøges, i dette eksempel.

Det er dog muligt at generere flere butikker automatisk vha. scriptet [compare/generate.py](compare/generate.py), som er skrevet i Python.
Variablen `N` sættes blot til det ønskede antal stores, hvorefter filen køres (*skal* køres fra `compare/` mappen, idet filstierne er relative.)
Ønkes færre butikker til testen, slettes en enkelt vare blot fra indkøbslisten, da hver vare indikerer en ny butik.

```bash
./generate.py                                                             
```
