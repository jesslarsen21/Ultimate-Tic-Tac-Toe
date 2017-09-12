//
//  TTTViewController.m
//  UTTT-iOS
//
//  Created by STUDENT #1 on 1/31/15.
//  Copyright (c) 2015 Stem. All rights reserved.
//

#import "TTTViewController.h"

@interface TTTViewController ()

@property (strong, nonatomic) IBOutletCollection(UILabel) NSArray *buttonLabels;

@property (strong, nonatomic) IBOutlet UILabel *validityDisplayLabel;

@end

@implementation TTTViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self setupBoard];
    [self setDisplay];
    
    
    // Do any additional setup after loading the view, typically from a nib.
    
    // set background image
    //self.view.backgroundColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"emptyboard.png"]];
    
    //UIImageView *backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"emptyboard"]];
    //[self.view addSubview:backgroundView];
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (void)changePlayerTurn
{
    if ([self.currentPlayer isEqualToString:@"X"])
    {
        self.currentPlayer = @"O";
    }
    else
    {
        self.currentPlayer = @"X";
    }
}


- (IBAction)pressedButton:(UIButton *)sender
{
    if (self.boardIsValid == YES)
    {
        for (UILabel *label in self.buttonLabels)
        {
            if (label.tag == sender.tag && ([label.text isEqualToString:@""] || [label.text isEqualToString:@"  "]))
            {
                label.text = self.currentPlayer;
                self.board[sender.tag] = self.currentPlayer;
                self.lastPosition = sender.tag;
                self.boardWon = [self haveWinner];
                [self changePlayerTurn];
                [self.delegate tttViewController:self didUpdateBoard:self.board withPlayer:self.currentPlayer atPos:self.lastPosition haveWinner:self.boardWon];
                [self.navigationController popViewControllerAnimated:YES];
            }
        }
        NSLog(@"Pressed button %li", (long)sender.tag);
    }
}


- (void) setupBoard
{
    for (UILabel *label in self.buttonLabels)
    {
        label.text = self.board[label.tag];
    }
}

- (void) setDisplay
{
    if (self.boardIsValid)
    {
        self.validityDisplayLabel.text = @"Board is valid for play";
    }
    else
    {
        self.validityDisplayLabel.text = @"Board is not valid for play";
    }
}

- (BOOL)haveWinner
{
    /* check all possible wins */
    
    /* check rows */
    // check top row
    if ([self.board[0] isEqualToString:self.currentPlayer] && [self.board[1] isEqualToString:self.currentPlayer] && [self.board[2] isEqualToString:self.currentPlayer])
    {
        return YES;
    }
    // check mid row
    else if ([self.board[3] isEqualToString:self.currentPlayer] && [self.board[4] isEqualToString:self.currentPlayer] && [self.board[5] isEqualToString:self.currentPlayer])
    {
        return YES;
    }
    // check bottom row
    else if ([self.board[6] isEqualToString:self.currentPlayer] && [self.board[7] isEqualToString:self.currentPlayer] && [self.board[8] isEqualToString:self.currentPlayer])
    {
        return YES;
    }
    
    /* check columns */
    // check left column
    else if ([self.board[0] isEqualToString:self.currentPlayer] && [self.board[3] isEqualToString:self.currentPlayer] && [self.board[6] isEqualToString:self.currentPlayer])
    {
        return YES;
    }
    // check mid column
    else if ([self.board[1] isEqualToString:self.currentPlayer] && [self.board[4] isEqualToString:self.currentPlayer] && [self.board[7] isEqualToString:self.currentPlayer])
    {
        return YES;
    }
    // check right column
    else if ([self.board[2] isEqualToString:self.currentPlayer] && [self.board[5] isEqualToString:self.currentPlayer] && [self.board[8] isEqualToString:self.currentPlayer])
    {
        return YES;
    }
    
    /* check both diagnols */
    else if ([self.board[0] isEqualToString:self.currentPlayer] && [self.board[4] isEqualToString:self.currentPlayer] && [self.board[8] isEqualToString:self.currentPlayer])
    {
        return YES;
    }
    else if ([self.board[2] isEqualToString:self.currentPlayer] && [self.board[4] isEqualToString:self.currentPlayer] && [self.board[6] isEqualToString:self.currentPlayer])
    {
        return YES;
    }
    
    return NO;
}

@end