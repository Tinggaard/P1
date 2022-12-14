# P1 projekt

P1 projekt for gruppe `cs-22-dat-1-p1-05` på datalogiuddannelsen, Aalborg Universitet.

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

Nu kan det kompilerede program findes i [src](src/) mappen.

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


### Filstruktur

`src/files/shopping_list.txt`: Filen betår af de varer som ønskes handlet - én vare pr. linje.
Disse varer *skal* fremgå i filen [src/files/normal_prices.csv](src/files/normal_prices.csv), ellers terminerer programmet, idet der ønskes en vare som ikke findes.

Filen har format som følger:
```
cheese
milk
chicken breast
vare 4
vare 5
```

Hver vare kan maskimalt have 14 karakterer, idet størrelsen af de structs som holder på vareinformationen er begrænset af dette.
Alternativt skal `MAX_NAME_SIZE` ændres i [src/lib.h](src/lib.h).

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
