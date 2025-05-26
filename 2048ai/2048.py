import time
import numpy as np
import os
import random

score = 0
key_up = 'w'
key_down = 's'
key_left = 'a'
key_right = 'd'
moves = [key_up, key_down, key_left, key_right]
INF = 10000000
num_monotone = random.uniform(1, 10000)
num_smooth = random.uniform(1, 10000)
num_free = random.randint(1, 10000)

# num_monotone = 1.5
# num_smooth = num_monotone
# num_free = 9000


def initial():
    init = [[0 for i in range(4)] for i in range(4)]
    num = [2, 2, 2, 2, 2, 2, 2, 2, 2, 4]
    point1 = np.random.randint(0, 4, 3)
    point2 = point1
    while point2[0] == point1[0] and point2[1] == point1[1]:
        point2 = np.random.randint(0, 4, 3)
    point1[2] = num[point1[2] % 10]
    point2[2] = num[point2[2] % 10]
    init[int(point1[0])][int(point1[1])] = point1[2]
    init[int(point2[0])][int(point2[1])] = point2[2]
    return init


baseBoard = initial()


def add(baseBoard):
    number = 0
    cnt = 0
    num = [2, 2, 2, 2, 2, 2, 2, 2, 2, 4]
    for i in range(4):
        for j in range(4):
            if baseBoard[i][j] == 0:
                number += 1
    if number >= 1:
        number = 1
    while True:
        t = np.random.randint(0, 15, 3)
        if baseBoard[t[0] // 4][t[1] % 4] == 0:
            baseBoard[t[0] // 4][t[1] % 4] = num[t[2] % 10]
            cnt += 1
        if cnt == number:
            break
    return baseBoard


def rotate(baseBoard):
    matrix = [[baseBoard[i][j] for i in range(4)] for j in reversed(range(4))]
    return matrix


def calculate(baseBoard):
    global score
    flag = False
    for a in baseBoard:
        b = []
        last = 0
        for v in a:
            if v != 0:
                if v == last:
                    s = b.pop() * 2
                    b.append(s)
                    score += s
                    last = 0
                else:
                    b.append(v)
                    last = v
        b += [0] * (4 - len(b))
        for i in range(4):
            if a[i] != b[i]:
                flag = True
        a[:] = b
    return baseBoard, flag


def operate(dir):
    global baseBoard
    if dir == 'w':
        baseBoard = rotate(baseBoard)
        if calculate(baseBoard)[1]:
            baseBoard = add(baseBoard)
        baseBoard = rotate(baseBoard)
        baseBoard = rotate(baseBoard)
        baseBoard = rotate(baseBoard)
    if dir == 'd':
        baseBoard = rotate(baseBoard)
        baseBoard = rotate(baseBoard)
        if calculate(baseBoard)[1]:
            baseBoard = add(baseBoard)
        baseBoard = rotate(baseBoard)
        baseBoard = rotate(baseBoard)
    if dir == 's':
        baseBoard = rotate(baseBoard)
        baseBoard = rotate(baseBoard)
        baseBoard = rotate(baseBoard)
        if calculate(baseBoard)[1]:
            baseBoard = add(baseBoard)
        baseBoard = rotate(baseBoard)
    if dir == 'a':
        if calculate(baseBoard)[1]:
            baseBoard = add(baseBoard)


def gameOver(baseBoard):
    for r in range(4):
        for c in range(4):
            if baseBoard[r][c] == 0:
                return False
    for r in range(4):
        for c in range(4 - 1):
            if baseBoard[r][c] == baseBoard[r][c + 1]:
                return False
    for r in range(4 - 1):
        for c in range(4):
            if baseBoard[r][c] == baseBoard[r + 1][c]:
                return False
    return True


def show(M):
    print("\n" + "使用W A S D控制方向", end="")
    print("\n" + " " * 25 + "当前分数: " + "{:}".format(score), end="")
    for i in range(4):
        print("\n")
        for j in range(4):
            print("{: >6}".format(M[i][j]), end="")
    print("\n")


class AI(object):

    def rotateLeft(self, grid):
        matrix = [[grid[i][j] for i in reversed(range(4))] for j in range(4)]
        return matrix

    def state_board(self, grid, i, j):
        return grid[i][j]

    def move(self, grid, direction):
        flag = 0
        out = []
        matrix = grid
        if direction == key_up:
            rotation = 1
        elif direction == key_right:
            rotation = 2
        elif direction == key_down:
            rotation = 3
        else:
            rotation = 0
        for i in range(4 - rotation):
            grid = self.rotateLeft(grid)
        for a in grid:
            b = []
            last = 0
            for v in a:
                if v != 0:
                    if v == last:
                        s = b.pop() * 2
                        b.append(s)
                        last = 0
                    else:
                        b.append(v)
                        last = v
            b += [0] * (4 - len(b))
            out.append(b)
        for i in range(rotation):
            out = self.rotateLeft(out)
        for i in range(4):
            for j in range(4):
                if matrix[i][j] != out[i][j]:
                    flag = 1
                    break
        return out, flag

    def eval_monotone(self, grid):
        L = R = U = D = 0
        LR = UD = 0
        for x in range(4):
            m = 0
            for y in range(3):
                if grid[x][y] and grid[x][y] >= grid[x][y + 1]:
                    m += 1
                    L += m**2 * 4
                else:
                    L -= abs((grid[x][y]) - (grid[x][y + 1])) * 1.5
                    m = 0
            n = 0
            for y in range(3):
                if grid[x][y] <= grid[x][y + 1] and grid[x][y + 1]:
                    n += 1
                    R += n**2 * 4
                else:
                    R -= abs((grid[x][y]) - (grid[x][y + 1])) * 1.5
                    n = 0
        LR = max(L, R)
        for y in range(4):
            r = 0
            for x in range(3):
                if grid[x][y] and grid[x][y] >= grid[x + 1][y]:
                    r += 1
                    U += r**2 * 4
                else:
                    U -= abs((grid[x][y]) - (grid[x + 1][y])) * 1.5
                    r = 0
            s = 0
            for x in range(3):
                if grid[x][y] <= grid[x + 1][y] and grid[x + 1][y]:
                    s += 1
                    D += s**2 * 4
                else:
                    D -= abs((grid[x][y]) - (grid[x + 1][y])) * 1.5
                    s = 0
        UD = max(U, D)
        return LR + UD

    def eval_smoothness(self, grid):
        score_smooth = 0
        for x in range(4):
            for y in range(3):
                score_smooth -= abs((grid[x][y] or 2) / (grid[x][y + 1] or 2))
                score_smooth -= abs((grid[y][x] or 2) / (grid[y + 1][x] or 2))
                score_smooth -= abs((grid[x][y + 1] or 2) / (grid[x][y] or 2))
                score_smooth -= abs((grid[y + 1][x] or 2) / (grid[y][x] or 2))
        return score_smooth

    def eval_free(self, grid):
        free = grid[0].count(0) + grid[1].count(0) + grid[2].count(
            0) + grid[3].count(0)
        return free

    def eval(self, grid):
        score_monotone = self.eval_monotone(grid) * num_monotone
        score_smooth = self.eval_smoothness(grid) * num_smooth
        score_free = self.eval_free(grid) * num_free
        score = 0
        score += score_smooth
        score += score_monotone
        score += score_free
        return score

    def minimax_alpha_beta(self, grid, depth, alpha, beta, turn):
        if depth == 0:
            return self.eval(grid)
        if turn == 0:
            max_eval = -INF
            for move in moves:
                new_grid, moved = self.move(grid, move)
                if moved:
                    evaluate = self.minimax_alpha_beta(new_grid, depth - 1,
                                                       alpha, beta, 1)
                    max_eval = max(max_eval, evaluate)
                    alpha = max(alpha, max_eval)
                    if beta < alpha:
                        break
            return max_eval
        else:
            min_eval = INF
            empty_cells = [(i, j) for i in range(4) for j in range(4)
                           if grid[i][j] == 0]
            for i, j in empty_cells:
                for tile in [2, 4]:
                    new_grid = [row[:] for row in grid]
                    new_grid[i][j] = tile
                    eval = self.minimax_alpha_beta(new_grid, depth - 1, alpha,
                                                   beta, 0)
                    min_eval = min(min_eval, eval)
                    beta = min(beta, eval)
                    if beta < alpha:
                        break
            return min_eval

    def expectimax(self, grid, depth):
        if depth == 0:
            return self.eval(grid)
        max_value = -INF
        for move in moves:
            new_grid, moved = self.move(grid, move)
            if moved:
                max_value = max(max_value,
                                self.expectimax(new_grid, depth - 1))
        empty_cells = [(i, j) for i in range(4) for j in range(4)
                       if grid[i][j] == 0]
        total_value = 0
        total_prob = 0
        prob = 0
        for i, j in empty_cells:
            new_grid_2 = [row[:] for row in grid]
            new_grid_2[i][j] = 2
            new_grid_4 = [row[:] for row in grid]
            new_grid_4[i][j] = 4
            if self.state_board(grid, i, j):
                prob = 0.9
            total_prob += prob
            total_value = prob * (
                0.9 * self.expectimax(new_grid_2, depth - 1) +
                0.1 * self.expectimax(new_grid_4, depth - 1))

        if total_prob == 0:
            return max_value
        else:
            return total_value / total_prob

    def getNextMove(self, grid, depth):
        best_move = None
        best_score = -INF
        alpha = -INF
        beta = INF
        for move in moves:
            new_grid, moved = self.move(grid, move)
            if moved:
                # score = self.minimax_alpha_beta(new_grid, depth, alpha, beta,
                #                                 0)
                # if score > best_score:
                #     best_score = score
                #     best_move = move
                # alpha = max(alpha, score)
                score = self.expectimax(new_grid, depth)
                if score > best_score:
                    best_score = score
                    best_move = move
        return best_move


def main():
    show(baseBoard)
    # play_choice = input('请输入你想要的游戏模式:\nA:AI进行\nU:自己进行\n')
    # if play_choice == 'U':
    #     while not gameOver(baseBoard):
    #         op = input()
    #         operate(op)
    #         show(baseBoard)
    #     maxvalue = 0
    #     for i in range(4):
    #         for j in range(4):
    #             maxvalue = max(maxvalue, baseBoard[i][j])
    #     if maxvalue < 2048:
    #         print('游戏结束！你失败了！你的分数是:' + '{:}'.format(score))
    #         print('\n')
    #     else:
    #         print('游戏结束！你成功了！你的分数是:' + '{:}'.format(score))
    #         print('\n')
    # elif play_choice == 'A':
    auto = AI()
    while not gameOver(baseBoard):
        best_choice = None
        best_choice = auto.getNextMove(baseBoard, 3)
        if best_choice is None:
            break
        # print('当前走的方向是:' + '{:}'.format(best_choice))
        operate(best_choice)
        show(baseBoard)
    maxvalue = 0
    for i in range(4):
        for j in range(4):
            maxvalue = max(maxvalue, baseBoard[i][j])
    if maxvalue < 2048:
        print('游戏结束！你失败了！你的分数是:' + '{:}'.format(score))
        print('\n')
        print('最大方块是:' + '{:}'.format(maxvalue))
    else:
        print('游戏结束！你成功了！你的分数是:' + '{:}'.format(score))
        print('\n')
        print('最大方块是:' + '{:}'.format(maxvalue))
    print('单调:' + '{:}'.format(num_monotone) + '\n平滑:' +
          '{:}'.format(num_smooth) + '\n空白:' + '{:}'.format(num_free))


if __name__ == '__main__':
    main()
