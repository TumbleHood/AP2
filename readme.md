# Advanced Programming
## Assignment 2 - Guy Gur-Arieh
\
To compile the program, type:
```ruby
make coder
```
To change the number of threads in the thread pool, go to ```coder.h``` and change the macro ```THREAD_NUM``` to the desired number of threads, then compile again.
```c
#define THREAD_NUM 4
```

\
To run the program, type "./coder", then a key (number), then a flag.\
To use the encrypt feature, use the "-e" flag.\
To use the decrypt feature, use the "-d" flag.\
An input file is required but an output file is optional.
```ruby
./coder [KEY] -e/-d < [INPUT FILE]
./coder [KEY] -e/-d < [INPUT FILE] > [OUTPUT FILE]
```

## Example
```ruby
./coder 3 -e < script.txt > e.txt
./coder 3 -d < e.txt > d.txt
diff script.txt d.txt
```
Running "diff" makes sure that the decrypted file indeed matches the original file.