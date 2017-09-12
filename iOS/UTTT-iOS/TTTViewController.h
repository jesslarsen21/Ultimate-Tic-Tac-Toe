//
//  TTTViewController.h
//  UTTT-iOS
//
//  Created by STUDENT #1 on 1/31/15.
//  Copyright (c) 2015 Stem. All rights reserved.
//

#import <UIKit/UIKit.h>

@class TTTViewController;

@protocol TTTEditedDelegate <NSObject>

- (void)tttViewController:(TTTViewController*)vc didUpdateBoard:(NSMutableArray*)board withPlayer:(NSString*)currentPlayer atPos:(NSInteger)lastPosition haveWinner:(BOOL)winner;

@end

@interface TTTViewController : UIViewController

@property (nonatomic, strong) NSMutableArray *board;
@property (nonatomic, strong) NSString *currentPlayer;
@property (nonatomic, assign) NSInteger lastPosition;
@property (nonatomic, assign) BOOL boardIsValid;
@property (nonatomic, assign) id<TTTEditedDelegate> delegate;
@property (nonatomic, assign) BOOL boardWon;

@end
