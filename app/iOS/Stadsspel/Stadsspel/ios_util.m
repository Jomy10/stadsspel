//
//  ios_util.m
//  Stadsspel
//
//  Created by Jonas Everaert on 14/09/2023.
//

#import "ios_util.h"

arRect CGRectToArRect(CGRect rect) {
    return (arRect){
        rect.origin.x,
        rect.origin.y,
        rect.size.width,
        rect.size.height};
}

CGRect arRectToCGRect(arRect rect) {
    return CGRectMake(rect.x, rect.y, rect.w, rect.h);
}
