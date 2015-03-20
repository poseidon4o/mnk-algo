#ifndef _TREAP_H_INCLUDED_
#define _TREAP_H_INCLUDED_

#include <random>
#include <algorithm>
#include <limits>

typedef float TreapPriority_t;
std::mt19937 mt(5);
std::uniform_real_distribution<TreapPriority_t> dist(std::numeric_limits<TreapPriority_t>::min(), std::numeric_limits<TreapPriority_t>::max());

class TreapNode
{
public:
	TreapNode(int k)
	{
		key = k;
        // changed to use the whole float because just [0, 1] is not enough to cover all ints and collisions will arise
        priority = dist(mt);// (float)rand() / RAND_MAX;
		left = 0;
		right = 0;
	}
	int key;	// key provided by user
    TreapPriority_t priority;	// node's generated priority
	TreapNode* left;	// pointer for the left subtree
	TreapNode* right;	// pointer for the right subtree
};


class Treap
{
	TreapNode * root;
    int elements;

    friend bool valid(const Treap &);
    friend int maxHeight(const Treap &);
    friend int avgHeight(const Treap &);
public:
    Treap() : root(nullptr), elements(0) {

    }

    ~Treap() {
        free(root);
    }


    /**
    * Removes all elements from the Treap
    *
    */
    void clear() {
        elements = 0;
        free(root);
        root = nullptr;
    }

    /**
    * Returns the number of elements in the Treap
    * 
    */
    int size() const {
        return elements;
    }

	/**
	* Inserts a node in the treap with the provided @key
	*
	* @param key: key to be inserted
	*/
    void insert(int key) {
        TreapNode *& where = find(root, key);
        if (!where) {
            ++elements;
            where = new TreapNode(key);
            find(root, key, true);
        }
    }

	/**
	* Removes the node from the treap with the provided @key
	*
	* @param key: key to be removed
	*/
    void remove(int key) {
        TreapNode *& where = find(root, key);
        if (where) {
            --elements;
            remove(where);
        }
    }

	/**
	* Checks whether a given @key is already in the treap
	*
	* @param key: key to be searched for
	* @return true, if the key is in the treap, and false, otherwise
	*/
    bool containsKey(int key) const {
        // not breaking the interface leads to ugly casts
        return const_cast<Treap*>(this)->find(const_cast<TreapNode*>(root), key) != nullptr;
    }

private:

    TreapNode * left(TreapNode *& n) {
        TreapNode * parent = n->right;
        n->right = parent->left;
        parent->left = n;
        return parent;
    }

    TreapNode * right(TreapNode *& n) {
        TreapNode * parent = n->left;
        n->left = parent->right;
        parent->right = n;
        return parent;
    }

    void remove(TreapNode *& n) {
        if (!n->left && !n->right) {
            delete n;
            n = nullptr;
        } else if (n->left || n->right) {
            TreapNode * del = n;
            n = n->left ? n->left : n->right;
            delete del;
        } else {
            if (n->left->priority > n->right->priority) {
                n = right(n);
                remove(n->right);
            } else {
                n = left(n);
                remove(n->left);
            }
        }
    }

	TreapNode *& find(TreapNode *& n, int key, bool heap_up = false) {
		if (!n || n->key == key) {
			return n;
		}

        TreapNode *& node = key < n->key ? find(n->left, key, heap_up) : find(n->right, key, heap_up);

        if (heap_up) {
            if (n->right && n->right->key == key && n->right->priority > n->priority) {
                n = left(n);
            } else if (n->left && n->left->key == key && n->left->priority > n->priority) {
                n = right(n);
            }
        }

        return node;
	}

    void free(TreapNode * n) {
        if (n) {
            free(n->left);
            free(n->right);
            delete n;
        }
    }


	Treap& operator=(const Treap&); // Do not implement me!
	Treap(const Treap&); // Do not implement me!
};




bool valid_heap(const TreapNode * n) {
    if (!n || (n && !n->left && !n->right)) {
        return true;
    }

    if (n->left && n->priority < n->left->priority) {
        return false;
    } else if (n->right && n->priority < n->right->priority) {
        return false;
    }

    return valid_heap(n->left) && valid_heap(n->right);
}

bool valid_bst(const TreapNode * n) {
    if (!n || (n && !n->left && !n->right)) {
        return true;
    }

    if (n->left && n->key < n->left->key) {
        return false;
    } else if (n->right && n->key >= n->right->key) {
        return false;
    }

    return valid_bst(n->left) && valid_bst(n->right);
}

int maxHeight(const TreapNode * n) {
    return n ? 1 + std::max(maxHeight(n->left), maxHeight(n->right)) : 0;
}

int avgHeight(const TreapNode * n) {
    return n ? 1 + (avgHeight(n->left) + avgHeight(n->right)) / 2 : 0;
}

bool valid(const Treap & treap) {
    return valid_heap(treap.root) && valid_bst(treap.root);
}

int maxHeight(const Treap & treap) {
    return maxHeight(treap.root);
}

int avgHeight(const Treap & treap) {
    return avgHeight(treap.root);
}




#endif // _TREAP_H_INCLUDED_