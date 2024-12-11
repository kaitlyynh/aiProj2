import sys
# N is the size of the 2D matrix   N*N
N = 9

    

            
# A utility function to print grid
def printing(arr):
    for i in range(len(arr)):
        for j in range(len(arr[i])):
            print(arr[i][j], end = " ")
        print()
    print()

#
def check_white(grid, row, col, num):
    other = grid[row][col]
    if other == 0: return False
    return abs(other - num) == 1
    
def check_black(grid, row, col, num):
    other = grid[row][col]
    if other == 0: return False
    return (other / num) == 0.5 or (other / num) == 2

# verify if white constraint is satisfied horizontally
def check_white_row(grid, row, col, num):
    cell1_val = grid[row][col]
    return abs(cell1_val - num) == 1
    
def check_black_row(grid, row, col, num):
    
    cell1_val = grid[row][col]
    return (cell1_val / num) == 0.5 or (cell1_val / num) == 2

#verify if constraint is satisfied vertically
def check_white_col(grid, row, col, num):
   
    cell1_val = grid[row][col]
    return abs(cell1_val - num) == 1
    
def check_black_col(grid, row, col, num):
    cell1_val = grid[row][col]
    return (cell1_val / num) == 0.5 or (cell1_val / num) == 2

def isSafe(grid, row, col, num, rbw, cbw): # pass in RowBlackWhite (horizontal constraints) matrix
    
    for x in range(9):
        if grid[row][x] == num:
            return False # violated unique row constraint

    for x in range(9):
        if grid[x][col] == num:
            return False # violated unique column constraint

    startRow = row - row % 3
    startCol = col - col % 3
    for i in range(3):
        for j in range(3):
            if grid[i + startRow][j + startCol] == num:
                return False

   
    dot_constraints = True
    # case 1: dot b/w me and left
    # col - 1
    if 0 <= row < len(rbw) and 0 <= col - 1 < len(rbw[0]):
        if rbw[row][col - 1] == 1 and not check_white(grid, row, col - 1, num): return False
        elif rbw[row][col - 1] == 2 and  not  check_black(grid, row, col - 1, num): return False
    # case 2 : dot b/w me and right
    # col 
    if 0 <= row < len(rbw) and 0 <= col < len(rbw[0]):
        if rbw[row][col] == 1 and not check_white(grid, row, col, num): return False
        elif rbw[row][col] == 2 and  not  check_black(grid, row, col, num): return False
    #case 3: dot b/w me and above
    # row - 1 
    if 0 <= row - 1 < len(cbw) and 0 <= col  < len(cbw[0]):
        if cbw[row - 1][col] == 1 and not check_white(grid, row - 1, col, num): return False
        elif cbw[row - 1][col] == 2 and not check_black(grid, row - 1, col, num): return False
    # case 4: dot b/w me and below
    # row
    if 0 <= row < len(cbw) and 0 <= col < len(cbw[0]):
        if cbw[row][col] == 1 and not check_white(grid, row, col, num): return False
        elif cbw[row][col] == 2 and not check_black(grid, row, col, num): return False
    # print(row, " ,", col, " Dot: ", dot_constraints)
    return True


    # if 0 <= row < len(rbw) and 0 <= col - 1 < len(rbw[0]):
    #     if rbw[row][col - 1] == 1: # check if its white
    #         return check_white_row(grid, row, col - 1, num)
    #     elif rbw[row][col - 1] == 2: # check if its black
    #         return check_black_row(grid, row, col - 1, num)


    # if 0 <= row - 1 < len(cbw) and 0 <= col < len(cbw[0]):
    #     if cbw[row - 1][col] == 1:
    #         return check_white_col(grid, row - 1, col, num)
    #     elif cbw[row - 1][col] == 2:
    #         return check_black_col(grid, row - 1, col, num)




def mrv(grid, rbw, cbw):
    candidates = []
    min_legal_states = float("inf")
    # iterate through every cell in the grid and count how many legal states they have
    for r in range(9):
        for c in range(9):
            legal_states = 0
            if grid[r][c] == 0: # skip assigned cells
                for val in range(1,10):
                    if isSafe(grid, r, c, val, rbw, cbw): legal_states += 1
                #print("legal_states: " , legal_states)
                if legal_states < min_legal_states:
                    candidates = [(r, c)]
                    min_legal_states = legal_states
                elif legal_states == min_legal_states:
                    candidates.append((r,c))
    print("mrv candidates: ", candidates)
    return candidates


def degree_heuristic(mrv_results, rbw, cbw):
    candidates = []
    max_degree = -1
    for coord in mrv_results:
        degree = 0
        
        r, c = coord[0], coord[1]
        #print(r, c)
        # horizontal dot constraints              
        if 0 <= c - 1 < len(rbw[0]) and 0 <= r < len(rbw):
            if rbw[r][c - 1] != 0: degree += 1 
        if 0 <= c + 1 < len(rbw[0]) and 0 <= r < len(rbw):
            if  rbw[r][c] != 0: degree += 1
        # vertical dot constraints
        if 0 <= c < len(cbw[0]) and 0 <= r - 1 < len(cbw) :
            if cbw[r - 1][c] != 0 : degree += 1 
        if 0 <= c  < len(cbw[0]) and 0 <= r < len(cbw):
            if cbw[r][c] != 0 : degree += 1
        #print("degree: " ,degree)
        if degree > max_degree:
            candidates = [(r, c)]
            max_degree = degree
        elif degree == max_degree:
            candidates.append((r,c))
    print("dh candidates: ", candidates)
    return candidates[0]

def count_zeros(matrix):
    count = 0
    for r in range(len(matrix)):
        for c in range(len(matrix[r])):
            if matrix[r][c] == 0:
                count += 1
    return count
    
def select_unassigned_variable(grid, rbw, cbw):
    return degree_heuristic(mrv(grid, rbw, cbw), rbw, cbw)

def solveSudoku(grid, rbw, cbw):
    
    # grid is complete
    #if g_assigned_cells == 81: return True
    # print(count_zeros(grid))
    
    if count_zeros(grid) == 0: return True
    # if (row == N - 1 and col == N):
    #     return True
    
    # Traverse from top to bottom
    # if col == N:
    #     row += 1
    #     col = 0
    
    row, col = select_unassigned_variable(grid, rbw, cbw)
    
    print(row, col)

    # num represents all possible values from [1 to 9] one by one
    for num in range(1, N + 1):

        if isSafe(grid, row, col, num, rbw, cbw):
            #print("Here 1")
            # not doing MRV, not tracking invalid values
            #coord = degree_heuristic(mrv(grid, rbw, cbw))
            grid[row][col] = num 
            if solveSudoku(grid, rbw, cbw):
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
    matrix_9x8 = [] # rbw
    matrix_8x9 = [] # cbw
    
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
    grid, rbw, cbw  = parse_file()
    # printing(grid)
    # printing(rbw)
    # printing(cbw)   

    
    if (solveSudoku(grid, rbw, cbw)):
        printing(grid)
        print()
    else:
        printing(grid)
        print("no solution  exists ")
        print()
    
main()
