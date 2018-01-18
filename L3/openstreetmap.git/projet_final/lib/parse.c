#include "parse.h"
#include "utils.h"

extern int global_error;

/**********************************************************************/
/*parseBounds : récupère les attributs de l'élément bounds ************/
/**********************************************************************/
error parseBounds(xmlNodePtr cur,osm_bounds* bounds) {

    xmlChar* prop;

    prop = xmlGetProp(cur, (const xmlChar*)"minlat");
    if (!prop) {free(prop);global_error=NO_MINLAT_ATTRIBUT_DEFINED; return global_error;}
    bounds->minlat = atof((const char*) prop);
    free(prop);

    prop = xmlGetProp(cur, (const xmlChar*)"maxlat");
    if (!prop) {free(prop); global_error=NO_MAXLAT_ATTRIBUT_DEFINED; return global_error;}
    bounds->maxlat = atof((const char*) prop);
    free(prop);

    prop = xmlGetProp(cur, (const xmlChar*)"minlon");
    if (!prop) {free(prop); global_error=NO_MINLON_ATTRIBUT_DEFINED; return global_error;}
    bounds->minlon = atof((const char*) prop);
    free(prop);

    prop = xmlGetProp(cur, (const xmlChar*)"maxlon");
    if (!prop) {free(prop); global_error=NO_MAXLON_ATTRIBUT_DEFINED ;return global_error;}
    bounds->maxlon = atof((const char*) prop);
    free(prop);

    return 0;

}

/**********************************************************************/
/*cat : Sert à récupérer les ways de type coastline et natural ********/
/**********************************************************************/
void cat(osm *os) {

  int i = 0 ;
  int y = 0 ;
  int moins1 = 0 ;
  int moins2 = 0 ;
  int moins3 = 0 ;
  int moins45 = 0 ;
  int zero=0;
  int plus1 = 0 ;
  int plus2 = 0 ;
  int plus3 = 0;
  int plus4 = 0 ;
  int plus5=0 ;

  os->waters = (osm_way**)malloc(os->n_water*sizeof(osm_way*));
  os->layerMoins45 = (osm_way**)malloc(os->n_layerMoins45*sizeof(osm_way*));
  os->layerMoins1 = (osm_way**)malloc(os->n_layerMoins1*sizeof(osm_way*));
  os->layerMoins2 = (osm_way**)malloc(os->n_layerMoins2*sizeof(osm_way*));
  os->layerMoins3 = (osm_way**)malloc(os->n_layerMoins3*sizeof(osm_way*));
  os->layer0 = (osm_way**)malloc(os->n_layer0*sizeof(osm_way*));
  os->layer5 = (osm_way**)malloc(os->n_layer5*sizeof(osm_way*));
  os->layer4 = (osm_way**)malloc(os->n_layer4*sizeof(osm_way*));
  os->layer1 =(osm_way**)malloc(os->n_layer1*sizeof(osm_way*));
  os->layer2 =(osm_way**)malloc(os->n_layer2*sizeof(osm_way*));
  os->layer3 =(osm_way**)malloc(os->n_layer3*sizeof(osm_way*));

  for(;i<os->n_way;i++)
  {
    if(os->ways[i].water)
    {
        os->waters[y]=&(os->ways[i]) ;
        y++;
    }
    if(os->ways[i].haslayer)
    {

      if(os->ways[i].layer==-1 )
      {
        os->layerMoins1[moins1]=&(os->ways[i]) ;
        moins1++;
      }
      if(os->ways[i].layer==-2)
      {
        os->layerMoins2[moins2]=&(os->ways[i]) ;
        moins2++;
      }
      if(os->ways[i].layer==-3)
      {
        os->layerMoins3[moins3]=&(os->ways[i]) ;
        moins3++;
      }

      if(os->ways[i].layer==-4 || os->ways[i].layer==-5)
      {

        os->layerMoins45[moins45]=&(os->ways[i]) ;
        moins45++;
      }

      if(os->ways[i].layer==0)
      {
        os->layer0[zero]=&(os->ways[i]) ;
        zero++;
      }

      if(os->ways[i].layer==1 )
      {
        os->layer1[plus1]=&(os->ways[i]) ;
        plus1++;
      }
      if(os->ways[i].layer==2 )
      {
        os->layer2[plus2]=&(os->ways[i]) ;
        plus2++;
      }
      if(os->ways[i].layer==3)
      {
        os->layer3[plus3]=&(os->ways[i]) ;
        plus3++;
      }
      if(os->ways[i].layer==4)
      {
        os->layer4[plus4]=&(os->ways[i]) ;
        plus4++;
      }
      if(os->ways[i].layer==5)
      {
        os->layer5[plus5]=&(os->ways[i]) ;
        plus5++;
      }



    }

  }
  os->n_water =y ;
  os->n_layerMoins45 = moins45 ;
  os->n_layerMoins1 = moins1 ;
  os->n_layerMoins2 = moins2 ;
  os->n_layerMoins3 = moins3 ;
  os->n_layer0 = zero ;
  os->n_layer5 = plus5 ;
  os->n_layer4 = plus4 ;
  os->n_layer3 = plus3 ;
  os->n_layer2 = plus2 ;
  os->n_layer1 = plus1 ;


}

/**********************************************************************/
/*parseWay : récupère les attributs de l'élément way       ************/
/**********************************************************************/
error parseWay(xmlNodePtr cur, osm_way *way, osm* os) {
  xmlChar* prop;

  prop = xmlGetProp(cur, (const xmlChar*)"id");
  if (!prop) {free(prop); global_error= NO_ID_ATTRIBUT_DEFINED; return global_error;}
  way->id= atoll((const char*) prop);

  prop = xmlGetProp(cur, (const xmlChar*)"visible");
  if (prop)

  {
    way->visible= atoi((const char*) prop);

  }

  way->n_node = 0 ;

  way->nodes = NULL ;

  way->tags = NULL ;

  way->water = 0 ;
  way->haslayer=0;

  /* tag pour ne pas allouer plusieurs fois le tableau tags de la structure */
  int tag = 1;

  /* ref pour ne pas allouer plusieurs fois le tableau ref de la structure */
  int ref = 1;

  /* n_tag permet de contrôler les réallocations  du tableau tags*/
  int n_tag = LENGTH_TAG_ARRAY ;

  /* n_ref permet de contrôler les réallocations  du tableau ref*/
  int n_ref = LENGTH_REF_ARRAY ;

  /* i_tag et i_ref servent   de curseur  aux tableaux tags et ref*/
  int i_tag = 0 ;

  int i_ref = 0 ;

  xmlNodePtr curs = cur->xmlChildrenNode ;

  while(curs!= NULL)
  {

    if ( ( !xmlStrcmp(curs->name,(const xmlChar *) "tag" ) ) )
    {

      if(tag)
      {
        way->tags =(struct osm_tag*) malloc(LENGTH_TAG_ARRAY * sizeof( struct osm_tag));
        tag = 0 ;

      }


      prop = xmlGetProp(curs, (const xmlChar*)"k");
      if (!prop) {free(prop); global_error=NO_KEY_ATTRIBUT_DEFINED;}

      (way->tags[i_tag]).key =(char *)malloc(40*sizeof(char));
      strcpy((way->tags[i_tag]).key,(char *)prop);

      prop = xmlGetProp(curs, (const xmlChar*)"v");

      if (!prop) {free(prop); global_error=NO_VALUE_ATTRIBUT_DEFINED;}
      (way->tags[i_tag]).value =(char *)malloc(400*sizeof(char));
      strcpy((way->tags[i_tag]).value ,(char *)prop);

      if(!strcmp((way->tags[i_tag]).key,"natural") && !strcmp((way->tags[i_tag]).value,"coastline"))
      {
        os->coastline = 1 ;
        way->water = 1 ;

      }

      if(!strcmp((way->tags[i_tag]).key,"layer") )
      {
        way->haslayer = 1 ;
        way->layer = atoi((way->tags[i_tag]).value);

        if(way->layer==-1)
          os->n_layerMoins1++;
        if(way->layer==-2)
          os->n_layerMoins2++;
        if(way->layer==-3)
          os->n_layerMoins2++;
        if(way->layer==-4 || way->layer==-5 )
          os->n_layerMoins45++;
        if(way->layer==0)
          os->n_layer0++;
        if(way->layer==1)
          os->n_layer1++ ;
        if(way->layer==2)
          os->n_layer2++ ;
        if(way->layer==3)
          os->n_layer3++ ;
        if(way->layer==4)
          os->n_layer4++ ;
        if(way->layer==5 )
          os->n_layer5++ ;

      }

      if((!strcmp((way->tags[i_tag]).key,"natural") || !strcmp((way->tags[i_tag]).key,"waterway")) && (strcmp((way->tags[i_tag]).value,"water")!=0) )
      {

        os->n_water ++ ;
        way->water = 1 ;
     }
      i_tag++;

      /*On verifie si le tableau  tags de  la structure node est plein */
      /*, si oui on le réalloue ****************************************/

     if(i_tag==n_tag)
      {

        way->tags = (struct osm_tag* )realloc( way->tags,(LENGTH_TAG_ARRAY + n_tag)*sizeof(struct osm_tag) ) ;

        #ifdef DEBUG
          if(way->tags!=NULL)printf( "%s\n",way->tags[i_tag-1].value );
        #endif

        n_tag += LENGTH_TAG_ARRAY ;
      }

    }

    if ( ( !xmlStrcmp(curs->name,(const xmlChar *) "nd" ) ) )
    {

      if(ref)
      {
        way->nodes =(osm_node**) malloc(LENGTH_REF_ARRAY * sizeof(osm_node*));
        ref = 0 ;

      }
      prop = xmlGetProp(curs, (const xmlChar*)"ref");
      if (!prop) {free(prop); global_error= NO_REF_ATTRIBUT_DEFINED; return global_error;}


      way->nodes[i_ref]=getNodeById(os,atoll((const char*) prop));

      if(way->nodes[i_ref]!=NULL)
        i_ref++;

      /*On verifie si le tableau  ref de  la structure node est plein */
      /*, si oui on le réalloue ****************************************/

      if(i_ref==n_ref)
      {

        way->nodes = ( osm_node **)realloc( way->nodes,(LENGTH_REF_ARRAY + n_ref)*sizeof(osm_node *) ) ;

        n_ref += LENGTH_REF_ARRAY ;

      }

    }

    curs = curs->next ;

  }

  way->n_tag = 0 ;
  if(way->tags!=NULL)
  {

    way->n_tag = i_tag ;

  }

  if(way->nodes!=NULL)
  {

    way->n_node = i_ref ;

  }
  way->draw = 0 ;

  return 0 ;

}

/**********************************************************************************/
/*parseRelation: récupère les éléments relation du document ***********************/
/***********************************************************************************/
error parseRelation(xmlNodePtr cur, osm_relation *relation,osm* os) {
  xmlChar* prop;

  prop = xmlGetProp(cur, (const xmlChar*)"id");
  if (!prop) {free(prop); global_error= NO_ID_ATTRIBUT_DEFINED; return global_error;}
  relation->id= atoll((const char*) prop);

  prop = xmlGetProp(cur, (const xmlChar*)"visible");
  if (prop)
  {
    relation->visible= atoi((const char*) prop);

  }

  /*Initilialisation des champs de la structure*/
  relation->n_node = 0 ;
  relation->nodes = NULL ;
  relation->n_way = 0 ;
  relation->ways = NULL ;
  relation->n_relation = 0 ;
  relation->relations = NULL ;
  relation->n_tag = 0 ;
  relation->tags = NULL ;
  relation->draw = 0 ;

  /* tag pour ne pas allouer plusieurs fois le tableau tags de la structure */
  int tag = 1;

  /* node pour ne pas allouer plusieurs fois le tableau nodes de la structure */
  int node = 1;

  /* rel pour ne pas allouer plusieurs fois le tableau relations de la structure */
  int rel = 1;

  /* way pour ne pas allouer plusieurs fois le tableau ways de la structure */
  int way= 1;

  /* n_tag permet de contrôler les réallocations  du tableau tags*/

  int n_tag = LENGTH_TAG_ARRAY ;

  /* n_ref permet de contrôler les réallocations  du tableau nodes*/

  int n_ref = LENGTH_MEMBER_NODE;

  /*n_rel permet de contrôler les réallocations du tableau relations */

  int n_rel = LENGTH_MEMBER_REL ;

  /*n_way permet de contrôler les réallocations du tableau ways */

  int n_way = LENGTH_MEMBER_WAY;

  /* i_tag, i_rel et i_ref servent   de curseur  aux tableaux tags et ref*/

  int i_tag = 0 ;

  int i_ref = 0 ;

  int i_rel = 0 ;

  int i_way = 0 ;

  xmlNodePtr curs = cur->xmlChildrenNode ;

  int trie_node = 0 ;

  while(curs!=NULL)
  {

    if ( ( !xmlStrcmp(curs->name,(const xmlChar *) "tag" ) ) )
    {

      if(tag)
      {
        relation->tags =(struct osm_tag*) malloc(LENGTH_TAG_ARRAY * sizeof( struct osm_tag));
        tag = 0 ;

      }
      prop = xmlGetProp(curs, (const xmlChar*)"k");
      if (!prop) {free(prop); global_error=NO_KEY_ATTRIBUT_DEFINED;}

      (relation->tags[i_tag]).key =(char *)malloc(40*sizeof(char));
      strcpy((relation->tags[i_tag]).key,(char *)prop);

      prop = xmlGetProp(curs, (const xmlChar*)"v");
      if (!prop) {free(prop); global_error=NO_VALUE_ATTRIBUT_DEFINED;}

      (relation->tags[i_tag]).value =(char *)malloc(400*sizeof(char));
      strcpy((relation->tags[i_tag]).value ,(char *)prop);

      i_tag++;

      /*On verifie si le tableau  tags de  la structure node est plein */
      /*, si oui on le réalloue ****************************************/

      if(i_tag==n_tag)
      {

        relation->tags = (struct osm_tag* )realloc( relation->tags,(LENGTH_TAG_ARRAY + n_tag)*sizeof(struct osm_tag) ) ;

        #ifdef DEBUG
          if(relation->tags!=NULL)printf( "%s\n",relation->tags[i_tag-1].value );
        #endif

        n_tag += LENGTH_TAG_ARRAY ;

      }

    }

    if(!xmlStrcmp(curs->name, (const xmlChar*)"member"))
    {

      prop = xmlGetProp(curs, (const xmlChar*)"type");

      if(!xmlStrcmp(prop, (const xmlChar*)"node") )
      {
          if(!trie_node)
          {
              sort_node(os);
              trie_node = 1 ;
          }

          if(node)
          {
            relation->nodes = (member_node*)malloc(LENGTH_MEMBER_NODE*sizeof(member_node));
            node = 0 ;
          }

          prop = xmlGetProp(curs, (const xmlChar*)"ref");
          relation->nodes[i_ref].node = getNodeById(os,atoll((char*) prop));

          if(relation->nodes[i_ref].node)
          {
            prop = xmlGetProp(curs, (const xmlChar*)"role");
            relation->nodes[i_ref].role = (char *)malloc(50*sizeof(char));

            if(!strcmp("",(const char *)prop))
                          strcpy(relation->nodes[i_ref].role,"inner");
            else
                          strcpy(relation->nodes[i_ref].role,(char *)prop);

            i_ref++;

          }


          if(i_ref==n_ref)
          {
            relation->nodes = ( member_node*)realloc( relation->nodes,(LENGTH_MEMBER_NODE + n_ref)*sizeof(member_node ) ) ;

            n_ref += LENGTH_MEMBER_NODE ;
          }

        }

        if(!xmlStrcmp(prop, (const xmlChar*)"way"))
        {
          if(way)
          {
            relation->ways = (member_way*)malloc(LENGTH_MEMBER_WAY*sizeof(member_way));
            way = 0 ;
          }

          prop = xmlGetProp(curs, (const xmlChar*)"ref");
          relation->ways[i_way].way = getWayById(os,atoll((char*) prop));

          #ifdef DEBUG
          if(relation->ways[i_way].way==NULL)printf("NULL :%s\n",prop );
          printf("NON NULL %s\n", prop);
          #endif

          if(relation->ways[i_way].way)
          {

            prop = xmlGetProp(curs, (const xmlChar*)"role");
            relation->ways[i_way].role = (char *)malloc(40*sizeof(char));

            if(!strcmp("",(const char *)prop))
              strcpy(relation->ways[i_way].role,"inner");
            else
              strcpy(relation->ways[i_way].role,(char *)prop);

            i_way++;

          }

          if(i_way==n_way)
          {
            relation->ways = ( member_way*)realloc( relation->ways,(LENGTH_MEMBER_WAY + n_way)*sizeof(member_way) ) ;

            n_way += LENGTH_MEMBER_WAY ;

          }

        }

        if(!xmlStrcmp(prop, (const xmlChar*)"relation"))
        {
          if(rel)
          {
            relation->relations = (member_relation*)malloc(LENGTH_MEMBER_REL*sizeof(member_relation));
            rel = 0 ;
          }

          prop = xmlGetProp(curs, (const xmlChar*)"ref");
          relation->relations[i_rel].relation = getRelationById(os,atoll((char*) prop));

          if(relation->relations[i_rel].relation)
          {
            prop = xmlGetProp(curs, (const xmlChar*)"role");
            relation->relations[i_rel].role = (char *)malloc(40*sizeof(char));

            if(!strcmp("",(const char *)prop))
                strcpy(relation->relations[i_rel].role,"inner");
            else
                strcpy(relation->relations[i_rel].role,(char *)prop);

            i_rel++;

          }

          if(i_rel==n_rel)
          {
            relation->relations = ( member_relation*)realloc( relation->relations,(LENGTH_MEMBER_REL + n_rel)*sizeof(member_relation) ) ;

            n_rel += LENGTH_MEMBER_REL ;

          }

        }
    }

    curs = curs->next ;

  }
  #ifdef DEBUG
  printf("Nombre de relations %d\n",i_rel );
  #endif
  relation->n_node = i_ref;
  relation->n_tag = i_tag;
  relation->n_way = i_way ;
  relation->n_relation = i_rel ;

  return 0 ;

}

/**********************************************************************************/
/*parseNode: récupère les éléments node du document  ******************************/
/***********************************************************************************/
error parseNode(xmlNodePtr cur , osm_node *node) {

  xmlChar* prop;

  prop = xmlGetProp(cur, (const xmlChar*)"id");

  if (!prop) {free(prop); global_error=NO_ID_ATTRIBUT_DEFINED; return global_error;}
  node->id = atoll((const char*) prop);

  #ifdef DEBUG
  printf("ID noeud %s\n",prop );
  printf("%lld\n",node->id );printf("%lld\n",atoll((const char*) prop));
  #endif

  free(prop);

  prop = xmlGetProp(cur, (const xmlChar*)"lat");
  if (!prop) {free(prop); global_error=NO_LAT_ATTRIBUT_DEFINED; return global_error;}
  node->lat = atof((const char*) prop);
  free(prop);

  prop = xmlGetProp(cur, (const xmlChar*)"lon");
  if (!prop) {free(prop);global_error=NO_LON_ATTRIBUT_DEFINED; return global_error;}
  node->lon = atof((const char*) prop);
  free(prop);

  node->visible = xmlStrcmp(xmlGetProp(cur,(const xmlChar*)"visible"),(const xmlChar *)"true");


  node->tags = NULL ;
  node->draw = 0 ;
  /* tag pour ne pas allouer plusieurs fois le tableau tags de la structure */
  int tag = 1;

  /* n permet de contrôler les réallocations */
  int n = LENGTH_TAG_ARRAY ;

  /* i sert  de curseur */
  int i = 0 ;

  xmlNodePtr curs = cur->xmlChildrenNode ;

  while(curs!= NULL)
  {
    if ( ( !xmlStrcmp(curs->name,(const xmlChar *) "tag" ) ) )
    {
      if(tag)
      {

        node->tags =(struct osm_tag*) malloc(LENGTH_TAG_ARRAY * sizeof( struct osm_tag));
        tag = 0 ;

      }
      prop = xmlGetProp(curs, (const xmlChar*)"k");
      if (!prop) {free(prop); global_error=NO_KEY_ATTRIBUT_DEFINED;}
      (node->tags[i]).key =(char *)malloc(40*sizeof(char));
      strcpy((node->tags[i]).key,(char *)prop);

      prop = xmlGetProp(curs, (const xmlChar*)"v");
      if (!prop) {free(prop); global_error=NO_VALUE_ATTRIBUT_DEFINED;}
      (node->tags[i]).value =(char *)malloc(400*sizeof(char));
      strcpy((node->tags[i]).value ,(char *)prop);

      i++;

      /*On verifie si le tableau  tags de  la structure node est plein */
      /*, si oui on le réalloue ****************************************/
      if(i==n)
      {

        node->tags = ( struct osm_tag *)realloc( node->tags,(LENGTH_TAG_ARRAY + n)*sizeof( struct osm_tag) ) ;

        n += LENGTH_TAG_ARRAY ;

      }

    }

    curs = curs->next ;

  }

  /*Marquer la fin du tableau tags*/
  node->n_tag = 0 ;
  if(node->tags!=NULL)
  {

    node->n_tag = i ;

  }
  i=0;

  #ifdef DEBUG
  printf("\n\nid noeud %d\n",node->id );
  printf("nombre de tags %d\n",node->n_tag );

  for(;i<node->n_tag;i++)
  {
    printf("nombre de tags %d\n",node->n_tag );
    printf("i %d \n",i );
    printf("%s\n",node->tags[i].key );
    printf("%s\n",node->tags[i].value );
  }
  #endif

  return 0 ;

}

/********************************************************************************/
/* Parse le document donné en argument ******************************************/
/********************************************************************************/
error parseDoc(char * docname,osm *os) {

  xmlDocPtr doc ;

  xmlNodePtr cur ;

  doc = xmlParseFile(docname);

  if(doc==NULL) {
    return ERROR_PARSING ;
  }

  cur = xmlDocGetRootElement(doc);

  if(cur==NULL)
  {
    xmlFreeDoc(doc);
    return EMPTY_DOC ;
  }

  if ( xmlStrcmp(cur->name, (const xmlChar*)"osm") )
  {
    xmlFreeDoc(doc);
    return WRONG_FORMAT ;
  }

  cur = (cur->xmlChildrenNode)->next ;

  if ( ( xmlStrcmp((cur->name),(const xmlChar*)"bounds")))
  {
    xmlFreeDoc(doc);
    return WRONG_FORMAT ;
  }

  os->bounds = (osm_bounds*)malloc(sizeof(osm_bounds)) ;

  if(parseBounds(cur,os->bounds)<0) return global_error;

  cur = cur->next ;

  /* Tableau pour stocker les noeuds */
  osm_node* node_array = ( osm_node *)malloc(LENGTH_NODE_ARRAY*sizeof(osm_node));

  /* Tableau pour stocker les ways */
  osm_way* way_array = (osm_way *)malloc(LENGTH_WAY_ARRAY*sizeof(osm_way));

  /* Tableau pour stocker les relations */
  osm_relation* relation_array = (osm_relation*)malloc(LENGTH_RELATION_ARRAY*sizeof(osm_relation));


  /* n_node et et n_way permettent  de contrôler les réallocations */
  int n_node = LENGTH_NODE_ARRAY ;

  int n_way = LENGTH_WAY_ARRAY ;

  int n_rel = LENGTH_RELATION_ARRAY ;

  /* i_node sert  de curseur aux nodes */
  int i_node= 0 ;

  int i_way = 0 ;

  int i_rel = 0 ;

  /* Variable pour trier les noeuds et les ways ont une seule fois */
  int trie_node = 0 ;
  int trie_way = 0 ;

  os->coastline = 0;
  os->n_water = 0 ;
  os->n_layer0 = 0;
  os->n_layerMoins45 = 0 ;
  os->n_layerMoins1= 0 ;
  os->n_layerMoins2 = 0 ;
  os->n_layerMoins3 = 0 ;
  os->n_layer5 = 0 ;
  os->n_layer4 = 0 ;
  os->n_layer1 = 0 ;
  os->n_layer2 = 0 ;
  os->n_layer0 = 0 ;
  os->n_layer3 = 0 ;

  while(cur!=NULL)
  {
    if( ( !xmlStrcmp(cur->name,(const xmlChar*)"node" ) ))
    {

       if(parseNode(cur,&node_array[i_node])<0)return global_error ;

      i_node ++ ;

      /*On verifie si le tableau est plein */
      /*, si oui on le réalloue ************/
      if(i_node==n_node)
      {

        node_array = (osm_node *)realloc(node_array,(LENGTH_NODE_ARRAY + n_node)*sizeof(osm_node)) ;

        n_node += LENGTH_NODE_ARRAY ;

      }

    }

    if( ( !xmlStrcmp(cur->name,(const xmlChar*)"way" ) ))
    {

      if(!trie_node)
      {
        os->n_node = i_node ;
        os->nodes = node_array;
        sort_node(os);
        trie_node=1 ;
      }


      if(parseWay(cur,&way_array[i_way],os)<0) return global_error ;


      i_way ++ ;

      /*On verifie si le tableau est plein */
      /*, si oui on le réalloue ************/
        if(i_way==n_way)
        {

          way_array = (osm_way *)realloc(way_array,(LENGTH_WAY_ARRAY + n_way)*sizeof(osm_way)) ;

          n_way += LENGTH_WAY_ARRAY ;

        }

      }

      if( ( !xmlStrcmp(cur->name,(const xmlChar*)"relation" ) ))
      {

        os->n_node = i_node ;
        os->nodes = node_array ;
        os->n_way = i_way ;
        os->ways = way_array ;
        os->n_relation = i_rel ;
        os->relations = relation_array;

        if(!trie_way)
        {

          sort_node(os);
          sort_way(os);
          trie_way=1 ;
        }
        if(parseRelation(cur,&relation_array[i_rel],os)<0) return global_error ;

        i_rel ++ ;

        /*On verifie si le tableau est plein */
        /*, si oui on le réalloue ************/
          if(i_rel==n_rel)
          {

            relation_array = (osm_relation *)realloc(relation_array,(LENGTH_RELATION_ARRAY + n_rel)*sizeof(osm_relation)) ;

            n_rel += LENGTH_RELATION_ARRAY ;

          }
    }
    cur = cur->next ;
  }
  os->nodes = node_array;

  os->ways = way_array;
  os->n_node = i_node ;
  os->n_way = i_way ;
  os->n_relation = i_rel ;
  os->relations = relation_array ;
  cat(os);

  /*Free */
  xmlFreeDoc(doc) ;

  return 0;
}
