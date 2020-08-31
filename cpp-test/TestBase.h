//
//  TestBase.h
//  DemoTerm
//
//  Created by shiruiwei on 2020/8/9.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#ifndef TestBase_h
#define TestBase_h

class TestBase {
  
public:
    virtual void setUp() = 0;
    virtual void run()  = 0;
    virtual void tearDown() = 0;
    
};


#endif /* TestBase_h */
