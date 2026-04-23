/**
* implement a container like std::map
*/
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<class Key, class T, class Compare = std::less<Key>>
class map {
public:
    typedef pair<const Key, T> value_type;
    
private:
    enum Color { RED, BLACK };
    
    struct Node {
        value_type *data;
        Node *left, *right, *parent;
        Color color;
        
        Node(const value_type &val, Node *p = nullptr) 
            : left(nullptr), right(nullptr), parent(p), color(RED) {
            data = new value_type(val);
        }
        
        Node() : data(nullptr), left(nullptr), right(nullptr), parent(nullptr), color(BLACK) {}
        
        ~Node() {
            if (data) delete data;
        }
    };
    
    Node *root;
    Node *header;
    size_t tree_size;
    Compare comp;
    
    void rotateLeft(Node *x) {
        Node *y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }
    
    void rotateRight(Node *x) {
        Node *y = x->left;
        x->left = y->right;
        if (y->right) y->right->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }
    
    void insertFixup(Node *z) {
        while (z->parent && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node *y = z->parent->parent->right;
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateRight(z->parent->parent);
                }
            } else {
                Node *y = z->parent->parent->left;
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }
    
    void transplant(Node *u, Node *v) {
        if (!u->parent) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v) v->parent = u->parent;
    }
    
    Node* minimum(Node *x) const {
        while (x->left) x = x->left;
        return x;
    }
    
    Node* maximum(Node *x) const {
        while (x->right) x = x->right;
        return x;
    }
    
    void deleteFixup(Node *x, Node *xp) {
        while (x != root && (!x || x->color == BLACK)) {
            if (x == xp->left) {
                Node *w = xp->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    xp->color = RED;
                    rotateLeft(xp);
                    w = xp->right;
                }
                if ((!w->left || w->left->color == BLACK) && 
                    (!w->right || w->right->color == BLACK)) {
                    w->color = RED;
                    x = xp;
                    xp = x->parent;
                } else {
                    if (!w->right || w->right->color == BLACK) {
                        if (w->left) w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(w);
                        w = xp->right;
                    }
                    w->color = xp->color;
                    xp->color = BLACK;
                    if (w->right) w->right->color = BLACK;
                    rotateLeft(xp);
                    x = root;
                }
            } else {
                Node *w = xp->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    xp->color = RED;
                    rotateRight(xp);
                    w = xp->left;
                }
                if ((!w->right || w->right->color == BLACK) && 
                    (!w->left || w->left->color == BLACK)) {
                    w->color = RED;
                    x = xp;
                    xp = x->parent;
                } else {
                    if (!w->left || w->left->color == BLACK) {
                        if (w->right) w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(w);
                        w = xp->left;
                    }
                    w->color = xp->color;
                    xp->color = BLACK;
                    if (w->left) w->left->color = BLACK;
                    rotateRight(xp);
                    x = root;
                }
            }
        }
        if (x) x->color = BLACK;
    }
    
    void deleteNode(Node *z) {
        Node *y = z;
        Node *x, *xp;
        Color y_original_color = y->color;
        
        if (!z->left) {
            x = z->right;
            xp = z->parent;
            transplant(z, z->right);
        } else if (!z->right) {
            x = z->left;
            xp = z->parent;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                xp = y;
            } else {
                xp = y->parent;
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        
        delete z;
        tree_size--;
        
        if (y_original_color == BLACK && root) {
            deleteFixup(x, xp);
        }
    }
    
    void clearTree(Node *node) {
        if (!node) return;
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
    
    Node* copyTree(Node *node, Node *parent) {
        if (!node) return nullptr;
        Node *newNode = new Node(*(node->data), parent);
        newNode->color = node->color;
        newNode->left = copyTree(node->left, newNode);
        newNode->right = copyTree(node->right, newNode);
        return newNode;
    }
    
    Node* findNode(const Key &key) const {
        Node *curr = root;
        while (curr) {
            if (comp(key, curr->data->first)) {
                curr = curr->left;
            } else if (comp(curr->data->first, key)) {
                curr = curr->right;
            } else {
                return curr;
            }
        }
        return nullptr;
    }
    
public:
    class const_iterator;
    class iterator {
        friend class map;
        friend class const_iterator;
    private:
        Node *node;
        const map *container;
        
        iterator(Node *n, const map *c) : node(n), container(c) {}
        
    public:
        iterator() : node(nullptr), container(nullptr) {}
        
        iterator(const iterator &other) : node(other.node), container(other.container) {}
        
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        iterator &operator++() {
            if (!node) throw invalid_iterator();
            if (node == container->header) throw invalid_iterator();
            
            if (node->right) {
                node = container->minimum(node->right);
            } else {
                Node *p = node->parent;
                while (p && node == p->right) {
                    node = p;
                    p = p->parent;
                }
                if (!p) node = container->header;
                else node = p;
            }
            return *this;
        }
        
        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }
        
        iterator &operator--() {
            if (!node) throw invalid_iterator();
            
            if (node == container->header) {
                if (!container->root) throw invalid_iterator();
                node = container->maximum(container->root);
            } else if (node->left) {
                node = container->maximum(node->left);
            } else {
                Node *p = node->parent;
                while (p && node == p->left) {
                    node = p;
                    p = p->parent;
                }
                if (!p) throw invalid_iterator();
                node = p;
            }
            return *this;
        }
        
        value_type &operator*() const {
            return *(node->data);
        }
        
        bool operator==(const iterator &rhs) const {
            return node == rhs.node && container == rhs.container;
        }
        
        bool operator==(const const_iterator &rhs) const {
            return node == rhs.node && container == rhs.container;
        }
        
        bool operator!=(const iterator &rhs) const {
            return !(*this == rhs);
        }
        
        bool operator!=(const const_iterator &rhs) const {
            return !(*this == rhs);
        }
        
        value_type *operator->() const noexcept {
            return node->data;
        }
    };
    
    class const_iterator {
        friend class map;
    private:
        Node *node;
        const map *container;
        
        const_iterator(Node *n, const map *c) : node(n), container(c) {}
        
    public:
        const_iterator() : node(nullptr), container(nullptr) {}
        
        const_iterator(const const_iterator &other) : node(other.node), container(other.container) {}
        
        const_iterator(const iterator &other) : node(other.node), container(other.container) {}
        
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        const_iterator &operator++() {
            if (!node) throw invalid_iterator();
            if (node == container->header) throw invalid_iterator();
            
            if (node->right) {
                node = container->minimum(node->right);
            } else {
                Node *p = node->parent;
                while (p && node == p->right) {
                    node = p;
                    p = p->parent;
                }
                if (!p) node = container->header;
                else node = p;
            }
            return *this;
        }
        
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }
        
        const_iterator &operator--() {
            if (!node) throw invalid_iterator();
            
            if (node == container->header) {
                if (!container->root) throw invalid_iterator();
                node = container->maximum(container->root);
            } else if (node->left) {
                node = container->maximum(node->left);
            } else {
                Node *p = node->parent;
                while (p && node == p->left) {
                    node = p;
                    p = p->parent;
                }
                if (!p) throw invalid_iterator();
                node = p;
            }
            return *this;
        }
        
        const value_type &operator*() const {
            return *(node->data);
        }
        
        bool operator==(const iterator &rhs) const {
            return node == rhs.node && container == rhs.container;
        }
        
        bool operator==(const const_iterator &rhs) const {
            return node == rhs.node && container == rhs.container;
        }
        
        bool operator!=(const iterator &rhs) const {
            return !(*this == rhs);
        }
        
        bool operator!=(const const_iterator &rhs) const {
            return !(*this == rhs);
        }
        
        const value_type *operator->() const noexcept {
            return node->data;
        }
    };
    
    map() : root(nullptr), tree_size(0) {
        header = new Node();
    }
    
    map(const map &other) : root(nullptr), tree_size(other.tree_size) {
        header = new Node();
        header->color = BLACK;
        root = copyTree(other.root, nullptr);
    }
    
    map &operator=(const map &other) {
        if (this == &other) return *this;
        clear();
        root = copyTree(other.root, nullptr);
        tree_size = other.tree_size;
        return *this;
    }
    
    ~map() {
        clear();
        delete header;
    }
    
    T &at(const Key &key) {
        Node *node = findNode(key);
        if (!node) throw index_out_of_bound();
        return node->data->second;
    }
    
    const T &at(const Key &key) const {
        Node *node = findNode(key);
        if (!node) throw index_out_of_bound();
        return node->data->second;
    }
    
    T &operator[](const Key &key) {
        Node *node = findNode(key);
        if (node) return node->data->second;
        
        value_type val(key, T());
        auto result = insert(val);
        return result.first->second;
    }
    
    const T &operator[](const Key &key) const {
        Node *node = findNode(key);
        if (!node) throw index_out_of_bound();
        return node->data->second;
    }
    
    iterator begin() {
        if (!root) return end();
        return iterator(minimum(root), this);
    }
    
    const_iterator cbegin() const {
        if (!root) return cend();
        return const_iterator(minimum(root), this);
    }
    
    iterator end() {
        return iterator(header, this);
    }
    
    const_iterator cend() const {
        return const_iterator(header, this);
    }
    
    bool empty() const {
        return tree_size == 0;
    }
    
    size_t size() const {
        return tree_size;
    }
    
    void clear() {
        clearTree(root);
        root = nullptr;
        tree_size = 0;
    }
    
    pair<iterator, bool> insert(const value_type &value) {
        Node *parent = nullptr;
        Node *curr = root;
        
        while (curr) {
            parent = curr;
            if (comp(value.first, curr->data->first)) {
                curr = curr->left;
            } else if (comp(curr->data->first, value.first)) {
                curr = curr->right;
            } else {
                return pair<iterator, bool>(iterator(curr, this), false);
            }
        }
        
        Node *newNode = new Node(value, parent);
        tree_size++;
        
        if (!parent) {
            root = newNode;
        } else if (comp(value.first, parent->data->first)) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
        
        insertFixup(newNode);
        return pair<iterator, bool>(iterator(newNode, this), true);
    }
    
    void erase(iterator pos) {
        if (pos.container != this || pos.node == header || !pos.node) {
            throw invalid_iterator();
        }
        deleteNode(pos.node);
    }
    
    size_t count(const Key &key) const {
        return findNode(key) ? 1 : 0;
    }
    
    iterator find(const Key &key) {
        Node *node = findNode(key);
        return node ? iterator(node, this) : end();
    }
    
    const_iterator find(const Key &key) const {
        Node *node = findNode(key);
        return node ? const_iterator(node, this) : cend();
    }
};

}

#endif