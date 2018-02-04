/**
 * @file SparseMatrix.tpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

template <typename T>
OSQPWrapper::SparseMatrix::SparseMatrix(const Eigen::SparseMatrix<T> &matrix)
{
    m_matrix = nullptr;
    this->operator<<(matrix);
}

template<typename T>
void OSQPWrapper::SparseMatrix::operator<<(const Eigen::SparseMatrix<T> &matrix)
{
    // clear all data if m_matrix was already set
    if(m_matrix != nullptr){
        c_free(m_matrix);
        m_innerIndex.clear();
        m_values.clear();
        m_outerIndex.clear();
    }

    // get number of row, columns and nonZeros from Eigen SparseMatrix
    c_int rows = matrix.rows();
    c_int cols = matrix.cols();
    c_int numberOfNonZeroCoeff = matrix.nonZeros();

    // get innerr and outer index
    const int* innerIndexPtr = matrix.innerIndexPtr();
    const int* outerIndexPtr = matrix.outerIndexPtr();

    // get nonzero values
    const T* valuePtr = matrix.valuePtr();

    // cast the elements.
    // OSQP uses c_int and c_float while eigen uses int and double.
    for(int i = 0; i < numberOfNonZeroCoeff; i++) {
        double value = valuePtr[i];
        int index = innerIndexPtr[i];
        m_innerIndex.push_back((c_int)index);
        m_values.push_back((c_float)value);
    }

    for(int i = 0; i < cols + 1; i++) {
        int index = outerIndexPtr[i];
        m_outerIndex.push_back((c_int)index);
    }

    // instantiate csc matrix
    m_matrix = csc_matrix(rows, cols, numberOfNonZeroCoeff, m_values.data(),
                          m_innerIndex.data(), m_outerIndex.data());
}


template <typename T>
std::vector<Eigen::Triplet<T>> OSQPWrapper::SparseMatrix::toTriplets()
{
    std::vector<Eigen::Triplet<T>> triplets;

    // if the matrix is not instantiate the triplets vector is empty
    if(m_matrix == nullptr)
        return triplets;

    // get row and column data
    c_int* innerIndexPtr = m_matrix->i;
    c_int* outerIndexPtr = m_matrix->p;

    // get values data
    double* valuePtr = m_matrix->x;
    c_int numberOfNonZeroCoeff =  m_matrix->nzmax;

    // populate the tripletes vector
    int column=0;
    int row;
    double value;
    for(int i = 0; i<numberOfNonZeroCoeff; i++) {
        row = innerIndexPtr[i];
        value = valuePtr[i];

        while(i >= outerIndexPtr[column+1])
            column++;

        triplets.push_back(Eigen::Triplet<T>(row, column, value));
    }
    return triplets;
}

template <typename T>
Eigen::SparseMatrix<T> OSQPWrapper::SparseMatrix::toEigen()
{
    Eigen::SparseMatrix<T> matrix;

    // if the matrix is not instantiate the eigen matrix is empty
    if(m_matrix == nullptr) {
        matrix.resize(0,0);
        return matrix;
    }

    // get the number of rows and columns
    int rows = m_matrix->m;
    int cols = m_matrix->n;

    // get the triplets from the csc matrix
    std::vector<Eigen::Triplet<T>> tripletList = toTriplets<T>();

    // resize the eigen matrix
    matrix.resize(rows, cols);
    matrix.data().squeeze();

    // set the eigen matrix from triplets
    matrix.setFromTriplets(tripletList.begin(), tripletList.end());

    return matrix;
}
