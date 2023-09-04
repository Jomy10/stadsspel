//
//  rendererImpl.m
//  Stadsspel
//
//  Implement renderer frontend with CALayer
//
//  Created by Jonas Everaert on 31/08/2023.
//

#import <UIKit/UIKit.h>
#import <CoreGraphics/CoreGraphics.h>
#include <renderer/renderer.h>

static GColor __fillCol = (GColor){0,0,0,1};
static GColor __strokeCol = (GColor){0,0,0,1};
static float __lineWidth = 1;

static inline CGPoint gPt(GPoint p) {
    return CGPointMake((CGFloat)p.x, (CGFloat)p.y);
}

#define __GET_COLOR_VAL(c) (((CGFloat)c)/255.0)
static inline struct CGColor* gCol(GColor c) {
    return CGColorCreateGenericRGB(__GET_COLOR_VAL(c.r), __GET_COLOR_VAL(c.g), __GET_COLOR_VAL(c.b), __GET_COLOR_VAL(c.a));
}

void gDrawLine(Renderer ren, GPoint point1, GPoint point2, GLineOpts* _opts) {
    CALayer* base = (__bridge CALayer*) ren.ptr;
    GLineOpts opts = _opts == nil ? (GLineOpts){__lineWidth, __strokeCol} : *_opts;

    UIBezierPath* path = [[UIBezierPath alloc] init];
    [path moveToPoint:gPt(point1)];
    [path addLineToPoint:gPt(point2)];

    CAShapeLayer* shape = [[CAShapeLayer alloc] init];
    shape.path = path.CGPath;
    //shape.fillColor = UIColor.blueColor.CGColor;
    shape.lineWidth = (CGFloat)opts.width;
    shape.strokeColor = gCol(opts.color);
    shape.fillColor = nil;

    [base addSublayer:shape];
}

GFont gLoadFont(char* fontfile) {
  // return (__bridge GFont)[CGFont CGFontCreateWithFontName: fontfile];
  return (GFont)CGFontCreateWithFontName(CFStringCreateWithCString(kCFAllocatorDefault, fontfile, kCFStringEncodingUTF8));
  //return (__bridge GFont)[NSFont fontWithName: fontfile, fontSize: 1];
  // UIFont* font = [UIFont fontWithName:fontfile fontSize: 1];
  // return (__bridge GFont)font;
  //return (__bridge GFont)[NSString stringWithUTF8String:fontFile];
}
GFont gLoadFontFileDescriptor(__unused FILE* f) {
  NSLog(@"Unimplemented");
  return nil;
}
void gFreeFont(__attribute__((unused)) GFont _) {}

void gDrawLines(Renderer ren, const GPoint* points, int count, GLineOpts* _opts) {
    if (count == 0) return;

    CALayer* base = (__bridge CALayer*) ren.ptr;
    GLineOpts opts = _opts == nil ? (GLineOpts){__lineWidth, __strokeCol} : *_opts;

    UIBezierPath* path = [[UIBezierPath alloc] init];
    [path moveToPoint:gPt(points[0])];
    for (int i = 1; i < count; i++) {
        [path addLineToPoint:gPt(points[i])];
    }

    CAShapeLayer* shape = [[CAShapeLayer alloc] init];
    shape.path = path.CGPath;
    shape.lineWidth = (CGFloat)opts.width;
    shape.strokeColor = gCol(opts.color);
    shape.fillColor = nil;

    [base addSublayer:shape];
}

void gDrawRect(Renderer ren, GRect rect, GRectOpts* _opts) {
    CALayer* base = (__bridge CALayer*) ren.ptr;
    GRectOpts opts = _opts == nil ? (GRectOpts){__fillCol, &(GLineOpts){__lineWidth, __strokeCol}} : *_opts;

    UIBezierPath* path = [UIBezierPath bezierPathWithRect:CGRectMake(rect.x, rect.y, rect.w, rect.h)];

    CAShapeLayer* shape = [[CAShapeLayer alloc] init];
    shape.path = path.CGPath;
    shape.fillColor = gCol(opts.fill);
    if (opts.stroke != nil) {
        shape.strokeColor = gCol(opts.stroke->color);
        shape.lineWidth = (CGFloat)opts.stroke->width;
    }

    [base addSublayer:shape];
}

void gDrawText(GSurface* surface, GFont font, const char* text, const GFontOpts* opts) {
  CATextLayer* layer = [[CATextLayer alloc] init];
  [layer setFont:font];
  [layer setFontSize:opts->lineHeight];
  [layer setFrame:[((__bridge CALayer*)surface->texture) frame]];
  [layer setString:[NSString stringWithUTF8String:text]];
  [layer setAlignmentMode:kCAAlignmentLeft];
  [layer setForegroundColor:CGColorCreateGenericRGB(opts->fill.r, opts->fill.g, opts->fill.b, opts->fill.a)];
  [((__bridge CALayer*)surface->texture) addSublayer:layer];
}

GSurface gCreateSurface(__attribute__((unused)) Renderer ren, int w, int h) {
  CALayer* layer = [[CALayer alloc] init];
  layer.bounds = CGRectMake(0, 0, w, h);
  layer.masksToBounds = true;
  return (GSurface) {
    .texture = (__bridge void*) layer,
    .w = w, .h = h,
    .pixels = nil,
  };
}

void gFreeSurface(__unused GSurface* surface) {
  //[((__bridge CALayer*) surface->texture) dealloc];
}

void gDrawSurface(Renderer ren, GSurface* surface, GPoint pos) {
  CALayer* base = (__bridge CALayer*) ren.ptr;

  ((__bridge CALayer*)surface).position = (CGPoint){(CGFloat)pos.x, (CGFloat)pos.y};

  [base addSublayer:(__bridge CALayer*)surface];
}

void gSetStrokeColor(__attribute__((unused)) Renderer _, GColor col) {
    __strokeCol = col;
}

void gSetFillColor(__attribute__((unused)) Renderer _, GColor col) {
    __fillCol = col;
}

void gSetLineWidth(__attribute__((unused)) Renderer _, float lw) {
    __lineWidth = lw;
}

void gRenderUpdate(Renderer ren) {
    CALayer* base = (__bridge CALayer*) ren.ptr;
    [base setNeedsDisplay];
}

GISize gGetScreenSize(Renderer ren) {
    CALayer* base = (__bridge CALayer*) ren.ptr;
    CGRect bounds = base.bounds;
    return (GISize){(int)bounds.size.width, (int)bounds.size.height};
}

void gRenderClear(Renderer ren) {
  CALayer* base = (__bridge CALayer*) ren.ptr;
  [base.sublayers makeObjectsPerformSelector:@selector(removeFromSuperlayer)];
}

void gRenderClearColor(Renderer ren, GColor col) {
  CALayer* base = (__bridge CALayer*) ren.ptr;
  //[base.sublayers makeObjectsPerformSelector:@selector(removeFromSuperlayer)];
  while (base.sublayers.count > 0) {
    [base.sublayers[base.sublayers.count - 1] removeFromSuperLayer];
  }
  base.backgroundColor = gCol(col);
}
