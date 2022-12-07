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

Nu kan det kompilerede program findes i `src` mappen.

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
Disse varer *skal* fremgå i filen `src/files/normal_prices.txt`, ellers terminerer programmet, idet der ønskes en vare som ikke findes.

Filen har format som følger:
```
cheese
milk
chicken breast
vare 4
vare 5
```
