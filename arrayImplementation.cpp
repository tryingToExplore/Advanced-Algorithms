#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
using namespace std;

// SparseMatrixArray class using dynamic arrays for sparse matrix representation
class SparseMatrixArray {
private:
    int rows;
    int cols;
    int capacity;
    int size; // Number of non-zero elements
    int* rowIndices;
    int* colIndices;
    double* values;
    
    // Helper function to find insertion point using binary search
    int findInsertionPoint(int row, int col) const {
        int left = 0;
        int right = size;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (rowIndices[mid] < row || 
                (rowIndices[mid] == row && colIndices[mid] < col)) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        return left;
    }
    
    // Helper function to find element index using binary search
    int findElementIndex(int row, int col) const {
        int left = 0;
        int right = size - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (rowIndices[mid] == row && colIndices[mid] == col) {
                return mid;
            } else if (rowIndices[mid] < row || 
                      (rowIndices[mid] == row && colIndices[mid] < col)) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return -1; // Not found
    }
    
    // Resize arrays when capacity is exceeded
    void resize() {
        capacity *= 2;
        int* newRowIndices = new int[capacity];
        int* newColIndices = new int[capacity];
        double* newValues = new double[capacity];
        
        for (int i = 0; i < size; i++) {
            newRowIndices[i] = rowIndices[i];
            newColIndices[i] = colIndices[i];
            newValues[i] = values[i];
        }
        
        delete[] rowIndices;
        delete[] colIndices;
        delete[] values;
        
        rowIndices = newRowIndices;
        colIndices = newColIndices;
        values = newValues;
    }
    
    // Shift elements to the right from given index
    void shiftRight(int index) {
        for (int i = size; i > index; i--) {
            rowIndices[i] = rowIndices[i - 1];
            colIndices[i] = colIndices[i - 1];
            values[i] = values[i - 1];
        }
    }
    
    // Shift elements to the left from given index
    void shiftLeft(int index) {
        for (int i = index; i < size - 1; i++) {
            rowIndices[i] = rowIndices[i + 1];
            colIndices[i] = colIndices[i + 1];
            values[i] = values[i + 1];
        }
    }
    
public:
    // Constructor
    SparseMatrixArray(int r, int c) : rows(r), cols(c), capacity(10), size(0) {
        rowIndices = new int[capacity];
        colIndices = new int[capacity];
        values = new double[capacity];
    }
    
    // Destructor
    ~SparseMatrixArray() {
        delete[] rowIndices;
        delete[] colIndices;
        delete[] values;
    }
    
    // Copy constructor
    SparseMatrixArray(const SparseMatrixArray& other) 
        : rows(other.rows), cols(other.cols), capacity(other.capacity), size(other.size) {
        rowIndices = new int[capacity];
        colIndices = new int[capacity];
        values = new double[capacity];
        
        for (int i = 0; i < size; i++) {
            rowIndices[i] = other.rowIndices[i];
            colIndices[i] = other.colIndices[i];
            values[i] = other.values[i];
        }
    }
    
    // Assignment operator
    SparseMatrixArray& operator=(const SparseMatrixArray& other) {
        if (this != &other) {
            delete[] rowIndices;
            delete[] colIndices;
            delete[] values;
            
            rows = other.rows;
            cols = other.cols;
            capacity = other.capacity;
            size = other.size;
            
            rowIndices = new int[capacity];
            colIndices = new int[capacity];
            values = new double[capacity];
            
            for (int i = 0; i < size; i++) {
                rowIndices[i] = other.rowIndices[i];
                colIndices[i] = other.colIndices[i];
                values[i] = other.values[i];
            }
        }
        return *this;
    }
    
    // Insert a value at given position
    void insert(int row, int col, double value) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw out_of_range("Index out of bounds");
        }
        
        // If value is zero, remove the element if it exists
        if (value == 0.0) {
            remove(row, col);
            return;
        }
        
        int index = findElementIndex(row, col);
        
        if (index != -1) {
            // Element exists, update its value
            values[index] = value;
        } else {
            // Element doesn't exist, insert new element
            if (size >= capacity) {
                resize();
            }
            
            int insertPos = findInsertionPoint(row, col);
            shiftRight(insertPos);
            
            rowIndices[insertPos] = row;
            colIndices[insertPos] = col;
            values[insertPos] = value;
            size++;
        }
    }
    
    // Get value at given position
    double get(int row, int col) const {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw out_of_range("Index out of bounds");
        }
        
        int index = findElementIndex(row, col);
        return (index != -1) ? values[index] : 0.0;
    }
    
    // Set value at given position (alias for insert)
    void set(int row, int col, double value) {
        insert(row, col, value);
    }
    
    // Remove element at given position
    void remove(int row, int col) {
        int index = findElementIndex(row, col);
        if (index != -1) {
            shiftLeft(index);
            size--;
        }
    }
    
    // Add two sparse matrices
    SparseMatrixArray add(const SparseMatrixArray& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw invalid_argument("Matrix dimensions must match for addition");
        }
        
        SparseMatrixArray result(rows, cols);
        
        // Copy all elements from this matrix
        for (int i = 0; i < size; i++) {
            result.insert(rowIndices[i], colIndices[i], values[i]);
        }
        
        // Add elements from other matrix
        for (int i = 0; i < other.size; i++) {
            double currentValue = result.get(other.rowIndices[i], other.colIndices[i]);
            result.insert(other.rowIndices[i], other.colIndices[i], 
                         currentValue + other.values[i]);
        }
        
        return result;
    }
    
    // Multiply two sparse matrices
    SparseMatrixArray multiply(const SparseMatrixArray& other) const {
        if (cols != other.rows) {
            throw invalid_argument("Matrix dimensions incompatible for multiplication");
        }
        
        SparseMatrixArray result(rows, other.cols);
        
        // For each non-zero element in this matrix
        for (int i = 0; i < size; i++) {
            // For each non-zero element in other matrix
            for (int j = 0; j < other.size; j++) {
                if (colIndices[i] == other.rowIndices[j]) {
                    double product = values[i] * other.values[j];
                    double existingValue = result.get(rowIndices[i], other.colIndices[j]);
                    result.insert(rowIndices[i], other.colIndices[j], 
                                existingValue + product);
                }
            }
        }
        
        return result;
    }
    
    // Transpose the matrix
    SparseMatrixArray transpose() const {
        SparseMatrixArray result(cols, rows);
        
        for (int i = 0; i < size; i++) {
            result.insert(colIndices[i], rowIndices[i], values[i]);
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
        for (int i = 0; i < size; i++) {
            cout << "(" << rowIndices[i] << ", " << colIndices[i] << ") = " 
                 << values[i] << endl;
        }
        cout << endl;
    }
    
    // Get number of non-zero elements
    int getNonZeroCount() const {
        return size;
    }
    
    // Check if matrix is empty (all zeros)
    bool isEmpty() const {
        return size == 0;
    }
    
    // Clear all elements
    void clear() {
        size = 0;
    }
    
    // Get matrix dimensions
    pair<int, int> getDimensions() const {
        return make_pair(rows, cols);
    }
    
    // Get memory usage (approximate)
    int getMemoryUsage() const {
        return sizeof(int) * capacity * 2 + sizeof(double) * capacity + sizeof(*this);
    }
    
    // Get capacity
    int getCapacity() const {
        return capacity;
    }
    
    // Get efficiency ratio (non-zero elements / capacity)
    double getEfficiency() const {
        return (capacity > 0) ? (double)size / capacity : 0.0;
    }
};

int main() {
    // Example usage
    SparseMatrixArray matrix(3, 3);
    matrix.insert(0, 0, 1.0);
    matrix.insert(0, 2, 2.0);
    matrix.insert(1, 1, 3.0);
    matrix.insert(2, 0, 4.0);
    matrix.insert(2, 2, 5.0);
    
    cout << "Sparse Matrix:" << endl;
    matrix.display();
    
    return 0;
}
