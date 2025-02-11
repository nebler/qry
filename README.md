# qry
build: 

cmake -DCMAKE_OSX_ARCHITECTURES=arm64 -G Ninja ..




# Concners/Ideas


## Symbol Table 

As of right now I will use a Tree as a Data Structure since I think it is the easiest. 
However look ups to the global (or any higher scope) will be getting more and more expensive the deeper nested the programms will be. 
If 