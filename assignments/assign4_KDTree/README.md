# KDTree
* In this programming assignment, implement a k-dimension tree which is designed for efficient storage and query of multi-dimension data. 
* For detailed description of KDTree and guidance on this assignment, you can refer to this [document](./doc/005_assignment_3_kdtree.pdf). 
* To build and test your codes, you can refer to following commands:
```shell
mkdir -p build && cd build
cmake ..
make
./kd_tree_test
```
* can selectively enable testcases corresponding to the functionalities you have already implemented by changing macro definitions in the header of [kd_tree_test.cc](./kd_tree_test.cc).

# Here is what I have implemented
* Implement Basic Functionality
    * for destructor, I write a helper function to delete the tree recursively.
    * as suggested in the document, I write one helper function `find_node` to implement the operator[]and at() function.
* Implement Nearest-Neighbor Lookup
    * just follow the pseudo code in the document and notice when we have to search the other subtree.
    * when find the most common label of the nearby points, I use a “challenge-like” strategy to implement it which has a time complexity of O(n).
* Implement Copy Functions
    * for copy constructor, I write a helper function to copy the tree recursively.
    * for assignment operator, can use the delete function (which appear in destructor) to delete the original tree and then copy the new tree.

 I also implement some extensions:
 * Build the kd-tree more intelligently.
    * use the median element as the root of the subtree.
    * note: you should sort the elements by the current dimension before finding the median.
* Implement Range Search
    * pay attention to the case when the query range intersects with the boundary of the subtree. If the query range intersects with the boundary, we need to search the other subtree.
* add support for element removal
    * this is a bit tricky, we need to pay attention to the case when the removed element has two children. In this case, we need to find the nearest neighbor of the removed element in the other subtree and replace the removed element with the nearest neighbor.
