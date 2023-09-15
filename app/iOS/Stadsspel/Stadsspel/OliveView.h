//
//  OliveView.h
//  Stadsspel
//
//  Created by Jonas Everaert on 07/09/2023.
//

#ifndef OliveView_h
#define OliveView_h

#import <UIKit/UIKit.h>
#import <MetalKit/MetalKit.h>
#include "bufferReallocator.h"
#include <map_data/render_objects.h>
#include <hashmap.h>
#include <ui/view.h>
#include <arena/varena.h>
#include <ui/nav_view.h>

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

@interface OliveView : MTKView {
    int scaleFactor; // retina displays
    
    // Metal
    id<MTLCommandQueue> commandQueue;
    CIContext* ciContext;
}

@property UITapGestureRecognizer* tapGestureRecognizer;
@property BOOL forceRedraw;

@end

#endif /* OliveView_h */
