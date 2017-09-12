//
//  ViewController.m
//  HelloWorld
//
//  Created by ptrujillo on 1/13/15.
//  Copyright (c) 2015 Stem. All rights reserved.
//

#import "LobbyViewController.h"

@interface LobbyViewController ()

@end

@implementation LobbyViewController

@synthesize lobbyView;
@synthesize inputStream, outputStream;
@synthesize inputMessageField;
@synthesize tView, messages;

- (void)viewDidLoad {
    //NSLog(@"viewDidLoad");
    [super viewDidLoad];
    [self initNetworkCommunication];
    messages = [[NSMutableArray alloc] init];
    
    self.tView.delegate = self;
    self.tView.dataSource = self;
}

- (void)didReceiveMemoryWarning {
    //NSLog(@"didReceiveMemoryWarning");
    [super didReceiveMemoryWarning];
}

/////View controllers/////
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"ChatCellIdentifier";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    NSString *s = (NSString *) [messages objectAtIndex:indexPath.row];
    cell.textLabel.text = s;
    return cell;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return messages.count;
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

- (IBAction)sendMessage {
    NSString *response  = [NSString stringWithFormat:@"%@", inputMessageField.text];
    NSData *data = [[NSData alloc] initWithData:[response dataUsingEncoding:NSASCIIStringEncoding]];
    [outputStream write:[data bytes] maxLength:[data length]];
    inputMessageField.text = @"";
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
    [self.tView reloadData];
}

@end

