#include "hashmap.h"
#include "hashmap_iterator.h"
#include <algorithm>
#include <cstddef>
#include <stdexcept>

template<typename K, typename M, typename H>
HashMap<K, M, H>::HashMap() : _size(0), _hash_function(H()), _buckets_array(kDefaultBuckets, nullptr) {};

template<typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(size_t bucket_count, const H& hash):
    _size(0), 
    _hash_function(hash), 
    _buckets_array(bucket_count, nullptr) {};

template<typename K, typename M, typename H>
HashMap<K, M, H>::~HashMap() {
    clear();
}

template<typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::size() const{
    return _size;
}

template<typename K, typename M, typename H>
inline bool HashMap<K, M, H>::empty() const{
    return _size == 0;
}

template<typename K, typename M, typename H>
inline float HashMap<K, M, H>::load_factor() const{
    return static_cast<float>(_size) / bucket_count();
}

template<typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::bucket_count() const{
    return _buckets_array.size();
}

template<typename K, typename M, typename H>
bool HashMap<K, M, H>::contains(const K& key) const{
    return find_node(key).second != nullptr;
}

template<typename K, typename M, typename H>
M& HashMap<K, M, H>::at(const K& key) {
    auto [prev, node] = find_node(key);
    if (node == nullptr) {
        throw std::out_of_range("HashMap<K, M, H>::at: key not found");
    }
    return node->value.second;
}

template <typename K, typename M, typename H>
const M& HashMap<K, M, H>::at(const K& key) const {
    // This is called the static_cast/const_cast trick, which allows us to reuse
    // the non-const version of find to implement the const version.
    // The idea is to cast this so it's pointing to a non-const HashMap, which
    // calls the overload above (and prevent infinite recursion).
    // Also note that we are calling the conversion operator in the iterator class!
    return static_cast<const M&>(const_cast<HashMap<K, M, H>*>(this)->at(key));
}
   
template <typename K, typename M, typename H>
void HashMap<K, M, H>::clear() {
    for (auto& bucket : _buckets_array) {
        while (bucket != nullptr) {
            auto to_delete = bucket;
            bucket = bucket->next;
            delete to_delete;
        }
    }
    _size = 0;
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::find(const K& key) {
    return make_iterator(find_node(key).second);
}

template<typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::find(const K& key) const {
    // This is called the static_cast/const_cast trick, which allows us to reuse
    // the non-const version of find to implement the const version.
    // The idea is to cast this so it's pointing to a non-const HashMap, which
    // calls the overload above (and prevent infinite recursion).
    // Also note that we are calling the conversion operator in the iterator class!
    return static_cast<const_iterator>(const_cast<HashMap<K, M, H>*>(this)->find(key));
}

template <typename K, typename M, typename H>
std::pair<typename HashMap<K, M, H>::iterator, bool> HashMap<K, M, H>::insert(const value_type& key_value) {
    const auto& [key, value] = key_value;
    auto [prev, node_to_edit] = find_node(key);
    size_t index = _hash_function(key) % bucket_count();

    if (node_to_edit != nullptr) {
        return {make_iterator(node_to_edit), false};
    }

    auto temp = new Node(key_value, _buckets_array[index]);
    _buckets_array[index] = temp;
    ++_size;
    return {make_iterator(temp), true};
}

// Implementation of try_emplace using perfect forwarding and in-place construction
template <typename K, typename M, typename H>
template <typename... Args>
std::pair<typename HashMap<K, M, H>::iterator, bool> HashMap<K, M, H>::try_emplace(const K& key, Args&&... args) {
    // Check if key already exists
    auto [prev, node_to_edit] = find_node(key);
    
    // If key exists, return iterator to existing node and false
    if (node_to_edit != nullptr) {
        return {make_iterator(node_to_edit), false};
    }
    
    // Calculate bucket index (after potential rehash)
    size_t index = _hash_function(key) % bucket_count();
    
    // Create a new node with true in-place construction of the value
    // This eliminates all unnecessary copies by constructing the mapped value directly
    // inside the Node's value member
    auto temp = new Node(
        std::piecewise_construct,
        std::forward_as_tuple(key),
        std::forward_as_tuple(std::forward<Args>(args)...)
    );
    
    // Set the next pointer after construction
    temp->next = _buckets_array[index];
    
    // Update the bucket's linked list
    _buckets_array[index] = temp;
    ++_size;
    
    // Return iterator to new node and true
    return {make_iterator(temp), true};
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::node_pair HashMap<K, M, H>::find_node(const K& key) const{
    size_t index = _hash_function(key) % bucket_count();
    Node* cur = _buckets_array[index];
    Node* prev = nullptr; // if first node is the key, return {nullptr, front}
    while (cur != nullptr) {
        const auto& [k, v] = cur->value;
        if (k == key) {
            return {prev, cur};
        }
        prev = cur;
        cur = cur->next;
    }
    return {nullptr, nullptr};  // key not found at all.
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::begin() {
    size_t index = first_not_empty_bucket();
    if (index == bucket_count()) {
        return end();
    }
    return make_iterator(_buckets_array[index]);
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::begin() const {
    return static_cast<const_iterator>(const_cast<HashMap<K, M, H>*>(this)->begin());
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::end() {
    return make_iterator(nullptr);
}

template<typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::end() const {
    return const_cast<HashMap<K, M, H> *>(this)->end();
}

template <typename K, typename M, typename H>
size_t HashMap<K, M, H>::first_not_empty_bucket() const {
    auto isNotNullptr = [ ](const auto& v){
        return v != nullptr;
    };

    auto found = std::find_if(_buckets_array.begin(), _buckets_array.end(), isNotNullptr);
    return found - _buckets_array.begin();
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::make_iterator(Node* curr) {
    if (curr == nullptr) {
        return {&_buckets_array, curr, bucket_count()};
    }
    size_t index = _hash_function(curr->value.first) % bucket_count();
    return {&_buckets_array, curr, index};
}

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::erase(const K& key) {
    auto [prev, node_to_erase] = find_node(key);
    if (node_to_erase == nullptr) {
        return false;
    }
    size_t index = _hash_function(key) % bucket_count();
    // if the prev is nullptr, it means the node_to_erase is the first node in the bucket
    (prev ? prev->next : _buckets_array[index]) = node_to_erase->next;
    --_size;
    delete node_to_erase;
    return true;
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::erase(typename HashMap<K, M, H>::const_iterator pos) {
    erase(pos++->first);
    return make_iterator(pos._node); // unfortunately we need a regular iterator, not a const_iterator
}

template <typename K, typename M, typename H>
    void HashMap<K, M, H>::debug() {
    std::cout << std::setw(30) << std::setfill('-') << '\n' << std::setfill(' ')
          << "Printing debug information for your HashMap implementation\n"
          << "Size: " << size() << std::setw(15) << std::right
          << "Buckets: " << bucket_count() << std::setw(20) << std::right
          << "(load factor: " << std::setprecision(2) << load_factor() << ") \n\n";

    for (size_t i = 0; i < bucket_count(); ++i) {
        std::cout << "[" << std::setw(3) << i << "]:";
        Node* curr = _buckets_array[i];
        while (curr != nullptr) {
            const auto& [key, mapped] = curr->value;
            // next line will not compile if << not supported for K or M
            std::cout <<  " -> " << key << ":" << mapped;
            curr = curr->next;
        }
        std::cout <<  " /" <<  '\n';
    }
    std::cout << std::setw(30) << std::setfill('-') << '\n' << std::setfill(' ');
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::rehash(size_t new_bucket_count) {
    if (new_bucket_count == 0) {
        throw std::out_of_range("HashMap<K, M, H>::rehash: new_bucket_count must be positive.");
    }

    std::vector<Node*> new_buckets_array(new_bucket_count, nullptr);
    for (auto& curr : _buckets_array) {
        while (curr != nullptr) {
            const auto& [key, mapped] = curr->value;
            size_t new_index = _hash_function(key) % new_bucket_count;
            auto temp = curr;
            curr = curr->next;
            temp->next = new_buckets_array[new_index];
            new_buckets_array[new_index] = temp;
        }
    }
    _buckets_array = std::move(new_buckets_array);  
}

/* Begin Milestone 2: Iterator-Based Constructos */
template <typename K, typename M, typename H>
template <typename InputIt>
HashMap<K, M, H>::HashMap(InputIt first, InputIt last, size_t bucket_count, const H& hash) : 
    HashMap(bucket_count, hash) {
    for (auto it = first; it != last; ++it) {
        insert(*it);
    }
}

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(std::initializer_list<value_type> init, size_t bucket_count, const H& hash) : 
    HashMap{init.begin(), init.end(), bucket_count, hash} {
}
/* end student code */

/* Begin Milestone 3: Operator Overloading */

template <typename K, typename M, typename H>
M& HashMap<K, M, H>::operator[](const K& key) {
    return insert({key, {}}).first->second;
}

template <typename K, typename M, typename H>
bool operator==(const HashMap<K, M, H>& lhs, const HashMap<K, M, H>& rhs) {
    // std::is_permutation checks if two ranges are permutations of each other
    return lhs.size() == rhs.size() && std::is_permutation(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename K, typename M, typename H>
bool operator!=(const HashMap<K, M, H>& lhs, const HashMap<K, M, H>& rhs) {
    return !(lhs == rhs);
}

template <typename K, typename M, typename H>
std::ostream& operator<<(std::ostream& os, const HashMap<K, M, H>& rhs) {
    // std::ostringstream::ate appends the string to the end of the {
    std::ostringstream oss("{", std::ostringstream::ate);
    for (const auto& [key, mapped] : rhs) {
        oss << key << ":" << mapped << ", ";
    }
    std::string s = oss.str();
    os << s.substr(0, s.length()-2) << "}";
    return os;
}

/* end student code */


/* Begin Milestone 4: Implement Special Member Functions */
// Copy constructor
template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(const HashMap<K, M, H>& map) : 
    HashMap(map.bucket_count(), map._hash_function) {
    for (const auto& [key, mapped] : map) {
        insert({key, mapped});
    }
}

// Copy assignment operator
template <typename K, typename M, typename H>
HashMap<K, M, H>& HashMap<K, M, H>::operator=(const HashMap<K, M, H>& map) {
    if (this != &map) {
        clear();
        for (const auto& [key, mapped] : map) {
            insert({key, mapped});
        }
    }
    return *this;
}

// Move constructor
template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(HashMap<K, M, H>&& map) : 
    _size(std::move(map._size)),
    _hash_function(std::move(map._hash_function)),
    _buckets_array{map.bucket_count(), nullptr} {
    for (size_t i = 0; i < map.bucket_count(); ++i) {
        _buckets_array[i] = std::move(map._buckets_array[i]);
        map._buckets_array[i] = nullptr;
    }
    map._size = 0;
}

// Move assignment operator
template <typename K, typename M, typename H>
HashMap<K, M, H>& HashMap<K, M, H>::operator=(HashMap<K, M, H>&& map) {
    if (this != &map) {
        clear();
        _size = std::move(map._size);
        _hash_function = std::move(map._hash_function);
        _buckets_array.resize(map.bucket_count());
        for (size_t i = 0; i < map.bucket_count(); ++i) {
            _buckets_array[i] = std::move(map._buckets_array[i]);
            map._buckets_array[i] = nullptr;
        }
        map._size = 0;
    }
    return *this;
}

/* end student code */






