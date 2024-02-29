//
//  UrlConnViewController.h
//  Demo2
//
//  Created by shiruiwei on 2020/2/7.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface UrlConnViewController : UIViewController
- (IBAction)buttonTouched:(id)sender;
- (IBAction)editExit:(id)sender;

@property (weak, nonatomic) IBOutlet UITextView *resultText;
@property (weak, nonatomic) IBOutlet UITextField *inputTextField;

@end

NS_ASSUME_NONNULL_END
