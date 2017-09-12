//
//  JoinViewController.m
//  HelloWorld
//
//  Created by Eric Beilmann on 1/28/15.
//  Copyright (c) 2015 Stem. All rights reserved.
//

#import "LoginViewController.h"

@interface LoginViewController ()

@end

@implementation LoginViewController

@synthesize loginView;
@synthesize inputStream, outputStream;
@synthesize inputUsernameField, inputPasswordField;
@synthesize loginInfo;

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initNetworkCommunication];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

/////View controllers/////
- (IBAction)enterLobby {
    NSString *nameResponse  = [NSString stringWithFormat:@"%@", inputUsernameField.text];
    NSString *passResponse  = [NSString stringWithFormat:@"%@", inputPasswordField.text];
    //[loginInfo addObject:nameResponse];
    //[loginInfo addObject:passResponse];
    NSString *response = [NSString stringWithFormat:@"%@,%@", nameResponse, passResponse];
    
    NSData *data = [[NSData alloc] initWithData:[response dataUsingEncoding:NSASCIIStringEncoding]];
    [outputStream write:[data bytes] maxLength:[data length]];
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
                            //NSString *logInfo = [NSString stringWithFormat:@"ES: %@,%@", loginInfo[0], loginInfo[1]];
                            //if([output isEqualToString:logInfo]) {
                            if([output isEqualToString:@"ES: abc,123"]) {
                                NSLog(@"Welcome: %@", output);
                            }
                            else {
                                NSLog(@"You are not allowed: %@", output);
                            }
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

- (void) loginInfoReceived:(NSString *)info {
    NSLog(@"messageReceived");
    [loginInfo addObject:info];
}

@end

