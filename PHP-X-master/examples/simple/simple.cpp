#include <iostream>
#include <phpx.h>
#include <string>

using namespace std;
using namespace php;

void hello(Args &args, Variant &retval)
{
    if(args.count() != 1){
        error(E_NOTICE, "函数 'hello' 只接受1个参数, 你却任性地传了 %d 个 ", args.count());
    }

    string s = "Hello , " ;
    s.append(args[0].toString());
    retval = s;
    return;
}


PHPX_EXTENSION()
{
    Extension *pExtension = new Extension("HelloWolrd" , "1.0.0");

    pExtension->registerFunction("sayHello", hello);
    
    return pExtension;
}
