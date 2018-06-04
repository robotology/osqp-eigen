/**
 * @file SparseMatrixHelper.tpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

template<typename T>
bool OSQPWrapper::SparseMatrixHelper::createOsqpSparseMatrix(const Eigen::SparseMatrix<T> &eigenSparseMatrix,
                                                             csc*& osqpSparseMatrix)

{
    // get number of row, columns and nonZeros from Eigen SparseMatrix
    c_int rows = eigenSparseMatrix.rows();
    c_int cols = eigenSparseMatrix.cols();
    c_int numberOfNonZeroCoeff = eigenSparseMatrix.nonZeros();

    // get innerr and outer index
    const int* innerIndexPtr = eigenSparseMatrix.innerIndexPtr();
    const int* outerIndexPtr = eigenSparseMatrix.outerIndexPtr();

    // get nonzero values
    const T* valuePtr = eigenSparseMatrix.valuePtr();

    // cast the elements.
    // OSQP uses c_int and c_float while eigen uses int and double.
    std::vector<c_int> innerIndex(numberOfNonZeroCoeff);
    std::vector<c_int> outerIndex(cols + 1);
    std::vector<c_float> values(numberOfNonZeroCoeff);

    int innerOsqpPosition = 0;
    for(int k = 0; k < cols; k++) {
        outerIndex[k] = static_cast<c_int>(outerIndexPtr[k]);
        for (typename Eigen::SparseMatrix<T>::InnerIterator it(eigenSparseMatrix,k); it; ++it) {
            innerIndex[innerOsqpPosition] = static_cast<c_int>(it.row());
            values[innerOsqpPosition] = static_cast<c_float>(it.value());
            innerOsqpPosition++;
        }
    }
    outerIndex[cols] = static_cast<c_int>(innerOsqpPosition);

    assert(innerOsqpPosition == numberOfNonZeroCoeff);


    // instantiate csc matrix
    // MEMORY ALLOCATION!!
    if(osqpSparseMatrix != nullptr){
        std::cerr << "[createOsqpSparseMatrix] osqpSparseMatrix pointer is not a null pointer! "
                  << std::endl;
        return false;
    }

    osqpSparseMatrix = csc_spalloc(rows, cols, (c_int)values.size(), 1, 0);

    // copy the inner index vector
    prea_int_vec_copy(innerIndex.data(),
                      osqpSparseMatrix->i,
                      (c_int)innerIndex.size());

    // copy the outer index vector
    prea_int_vec_copy(outerIndex.data(),
                      osqpSparseMatrix->p,
                      (c_int)outerIndex.size());

    // copy the values
    prea_vec_copy(values.data(),
                  osqpSparseMatrix->x,
                  (c_int)values.size());

    return true;
}

template<typename T>
bool OSQPWrapper::SparseMatrixHelper::osqpSparseMatrixToTriplets(const csc* const & osqpSparseMatrix,
                                                                 std::vector<Eigen::Triplet<T>> &tripletList)
{
    // if the matrix is not instantiate the triplets vector is empty
    if(osqpSparseMatrix == nullptr){
        std::cerr << "[osqpSparseMatrixToEiegenTriplets] the osqpSparseMatrix is not initialized."
                  << std::endl;
        return false;
    }

    // get row and column data
    c_int* innerIndexPtr = osqpSparseMatrix->i;
    c_int* outerIndexPtr = osqpSparseMatrix->p;

    // get values data
    c_float* valuePtr = osqpSparseMatrix->x;
    c_int numberOfNonZeroCoeff =  osqpSparseMatrix->p[osqpSparseMatrix->n];

    // populate the tripletes vector
    int column=0;
    int row;
    c_float value;

    // clear the std::vector
    tripletList.resize(numberOfNonZeroCoeff);
    for(int i = 0; i<numberOfNonZeroCoeff; i++) {
        row = innerIndexPtr[i];
        value = valuePtr[i];

        while(i >= outerIndexPtr[column+1])
            column++;

        tripletList[i] = Eigen::Triplet<T>(row, column, value);
    }
    return true;
}

template<typename T>
bool OSQPWrapper::SparseMatrixHelper::osqpSparseMatrixToEigenSparseMatrix(const csc* const & osqpSparseMatrix,
                                                                          Eigen::SparseMatrix<T> &eigenSparseMatrix)
{
    // if the matrix is not instantiate the eigen matrix is empty
    if(osqpSparseMatrix == nullptr) {
        std::cerr << "[osqpSparseMatrixToEigenSparseMatrix] the osqpSparseMatrix is not initialized."
                  << std::endl;
        return false;
    }

    // get the number of rows and columns
    int rows = osqpSparseMatrix->m;
    int cols = osqpSparseMatrix->n;

    // get the triplets from the csc matrix
    std::vector<Eigen::Triplet<T>> tripletList;

    OSQPWrapper::SparseMatrixHelper::osqpSparseMatrixToTriplets(osqpSparseMatrix, tripletList);

    // resize the eigen matrix
    eigenSparseMatrix.resize(rows, cols);

    // set the eigen matrix from triplets
    eigenSparseMatrix.setFromTriplets(tripletList.begin(), tripletList.end());
    return true;
}

template<typename T>
bool OSQPWrapper::SparseMatrixHelper::eigenSparseMatrixToTriplets(const Eigen::SparseMatrix<T> &eigenSparseMatrix,
                                                                  std::vector<Eigen::Triplet<T>> &tripletList)
{
    if(eigenSparseMatrix.nonZeros() == 0){
        std::cerr << "[eigenSparseMatrixToTriplets] The eigenSparseMatrix is empty."
                  << std::endl;
        return false;
    }

    // clear the std::vector
    tripletList.resize(eigenSparseMatrix.nonZeros());
    // populate the triplet list
    int nonZero = 0;
    for (int k=0; k < eigenSparseMatrix.outerSize(); ++k){
        for (typename Eigen::SparseMatrix<T>::InnerIterator it(eigenSparseMatrix,k); it; ++it){
            tripletList[nonZero] = Eigen::Triplet<T>(it.row(), it.col(), it.value());
            nonZero++;
        }
    }

    return true;
}
