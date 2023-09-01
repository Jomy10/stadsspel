//
//  rendererImpl.m
//  Stadsspel
//
//  Implement renderer frontend with CALayer
//
//  Created by Jonas Everaert on 31/08/2023.
//

#import <UIKit/UIKit.h>
#include <render_frontend/renderer.h>

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

void gDrawLine(Renderer r, GPoint p1, GPoint p2) {
    gDrawLineO(r, p1, p2, nil);
}

void gDrawLineO(Renderer ren, GPoint point1, GPoint point2, GLineOpts* _opts) {
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

void gDrawLines(Renderer ren, const GPoint* points, int count) {
    gDrawLinesO(ren, points, count, nil);
}

void gDrawLinesO(Renderer ren, const GPoint* points, int count, GLineOpts* _opts) {
    if (count == 0) return;

    CALayer* base = (__bridge CALayer*) ren.ptr;
    printf("%p\n", base);
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

void gDrawRect(Renderer ren, GRect rect) {
    gDrawRectO(ren, rect, nil);
}

void gDrawRectO(Renderer ren, GRect rect, GRectOpts* _opts) {
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

void gSetStrokeColor(Renderer _, GColor col) {
    __strokeCol = col;
}

void gSetFillColor(Renderer _, GColor col) {
    __fillCol = col;
}

void gSetLineWidth(Renderer _, float lw) {
    __lineWidth = lw;
}

void gRenderUpdate(Renderer ren) {
    CALayer* base = (__bridge CALayer*) ren.ptr;
    [base setNeedsDisplay];
}

GSize gGetScreenSize(Renderer ren) {
    CALayer* base = (__bridge CALayer*) ren.ptr;
    CGRect bounds = base.bounds;
    return (GSize){bounds.size.width, bounds.size.height};
}
