//
//  ViewController.h
//  HelloWorld
//
//  Created by ptrujillo on 1/13/15.
//  Copyright (c) 2015 Stem. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface LobbyViewController : UIViewController <NSStreamDelegate, UITableViewDelegate, UITableViewDataSource> {
    UIView			*lobbyView;
    NSInputStream	*inputStream;
    NSOutputStream	*outputStream;
    UITextField		*inputMessageField;
    UITableView		*tView;
    NSMutableArray	*messages;
}

@property (nonatomic, retain) IBOutlet UIView *lobbyView;
@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;
@property (nonatomic, retain) IBOutlet UITextField	*inputMessageField;
@property (nonatomic, retain) IBOutlet UITableView	*tView;
@property (nonatomic, retain) NSMutableArray *messages;

- (IBAction) sendMessage;

- (void) initNetworkCommunication;
- (void) messageReceived:(NSString *)message;

@end

