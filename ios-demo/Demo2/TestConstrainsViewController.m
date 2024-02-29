//
//  TestConstrainsViewController.m
//  Demo2
//
//  Created by shiruiwei on 2020/2/6.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#import "TestConstrainsViewController.h"
#import "UrlConnViewController.h"
// https://www.youtube.com/watch?v=1j-Frd5snYc&list=PL23Revp-82LI-MTPyLtvzTCDl-vJKwjlU&index=4&t=0s

@interface TestConstrainsViewController ()

@end

@implementation TestConstrainsViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

    
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (IBAction)textDidExit:(id)sender {
    
    [sender resignFirstResponder];
    NSLog(@"sender");
}


- (IBAction)buttonTouch:(id)sender {
    
    UrlConnViewController *my2 = [[UrlConnViewController alloc] initWithNibName:@"My2ViewController" bundle:nil];
    [self presentViewController:my2 animated:YES completion:nil];
}
@end
