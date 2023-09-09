#include "include/default_ui.h"
#include <ui/view.h>
#include <ui/map_view.h>
#include <ui/nav_view.h>
#include <ui/list_view.h>
#include <ui/scoreboard_nav_item.h>

void createRootView(
  arView** root,
  // output data
  float** mapZoomLevel, int** navSize, int** selectedNav,
  NavViewData** navViewDataOut,
  // input data
  const MapRenderObjects** objs, const struct hashmap** mapnodes)
{
  *root = createArView();
 
  // Map
  MapViewData* mapViewData = al_alloc(arAllocator, sizeof(MapViewData));
  mapViewData->mapnodes = mapnodes;
  mapViewData->objs = objs;
  mapViewData->zoomLevel = 1;
  arView* mapView = createMapView(mapViewData);
  
  *mapZoomLevel = &mapViewData->zoomLevel;
  
  arView* mapViewNavItem = createMapViewNavItem();
  
  // Nav
  const int NAV_VIEW_ITEM_COUNT =  2;
  NavViewData* navViewData = al_alloc(arAllocator, sizeof(NavViewData));
  navViewData->navSize = 400;
  navViewData->navViews = createVecA(arView**, NAV_VIEW_ITEM_COUNT, arAllocator);
  navViewData->selectedView = 0;
  *navViewDataOut = navViewData;
  arView* navView = createNavView(navViewData);
  
  *selectedNav = &navViewData->selectedView;
  *navSize = &navViewData->navSize;
  
  // nav view items
  ar_addSubView(navView, mapView);
  veca_append(navViewData->navViews, &mapViewNavItem);
  
  // Leaderboard
  UniformListViewData* scoreboardViewData = al_alloc(arAllocator, sizeof(UniformListViewData));
  scoreboardViewData->subviewHeight = 400;
  scoreboardViewData->itemMargin = 20;
  arView* scoreboardView = createUniformListView(scoreboardViewData);
  
  arView* scoreboardViewNavItem = createScoreboardViewNavItem();
  
  ar_addSubView(navView, scoreboardView);
  veca_append(navViewData->navViews, &scoreboardViewNavItem);
  
  ar_addSubView(*root, navView);
}
