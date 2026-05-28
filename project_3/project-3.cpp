#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// ============================================================
// (1) Recursive Definition of P(C)
// ============================================================
//
//   P(C) = 0                                        if C = 0
//   P(C) = max { p_i + P(C - w_i) | w_i <= C }     if C > 0
//
//   where i ranges over all n item types (0 to n-1).
//   There are unlimited copies of each item type.
//
// ============================================================


// ============================================================
// (3) Bottom-Up Dynamic Programming Algorithm
// ============================================================
//
//   Create dp[0..C], initialised to 0.
//   For each capacity c from 1 to C:
//       For each item i (0 to n-1):
//           If w_i <= c:
//               dp[c] = max(dp[c], p_i + dp[c - w_i])
//   Return dp[C].
//
//   Time complexity:  O(C * n)
//   Space complexity: O(C)
//
// ============================================================

// Returns the full dp table AND prints it step by step
vector<int> unboundedKnapsackBottomUp(int capacity,
                                       const vector<int>& weights,
                                       const vector<int>& profits) {
    vector<int> dp(capacity + 1, 0);

    for (int c = 1; c <= capacity; ++c) {
        for (size_t i = 0; i < weights.size(); ++i) {
            if (weights[i] <= c) {
                dp[c] = max(dp[c], profits[i] + dp[c - weights[i]]);
            }
        }
    }
    return dp;
}

// ============================================================
// (2) Subproblem Graph for P(14), case (w,p) = {(4,7),(6,6),(8,9)}
// ============================================================
//
//   Each node is a subproblem P(c). There is a directed edge
//   from P(c) to P(c - w_i) whenever w_i <= c.
//
//   Starting from P(14), the reachable nodes and their edges are:
//
//   P(14) --> P(10) [via w=4], P(8) [via w=6], P(6) [via w=8]
//   P(10) --> P( 6) [via w=4], P(4) [via w=6], P(2) [via w=8]
//   P( 8) --> P( 4) [via w=4], P(2) [via w=6], P(0) [via w=8]
//   P( 6) --> P( 2) [via w=4], P(0) [via w=6]
//   P( 4) --> P( 0) [via w=4]
//   P( 2) --> (no outgoing edges, all weights > 2)
//   P( 0) --> (base case, value = 0)
//
//   Note: P(2) is a dead end — no item fits, so P(2) = 0.
//
// ============================================================

void printSubproblemGraph() {
    cout << "------------------------------------------------------------\n";
    cout << "(2) Subproblem Graph for P(14)\n";
    cout << "    Items: w={4,6,8}, p={7,6,9}\n";
    cout << "------------------------------------------------------------\n";
    cout << "  Reachable nodes: { 14, 10, 8, 6, 4, 2, 0 }\n\n";
    cout << "  Directed edges (P(c) --> P(c - w_i)):\n";
    cout << "    P(14) --> P(10) [w=4], P(8) [w=6], P(6) [w=8]\n";
    cout << "    P(10) --> P( 6) [w=4], P(4) [w=6], P(2) [w=8]\n";
    cout << "    P( 8) --> P( 4) [w=4], P(2) [w=6], P(0) [w=8]\n";
    cout << "    P( 6) --> P( 2) [w=4], P(0) [w=6]\n";
    cout << "    P( 4) --> P( 0) [w=4]\n";
    cout << "    P( 2) --> (no outgoing edges — no item fits)\n";
    cout << "    P( 0) --> (base case)\n";
    cout << "\n";
}

// Helper: print the full dp table and trace the optimal solution
void runCase(const string& label,
             int capacity,
             const vector<int>& weights,
             const vector<int>& profits) {

    cout << "------------------------------------------------------------\n";
    cout << label << "\n";
    cout << "------------------------------------------------------------\n";
    cout << "  Capacity C = " << capacity << "\n";

    cout << "  Weights : ";
    for (size_t i = 0; i < weights.size(); ++i)
        cout << weights[i] << (i + 1 < weights.size() ? ", " : "\n");

    cout << "  Profits : ";
    for (size_t i = 0; i < profits.size(); ++i)
        cout << profits[i] << (i + 1 < profits.size() ? ", " : "\n");

    vector<int> dp = unboundedKnapsackBottomUp(capacity, weights, profits);

    // Print the full dp table
    cout << "\n  Bottom-up DP table (dp[0] to dp[" << capacity << "]):\n  ";
    for (int c = 0; c <= capacity; ++c)
        cout << "dp[" << c << "]=" << dp[c]
             << (c < capacity ? "  " : "\n");

    cout << "\n  Maximum profit P(" << capacity << ") = " << dp[capacity] << "\n";

    // Trace back which items were chosen
    cout << "\n  Items chosen (greedy trace from dp table):\n";
    int rem = capacity;
    int itemCount = 0;
    while (rem > 0) {
        bool found = false;
        for (size_t i = 0; i < weights.size(); ++i) {
            if (weights[i] <= rem &&
                dp[rem] == profits[i] + dp[rem - weights[i]]) {
                cout << "    Take item " << i
                     << " (w=" << weights[i]
                     << ", p=" << profits[i]
                     << "), remaining capacity: "
                     << rem << " -> " << rem - weights[i] << "\n";
                rem -= weights[i];
                ++itemCount;
                found = true;
                break;
            }
        }
        if (!found) break;   // e.g. P(2) = 0, nothing fits
    }
    if (itemCount == 0)
        cout << "    (no items fit — profit = 0)\n";

    cout << "\n";
}

int main() {

    // ----------------------------------------------------------
    // (1) Recursive definition
    // ----------------------------------------------------------
    cout << "============================================================\n";
    cout << "(1) Recursive Definition of P(C)\n";
    cout << "============================================================\n";
    cout << "  Base case : P(0) = 0\n";
    cout << "  Recursive : P(C) = max{ p_i + P(C - w_i) | w_i <= C }\n";
    cout << "              for i = 0, 1, ..., n-1  (C > 0)\n\n";

    // ----------------------------------------------------------
    // (2) Subproblem graph
    // ----------------------------------------------------------
    printSubproblemGraph();

    // ----------------------------------------------------------
    // (3) Algorithm description (printed, not just a comment)
    // ----------------------------------------------------------
    cout << "============================================================\n";
    cout << "(3) Bottom-Up Dynamic Programming Algorithm\n";
    cout << "============================================================\n";
    cout << "  1. Initialise dp[0..C] = 0.\n";
    cout << "  2. For c = 1 to C:\n";
    cout << "       For each item i (0 to n-1):\n";
    cout << "         If w_i <= c:\n";
    cout << "           dp[c] = max(dp[c], p_i + dp[c - w_i])\n";
    cout << "  3. Return dp[C].\n";
    cout << "\n  Time:  O(C * n)\n";
    cout << "  Space: O(C)\n\n";

    // ----------------------------------------------------------
    // (4a) P(14) with weights {4,6,8}, profits {7,6,9}
    // ----------------------------------------------------------
    cout << "============================================================\n";
    cout << "(4) Running Results\n";
    cout << "============================================================\n\n";

    runCase("(4a) weights={4,6,8}, profits={7,6,9}",
            14, {4, 6, 8}, {7, 6, 9});

    // ----------------------------------------------------------
    // (4b) P(14) with weights {5,6,8}, profits {7,6,9}
    // ----------------------------------------------------------
    runCase("(4b) weights={5,6,8}, profits={7,6,9}",
            14, {5, 6, 8}, {7, 6, 9});

    return 0;
}
