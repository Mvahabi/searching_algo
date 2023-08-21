# Assignment pa8 : Order, Red-Black Tree, and The Dictionary in C++ 
In overal, this assignment follows a binary search tree procedure. It can 
iterate through a tree and manioulate the tree in different ways. 
The Dictionary ADT maintains a set of pairs whose members are called key 
and value, respectively. It will enforce the constraint that all keys
are unique, while values may occur multiple times.


## Build
    
    $ make

## Running

### ./Order "input" "output"
### ./DictionaryTest

./Order will take 2 command line arguments:
In the "input" file, It will read in the lines of an input file, each
line being a (unique) single string, then insert these strings (as keys) into a Dictionary. The corresponding
values will be the line number in the input file where the string was read. Your program will write two
string representations of the Dictionary the output file. When printed, each pair appears on its own line. 
The keys will be in alphabetical order. The second representation will consist of keys only, one to a line, 
with the order obtained from a pre-order tree walk. 

No need to do anything for ./DictionaryTest
