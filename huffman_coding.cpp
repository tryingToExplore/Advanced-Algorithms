#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

// Huffman Tree Node class
class HuffmanNode {
public:
    char data;                    // Character (only for leaf nodes)
    int frequency;               // Frequency of the character
    HuffmanNode* left;           // Left child
    HuffmanNode* right;          // Right child
    bool isLeaf;                 // True if this is a leaf node

    // Constructor for leaf nodes
    HuffmanNode(char ch, int freq) : data(ch), frequency(freq), left(nullptr), right(nullptr), isLeaf(true) {}
    
    // Constructor for internal nodes
    HuffmanNode(int freq, HuffmanNode* l, HuffmanNode* r) : data('\0'), frequency(freq), left(l), right(r), isLeaf(false) {}
    
    // Destructor to clean up memory
    ~HuffmanNode() {
        delete left;
        delete right;
    }
};

// Comparator for priority queue (min-heap based on frequency)
struct CompareNodes {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency; // Min-heap: smaller frequency has higher priority
    }
};

class HuffmanCoding {
private:
    HuffmanNode* root;
    map<char, string> huffmanCodes;
    
    // Helper function to generate Huffman codes using preorder traversal
    void generateCodes(HuffmanNode* node, string code) {
        if (node == nullptr) return;
        
        // If it's a leaf node, store the code
        if (node->isLeaf) {
            huffmanCodes[node->data] = code;
            return;
        }
        
        // Recursively traverse left (add '0') and right (add '1')
        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }
    
    // Helper function for preorder traversal to print codes in the required order
    void preorderTraversal(HuffmanNode* node, vector<string>& codes, string currentCode = "") {
        if (node == nullptr) return;
        
        // If it's a leaf node, add its code to the result
        if (node->isLeaf) {
            codes.push_back(currentCode);
            return;
        }
        
        // Visit left subtree first (add '0'), then right subtree (add '1')
        preorderTraversal(node->left, codes, currentCode + "0");
        preorderTraversal(node->right, codes, currentCode + "1");
    }

public:
    HuffmanCoding() : root(nullptr) {}
    
    ~HuffmanCoding() {
        delete root;
    }
    
    // Build Huffman tree from characters and their frequencies
    void buildTree(const string& characters, const vector<int>& frequencies) {
        // Create a min-heap priority queue
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> minHeap;
        
        // Create leaf nodes for each character and add to min-heap
        for (size_t i = 0; i < characters.length(); i++) {
            HuffmanNode* node = new HuffmanNode(characters[i], frequencies[i]);
            minHeap.push(node);
        }
        
        // Build the tree by repeatedly combining the two nodes with smallest frequencies
        while (minHeap.size() > 1) {
            // Extract the two nodes with minimum frequency
            HuffmanNode* left = minHeap.top();
            minHeap.pop();
            
            HuffmanNode* right = minHeap.top();
            minHeap.pop();
            
            // Create a new internal node with frequency equal to sum of the two nodes
            HuffmanNode* merged = new HuffmanNode(left->frequency + right->frequency, left, right);
            
            // Add the merged node back to the min-heap
            minHeap.push(merged);
        }
        
        // The remaining node is the root of the Huffman tree
        root = minHeap.top();
        
        // Generate Huffman codes
        generateCodes(root, "");
    }
    
    // Get all Huffman codes in preorder traversal order
    vector<string> getCodesInPreorder() {
        vector<string> codes;
        if (root != nullptr) {
            preorderTraversal(root, codes, "");
        }
        return codes;
    }
    
    // Get the Huffman code for a specific character
    string getCode(char ch) {
        return huffmanCodes[ch];
    }
    
    // Print all Huffman codes
    void printAllCodes() {
        cout << "\nHuffman codes:" << endl;
        for (auto& pair : huffmanCodes) {
            cout << "  " << pair.first << " : " << pair.second << endl;
        }
    }
    
    // Print codes in preorder traversal format (as required by the assignment)
    void printCodesInPreorder() {
        vector<string> codes = getCodesInPreorder();
        cout << "\nHuffman codes in preorder traversal:" << endl;
        for (size_t i = 0; i < codes.size(); i++) {
            cout << setw(4) << codes[i];
        }
        cout << endl;
    }
    
    // Display the tree structure (for debugging/visualization)
    void displayTree(HuffmanNode* node, int indent = 0) {
        if (node == nullptr) return;
        
        for (int i = 0; i < indent; i++) cout << "  ";
        
        if (node->isLeaf) {
            cout << "Leaf: '" << node->data << "' (freq: " << node->frequency << ")" << endl;
        } else {
            cout << "Internal: (freq: " << node->frequency << ")" << endl;
        }
        
        displayTree(node->left, indent + 1);
        displayTree(node->right, indent + 1);
    }
    
    HuffmanNode* getRoot() { return root; }
};

// Function to solve the given problem
void solveHuffmanProblem(const string& characters, const vector<int>& frequencies) {
    cout << "\n=== Huffman Coding Problem ===" << endl;
    cout << "Characters: " << characters << endl;
    cout << "Frequencies: ";
    for (size_t i = 0; i < frequencies.size(); i++) {
        cout << frequencies[i];
        if (i < frequencies.size() - 1) cout << ", ";
    }
    cout << endl;
    
    HuffmanCoding huffman;
    huffman.buildTree(characters, frequencies);
    
    // Print the result in the required format
    cout << "\nOutput:" << endl;
    huffman.printCodesInPreorder();
    
    // Also show detailed codes for verification
    huffman.printAllCodes();
}

// Test with the provided example
void testProvidedExample() {
    cout << "\n=== Testing with Provided Example ===" << endl;
    string S = "abcdef";
    vector<int> f = {5, 9, 12, 13, 16, 45};
    
    solveHuffmanProblem(S, f);
}

// Test with additional examples
void testAdditionalExamples() {
    cout << "\n=== Additional Test Cases ===" << endl;
    
    // Test case 1: Simple case
    cout << "\nTest Case 1: Simple case" << endl;
    string S1 = "abc";
    vector<int> f1 = {5, 9, 12};
    solveHuffmanProblem(S1, f1);
    
    // Test case 2: Two characters
    cout << "\nTest Case 2: Two characters" << endl;
    string S2 = "ab";
    vector<int> f2 = {1, 2};
    solveHuffmanProblem(S2, f2);
    
    // Test case 3: Single character
    cout << "\nTest Case 3: Single character" << endl;
    string S3 = "a";
    vector<int> f3 = {10};
    solveHuffmanProblem(S3, f3);
}

// Interactive mode for user input
void interactiveMode() {
    cout << "\n=== Interactive Mode ===" << endl;
    
    string characters;
    cout << "Enter the string of characters: ";
    cin >> characters;
    
    vector<int> frequencies(characters.length());
    cout << "Enter the frequencies for each character:" << endl;
    for (size_t i = 0; i < characters.length(); i++) {
        cout << "Frequency for '" << characters[i] << "': ";
        cin >> frequencies[i];
    }
    
    solveHuffmanProblem(characters, frequencies);
}

int main() {
    cout << "Huffman Coding Implementation" << endl;
    cout << "============================" << endl;
    
    int choice;
    
    while (true) {
        cout << "\nOptions:" << endl;
        cout << "1. Test with provided example (abcdef with frequencies [5,9,12,13,16,45])" << endl;
        cout << "2. Run additional test cases" << endl;
        cout << "3. Interactive mode (enter your own data)" << endl;
        cout << "4. Quit" << endl;
        cout << "Choose (1-4): ";
        
        cin >> choice;
        
        switch (choice) {
            case 1:
                testProvidedExample();
                break;
                
            case 2:
                testAdditionalExamples();
                break;
                
            case 3:
                interactiveMode();
                break;
                
            case 4:
                cout << "\nGoodbye!" << endl;
                return 0;
                
            default:
                cout << "Invalid choice. Try again." << endl;
                break;
        }
    }
    
    return 0;
}
