//
//  AIViewController.h
//  UTTT-iOS
//
//  Created by Eric Beilmann on 2/4/15.
//  Copyright (c) 2015 Stem. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface AIViewController : UIViewController <NSStreamDelegate> {
    UIView			*aiView;
    NSInputStream	*inputStream;
    NSOutputStream	*outputStream;
    NSMutableArray	*messages;
    
    IBOutlet UILabel *easyLabel;
    IBOutlet UILabel *mediumLabel;
    IBOutlet UILabel *hardLabel;
    IBOutlet UILabel *xLabel;
    IBOutlet UILabel *oLabel;
}

@property (nonatomic, retain) IBOutlet UIView *aiView;
@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;
@property (nonatomic, retain) NSMutableArray *messages;

@property (nonatomic, retain) UILabel *easyLabel;
@property (nonatomic, retain) UILabel *mediumLabel;
@property (nonatomic, retain) UILabel *hardLabel;
@property (nonatomic, retain) UILabel *xLabel;
@property (nonatomic, retain) UILabel *oLabel;

- (IBAction) enterUTTT;

- (void) initNetworkCommunication;
- (void) messageReceived:(NSString *)message;

@end