# HashMap

## Preview
* TASK: implement an associative container class which has similar functionalities as [unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map) provided in the STL. 
* For detailed description of HashMap and guidance on this assignment, you can refer to this [document](./doc/HashMap_doc.pdf). 
* To build and test codes, can refer to following commands. And can selectively enable testcases corresponding to the functionalities you have already implemented by changing macro definitions in the [test_settings.h](./test_settings.h).
```shell
mkdir -p build && cd build
cmake ..
make
# functionality test
./hashmap_test
# performance test
./hashmap_perf
```
## What I have done in assign3

* Milestone 2: Iterator-Based Constructors
    * reuse previous constructor and iterator calss 
* Milestone 3: Operator Overloading
    * []: reuse `insert` method
    * ==: use `std::is_permutation`
    * !=: use ==
    * <<: use ostringstream
* Milestone 4: Implement Special Member Functions
    * Copy constructor: reuse construtor and insert
    * Copy assignment operator: 
        * avoid self assigning
        * clear befor reassign
    * Move constructor:
        * iterate over `_buckets_array` to move and set every element to nullptr
    * Move assignment operator:
        * avoid self assigning
        * resize `_buckets_array` 
        * clear befor reassign
* Extensions: Implement try_emplace