//
//  main.cpp
//  gamma
//
//  Created by Jian He on 2019/11/23.
//  Copyright © 2019 Jian He. All rights reserved.
//

#include <iostream>
#include <math.h>

double production(int n, double deltaN)
{
    double ret=1;
    for(int i=2;i<=n;++i)
    {
        ret*=(i+deltaN)/i;
    }
    return ret;
}

int main(int argc, const char * argv[]) {
    std::cout <<pow(1000+0.5,0.5)/production(1000, 0.5) <<"\n";
    return 0;
}
