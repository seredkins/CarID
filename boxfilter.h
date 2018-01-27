#ifndef BOXFILTER_H
#define BOXFILTER_H

class BoxFilter{
private:
    int SIZE;
public:
    BoxFilter();
    BoxFilter(int);

    long long box[30][30];
    void set_size(int);
    int size(){ return SIZE; }
};

#endif // BOXFILTER_H
