# qry
build: 

cmake -DCMAKE_OSX_ARCHITECTURES=arm64 -G Ninja ..




# Concners/Ideas


## Symbol Table 

As of right now I will use a Tree as a Data Structure since I think it is the easiest. 
However look ups to the global (or any higher scope) will be getting more and more expensive the deeper nested the programms will be. 


### Tracking of usages
As of right now the usages tracking is not working as I would like it to be example:

var a = 1 + 1;
var b = a + 1;

The symbol table will have the usage recorded but only that a is being used as an identifier expression not that its part of a var decleration.


var foofer2: Foo(Foo.a + 1, Foo.b == true, Foo.c);