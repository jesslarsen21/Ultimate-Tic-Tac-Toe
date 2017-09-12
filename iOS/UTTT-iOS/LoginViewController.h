//
//  LoginViewController.h
//  HelloWorld
//
//  Created by Eric Beilmann on 1/28/15.
//  Copyright (c) 2015 Stem. All rights reserved.
//
#import <UIKit/UIKit.h>

@interface LoginViewController : UIViewController <NSStreamDelegate> {
    UIView			*loginView;
    NSInputStream	*inputStream;
    NSOutputStream	*outputStream;
    UITextField		*inputUsernameField;
    UITextField		*inputPasswordField;
    NSMutableArray	*loginInfo;
}

@property (nonatomic, retain) IBOutlet UIView *loginView;
@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;
@property (nonatomic, retain) IBOutlet UITextField	*inputUsernameField;
@property (nonatomic, retain) IBOutlet UITextField	*inputPasswordField;
@property (nonatomic, retain) NSMutableArray *loginInfo;

- (IBAction) enterLobby;

- (void) initNetworkCommunication;
- (void) loginInfoReceived:(NSString *)loginInfo;

@end