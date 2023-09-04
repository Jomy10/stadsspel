//
//  ViewController.h
//  Stadsspel
//
//  Created by Jonas Everaert on 31/08/2023.
//

#import <UIKit/UIKit.h>
#import <map_data/render_objects.h>
#import <hashmap.h>
#import <renderer/renderer.h>

@interface ViewController : UIViewController {
    MapRenderObjects objs;
    struct hashmap* mapnodes;
    Renderer renderer;
}

@property UITapGestureRecognizer* tgr;

@end
