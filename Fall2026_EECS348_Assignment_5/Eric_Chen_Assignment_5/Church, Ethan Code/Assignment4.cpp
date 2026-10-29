
/*
 * Program: EECS 348 Assignment 4 - Sudoku Solver
 * Description: Solves five Sudoku puzzles using object-oriented
 * programming and recursive backtracking.
 *
 * Inputs: puzzle1.txt, puzzle2.txt, puzzle3.txt,
 *         puzzle4.txt, puzzle5.txt
 *
 * Output: Displays each original puzzle, all possible
 * solutions, or "No solution found".
 *
 * Author: Ethan Church
 * Creation Date: October 13, 2026
 * Collaborators: Trey Curtis
 * Sources: ChatGPT, Gemini
 *
 * Improvements:
 * - Added more detailed input error messages.
 * - Improved comments for readability.
 * - Kept the optimized recursive backtracking method.
 * 
 * 
 * 
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Sudoku {
private:
    int grid[9][9]{};
    long long solutions = 0;
    string errorMessage;

    /*
     * Checks whether a number can be placed in a cell.
     * The number cannot already exist in the same row,
     * column, or 3x3 block.
     */
    bool canPlace(int row, int col, int digit) const {

        // Check the row and column.
        for (int i = 0; i < 9; i++) {
            if (grid[row][i] == digit ||
                grid[i][col] == digit) {
                return false;
            }
        }

        // Find the beginning of the 3x3 block.
        int startRow = (row / 3) * 3;
        int startCol = (col / 3) * 3;

        // Check every cell inside the block.
        for (int r = startRow; r < startRow + 3; r++) {
            for (int c = startCol; c < startCol + 3; c++) {
                if (grid[r][c] == digit) {
                    return false;
                }
            }
        }

        return true;
    }

    /*
     * Uses recursive backtracking to solve the puzzle.
     * Chooses the empty cell with the fewest valid numbers.
     * If a guess does not work, it removes the number
     * and tries another possibility.
     */
    void backtrack() {

        int bestRow = -1;
        int bestCol = -1;
        int fewest = 10;

        // Search for the empty cell with the fewest choices.
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {

                // Skip cells that already contain a number.
                if (grid[row][col] != 0) {
                    continue;
                }

                int choices = 0;

                // Count the valid numbers for this cell.
                for (int digit = 1; digit <= 9; digit++) {
                    if (canPlace(row, col, digit)) {
                        choices++;
                    }
                }

                // No valid numbers means this path cannot work.
                if (choices == 0) {
                    return;
                }

                // Remember the cell with the fewest choices.
                if (choices < fewest) {
                    fewest = choices;
                    bestRow = row;
                    bestCol = col;
                }
            }
        }

        /*
         * If no empty cells remain, the puzzle is solved.
         * Print the solution and continue searching
         * for any additional solutions.
         */
        if (bestRow == -1) {
            solutions++;

            cout << "Solution " << solutions << ":" << endl;
            print();

            return;
        }

        // Try numbers 1 through 9 in the selected cell.
        for (int digit = 1; digit <= 9; digit++) {

            if (canPlace(bestRow, bestCol, digit)) {

                // Place the number.
                grid[bestRow][bestCol] = digit;

                // Recursively solve the remaining puzzle.
                backtrack();

                // Remove the number to try another possibility.
                grid[bestRow][bestCol] = 0;
            }
        }
    }

public:

    /*
     * Reads a Sudoku puzzle from a text file.
     * Underscores represent empty cells.
     * Displays a specific error if the file is invalid.
     */
    bool load(const string& filename) {

        ifstream input(filename);

        // Check whether the file exists and can be opened.
        if (!input.is_open()) {
            errorMessage = "Could not open file: " + filename;
            return false;
        }

        char cell;

        // Read all 81 cells from the file.
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {

                // Check for missing puzzle data.
                if (!(input >> cell)) {
                    errorMessage =
                        "Incomplete puzzle: Expected 81 cells.";
                    return false;
                }

                // Convert underscores into empty cells.
                if (cell == '_') {
                    grid[row][col] = 0;
                }

                // Convert character digits into integers.
                else if (cell >= '1' && cell <= '9') {
                    grid[row][col] = cell - '0';
                }

                // Reject invalid characters.
                else {
                    errorMessage =
                        "Invalid character found in puzzle.";
                    return false;
                }
            }
        }

        // Check whether the file contains extra data.
        if (input >> cell) {
            errorMessage =
                "Invalid puzzle: More than 81 cells found.";
            return false;
        }

        solutions = 0;
        errorMessage = "";

        return true;
    }

    /*
     * Prints the Sudoku grid.
     * Empty cells are displayed as underscores.
     */
    void print() const {

        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {

                if (grid[row][col] == 0) {
                    cout << "_";
                }
                else {
                    cout << grid[row][col];
                }

                if (col < 8) {
                    cout << " ";
                }
            }

            cout << endl;
        }

        cout << endl;
    }

    /*
     * Checks whether the original puzzle is valid.
     * Filled cells cannot contain duplicate numbers
     * in the same row, column, or 3x3 block.
     */
    bool validStartingGrid() const {

        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {

                int digit = grid[row][col];

                // Empty cells do not need to be checked.
                if (digit == 0) {
                    continue;
                }

                // Check for duplicates in rows and columns.
                for (int i = 0; i < 9; i++) {

                    if (i != col && grid[row][i] == digit) {
                        return false;
                    }

                    if (i != row && grid[i][col] == digit) {
                        return false;
                    }
                }

                // Find the starting position of the 3x3 block.
                int startRow = (row / 3) * 3;
                int startCol = (col / 3) * 3;

                // Check the block for duplicate numbers.
                for (int r = startRow; r < startRow + 3; r++) {
                    for (int c = startCol; c < startCol + 3; c++) {

                        if ((r != row || c != col) &&
                            grid[r][c] == digit) {
                            return false;
                        }
                    }
                }
            }
        }

        return true;
    }

    // Returns the error message if loading fails.
    string getError() const {
        return errorMessage;
    }

    /*
     * Starts the solving process.
     * Checks the original puzzle before using backtracking.
     * Displays all solutions or reports if none exist.
     */
    void solve() {

        if (!validStartingGrid()) {
            cout << "Invalid starting puzzle: "
                 << "Conflicting numbers found." << endl;
            cout << "No solution found" << endl;
            cout << "Solutions found: 0" << endl;
            return;
        }

        // Start recursive backtracking.
        backtrack();

        // Report if no valid solutions were found.
        if (solutions == 0) {
            cout << "No solution found" << endl;
        }

        // Display the total number of solutions.
        cout << "Solutions found: " << solutions << endl;
    }
};

/*
 * Main program:
 * Reads and solves all five Sudoku puzzle files.
 * Each puzzle uses a new Sudoku object.
 */
int main() {

    for (int number = 1; number <= 5; number++) {

        // Create the filename for the current puzzle.
        string filename =
            "puzzle" + to_string(number) + ".txt";

        // Create a new Sudoku object.
        Sudoku puzzle;

        cout << "========== " << filename
             << " ==========" << endl;

        // Attempt to load the puzzle.
        if (!puzzle.load(filename)) {

            // Display the specific error message.
            cout << "Error: " << puzzle.getError()
                 << endl << endl;

            // Continue to the next puzzle.
            continue;
        }

        // Display the original puzzle.
        cout << "Original Puzzle:" << endl;
        puzzle.print();

        // Solve the puzzle and display its solutions.
        puzzle.solve();

        cout << endl;
    }

    return 0;
}
