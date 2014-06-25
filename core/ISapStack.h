//
//  ISapStack.h
//  mystack
//
//  Created by mike on 14-6-23.
//  Copyright (c) 2014年 km. All rights reserved.
//

#ifndef mystack_ISapStack_h
#define mystack_ISapStack_h

class ISapStack
{
public:
    virtual void Start(int nThread) = 0;
    virtual void Stop() = 0;
    virtual ~ISapStack() {}
};

ISapStack *GetSapStackInstance();

#endif
