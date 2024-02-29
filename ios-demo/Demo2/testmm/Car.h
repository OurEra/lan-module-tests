//
//  Car.h
//  Demo2
//
//  Created by shiruiwei on 2020/2/9.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#ifndef Car_h
#define Car_h


@protocol CarProtocal<NSObject>

-(void) Car:(NSString*) name CarStarted:(void*) message;
-(void) Car:(NSString*) name CarStopped:(void*) message;

@end

@interface Car : NSObject

-(void)dump;

@property (nonatomic, strong) NSString *name;

@end


#endif /* Car_h */
