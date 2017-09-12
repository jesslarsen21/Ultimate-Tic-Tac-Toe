//
//  AppDelegate.h
//  HelloWorld
//
//  Created by ptrujillo on 1/13/15.
//  Copyright (c) 2015 Stem. All rights reserved.
//

#import <UIKit/UIKit.h>

@class LoginViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate> {
    UIWindow *window;
    LoginViewController *loginViewController;
}

@property (strong, nonatomic) UIWindow *window;
@property (nonatomic, retain) IBOutlet LoginViewController *loginViewController;

@end

