#ifndef UTILS_H
#define UTILS_H

#include "parse.h"

/**********************************************************************/
/*getNodeById : récupère un node avec son identifiant *****************/
/**********************************************************************/
osm_node * getNodeById(osm *, long long);

/**********************************************************************/
/*getWayById : récupère un way avec son identifiant *******************/
/**********************************************************************/
osm_way * getWayById(osm*, long long);

/**********************************************************************/
/*getRelationById : récupère une relation avec son identifiant ********/
/**********************************************************************/
osm_relation * getRelationById(osm*, long long);

void sort_node(osm *);
void sort_way(osm *);

/**********************************************************************/
/*afficher : affiche les resultats du parsing *************************/
/**********************************************************************/
void affciher(osm);

void zoomIn();
void zoomOut();

#endif
