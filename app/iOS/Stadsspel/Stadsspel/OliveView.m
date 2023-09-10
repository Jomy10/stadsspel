//
//  OliveView.m
//  Stadsspel
//
//  Created by Jonas Everaert on 07/09/2023.
//

#include <olive.c>

#import <UIKit/UIKit.h>
#import "OliveView.h"

#include <app/renderer.h>
#include <app/default_ui.h>
#include <ui/view.h>
#include <ui/nav_view.h>

#include <olive.c>

#define BYTES_PER_COMPONENT 4

arRect CGRectToArRect(CGRect rect) {
    return (arRect){rect.origin.x, rect.origin.y, rect.size.width, rect.size.height};
}

BOOL rectsizeeq(CGRect* rect1, CGRect* rect2) {
    return rect1->size.width == rect2->size.width
        && rect1->size.height == rect2->size.height;
}

@implementation OliveView

- (void)commonInit {
    if (self.device == nil) self.device = MTLCreateSystemDefaultDevice();
    self->previousRect = CGRectMake(-1, -1, -1, -1);
    self->bitmap = nil;
    self->reallocator = br_init();
    self->uiAllocator = makeVariableArenaAllocator(1024);
    self->objs = nil;
    self->mapnodes = nil;
    ar_setAllocator(self->uiAllocator);
    self->scaleFactor = [self contentScaleFactor]; // support for retina displays
    
    createRootView(&self->root, &self->mapViewLevel, &self->navSize, &self->selectedNavItem,
                   &self->navViewData,
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
                   &self->objs, &self->mapnodes);
#pragma clang diagnostic pop
    
    // Metal
    assert(self.device != nil);
    self->commandQueue = [self.device newCommandQueue];
    self->ciContext = [CIContext contextWithMTLDevice:self.device options:nil];
    
    // this allows us to render into the view's drawable
    self.framebufferOnly = false;

    NSLog(@"Created new OliveView: %@\n", self);

    UITapGestureRecognizer* tgr = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTapFrom:)];

    [self addGestureRecognizer:tgr];
    [self setTapGestureRecognizer:tgr];
}

- (id)init {
    NSLog(@"Initializing OliveView\n");
    self = [super init];
    [self commonInit];
    return self;
}

- (id)initWithFrame:(CGRect)frame {
    NSLog(@"Initializing OliveView with frame %@\n", NSStringFromCGRect(frame));
    self = [super initWithFrame:frame];
    [self commonInit];
    [self reinitWithFrame:frame];
    NSLog(@"Initialized\n");
    return self;
}

- (id)initWithFrame:(CGRect)frameRect device:(id<MTLDevice>)device {
    self = [super initWithFrame:frameRect device:device];
    [self commonInit];
    [self reinitWithFrame:frameRect];
    return self;
}

/// (re-)initialize canvas and bitmap buffer
- (void)reinitWithFrame:(CGRect)rect {
    self->bitmap = br_alloc(&self->reallocator, rect.size.width * rect.size.height * BYTES_PER_COMPONENT);
    self->canvas = olivec_canvas((UInt32*)self->bitmap, rect.size.width, rect.size.height, rect.size.width);
    self->previousRect = rect;
}

- (void)setData:(MapRenderObjects*)objs
          nodes:(struct hashmap*) mapnodes {
    self->objs = objs;
    self->mapnodes = mapnodes;
    CGRect bounds = self.frame;
    [self drawUI:bounds];
    [self setNeedsDisplay];
}

- (void)dealloc {
    br_deinit(&self->reallocator);
    vArenaAllocator_free(self->uiAllocator);
}

- (CGRect)getRetinaBounds:(CGRect)_rect {
    int w = _rect.size.width * self->scaleFactor;
    int h = _rect.size.height * self->scaleFactor;
    
    return CGRectMake(_rect.origin.x, _rect.origin.y, w, h);
}

// https://stackoverflow.com/a/64285288/14874405
// TODO: only redraw when changed
- (void)drawRect:(CGRect)_rect {
    CGRect rect = [self getRetinaBounds:_rect];
    int w = rect.size.width;
    int h = rect.size.height;

    if (!rectsizeeq(&self->previousRect, &rect)) {
        [self reinitWithFrame:rect];
    }
    
    id<MTLCommandBuffer> buffer = [self->commandQueue commandBuffer];
    assert(buffer != nil);
    
    [self drawUI:rect];

    NSData* bitmapData = [NSData
                          dataWithBytes:self->bitmap
                          length:w * h * BYTES_PER_COMPONENT];

    // todo: does auto update on buffer update?
    CIImage* ciimg = [CIImage
                      imageWithBitmapData:bitmapData
                      bytesPerRow:w * BYTES_PER_COMPONENT
                      size:rect.size
                      format:kCIFormatRGBA8
                      colorSpace:CGColorSpaceCreateDeviceRGB()];
   
    // Resize retina image to fit the screen
    CGFloat scale = self.drawableSize.width / ciimg.extent.size.width;
    [ciimg imageByApplyingTransform:CGAffineTransformMakeScale(scale, scale)];
    
    CIRenderDestination* destination = [[CIRenderDestination alloc]
                                        initWithWidth:w height:h
                                        pixelFormat:self.colorPixelFormat
                                        commandBuffer:nil
                                        mtlTextureProvider:^id<MTLTexture> _Nonnull{
                                            return self.currentDrawable.texture;
                                        }];
    
    NSError* error;
    [self->ciContext startTaskToRender:ciimg
     // TODO: only render parts that have changed (when updating view,
     // put the rect to be redrawn into an array
                              fromRect:rect
                         toDestination:destination
                               atPoint:rect.origin
                                 error:&error];
    if (error != nil) NSLog(@"RENDER ERROR: %@", error);
    
    [buffer presentDrawable:self.currentDrawable];
    [buffer commit];
}

- (void)drawUI:(CGRect)rect {
    olivec_fill(self->canvas, 0xFF000000); // clear screen
    renderApp(self->canvas, self->root, (arRect){rect.origin.x, rect.origin.y, rect.size.width, rect.size.height});
}

- (void)handleTapFrom: (UITapGestureRecognizer*)recognizer {
    CGPoint touchPoint = [recognizer locationInView:self];
    
    int index = touchedNavViewIndex(
        self->navViewData,
        CGRectToArRect(self->previousRect),
        (arPoint){touchPoint.x * self->scaleFactor, touchPoint.y * self->scaleFactor});
    
    if (index == -1) return;
    
    *self->selectedNavItem = index;
    
    [self drawUI:self->previousRect];
    [self setNeedsDisplay];
}

@end
