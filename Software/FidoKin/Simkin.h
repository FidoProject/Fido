#ifndef __FidoKin__Simkin__
#define __FidoKin__Simkin__

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Simkin {
public:
    Simkin(int robWidth,int robHeight);
    
private:
    void updateLoop();
    
    int mLeft, mRight;
};


#endif /* defined(__FidoKin__Simkin__) */
