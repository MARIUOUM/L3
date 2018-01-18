#include "utils.h"

static int cmpfuncNode(const void *a , const void *b) {
  osm_node nodea = *(osm_node const *)a;
  osm_node nodeb = *(osm_node const *)b;
  long long result;
  result = nodea.id - nodeb.id;
  if(result<0) return -1;
  if(result>0) return 1;
  return 0;
}

static int cmpfuncWay(const void *a, const void *b) {
  osm_way waya = *(osm_way const *)a;
  osm_way wayb = *(osm_way const *)b;
  long long result = waya.id - wayb.id;
  if(result<0) return -1;
  if(result>0) return 1;
  return 0;
}

void sort_node(osm *os) {
  qsort(os->nodes, os->n_node, sizeof(osm_node), cmpfuncNode);
}

void sort_way(osm *os) {
  qsort(os->ways, os->n_way, sizeof(osm_way), cmpfuncWay);
}

/**********************************************************************/
/*getNodeById : récupère un node avec son identifiant *****************/
/**********************************************************************/
osm_node* getNodeById(osm *os, long long ref) {
  int trouve =  0 ;
  int idebut = 0 ;
  int ifin = os->n_node ;
  int imil = 0 ;

  while(!trouve && ( (ifin-idebut) > 1)) {
    imil = (idebut + ifin)/2;
    trouve = (os->nodes[imil].id== ref);

    if(os->nodes[imil].id > ref) ifin = imil;

    else idebut = imil ;
  }

  if(os->nodes[idebut].id == ref) return &(os->nodes[idebut]);

  return NULL ;
}

/**********************************************************************/
/*getWayById : récupère un way avec son identifiant *******************/
/**********************************************************************/
osm_way* getWayById(osm *os, long long ref) {

  int trouve = 0;
  int idebut = 0;
  int ifin = os->n_way;
  int imil = 0;

  while(!trouve && ((ifin-idebut) > 1)) {
    imil = (idebut + ifin)/2;

    trouve = (os->ways[imil].id== ref);

    if(os->ways[imil].id > ref) ifin = imil;
    else idebut = imil;
  }

  if(os->ways[idebut].id == ref) return &(os->ways[idebut]);

  return NULL ;
}

/**********************************************************************/
/*getRelationById : récupère une relation avec son identifiant ********/
/**********************************************************************/
osm_relation* getRelationById(osm *os, long long ref) {
  int i = 0 ;

  for(;i<os->n_relation;i++) {
    if(os->relations[i].id==ref)
      return &(os->relations[i]);
  }

  return NULL ;
}

/**********************************************************************/
/*afficher : affiche les resultats du parsing *************************/
/**********************************************************************/
void afficher(osm os ) {

    /* Afficher bounds*/
    printf("boud minlon %f\n", os.bounds->minlon);
    printf("boud minlat %f\n", os.bounds->minlat);
    printf("boud maxlon %f\n", os.bounds->maxlon);
    printf("boud maxlat %d\n", os.n_node);
    printf("Nombre de noeuds %d\n",os.n_node);
    printf("Nombre de ways %d\n",os.n_way);
    printf("Nombre de relations %d\n",os.n_relation );
    printf("\n\n\n");
    
    /* Afficher nodes */
    osm_node * nodes = os.nodes;
    int i = 0 ;

    for(;i<os.n_node;i++) {

      printf(" id node %lld\n",nodes->id );
      printf(" lon node%f\n", nodes->lon);
      printf(" lat node%f\n", nodes->lat);
      printf(" visible node%d\n", nodes->visible);
      printf("\n\n\n");

      int itag_node = 0 ;
      for(;itag_node < nodes->n_tag;itag_node++) {
        printf("\t key %s\n", nodes->tags[itag_node].key);
        printf("\t value %s\n\n\n", nodes->tags[itag_node].value);
      }

      nodes++;
    }

    osm_way * ways = os.ways;

    for(i=0;i<os.n_way;i++) {

      printf(" id way %lld\n",ways->id);
      printf(" nombre de  node %d\n", ways->n_node);
      printf(" nombre de tags %d\n", ways->n_tag);
      printf(" visible %d\n", ways->visible);
      printf("\n\n\n");

      int itag= 0 ;
      for(;itag < ways->n_tag;itag++) {
        printf("\t key %s\n", ways->tags[itag].key);
        printf("\t value %s\n\n\n", ways->tags[itag].value);
      }

      for(itag=0;itag < ways->n_node;itag++) {
        printf("\t ref node: %lld\n", ways->nodes[itag]->id);
      }
      printf("\n\n\n");
      ways++;
    }

    osm_relation * relation = os.relations;

    for(i=0;i<os.n_relation;i++) {

      printf(" id relation %lld\n",relation->id);
      printf(" nombre de  node %d\n", relation->n_node);
      printf(" nombre de tags %d\n", relation->n_tag);
      printf(" nombre de ways %d\n", relation->n_way);
      printf(" nombre de relations %d\n", relation->n_relation);
      printf(" visible %d\n", relation->visible);
      printf("\n\n\n");

      int itag= 0 ;
      for(;itag < relation->n_tag;itag++) {
        printf("\t key %s\n", relation->tags[itag].key);
        printf("\t value %s\n\n\n", relation->tags[itag].value);
      }

      for(itag=0;itag <relation->n_way;itag++) {
        printf("\t member way : %lld\n", relation->ways[itag].way->id);
        printf("\t member role : %s\n", relation->ways[itag].role);

      }
      printf("\n\n\n");

      for(itag=0;itag <relation->n_relation;itag++) {
        printf("\t member relation : %lld\n", relation->relations[itag].relation->id );
        printf("\t member role : %s\n", relation->relations[itag].role  );
      }
      printf("\n\n\n");

      relation++;
    }
}
