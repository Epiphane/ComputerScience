//
//  track.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/12/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef __FinalProject__track__
#define __FinalProject__track__

#include "entity.h"

class Track : public Entity {
public:
    Track();
    
    void bend(float bend);
};

#endif /* defined(__FinalProject__track__) */
