#include <iostream>
#include "IOStuff/program.h"

#include "menu.h"

//#define TRI_SORTED

using namespace std;

int main()
{
    Program p;
    ProgramStage::next_stage<Menu>();
    p.run();
    cout << "Hello World!" << endl;
    return 0;
}
