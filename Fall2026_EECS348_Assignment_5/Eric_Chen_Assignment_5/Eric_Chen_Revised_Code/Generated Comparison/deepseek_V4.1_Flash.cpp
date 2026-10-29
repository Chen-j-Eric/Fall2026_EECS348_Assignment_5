/*
 * Program: EECS 348 Assignment 4 - Sudoku Solver
 * Description: Solves five Sudoku puzzles using object-oriented
 * programming and recursive backtracking. Writes each puzzle's
 * solutions to solutionN.txt.
 *
 * Inputs: puzzle1.txt, puzzle2.txt, puzzle3.txt,
 *         puzzle4.txt, puzzle5.txt
 *
 * Outputs: solution1.txt ... solution5.txt
 *
 * Author: Ethan Church
 * Creation Date: October 13, 2026
 * Collaborators: Trey Curtis
 * Sources: ChatGPT, Gemini
 *
 * Improvements:
 * - Added output files solutionN.txt.
 * - Writes all solutions found for each puzzle.
 * - Writes "No solution found." when no solutions exist.
 * - Keeps detailed input error messages.
 * 
 * Author: Eric Chen
 * Creation Date: October 28th, 2026
 * Sources: DeepSeek
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
    ofstream solutionOut;

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
     * Writes every complete solution to the output file.
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
         * Write the solution and continue searching
         * for any additional solutions.
         */
        if (bestRow == -1) {
            solutions++;
            print(solutionOut);
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
     * Prints the Sudoku grid to an output stream.
     * Empty cells are displayed as underscores.
     * The format matches the puzzle input files.
     */
    void print(ostream& out) const {

        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {

                if (grid[row][col] == 0) {
                    out << "_";
                }
                else {
                    out << grid[row][col];
                }

                out << " ";
            }

            out << endl;
        }

        out << endl;
    }

    /*
     * Prints the Sudoku grid to the console.
     */
    void print() const {
        print(cout);
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
     * Starts the solving process and writes the result
     * to the specified solution file.
     */
    void solve(const string& solutionFilename) {

        solutionOut.open(solutionFilename);

        if (!solutionOut.is_open()) {
            cout << "Error: Could not open solution file: "
                 << solutionFilename << endl;
            return;
        }

        if (!validStartingGrid()) {
            cout << "Invalid starting puzzle: "
                 << "Conflicting numbers found." << endl;
            cout << "No solution found." << endl;

            solutionOut << "No solution found." << endl;
            solutionOut.close();
            return;
        }

        // Start recursive backtracking.
        backtrack();

        if (solutions == 0) {
            cout << "No solution found." << endl;
            solutionOut << "No solution found." << endl;
        }
        else {
            cout << "Solutions found: " << solutions << endl;
            solutionOut << "Solutions found: " << solutions << endl;
        }

        solutionOut.close();
    }
};

/*
 * Main program:
 * Reads and solves all five Sudoku puzzle files.
 * Each puzzle writes its result to solutionN.txt.
 */
int main() {

    for (int number = 1; number <= 5; number++) {

        string filename =
            "puzzle" + to_string(number) + ".txt";

        string solutionFilename =
            "solution" + to_string(number) + ".txt";

        Sudoku puzzle;

        cout << "========== " << filename
             << " ==========" << endl;

        if (!puzzle.load(filename)) {

            cout << "Error: " << puzzle.getError()
                 << endl << endl;

            // Still create the required solution file.
            ofstream out(solutionFilename);
            if (out.is_open()) {
                out << "No solution found." << endl;
            }

            continue;
        }

        cout << "Original Puzzle:" << endl;
        puzzle.print();

        puzzle.solve(solutionFilename);

        cout << endl;
    }

    return 0;
}
