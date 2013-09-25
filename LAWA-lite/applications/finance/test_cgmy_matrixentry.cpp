#include <iostream>
#include <fstream>
#include <lawa/lawa.h>
#include <applications/finance/operators/cgmyoperator1d.h>
#include <applications/finance/righthandsides/righthandsides.h>

using namespace std;
using namespace lawa;

typedef long double T;

typedef Basis<T, Orthogonal, Interval, Multi>                       PrimalBasis;
//typedef Basis<T, Primal, Interval, Dijkema>                       PrimalBasis;


struct CGMYKernel {

    CGMYKernel(const Kernel<T,CGMY> &_kernel) : kernel(_kernel) { };

    T
    operator()(T x) const { return kernel.SecondTailIntegral(-x); }

    const Kernel<T,CGMY> &kernel;
};

T
referenceValue(const PrimalBasis &basis, const Kernel<T,CGMY> &kernel,
               const Integral<Gauss, PrimalBasis, PrimalBasis> &integral,
               int j1, int k1, XType xtype1, int j2, int k2, XType xtype2);

int main()
{
    cout.precision(16);
    /// wavelet basis parameters:
    int d = 3;          // mandatory for this test case!!!

    //int j0 = 3;         // minimal level
    //int J = 6;
    //PrimalBasis basis(d,d, j0);
    int j0 = 0;         // minimal level
    int J = 4;
    PrimalBasis basis(d, j0);
    basis.enforceBoundaryCondition<DirichletBC>();

    Integral<Gauss, PrimalBasis, PrimalBasis> integral(basis,basis);

    //std::cerr << basis.psi(0.4,2,12,0) << endl;
    //return 0;

    ProcessParameters1D<T,CGMY> processparameters(0., 1., 7.4, 8.5, 0.8);
    Kernel<T,CGMY> kernel(processparameters);
    CGMYKernel cgmykernel(kernel);

    //SingularIntegral<CGMYKernel,PrimalBasis,PrimalBasis> singularIntegral(cgmykernel,basis,basis);
    SingularIntegral<Kernel<T,CGMY>,PrimalBasis,PrimalBasis> singularIntegral(kernel,basis,basis);
    int order = 10, n = 40;
    T sigma = 0.1, mu = 0.3, omega = 0.01;
    singularIntegral.singularquadrature.setParameters(order, n, sigma, mu, omega);

    int j1, k1, j2, k2;
    XType xtype1, xtype2;

    T maxError = 0.;
    Index1D maxIndex1, maxIndex2;

    cout << "Full scaling range:  " << basis.mra.rangeI(j0) << endl;
    cout << "Left scaling range:  " << basis.mra.rangeIL(j0) << endl;
    cout << "Right scaling range: " << basis.mra.rangeIR(j0) << endl;
    cout << "Full wavelet range:  " << basis.rangeJ(j0) << endl;
    cout << "Left wavelet range:  " << basis.rangeJL(j0) << endl;
    cout << "Right wavelet range: " << basis.rangeJR(j0) << endl;

    for (int k_row=basis.mra.rangeI(j0).firstIndex(); k_row<=basis.mra.rangeI(j0).lastIndex(); ++k_row) {
        if (   (d==3 && k_row<=basis.mra.rangeIL(j0).lastIndex())
            || (d==3 && k_row>=basis.mra.rangeIR(j0).firstIndex()) )  continue;
        j1 = j0; k1 = k_row; xtype1 = XBSpline;
        for (int k_col=basis.mra.rangeI(j0).firstIndex(); k_col<=basis.mra.rangeI(j0).lastIndex(); ++k_col) {
            if (   (d==3 && k_col<=basis.mra.rangeIL(j0).lastIndex())
                || (d==3 && k_col>=basis.mra.rangeIR(j0).firstIndex()) ) continue;
            j2 = j0; k2 = k_col; xtype2 = XBSpline;

            T refVal    = referenceValue(basis, kernel, integral, j1, k1, xtype1, j2, k2, xtype2);
            T approxVal = singularIntegral(j1,k1,xtype1,1, j2,k2,xtype2,1);
            T error     = fabs(refVal-approxVal);
            cout << Index1D(j1,k1,xtype1) << " " << Index1D(j2,k2,xtype2) << " : "
                 << refVal << " " << approxVal << " " << error << endl;
            if (error>maxError) {
                maxError = error; maxIndex1.j = j1; maxIndex1.k = k1; maxIndex1.xtype = xtype1;
                                  maxIndex2.j = j2; maxIndex2.k = k2; maxIndex2.xtype = xtype2;
            }
        }
        for (int j_col=j0; j_col<J; ++j_col) {
            for (int k_col=basis.rangeJ(j_col).firstIndex(); k_col<=basis.rangeJ(j_col).lastIndex(); ++k_col) {
                if (   (d==3 && k_col<=basis.rangeJL(j_col).lastIndex())
                    || (d==3 && k_col>=basis.rangeJR(j_col).firstIndex()) ) continue;
                j2 = j_col; k2 = k_col; xtype2 = XWavelet;

                T refVal    = referenceValue(basis, kernel, integral, j1, k1, xtype1, j2, k2, xtype2);
                T approxVal = singularIntegral(j1,k1,xtype1,1, j2,k2,xtype2,1);
                T error     = fabs(refVal-approxVal);
                cout << Index1D(j1,k1,xtype1) << " " << Index1D(j2,k2,xtype2) << " : "
                     << refVal << " " << approxVal << " " << refVal-approxVal << endl;
                if (error>maxError) {
                    maxError = error; maxIndex1.j = j1; maxIndex1.k = k1; maxIndex1.xtype = xtype1;
                                      maxIndex2.j = j2; maxIndex2.k = k2; maxIndex2.xtype = xtype2;
                }
            }
        }
    }
    for (int j_row=j0; j_row<J; ++j_row) {
        for (int k_row=basis.rangeJ(j_row).firstIndex(); k_row<=basis.rangeJ(j_row).lastIndex(); ++k_row) {
            j1 = j_row; k1 = k_row; xtype1 = XWavelet;
            if (   (d==3 && k_row<=basis.rangeJL(j_row).lastIndex())
                || (d==3 && k_row>=basis.rangeJR(j_row).firstIndex()) ) continue;
            for (int k_col=basis.mra.rangeI(j0).firstIndex(); k_col<=basis.mra.rangeI(j0).lastIndex(); ++k_col) {
                j2 = j0; k2 = k_col; xtype2 = XBSpline;
                if (   (d==3 && k_col<=basis.mra.rangeIL(j0).lastIndex())
                    || (d==3 && k_col>=basis.mra.rangeIR(j0).firstIndex()) ) continue;
                T refVal    = referenceValue(basis, kernel, integral, j1, k1, xtype1, j2, k2, xtype2);
                T approxVal = singularIntegral(j1,k1,xtype1,1, j2,k2,xtype2,1);
                T error     = fabs(refVal-approxVal);
                cout << Index1D(j1,k1,xtype1) << " " << Index1D(j2,k2,xtype2) << " : "
                     << refVal << " " << approxVal << " " << refVal-approxVal << endl;
                if (error>maxError) {
                    maxError = error; maxIndex1.j = j1; maxIndex1.k = k1; maxIndex1.xtype = xtype1;
                                      maxIndex2.j = j2; maxIndex2.k = k2; maxIndex2.xtype = xtype2;
                }
            }
            for (int j_col=j0; j_col<J; ++j_col) {
                for (int k_col=basis.rangeJ(j_col).firstIndex(); k_col<=basis.rangeJ(j_col).lastIndex(); ++k_col) {
                    if (   (d==3 && k_col<=basis.rangeJL(j_col).lastIndex())
                        || (d==3 && k_col>=basis.rangeJR(j_col).firstIndex()) ) continue;
                    j2 = j_col; k2 = k_col; xtype2 = XWavelet;

                    T refVal    = referenceValue(basis, kernel, integral, j1, k1, xtype1, j2, k2, xtype2);
                    T approxVal = singularIntegral(j1,k1,xtype1,1, j2,k2,xtype2,1);
                    T error     = fabs(refVal-approxVal);
                    cout << Index1D(j1,k1,xtype1) << " " << Index1D(j2,k2,xtype2) << " : "
                         << refVal << " " << approxVal << " " << refVal-approxVal << endl;
                    if (error>maxError) {
                        maxError = error; maxIndex1.j = j1; maxIndex1.k = k1; maxIndex1.xtype = xtype1;
                                          maxIndex2.j = j2; maxIndex2.k = k2; maxIndex2.xtype = xtype2;
                    }
                }
            }
        }
    }

    cout << "Maximum absolute error: " << maxError << " encountered for " << maxIndex1 << " "
         << maxIndex2 << endl;


    return 0;

}

T
referenceValue(const PrimalBasis &basis, const Kernel<T,CGMY> &kernel,
               const Integral<Gauss, PrimalBasis, PrimalBasis> &integral,
               int j1, int k1, XType xtype1, int j2, int k2, XType xtype2)
{
    GeMatrix<FullStorage<T,ColMajor> > varphi_row_deltas, varphi_col_deltas;
    varphi_row_deltas = computeDeltas<T,PrimalBasis>(basis,j1,k1,xtype1);
    varphi_col_deltas = computeDeltas<T,PrimalBasis>(basis,j2,k2,xtype2);

    T part1=(T)0., part2=(T)0.;

    if (basis.d==2) {


        for (int lambda=varphi_row_deltas.rows().firstIndex(); lambda<=varphi_row_deltas.rows().lastIndex(); ++lambda) {
            T x = varphi_row_deltas(lambda,1);

            part1 += varphi_row_deltas(lambda,2)*basis.generator(xtype2)(x,j2,k2,0)*kernel.c3;

            for (int mu=varphi_col_deltas.rows().firstIndex(); mu<=varphi_col_deltas.rows().lastIndex(); ++mu) {
                T y = varphi_col_deltas(mu,1);
                T c = varphi_col_deltas(mu,2)*varphi_row_deltas(lambda,2);

                if (fabs(x-y)>1e-10)  {
                    T value_tailintegral=kernel.ForthTailIntegral(y-x);
                    if (y-x>0)  part2 += c * (value_tailintegral - kernel.constants[2]);
                    else        part2 += c * (value_tailintegral - kernel.constants[3]);
                }
            }
        }
        return -(part1 + part2);
    }
    else if (basis.d==3) {
        T int_val = 0.;

        int_val -= kernel.c3*integral(j1,k1,xtype1,1,j2,k2,xtype2,1);

        for (int mu=varphi_col_deltas.rows().firstIndex();
                mu<=varphi_col_deltas.rows().lastIndex(); ++mu) {

           T y = varphi_col_deltas(mu,1);
           if (fabs(varphi_col_deltas(mu,2)) < 1e-14) continue;

           part1 += varphi_col_deltas(mu,2)*basis.generator(xtype1)(y,j1,k1,0)*kernel.c4;
           part1 -= varphi_col_deltas(mu,2)*basis.generator(xtype1)(y,j1,k1,1)*kernel.c5;

           for (int lambda=varphi_row_deltas.rows().firstIndex();
                    lambda<=varphi_row_deltas.rows().lastIndex(); ++lambda) {

               if (fabs(varphi_row_deltas(lambda,2)) < 1e-14) continue;
               T x = varphi_row_deltas(lambda,1);
               T c = varphi_col_deltas(mu,2)*varphi_row_deltas(lambda,2);
               if (x!=y)  {
                   if (y-x>0)  {
                       part2 -= c * (kernel.SixthTailIntegral(y-x) - kernel.constants[6]);
                   }
                   else    {
                       part2 -= c * (kernel.SixthTailIntegral(y-x) - kernel.constants[7]);
                   }
               }
           }
        }
        int_val += part1 + part2;

        return -int_val;
    }
}
