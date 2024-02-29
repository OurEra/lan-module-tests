//
//  UrlConnViewController.m
//  Demo2
//
//  Created by shiruiwei on 2020/2/7.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#import "UrlConnViewController.h"
// https://www.youtube.com/watch?v=bSM1xvBR_0g&list=PL2B8A5471837F2851&index=5

NSMutableData *dowmload;

@interface UrlConnViewController ()
@end

@implementation UrlConnViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

-(void)loadData:(NSString *)url {
    
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:url]];
    NSURLConnection *connection = [NSURLConnection connectionWithRequest:request delegate:self];
}

#pragma NSURLConnectionDataDelegate
-(void)connection:(NSURLConnection *) connection didReceiveData:(nonnull NSData *)data {
    
    [dowmload appendData:data];
}

-(void)connection:(NSURLConnection *) connection didReceiveResponse:(nonnull NSURLResponse *)response {
    
    dowmload = [NSMutableData data];
}

-(void)connection:(NSURLConnection *) connection didFailWithError:(nonnull NSError *)error {
    
}

-(void)connectionDidFinishLoading:(NSCondition *) connection {

    NSString *str = [[NSString alloc] initWithData:dowmload encoding:NSUTF8StringEncoding];
    _resultText.text = str;
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (IBAction)buttonTouched:(id)sender {
    
    NSString * url = _inputTextField.text;
    [self loadData:url];
    //[self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)editExit:(id)sender {
    [self resignFirstResponder];
}
@end
