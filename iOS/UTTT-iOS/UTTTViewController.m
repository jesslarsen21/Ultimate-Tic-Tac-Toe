//
//  UTTTViewController.m
//  UTTT-iOS
//
//  Created by STUDENT #1 on 1/31/15.
//  Copyright (c) 2015 Stem. All rights reserved.
//

#import "UTTTViewController.h"
#import "TTTViewController.h"
@interface UTTTViewController ()

@property (nonatomic, strong) NSMutableArray *board0;
@property (nonatomic, strong) NSMutableArray *board1;
@property (nonatomic, strong) NSMutableArray *board2;
@property (nonatomic, strong) NSMutableArray *board3;
@property (nonatomic, strong) NSMutableArray *board4;
@property (nonatomic, strong) NSMutableArray *board5;
@property (nonatomic, strong) NSMutableArray *board6;
@property (nonatomic, strong) NSMutableArray *board7;
@property (nonatomic, strong) NSMutableArray *board8;
@property (nonatomic, strong) NSMutableArray *boards;
@property (nonatomic, strong) NSMutableArray *wonBoards;
@property (strong, nonatomic) IBOutletCollection(UILabel) NSArray *buttonLabels;

@property (strong, nonatomic) IBOutletCollection(UIButton) NSArray *boardButtons;
@property (nonatomic, strong) NSString *currPlayer;

@property(nonatomic, assign) NSInteger prevBoard;
@property(nonatomic, assign) NSInteger lastBoardPlayed;
@property (strong, nonatomic) IBOutlet UILabel *currMoveDisplay;
@property (strong, nonatomic) IBOutlet UILabel *currPlayerDisplay;

@end

@implementation UTTTViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
    [self initBoards];
    self.currPlayer = @"X";
    self.prevBoard = 100;
    [self setLabelColors];
    // Do any additional setup after loading the view.
    
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (void)setLabelColors
{
    for (UILabel *label in self.buttonLabels)
    {
        if (label.tag == self.prevBoard)
        {
            [label setBackgroundColor:[UIColor cyanColor]];
            //break;
        }
        else
        {
            [label setBackgroundColor:[UIColor colorWithRed:0.133 green:0.87 blue:0.99 alpha:.98]];
        }
    }
}

- (void)updateDisplayLabel
{
    for (UILabel *label in self.buttonLabels)
    {
        if (label.tag == self.lastBoardPlayed)
        {
            NSMutableAttributedString *textString = [[NSMutableAttributedString alloc] initWithString:[NSString stringWithFormat: @"  %@  %@  %@\n  %@  %@  %@\n  %@  %@  %@",
                                                            self.boards[self.lastBoardPlayed][0],
                                                            self.boards[self.lastBoardPlayed][1],
                                                            self.boards[self.lastBoardPlayed][2],
                                                            self.boards[self.lastBoardPlayed][3],
                                                            self.boards[self.lastBoardPlayed][4],
                                                            self.boards[self.lastBoardPlayed][5],
                                                            self.boards[self.lastBoardPlayed][6],
                                                            self.boards[self.lastBoardPlayed][7],
                                                            self.boards[self.lastBoardPlayed][8]]];
            
            UIFont *font = [UIFont fontWithName:@"Marker Felt" size:26];
            NSMutableParagraphStyle* style = [NSMutableParagraphStyle new];
            style.alignment = NSTextAlignmentLeft;
            [textString addAttribute:NSFontAttributeName value:font range:NSMakeRange(0, textString.length)];
            [textString addAttribute:NSParagraphStyleAttributeName value:style range:NSMakeRange(0, textString.length)];
            
            [label setAttributedText:textString];  // sets the string as text to be displayed in label
            
//            [label setFont:[UIFont fontWithName:@"Marker Felt" size:26]];
            label.textColor = [UIColor whiteColor];
//            label.textAlignment = NSTextAlignmentCenter;
//            label.numberOfLines = 3;
        }
    }
}


- (void) initBoards
{
    self.board0 = [[NSMutableArray alloc] initWithObjects:@"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", nil];
    self.board1 = [[NSMutableArray alloc] initWithObjects:@"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", nil];
    self.board2 = [[NSMutableArray alloc] initWithObjects:@"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", nil];
    self.board3 = [[NSMutableArray alloc] initWithObjects:@"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", nil];
    self.board4 = [[NSMutableArray alloc] initWithObjects:@"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", nil];
    self.board5 = [[NSMutableArray alloc] initWithObjects:@"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", nil];
    self.board6 = [[NSMutableArray alloc] initWithObjects:@"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", nil];
    self.board7 = [[NSMutableArray alloc] initWithObjects:@"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", nil];
    self.board8 = [[NSMutableArray alloc] initWithObjects:@"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", @"  ", nil];
    self.boards = [[NSMutableArray alloc] initWithObjects:self.board0, self.board1, self.board2, self.board3, self.board4, self.board5, self.board6, self.board7, self.board8, nil];
    
    self.wonBoards = [[NSMutableArray alloc] initWithObjects:@"", @"", @"", @"", @"", @"", @"", @"", @"", nil];
}


- (BOOL)validMoveOnBoard:(int)boardNumber
{
    if (self.prevBoard > 8)
    {
        return YES;
    }
    else if (self.prevBoard == boardNumber)
    {
        return YES;
    }
    else
    {
        return NO;
    }
    
}

- (IBAction)buttonPressed:(UIButton *)sender
{
    self.lastBoardPlayed = sender.tag;
}

#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(UIButton *)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
    if ([[segue identifier] isEqualToString:@"open ttt 0 view controller"])
    {
        TTTViewController *tttvc = [segue destinationViewController];
        tttvc.board = self.board0;
        tttvc.currentPlayer = self.currPlayer;
        tttvc.delegate = self;
        if ([self validMoveOnBoard:0])
        {
            tttvc.boardIsValid = YES;
        }
        else
        {
            tttvc.boardIsValid = NO;
        }
    }
    if ([[segue identifier] isEqualToString:@"open ttt 1 view controller"])
    {
        TTTViewController *tttvc = [segue destinationViewController];
        tttvc.board = self.board1;
        tttvc.currentPlayer = self.currPlayer;
        tttvc.delegate = self;
        if ([self validMoveOnBoard:1])
        {
            tttvc.boardIsValid = YES;
        }
        else
        {
            tttvc.boardIsValid = NO;
        }
    }
    if ([[segue identifier] isEqualToString:@"open ttt 2 view controller"])
    {
        TTTViewController *tttvc = [segue destinationViewController];
        tttvc.board = self.board2;
        tttvc.currentPlayer = self.currPlayer;
        tttvc.delegate = self;
        if ([self validMoveOnBoard:2])
        {
            tttvc.boardIsValid = YES;
        }
        else
        {
            tttvc.boardIsValid = NO;
        }
    }
    if ([[segue identifier] isEqualToString:@"open ttt 3 view controller"])
    {
        TTTViewController *tttvc = [segue destinationViewController];
        tttvc.board = self.board3;
        tttvc.currentPlayer = self.currPlayer;
        tttvc.delegate = self;
        if ([self validMoveOnBoard:3])
        {
            tttvc.boardIsValid = YES;
        }
        else
        {
            tttvc.boardIsValid = NO;
        }
    }
    if ([[segue identifier] isEqualToString:@"open ttt 4 view controller"])
    {
        TTTViewController *tttvc = [segue destinationViewController];
        tttvc.board = self.board4;
        tttvc.currentPlayer = self.currPlayer;
        tttvc.delegate = self;
        if ([self validMoveOnBoard:4])
        {
            tttvc.boardIsValid = YES;
        }
        else
        {
            tttvc.boardIsValid = NO;
        }
    }
    if ([[segue identifier] isEqualToString:@"open ttt 5 view controller"])
    {
        TTTViewController *tttvc = [segue destinationViewController];
        tttvc.board = self.board5;
        tttvc.currentPlayer = self.currPlayer;
        tttvc.delegate = self;
        if ([self validMoveOnBoard:5])
        {
            tttvc.boardIsValid = YES;
        }
        else
        {
            tttvc.boardIsValid = NO;
        }
    }
    if ([[segue identifier] isEqualToString:@"open ttt 6 view controller"])
    {
        TTTViewController *tttvc = [segue destinationViewController];
        tttvc.board = self.board6;
        tttvc.currentPlayer = self.currPlayer;
        tttvc.delegate = self;
        if ([self validMoveOnBoard:6])
        {
            tttvc.boardIsValid = YES;
        }
        else
        {
            tttvc.boardIsValid = NO;
        }
    }
    if ([[segue identifier] isEqualToString:@"open ttt 7 view controller"])
    {
        TTTViewController *tttvc = [segue destinationViewController];
        tttvc.board = self.board7;
        tttvc.currentPlayer = self.currPlayer;
        tttvc.delegate = self;
        if ([self validMoveOnBoard:7])
        {
            tttvc.boardIsValid = YES;
        }
        else
        {
            tttvc.boardIsValid = NO;
        }
    }
    if ([[segue identifier] isEqualToString:@"open ttt 8 view controller"])
    {
        TTTViewController *tttvc = [segue destinationViewController];
        tttvc.board = self.board8;
        tttvc.currentPlayer = self.currPlayer;
        tttvc.delegate = self;
        if ([self validMoveOnBoard:8])
        {
            tttvc.boardIsValid = YES;
        }
        else
        {
            tttvc.boardIsValid = NO;
        }
    }
}


#pragma mark - TTTEditedDelegate
- (void)tttViewController:(TTTViewController *)vc didUpdateBoard:(NSMutableArray *)board withPlayer:(NSString *)currentPlayer atPos:(NSInteger)lastPosition haveWinner:(BOOL)winner;
{
    NSLog(@"Hurray, board was edited with player%@", currentPlayer);
    
    if (winner)
    {
        for (UILabel *label in self.buttonLabels)
        {
            if (label.tag == (self.lastBoardPlayed + 9))
            {
                if ([self.wonBoards[self.lastBoardPlayed] isEqualToString:@""])
                {
                    label.text = self.currPlayer;
                    self.wonBoards[self.lastBoardPlayed] = self.currPlayer;
                }
            }
        }
    }
    
    [self updateDisplayLabel];
    if ([self haveWinner])
    {
        self.currMoveDisplay.text = [NSString stringWithFormat:@"Player %@ Won!", self.currPlayer];
        return;
    }
    
    self.currPlayer = currentPlayer;
    self.prevBoard = lastPosition;
    self.currMoveDisplay.text = [NSString stringWithFormat:@"Must play board: %li", (long)lastPosition];
    self.currPlayerDisplay.text = [NSString stringWithFormat:@"Player %@ turn", self.currPlayer];
    [self setLabelColors];
}


- (BOOL)haveWinner
{
    /* check all possible wins */
    
    /* check rows */
    // check top row
    if ([self.wonBoards[0] isEqualToString:self.currPlayer] && [self.wonBoards[1] isEqualToString:self.currPlayer] && [self.wonBoards[2] isEqualToString:self.currPlayer])
    {
        return YES;
    }
    // check mid row
    else if ([self.wonBoards[3] isEqualToString:self.currPlayer] && [self.wonBoards[4] isEqualToString:self.currPlayer] && [self.wonBoards[5] isEqualToString:self.currPlayer])
    {
        return YES;
    }
    // check bottom row
    else if ([self.wonBoards[6] isEqualToString:self.currPlayer] && [self.wonBoards[7] isEqualToString:self.currPlayer] && [self.wonBoards[8] isEqualToString:self.currPlayer])
    {
        return YES;
    }
    
    /* check columns */
    // check left column
    else if ([self.wonBoards[0] isEqualToString:self.currPlayer] && [self.wonBoards[3] isEqualToString:self.currPlayer] && [self.wonBoards[6] isEqualToString:self.currPlayer])
    {
        return YES;
    }
    // check mid column
    else if ([self.wonBoards[1] isEqualToString:self.currPlayer] && [self.wonBoards[4] isEqualToString:self.currPlayer] && [self.wonBoards[7] isEqualToString:self.currPlayer])
    {
        return YES;
    }
    // check right column
    else if ([self.wonBoards[2] isEqualToString:self.currPlayer] && [self.wonBoards[5] isEqualToString:self.currPlayer] && [self.wonBoards[8] isEqualToString:self.currPlayer])
    {
        return YES;
    }
    
    /* check both diagnols */
    else if ([self.wonBoards[0] isEqualToString:self.currPlayer] && [self.wonBoards[4] isEqualToString:self.currPlayer] && [self.wonBoards[8] isEqualToString:self.currPlayer])
    {
        return YES;
    }
    else if ([self.wonBoards[2] isEqualToString:self.currPlayer] && [self.wonBoards[4] isEqualToString:self.currPlayer] && [self.wonBoards[6] isEqualToString:self.currPlayer])
    {
        return YES;
    }
    
    return NO;
}

@end
