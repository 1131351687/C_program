#include "hw1.h"

namespace algebra
{

Matrix zeros(size_t n, size_t m)
{
    return Matrix(n, vector<double>(m, 0.0));
}

Matrix ones(size_t n, size_t m)
{
    return Matrix(n, vector<double>(m, 1.0));
}

Matrix random(size_t n, size_t m, double min, double max)
{
    if (min > max)
        throw logic_error("min cannot be greater than max");

    random_device rd;  // 获取随机数种子
    mt19937 gen(rd()); // 使用 Mersenne Twister 算法生成随机数

    uniform_real_distribution<double> dist(min, max); // 定义随机数分布范围
    Matrix mat;
    for (size_t i = 0; i < n; i++)
    {
        vector<double> row;
        for (size_t j = 0; j < m; j++)
        {
            row.push_back(dist(gen)); // 生成随机数并添加到行中
        }
        mat.push_back(row); // 将行添加到矩阵中
    }
    return mat;
}

void show(const Matrix &matrix)
{
    if (matrix.empty())
    {
        cout << "[]" << endl; // 如果矩阵为空，打印空矩阵
        return;
    }
    cout << fixed << setprecision(3); // 设置输出格式为固定小数点，保留三位小数
    for (size_t i = 0; i < matrix.size(); i++)
    {
        for (size_t j = 0; j < matrix[i].size(); j++)
        {
            cout << setw(8) << matrix[i][j]; // 输出矩阵元素，设置宽度为8
        }
        cout << endl; // 每行结束后换行
    }
}

Matrix multiply(const Matrix &matrix, double c)
{
    Matrix mat;
    for (size_t i = 0; i < matrix.size(); i++)
    {
        vector<double> row;
        for (size_t j = 0; j < matrix[i].size(); j++)
        {
            row.push_back(matrix[i][j] * c); // 将矩阵元素与标量相乘并添加到行中
        }
        mat.push_back(row); // 将行添加到矩阵中
    }
    return mat;
}

Matrix multiply(const Matrix &matrix1, const Matrix &matrix2)
{
    if (matrix1.empty() && matrix2.empty())
        return {};
    if (matrix1.empty() || matrix2.empty())
        throw logic_error("invalid matrix dimensions");
    if (matrix1[0].size() != matrix2.size())
        throw logic_error("invalid matrix dimensions");

    Matrix mat(matrix1.size(), vector<double>(matrix2[0].size(), 0.0));
    for (size_t i = 0; i < matrix1.size(); i++)
    {
        for (size_t j = 0; j < matrix2[0].size(); j++)
        {
            for (size_t k = 0; k < matrix2.size(); k++)
            {
                mat[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return mat;
}

Matrix sum(const Matrix &matrix, double c)
{
    Matrix mat;
    for (size_t i = 0; i < matrix.size(); i++)
    {
        vector<double> row;
        for (size_t j = 0; j < matrix[i].size(); j++)
        {
            row.push_back(matrix[i][j] + c); // 将矩阵元素与标量相加并添加到行中
        }
        mat.push_back(row); // 将行添加到矩阵中
    }
    return mat;
}

Matrix sum(const Matrix &matrix1, const Matrix &matrix2)
{
    if (matrix1.empty() && matrix2.empty())
        return {};
    if (matrix1.empty() || matrix2.empty())
        throw logic_error("invalid matrix dimensions");
    if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size())
        throw logic_error("invalid matrix dimensions");

    Matrix mat;
    for (size_t i = 0; i < matrix1.size(); i++)
    {
        vector<double> row;
        for (size_t j = 0; j < matrix1[i].size(); j++)
        {
            row.push_back(matrix1[i][j] + matrix2[i][j]); // 将两个矩阵的对应元素相加并添加到行中
        }
        mat.push_back(row); // 将行添加到矩阵中
    }
    return mat;
}

Matrix transpose(const Matrix &matrix)
{
    if (matrix.empty())
        return {}; // 如果矩阵为空，返回空矩阵
    Matrix mat;
    for (size_t i = 0; i < matrix[0].size(); i++)
    {
        vector<double> row;
        for (size_t j = 0; j < matrix.size(); j++)
        {
            row.push_back(matrix[j][i]); // 将矩阵的行列互换并添加到行中
        }
        mat.push_back(row); // 将行添加到矩阵中
    }
    return mat;
}

Matrix minor(const Matrix &matrix, size_t n, size_t m)
{
    Matrix mat;
    for (size_t i = 0; i < matrix.size(); i++)
    {
        if (i == n)
            continue; // 跳过第 n 行
        vector<double> row;
        for (size_t j = 0; j < matrix[i].size(); j++)
        {
            if (j == m)
                continue;                // 跳过第 m 列
            row.push_back(matrix[i][j]); // 将矩阵元素添加到行中
        }
        mat.push_back(row); // 将行添加到矩阵中
    }
    return mat;
}

double determinant(const Matrix &matrix)
{
    if (matrix.empty())
        return 1;
    if (matrix.size() != matrix[0].size())
        throw logic_error("matrix must be square");
    if (matrix.size() == 1)
        return matrix[0][0]; // 1x1 矩阵的行列式等于其唯一元素

    double det = 0;
    for (size_t j = 0; j < matrix[0].size(); j++)
    {
        det += (j % 2 == 0 ? 1 : -1) * matrix[0][j] * determinant(minor(matrix, 0, j)); // 使用递归计算行列式，按照第一行展开
    }
    return det;
}

Matrix inverse(const Matrix &matrix)
{
    if (matrix.empty())
        return {};
    if (matrix.size() != matrix[0].size())
        throw logic_error("matrix must be square");

    double det = determinant(matrix);
    if (det == 0)
        throw logic_error("matrix is singular");

    if (matrix.size() == 1)
        return Matrix{{1.0 / det}};

    Matrix mat;
    for (size_t i = 0; i < matrix.size(); i++)
    {
        vector<double> row;
        for (size_t j = 0; j < matrix[i].size(); j++)
        {
            row.push_back(
                ((i + j) % 2 == 0 ? 1 : -1) * determinant(minor(matrix, j, i)) / det); // 将矩阵元素添加到行中
        }
        mat.push_back(row); // 将行添加到矩阵中
    }
    return mat;
}

Matrix concatenate(const Matrix &matrix1, const Matrix &matrix2, int axis)
{
    // 空矩阵检查
    if (matrix1.empty() && matrix2.empty())
        return {};
    if (matrix1.empty())
        return matrix2;
    if (matrix2.empty())
        return matrix1;

    Matrix mat = matrix1;
    if (axis == 0)
    {
        if (matrix1[0].size() != matrix2[0].size())
            throw logic_error("invalid matrix dimensions");

        for (size_t i = 0; i < matrix2.size(); i++)
        {
            mat.push_back(matrix2[i]); // 将 matrix2 的行添加到 matrix1 中
        }
    }
    else
    {
        if (matrix1.size() != matrix2.size())
            throw logic_error("invalid matrix dimensions");

        for (size_t i = 0; i < matrix2.size(); i++)
        {
            for (size_t j = 0; j < matrix2[0].size(); j++)
            {
                mat[i].push_back(matrix2[i][j]); // 将 matrix2 的列添加到 matrix1 中
            }
        }
    }
    return mat;
}

Matrix ero_swap(const Matrix &matrix, size_t r1, size_t r2)
{
    if (matrix.empty() || r1 >= matrix.size() || r2 >= matrix.size())
        throw logic_error("invalid row index");

    Matrix mat = matrix;
    swap(mat[r1], mat[r2]); // 交换矩阵的两行
    return mat;
}
Matrix ero_multiply(const Matrix &matrix, size_t r, double c)
{
    if (matrix.empty() || r >= matrix.size())
        throw logic_error("invalid row index");

    Matrix mat = matrix;
    for (auto &x : mat[r])
        x *= c; // 将矩阵的第 r 行乘以常数 c
    return mat;
}
Matrix ero_sum(const Matrix &matrix, size_t r1, double c, size_t r2)
{
    if (matrix.empty() || r1 >= matrix.size() || r2 >= matrix.size())
        throw logic_error("invalid row index");

    Matrix mat = matrix;
    for (size_t j = 0; j < mat[r1].size(); j++)
    {
        mat[r2][j] += c * mat[r1][j]; // 将矩阵的第 r1 行乘以常数 c 后加到第 r2 行
    }
    return mat;
}

Matrix upper_triangular(const Matrix& matrix)
{
    if (matrix.empty())
        return {}; // 如果矩阵为空，返回空矩阵
    if (matrix.size() != matrix[0].size())
        throw logic_error("matrix must be square");

    Matrix mat = matrix;
    size_t row = matrix.size();
    size_t col = matrix[0].size();
    for (size_t i = 0; i < row; i++)
    {
        if (mat[i][i] == 0) // 如果主元为零，尝试交换行以找到非零主元
        {
            for (size_t j = i + 1; j < row; j++)
            {
                if (mat[j][i] != 0)
                {
                    swap(mat[i], mat[j]); // 交换行
                    break;
                }
            }
        }
        double pivot=mat[i][i]; // 获取主元
        for (size_t j = i + 1; j < row; j++)
        {
            if (mat[j][i]!=0)
            {
                double factor=mat[j][i]/pivot; // 计算消元因子
                mat=ero_sum(mat,i,-factor,j); // 将第 i 行乘以 -factor 后加到第 j 行
            }
        }
    }
    return mat;

}

} // namespace algebra