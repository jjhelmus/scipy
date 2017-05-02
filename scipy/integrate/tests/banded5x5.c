/* banded5x5.f -- translated by f2c (version 20100827).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Common Block Declarations */

struct jac_1_ {
    doublereal bands[20]	/* was [4][5] */;
};
struct jac_2_ {
    doublereal jacband[20]	/* was [4][5] */;
};

#define jac_1 (*(struct jac_1_ *) &jac_)
#define jac_2 (*(struct jac_2_ *) &jac_)

/* Initialized data */

struct {
    doublereal e_1[20];
    } jac_ = { 0., -1., .25, .1, .25, -5., .25, .1, .25, -25., .25, .1, .25, 
	    -125., .25, 0., .25, -625., 0., 0. };


/* Table of constant values */

static integer c__1 = 1;

/*   banded5x5.f */

/*   This Fortran library contains implementations of the */
/*   differential equation */
/*       dy/dt = A*y */
/*   where A is a 5x5 banded matrix (see below for the actual */
/*   values).  These functions will be used to test */
/*   scipy.integrate.odeint. */

/*   The idea is to solve the system two ways: pure Fortran, and */
/*   using odeint.  The "pure Fortran" solver is implemented in */
/*   the subroutine banded5x5_solve below.  It calls LSODA to */
/*   solve the system. */

/*   To solve the same system using odeint, the functions in this */
/*   file are given a python wrapper using f2py.  Then the code */
/*   in test_odeint_jac.py uses the wrapper to implement the */
/*   equation and Jacobian functions required by odeint.  Because */
/*   those functions ultimately call the Fortran routines defined */
/*   in this file, the two method (pure Fortran and odeint) should */
/*   produce exactly the same results.  (That's assuming floating */
/*   point calculations are deterministic, which can be an */
/*   incorrect assumption.)  If we simply re-implemented the */
/*   equation and Jacobian functions using just python and numpy, */
/*   the floating point calculations would not be performed in */
/*   the same sequence as in the Fortran code, and we would obtain */
/*   different answers.  The answer for either method would be */
/*   numerically "correct", but the errors would be different, */
/*   and the counts of function and Jacobian evaluations would */
/*   likely be different. */

/* Subroutine */ int jacobian_(void)
{
    return 0;
} /* jacobian_ */

/*     The data for a banded Jacobian stored in packed banded */
/*     format.  The full Jacobian is */

/*           -1,  0.25,     0,     0,     0 */
/*         0.25,    -5,  0.25,     0,     0 */
/*         0.10,  0.25,   -25,  0.25,     0 */
/*            0,  0.10,  0.25,  -125,  0.25 */
/*            0,     0,  0.10,  0.25,  -625 */

/*     The columns in the following layout of numbers are */
/*     the upper diagonal, main diagonal and two lower diagonals */
/*     (i.e. each row in the layout is a column of the packed */
/*     banded Jacobian).  The values 0.00D0 are in the "don't */
/*     care" positions. */

/* Subroutine */ int getbands_(doublereal *jac)
{
    static integer i__, j;

/* f2py intent(out) jac */
    /* Parameter adjustments */
    jac -= 5;

    /* Function Body */
    for (i__ = 1; i__ <= 4; ++i__) {
	for (j = 1; j <= 5; ++j) {
	    jac[i__ + (j << 2)] = jac_1.bands[i__ + (j << 2) - 5];
/* L5: */
	}
    }
    return 0;
} /* getbands_ */


/* Differential equations, right-hand-side */

/* Subroutine */ int banded5x5_(integer *n, doublereal *t, doublereal *y, 
	doublereal *f)
{
    /* Parameter adjustments */
    --f;
    --y;

    /* Function Body */
    f[1] = jac_1.bands[1] * y[1] + jac_1.bands[4] * y[2];
    f[2] = jac_1.bands[2] * y[1] + jac_1.bands[5] * y[2] + jac_1.bands[8] * y[
	    3];
    f[3] = jac_1.bands[3] * y[1] + jac_1.bands[6] * y[2] + jac_1.bands[9] * y[
	    3] + jac_1.bands[12] * y[4];
    f[4] = jac_1.bands[7] * y[2] + jac_1.bands[10] * y[3] + jac_1.bands[13] * 
	    y[4] + jac_1.bands[16] * y[5];
    f[5] = jac_1.bands[11] * y[3] + jac_1.bands[14] * y[4] + jac_1.bands[17] *
	     y[5];
    return 0;
} /* banded5x5_ */


/* Jacobian */

/* The subroutine assumes that the full Jacobian is to be computed. */
/* ml and mu are ignored, and nrowpd is assumed to be n. */

/* Subroutine */ int banded5x5_jac__(integer *n, doublereal *t, doublereal *y,
	 integer *ml, integer *mu, doublereal *jac, integer *nrowpd)
{
    /* System generated locals */
    integer jac_dim1, jac_offset;

    /* Local variables */
    static integer i__, j;

    /* Parameter adjustments */
    --y;
    jac_dim1 = *nrowpd;
    jac_offset = 1 + jac_dim1;
    jac -= jac_offset;

    /* Function Body */
    for (i__ = 1; i__ <= 4; ++i__) {
	for (j = 1; j <= 5; ++j) {
	    if (i__ - j > 0) {
		jac[i__ - j + j * jac_dim1] = jac_1.bands[i__ + (j << 2) - 5];
	    }
/* L15: */
	}
    }
    return 0;
} /* banded5x5_jac__ */


/* Banded Jacobian */

/* ml = 2, mu = 1 */

/* Subroutine */ int banded5x5_bjac__(integer *n, doublereal *t, doublereal *
	y, integer *ml, integer *mu, doublereal *bjac, integer *nrowpd)
{
    /* System generated locals */
    integer bjac_dim1, bjac_offset;

    /* Local variables */
    static integer i__, j;

    /* Parameter adjustments */
    --y;
    bjac_dim1 = *nrowpd;
    bjac_offset = 1 + bjac_dim1;
    bjac -= bjac_offset;

    /* Function Body */
    for (i__ = 1; i__ <= 4; ++i__) {
	for (j = 1; j <= 5; ++j) {
	    bjac[i__ + j * bjac_dim1] = jac_1.bands[i__ + (j << 2) - 5];
/* L20: */
	}
    }
    return 0;
} /* banded5x5_bjac__ */

/* Subroutine */ int banded5x5_solve__(doublereal *y, integer *nsteps, 
	doublereal *dt, integer *jt, integer *nst, integer *nfe, integer *nje)
{
    /* Format strings */
    static char fmt_89[] = "(1x,\002Error: istate=\002,i3)";

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);

    /* Local variables */
    extern /* Subroutine */ int banded5x5_(integer *, doublereal *, 
	    doublereal *, doublereal *);
    static integer i__;
    static doublereal t;
    static integer neq, liw, lrw;
    extern /* Subroutine */ int banded5x5_jac__(integer *, doublereal *, 
	    doublereal *, integer *, integer *, doublereal *, integer *);
    static doublereal atol;
    static integer itol, iopt;
    static doublereal rtol, tout;
    extern /* Subroutine */ int banded5x5_bjac__(integer *, doublereal *, 
	    doublereal *, integer *, integer *, doublereal *, integer *), 
	    lsoda_(S_fp, integer *, doublereal *, doublereal *, doublereal *, 
	    integer *, doublereal *, doublereal *, integer *, integer *, 
	    integer *, doublereal *, integer *, integer *, integer *, S_fp, 
	    integer *);
    static integer itask, iwork[500];
    static doublereal rwork[500];
    static integer istate;

    /* Fortran I/O blocks */
    static cilist io___21 = { 0, 6, 0, fmt_89, 0 };


/*     jt is the Jacobian type: */
/*         jt = 1  Use the full Jacobian. */
/*         jt = 4  Use the banded Jacobian. */
/*     nst, nfe and nje are outputs: */
/*         nst:  Total number of internal steps */
/*         nfe:  Total number of function (i.e. right-hand-side) */
/*               evaluations */
/*         nje:  Total number of Jacobian evaluations */
/*     Arguments... */
/* f2py intent(inout) y */
/* f2py intent(in) nsteps, dt, jt */
/* f2py intent(out) nst, nfe, nje */
/*     Local variables... */
/*     Common block... */
/*     --- t range --- */
    /* Parameter adjustments */
    --y;

    /* Function Body */
    t = 0.;
/*     --- Solver tolerances --- */
    rtol = 1e-11;
    atol = 1e-13;
    itol = 1;
/*     --- Other LSODA parameters --- */
    neq = 5;
    itask = 1;
    istate = 1;
    iopt = 0;
    iwork[0] = 2;
    iwork[1] = 1;
    lrw = 500;
    liw = 500;
/*     --- Call LSODA in a loop to compute the solution --- */
    i__1 = *nsteps;
    for (i__ = 1; i__ <= i__1; ++i__) {
	tout = i__ * *dt;
	if (*jt == 1) {
	    lsoda_((S_fp)banded5x5_, &neq, &y[1], &t, &tout, &itol, &rtol, &
		    atol, &itask, &istate, &iopt, rwork, &lrw, iwork, &liw, (
		    S_fp)banded5x5_jac__, jt);
	} else {
	    lsoda_((S_fp)banded5x5_, &neq, &y[1], &t, &tout, &itol, &rtol, &
		    atol, &itask, &istate, &iopt, rwork, &lrw, iwork, &liw, (
		    S_fp)banded5x5_bjac__, jt);
	}
/* L40: */
	if (istate < 0) {
	    goto L80;
	}
    }
    *nst = iwork[10];
    *nfe = iwork[11];
    *nje = iwork[12];
    return 0;
L80:
    s_wsfe(&io___21);
    do_fio(&c__1, (char *)&istate, (ftnlen)sizeof(integer));
    e_wsfe();
    return 0;
} /* banded5x5_solve__ */

