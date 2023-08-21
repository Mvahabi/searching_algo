/*
 * Mahyar Mike Vahabi
 * mvahabi
 * Winter 22 - CSE101 - pa8
 * Dictionary.cpp - Dictionary ADTs
*/

#include "Dictionary.h"
#include <cstring>
#include <limits>
#include <sstream>
#include <stdexcept>

// Class Constructors & Destructors ----------------------------------------   
Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    color = 0;
}

// Dictionary fields
Dictionary::Dictionary() {
    nil = new Node ("MahiVahabiMoghaddam", 999);
    root = nil;
    current = nil;
    num_pairs = 0;
}

// copy constructor
Dictionary::Dictionary(const Dictionary& D) {
    nil = new Node("MahiVahabiMoghaddam", 999);
    root = nil;
    current = nil;
    num_pairs = 0;

    std::string s, line;
    preorder(s, D.root);
    std::stringstream X(s);
    while (getline(X, line, ' ')) {
        keyType k = line;
        getline(X, line, ' ');
        valType v = std::stoi(line);
        setValue(k, v);
    } 
}

// Creates new Dictionary in the empty state.
//Dictionary::Dictionary(const Dictionary& D{ }

// Destructor
Dictionary::~Dictionary(){
    clear();
    delete(nil);
}

// Helper Functions (Optional) ---------------------------------------------

// inOrderString()
// Appends a string representation of the tree rooted at R to string s. The
// string appended consists of: "key : value \n" for each key-value pair in
// tree R, arranged in order by keys.
void Dictionary::inOrderString(std::string& s, Node* R) const {
    if (R != nil && R->key != "MahiVahabiMoghaddam") {
        inOrderString(s, R->left);
        s += R->key;
        s += " : ";
        s += std::to_string(R->val);
        s +="\n";
        inOrderString(s, R->right);
    }
}       

// preOrderString()
// Appends a string representation of the tree rooted at R to s. The appended
// string consists of keys only, separated by "\n", with the order determined
// by a pre-order tree walk.
void Dictionary::preOrderString(std::string& s, Node* R) const {
    if (R != nil) {
        s += R->key;
        s += "\n";
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

// postOrderDelete()
// Deletes all Nodes in the subtree rooted at R, sets R to nil.
void Dictionary::postOrderDelete(Node* R) {
    if (R != nil) {
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete (R);
    }
}

void Dictionary::preorder(std::string& s, Node* R) const {
    if (R != nil && R->key != "MahiVahabiMoghaddam") {
        s += R->key;
        s += " ";
        s += std::to_string(R->val);
        s += " ";
        preorder(s, R->left);
        preorder(s, R->right);
    }
}

// search()
// Searches the subtree rooted at R for a Node with key==k. Returns
// the address of the Node if it exists, returns nil otherwise.
Dictionary::Node* Dictionary::search(Node* R, keyType k) const {
   while (R != nil || R->key != "MahiVahabiMoghaddam") {
       if (R->key == k) {
            return R;
        } else if (k < R->key) {
            R = R->left;
        } else {
            R = R->right;
        }
    }
    return nil;
}

// findMin()
// If the subtree rooted at R is not empty, returns a pointer to the 
// leftmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMin(Node* R){  
    while (R->left != nil && R->key != "MahiVahabiMoghaddam") {
        R = R->left;
    }
    return R;
}

// findMax()
// If the subtree rooted at R is not empty, returns a pointer to the 
// rightmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMax(Node* R){
    while (R->right != nil && R->key != "MahiVahabiMoghaddam") {
        R = R->right;
    }
    return R;
}

// findNext()
// If N does not point to the rightmost Node, returns a pointer to the
// Node after N in an in-order tree walk.  If N points to the rightmost 
// Node, or is nil, returns nil. 
Dictionary::Node* Dictionary::findNext(Node* N){
    if (N->right != nil && N->right->key != "MahiVahabiMoghaddam") {
        return (findMin(N->right));
    }
    Node* y = N->parent;
    while (y != nil && N == y->right && y->key != "MahiVahabiMoghaddam") {
        N = y;
        y = y->parent;
    }
    return y;
}

// findPrev()
// If N does not point to the leftmost Node, returns a pointer to the
// Node before N in an in-order tree walk.  If N points to the leftmost 
// Node, or is nil, returns nil.
Dictionary::Node* Dictionary::findPrev(Node* N){
    if (N->left != nil && N->left->key != "MahiVahabiMoghaddam") {
        return findMax(N->left);
    }
    Node* y = N->parent;
    while (y != nil && N == y->left && y->key != "MahiVahabiMoghaddam") {
        N = y;
        y = y->parent;
    }
    return y;   
}

// RBT Helper Functions (Optional) -----------------------------------------
   
// LeftRotate()
void Dictionary::LeftRotate(Node* x){
    Node* y = x->right; 
    
    // turn y's left subtree into x's right subtree
    x->right = y->left; 
    if (y->left != nil || y->left->key != "MahiVahabiMoghaddam"){ 
        y->left->parent = x;
    }
    // link y's parent to x
    y->parent = x->parent; 
    if (x->parent == nil || x->parent->key == "MahiVahabiMoghaddam") {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else { 
        x->parent->right = y;
    }

    // put x on y's left
    y->left = x; 
    x->parent = y;
}

// RightRotate()
void Dictionary::RightRotate(Node* x){
    // set y
    Node* y = x->left; 
    
    // turn y's right subtree into x's left subtree
    x->left = y->right; 
    if (y->right != nil || y->right->key == "MahiVahabiMoghaddam"){  
        y->right->parent = x;
    }
    // link y's parent to x
    y->parent = x->parent; 
    if (x->parent == nil || x->parent->key == "MahiVahabiMoghaddam"){
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else 
        x->parent->left = y;
    
    // put x on y's right
    y->right = x; 
    x->parent = y; 
}

// RB_InsertFixUP()
void Dictionary::RB_InsertFixUp(Node* z) {
    Node* y = nullptr;
    while (z->parent->color == 1) {
        if (z->parent == z->parent->parent->left) { 
            y = z->parent->parent->right;
            if (y->color == 1) {
                z->parent->color = 0;              // case 1
                y->color = 0;                     // case 1
                z->parent->parent->color = 1;         // case 1
                z = z->parent->parent;                 // case 1
            } else {
                if (z == z->parent->right) {
                    z = z->parent;                     // case 2
                    LeftRotate(z);                 // case 2
                }
                z->parent->color = 0;              // case 3
                z->parent->parent->color = 1;         // case 3
                RightRotate(z->parent->parent);     // case 3
            }
        } else {
            y = z->parent->parent->left;
            if (y->color == 1) {
                z->parent->color = 0;              // case 4
                y->color = 0;                     // case 4
                z->parent->parent->color = 1;         // case 4
                z = z->parent->parent;                 // case 4
            } else {
                if (z == z->parent->left) {
                    z = z->parent;                    // case 5
                    RightRotate(z);
                }                // case 5
                z->parent->color = 0;              // case 6
                z->parent->parent->color = 1;         // case 6
                LeftRotate(z->parent->parent);      // case 6
            }
        }
    }
    root->color = 0;
}

// RB_Transplant()
void Dictionary::RB_Transplant(Node* u, Node* v){
    if (u->parent == nil || u->parent->key == "MahiVahabiMoghaddam") {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nil && v->key != "MahiVahabiMoghaddam") {
        v->parent = u->parent;
    }
}

// RB_DeleteFixUp()
void Dictionary::RB_DeleteFixUp(Node* x) {
    Node* w = nullptr;
    while (x != root && x->color == 0){
        if (x == x->parent->left){
            w = x->parent->right;
            // case 1
            if (w->color == 1){
                w->color = 0;                        
                x->parent->color = 1;                   
                LeftRotate(x->parent);                
                w = x->parent->right;
            }                     
            // case 2
            if (w->left->color == 0 && w->right->color == 0){
                w->color = 1;                          
                x = x->parent;                           
            } else {
                // case 3
                if (w->right->color == 0){
                    w->left->color = 0;                
                    w->color = 1;                       
                    RightRotate(w);                   
                    w = x->parent->right;                  
                } 
                // case 4
                w->color = x->parent->color;               
                x->parent->color = 0;                 
                w->right->color = 0;                  
                LeftRotate(x->parent);                
                x = root;
            }                             
        } else { 
            w = x->parent->left;
            // case 5
            if (w->color == 1) {
                w->color = 0;                        
                x->parent->color = 1;                   
                RightRotate(x->parent);               
                w = x->parent->left;    
            }
            // case 6
            if (w->right->color == 0 && w->left->color == 0) {
                w->color = 1;                          
                x = x->parent;                           
            } else {
                // case 7
                if (w->left->color == 0) {
                    w->right->color = 0;               
                    w->color = 1;                       
                    LeftRotate(w);                    
                    w = x->parent->left;
                }
                // case 8
                w->color = x->parent->color;               
                x->parent->color = 0;                 
                w->left->color = 0;                   
                RightRotate(x->parent);               
                x = root;
            }
        }
    }                             
    x->color = 0;
}

// RB_Delete()
void Dictionary::RB_Delete(Node* z){
    Node* y = z;
    Node* x = nullptr;
    int y_original_color = y->color;
    if (z->left == nil || z->left->key == "MahiVahabiMoghaddam") {
        x = z->right;
        RB_Transplant(z, z->right);
    } else if (z->right == nil || z->right->key == "MahiVahabiMoghaddam") {
        x = z->left;
        RB_Transplant(z, z->left);
    } else {
        y = findMin(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else { 
            RB_Transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RB_Transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == 0) {
        RB_DeleteFixUp(x);
    }    
}


// Access functions --------------------------------------------------------
   
// size()
// Returns the size of this Dictionary.
int Dictionary::size() const {
    return num_pairs;
}

// contains()
// Returns true if there exists a pair such that key==k, and returns false
// otherwise.
bool Dictionary::contains(keyType k) const {
    Node* N = search(root, k);
    return (N != nil);
}

// getValue()
// Returns a reference to the value corresponding to key k.
// Pre: contains(k)
valType& Dictionary::getValue(keyType k) const {    
    Node* N = search(root, k);
    if (N != nil) {
        return N->val;
    } else {
        throw std::logic_error("Error in getValue(). Node is nil.");
    }
}

// hasCurrent()
// Returns true if the current iterator is defined, and returns false 
// otherwise.
bool Dictionary::hasCurrent() const{
    return (current != nil);
}

// currentKey()
// Returns the current key.
// Pre: hasCurrent() 
keyType Dictionary::currentKey() const{
    if (hasCurrent() == true){
        return current->key;
    }
    else {
        throw std::invalid_argument("Error in currentKey(). No Current Key");
    }
}

// currentVal()
// Returns a reference to the current value.
// Pre: hasCurrent()
valType& Dictionary::currentVal() const{
    if (hasCurrent() == true) {
        return current->val;
    } else {
        throw std::invalid_argument("Error in currentVal(). No Current val");
    }
}

// Manipulation procedures -------------------------------------------------

// clear()
// Resets this Dictionary to the empty state, containing no pairs.
void Dictionary::clear(){
    postOrderDelete(root);
    root = nil;
    current = nil;
    num_pairs = 0;
}

// setValue()
// If a pair with key==k exists, overwrites the corresponding value with v, 
// otherwise inserts the new pair (k, v).
void Dictionary::setValue(keyType k, valType v) {
    Node *y = nil;
    Node *x = root;
    Node *z = new Node(k, v);
    z->parent = nil;
    z->left = nil;
    z->right = nil;
    bool flag = true;
    while (x != nil) {
        if (z->key != x->key) {
            y = x;
        }
        if (z->key < x->key) {
            x = x->left;
        }
        else if (z->key == x->key) {
            x->val = z->val;
            flag = false;
            break;
        }
        else {
            x = x->right;
        }
    }
    z->parent = y;
    if (flag) {
        if (y == nil) {
            root = z;
            num_pairs++;
        } else if (z->key < y->key) {
            y->left = z;
            num_pairs++;
        } else {
            y->right = z;
            z->parent = y;
            num_pairs++;
        }
    }
    z->left = nil;
    z->right = nil;
    z->color = 1;
    RB_InsertFixUp(z);
}

// remove()
// Deletes the pair for which key==k. If that pair is current, then current
// becomes undefined.
// Pre: contains(k).
void Dictionary::remove(keyType k) {
    Node* z = search(root, k);
    if (z == nil || z->key == "MahiVahabiMoghaddam") {
        throw std::logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
    }
    if (z->left == nil || z->left->key == "MahiVahabiMoghaddam") {
        RB_Transplant(z, z->right);
        num_pairs--;
    }
    else if (z->right == nil || z->right->key == "MahiVahabiMoghaddam") {
        RB_Transplant(z, z->left);
        num_pairs--;
    }
    else {
        Node* y = findMin(z->right);
        if (y->parent != z) {
            RB_Transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RB_Transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        num_pairs--;
    }
}

// begin()
// If non-empty, places current iterator at the first (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::begin(){
    if (num_pairs > 0) {
        current = findMin(root);
    } else {
        throw std::logic_error("Error on begin(). No pairs.");
    }
}

// end()
// If non-empty, places current iterator at the last (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::end(){
    if (num_pairs > 0) {
        current = findMax(root);
    } else {
        throw std::logic_error("Error in end(). No pairs.");
    }
}

// next()
// If the current iterator is not at the last pair, advances current 
// to the next pair (as defined by the order operator < on keys). If 
// the current iterator is at the last pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::next(){
    if (hasCurrent()) {
        current = findNext(current);
    } else { 
        throw std::logic_error("Error in next(). No current.");
    }
}

// prev()
// If the current iterator is not at the first pair, moves current to  
// the previous pair (as defined by the order operator < on keys). If 
// the current iterator is at the first pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::prev(){
    if (hasCurrent()) {
        current = findPrev(current);
    } else {
        throw std::logic_error("Error in prev(). No current.");
    }
}

// Other Functions ---------------------------------------------------------
// to_string()
// Returns a string representation of this Dictionary. Consecutive (key, value)
// pairs are separated by a newline "\n" character, and the items key and value 
// are separated by the sequence space-colon-space " : ". The pairs are arranged 
// in order, as defined by the order operator <.
std::string Dictionary::to_string() const {
    std::string s = "";
    inOrderString(s, root);
    return s;
}   

// pre_string()
// Returns a string consisting of all keys in this Dictionary. Consecutive
// keys are separated by newline "\n" characters. The key order is given
// by a pre-order tree walk.
std::string Dictionary::pre_string() const {
    std::string s = "";
    preOrderString(s, root);
    return s;
}

// equals()
// Returns true if and only if this Dictionary contains the same (key, value)
// pairs as Dictionary D.
bool Dictionary::equals(const Dictionary& D) const {
    std::string x;
    std::string y;
    inOrderString(x, root);
    inOrderString(y, D.root);
    return (x == y);
}
    
// Overloaded Operators ----------------------------------------------------
   
// operator<<()
// Inserts string representation of Dictionary D into stream, as defined by
// member function to_string().
std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    return stream << D.to_string();
}

// operator==()
// Returns true if and only if Dictionary A equals Dictionary B, as defined
// by member function equals(). 
bool operator==( const Dictionary& A, const Dictionary& B ) {
    return A.Dictionary::equals(B);
}
// operator=()
// Overwrites the state of this Dictionary with state of D, and returns a
// reference to this Dictionary.
Dictionary& Dictionary::operator=( const Dictionary& D ){
    Dictionary temp = D;
    std::swap(root, temp.root);
    std::swap(nil, temp.nil);
    std::swap(current, temp.current);
    std::swap(num_pairs, temp.num_pairs);
    return *this;
}
