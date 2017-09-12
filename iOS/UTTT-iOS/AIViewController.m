//
//  AIViewController.m
//  UTTT-iOS
//
//  Created by Eric Beilmann on 2/4/15.
//  Copyright (c) 2015 Stem. All rights reserved.
//

#import "AIViewController.h"

@interface AIViewController ()

@end

@implementation AIViewController

@synthesize aiView;
@synthesize inputStream, outputStream;
@synthesize messages;

@synthesize easyLabel;
@synthesize mediumLabel;
@synthesize hardLabel;
@synthesize xLabel;
@synthesize oLabel;

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initNetworkCommunication];
    messages = [[NSMutableArray alloc] init];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

/////View controllers/////
- (IBAction)difficultyEasy:(UIButton *)sender {
    easyLabel.text = @"x";
    mediumLabel.text = @"";
    hardLabel.text = @"";
}

- (IBAction)difficultyMedium:(UIButton *)sender {
    easyLabel.text = @"";
    mediumLabel.text = @"x";
    hardLabel.text = @"";
}

- (IBAction)difficultyHard:(UIButton *)sender {
    easyLabel.text = @"";
    mediumLabel.text = @"";
    hardLabel.text = @"x";
}

- (IBAction)playSymbolX:(UIButton *)sender {
    xLabel.text = @"x";
    oLabel.text = @"";
}

- (IBAction)playSymbolO:(UIButton *)sender {
    xLabel.text = @"";
    oLabel.text = @"x";
}

- (IBAction)enterUTTT {
}

/////Network Code/////
- (void)initNetworkCommunication {
    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
    CFStreamCreatePairWithSocketToHost(NULL, (CFStringRef)@"rookie.cs.dixie.edu", 3333, &readStream, &writeStream);
    inputStream = (__bridge NSInputStream *)readStream;
    outputStream = (__bridge NSOutputStream *)writeStream;
    
    //set delegate
    [inputStream setDelegate:self];
    [outputStream setDelegate:self];
    
    //schedule streams
    [inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    //open connection
    [inputStream open];
    [outputStream open];
}

- (void)stream:(NSStream *)theStream handleEvent:(NSStreamEvent)streamEvent {
    //NSLog(@"theStream");
    NSLog(@"stream event %lu", streamEvent);
    switch (streamEvent) {
        case NSStreamEventOpenCompleted:
            NSLog(@"Stream opened (NSStreamEventOpenCompleted)");
            break;
        case NSStreamEventHasBytesAvailable:
            NSLog(@"Message recieved (NSStreamEventHasBytesAvailable)");
            if (theStream == inputStream) {
                uint8_t buffer[1024];
                long len;
                while ([inputStream hasBytesAvailable]) {
                    len = [inputStream read:buffer maxLength:sizeof(buffer)];
                    if (len > 0) {
                        NSString *output = [[NSString alloc] initWithBytes:buffer length:len encoding:NSASCIIStringEncoding];
                        if (nil != output) {
                            NSLog(@"server said: %@", output);
                            [self messageReceived:output];
                        }
                    }
                }
            }
            break;
        case NSStreamEventErrorOccurred:
            NSLog(@"Can not connect to the host! (NSStreamEventErrorOccurred)");
            [theStream close];
            [theStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
            theStream = nil;
            break;
        case NSStreamEventEndEncountered:
            NSLog(@"Stream event end (NSStreamEventEndEncountered)");
            break;
        default:
            NSLog(@"Unknown event (NSStreamEventEndEncountered)");
    }
}

- (void) messageReceived:(NSString *)message {
    NSLog(@"messageReceived");
    [messages addObject:message];
}

@end
