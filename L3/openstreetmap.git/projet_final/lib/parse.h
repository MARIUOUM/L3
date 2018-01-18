#ifndef PARSE_H
#define PARSE_H

#define LENGTH_REF_ARRAY 10
#define LENGTH_NODE_ARRAY 150
#define LENGTH_WAY_ARRAY 20
#define LENGTH_RELATION_ARRAY 10
#define LENGTH_MEMBER_NODE 5
#define LENGTH_MEMBER_WAY 5
#define LENGTH_MEMBER_REL 5
#define LENGTH_TAG_ARRAY 5
#define LENGTH_XML_STRING 15

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "error.h"

/************************************************************************/
/*Structure pour stocker les attributs k et v d'un élément tag **********/
/************************************************************************/
struct osm_tag {
  char *key;
  char *value;
};

/************************************************************************/
/*Structure pour stocker les attributs des noeuds ***********************/
/************************************************************************/
typedef struct {
  long long id; /* id d'un noeud */
  double lon; /* lon d'un noeud */
  double lat; /* lat d'un noeud */
  struct osm_tag *tags; /* Tableau de tag */
  int n_tag; /* Le nombre de tags de l'élément */
  int draw; /* vaut 1 si on a déjà dessiné 0 sinon */
  int visible;
} osm_node;

/************************************************************************/
/*Structure pour stocker les attributs de l'élément bounds **************/
/************************************************************************/
typedef struct {
  double minlon;
  double minlat;
  double maxlat;
  double maxlon;
} osm_bounds;

/***********************************************************************/
/*Structure pour stocker les attributs d'un élément way ****************/
/***********************************************************************/
typedef struct {
  long long id; /* id d'un élément way */
  int visible ; /* La visibilté d'un chemin */
  int  n_node; /* Le nombre de noeuds du way */
  osm_node** nodes; /* Tableau pour stocker  l'adresse des noeuds */
  int n_tag; /* Nombre de tags du way */
  struct osm_tag* tags; /* Tableau pour stocker les tags */
  int draw;
  int water;
  int haslayer; /* Si le way a un tag layer */
  int layer; /* La valeur de la clé layer */
} osm_way;

typedef struct osm_relation osm_relation ;

/************************************************************************/
/*Cette structure sert à stocker les membre de type way d'une relation **/
/************************************************************************/
typedef struct {
  osm_way* way;
  char* role;
} member_way;

/*************************************************************************/
/*Cette structure sert à stocker les membre de type node d'une relation **/
/*************************************************************************/
typedef struct {
  osm_node * node;
  char* role;
} member_node;

/*****************************************************************************/
/*Cette structure sert à stocker les membre de type relation d'une relation **/
/*****************************************************************************/
typedef struct {
  osm_relation* relation;
  char* role;
} member_relation;

/************************************************************************/
/*Structure pour stocker les attributs d'un élément relation ************/
/************************************************************************/
struct osm_relation {
  long long id;
  int visible;
  int n_node; /* Nombre de noeuds */
  member_node* nodes; /* Tableau des noeuds */
  int n_way; /* Nombre de ways */
  member_way* ways; /* Tableau des ways */
  int n_relation; /* Nombre de relations */
  member_relation* relations; /* Tableau des relations*/
  int n_tag; /* Nombre de tags */
  struct osm_tag* tags; /* Tableau des tags */
  int draw ; /* vaut 1 si on a déjà dessiné, 0 sinon */
};


/***********************************************************************/
/*Structure pour stocker tous les éléments d'une carte *****************/
/***********************************************************************/
typedef struct {
  osm_bounds* bounds;
  int n_node; /* Nombre de node */
  osm_node* nodes; /* Tableau pour stocker les nodes */
  int  n_way; /* Nombre de ways */
  osm_way* ways; /* Tableau pour stocker les ways */
  int n_relation; /* Nombre de relations */
  osm_relation* relations; /* Tableau pour stocker les relations */
  int coastline ; /* S'il y'a un coastine */
  osm_way ** waters ; /* Les ways qui ont le tag natural et waterway */
  osm_way ** primary ; /* L'adresse des ways qui ont un tag avec une clé highway et comme valeur primary */
  osm_way ** secondary ; /* L'adresse des ways qui ont un tag avec une clé highway et comme valeur secondary */
  osm_way ** tertiary ; /* L'adresse des ways qui ont un tag avec une clé highway et comme valeur tertiary */
  osm_way ** names ; /* Les noms des rues */
  osm_way ** layerMoins1; /* Layer avec les valeurs -1 */
  int n_layerMoins1; /* Nombre de layers avec les valeurs -1 */
  osm_way ** layerMoins2; /* Layer avec les valeurs -2 */
  int n_layerMoins2; /* Nombre de layers avec les valeurs -2 */
  osm_way ** layerMoins3; /* Layer avec les valeurs -3 */
  int n_layerMoins3; /* Nombre de layers avec les valeurs -3 */
  osm_way ** layerMoins45 ; /* Layer avec les valeurs -4 -5 */
  int n_layerMoins45 ; /* Nombre de layers avec les valeurs -4 -5 */
  osm_way ** layer0; /* Layer avec la valeur 0 */
  int n_layer0; /* Nombre de layers avec les valeurs 0 */
  osm_way ** layer1 ; /* Layer  avec les valeurs 1 */
  int n_layer1; /* Nombre de layers avec les valeurs 1 */
  osm_way ** layer2 ; /* Layer  avec les valeurs 2 */
  int n_layer2; /* Nombre de layers avec les valeurs 2 */
  osm_way ** layer3 ; /* Layer avec les valeurs 3 */
  int n_layer3; /* Nombre de layers avec les valeurs 3 */
  osm_way ** layer4 ; /* Layer avec les valeurs 4 */
  int n_layer4; /* Nombre de layers avec les valeurs 4 */
  osm_way ** layer5 ; /* Layer avec les valeurs 5 */
  int n_layer5; /* Nombre de layers avec les valeurs 5*/
  int n_name ;/* Nombre de ways  avec le tag names */
  int n_primary ; /* Nombre de ways avec le tag primary */
  int n_secondary ; /* Nombre de ways avec le tag secondary */
  int n_tertiary ; /* Nombre de ways avec le tags tertiary */
  int n_water ; /* Nombre de ways avec water */
  osm_way* coastWay ;
} osm;

/**********************************************************************/
/*parseBounds : récupère les attributs de l'élément bounds ************/
/**********************************************************************/
error parseBounds(xmlNodePtr node,osm_bounds*) ;

/**********************************************************************/
/*parseNode: récupère les éléments node du document *******************/
/**********************************************************************/
error parseNode(xmlNodePtr cur, osm_node*) ;

/**********************************************************************/
/*Parse le document donné en argument *********************************/
/**********************************************************************/
error parseDoc(char * docname, osm * os);

/**********************************************************************/
/*parseWay : récupère les attributs de l'élément way ******************/
/**********************************************************************/
error parseWay(xmlNodePtr cur,osm_way*, osm* ) ;

/**********************************************************************/
/*parseRelation : récupère les attributs de l'élément relation ********/
/**********************************************************************/
error parseRelation(xmlNodePtr cur,osm_relation*, osm* ) ;

/**********************************************************************/
/*Affiche les valeurs de la structure osm *****************************/
/**********************************************************************/
void afficher(osm os );

#endif
