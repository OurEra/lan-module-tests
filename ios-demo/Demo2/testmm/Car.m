//
//  Car.m
//  Demo2
//
//  Created by shiruiwei on 2020/2/9.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Car.h"

@implementation Car

-(instancetype) init {
    
    NSLog(@"init in Car");
    if (self = [super init]) {
        [self setName:@"car"];
    }
    return self;
}

-(void)dump {
    NSLog(@"name %@", _name);
}

@end
