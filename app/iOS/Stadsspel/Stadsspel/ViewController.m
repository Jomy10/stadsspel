//
//  ViewController.m
//  Stadsspel
//
//  Created by Jonas Everaert on 31/08/2023.
//

#import "ViewController.h"
#include <renderer/renderer.h>
#include <uirenderer/apprenderer.h>
#include <map_data/render_objects.h>
#include <map_data/parse.h>
#include <uirenderer/coordinate_transform.h>
#include <app/app.h>

#include <stdio.h>

extern struct ViewState viewState;

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    printf("View loaded\n");

    // Retrieve map data
    NSDataAsset* dasset = [[NSDataAsset alloc] initWithName:@"testMap" bundle:[NSBundle mainBundle]];

    FILE* f = fmemopen((void*)[[dasset data] bytes], [[dasset data] length], "rb");

    // Parse map data
//    self->objs = parseMapFileDescriptorToRenderObjects(f);
    initMap(f);
    fclose(f);
//    RO_determineBoundsFromStreets(&self->objs);
//    printObjs(&self->objs);
//    self->mapnodes = convertNodes(&self->objs);
//    __objs = &objs;
//    __mapnodes = mapnodes;

    // Set background to black for now
    CALayer* calayer = self.view.layer;
    calayer.backgroundColor = [UIColor blackColor].CGColor;
    [calayer setNeedsDisplay]; // should update
    
    self->renderer = (Renderer){(__bridge void*)self.view.layer};
    setView(VIEW_MAIN);

    [self render];
    
    // Handle input
    UITapGestureRecognizer* tgr = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTapFrom:)];
    
    [self.view addGestureRecognizer:tgr];
    [self setTgr:tgr];
}

- (void)render {
    //Renderer renderer = (Renderer){(__bridge void*)self.view.layer};

    CGRect bounds = self.view.bounds;

    renderApp(self->renderer, (GRect){bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height});
}

- (void)handleTapFrom: (UITapGestureRecognizer*)recognizer {
    if (!viewState._navView.active) return;
    
    CGPoint touchPoint = [recognizer locationInView:self.view];
    
    
    int selectedNavItem = [self getNavItemClicked:&touchPoint];
    if (selectedNavItem == -1) return;
    printf("Selected element = %i\n", selectedNavItem);
    viewState._navView.currentNavView->currentView = selectedNavItem;
    
    [self render];
}

- (int)getNavItemClicked: (CGPoint*)tappos {
    GISize size = gGetScreenSize(self->renderer);
    
    float navHeight = (float)size.y / 7;
    float navWidth = (float)size.x / viewState._navView.buttons.count;
    float navY = size.y - navHeight;
    
    if (tappos->y >= navY && tappos->y <= size.y) {
        for (int i = 0; i < viewState._navView.buttons.count; i++) {
            if (tappos->x >= i * navWidth && tappos->x <= navWidth + i * navWidth) {
                return i;
            }
        }
    }
    
    return -1;
}

@end
