//
//  TestTableViewController.m
//  Demo2
//
//  Created by shiruiwei on 2020/2/14.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#import "TestTableViewController.h"
#import "tablecell/CustomTableViewCell.h"
// http://bcdilumonline.blogspot.com/2014/08/creating-tableview-in-xib.html?q=TableView
// https://www.iteye.com/blog/eric-gao-2234047

@interface TestTableViewController () <UITableViewDelegate, UITableViewDataSource>
@property (weak, nonatomic) IBOutlet UITableView *tableView;

@end

@implementation TestTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [_tableView registerNib:[UINib nibWithNibName:@"CustomTableViewCell" bundle:nil] forCellReuseIdentifier:@"tablecell"];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    
    NSLog(@"number rows");
    return 10;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    return 300;
}

- (UITableViewCell *)tableView:(UITableView *)tableView
         cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    CustomTableViewCell *cell = [_tableView dequeueReusableCellWithIdentifier:@"tablecell" forIndexPath:indexPath];
    
    //cell.textLabel.text = @"test";
    UILabel *label = [cell viewWithTag:102];
    label.text = [NSString stringWithFormat:@"测试 %zi", indexPath.row];
    
    [cell bringSubviewToFront:label];
    return cell;
}


/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
