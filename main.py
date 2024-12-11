# N is the size of the 2D matrix   N*N
N = 9

# A utility function to print grid
def printing(arr):
    for i in range(N):
        for j in range(N):
            print(arr[i][j], end = " ")
        print()

# verify if white constraint is satisfied horizontally
def check_white_row(grid, row, col, num):
    cell1_val = grid[row][col - 1]
    return abs(cell1_val - num) == 1
    
def check_black_row(grid, row, col, num):
    cell1_val = grid[row][col - 1]
    return (cell1_val / num) == 0.5 or (cell1_val / num) == 2

#verify if constraint is satisfied vertically
def check_white_col(grid, row, col, num):
    cell1_val = grid[row - 1][col]
    return abs(cell1_val - num) == 1
    
def check_black_col(grid, row, col, num):
    cell1_val = grid[row - 1][col]
    # print((cell1_val, num) if (cell1_val / num) == 0.5 or (cell1_val / num) == 2 else "false")
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

    # implement checking if a white constraint is satisfied on a row
    # if col is > 0 (within bounds)
        # if there is a white constraint
            # if white constraint is violated
                # ret False

    if col > 0:
        if rbw[row][col - 1] == 1: # check if its white
            if not check_white_row(grid, row, col, num):
                return False
        elif rbw[row][col - 1] == 2: # check if its black
            if not check_black_row(grid, row, col, num):
                return False

    # if row > 0:
    #     if cbw[row - 1][col] == 1:
    #         if not check_white_col(grid, row, col, num):
    #             return False
    #     elif cbw[row - 1][col] == 2:
    #         if not check_black_col(grid, row, col, num):
    #             return False
    
    return True


def solveSudoku(grid, row, col, rbw, cbw):
  
    # grid is complete
    if (row == N - 1 and col == N):
        return True

    # Traverse from top to bottom
    if col == N:
        row += 1
        col = 0

    if grid[row][col] > 0:
        return solveSudoku(grid, row, col + 1, rbw, cbw) # Traverse from left to right

    # num represents all possible values from [1 to 9] one by one
    for num in range(1, N + 1, 1):
      
        if isSafe(grid, row, col, num, rbw, cbw):
            # not doing MRV, not tracking invalid values
            grid[row][col] = num 

            if solveSudoku(grid, row, col + 1, rbw, cbw):
                return True

        grid[row][col] = 0
    return False

# horizontal constraints (9 x 8)
rbw = [ [0, 0 , 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0],
    ]
    
# vertical contraints (8 x 9)
cbw = [ [1, 2 , 0, 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 0, 0, 0],
        [0, 0 , 0, 0, 0, 0, 3, 2, 1],
    ]
# Driver Code

# 0 means unassigned cells
grid = [[1, 2, 3, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0]]

if (solveSudoku(grid, 0, 0, rbw, cbw)):
    printing(grid)
    print()
else:
    print("no solution  exists ")
    print()
