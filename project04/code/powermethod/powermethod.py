import numpy as np
import argparse

def generate_matrix(n, test_case):
    """
    Generate matrix A based on the given test case.

    Parameters
    ----------
    n : int
        Size of the matrix (n x n).
    test_case : int
        The test case number.

    Returns
    -------
    A : ndarray
        The generated matrix.
    """
    A = np.zeros((n, n))
    
    if test_case == 1:
        # Test case 1: A(i, j) = 1 for 1 <= i, j <= n
        A[:, :] = 1.0
    elif test_case == 2:
        # Test case 2: A(i, j) = 1 if i == j, otherwise 0
        np.fill_diagonal(A, 1.0)
    elif test_case == 3:
        # Test case 3: A(i, j) = i if i == j, otherwise 0
        np.fill_diagonal(A, np.arange(1, n+1))
    elif test_case == 4:
        # Test case 4: A(i, j) = uniform random number [0, 1[
        rng = np.random.default_rng(42)
        A = rng.random((n, n))
    else:
        raise ValueError("Invalid test case number.")
    
    return A

def powermethod(A, y, niter, tol, verbose=False):
    """
    Compute approximations of the largest absolute eigenvalue and eigenvector
    with the power method.

    Parameters
    ----------
    A : array_like, shape (n, n)
        Matrix to compute largest absolute eigenvalue and eigenvector.
    y : array_like
        Initial vector.
    niter : int
        Maximum number of iterations.
    tol : float
        Tolerance when to stop iterating.
    verbose : bool, optional
        Print iterations. Default False.

    Returns
    -------
    theta : float
        Largest absolute eigenvalue.
    v : array_like
        Corresponding eigenvector.
    """
    for it in range(niter):
        v = y / np.linalg.norm(y)  # Normalize vector
        y = np.dot(A, v)           # Matrix-vector multiply
        theta = np.dot(v, y)       # Compute eigenvalue
        if verbose:
            print(f"{it = }, {theta = :25.15f}")
        if np.linalg.norm(y - theta * v) <= tol * np.abs(theta):
            v = y / np.linalg.norm(y)
            break

    if it == (niter - 1):
        raise ValueError("Did not converge.")
    else:
        return theta, v

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Power method for eigenvalue calculation with different test cases.")
    parser.add_argument('--test_case', type=int, default=1, help="Test case number (1, 2, 3, or 4).")
    parser.add_argument('--n', type=int, default=5, help="Size of the matrix (n x n).")
    parser.add_argument('--niter', type=int, default=10, help="Maximum number of iterations.")
    parser.add_argument('--tol', type=float, default=1.e-15, help="Tolerance for convergence.")
    
    args = parser.parse_args()
    
    # Generate the matrix A based on test case
    A = generate_matrix(args.n, args.test_case)
    
    # Initialize the vector y with random values
    rng = np.random.default_rng(42)
    y = rng.random(args.n)
    
    # Run the power method
    try:
        theta, y = powermethod(A, y, args.niter, args.tol, verbose=True)
        print(f"Power method: {theta = }, {y = }")
    except ValueError as e:
        print(e)

