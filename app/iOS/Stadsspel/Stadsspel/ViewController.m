//
//  ViewController.m
//  Stadsspel
//
//  Created by Jonas Everaert on 31/08/2023.
//

#import "ViewController.h"
#include <renderer/renderer.h>
#include <render_frontend/renderer.h>
#include <map_data/render_objects.h>
#include <map_data/parse.h>
#include <renderer/coordinate_transform.h>

#include <stdio.h>

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    printf("View loaded\n");
   
    // Retrieve map data
    NSDataAsset* dasset = [[NSDataAsset alloc] initWithName:@"testMap" bundle:[NSBundle mainBundle]];
    
    FILE* f = fmemopen((void*)[[dasset data] bytes], [[dasset data] length], "rb");

    // Parse map data
    self->objs = parseMapFileDescriptorToRenderObjects(f);
    fclose(f);
    RO_determineBoundsFromStreets(&self->objs);
    printObjs(&self->objs);
    self->mapnodes = convertNodes(&self->objs);
    
    // Set background to black for now
    CALayer* calayer = self.view.layer;
    calayer.backgroundColor = [UIColor blackColor].CGColor;
    [calayer setNeedsDisplay]; // should update
    
    
    [self render];
}

- (void)render {
    Renderer renderer = (Renderer){(__bridge void*)self.view.layer};
    
    CGRect bounds = self.view.bounds;
   
    renderApp(renderer, &self->objs, self->mapnodes, (GRect){bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height});
}

@end
