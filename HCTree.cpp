#include "HCTree.hpp"

// Recursive function to delete the tree nodes
void deleteTree(HCNode* node) {
    if (node) {
        deleteTree(node->c0); // Delete left child
        deleteTree(node->c1); // Delete right child
        delete node; // Delete current node
    }
}

HCTree::~HCTree() {
    deleteTree(root);
}

// Build the Huffman coding tree
void HCTree::build(const vector<int>& freqs) {
    // Create a priority queue to store the nodes based on their frequencies
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    // Create leaf nodes for each symbol with non-zero frequency
    for (long unsigned int i = 0; i < freqs.size(); i++) {
        if (freqs[i] > 0) {
            HCNode* node = new HCNode(freqs[i], static_cast<unsigned char>(i));
            leaves[i] = node;
            pq.push(node);
        }
    }

    // Build the Huffman tree
    while (pq.size() > 1) {
        HCNode* left = pq.top();
        pq.pop();
        HCNode* right = pq.top();
        pq.pop();

        // Create a parent node with the sum of frequencies
        HCNode* parent = new HCNode(left->count + right->count, '\0');
        parent->c0 = left;
        parent->c1 = right;

        // Set the parent node as the children's parent
        left->p = parent;
        right->p = parent;

        pq.push(parent);
    }
    
    // Set the root to the top node in the priority queue
    if (!pq.empty()) {
        root = pq.top();
        printTree(root);
    }
}

// Debug function to print the Huffman tree
void HCTree::printTree(HCNode* root) const {
    if (root == nullptr)
        return;
    queue<HCNode*> q;
    q.push(root);

    while (!q.empty()) {
        HCNode* current = q.front();
        q.pop();

        //cout << "Value: " << current->symbol << ", Frequency: " << current->count <<endl;

        if (current->c0 != nullptr)
            q.push(current->c0);

        if (current->c1 != nullptr)
            q.push(current->c1);
    }
}

// Write to the given FancyOutputStream the sequence of bits coding the given symbol.
void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{
    if (!root) {
        // Log an error or throw an exception if the tree is not built
        error("Error: Huffman tree not built in encode()");
        return;
    }

    HCNode* node = leaves[symbol];

    // Traverse from the leaf node to the root to determine the bit sequence
    string bitSeq;
    HCNode* current = node;
    while (current->p != nullptr) {
        if(current == current->p->c0){
            bitSeq = "0" + bitSeq;
        }
        else{
            bitSeq = "1" + bitSeq;
        }
        current = current->p;
    }
    
    //cout<< "bitseq: " << bitSeq<< endl;
    for (char b: bitSeq) {
        if(b == '0'){
            out.write_bit(0);  
        }
        else{
            out.write_bit(1);
        }
    }
}

// Return symbol coded in the next sequence of bits from the stream.
unsigned char HCTree::decode(FancyInputStream & in) const{
    
	HCNode* curr = root; 
    int bit = 0;
	while (bit != -1) 
	{ 
        bit = in.read_bit();
		if (bit == 0){
            curr = curr->c0; 
        }
		else{
		    curr = curr->c1;
        }
		// reached leaf node 
		if (curr->c0==nullptr && curr->c1==nullptr) 
		{ 
			return curr->symbol;
		} 
	} 
	return 0; 
}