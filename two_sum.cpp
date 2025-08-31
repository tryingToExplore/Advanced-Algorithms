#include <vector>
#include <iostream>
using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> result;
        for (int i = 0; i < nums.size(); i++) {
            for (int j = i + 1; j < nums.size(); j++) {
                if (static_cast<long long>(nums[i]) + nums[j] == target) {
                    result.push_back(i);
                    result.push_back(j);
                    return result;
                }
            }
        }
        return result;
    }
};

int main() {
    Solution solution;
    
    // Read array size
    cout << "Enter the number of elements in the array (2 to 10000): ";
    int n;
    cin >> n;
    
    // Validate array size
    if (cin.fail() || n < 2 || n > 10000) {
        cout << "Invalid input. Array size must be an integer between 2 and 10^4.\n";
        return 1;
    }
    
    // Read array elements
    vector<int> nums(n);
    cout << "Enter " << n << " integers (each between -10^9 and 10^9): ";
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
        if (cin.fail() || nums[i] < -1000000000 || nums[i] > 1000000000) {
            cout << "Invalid input. Array elements must be integers between -10^9 and 10^9.\n";
            return 1;
        }
    }
    
    // Read target
    cout << "Enter the target sum (between -10^9 and 10^9): ";
    int target;
    cin >> target;
    if (cin.fail() || target < -1000000000 || target > 1000000000) {
        cout << "Invalid input. Target must be an integer between -10^9 and 10^9.\n";
        return 1;
    }
    
    // Call twoSum
    vector<int> result = solution.twoSum(nums, target);
    
    // Print result
    if (result.empty()) {
        cout << "No solution found.\n";
    } else {
        cout << "Result: [" << result[0] << ", " << result[1] << "]\n";
    }
    
    return 0;
}
