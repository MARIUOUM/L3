#include <string.h>
#include "render.h"
#include "error.h"

int main(int argc, char* argv[]) {
	int export = 0;

    if(argc > 3) {
		fprintf(stderr,"Erreur arguments: parse <nom_fichier_valide_osm>\n");
		return 2 ;
	}

	// Si on ajoute l'option export, le rendu sera exporté sous format BMP
	if(argc == 3) {
		if (!(strcmp(argv[2], "-export"))) {
			printf ("Export du rendu sous forme d'image BMP\n");
			export = 1;
		}
	}

    int a = docRender(argv[1], export);

    return a;
}
