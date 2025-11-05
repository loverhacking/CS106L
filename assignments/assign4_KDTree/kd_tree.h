/**
 * File: kd_tree.h
 * Author: zjy
 * ------------------------
 * An interface representing a kd-tree in some number of dimensions. The tree
 * can be constructed from a set of data and then queried for membership and
 * nearest neighbors.
 */

#ifndef KDTREE_INCLUDED
#define KDTREE_INCLUDED

#include <cstddef>
#include <map>
#include "point.h"
#include "math.h"
#include "bounded_priority_queue.h"

// "using namespace" in a header file is conventionally frowned upon, but I'm
// including it here so that you may use things like size_t without having to
// type std::size_t every time.

template <size_t N, typename ElemType>
class KDTree {
public:
    struct Node {
        Point<N> point;
        ElemType element;
        Node* left_node;
        Node* right_node;
    };
    
    // Constructor: KDTree();
    // Usage: KDTree<3, int> myTree;
    // ----------------------------------------------------
    // Constructs an empty KDTree.
    KDTree();
    
    // Constructor: KDTree(Iterator begin, Iterator end);
    // Usage: KDTree<2, int> tree(points.begin(), points.end());
    // ----------------------------------------------------
    // Constructs a balanced KDTree from a range of points and values.
    // The input range should be a sequence of std::pair<Point<N>, ElemType>.
    template <typename Iterator>
    KDTree(Iterator begin, Iterator end);
    
    // Destructor: ~KDTree()
    // Usage: (implicit)
    // ----------------------------------------------------
    // Cleans up all resources used by the KDTree.
    ~KDTree();
    
    // KDTree(const KDTree& rhs);
    // KDTree& operator=(const KDTree& rhs);
    // Usage: KDTree<3, int> one = two;
    // Usage: one = two;
    // -----------------------------------------------------
    // Deep-copies the contents of another KDTree into this one.
    KDTree(const KDTree& rhs);
    KDTree& operator=(const KDTree& rhs);
    
    // size_t dimension() const;
    // Usage: size_t dim = kd.dimension();
    // ----------------------------------------------------
    // Returns the dimension of the points stored in this KDTree.
    size_t dimension() const;
    
    // size_t size() const;
    // bool empty() const;
    // Usage: if (kd.empty())
    // ----------------------------------------------------
    // Returns the number of elements in the kd-tree and whether the tree is
    // empty.
    size_t size() const;
    bool empty() const;
    
    // bool contains(const Point<N>& pt) const;
    // Usage: if (kd.contains(pt))
    // ----------------------------------------------------
    // Returns whether the specified point is contained in the KDTree.
    bool contains(const Point<N>& pt) const;
    
    // void insert(const Point<N>& pt, const ElemType& value);
    // Usage: kd.insert(v, "This value is associated with v.");
    // ----------------------------------------------------
    // Inserts the point pt into the KDTree, associating it with the specified
    // value. If the element already existed in the tree, the new value will
    // overwrite the existing one.
    void insert(const Point<N>& pt, const ElemType& value);
    
    // ElemType& operator[](const Point<N>& pt);
    // Usage: kd[v] = "Some Value";
    // ----------------------------------------------------
    // Returns a reference to the value associated with point pt in the KDTree.
    // If the point does not exist, then it is added to the KDTree using the
    // default value of ElemType as its key.
    ElemType& operator[](const Point<N>& pt);
    
    // ElemType& at(const Point<N>& pt);
    // const ElemType& at(const Point<N>& pt) const;
    // Usage: cout << kd.at(v) << endl;
    // ----------------------------------------------------
    // Returns a reference to the key associated with the point pt. If the point
    // is not in the tree, this function throws an out_of_range exception.
    ElemType& at(const Point<N>& pt);
    const ElemType& at(const Point<N>& pt) const;
    
    // ElemType kNNValue(const Point<N>& key, size_t k) const
    // Usage: cout << kd.kNNValue(v, 3) << endl;
    // ----------------------------------------------------
    // Given a point v and an integer k, finds the k points in the KDTree
    // nearest to v and returns the most common value associated with those
    // points. In the event of a tie, one of the most frequent value will be
    // chosen.
    ElemType kNNValue(const Point<N>& key, size_t k) const;
    
    // vector<pair<Point<N>, ElemType>> rangeSearch(const Point<N>& lower, const Point<N>& upper) const
    // Usage: auto results = kd.rangeSearch(lowerBound, upperBound);
    // ----------------------------------------------------
    // Finds all points in the KDTree that lie within the specified rectangular range.
    // The range is defined by the lower and upper bounds, where each component of lower
    // must be less than or equal to the corresponding component of upper.
    vector<pair<Point<N>, ElemType>> rangeSearch(const Point<N>& lower, const Point<N>& upper) const;
    
    // bool remove(const Point<N>& pt);
    // Usage: if (kd.remove(pt)) {/* point was removed */}
    // ----------------------------------------------------
    // Removes the point pt from the KDTree if it exists. Returns true if the
    // point was found and removed, false if the point was not in the tree.
    bool remove(const Point<N>& pt);

private:
    // TODO: Add implementation details here.
    size_t tree_size;
    Node* root;

    // Recursively deletes all nodes in the KDTree.
    void delete_tree(Node* node);

    // Compares the point pt with the node's point along the dimension dim.
    // Returns a negative value if pt is less than the node's point, a positive
    // value if pt is greater than the node's point, and 0 if they are equal.
    int compare(const Point<N>& pt, const Node* node, size_t dim) const;

    // Recursively finds the node with the point pt.
    Node* findNode(const Point<N>& pt) const;

    // Recursively finds the k nearest neighbors of key in the subtree rooted at node.
    void findKNearest(const Point<N>& key, const Node* node
        , size_t dim, BoundedPriorityQueue<pair<Point<N>, ElemType>>& pq) const;

    // Recursively copies the subtree rooted at rhs to the subtree rooted at root.
    void copyTree(Node*& root, const Node* rhs);
    
    // Helper function to build a balanced KDTree recursively
    // Takes a vector of pairs (points and values), sorts by the current dimension,
    // uses the median as the root, and recursively builds left and right subtrees.
    template <typename Iterator>
    Node* buildTree(Iterator begin, Iterator end, size_t dim);
    
    // Helper function to perform a range search recursively
    // Searches the subtree rooted at node for points within the range [lower, upper]
    // and adds them to the results vector.
    void rangeSearchRecursive(const Node* node, size_t dim, const Point<N>& lower, 
                              const Point<N>& upper, vector<pair<Point<N>, ElemType>>& results) const;
    
    // Helper function to check if a point is within the range [lower, upper]
    bool isPointInRange(const Point<N>& point, const Point<N>& lower, const Point<N>& upper) const;
    
    // Helper function to remove a node with the given point from the KDTree
    Node* removeNode(Node* node, const Point<N>& pt, size_t dim);
    
    // Helper function to find the minimum node in a subtree along a specific dimension
    Node* findMinNode(Node* node, size_t dim, size_t currentDim);
    
    // Helper function to find and remove the minimum node in a subtree along a specific dimension
    Node* removeMinNode(Node* node, size_t dim, size_t currentDim);
    
};

/*
 * Step One: Implement Basic Functionality
 */
template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() {
    tree_size = 0;
    root = nullptr;
}

// Recursively deletes all nodes in the KDTree.
template<size_t N, typename ElemType>
void KDTree<N, ElemType>::delete_tree(Node* node) {
    if (node == nullptr) {
        return;
    }
    delete_tree(node->left_node);
    delete_tree(node->right_node);
    delete node;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
    delete_tree(root);
    tree_size = 0;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
    return N;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::size() const {
    return tree_size;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const {
    return tree_size == 0;
}

// Compares the point pt with the node's point along the dimension dim.
// Returns a negative value if pt is less than the node's point, a positive
// value if pt is greater than the node's point, and 0 if they are equal.
template <size_t N, typename ElemType>
int KDTree<N, ElemType>::compare(const Point<N>& pt, const Node* node, size_t dim) const {
    // Using a small epsilon for floating point comparisons to avoid precision issues
    if (pt[dim] < node->point[dim]) return -1;
    if (pt[dim] > node->point[dim]) return 1;
    return 0;
}

// Inserts the point pt into the KDTree, associating it with the specified
// value. If the element already existed in the tree, the new value will
// overwrite the existing one.
template <size_t N, typename ElemType>
void KDTree<N, ElemType>::insert(const Point<N>& pt, const ElemType& value) {
    if (root == nullptr) {
        root = new Node{pt, value, nullptr, nullptr};
        tree_size++;
        return;
    }
        
    int tree_level = 0;    // tree_level: the level of the tree to insert
    Node* current = root;
    while (true) {
        int comp_result = compare(pt, current, tree_level % N);
        // If the points are identical across all dimensions, update the value
        if (pt == current->point) {
            current->element = value;
            return;
        } else if (comp_result < 0) {
            if (current->left_node == nullptr) {
                current->left_node = new Node{pt, value, nullptr, nullptr};
                tree_size++;
                return;
            } else {
                tree_level++;
                current = current->left_node;
            }
        } else { // comp_result >= 0
            if (current->right_node == nullptr) {
                current->right_node = new Node{pt, value, nullptr, nullptr};
                tree_size++;
                return;
            } else {
                tree_level++;
                current = current->right_node;
            }
        }
    }
}

// Recursively finds the node with the point pt.
template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::Node* KDTree<N, ElemType>::findNode(const Point<N>& pt) const {
    Node* cur = root;
    int tree_level = 0;    // tree_level: the level of the tree to compare with
    while (cur != nullptr) {
        // First check if the points are identical across all dimensions
        if (pt == cur->point) {
            return cur;
        }
        
        int comp_result = compare(pt, cur, tree_level % N);
        if (comp_result < 0) {
            tree_level++;
            cur = cur->left_node;
        } else {
            tree_level++;
            cur = cur->right_node;
        }
    }
    return nullptr;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N>& pt) const {
    return findNode(pt) != nullptr;
}

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::operator[](const Point<N>& pt) {
    Node* node = findNode(pt);
    if (node == nullptr) {
        insert(pt, ElemType());
        node = findNode(pt);
    }
    return node->element;
}

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::at(const Point<N>& pt) {
    Node* node = findNode(pt);
    if (node == nullptr) {
        throw std::out_of_range("Point not found");
    }
    return node->element;
}

template <size_t N, typename ElemType>
const ElemType& KDTree<N, ElemType>::at(const Point<N>& pt) const {
    const Node* node = findNode(pt);
    if (node == nullptr) {
        throw std::out_of_range("Point not found");
    }
    return const_cast<const ElemType&>(node->element);
}

/*
 * Step Two: Implement Nearest-Neighbor Lookup
 */

// Recursively finds the k nearest neighbors of the key point.
template <size_t N, typename ElemType>
void KDTree<N, ElemType>::findKNearest(const Point<N>& key, const Node* node
    , size_t dim, BoundedPriorityQueue<pair<Point<N>, ElemType>>& pq) const {
    if (node == nullptr) {
        return;
    }

    // Calculate the distance from the key to the current node's point
    double dist = Distance(key, node->point);
    
    // Add current node to the priority queue
    pq.enqueue(make_pair(node->point, node->element), dist);
    
    // Determine which child to visit first
    const Node* firstChild = nullptr;
    const Node* secondChild = nullptr;
    
    if (compare(key, node, dim) < 0) {
        firstChild = node->left_node;
        secondChild = node->right_node;
    } else {
        firstChild = node->right_node;
        secondChild = node->left_node;
    }
    
    // Visit the child that is more likely to contain closer points first
    findKNearest(key, firstChild, (dim + 1) % N, pq);
    
    // Check if we need to visit the other child
    // The distance along the current dimension to the node's plane
    double planeDist = fabs(key[dim] - node->point[dim]);
    
    // If the priority queue isn't full or the plane distance is less than the
    // worst distance in the queue, we need to check the other branch
    if (pq.size() < pq.maxSize() || planeDist < pq.worst()) {
        findKNearest(key, secondChild, (dim + 1) % N, pq);
    }
}

template <size_t N, typename ElemType>
ElemType KDTree<N, ElemType>::kNNValue(const Point<N>& key, size_t k) const {
    // Create a bounded priority queue to store the k nearest neighbors
    BoundedPriorityQueue<pair<Point<N>, ElemType>> pq(k);
    
    // Find the k nearest neighbors
    findKNearest(key, root, 0, pq);

    // Find the most common value among the k nearest neighbors
    pair<Point<N>, ElemType> mostFreq = pq.dequeueMin();
    ElemType mostCommon = mostFreq.second;
    int maxFreq = 1;
    while (!pq.empty()) {
        pair<Point<N>, ElemType> current = pq.dequeueMin();
        if (current.second == mostCommon) {
            maxFreq++;
        } else {
            maxFreq--;
        }
        if (maxFreq == 0) {
            maxFreq = 1;
            mostCommon = current.second;
        }
    }
    return mostCommon;  
}

/*
 * Step Three: Implement Copy Functions
 */

// Recursively copies the subtree rooted at rhs to the subtree rooted at root.
template <size_t N, typename ElemType>
void KDTree<N, ElemType>::copyTree(Node*& root, const Node* rhs) {
    if (rhs == nullptr) {
        root = nullptr;
    } else {
        root = new Node{rhs->point, rhs->element, nullptr, nullptr};
        copyTree(root->left_node, rhs->left_node);
        copyTree(root->right_node, rhs->right_node);
    }
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree(const KDTree& rhs) {
    root = nullptr;
    tree_size = 0;
    copyTree(root, rhs.root);
    tree_size = rhs.tree_size;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>& KDTree<N, ElemType>::operator=(const KDTree& rhs) {
    if (this != &rhs) {
        tree_size = 0;
        delete_tree(root);
        copyTree(root, rhs.root);
        tree_size = rhs.tree_size;
    }
    return *this;
}

/*
 * Extensions: Build the kd-tree more intelligently.
 * You can use the median element as the root of the subtree.
 * Note: You should sort the elements by the current dimension before finding the median.
*/

// Implementation of the iterator-based constructor to build a balanced KDTree
template <size_t N, typename ElemType>
template <typename Iterator>
KDTree<N, ElemType>::KDTree(Iterator begin, Iterator end) {
    // First, copy the data into a vector to allow for sorting and median selection
    vector<pair<Point<N>, ElemType>> elements(begin, end);
    tree_size = elements.size();
    
    // Build the balanced tree recursively, starting with dimension 0
    root = buildTree(elements.begin(), elements.end(), 0);
}

// Helper function to build a balanced KDTree recursively
template <size_t N, typename ElemType>
template <typename Iterator>
typename KDTree<N, ElemType>::Node* KDTree<N, ElemType>::buildTree(Iterator begin, Iterator end, size_t dim) {
    // Base case: empty range
    if (begin == end) {
        return nullptr;
    }

    // Comparison function for sorting points by a specific dimension
    auto CompareByDimension = [](const pair<Point<N>, ElemType>& a, const pair<Point<N>, ElemType>& b, size_t dim) {
        return a.first[dim] < b.first[dim];
    };
    
    // Sort the range by the current dimension
    sort(begin, end, CompareByDimension(dim));

    // Find the median element
    Iterator median = begin + (end - begin) / 2;
    
    // Create a new node with the median element
    Node* node = new Node{median->first, median->second, nullptr, nullptr};
    
    // Recursively build left and right subtrees
    // Left subtree contains all elements before the median
    node->left_node = buildTree(begin, median, (dim + 1) % N);
    
    // Right subtree contains all elements after the median
    Iterator nextMedian = median;
    ++nextMedian;
    node->right_node = buildTree(nextMedian, end, (dim + 1) % N);
    
    return node;
}

/*
 * Extensions: Implement Range Search
 */

// Helper function to check if a point is within the range [lower, upper]
template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::isPointInRange(const Point<N>& point, const Point<N>& lower, const Point<N>& upper) const {
    for (size_t i = 0; i < N; ++i) {
        // If any dimension is outside the range, the point is not in the range
        if (point[i] < lower[i] || point[i] > upper[i]) {
            return false;
        }
    }
    return true;
}

// Helper function to perform a range search recursively
template <size_t N, typename ElemType>
void KDTree<N, ElemType>::rangeSearchRecursive(const Node* node, size_t dim, const Point<N>& lower, 
                                              const Point<N>& upper, vector<pair<Point<N>, ElemType>>& results) const {
    if (node == nullptr) {
        return;
    }
    
    // Check if the current node's point is within the range
    if (isPointInRange(node->point, lower, upper)) {
        results.push_back(make_pair(node->point, node->element));
    }
    
    // Determine which child subtrees to search
    // The current dimension's value determines whether we need to search left and/or right
    bool searchLeft = (node->point[dim] >= lower[dim]);
    bool searchRight = (node->point[dim] <= upper[dim]);
    
    // Search the left subtree if necessary
    if (searchLeft) {
        rangeSearchRecursive(node->left_node, (dim + 1) % N, lower, upper, results);
    }
    
    // Search the right subtree if necessary
    if (searchRight) {
        rangeSearchRecursive(node->right_node, (dim + 1) % N, lower, upper, results);
    }
}

// Finds all points in the KDTree that lie within the specified rectangular range
template <size_t N, typename ElemType>
vector<pair<Point<N>, ElemType>> KDTree<N, ElemType>::rangeSearch(const Point<N>& lower, const Point<N>& upper) const {
    vector<pair<Point<N>, ElemType>> results;
    
    // Validate input ranges
    for (size_t i = 0; i < N; ++i) {
        if (lower[i] > upper[i]) {
            throw std::invalid_argument("Lower bound cannot be greater than upper bound");
        }
    }
    
    // Perform the recursive range search
    rangeSearchRecursive(root, 0, lower, upper, results);
    
    return results;
}

/*
 * Extensions: Add support for element removal. 
 */

// Public remove method implementation
template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::remove(const Point<N>& pt) {
    // Check if tree is empty
    if (root == nullptr) {
        return false;
    }
    
    // Use recursive helper function to remove the node
    size_t initialSize = tree_size;
    root = removeNode(root, pt, 0);
    
    // If size decreased, the node was removed
    if (tree_size < initialSize) {
        return true;
    }
    return false;
}

// Helper function to remove a node with the given point from the KDTree
template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::Node* KDTree<N, ElemType>::removeNode(Node* node, const Point<N>& pt, size_t dim) {
    if (node == nullptr) {
        return nullptr;
    }
    
    // First check if this is the node we want to remove
    if (node->point == pt) {
        // Decrement the tree size
        tree_size--;
        
        // Case 1: Node has no children (leaf node)
        if (node->left_node == nullptr && node->right_node == nullptr) {
            delete node;
            return nullptr;
        }
        
        // Case 2: Node has only one child
        if (node->left_node == nullptr) {
            Node* rightChild = node->right_node;
            delete node;
            return rightChild;
        }
        if (node->right_node == nullptr) {
            Node* leftChild = node->left_node;
            delete node;
            return leftChild;
        }
        
        // Case 3: Node has two children
        // Find the minimum node in the right subtree along the current dimension
        Node* minNode = findMinNode(node->right_node, dim, (dim + 1) % N);
        
        // Copy the minNode's data to the current node
        node->point = minNode->point;
        node->element = minNode->element;
        
        // Remove the minNode from the right subtree
        node->right_node = removeMinNode(node->right_node, dim, (dim + 1) % N);
        
        return node;
    }
    
    // Determine which subtree to search
    int compResult = compare(pt, node, dim);
    if (compResult < 0) {
        node->left_node = removeNode(node->left_node, pt, (dim + 1) % N);
    } else {
        node->right_node = removeNode(node->right_node, pt, (dim + 1) % N);
    }
    
    return node;
}

// Helper function to find the minimum node in a subtree along a specific dimension
template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::Node* KDTree<N, ElemType>::findMinNode(Node* node, size_t dim, size_t currentDim) {
    if (node == nullptr) {
        return nullptr;
    }
    
    // If we're at the target dimension, we need to check the left subtree first
    if (currentDim == dim) {
        if (node->left_node == nullptr) {
            return node;
        }
        return findMinNode(node->left_node, dim, (currentDim + 1) % N);
    }
    
    // For other dimensions, we need to check all three possibilities:
    // current node, minimum in left subtree, and minimum in right subtree
    Node* currentMin = node;
    Node* leftMin = findMinNode(node->left_node, dim, (currentDim + 1) % N);
    Node* rightMin = findMinNode(node->right_node, dim, (currentDim + 1) % N);
    
    if (leftMin != nullptr && leftMin->point[dim] < currentMin->point[dim]) {
        currentMin = leftMin;
    }
    if (rightMin != nullptr && rightMin->point[dim] < currentMin->point[dim]) {
        currentMin = rightMin;
    }
    
    return currentMin;
}

// Helper function to find and remove the minimum node in a subtree along a specific dimension
template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::Node* KDTree<N, ElemType>::removeMinNode(Node* node, size_t dim, size_t currentDim) {
    if (node == nullptr) {
        return nullptr;
    }
    
    // If we're at the target dimension, check if this is the minimum node
    if (currentDim == dim) {
        if (node->left_node == nullptr) {
            // This is the minimum node, return its right child to replace it
            Node* rightChild = node->right_node;
            delete node;
            return rightChild;
        }
        // Continue searching in the left subtree
        node->left_node = removeMinNode(node->left_node, dim, (currentDim + 1) % N);
        return node;
    }
    
    // For other dimensions, we need to check both subtrees
    node->left_node = removeMinNode(node->left_node, dim, (currentDim + 1) % N);
    node->right_node = removeMinNode(node->right_node, dim, (currentDim + 1) % N);
    
    return node;
}

#endif
