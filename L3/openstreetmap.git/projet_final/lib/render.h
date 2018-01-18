#ifndef RENDER_H
#define RENDER_H

#include <math.h>
#include "utils.h"
#include "render_colors.h"
#include "render_roads_thick.h"

double merc_x (double lon);

double merc_y (double lat);

/***********************************************************************************************************/
/*DrawArea : dessine tout ce qui est représenté sur la carte par un chemin fermé ***************************/
/***********************************************************************************************************/
void DrawArea(osm_way* way,int relation);

/**********************************************************************/
/*DrawWay : dessine les chemins ***************************************/
/**********************************************************************/
void DrawWay(osm_way* way);

void draw_relation(osm_relation* relation);

void parcoursDrawWay(int way, int taille_tab, osm_way** ways);

/*********************************************************************************************/
/*DrawOsm : dessine une carte à partir des résultats du parsing d'un fichier xml *************/
/*********************************************************************************************/
void DrawOsm(osm* map);

/**********************************************************************/
/*drawWayName : dessine le nom des rues *******************************/
/**********************************************************************/
void drawWayName(osm_way* way);

/**********************************************************************/
/*docRender : lit un fichier xml, le parse et l'affiche ***************/
/**********************************************************************/
int docRender(char* docname, int export);

/****************************************************************************************************/
/*exportRender : exporte le rendu sous forme d'image BMP et la stocke dans le dossier fichiers_test */
/****************************************************************************************************/
void exportRender(char* docname);

/*****************************************************************************************************/
/*docRender : affiche une fenêtre avec une carte OpenStreetMap à partir d'un fichier xml *************/
/*****************************************************************************************************/
int docRender(char* docname, int export);

#endif
