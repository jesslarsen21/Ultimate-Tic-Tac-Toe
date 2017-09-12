//
//  PlayerViewController.h
//  UTTT-iOS
//
//  Created by Eric Beilmann on 2/4/15.
//  Copyright (c) 2015 Stem. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PlayerViewController : UIViewController <NSStreamDelegate> {
    UIView			*playerView;
    NSInputStream	*inputStream;
    NSOutputStream	*outputStream;
    UITextField		*findPlayerField;
    NSMutableArray	*messages;
    
    IBOutlet UILabel *xLabel;
    IBOutlet UILabel *oLabel;
}

@property (nonatomic, retain) IBOutlet UIView *playerView;
@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;
@property (nonatomic, retain) IBOutlet UITextField	*findPlayerField;
@property (nonatomic, retain) NSMutableArray *messages;
@property (nonatomic, retain) UILabel *xLabel;
@property (nonatomic, retain) UILabel *oLabel;

- (IBAction) randomPlayer;
- (IBAction) enterUTTT;

- (void) initNetworkCommunication;
- (void) messageReceived:(NSString *)message;

@end