//
//  ViewController.m
//  Stadsspel
//
//  Created by Jonas Everaert on 31/08/2023.
//

#import "ViewController.h"
#include <map_data/render_objects.h>
#include <map_data/parse.h>
#include <app/map.h>
#include <app/renderer.h>
#include "OliveView.h"

#include <stdio.h>

extern struct ViewState viewState;

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    printf("View loaded\n");

    // Retrieve map data
    NSDataAsset* mapAsset = [[NSDataAsset alloc] initWithName:@"testMap" bundle:[NSBundle mainBundle]];
    if (mapAsset == nil) {
        NSLog(@"Couldn't open NSDataAsset\n");
    }

    FILE* f = fmemopen((void*)[[mapAsset data] bytes], [[mapAsset data] length], "rb");

    // Parse map data
    if (!initMap(f, &self->objs, &self->mapnodes)) {
        NSLog(@"ERR: Couldn't load map data");
    }
    fclose(f);
    
    // Init renderer
    OliveView* oliveView = [[OliveView alloc] initWithFrame:self.view.bounds device:MTLCreateSystemDefaultDevice()];
    [oliveView setData:&self->objs nodes:self->mapnodes];
    [self.view addSubview:oliveView];
    [oliveView setNeedsDisplay];
    [self.view setNeedsDisplay];
    
    // Handle input
//    UITapGestureRecognizer* tgr = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTapFrom:)];
//    
//    [self.view addGestureRecognizer:tgr];
//    [self setTgr:tgr];
}

- (void)viewDidAppear:(BOOL)animated {
//    [[self.view subviews].firstObject setFrame:self.view.frame];
    NSLog(@"%@", [self.view subviews]);
    [self.view setNeedsDisplay];
}

//- (void)handleTapFrom: (UITapGestureRecognizer*)recognizer {
//    if (!viewState._navView.active) return;
//
//    CGPoint touchPoint = [recognizer locationInView:self.view];
//
//
//    int selectedNavItem = [self getNavItemClicked:&touchPoint];
//    if (selectedNavItem == -1) return;
//    printf("Selected element = %i\n", selectedNavItem);
//    viewState._navView.currentNavView->currentView = selectedNavItem;
//
//    [self render];
//}

//- (int)getNavItemClicked: (CGPoint*)tappos {
//    GISize size = gGetScreenSize(self->renderer);
//
//    float navHeight = (float)size.y / 7;
//    float navWidth = (float)size.x / viewState._navView.buttons.count;
//    float navY = size.y - navHeight;
//
//    if (tappos->y >= navY && tappos->y <= size.y) {
//        for (int i = 0; i < viewState._navView.buttons.count; i++) {
//            if (tappos->x >= i * navWidth && tappos->x <= navWidth + i * navWidth) {
//                return i;
//            }
//        }
//    }
    
//    return -1;
//}

- (void)dealloc {
    deinitMap(&self->objs, self->mapnodes);
}

@end
