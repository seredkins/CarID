#include "boxfilter.h"


void BoxFilter::set_size(int size){
    if (size > 0 && size <= 10) SIZE = size;
    else SIZE = 10;
}

BoxFilter::BoxFilter(){
    set_size(3);
    for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                box[i][j] = 0;
}
