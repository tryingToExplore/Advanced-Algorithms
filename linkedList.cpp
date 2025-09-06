#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

// Node structure to represent a non-zero element in the sparse matrix
struct Node {
    int row;
    int col;
    double value;
    Node* next;
    
    // Constructor
    Node(int r, int c, double v) : row(r), col(c), value(v), next(nullptr) {}
};

// SparseMatrix class using LinkedList implementation
class SparseMatrix {
private:
    int rows;
    int cols;
    Node* head; // Head of the linked list
    
public:
    // Constructor
    SparseMatrix(int r, int c) : rows(r), cols(c), head(nullptr) {}
    
    // Destructor
    ~SparseMatrix() {
        clear();
    }
    
    // Copy constructor
    SparseMatrix(const SparseMatrix& other) : rows(other.rows), cols(other.cols), head(nullptr) {
        Node* current = other.head;
        while (current != nullptr) {
            insert(current->row, current->col, current->value);
            current = current->next;
        }
    }
    
    // Assignment operator
    SparseMatrix& operator=(const SparseMatrix& other) {
        if (this != &other) {
            clear();
            rows = other.rows;
            cols = other.cols;
            Node* current = other.head;
            while (current != nullptr) {
                insert(current->row, current->col, current->value);
                current = current->next;
            }
        }
        return *this;
    }
    
    // Insert a value at given position
    void insert(int row, int col, double value) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw out_of_range("Index out of bounds");
        }
        
        // If value is zero, remove the node if it exists
        if (value == 0.0) {
            remove(row, col);
            return;
        }
        
        // If list is empty, insert at head
        if (head == nullptr) {
            head = new Node(row, col, value);
            return;
        }
        
        // If new node should be inserted before head
        if (row < head->row || (row == head->row && col < head->col)) {
            Node* newNode = new Node(row, col, value);
            newNode->next = head;
            head = newNode;
            return;
        }
        
        // If head has same position, update value
        if (row == head->row && col == head->col) {
            head->value = value;
            return;
        }
        
        // Find the correct position to insert
        Node* current = head;
        while (current->next != nullptr) {
            if (row < current->next->row || 
                (row == current->next->row && col < current->next->col)) {
                break;
            }
            if (row == current->next->row && col == current->next->col) {
                current->next->value = value;
                return;
            }
            current = current->next;
        }
        
        // Insert new node
        Node* newNode = new Node(row, col, value);
        newNode->next = current->next;
        current->next = newNode;
    }
    
    // Get value at given position
    double get(int row, int col) const {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw out_of_range("Index out of bounds");
        }
        
        Node* current = head;
        while (current != nullptr) {
            if (current->row == row && current->col == col) {
                return current->value;
            }
            if (current->row > row || (current->row == row && current->col > col)) {
                break;
            }
            current = current->next;
        }
        return 0.0; // Default value for sparse matrix
    }
    
    // Set value at given position (alias for insert)
    void set(int row, int col, double value) {
        insert(row, col, value);
    }
    
    // Remove a node at given position
    void remove(int row, int col) {
        if (head == nullptr) return;
        
        // If head is the node to remove
        if (head->row == row && head->col == col) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        
        // Find and remove the node
        Node* current = head;
        while (current->next != nullptr) {
            if (current->next->row == row && current->next->col == col) {
                Node* temp = current->next;
                current->next = current->next->next;
                delete temp;
                return;
            }
            current = current->next;
        }
    }
    
    // Add two sparse matrices
    SparseMatrix add(const SparseMatrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw invalid_argument("Matrix dimensions must match for addition");
        }
        
        SparseMatrix result(rows, cols);
        
        // Copy all elements from this matrix
        Node* current = head;
        while (current != nullptr) {
            result.insert(current->row, current->col, current->value);
            current = current->next;
        }
        
        // Add elements from other matrix
        current = other.head;
        while (current != nullptr) {
            double currentValue = result.get(current->row, current->col);
            result.insert(current->row, current->col, currentValue + current->value);
            current = current->next;
        }
        
        return result;
    }
    
    // Multiply two sparse matrices
    SparseMatrix multiply(const SparseMatrix& other) const {
        if (cols != other.rows) {
            throw invalid_argument("Matrix dimensions incompatible for multiplication");
        }
        
        SparseMatrix result(rows, other.cols);
        
        // For each non-zero element in this matrix
        Node* current = head;
        while (current != nullptr) {
            // For each non-zero element in other matrix
            Node* otherCurrent = other.head;
            while (otherCurrent != nullptr) {
                if (current->col == otherCurrent->row) {
                    double product = current->value * otherCurrent->value;
                    double existingValue = result.get(current->row, otherCurrent->col);
                    result.insert(current->row, otherCurrent->col, existingValue + product);
                }
                otherCurrent = otherCurrent->next;
            }
            current = current->next;
        }
        
        return result;
    }
    
    // Transpose the matrix
    SparseMatrix transpose() const {
        SparseMatrix result(cols, rows);
        
        Node* current = head;
        while (current != nullptr) {
            result.insert(current->col, current->row, current->value);
            current = current->next;
        }
        
        return result;
    }
    
    // Display the matrix
    void display() const {
        cout << "Sparse Matrix (" << rows << "x" << cols << "):" << endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << get(i, j) << "\t";
            }
            cout << endl;
        }
        cout << endl;
    }
    
    // Display only non-zero elements
    void displaySparse() const {
        cout << "Non-zero elements:" << endl;
        Node* current = head;
        while (current != nullptr) {
            cout << "(" << current->row << ", " << current->col << ") = " 
                 << current->value << endl;
            current = current->next;
        }
        cout << endl;
    }
    
    // Get number of non-zero elements
    int getNonZeroCount() const {
        int count = 0;
        Node* current = head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }
    
    // Check if matrix is empty (all zeros)
    bool isEmpty() const {
        return head == nullptr;
    }
    
    // Clear all elements
    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    // Get matrix dimensions
    pair<int, int> getDimensions() const {
        return make_pair(rows, cols);
    }
};

// Test function
void runTests() {
    cout << "=== Sparse Matrix LinkedList Implementation Tests ===" << endl << endl;
    
    // Test 1: Basic operations
    cout << "Test 1: Basic Insert and Display" << endl;
    SparseMatrix matrix1(3, 3);
    matrix1.insert(0, 0, 1.0);
    matrix1.insert(0, 2, 2.0);
    matrix1.insert(1, 1, 3.0);
    matrix1.insert(2, 0, 4.0);
    matrix1.insert(2, 2, 5.0);
    
    matrix1.display();
    matrix1.displaySparse();
    cout << "Non-zero elements: " << matrix1.getNonZeroCount() << endl << endl;
    
    // Test 2: Matrix addition
    cout << "Test 2: Matrix Addition" << endl;
    SparseMatrix matrix2(3, 3);
    matrix2.insert(0, 0, 1.0);
    matrix2.insert(1, 1, 2.0);
    matrix2.insert(2, 2, 3.0);
    
    cout << "Matrix 2:" << endl;
    matrix2.display();
    
    SparseMatrix sum = matrix1.add(matrix2);
    cout << "Sum (Matrix1 + Matrix2):" << endl;
    sum.display();
    cout << endl;
    
    // Test 3: Matrix multiplication
    cout << "Test 3: Matrix Multiplication" << endl;
    SparseMatrix matrix3(3, 2);
    matrix3.insert(0, 0, 1.0);
    matrix3.insert(0, 1, 2.0);
    matrix3.insert(1, 0, 3.0);
    matrix3.insert(1, 1, 4.0);
    matrix3.insert(2, 0, 5.0);
    matrix3.insert(2, 1, 6.0);
    
    cout << "Matrix 3 (3x2):" << endl;
    matrix3.display();
    
    SparseMatrix matrix4(2, 3);
    matrix4.insert(0, 0, 1.0);
    matrix4.insert(0, 1, 2.0);
    matrix4.insert(0, 2, 3.0);
    matrix4.insert(1, 0, 4.0);
    matrix4.insert(1, 1, 5.0);
    matrix4.insert(1, 2, 6.0);
    
    cout << "Matrix 4 (2x3):" << endl;
    matrix4.display();
    
    SparseMatrix product = matrix3.multiply(matrix4);
    cout << "Product (Matrix3 * Matrix4):" << endl;
    product.display();
    cout << endl;
    
    // Test 4: Transpose
    cout << "Test 4: Matrix Transpose" << endl;
    SparseMatrix transposed = matrix1.transpose();
    cout << "Original Matrix1:" << endl;
    matrix1.displaySparse();
    cout << "Transposed Matrix1:" << endl;
    transposed.displaySparse();
    cout << endl;
    
    // Test 5: Edge cases
    cout << "Test 5: Edge Cases" << endl;
    SparseMatrix emptyMatrix(2, 2);
    cout << "Empty matrix is empty: " << (emptyMatrix.isEmpty() ? "true" : "false") << endl;
    
    // Test setting zero (should remove element)
    matrix1.set(0, 0, 0.0);
    cout << "After setting (0,0) to 0:" << endl;
    matrix1.displaySparse();
    cout << "Non-zero elements: " << matrix1.getNonZeroCount() << endl << endl;
    
    // Test 6: Error handling
    cout << "Test 6: Error Handling" << endl;
    try {
        matrix1.insert(5, 5, 1.0); // Out of bounds
    } catch (const out_of_range& e) {
        cout << "Caught expected error: " << e.what() << endl;
    }
    
    try {
        SparseMatrix incompatible1(2, 3);
        SparseMatrix incompatible2(4, 2);
        incompatible1.add(incompatible2); // Incompatible dimensions
    } catch (const invalid_argument& e) {
        cout << "Caught expected error: " << e.what() << endl;
    }
    cout << endl;
}

int main() {
    runTests();
    return 0;
}
