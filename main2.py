import sys
# GRID_SIZE is the size of the 2D matrix   GRID_SIZE*GRID_SIZE
GRID_SIZE = 9
 
# A utility function to print grid
def printing(arr):
    for i in range(len(arr)):
        for j in range(len(arr[i])):
            print(arr[i][j], end = " ")
        print()
    print()


def check_white(other, num): #White dot constraint
    if other == 0: return False
    return abs(other - num) == 1
    
def check_black(other, num): # Black dot constraint
    if other == 0: return False
    return (other / num) == 0.5 or (other / num) == 2


def row_col_subgrid_traverse(grid, row, col, action): 
    """
    this will traverse the corresponding row, column, and subgrid
    and call action on each cell 
    """
    for c in range(9):
        action(grid[row][c])

    for r in range(9):
        action(grid[r][col])
    startRow = row - row % 3
    startCol = col - col % 3
    for i in range(3):
        for j in range(3):
            action(grid[i + startRow][j + startCol])


def dot_traverse(grid, row, col, hor_dots, vert_dots, action):
    """
    Given a cell, it will look up all adjacent cells that are
    involved in dot contraints with it and pass them to the function
    action.
    
    """
    # case 1: dot b/w me and left
    # col - 1
    if 0 <= row < len(hor_dots) and 0 <= col - 1 < len(hor_dots[0]):
        action(hor_dots[row][col - 1], grid[row][col - 1])
        
    # case 2 : dot b/w me and right
    # col 
    if 0 <= row < len(hor_dots) and 0 <= col < len(hor_dots[0]):
        action(hor_dots[row][col - 1], grid[row][col + 1])
    #case 3: dot b/w me and above
    # row - 1 
    if 0 <= row - 1 < len(vert_dots) and 0 <= col  < len(vert_dots[0]):
        action(hor_dots[row][col - 1], grid[row - 1][col])
    # case 4: dot b/w me and below
    # row
    if 0 <= row < len(vert_dots) and 0 <= col < len(vert_dots[0]):
        action(hor_dots[row][col - 1], grid[row + 1][col])
    

def isSafe(grid, row, col, num, hor_dots, vert_dots): # pass in RowBlackWhite (horizontal constraints) matrix
    is_diff = True
    # check alldiff constraint
    def diff_check(value):
        nonlocal is_diff
        if value == num:
            is_diff = False

    row_col_subgrid_traverse(grid, row, col, diff_check)
    if not is_diff: return False
    
    #check dot constraints
    def dot_check(dot, cell):
        nonlocal is_diff
        if dot == 1 and \
            not check_white(cell, num): is_diff = False
        elif dot == 2 and \
            not  check_black(cell, num): is_diff = False
        
    dot_traverse(grid, row, col, hor_dots, vert_dots, dot_check)
    if not is_diff: return False
    return True

def mrv(grid, hor_dots, vert_dots):
    candidates = []
    min_legal_states = float("inf")
    # iterate through every cell in the grid and count how many legal states they have
    for r in range(9):
        for c in range(9):
            legal_states = 0
            if grid[r][c] == 0: # skip assigned cells
                for val in range(1,10):
                    if isSafe(grid, r, c, val, hor_dots, vert_dots): legal_states += 1
                #print("legal_states: " , legal_states)
                if legal_states < min_legal_states:
                    candidates = [(r, c)]
                    min_legal_states = legal_states
                elif legal_states == min_legal_states:
                    candidates.append((r,c))
    print("mrv candidates: ", candidates)
    return candidates


def degree_heuristic(grid, mrv_results, hor_dots, vert_dots):
    candidates = []
    max_degree = -1
    non_loc_degs = 0
    def row_grid_sub_degs(value): 
        nonlocal non_loc_degs
        if value == 0:
            non_loc_degs += 1

    def dot_check(dot, cell):
        nonlocal non_loc_degs
        if dot != 0 and cell == 0: non_loc_degs += 1
        
    for coord in mrv_results:
        degree = 0
        non_loc_degs = 0 
        r, c = coord[0], coord[1]
        row_col_subgrid_traverse(grid, r, c, row_grid_sub_degs)
        degree += row_grid_sub_degs
        
        
       

def count_zeros(matrix):
    count = 0
    for r in range(len(matrix)):
        for c in range(len(matrix[r])):
            if matrix[r][c] == 0:
                count += 1
    return count
    
def select_unassigned_variable(grid, hor_dots, vert_dots):
    return degree_heuristic(grid, mrv(grid, hor_dots, vert_dots), hor_dots, vert_dots)

def solveSudoku(grid, hor_dots, vert_dots):
    
    # grid is complete
    #if g_assigned_cells == 81: return True
    # print(count_zeros(grid))
    
    if count_zeros(grid) == 0: return True
    # if (row == GRID_SIZE - 1 and col == GRID_SIZE):
    #     return True
    
    # Traverse from top to bottom
    # if col == GRID_SIZE:
    #     row += 1
    #     col = 0
    
    #row, col = select_unassigned_variable(grid, hor_dots, vert_dots)
    row, col = mrv(grid, hor_dots, vert_dots)[0]
    print(row, col)

    # num represents all possible values from [1 to 9] one by one
    for num in range(1, GRID_SIZE + 1):

        if isSafe(grid, row, col, num, hor_dots, vert_dots):
            #print("Here 1")
            # not doing MRV, not tracking invalid values
            #coord = degree_heuristic(mrv(grid, hor_dots, vert_dots))
            grid[row][col] = num 
            if solveSudoku(grid, hor_dots, vert_dots):
                return True 
        #print("Done", num)
        grid[row][col] = 0 #undo assignment if backtracking yields no solution

    
        
    return False


def parse_file():
    if (len(sys.argv) != 2): 
        print("Input file required")
        return []
    
    # Initialize empty matrices
    matrix_9x9 = [] # grid
    matrix_9x8 = [] # hor_dots
    matrix_8x9 = [] # vert_dots
    
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
    grid, hor_dots, vert_dots  = parse_file()
    # printing(grid)
    # printing(hor_dots)
    # printing(vert_dots)   

    
    if (solveSudoku(grid, hor_dots, vert_dots)):
        printing(grid)
        print()
    else:
        printing(grid)
        print("no solution  exists ")
        print()
    
main()