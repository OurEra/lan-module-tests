//
//  bmwCar.m
//  Demo2
//
//  Created by shiruiwei on 2020/2/9.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "bmwCar.h"

@implementation bmwCar

-(instancetype) init {
    
    NSLog(@"init bmwCar");
    if (self = [super init]) {
        [self setName:@"bmwcar"];
    }
    return self;
}



@end
