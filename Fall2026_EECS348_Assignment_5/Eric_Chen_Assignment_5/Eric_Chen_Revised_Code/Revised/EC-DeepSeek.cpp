/*
 * Program: EECS 348 Assignment 4 - Sudoku Solver
 * Description: Solves five Sudoku puzzles using object-oriented
 * programming and recursive backtracking. Displays results on the
 * console and also writes each puzzle's solutions to solutionN.txt.
 *
 * Inputs: puzzle1.txt, puzzle2.txt, puzzle3.txt,
 *         puzzle4.txt, puzzle5.txt
 *
 * Outputs: Console display + solution1.txt ... solution5.txt
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
 * Author: Eric Chen
 * Creation Date: October 28th, 2026
 * Sources: DeepSeek
 * 
 * 
 * Improvements:
 * - Added output files solutionN.txt.
 * - Writes all solutions found for each puzzle.
 * - Writes "No solution found." when no solutions exist.
 * - Handles completely empty board gracefully.
 * - Distinguishes file I/O errors from unsolvable puzzles.
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
     * Prints every complete solution to the console and
     * writes it to the output file.
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
         * Write the solution to the console and to the file,
         * then continue searching for additional solutions.
         */
        if (bestRow == -1) {
            solutions++;

            cout << "Solution " << solutions << ":" << endl;
            print(cout);

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
     * Checks whether the board is completely empty.
     */
    bool isEmptyBoard() const {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                if (grid[row][col] != 0) {
                    return false;
                }
            }
        }
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
     * Starts the solving process.
     * Prints results to the console and also writes them
     * to the specified solution file.
     */
    void solve(const string& solutionFilename) {

        solutionOut.open(solutionFilename);

        if (!solutionOut.is_open()) {
            cerr << "Error: Could not open solution file: "
                 << solutionFilename << endl;
            return;
        }

        // Edge case: completely empty board
        if (isEmptyBoard()) {
            cout << "Board is empty; cannot enumerate all solutions."
                 << endl;

            solutionOut << "Board is empty; cannot enumerate all solutions."
                        << endl;

            solutionOut.close();
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
 * Displays the original puzzle and every solution on the console,
 * and mirrors the results into solutionN.txt.
 * I/O failures are distinguished from unsolvable puzzles.
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

        // Attempt to load the puzzle.
        if (!puzzle.load(filename)) {

            // File read error – do NOT treat as "no solution".
            cout << "Error: " << puzzle.getError() << endl << endl;

            ofstream out(solutionFilename);
            if (out.is_open()) {
                out << "Error: Could not read puzzle file." << endl;
            } else {
                cerr << "Error: Could not create solution file: "
                     << solutionFilename << endl;
            }

            continue;
        }

        // Display the original puzzle on the console.
        cout << "Original Puzzle:" << endl;
        puzzle.print();

        // Solve, print solutions to the console, and write to file.
        puzzle.solve(solutionFilename);

        cout << endl;
    }

    return 0;
}