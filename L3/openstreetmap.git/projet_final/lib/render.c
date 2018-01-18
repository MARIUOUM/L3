#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <string.h>
#include "render.h"
#include "graphics.h"

#define WINDOW_SIZE 750
#define earth_radius 6378137

#define D_R (M_PI / 180.0)
#define R_D (180.0 / M_PI)
#define R_MAJOR 6378137.0
#define R_MINOR 6356752.3142
#define RATIO (R_MINOR/R_MAJOR)
#define ECCENT (sqrt(1.0 - (RATIO * RATIO)))
#define COM (0.5 * ECCENT)

int coast = 0 ;

/**************************
 * Local rendering variables
 */
static double minlat;
static double maxlat;
static double minlon;
static double maxlon;

/****************************
 * Screen dimension constants
 */
int SCREEN_WIDTH;
int SCREEN_HEIGHT;

/*********************************
 *The window we'll be rendering to
 */
SDL_Window* mapWindow  = NULL;

TTF_Font *font ;

/*************
 *SDL Renderer
 */
SDL_Renderer*  mapRenderer  = NULL;

SDL_Texture *texture =NULL;

/*************************
 * Mercator transformation
 */
static double deg_rad (double ang) {
  return ang * D_R;
}

double merc_x (double lon) {
  return R_MAJOR * deg_rad (lon);
}

double merc_y (double lat) {
  lat = fmin (89.5, fmax (lat, -89.5));
  double phi = deg_rad(lat);
  double sinphi = sin(phi);
  double con = ECCENT * sinphi;
  con = pow((1.0 - con) / (1.0 + con), COM);
  double ts = tan(0.5 * (M_PI * 0.5 - phi)) / con;
  return 0 - R_MAJOR * log(ts);
}

/***********************************************************************************************************/
/*positionX : Convertit GPS lon en une valeur en "pixel" (entre 0 et 1) ************************************/
/*On a juste besoin de multiplier cette valeur avec SCREEN_WIDTH pour connaitre la position d'un point *****/
/***********************************************************************************************************/
static Sint16 positionX(double lon) {
  return (Sint16)(SCREEN_WIDTH * (merc_x(lon)-merc_x(minlon))/(merc_x(maxlon)-merc_x(minlon)));
}

/***********************************************************************************************************/
/*positionY : Convertit GPS lat en une valeur en "pixel" (entre 0 et 1) ************************************/
/*On a juste besoin de multiplier cette valeur avec SCREEN_HEIGHT pour connaitre la position d'un point ****/
/***********************************************************************************************************/
static Sint16 positionY(double lat) {
  return (Sint16)(SCREEN_HEIGHT * (merc_y(maxlat)-merc_y(lat))/(merc_y(maxlat)-merc_y(minlat)));
}


/***********************************************************************************************************/
/*DrawArea : dessine tout ce qui est représenté sur la carte par un chemin fermé ***************************/
/***********************************************************************************************************/
void DrawArea(osm_way* way,int relation) {

  if(way->draw) return ;
  way->draw = 1 ;
  int node;

  Sint16* coordx = (Sint16*) malloc((way->n_node-1)*sizeof(Sint16));
  Sint16* coordy = (Sint16*) malloc((way->n_node-1)*sizeof(Sint16));

  for(node = 0; node<way->n_node-1; node++){
    coordy[node] = positionY(way->nodes[node]->lat);
    coordx[node] = positionX(way->nodes[node]->lon);

  }

  int8_t color_area_r = (uint8_t)169;
  uint8_t color_area_b = (uint8_t)169 ;
  uint8_t color_area_g = (uint8_t)169 ;

  uint32_t fill = 0 ;

  int i = 0 ;

  for(;i<way->n_tag;i++)
  {

    if(!strcmp((way->tags[i]).key,"waterway" )||!strcmp((way->tags[i]).key,"natural" ))
    {
      char key[20];
      strcpy(key,(way->tags[i]).value);
      color_area_r = (uint8_t)WATERWAY_COLOR_R ;
      color_area_b = (uint8_t)WATERWAY_COLOR_B ;
      color_area_g = (uint8_t)WATERWAY_COLOR_G;
      fill= 1 ;
      break;
    }
		if(!strcmp((way->tags[i]).key,"barrier" ))
    {
      relation=0;
      break;
    }
		if(!strcmp((way->tags[i]).key,"bridge" ))
    {
			fill = 1 ;
    }

		if(!strcmp((way->tags[i]).key,"natural"))
    {
        color_area_r = (uint8_t)WATERWAY_COLOR_R ;
        color_area_b = (uint8_t)WATERWAY_COLOR_B ;
        color_area_g = (uint8_t)WATERWAY_COLOR_G;
				fill = 1 ;
				char key[20];
	      strcpy(key,(way->tags[i]).value);

				if(!strcmp(key,(const char *)"wood") || !strcmp(key,(const char *)"wood") )
	      {

	        color_area_r = (uint8_t)NATURAL_WOOD_COLOR_R ;
	        color_area_g = (uint8_t)NATURAL_WOOD_COLOR_G ;
	        color_area_b =(uint8_t) NATURAL_WOOD_COLOR_B ;
					fill = 1;
	        break;

	      }

				if(!strcmp(key,(const char *)"trie_node"))
	      {

	        color_area_r = (uint8_t)NATURAL_WOOD_COLOR_R ;
	        color_area_g = (uint8_t)NATURAL_WOOD_COLOR_G ;
	        color_area_b =(uint8_t) NATURAL_WOOD_COLOR_B ;
					fill = 1 ;

	        break;
				}

				if(!strcmp(key,(const char *)"coastline")&& !coast)
	      {

	        color_area_r = (uint8_t)BACKGROUND_COLOR_R ;
	        color_area_g = (uint8_t)BACKGROUND_COLOR_G ;
	        color_area_b =(uint8_t) BACKGROUND_COLOR_B ;
					fill = 1 ;
					coast = 1 ;

	        break;
				}

				if(!strcmp(key,(const char *)"grassland"))
	      {

	        color_area_r = (uint8_t)NATURAL_GRASSLAND_COLOR_R ;
	        color_area_g = (uint8_t)NATURAL_GRASSLAND_COLOR_G ;
	        color_area_b =(uint8_t) NATURAL_GRASSLAND_COLOR_B ;
					fill = 1 ;

	        break;
	      }

				if(!strcmp(key,(const char *)"beach"))
				{

				color_area_r = (uint8_t)NATURAL_BEACH_COLOR_R ;
				color_area_g = (uint8_t)NATURAL_BEACH_COLOR_G ;
				color_area_b =(uint8_t) NATURAL_BEACH_COLOR_B ;
				fill = 1 ;

				break;

				}

				if(!strcmp(key,(const char *)"health")||!strcmp(key,(const char *)"fell")||!strcmp(key,(const char *)"sand"))
	      {

	        color_area_r = (uint8_t)NATURAL_HEALTH_COLOR_R ;
	        color_area_g = (uint8_t)NATURAL_HEALTH_COLOR_G ;
	        color_area_b =(uint8_t) NATURAL_HEALTH_COLOR_B ;
					fill = 1 ;

					break;

	      }

				if(!strcmp(key,(const char *)"scree")||!strcmp(key,(const char *)"bare-rock")||!strcmp(key,(const char *)"shingle")
						||!strcmp(key,(const char *)"mud"))
	      {

	        color_area_r = (uint8_t)NATURAL_SCREE_COLOR_R ;
	        color_area_g = (uint8_t)NATURAL_SCREE_COLOR_G ;
	        color_area_b =(uint8_t) NATURAL_SCREE_COLOR_B ;
					fill = 1 ;

	        break;

	      }
      break ;
    }

    if(!strcmp( (way->tags[i]).key,"leisure" ) )
    {

      char key[20];
      strcpy(key,(way->tags[i]).value);

      if(!strcmp(key,(const char *)"park"))
      {
        color_area_r = (uint8_t)LEISURE_PARK_COLOR_R ;
        color_area_b = (uint8_t)LEISURE_PARK_COLOR_B ;
        color_area_g = (uint8_t)LEISURE_PARK_COLOR_G ;
        fill = 1 ;
        break;
      }

      if(!strcmp(key,(const char *)"pitch"))
      {
        color_area_r = (uint8_t)LEISURE_PITCH_COLOR_R ;
        color_area_b = (uint8_t)LEISURE_PITCH_COLOR_B ;
        color_area_g = (uint8_t)LEISURE_PITCH_COLOR_G ;
        fill = 1 ;
        break;
      }

      if(!strcmp(key,(const char *)"sports_centre")||!strcmp(key,(const char *)"stadium"))
      {
        color_area_r = (uint8_t)LEISURE_STADIUM_COLOR_R ;
        color_area_b = (uint8_t)LEISURE_STADIUM_COLOR_B ;
        color_area_g = (uint8_t)LEISURE_STADIUM_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"track"))
      {
        color_area_r = (uint8_t)LEISURE_TRACK_COLOR_R ;
        color_area_b = (uint8_t)LEISURE_TRACK_COLOR_B ;
        color_area_g = (uint8_t)LEISURE_TRACK_COLOR_G ;
        fill = 1 ;
        break;
      }
      color_area_r = (uint8_t)LEISURE_DEFAULT_COLOR_R ;
      color_area_b = (uint8_t)LEISURE_DEFAULT_COLOR_B ;
      color_area_g = (uint8_t)LEISURE_DEFAULT_COLOR_G ;
      fill = 1 ;
      break ;
    }
    if(!strcmp( (way->tags[i]).key,"landuse" ) )
    {
      char key[20];
      strcpy(key,(way->tags[i]).value);
      if(!strcmp(key,(const char *)"allotments"))
      {
        color_area_r = (uint8_t)LANDUSE_ALLOTMENTS_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_ALLOTMENTS_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_ALLOTMENTS_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"basin"))
      {
        color_area_r = (uint8_t)LANDUSE_BASIN_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_BASIN_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_BASIN_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"brownfields"))
      {
        color_area_r = (uint8_t)LANDUSE_BROWN_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_BROWN_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_BROWN_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"cemetery"))
      {
        color_area_r = (uint8_t)LANDUSE_CEMETERY_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_CEMETERY_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_CEMETERY_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"commercial"))
      {
        color_area_r = (uint8_t)LANDUSE_COM_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_COM_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_COM_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"conservation"))
      {
        color_area_r = (uint8_t)LANDUSE_CONSERVATION_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_CONSERVATION_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_CONSERVATION_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"construction"))
      {
        color_area_r = (uint8_t)LANDUSE_CONST_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_CONST_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_CONST_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"farmland"))
      {
        color_area_r = (uint8_t)LANDUSE_FARM_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_FARM_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_FARM_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"farmyard"))
      {
        color_area_r = (uint8_t)LANDUSE_FARMYARD_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_FARMYARD_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_FARMYARD_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"forest"))
      {
        color_area_r = (uint8_t)LANDUSE_FOREST_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_FOREST_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_FOREST_COLOR_G ;
        fill = 1 ;
        break;
      }

      if(!strcmp(key,(const char *)"garages"))
      {
        color_area_r = (uint8_t)LANDUSE_GARAGES_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_GARAGES_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_GARAGES_COLOR_G ;
        fill = 1 ;
        break;
      }

      if(!strcmp(key,(const char *)"grass"))
      {
        color_area_r = (uint8_t)LANDUSE_GRASS_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_GRASS_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_GRASS_COLOR_G ;
        fill = 1 ;
        break;
      }

      if(!strcmp(key,(const char *)"greenfield"))
      {
        color_area_r = (uint8_t)LANDUSE_GREENFIELD_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_GREENFIELD_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_GREENFIELD_COLOR_G ;
        fill = 1 ;
        break;
      }

      if(!strcmp(key,(const char *)"greenhouse_horticulture"))
      {
        color_area_r = (uint8_t)LANDUSE_GREENHOUSE_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_GREENFIELD_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_GREENHOUSE_COLOR_G ;
        fill = 1 ;
        break;
      }

      if(!strcmp(key,(const char *)"industrial"))
      {
        color_area_r = (uint8_t)LANDUSE_INDUSTRIAL_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_INDUSTRIAL_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_INDUSTRIAL_COLOR_G ;
        fill = 1 ;
        break;
      }

      if(!strcmp(key,(const char *)"landfill"))
      {
        color_area_r = (uint8_t)LANDUSE_LANDFILL_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_LANDFILL_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_LANDFILL_COLOR_G ;
        fill = 1 ;
        break;
      }

      if(!strcmp(key,(const char *)"meadow"))
      {
        color_area_r = (uint8_t)LANDUSE_MEADOW_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_MEADOW_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_MEADOW_COLOR_G ;
        fill = 1 ;
        break;
      }

      if(!strcmp(key,(const char *)"military"))
      {
        color_area_r = (uint8_t)LANDUSE_MILIT_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_MILIT_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_MILIT_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"orchard"))
      {
        color_area_r = (uint8_t)LANDUSE_ORCHARD_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_ORCHARD_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_ORCHARD_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"quarry"))
      {
        color_area_r = (uint8_t)LANDUSE_QUARRY_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_QUARRY_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_QUARRY_COLOR_G ;
        fill = 1 ;
        break;
      }if(!strcmp(key,(const char *)"railway"))
      {
        color_area_r = (uint8_t)LANDUSE_RAILWAY_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_RAILWAY_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_RAILWAY_COLOR_G ;
        fill = 1 ;
        break;
      }if(!strcmp(key,(const char *)"recreation_ground"))
      {
        color_area_r = (uint8_t)LANDUSE_REACTION_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_REACTION_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_REACTION_COLOR_G ;
        fill = 1 ;
        break;
      }if(!strcmp(key,(const char *)"reservoir"))
      {
        color_area_r = (uint8_t)LANDUSE_RESERVOIR_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_RESERVOIR_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_RESERVOIR_COLOR_G ;
        fill = 1 ;
        break;
      }
      if(!strcmp(key,(const char *)"retail"))
      {
        color_area_r = (uint8_t)LANDUSE_RETAIL_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_RETAIL_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_RETAIL_COLOR_G ;
        fill = 1 ;
        break;
      }if(!strcmp(key,(const char *)"village_green"))
      {
        color_area_r = (uint8_t)LANDUSE_VILLGREEN_COLOR_R ;
        color_area_b = (uint8_t)LANDUSE_VILLGREEN_COLOR_B ;
        color_area_g = (uint8_t)LANDUSE_VILLGREEN_COLOR_G ;
        fill = 1 ;
        break;
      }
      color_area_r = (uint8_t)LANDUSE_COLOR_R ;
      color_area_b = (uint8_t)LANDUSE_COLOR_B ;
      color_area_g = (uint8_t)LANDUSE_COLOR_G ;
      break;
    }
    if(!strcmp((way->tags[i]).key,"building" ))
    {

      fill = 1 ;
      char key[20];
      strcpy(key,(way->tags[i]).value);
      color_area_r = (uint8_t)BUILDING_AREA_COLOR_R ;
      color_area_b = (uint8_t)BUILDING_AREA_COLOR_B ;
      color_area_g = (uint8_t)BUILDING_AREA_COLOR_G ;
      break;
    }
    if(!strcmp((way->tags[i]).key,"area" ))
    {
      color_area_g = (uint8_t)240 ;
      color_area_b = (uint8_t)240 ;
      color_area_r = (uint8_t)240 ;
      fill = 1 ;

      break;
    }

  }

  if(fill)
    filledPolygonRGBA(mapRenderer, coordx, coordy, way->n_node-1,color_area_r,color_area_g,color_area_b,255);
  else{
      if(relation)
        filledPolygonRGBA(mapRenderer, coordx, coordy, way->n_node-1,232,232,232,255);

    }
  aapolygonRGBA(mapRenderer, coordx, coordy, way->n_node-1,119,136,153,255);

  free(coordy);
  free(coordx);

}

/**********************************************************************/
/*DrawWay : dessine les chemins ***************************************/
/**********************************************************************/
void DrawWay(osm_way* way){

  if(way->draw) return ;
  way->draw = 1;
  int dotted_line = 0;

  uint8_t color_way_r = (uint8_t)DEFAULT_WAY_COLOR_RED ;
  uint8_t color_way_g =(uint8_t) DEFAULT_WAY_COLOR_GREEN;
  uint8_t color_way_b = (uint8_t)DEFAULT_WAY_COLOR_BLUE ;
  uint32_t width_way = DEFAULT_WIDTH_WAY ;

  int i = 0 ;

  for(;i<way->n_tag;i++)
  {

    if(!strcmp((way->tags[i]).key,"waterway"))
    {
      char key[30];
      strcpy(key,(way->tags[i]).value);
      color_way_r = (uint8_t)WATERWAY_COLOR_R ;
      color_way_b = (uint8_t)WATERWAY_COLOR_B ;
      color_way_g = (uint8_t)WATERWAY_COLOR_G;
      break;

    }

    if(!strcmp((way->tags[i]).key,"natural"))
    {
        color_way_r = (uint8_t)WATERWAY_COLOR_R ;
        color_way_b = (uint8_t)WATERWAY_COLOR_B ;
        color_way_g = (uint8_t)WATERWAY_COLOR_G;
				char key[20];
	      strcpy(key,(way->tags[i]).value);

				if(!strcmp(key,(const char *)"wood") ||!strcmp(key,(const char *)"wood") )
	      {

	        color_way_r = (uint8_t)NATURAL_WOOD_COLOR_R ;
	        color_way_g = (uint8_t)NATURAL_WOOD_COLOR_G ;
	        color_way_b =(uint8_t) NATURAL_WOOD_COLOR_B ;

	        break;

	      }

				if(!strcmp(key,(const char *)"trie_node"))
	      {

	        color_way_r = (uint8_t)NATURAL_WOOD_COLOR_R ;
	        color_way_g = (uint8_t)NATURAL_WOOD_COLOR_G ;
	        color_way_b =(uint8_t) NATURAL_WOOD_COLOR_B ;
					width_way = NATURAL_TREE_NODE_WIDTH_WAY;
	        break;

	      }
				if(!strcmp(key,(const char *)"coastline") && !coast)
	      {
					int node;

				  Sint16* coordx = (Sint16*) malloc((way->n_node-1)*sizeof(Sint16));
				  Sint16* coordy = (Sint16*) malloc((way->n_node-1)*sizeof(Sint16));

				  for(node = 0; node<way->n_node-1; node++){
				    coordy[node] = positionY(way->nodes[node]->lat);
				    coordx[node] = positionX(way->nodes[node]->lon);

				  }

					filledPolygonRGBA(mapRenderer, coordx, coordy, way->n_node-1,BACKGROUND_COLOR_R,BACKGROUND_COLOR_G,BACKGROUND_COLOR_B,255);
	        coast = 1 ;
	        break;

	      }

				if(!strcmp(key,(const char *)"grassland"))
	      {

	        color_way_r = (uint8_t)NATURAL_GRASSLAND_COLOR_R ;
	        color_way_g = (uint8_t)NATURAL_GRASSLAND_COLOR_G ;
	        color_way_b =(uint8_t) NATURAL_GRASSLAND_COLOR_B ;

	        break;
	      }
				if(!strcmp(key,(const char *)"beach"))
				{

				color_way_r = (uint8_t)NATURAL_BEACH_COLOR_R ;
				color_way_g = (uint8_t)NATURAL_BEACH_COLOR_G ;
				color_way_b =(uint8_t) NATURAL_BEACH_COLOR_B ;

				break;

				}

				if(!strcmp(key,(const char *)"health")||!strcmp(key,(const char *)"fell")||!strcmp(key,(const char *)"sand"))
	      {

	        color_way_r = (uint8_t)NATURAL_HEALTH_COLOR_R ;
	        color_way_g = (uint8_t)NATURAL_HEALTH_COLOR_G ;
	        color_way_b =(uint8_t) NATURAL_HEALTH_COLOR_B ;
	        break;

	      }

				if(!strcmp(key,(const char *)"scree")||!strcmp(key,(const char *)"bare-rock")||!strcmp(key,(const char *)"shingle")
						||!strcmp(key,(const char *)"mud"))
	      {

	        color_way_r = (uint8_t)NATURAL_SCREE_COLOR_R ;
	        color_way_g = (uint8_t)NATURAL_SCREE_COLOR_G ;
	        color_way_b =(uint8_t) NATURAL_SCREE_COLOR_B ;
	        break;

	      }
      break ;
    }

    if(!strcmp((way->tags[i]).key,"highway" ))
    {
      char key[20];
      strcpy(key,(way->tags[i]).value);

			#ifdef DEBUG
			printf("%s\n",key );
			#endif

      if(!strcmp(key,(const char *)"steps"))
      {
        width_way = STEP_WAY_WIDTH ;
        color_way_r = (uint8_t)STEP_WAY_COLOR_R ;
        color_way_g = (uint8_t)STEP_WAY_COLOR_G ;
        color_way_b =(uint8_t) STEP_WAY_COLOR_B ;
        break;
     }
      if(!strcmp(key,(const char *)"track"))
      {
        width_way = TRACK_WAY_WIDTH ;
        color_way_r = (uint8_t)TRACK_WAY_COLOR_R ;
        color_way_g = (uint8_t)TRACK_WAY_COLOR_G ;
        color_way_b =(uint8_t) TRACK_WAY_COLOR_B ;
      }
      if(!strcmp(key,(const char *)"motorway"))
      {
        width_way = MOTOR_WAY_WIDTH ;
        color_way_r = (uint8_t)MOTOR_WAY_COLOR_R ;
        color_way_g = (uint8_t)MOTOR_WAY_COLOR_G ;
        color_way_b =(uint8_t) MOTOR_WAY_COLOR_B ;
      }
      if(!strcmp(key,(const char *)"motorway_link"))
      {
        width_way = MOTOR_WAY_LINK_WIDTH;
        color_way_r = (uint8_t)MOTOR_WAY_COLOR_R ;
        color_way_g = (uint8_t)MOTOR_WAY_COLOR_G ;
        color_way_b =(uint8_t) MOTOR_WAY_COLOR_B ;
      }
      if(!strcmp(key,(const char *)"trunk"))
      {
        width_way = TRUNK_WAY_WIDTH ;
        color_way_r = (uint8_t)TRUNK_WAY_COLOR_R ;
        color_way_g = (uint8_t)TRUNK_WAY_COLOR_G ;
        color_way_b = (uint8_t)TRUNK_WAY_COLOR_B ;
      }

			if(!strcmp(key,(const char *)"living_street"))
      {
        width_way = LIVING_STREET_WIDTH ;
        color_way_r = (uint8_t)LIVING_STREET_COLOR_R ;
        color_way_g = (uint8_t)LIVING_STREET_COLOR_G;
        color_way_b = (uint8_t)LIVING_STREET_COLOR_B;
      }
      if(!strcmp(key,(const char *)"trunk_link"))
      {
        width_way = TRUNK_WAY_LINK_WIDTH ;
        color_way_r = (uint8_t)TRUNK_WAY_COLOR_R ;
       color_way_g = (uint8_t)TRUNK_WAY_COLOR_G ;
        color_way_b = (uint8_t)TRUNK_WAY_COLOR_B ;
      }
      if(!strcmp(key,(const char *)"pedestrian"))
      {
        width_way = PEDESTRIAN_WAY_WIDTH ;
        color_way_r = (uint8_t)PEDESTRIAN_WAY_COLOR_R ;
        color_way_g = (uint8_t)PEDESTRIAN_WAY_COLOR_G ;
        color_way_b = (uint8_t)PEDESTRIAN_WAY_COLOR_B ;
      }
      if(!strcmp(key,(const char *)"footway")||!strcmp(key,(const char *)"path"))
      {
        width_way = FOOT_WAY_WIDTH ;

        dotted_line = 1;

        color_way_r = (uint8_t)FOOT_WAY_COLOR_R ;
        color_way_g = (uint8_t)FOOT_WAY_COLOR_G ;
        color_way_b = (uint8_t)FOOT_WAY_COLOR_B ;
      }
      if(!strcmp(key,(const char *)"primary"))
      {
        width_way = PRIMARY_WAY_WIDTH ;
        color_way_r = (uint8_t)PRIMARY_WAY_COLOR_R ;
        color_way_g = (uint8_t)PRIMARY_WAY_COLOR_G ;
        color_way_b = (uint8_t)PRIMARY_WAY_COLOR_B ;
      }
      if(!strcmp(key,(const char *)"primary_link"))
      {
        width_way = PRIMARY_WAY_LINK_WIDTH ;
        color_way_r = (uint8_t)PRIMARY_WAY_COLOR_R ;
        color_way_g = (uint8_t)PRIMARY_WAY_COLOR_G ;
        color_way_b = (uint8_t)PRIMARY_WAY_COLOR_B ;
      }
      if(!strcmp(key,(const char *)"secondary"))
      {
        width_way = SECONDARY_WAY_WIDTH ;
        color_way_r = (uint8_t)SECONDARY_WAY_COLOR_R ;
        color_way_g = (uint8_t)SECONDARY_WAY_COLOR_G ;
        color_way_b = (uint8_t)SECONDARY_WAY_COLOR_B ;
      }
      if(!strcmp(key,(const char *)"tertiary"))
      {
        width_way = TERTIARY_WAY_WIDTH ;

        color_way_r = (uint8_t)TERTIARY_WAY_COLOR_R ;
        color_way_g = (uint8_t)TERTIARY_WAY_COLOR_G ;
        color_way_b = (uint8_t)TERTIARY_WAY_COLOR_B ;
      }
      if(!strcmp(key,(const char *)"unclassified"))
      {
        width_way = UNCLASSIFIED_WAY_WIDTH ;
      }
      if(!strcmp(key,(const char *)"residential"))
      {
        width_way = RESIDENTIAL_WAY_WIDTH ;
      }
      if(!strcmp(key,(const char *)"cycleway"))
      {

        width_way = CYCLE_WAY_WIDTH ;
        color_way_r = (uint8_t)CYCLE_WAY_COLOR_R;
        color_way_g = (uint8_t)CYCLE_WAY_COLOR_G ;
        color_way_b = (uint8_t)CYCLE_WAY_COLOR_B ;

      }
      break ;
    }
  }
  int node;
  if(way->n_node > 0)

    for(node = 0; node<way->n_node-1; node++) {
      thickLineRGBA(mapRenderer,
		  positionX(way->nodes[node]->lon), positionY(way->nodes[node]->lat),
		  positionX(way->nodes[node+1]->lon), positionY(way->nodes[node+1]->lat),
		  width_way,color_way_r,color_way_g,color_way_b,255);

    }
  return;

}

void draw_relation(osm_relation* relation)
{

  /*i_node, i_way, i_relation permettent de parcourir les tableau contenant respectivement*/
  /*les nodes, ways et relation de la relation  de la relation passée en paramètre */
  int i_node = 0 , i_way = 0 , i_relation = 0 ;

  while( (i_node<relation->n_node) || (i_way<relation->n_way) || (i_relation <relation->n_relation))
  {
    if(i_relation<relation->n_relation)
    {
      #ifdef DEBUG
      printf("relation id : %lld\n",relation->id );
      printf("relation nombre de noeuds : %d\n",relation->n_node );
      printf("relation nombdre de relation : %d\n",relation->n_relation );
      printf("relation nombre de way : %d\n",relation->n_way);
      printf("relation id : %lld\n",relation->relations[i_relation].relation->n_relation );
      #endif
      i_relation ++;
    }

    if(i_node<relation->n_node)
    {
        /*A suivre */
        i_node++;
    }

    if(i_way<relation->n_way)
    {
      if(relation->ways[i_way].way)
      if(!strcmp(relation->ways[i_way].role,"outer")){


        osm_way* w = relation->ways[i_way].way;
        if(w->n_node>2 && w->nodes[0]->id == w->nodes[w->n_node-1]->id) {
            DrawArea(w,1);
        }
        else {
            DrawWay(w);
        }

        w->draw = 1 ;
        }

        i_way++;
    }

  }
  i_node =0 ;
  i_way =0 ;
  i_relation = 0;

  while( (i_node<relation->n_node) || (i_way<relation->n_way) || (i_relation <relation->n_relation))
  {
    if(i_node<relation->n_node)
    {
        /*A suivre */
        i_node++;
    }

    if(i_way<relation->n_way)
    {
      if(relation->ways[i_way].way)
      if(!strcmp(relation->ways[i_way].role,"inner")){


        osm_way* w = relation->ways[i_way].way;
        if(w->n_node>2 && w->nodes[0]->id == w->nodes[w->n_node-1]->id)
            DrawArea(w,1);
        else
            DrawWay(w);

        w->draw = 1 ;
      }
        i_way++;

    }
    if(i_relation<relation->n_relation)
    {
      i_relation ++;
    }

  }
  relation->draw=1 ;
}

void parcoursDrawWay(int way, int taille_tab, osm_way** ways)
{

	for(way=0 ; way<taille_tab; way++)
	{
		osm_way * w = ways[way];
		if(w->nodes[0]->id == w->nodes[w->n_node-1]->id)
		{
			DrawArea(w,1);
		}

	}

	for(way=0 ; way<taille_tab;way++)
	{
		osm_way * w = ways[way];
		if(w->nodes[0]->id !=w->nodes[w->n_node-1]->id)
		{
				DrawWay(w);
		}

	}

}

/*********************************************************************************************/
/*DrawOsm : dessine une carte à partir des résultats du parsing d'un fichier xml *************/
/*********************************************************************************************/
void DrawOsm(osm* map){
  int way=0;
  int rel;
 //get Renderer
  mapRenderer = SDL_CreateRenderer(mapWindow, -1, SDL_RENDERER_ACCELERATED);

 if(map->coastline){

 		SDL_SetRenderDrawColor(mapRenderer,WATERWAY_COLOR_R,WATERWAY_COLOR_G,WATERWAY_COLOR_B,255);
 	}

 	else
 		SDL_SetRenderDrawColor(mapRenderer,BACKGROUND_COLOR_R,BACKGROUND_COLOR_G,BACKGROUND_COLOR_B,255);

	SDL_RenderClear(mapRenderer);

	/* Dessiner les  tags waterline et natural */
	parcoursDrawWay(way, map->n_water, map->waters);

	/* Dessiner les layers */
	parcoursDrawWay(way, map->n_layerMoins45, map->layerMoins45);
	parcoursDrawWay(way, map->n_layerMoins3, map->layerMoins3);
	parcoursDrawWay(way, map->n_layerMoins2, map->layerMoins2);
	parcoursDrawWay(way, map->n_layerMoins1, map->layerMoins1);
	parcoursDrawWay(way, map->n_layer0, map->layer0);
	parcoursDrawWay(way, map->n_layer1, map->layer1);
	parcoursDrawWay(way, map->n_layer2, map->layer2);
	parcoursDrawWay(way, map->n_layer3, map->layer3);
	parcoursDrawWay(way, map->n_layer4, map->layer4);
	parcoursDrawWay(way, map->n_layer5, map->layer5);

	/* Dessiner les relations */
  for(rel=0; rel<map->n_relation; rel++){

    draw_relation(&(map->relations[rel]));

  }

	/* Dessiner le reste */
	for(way=0; way<map->n_way; way++){
      osm_way* w = &map->ways[way];

      if(w->nodes[0]->id == w->nodes[w->n_node-1]->id)
      {
        DrawArea(w,0);
      }
  }

  for(way=0; way<map->n_way; way++){
    osm_way* w = &map->ways[way];
	  if( w->nodes[0]->id != w->nodes[w->n_node-1]->id)
	  {
	    DrawWay(w);
	  }
  }

  /* Dessiner le nom des rues */
  for(way=0; way<map->n_way; way++){
    osm_way* w = &map->ways[way];
    if( w->nodes[0]->id != w->nodes[w->n_node-1]->id)
    {
      drawWayName(w);
    }
  }

  SDL_RenderPresent(mapRenderer);
}

/****************************************************************************************************/
/*exportRender : exporte le rendu sous forme d'image BMP et la stocke dans le dossier fichiers_test */
/****************************************************************************************************/
void exportRender(char* docname) {
  SDL_Surface* render_image = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000); 

  if(render_image) { 
    // Lit les pixels à partir du render et les enregistre dans une surface
    SDL_RenderReadPixels(mapRenderer, NULL, SDL_GetWindowPixelFormat(mapWindow), render_image->pixels, render_image->pitch); 

    // Cree un fichier bmp à partir de la surface
    char* name_tokenized = strtok(docname, ".");
    if (name_tokenized == NULL) 
      fprintf(stderr, "Erreur : Le fichier n'est pas au bon format");
    SDL_SaveBMP(render_image, strcat(name_tokenized, ".bmp")); 

    // Detruit la surface 
    SDL_FreeSurface(render_image); 
  } 

}

/**********************************************************************/
/*drawWayName : dessine le nom des rues *******************************/
/**********************************************************************/
void drawWayName(osm_way* way) {

  char* nom_rue = "";
  int draw_name = 0;

  for(int i = 0; i < way->n_tag; i++) {
    if(!strcmp((way->tags[i]).key,"highway") && (!strcmp(way->tags[i].value,(const char *)"unclassified") || !strcmp(way->tags[i].value,(const char *)"residential"))) {
      draw_name = 1;
    }
    if (!strcmp(way->tags[i].key, "name") && draw_name == 1) {
      nom_rue = way->tags[i].value;
    }
  }

  stringRGBA(mapRenderer, (positionX(way->nodes[0]->lon) + positionX(way->nodes[way->n_node - 1]->lon))/2, 
          (positionY(way->nodes[0]->lat) + positionY(way->nodes[way->n_node - 1]->lat))/2, nom_rue, WAY_NAME_COLOR_R, WAY_NAME_COLOR_G, WAY_NAME_COLOR_B, 255);
}

/*****************************************************************************************************/
/*docRender : affiche une fenêtre avec une carte OpenStreetMap à partir d'un fichier xml *************/
/*****************************************************************************************************/
int docRender(char* docname, int export) {

  osm map;
  SDL_Event evt;

  if(parseDoc(docname, &map)<0) return ERROR_PARSING;

  // Initialize global variables
  minlat = map.bounds->minlat;
  maxlat = map.bounds->maxlat;
  minlon = map.bounds->minlon;
  maxlon = map.bounds->maxlon;

  // Determine window width and height
  double ratio = cos(M_PI*(minlat+maxlat)/360);
  if ((maxlat-minlat)<(maxlon-minlon)) {
    SCREEN_WIDTH = WINDOW_SIZE;
    SCREEN_HEIGHT = (int)(WINDOW_SIZE * (maxlat-minlat)/(ratio * (maxlon-minlon)));
  } else {
    SCREEN_WIDTH = (int)(WINDOW_SIZE * (maxlon-minlon) * ratio/(maxlat-minlat));
    SCREEN_HEIGHT = WINDOW_SIZE;
  }

  //Initialize SDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  if (TTF_Init() != 0){

	   SDL_Quit();

	  return 1;
  }

  //Create window
  mapWindow = SDL_CreateWindow("osmlib", SDL_WINDOWPOS_UNDEFINED,
			     SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
			     SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

  if( mapWindow == NULL ) {
    printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  DrawOsm(&map);

  if (export == 1)
    exportRender(docname);

  //Wait for close
  while(1) {
    while(SDL_PollEvent(&evt)) {
        if(evt.type == SDL_QUIT) {

  	SDL_DestroyRenderer( mapRenderer );

  	//Destroy window
  	SDL_DestroyWindow( mapWindow );
  	mapWindow = NULL;

  	//Quit SDL subsystems
  	SDL_Quit();


      return 1;
      }
    }
    }

  return 0;
}
