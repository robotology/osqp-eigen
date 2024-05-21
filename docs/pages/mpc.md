# Using osqp-eigen in MPC fashion


The problem is to develop a controller that allows a linear system to track a constant reference state \f$x_r\f$. This kind of problem can be solved using a lot of different controller architectures, however in order to write a tutorial for osqp-eigen library the [**MPC**](https://en.wikipedia.org/wiki/Model_predictive_control) approach will be chosen.
 Thus we have to find a controller low $u_0^*$ such that:

```math
\begin{split}\begin{array}{ll}
  u_0 ^* = \argmin{}_{x_k, u_k}   & (x_N-x_r)^T Q_N (x_N-x_r) + \sum_{k=0}^{N-1} (x_k-x_r)^T Q (x_k-x_r) + u_k^T R u_k \\
  \text{s.t} & x_{k+1} = A x_k + B u_k \\
  & x_{\rm min} \le x_k  \le x_{\rm max} \\
  & u_{\rm min} \le u_k  \le u_{\rm max} \\
  & x_0 = \bar{x}
\end{array}\end{split}
```

 where $Q$, $Q_N$ and $R$ are symmetric positive definite matrices;
 the states $x_k$ and the inputs $u_k$ have to be constrained between some  lower and upper bounds and the reference state $x_r$ is
 
 ```math
 x_r = \begin{bmatrix} 0 & 0 & 1 & 0 & \cdots & & 0 \end{bmatrix} ^\top
 ```
 

 ## Convert MPC into a QP
 First of all the MPC problem has to be casted to a standard QP problem.

```math
\begin{split}\begin{array}{ll}
  \min & \frac{1}{2} x^T P x + q^T x \\
  \text{s.t} & l \leq A_c x \leq u
\end{array}\end{split}
 ```

where the hessian matrix $P$ is equal to
```math
P  = \text{diag}(Q, Q, ..., Q_N, R, ..., R)
```
while the gradient vector is
```math
q  = \begin{bmatrix}
-Q x_r \\
-Q x_r \\
\vdots \\
-Q_N x_r \\
0\\
\vdots\\
0
\end{bmatrix}
```

 The linear constraint matrix $A_c$ is
```math
A_c  =
\left[
\begin{array}{ccccc|cccc}
-I & 0 & 0 & \cdots & 0 & 0 & 0 & \cdots & 0\\
A & -I & 0 & \cdots & 0 & B & 0 & \cdots & 0\\
0 & A & -I & \cdots & 0 & 0 & B & \cdots &  0\\
\vdots & \vdots & \vdots  & \ddots & \vdots & \vdots & \vdots & \ddots & \vdots \\
0 & 0 & 0  & \cdots & -I & 0 & 0 & \cdots & B\\
\hline
I & 0 & 0 & \cdots & 0 & 0 & 0 & \cdots & 0\\
0 & I & 0 & \cdots & 0 & 0 & 0 & \cdots & 0\\
0 & 0 & I & \cdots & 0 & 0 & 0 & \cdots & 0\\
\vdots & \vdots & \vdots  & \ddots & \vdots & \vdots & \vdots & \ddots & \vdots \\
0 & 0 & 0 & \cdots & I & 0 & 0 & \cdots & 0\\
0 & 0 & 0 & \cdots & 0 & I & 0 & \cdots & 0\\
0 & 0 & 0 & \cdots & 0 & 0 & I & \cdots & 0\\
\vdots & \vdots & \vdots  & \ddots & \vdots & \vdots & \vdots & \ddots & \vdots \\
0 & 0 & 0 & \cdots & 0 & 0 & 0 & \cdots & I
\end{array}
\right]
```
 while the upper and the lower bound are
```math
l  = \begin{bmatrix}
-x_0 \\
0 \\
\vdots \\
0 \\
x_{min}\\
\vdots\\
x_{min}\\
u_{min}\\
\vdots\\
u_{min}\\
\end{bmatrix} \quad
, \ u  = \begin{bmatrix}
-x_0 \\
0 \\
\vdots \\
0 \\
x_{max}\\
\vdots\\
x_{max}\\
u_{max}\\
\vdots\\
u_{max}\\
\end{bmatrix}
```

Since the osqp-eigen handles only QP problem this operation shall be done by the user.
 You can find the implementation of the following functions [**here**](https://github.com/GiulioRomualdi/osqp-eigen/blob/master/example/src/MPCExample.cpp#L71-L182).
```cpp
castMPCToQPHessian(Q, R, mpcWindow, hessian);
castMPCToQPGradient(Q, xRef, mpcWindow, gradient);
castMPCToQPConstraintMatrix(a, b, mpcWindow, linearMatrix);
castMPCToQPConstraintVectores(xMax, xMin, uMax, uMin, x0, mpcWindow, lowerBound, upperBound);
```

 \subsection OSQP_init Solver initialization
Now you are able to use the OSQP solver. We first create an instance of the solver
```cpp
// instantiate the solver
OsqpEigen::Solver solver;
```
 when the solver is instantiated the [**default settings**](http://osqp.readthedocs.io/en/latest/interfaces/solver_settings.html) are automatically loaded, however you can change each setting using
 the following function
```cpp
solver.settings()->set<Setting>()
``` 
where `set<Setting>()` is a setter function. You can find the list of all the setter
 functions in the `OsqpEigen::Settings` class.
 For example you can use the warm start variables in the optimization problem by calling
```cpp
solver.settings()->setWarmStart(true);
```

 Now you can set the data of the optimization problem (number of variables, number of constraints
 and so on)
```cpp
solver.data()->setNumberOfVariables(numberOfVariable);
solver.data()->setNumberOfConstraints(numberOfConstraints);
if(!solver.data()->setHessianMatrix(hessian)) return 1;
if(!solver.data()->setGradient(gradient)) return 1;
if(!solver.data()->setLinearConstraintMatrix(linearMatrix)) return 1;
if(!solver.data()->setLowerBound(lowerBound)) return 1;
if(!solver.data()->setUpperBound(upperBound)) return 1;
``` 
The setter functions return `True` in case of success and `False` otherwise.
Now you are able to initialize the solver. All data and settings will be stored inside the osqp struct and the optimization problem will be initialized.
```cpp
if(!solver.initSolver()) return 1;
```
 The optimization problem can be solved calling the following method
```cpp
if(solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) return 1;
``` 
and the solution can be easily got by calling the following method

```cpp
Eigen::VectorXd QPSolution = solver.getSolution();
```

 If you need to update the bounds constraints and the gradient vector you
 can use the following methods:
 - `OsqpEigen::Solver::updateBounds` to update both upper and lower bounds;
 - `OsqpEigen::Solver::updateLowerBound` to update the lower bound;
 - `OsqpEigen::Solver::updateUpperBound` to update the upper bound;
 - `OsqpEigen::Solver::updateGradient` to update the gradient vector.

## Results Example
In the following the example of MPC controller is shown.
\include MPCExample.cpp

The example presented generates the following results
![](/docs/figures/mpc_result.png)
