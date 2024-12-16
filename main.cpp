import sys

# GRID_SIZE is the size of the 2D matrix   GRID_SIZE*GRID_SIZE
GRID_SIZE = 9


# A utility function to print a grid
def printing(arr):
    for i in range(len(arr)):
        for j in range(len(arr[i])):
            print(arr[i][j], end=" ")
        print()
    print()

# Verify that white dot constraint is satisfied
def check_white(other, num):
    if other == 0: return True
    return abs(other - num) == 1

# Verify that black dot constraint is satisfied
def check_black(other, num):
    if other == 0: return True
    return (other * 2 == num) or (num * 2) == other


def row_col_subgrid_traverse(grid, row, col, action):
    """
    This will traverse the corresponding row, column, and subgrid
    and call action (a function passed as a parameter) on each cell
    """
    for c in range(9):
        if c == col: continue
        action(grid[row][c])

    for r in range(9):
        if r == row: continue
        action(grid[r][col])
    startRow = row - row % 3
    startCol = col - col % 3
    for i in range(3):
        if i == row: continue
        for j in range(3):
            if j == col: continue
            action(grid[i + startRow][j + startCol])


def dot_traverse(grid, row, col, hor_dots, vert_dots, action):
    """
    Given a cell, look up all adjacent cells that are
    involved in dot constraints with it and pass them to
    action (a function passed as a parameter).
    """
    # case 1: dot b/w me and left
    # col - 1
    if 0 <= row < len(hor_dots) and 0 <= col - 1 < len(hor_dots[0]):
        action(hor_dots[row][col - 1], grid[row][col - 1])

    # case 2 : dot b/w me and right
    # col
    if 0 <= row < len(hor_dots) and 0 <= col < len(hor_dots[0]):
        action(hor_dots[row][col], grid[row][col + 1])
    # case 3: dot b/w me and above
    # row - 1 
    if 0 <= row - 1 < len(vert_dots) and 0 <= col < len(vert_dots[0]):
        action(vert_dots[row - 1][col], grid[row - 1][col])
    # case 4: dot b/w me and below
    # row
    if 0 <= row < len(vert_dots) and 0 <= col < len(vert_dots[0]):
        action(vert_dots[row][col], grid[row + 1][col])


def isSafe(grid, row, col, num, hor_dots, vert_dots):
    """
    Given the grid, position (row, col), projected cell value indicated
    by num, and a grid of the vertical and horizontal constraints,
    determine whether "num" violates any constraints
    """
    is_diff = True

    # Check allDiff constraint
    def diff_check(value):
        nonlocal is_diff
        if value == num:
            is_diff = False
    # Traverse the cell's row, column, and subgrid to verify allDiff
    row_col_subgrid_traverse(grid, row, col, diff_check)
    if not is_diff: return False

    # Check dot constraints
    def dot_check(dot, cell):
        nonlocal is_diff
        if dot == 1 and \
                not check_white(cell, num):
            is_diff = False
        elif dot == 2 and \
                not check_black(cell, num):
            is_diff = False
    # Traverse the cell's adjacent tiles, determine if any dot constraint is violated
    dot_traverse(grid, row, col, hor_dots, vert_dots, dot_check)
    if not is_diff: return False
    return True

# Construct list of coordinates with smallest domain size
def mrv(grid, hor_dots, vert_dots):
    candidates = []
    min_legal_states = 10
    # iterate through every cell in the grid and count how many legal states they have
    for r in range(9):
        for c in range(9):
            legal_states = 0
            if grid[r][c] == 0:  # skip assigned cells
                for val in range(1, 10):
                    if isSafe(grid, r, c, val, hor_dots, vert_dots): legal_states += 1
                if legal_states < min_legal_states: # smaller domain size found
                    candidates = [(r, c)]
                    min_legal_states = legal_states
                elif legal_states == min_legal_states: # add to coordinate list
                    candidates.append((r, c))
    return candidates


def row_grid_sub_degs(value, deg):
    if value == 0:  # Unassigned cells contribute to the degree
        deg[0] += 1

def dot_check_degs(dot, cell, deg):
    if dot != 0 and cell == 0:  # Unassigned cells with dot constraints
        deg[0] += 1


def degree_heuristic(grid, mrv_results, hor_dots, vert_dots):
    max_degree = -1
    max_var = tuple() # track coordinate with most constraints

    for coord in mrv_results:
        degree = [0]  # update element within a mutable list
        r, c = coord[0], coord[1]
        # pass value of a cell to row_grid_sub_degs using lambda
        # count degree based on the row, column, and subgrid
        row_col_subgrid_traverse(grid, r, c, lambda value: row_grid_sub_degs(value, degree))

        # pass the dot (hor/vert) and the adjacent cell's value to dot_check_degs using lambda
        # count degree based on the dot constraints
        dot_traverse(grid, r, c, hor_dots, vert_dots, lambda dot, cell: dot_check_degs(dot, cell, degree))

        # Track the candidate with the highest degree
        if degree[0] > max_degree:
            max_degree = degree[0]
            max_var = coord

    return max_var


def count_zeros(matrix):
    count = 0
    for r in range(len(matrix)):
        for c in range(len(matrix[r])):
            if matrix[r][c] == 0:
                count += 1
    return count

# Select a coordinate with the smallest domain and highest number of constraints
def select_unassigned_variable(grid, hor_dots, vert_dots):
    return degree_heuristic(grid, mrv(grid, hor_dots, vert_dots), \
                            hor_dots, vert_dots)


def backtrack(grid, hor_dots, vert_dots):
    # Grid is complete
    if count_zeros(grid) == 0: return True

    # Select the next coordinate to populate
    row, col = select_unassigned_variable(grid, hor_dots, vert_dots)

    # num represents all possible values from [1, 9]
    for num in range(1, GRID_SIZE + 1):
        # verify whether num violates any constraint
        if isSafe(grid, row, col, num, hor_dots, vert_dots):
            grid[row][col] = num # try assignment
            if backtrack(grid, hor_dots, vert_dots):
                return True
        grid[row][col] = 0  # undo assignment if backtracking yields no solution
    return False


def parse_file():
    if (len(sys.argv) != 2):
        print("Input file required")
        return []

    # Initialize empty matrices
    matrix_9x9 = []  # grid
    matrix_9x8 = []  # hor_dots
    matrix_8x9 = []  # vert_dots

    with open(sys.argv[1], 'r') as file:
        lines = file.readlines()

        # Assume matrices are separated by a blank line
        # Split the content into the three sections based on blank lines
        sections = ''.join(lines).strip().split("\n\n")

        # Load 9x9 matrix
        for line in sections[0].splitlines():
            matrix_9x9.append(list(map(int, line.split())))

        # Load 9x8 matrix
        for line in sections[1].splitlines():
            matrix_9x8.append(list(map(int, line.split())))

        # Load 8x9 matrix
        for line in sections[2].splitlines():
            matrix_8x9.append(list(map(int, line.split())))

    return matrix_9x9, matrix_9x8, matrix_8x9


def main():
    grid, hor_dots, vert_dots = parse_file()

    if (backtrack(grid, hor_dots, vert_dots)):
        printing(grid)
        print()
    else:
        printing(grid)
        print("no solution  exists ")
        print()

main()
