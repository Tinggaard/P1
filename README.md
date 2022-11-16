# P1 projekt

P1 projekt løsningen skal handle om at informere den bedvidste forbruger om hvilken indkøbsmulighed der er billigst ud fra deres indkøbsliste

Her er lidt grim pseudokode, som ikke er færdigt:

### file structures

`normal_prices.txt`
```
item, price
ost, 50
```

`discounts.txt`
```
store, item, price
rema, ost, 40
```

`distances.txt`
```
store, distance
rema, 1000
```

`shopping_list.txt`
```
item
ost
```

```
struct vare 
	char 	navn[]
	int 	pris

struct butik
	char 	navn[]
	int 	afstand
	vare*	varer

struct handel
	butik butikken
	vare varen


main()
	indlæs afstande
	indlæs normalpriser
	indlæs tilbud
	indlæs brugerens indkøbsliste

    lav struct arrays for alle

if aller billigste
	handel indkøb[] 
	for alle varer i brugerens indkøbsliste
		for alle tilbud
			Hvis billigste
				Handel
```

