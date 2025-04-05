#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

//int dx[] = {-1, 1, 0, 0};  // UP, DOWN, LEFT, RIGHT
//int dy[] = {0, 0, -1, 1};

bool dfs(vector<vector<char>>& maze, vector<vector<bool>>& visited, vector<pair<int, int>>& path, int x, int y, int rows, int cols) {
    if (x < 0 || x >= rows || y < 0 || y >= cols || maze[x][y] == '#' || visited[x][y])
        return false;

    path.push_back({x, y});
    visited[x][y] = true;

    if (maze[x][y] == 'G')
        return true;

    int nextX = -1, nextY = -1;

    //up
    if (x > 0 && maze[x - 1][y] != '#' && !visited[x - 1][y]) {
        nextX = x - 1;
        nextY = y;
    }
    //down
    else if (x < rows - 1 && maze[x + 1][y] != '#' && !visited[x + 1][y]) {
        nextX = x + 1;
        nextY = y;
    }
    //left
    else if (y > 0 && maze[x][y - 1] != '#' && !visited[x][y - 1]) {
        nextX = x;
        nextY = y - 1;
    }
    //right
    else if (y < cols - 1 && maze[x][y + 1] != '#' && !visited[x][y + 1]) {
        nextX = x;
        nextY = y + 1;
    }

    if (nextX != -1 && nextY != -1) {
        return dfs(maze, visited, path, nextX, nextY, rows, cols);
    }

    return false;
}

void displayMaze(const vector<vector<char>>& maze) {
    for (const auto& row : maze) {
        for (char cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
    cout << endl;
}

pair<int, int> findStart(const vector<vector<char>>& maze, int rows, int cols) {
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            if(maze[i][j] == 'S')
                return {i, j};

    return{-1, -1};
}

vector<vector<char>> readMazeFromFile(const string& filename, int& rows, int& cols) {
    ifstream file(filename);
    if (!file) {
        cout << "Error opening file: " << filename << endl;
        exit(1);
    }

    file >> rows >> cols;

    vector<vector<char>> maze(rows, vector<char>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            file >> maze[i][j];
        }
    }

    file.close();
    return maze;
}

void markPath(vector<vector<char>>& maze, const vector<pair<int, int>>& path) {
    for (auto p : path) {
        if (maze[p.first][p.second] != 'S' && maze[p.first][p.second] != 'G') {
            maze[p.first][p.second] = '*';
        }
    }
}

int main() {
    srand(time(0));

    int size;
    cout << "Choose maze size (5 for 5x5, 6 for 6x6, 7 for 7x7): ";
    cin >> size;

    int randomMaze = rand() % 3 + 1;
    string filename = "size" + to_string(size) +"/size" + to_string(size) + "_"+ to_string(randomMaze) + ".txt";

    int rows, cols;
    vector<vector<char>> maze = readMazeFromFile(filename, rows, cols);

    cout << "\nSelected Maze File: " << filename << "\n";
    cout << "Original Maze:\n";
    displayMaze(maze);

    pair<int, int> start = findStart(maze, rows, cols);
    if(start.first == -1) {
        cout << "No starting position found!\n";
        return 1;
    }

    cout << "Start position found at (" << start.first << ", " << start.second << ")\n";

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<pair<int, int>> path;

    bool foundPath = dfs(maze, visited, path, start.first, start.second, rows, cols);

    cout << "Path attempted by DFS: Start -> ";
    for(auto p : path)
        cout << "(" << p.first << ", " << p.second << ") -> ";
    cout << (foundPath ? "Goal\n" : "Dead End\n");

    markPath(maze, path);
    cout << "Maze with attempted path:\n";
    displayMaze(maze);

    return 0;
}
