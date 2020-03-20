#include <cstdlib>
#include "Command.h"
#include "debug.h"

# ifdef FS_DEBUG
#   include <iostream>
# endif

using namespace std;

  Command::Command(string str) : string(str){
  }
  int Command::run()const{
# ifdef FS_DEBUG
    cout <<"RUN: "<< c_str() << endl;
    return 0;
# else
    return system(c_str());
# endif
  }



