#include <iostream>
#include <vector>
#include <tuple>

std::tuple<int, int> convertToTuple(const std::vector<std::vector<int>>& vec2D) {
    std::vector<int> flatData;

    for (const auto& row : vec2D) {
        flatData.insert(flatData.end(), row.begin(), row.end());
    }

    if (flatData.size() >= 2) {
        return std::make_tuple(flatData[0], flatData[1]);
    } else {
        return std::make_tuple(0, 0);  // エラーやデフォルト値の場合の処理
    }
}

int main() {
    std::vector<std::vector<int>> vec2D = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    std::tuple<int, int> tuple = convertToTuple(vec2D);

    int x, y;
    std::tie(x, y) = tuple;

    std::cout << "Tuple: (" << x << ", " << y << ")" << std::endl;

    return 0;
}





