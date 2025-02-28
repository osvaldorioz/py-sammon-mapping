#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <Eigen/Dense>
#include <vector>
#include <cmath>

namespace py = pybind11;
using namespace Eigen;

double compute_stress(const MatrixXd& D_high, const MatrixXd& D_low) {
    double stress = 0.0;
    for (int i = 0; i < D_high.rows(); ++i) {
        for (int j = i + 1; j < D_high.cols(); ++j) {
            double high_dist = D_high(i, j);
            double low_dist = D_low(i, j);
            if (high_dist > 0) {
                stress += std::pow((low_dist - high_dist), 2) / high_dist;
            }
        }
    }
    return stress;
}

std::tuple<py::array_t<double>, std::vector<double>> sammon_mapping(py::array_t<double> data, int max_iter, double alpha) {
    py::buffer_info buf = data.request();
    int n = buf.shape[0];
    int dim = buf.shape[1];

    // Convertir los datos de entrada a Eigen
    Eigen::Map<MatrixXd> X(static_cast<double*>(buf.ptr), n, dim);

    // Calcular la matriz de distancia en el espacio original
    MatrixXd D_high(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            if (i == j) {
                D_high(i, j) = 0.0;
            } else {
                D_high(i, j) = D_high(j, i) = (X.row(i) - X.row(j)).norm();
            }
        }
    }

    // Inicializar Y aleatoriamente en 2D
    MatrixXd Y = MatrixXd::Random(n, 2);

    // Lista para almacenar la pérdida
    std::vector<double> loss_history;

    // Optimización de Sammon Mapping
    for (int iter = 0; iter < max_iter; ++iter) {
        // Calcular la matriz de distancia en el espacio reducido
        MatrixXd D_low(n, n);
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                D_low(i, j) = D_low(j, i) = (Y.row(i) - Y.row(j)).norm();
            }
        }

        // Calcular el error de Sammon
        double loss = compute_stress(D_high, D_low);
        loss_history.push_back(loss);

        // Calcular la actualización de Y
        MatrixXd Y_new = Y;
        for (int i = 0; i < n; ++i) {
            Vector2d sum_update = Vector2d::Zero();
            for (int j = 0; j < n; ++j) {
                if (i != j && D_high(i, j) > 0) {
                    double diff = D_low(i, j) - D_high(i, j);
                    sum_update += (diff / D_high(i, j)) * (Y.row(i) - Y.row(j)) / D_low(i, j);
                }
            }
            Y_new.row(i) -= alpha * sum_update.transpose();
        }

        Y = Y_new;
    }

    // Convertir Y a un array de numpy
    py::array_t<double> Y_np({n, 2}, Y.data());

    return std::make_tuple(Y_np, loss_history);
}

PYBIND11_MODULE(sammon_module, m) {
    m.def("sammon_mapping", &sammon_mapping, "Aplica Sammon Mapping a datos de alta dimensión",
          py::arg("data"), py::arg("max_iter") = 100, py::arg("alpha") = 0.3);
}
