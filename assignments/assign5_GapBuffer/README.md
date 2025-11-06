# GapBuffer
* In this programming assignment, implement a sequential container class, which is optimized for fast inserttion and deletion operations at any position. 
* For detailed description of HashMap and guidance on this assignment, you can refer to this [document](./doc/Short%20Assignment%203%20-%20Gap%20Buffer.pdf). 
* To build and test codes, can refer to the following commands. Can selectively enable testcases corresponding to the functionalities which have already implemented by changing macro definitions in the header of [`gap_buffer_test.cc`](./gap_buffer_test.cc).

```shell
mkdir -p build && cd build
cmake ..
make
./gap_buffer_test
```

# Here is what I have implemented
* Part 1: basic functions
    * `insert_at_cursor`: notice that when `_gap_size` is 0, we need to resize the buffer first.
    * `delete_at_cursor`: notice that when `_cursor_index` is 0, we do nothing and return.
    * `get_at_cursor` and `at`: use `to_array_index` to convert to array index and check when we need to throw `std::out_of_range` exception.
* Part 2: const-correctness
    * use `static_cast` and `const_cast` trick to reuse the implementation of non-const functions.
* Part 3: operator overloading
    * operator[]: notice bracket operator does not check bounds.
    * When implementing `==` and `<`, we need use a const_cast to turn into a non-const reference as the document said.
    * operator<<: notice the edge case.
* Part 4: turn everything into a template!
* Part 5: Implement iterators
    * One tricky thing is that I have to implemnet `reference operator*() const` to make `std::sort()` work in test cases.
    * notice the difference between `operator++` and `operator++(int)`.

* Part 6: Constructors and assignment
    * follow the rule of three and the convention of C++ to implement copy constructor, copy assignment operator.

* Part 7: Move semantics
    * follow the rule of five and the convention of C++ to implement move constructor, move assignment operator.

* Part 8: Make your code RAII-compliant - change the code throughout
    * use `unique_ptr` to manage memory.
    * change initialization of `_elems` to use `make_unique` in Part 1 constructor.
    * no need for manual deletion with unique_ptr in Part 6 destructor.
    * no need for manual deletion with unique_ptr in Part 6 copy assignment operator and change initialization of `_elems` to use `make_unique` as well in Part 6 constructor.
    * no need for manual deletion with unique_ptr in Part 7 move constructor and change initialization of `_elems` to use `make_unique` as well in Part 7 constructor.
    * change `_elems` to `_elems.get()` when do pointer arithmetic operation in `move_cursor` and `reserve` function.

* Part 9: Emplacement
    * use `std::forward` to forward arguments.