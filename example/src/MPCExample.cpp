/**
 * @file MPCExample.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

// OSQPWrapper
#include "SparseMatrix.hpp"
#include "OptimizatorData.hpp"
#include "OptimizatorSettings.hpp"
#include "OptimizatorWorkspace.hpp"

// eigen
#include <Eigen/Dense>

void setDynamicsMatrices(Eigen::Matrix<double, 12, 12> &a, Eigen::Matrix<double, 12, 4> &b)
{
    a << 1.,      0.,     0., 0., 0., 0., 0.1,     0.,     0.,  0.,     0.,     0.    ,
        0.,      1.,     0., 0., 0., 0., 0.,      0.1,    0.,  0.,     0.,     0.    ,
        0.,      0.,     1., 0., 0., 0., 0.,      0.,     0.1, 0.,     0.,     0.    ,
        0.0488,  0.,     0., 1., 0., 0., 0.0016,  0.,     0.,  0.0992, 0.,     0.    ,
        0.,     -0.0488, 0., 0., 1., 0., 0.,     -0.0016, 0.,  0.,     0.0992, 0.    ,
        0.,      0.,     0., 0., 0., 1., 0.,      0.,     0.,  0.,     0.,     0.0992,
        0.,      0.,     0., 0., 0., 0., 1.,      0.,     0.,  0.,     0.,     0.    ,
        0.,      0.,     0., 0., 0., 0., 0.,      1.,     0.,  0.,     0.,     0.    ,
        0.,      0.,     0., 0., 0., 0., 0.,      0.,     1.,  0.,     0.,     0.    ,
        0.9734,  0.,     0., 0., 0., 0., 0.0488,  0.,     0.,  0.9846, 0.,     0.    ,
        0.,     -0.9734, 0., 0., 0., 0., 0.,     -0.0488, 0.,  0.,     0.9846, 0.    ,
        0.,      0.,     0., 0., 0., 0., 0.,      0.,     0.,  0.,     0.,     0.9846;

    b << 0.,      -0.0726,  0.,     0.0726,
        -0.0726,  0.,      0.0726, 0.    ,
        -0.0152,  0.0152, -0.0152, 0.0152,
        -0.,     -0.0006, -0.,     0.0006,
        0.0006,   0.,     -0.0006, 0.0000,
        0.0106,   0.0106,  0.0106, 0.0106,
        0,       -1.4512,  0.,     1.4512,
        -1.4512,  0.,      1.4512, 0.    ,
        -0.3049,  0.3049, -0.3049, 0.3049,
        -0.,     -0.0236,  0.,     0.0236,
        0.0236,   0.,     -0.0236, 0.    ,
        0.2107,   0.2107,  0.2107, 0.2107;
}

void setInequalityConstraints(Eigen::Matrix<double, 12, 1> &xMax, Eigen::Matrix<double, 12, 1> &xMin,
                              Eigen::Matrix<double, 4, 1> &uMax, Eigen::Matrix<double, 4, 1> &uMin)
{
    double u0 = 10.5916;

    // input inequality constraints
    uMin << 9.6 - u0,
        9.6 - u0,
        9.6 - u0,
        9.6 - u0;

    uMax << 13 - u0,
        13 - u0,
        13 - u0,
        13 - u0;

    // state inequality constraints
    xMin << -M_PI/6,-M_PI/6,-INFINITY,-INFINITY,-INFINITY,-1.,
        -INFINITY,-INFINITY,-INFINITY,-INFINITY,-INFINITY,-INFINITY;

    xMax << M_PI/6,M_PI/6, INFINITY,INFINITY,INFINITY,INFINITY,
        INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY;
}

void setWeightMatrices(Eigen::DiagonalMatrix<double, 12> &Q, Eigen::DiagonalMatrix<double, 4> &R)
{
    Q.diagonal() << 0, 0, 10., 10., 10., 10., 0, 0, 0, 5., 5., 5.;
    R.diagonal() << 0.1, 0.1, 0.1, 0.1;
}

void castMPCToQPHessian(const Eigen::DiagonalMatrix<double, 12> &Q, const Eigen::DiagonalMatrix<double, 4> &R, int mpcWindow,
                        OSQPWrapper::SparseMatrix &hessian_s)
{

    Eigen::SparseMatrix<double> h(12*(mpcWindow+1) + 4 * mpcWindow, 12*(mpcWindow+1) + 4 * mpcWindow);

    //populate hessian matrix
    for(int i = 0; i<12*(mpcWindow+1) + 4 * mpcWindow; i++){
        if(i < 12*(mpcWindow+1)){
            int posQ=i%12;
            float value = Q.diagonal()[posQ];
            if(value != 0)
                h.insert(i,i) = value;
        }
        else{
            int posR=i%4;
            float value = R.diagonal()[posR];
            if(value != 0)
                h.insert(i,i) = value;
        }
    }

    // turn the matrix into the standard compressed format
    h.makeCompressed();

    hessian_s << h;
}

void castMPCToQPGradient(const Eigen::DiagonalMatrix<double, 12> &Q, const Eigen::Matrix<double, 12, 1> &xRef, int mpcWindow,
                         Eigen::VectorXd &gradient)
{

    Eigen::Matrix<double,12,1> Qx_ref;
    Qx_ref = Q * (-xRef);

    // populate the gradient vector
    gradient = Eigen::VectorXd::Zero(12*(mpcWindow+1) +  4*mpcWindow, 1);
    for(int i = 0; i<12*(mpcWindow+1); i++){
        int posQ = i % 12;
        float value = Qx_ref(posQ,0);
        gradient(i,0) = value;
    }
}

void castMPCToQPConstraintMatrix(const Eigen::Matrix<double, 12, 12> &dynamicMatrix, const Eigen::Matrix<double, 12, 4> &controlMatrix,
                                 int mpcWindow, OSQPWrapper::SparseMatrix &linearMatrix)
{
    Eigen::SparseMatrix<double> linear(12*(mpcWindow+1)  + 12*(mpcWindow+1) + 4 * mpcWindow, 12*(mpcWindow+1) + 4 * mpcWindow);

    // populate linear constraint matrix
    for(int i = 0; i<12*(mpcWindow+1); i++){
        linear.insert(i,i) = -1;
    }

    for(int i = 0; i < mpcWindow; i++)
        for(int j = 0; j<12; j++)
            for(int k = 0; k<12; k++){
                float value = dynamicMatrix(j,k);
                if(value != 0){
                    linear.insert(12 * (i+1) + j, 12 * i + k) = value;
                }
            }

    for(int i = 0; i < mpcWindow; i++)
        for(int j = 0; j < 12; j++)
            for(int k = 0; k < 4; k++){
                float value = controlMatrix(j,k);
                if(value != 0){
                    linear.insert(12*(i+1)+j, 4*i+k+12*(mpcWindow + 1)) = value;
                }
            }

    for(int i = 0; i<12*(mpcWindow+1) + 4*mpcWindow; i++){
        linear.insert(i+(mpcWindow+1)*12,i) = 1;
    }

    // turn the matrix into the standard compressed format
    linear.makeCompressed();

    linearMatrix << linear;
}

void castMPCToQPConstraintVectores(const Eigen::Matrix<double, 12, 1> &xMax, const Eigen::Matrix<double, 12, 1> &xMin,
                                   const Eigen::Matrix<double, 4, 1> &uMax, const Eigen::Matrix<double, 4, 1> &uMin,
                                   const Eigen::Matrix<double, 12, 1> &x0,
                                   int mpcWindow, Eigen::VectorXd &lowerBound, Eigen::VectorXd &upperBound)
{
    // evaluate the lower and the upper inequality vectors
    Eigen::VectorXd lowerInequality = Eigen::MatrixXd::Zero(12*(mpcWindow+1) +  4 * mpcWindow, 1);
    Eigen::VectorXd upperInequality = Eigen::MatrixXd::Zero(12*(mpcWindow+1) +  4 * mpcWindow, 1);
    for(int i=0; i<mpcWindow+1; i++){
        lowerInequality.block(12*i,0,12,1) = xMin;
        upperInequality.block(12*i,0,12,1) = xMax;
    }
    for(int i=0; i<mpcWindow; i++){
        lowerInequality.block(4 * i + 12 * (mpcWindow + 1), 0, 4, 1) = uMin;
        upperInequality.block(4 * i + 12 * (mpcWindow + 1), 0, 4, 1) = uMax;
    }

    // evaluate the lower and the upper equality vectors
    Eigen::VectorXd lowerEquality = Eigen::MatrixXd::Zero(12*(mpcWindow+1),1 );
    Eigen::VectorXd upperEquality;
    lowerEquality.block(0,0,12,1) = -x0;
    upperEquality = lowerEquality;
    lowerEquality = lowerEquality;

    // merge inequality and equality vectors
    lowerBound = Eigen::MatrixXd::Zero(2*12*(mpcWindow+1) +  4*mpcWindow,1 );
    lowerBound << lowerEquality,
        lowerInequality;

    upperBound = Eigen::MatrixXd::Zero(2*12*(mpcWindow+1) +  4*mpcWindow,1 );
    upperBound << upperEquality,
        upperInequality;
}


void updateConstraintVectors(const Eigen::Matrix<double, 12, 1> &x0,
                             Eigen::VectorXd &lowerBound, Eigen::VectorXd &upperBound)
{
    lowerBound.block(0,0,12,1) = -x0;
    upperBound.block(0,0,12,1) = -x0;
}

int main()
{
    // set the preview window
    int mpcWindow = 20;

    // allocate the dynamics matrices
    Eigen::Matrix<double, 12, 12> a;
    Eigen::Matrix<double, 12, 4> b;

    // allocate the constraints vector
    Eigen::Matrix<double, 12, 1> xMax;
    Eigen::Matrix<double, 12, 1> xMin;
    Eigen::Matrix<double, 4, 1> uMax;
    Eigen::Matrix<double, 4, 1> uMin;

    // allocate the weight matrices
    Eigen::DiagonalMatrix<double, 12> Q;
    Eigen::DiagonalMatrix<double, 4> R;

    // allocate the initial and the reference state space
    Eigen::Matrix<double, 12, 1> x0;
    Eigen::Matrix<double, 12, 1> xRef;

    // allocate QP problem matrices and vectores
    OSQPWrapper::SparseMatrix hessian;
    Eigen::VectorXd gradient;
    OSQPWrapper::SparseMatrix linearMatrix;
    Eigen::VectorXd lowerBound;
    Eigen::VectorXd upperBound;

    // set the initial and the desired states
    x0 << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ;
    xRef <<  0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0;

    // set MPC problem quantities
    setDynamicsMatrices(a, b);
    setInequalityConstraints(xMax, xMin, uMax, uMin);
    setWeightMatrices(Q, R);

    // cast the MPC problem as QP problem
    castMPCToQPHessian(Q, R, mpcWindow, hessian);
    castMPCToQPGradient(Q, xRef, mpcWindow, gradient);
    castMPCToQPConstraintMatrix(a, b, mpcWindow, linearMatrix);
    castMPCToQPConstraintVectores(xMax, xMin, uMax, uMin, x0, mpcWindow, lowerBound, upperBound);

    // instantiate the OSQP solver
    OSQPWrapper::OptimizatorSettings settings;
    settings.setWarmStart(true);
    //settings.setVerbosity(false);

    OSQPWrapper::OptimizatorData data;
    // set the QP data in to OSQP optimization solver
    data.setNumberOfVariables(12 * (mpcWindow + 1) + 4 * mpcWindow);
    data.setNumberOfConstraints(2 * 12 * (mpcWindow + 1) +  4 * mpcWindow);

    if(!data.setHessianMatrix(hessian)){
        std::cerr << "Error when the hessian matrix is added to the optimization problem."
                  << std::endl;
        return 1;
    }

    if(!data.setGradient(gradient)){
        std::cerr << "Error when the gradient vector is added to the optimization problem."
                  << std::endl;
        return 1;
    }

    if(!data.setLinearConstraintsMatrix(linearMatrix)){
        std::cerr << "Error when the linear constraint matrix is added to the optimization problem."
                  << std::endl;
        return 1;
    }

    if(!data.setLowerBound(lowerBound)){
        std::cerr << "Error when the lower constraint vector is added to the optimization problem."
                  << std::endl;
        return 1;
    }

    if(!data.setUpperBound(upperBound)){
        std::cerr << "Error when the upper constraint vector is added to the optimization problem."
                  << std::endl;
        return 1;
    }

    // instantiate the solver
    OSQPWrapper::OptimizatorWorkspace solver;

    if(!solver.setWorkspace(data, settings)){
        std::cerr << "Error in the solver initialization."
                  << std::endl;
        return 1;
    }

    // controller input and QPSolution vector
    Eigen::Vector4d ctr;
    Eigen::VectorXd QPSolution;

    // number of iteration steps
    int numberOfSteps = 50;

    for (int i = 0; i < numberOfSteps; i++){

        // solve the QP problem
        if(!solver.solve()){
            std::cerr << "Error when the solution optimization problem is evaluated."
                      << std::endl;
            return 1;
        }

        // get the controller input
        QPSolution = solver.getSolution();
        ctr = QPSolution.block(12 * (mpcWindow + 1), 0, 4, 1);

        // save data into file
        auto x0Data = x0.data();

        // propagate the model
        x0 = a * x0 + b * ctr;

        // update the constraint bound
        updateConstraintVectors(x0, lowerBound, upperBound);
        if(!solver.updateBounds(lowerBound, upperBound)){
            std::cerr << "Error when the lower and upper bounds are updated."
                      << std::endl;
            return 1;
        }
      }

    return 0;
}
