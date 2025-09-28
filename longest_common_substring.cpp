#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <set>
#include <chrono>
#include <sstream>
#include <cctype>

using namespace std;
using namespace std::chrono;

class StringMatcher {
private:
    string text1, text2;
    vector<vector<int>> table;
    int longestMatch;
    vector<string> foundMatches;
    vector<pair<int, int>> matchLocations;

    const string NORMAL = "\033[0m";
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string YELLOW = "\033[33m";
    const string BLUE = "\033[34m";
    const string PURPLE = "\033[35m";
    const string CYAN = "\033[36m";
    const string BOLD = "\033[1m";

public:
    StringMatcher(const string& first, const string& second) 
        : text1(first), text2(second), longestMatch(0) {
        int rows = text1.length();
        int cols = text2.length();
        table = vector<vector<int>>(rows + 1, vector<int>(cols + 1, 0));
    }

    void findMatches() {
        auto startTime = high_resolution_clock::now();
        
        int rows = text1.length();
        int cols = text2.length();
        longestMatch = 0;
        foundMatches.clear();
        matchLocations.clear();
        
        cout << "Let me work through this step by step..." << endl;
        
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                if (text1[i-1] == text2[j-1]) {
                    table[i][j] = table[i-1][j-1] + 1;
                    longestMatch = max(longestMatch, table[i][j]);
                } else {
                    table[i][j] = 0;
                }
            }
        }
        
        set<string> uniqueMatches;
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                if (table[i][j] == longestMatch && longestMatch > 0) {
                    int startPos = i - longestMatch;
                    string match = text1.substr(startPos, longestMatch);
                    if (uniqueMatches.find(match) == uniqueMatches.end()) {
                        uniqueMatches.insert(match);
                        foundMatches.push_back(match);
                        matchLocations.push_back({startPos, j - longestMatch});
                    }
                }
            }
        }
        
        auto endTime = high_resolution_clock::now();
        auto timeTaken = duration_cast<microseconds>(endTime - startTime);
        cout << CYAN << "Done! That took " << timeTaken.count() << " microseconds." << NORMAL << endl;
    }

    void showTable() {
        int rows = text1.length();
        int cols = text2.length();
        
        cout << "\n" << BOLD << GREEN << "Here's how I compared your strings:" << NORMAL << endl;
        cout << "Each number shows how many characters matched in a row at that position.\n" << endl;
        
        cout << setw(6) << " ";
        for (int j = 0; j < cols; j++) {
            cout << setw(4) << j;
        }
        cout << endl;
        
        cout << setw(6) << " ";
        for (int j = 0; j < cols; j++) {
            cout << setw(4) << text2[j];
        }
        cout << endl;
        
        cout << string(50, '-') << endl;
        for (int i = 0; i <= rows; i++) {
            if (i == 0) {
                cout << setw(2) << " " << setw(4) << " ";
            } else {
                cout << setw(2) << (i-1) << setw(4) << text1[i-1];
            }
            
            for (int j = 0; j <= cols; j++) {
                if (table[i][j] == longestMatch && longestMatch > 0) {
                    cout << BOLD << RED << setw(4) << table[i][j] << NORMAL;
                } else if (table[i][j] > 0) {
                    cout << YELLOW << setw(4) << table[i][j] << NORMAL;
                } else {
                    cout << setw(4) << table[i][j];
                }
            }
            cout << endl;
        }
        cout << string(50, '-') << endl;
    }

    void showResults() {
        cout << "\n" << BOLD << BLUE << "What I Found:" << NORMAL << endl;
        cout << string(40, '=') << endl;
        
        cout << CYAN << "Your strings were:" << NORMAL << endl;
        cout << "First:  \"" << BOLD << text1 << NORMAL << "\" (" << text1.length() << " characters)" << endl;
        cout << "Second: \"" << BOLD << text2 << NORMAL << "\" (" << text2.length() << " characters)" << endl;
        
        cout << "\n" << GREEN << "The Results:" << NORMAL << endl;
        cout << "Longest matching piece: " << BOLD << RED << longestMatch << NORMAL << " characters long" << endl;
        
        if (longestMatch > 0) {
            if (foundMatches.size() == 1) {
                cout << "I found: \"" << BOLD << foundMatches[0] << NORMAL << "\"" << endl;
                cout << "It starts at position " << matchLocations[0].first << " in the first string" << endl;
                cout << "and position " << matchLocations[0].second << " in the second string." << endl;
            } else {
                cout << YELLOW << "Actually, I found " << foundMatches.size() << " different matches of the same length!" << NORMAL << endl;
                for (size_t i = 0; i < foundMatches.size(); i++) {
                    cout << "  " << (i+1) << ") \"" << BOLD << foundMatches[i] << NORMAL << "\"" << endl;
                    cout << "     Starts at position " << matchLocations[i].first << " in first string" << endl;
                    cout << "     and position " << matchLocations[i].second << " in second string" << endl;
                }
            }
        } else {
            cout << RED << "No matching substrings found." << NORMAL << endl;
        }
        cout << string(40, '=') << endl;
    }

    string getResult() const {
        return foundMatches.empty() ? "" : foundMatches[0];
    }

    int getMaxLength() const {
        return longestMatch;
    }
};

void testWithExample() {
    cout << "\nExample: Finding longest common substring between 'ABAB' and 'BABA'" << endl;
    cout << string(50, '-') << endl;
    
    StringMatcher matcher("ABAB", "BABA");
    matcher.findMatches();
    matcher.showTable();
    matcher.showResults();
}

void solveUserInput() {
    string str1, str2;
    
    cout << "\nEnter first string: ";
    cin >> str1;
    cout << "Enter second string: ";
    cin >> str2;
    
    cout << "\nFinding longest common substring..." << endl;
    cout << string(40, '-') << endl;
    
    StringMatcher matcher(str1, str2);
    matcher.findMatches();
    matcher.showTable();
    matcher.showResults();
}

int main() {
    cout << "Longest Common Substring Finder" << endl;
    cout << "===============================" << endl;
    
    int choice;
    
    while (true) {
        cout << "\nOptions:" << endl;
        cout << "1. Try example (ABAB and BABA)" << endl;
        cout << "2. Enter your own strings" << endl;
        cout << "3. Quit" << endl;
        cout << "Choose (1-3): ";
        
        cin >> choice;
        
        switch (choice) {
            case 1:
                testWithExample();
                break;
                
            case 2:
                solveUserInput();
                break;
                
            case 3:
                cout << "\nGoodbye!" << endl;
                return 0;
                
            default:
                cout << "Invalid choice. Try again." << endl;
                break;
        }
    }
    
    return 0;
}
