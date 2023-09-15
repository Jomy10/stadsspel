//
//  OliveView.m
//  Stadsspel
//
//  Created by Jonas Everaert on 07/09/2023.
//

#include <olive.c>

#import <UIKit/UIKit.h>
#import "OliveView.h"

#include <app/app.h>
#include <ui/view.h>
#include <ui/nav_view.h>
#include "ios_util.h"

#include <olive.c>

#define BYTES_PER_COMPONENT 4

BOOL rectsizeeq(CGRect* rect1, CGRect* rect2) {
    return rect1->size.width == rect2->size.width
        && rect1->size.height == rect2->size.height;
}

@implementation OliveView

- (void)commonInit {
    if (self.device == nil) self.device = MTLCreateSystemDefaultDevice();
    self->scaleFactor = [self contentScaleFactor]; // support for retina displays
    
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
    reinitAppWithFrame((arRect){
        rect.origin.x * self->scaleFactor,
        rect.origin.y * self->scaleFactor,
        rect.size.width * self->scaleFactor,
        rect.size.height * self->scaleFactor});
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

    id<MTLCommandBuffer> buffer = [self->commandQueue commandBuffer];
    assert(buffer != nil);
    
    if (![self drawUI:rect]) return;

    unsigned char* bitmapBuffer = getViewBitmap();
    if (bitmapBuffer == NULL) {
        NSLog(@"WARNING: bitmapBuffer not initialized");
        return;
    }
    
    NSData* bitmapData = [NSData
                          dataWithBytes:getViewBitmap()
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
    
    NSError* error = nil;
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
    
    [self setForceRedraw:false];
}

/// Retuns whether screen should rerender
- (BOOL)drawUI:(CGRect)rect {
    renderApp(CGRectToArRect(rect), [self forceRedraw]);
    return getDidViewChange();
}

- (void)handleTapFrom: (UITapGestureRecognizer*)recognizer {
    CGPoint touchPoint = [recognizer locationInView:self];
    
    NSLog(@"Touch at %@", NSStringFromCGPoint(touchPoint));
    
    handleNavViewTouch((arPoint){
        touchPoint.x * self->scaleFactor,
        touchPoint.y * self->scaleFactor,
    });
}

@end
