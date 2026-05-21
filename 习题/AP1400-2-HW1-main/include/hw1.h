#ifndef AP_HW1_H
#define AP_HW1_H

#include <vector> // 动态数组
#include <random> // 随机数生成
#include <iomanip> // 输入输出格式控制
#include <iostream> // 输入输出流

using Matrix = std::vector<std::vector<double>>;    // 替代 std::vector<std::vector<double>> 为 Matrix 类型
using namespace std; // 使用 std 命名空间

namespace algebra
{
    Matrix zeros(size_t n, size_t m);    // 生成 n 行 m 列的零矩阵
    Matrix ones(size_t n,size_t m);    // 生成 n 行 m 列的全 1 矩阵
    Matrix random(size_t n,size_t m,double min,double max);    // 生成 n 行 m 列的随机矩阵，元素值在 [min, max] 范围内
    void show(const Matrix& matrix);    // 打印矩阵内容
    Matrix multiply(const Matrix& matrix,double c);    // 矩阵与标量相乘
    Matrix multiply(const Matrix& matrix1,const Matrix& matrix2);    // 矩阵乘矩阵
    Matrix sum(const Matrix& matrix,double c);    // 矩阵与标量相加
    Matrix sum(const Matrix& matrix1,const Matrix& matrix2);    // 矩阵与矩阵相加
    Matrix transpose(const Matrix& matrix);    // 矩阵转置
    Matrix minor(const Matrix& matrix,size_t n,size_t m);    // 计算矩阵的 n 行 m 列的余子式
    double determinant(const Matrix& matrix);    // 计算矩阵的行列式
    Matrix inverse(const Matrix& matrix);    // 计算矩阵的逆矩阵
    Matrix concatenate(const Matrix& matrix1,const Matrix& matrix2,int axis);    // 矩阵拼接
    
    Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2);   // 行交换
    Matrix ero_multiply(const Matrix& matrix, size_t r, double c);   // 行乘以常数
    Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2);   // 行相加
    Matrix upper_triangular(const Matrix& matrix);    // 将矩阵化为上三角矩阵
}

#endif //AP_HW1_H
