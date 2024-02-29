//
//  CollectViewController.m
//  Demo2
//
//  Created by shiruiwei on 2020/2/13.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#import "CollectViewController.h"
#import "CustomCollectionViewCell.h"
// http://bcdilumonline.blogspot.com/2015/03/add-uicollectionview-to-xib-with-custom.html
// https://www.youtube.com/watch?v=SEzbgFbTT1A

@interface CollectViewController () <UICollectionViewDelegate, UICollectionViewDataSource>
@property (weak, nonatomic) IBOutlet UICollectionView *collectionView;
@property NSArray *cellImages;
@property NSArray *cellLables;

@end

@implementation CollectViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _cellImages = [[NSArray alloc] initWithObjects:@"girlimg1", @"girlimg2", @"girlimg3", @"girlimg4", nil];
    _cellLables = [[NSArray alloc] initWithObjects:@"美女1", @"美女2", @"美女3", @"美女4", nil];
    

    //Set Main Cell in Collection View
    [self.collectionView registerNib:[UINib nibWithNibName:@"CustomCollectionViewCell" bundle:nil] forCellWithReuseIdentifier:@"simplecell"];
    self.collectionView.backgroundColor = [UIColor clearColor];
    NSLog(@"ViewDidLoad");
}


#pragma mark : Collection View Datasource
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    
    NSLog(@"count %zi", _cellImages.count);
    return _cellImages.count;
}

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath {
    
    return CGSizeMake(500, 500);
}


- (CustomCollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    CustomCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"simplecell" forIndexPath:indexPath];
    
    UIImageView *uiimage = [cell viewWithTag:100];
    UILabel *uilabel = [cell viewWithTag:101];
    uiimage.image = [UIImage imageNamed:[_cellImages objectAtIndex:indexPath.row]];
    uilabel.text = [_cellLables objectAtIndex:indexPath.row];

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
