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
#include <app/app.h>
#include "OliveView.h"
#include "ios_util.h"

#include <stdio.h>

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    printf("View loaded\n");
    
    initAppState();

    // Retrieve map data
    NSDataAsset* mapAsset = [[NSDataAsset alloc] initWithName:@"testMap" bundle:[NSBundle mainBundle]];
    if (mapAsset == nil) {
        NSLog(@"Couldn't open NSDataAsset\n");
    }

    FILE* f = fmemopen((void*)[[mapAsset data] bytes], [[mapAsset data] length], "rb");
    
    readMapData(f);
    
    fclose(f);
    
    // Init renderer
    OliveView* oliveView = [[OliveView alloc] initWithFrame:self.view.bounds device:MTLCreateSystemDefaultDevice()];
    [self.view addSubview:oliveView];
}

- (void)viewDidAppear:(BOOL)animated {
    OliveView* view = [self.view subviews].firstObject;
    [view setForceRedraw:true];
    [self.view setNeedsDisplay];
}

- (UIStatusBarStyle)preferredStatusBarStyle {
    return UIStatusBarStyleLightContent;
}

- (void)dealloc {
    destroyApp();
}

@end
