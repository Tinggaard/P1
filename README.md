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

## Programmets output
Programmet giver et output to forskellige formater. Det ene format tager udgangspunkt i, at brugeren er villig til at 
tage en længere rute, for at spare mest mulig på sin indkøbsliste. Den andet format tager udgangspunkt i at alle varer
købes i en butik.

### På tværs af butikker
Den første oversigt viser hvilke varer der skal købes i hver butik, for
at opnå den billigste indkøbsliste. Butikkerne udskrives i den rækkefølge som anbefales at besøge butikkerne for at minimere
den tilbagelagte afstand. I "distance" kolonnen foran hver butik udskrives afstanden fra den forrige lokation. 
Hvis der skal handles flere vare i en butik udskrives det som følgende:
```
---------------------------------------------------------------
|           Your absolute cheapest shopping list              |
|   Follow the order of stores to achieve the shortest route  |
---------------------------------------------------------------
|Store          |Distance     |Item           |Price          |
|Butik1          1629m         vare1           12.00 DKK      |
|                              vare2           10.00 DKK      |
|Butik2          3762m         vare3            5.00 DKK      |
---------------------------------------------------------------
```
Dette output betyder at både *vare1* og *vare2* skal købes i *butik1* og *vare3* skal købes i *butik2*

Til sidst udskrives den totale afstand, den totale pris ekskl. transport, transportprisen og den totale pris inkl. transport.

### Enkelt butik
Det andet format som programmet kan vise antager at alle varer handles i samme butik. Dette udskrives som følgende:
```
  ------------------------------------------------------------------------------------
  |                        If you want to shop in one store only,                    |
  |                  here is sorted a list of cheapest stores for you.               |
  ------------------------------------------------------------------------------------
  |Store          |Distance     |Travel expenses    |Item expenses    |Total sum     |
  |Butik1          2566m         5.13 DKK            108.00 DKK        110.57 DKK    |
  |Butik2          3360m         6.72 DKK            116.00 DKK        119.36 DKK    |
  ------------------------------------------------------------------------------------
```
Outputtet ovenfor viser at det er billigst at handle alle varer i *butik1*. Butikkerne sorteres og udskrives således, at
de billigste butikker printes øverst.


### Filstruktur og ændring af standarder

Filen [src/files/shopping_list.csv](src/files/shopping_list.csv) består af de varer som ønskes handlet - én vare pr. linje.
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
