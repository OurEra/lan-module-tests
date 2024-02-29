//
//  AppDelegate.m
//  Demo2
//
//  Created by shiruiwei on 2020/2/6.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#import "AppDelegate.h"
#import "TestConstrainsViewController.h"
#import "CollectViewController.h"
#import "UrlConnViewController.h"
#import "TestTableViewController.h"
@interface AppDelegate ()

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    self.window = [[UIWindow alloc]initWithFrame:[UIScreen mainScreen].bounds];
    self.window.backgroundColor = [UIColor whiteColor];
    
    //CollectViewController *mainViewController = [[CollectViewController alloc] init];
    //TestConstrainsViewController *mainViewController = [[TestConstrainsViewController alloc] init];
    //UrlConnViewController *mainViewController = [[UrlConnViewController alloc] init];
    TestTableViewController *mainViewController = [[TestTableViewController alloc] init];
    UINavigationController *navgationController = [[UINavigationController alloc] initWithRootViewController:mainViewController];
         
    self.window.rootViewController = navgationController;
    [self.window makeKeyAndVisible];
    
    return YES;
}


#pragma mark - UISceneSession lifecycle


- (UISceneConfiguration *)application:(UIApplication *)application configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession options:(UISceneConnectionOptions *)options {
    // Called when a new scene session is being created.
    // Use this method to select a configuration to create the new scene with.
    return [[UISceneConfiguration alloc] initWithName:@"Default Configuration" sessionRole:connectingSceneSession.role];
}


- (void)application:(UIApplication *)application didDiscardSceneSessions:(NSSet<UISceneSession *> *)sceneSessions {
    // Called when the user discards a scene session.
    // If any sessions were discarded while the application was not running, this will be called shortly after application:didFinishLaunchingWithOptions.
    // Use this method to release any resources that were specific to the discarded scenes, as they will not return.
}


@end
