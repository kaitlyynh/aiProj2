#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
using namespace std;
/*
 * Constraints:
 *  - white dot btwn adjacent cells require abs(cell_1 - cell_2) = 1
 *  - black dot btwn adjacent cells require one to be double of the other
 *  Notes:
 *  horizontal constraints are between the current cell & the immediate right
 *  vertical constraints are between the current cell & directly below it
 */
struct Cell {
    int val;
    set<int> domain = {1, 2, 3, 4, 5, 6, 7, 8, 9};
};
const int NUM_ROWS_COLS = 9,
    NUM_CELL_GROUPS = 3;

vector<vector<int>> g_board;
vector<vector<int>> g_horizontal_dots;
vector<vector<int>> g_vertical_dots;
map<int, set<int>> g_row_to_values; // map row # to vals in row
map<int, set<int>> g_col_to_values; // map col # to vals in col
map<pair<int, int>, set<int>> g_subgrid_to_values; // map subgrid coordinates to vals in subgrid

void parse_file(const string& filepath){ // populate board & constraint matrices
    ifstream file_obj(filepath);
    string line;
    for (int i = 0; i < NUM_CELL_GROUPS; i++) {
        for (int j = 0; j < NUM_ROWS_COLS; j++) {
            getline(file_obj, line);
            vector<int> row;
            istringstream ss(line);
            int number;
            while (ss >> number && row.size() < NUM_ROWS_COLS) {
                row.push_back(number);
            }
            if (i == 0) {
                g_board.push_back(row);
            } else if (i == 1) {
                g_horizontal_dots.push_back(row);
            } else if (i == 2 && j < (NUM_ROWS_COLS - 1)) {
                g_vertical_dots.push_back(row);
            }
            if ((i == 0 || i == 1) && (j == (NUM_ROWS_COLS - 1))) {
                getline(file_obj, line);
            }
        }
    }

    file_obj.close();
}
void print_matrix(const vector<vector<int>>& some_matrix) {
    for (vector<int> row : some_matrix) {
        for (int val : row) { cout << val << " "; }
        cout << endl;
    }

}
void initialize() {
    for (int i = 0; i < NUM_ROWS_COLS; i++) {
        for (int j = 0; j < NUM_ROWS_COLS; j++) {
            pair<int, int> subgrid_coords = {i / 3, j / 3};
            if (g_board[i][j] != 0) { // add val to set, row & cols
                g_subgrid_to_values[subgrid_coords].insert(g_board[i][j]);
                g_row_to_values[i].insert(g_board[i][j]);
                g_col_to_values[j].insert(g_board[i][j]);
            }
        }
    }
}

int main(int argc, char* argv[]) {
//    if (argc != 2) {cerr << "Requires input file\n"; return -1;}
//    string file_name = argv[1];

    string file_name = "Sample_Input.txt";
    parse_file(file_name);

    initialize();


}

void backtrack() {
    for (int i = 0; i < NUM_ROWS_COLS; i++) {
        for (int j = 0; j < NUM_ROWS_COLS; j++) {
            set<int> domain = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            if (g_board[i][j] == 0) { // select unassigned variable (?)

                for (int val : g_row_to_values[i]) {
                    domain.erase(val);
                }
                for (int val : g_col_to_values[j]) {
                    domain.erase(val);
                }
                for (int val : g_subgrid_to_values[pair<int, int>{i / 3, j / 3}]) {
                    domain.erase(val);
                }
                if (g_horizontal_dots[i][j] == 1) { // white horizontal dot
                    if (g_horizontal_dots[i][j + 1] != 0) {}
                } else if (g_horizontal_dots[i][j] == 2) { // black horizontal dot

                }
                if (!domain.empty()) { // domain has values to pick from
                    g_board[i][j] = *domain.begin(); // assign to first value
                } else {
                    cerr << "Failed at (i, j) = " << i << " " << j << endl;
                    return; // failed if there are no values to pick from
                }
            }
        }
    }
}
