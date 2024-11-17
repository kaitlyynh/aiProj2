#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
/*
 * Constraints:
 *  - white dot btwn adjacent cells require abs(cell_1 - cell_2) = 1
 *  - black dot btwn adjacent cells require one to be double of the other
 */
const int NUM_ROWS_COLS = 9,
    NUM_CELL_GROUPS = 3;

vector<vector<int>> g_board;
vector<vector<int>> g_horizontal_dots; // curr position and tile to the right
vector<vector<int>> g_vertical_dots; // curr position and tile below it

void parse_file(const string& filepath){
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

int main(int argc, char* argv[]) {
//    if (argc != 2) {cerr << "Requires input file\n"; return -1;}
//    string file_name = argv[1];

    string file_name = "Sample_Input.txt";

    parse_file(file_name);
    print_matrix(g_board);
    cout << "= = = = = = = = = =" << endl;
    print_matrix(g_horizontal_dots);
    cout << "= = = = = = = = = =" << endl;
    print_matrix(g_vertical_dots);
    cout << "= = = = = = = = = =" << endl;


}
