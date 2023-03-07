#include <iostream> // Для ввода-вывода на консоль
#include <cmath> // Для математических операций, например sqrt(), sin() и т.д.
#include <vector> // Для работы с векторами
#include <string> // Для работы со строками
#include <algorithm> // Для работы со стандартными алгоритмами, например sort() и т.д.
#include <fstream> // Для работы с файлами
#include <chrono> // Для работы с временем


int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " max_num_iter max_toch raz\n";
        return 1;
    }

    const int max_num_iter = std::atoi(argv[1]);
    const double max_toch = std::atof(argv[2]);
    const int raz = std::atoi(argv[3]);

    auto arr_pred = std::vector<std::vector<double>>(raz, std::vector<double>(raz, 0.0));
    auto arr_new = std::vector<std::vector<double>>(raz, std::vector<double>(raz, 0.0));

    arr_pred[0][0] = 10;
    arr_pred[0][raz - 1] = 20;
    arr_pred[raz - 1][raz - 1] = 20;
    arr_pred[raz - 1][0] = 30;

    const auto start_time = std::chrono::high_resolution_clock::now();

    double error = 0.0;
    int num_iter = 0;

    while (max_num_iter > num_iter && max_toch < error) {
        error = 0.0;

#pragma omp parallel for reduction(max : error)
        for (int j = 1; j < raz - 1; ++j) {
            for (int i = 1; i < raz - 1; ++i) {
                arr_new[i][j] = (arr_pred[i - 1][j] + arr_pred[i][j - 1] + arr_pred[i][j + 1] + arr_pred[i + 1][j]) * 0.25;
                error = std::max(std::fabs(arr_pred[i][j] - arr_new[i][j]), error);
            }
        }

#pragma omp parallel for
        for (int j = 1; j < raz - 1; ++j) {
            for (int i = 1; i < raz - 1; ++i) {
                arr_pred[j][i] = arr_new[j][i];
            }
        }

        if (num_iter % 10 == 0) {
            std::printf("Номер итерации: %d, ошибка: %0.8lf\n", num_iter, error);
        }

        ++num_iter;
    }

    const auto end_time = std::chrono::high_resolution_clock::now();
    const auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::printf("Elapsed time: %ld ms\n", elapsed_time);

    return 0;
}
