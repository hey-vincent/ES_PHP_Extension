#include <iostream>
using namespace std;

int x = 1;


class A {
    public : int x; int y;

};

void after(A *p)
{
    cout << "I am " << p->x << endl;
}




int main(){
    A *pX = new A();
    after(pX);
    delete pX;

    cout << pX-> y << "???" << endl;



    printf("\n%p\n", pX);

    // auto func = [&](){
    //      printf("\n%d\n", pX->x);
    // };

    // func();
    return 0;
}