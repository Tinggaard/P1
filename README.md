### P1 projekt

P1 projekt løsningen skal handle om at informere den bedvidste forbruger om hvilken indkøbsmulighed der er billigst ud fra deres indkøbsliste

Her er lidt grim pseudokode, som ikke er færdigt:

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
		

		

	