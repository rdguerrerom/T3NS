#ifndef SYMMETRY_U1_H
# define SYMMETRY_U1_H

/**
 * \file symmetry_u1.h
 * \brief file for the \f$U(1)\f$ symmetry.
 *
 * The labels of the irreps are:\n
 * label = \f$N\f$
 */

/**
 * \brief Gives the maximal label + 1 of the irreps that can be generated by U1.
 *
 * \param [in] prop1 The first array of irreps.
 * \param [in] nr1 The number of irreps in prop1.
 * \param [in] prop2 The second array of irreps.
 * \param [in] nr2 The number of irreps in prop2.
 * \param [in] inc increment between irreps in prop1 and prop2.
 * \return returns the maximal label of the irreps that can be generated.
 */
int U1_get_max_irrep( int *prop1, int nr1, int *prop2, int nr2, int inc );

/**
 * \brief Gives the resulting irreps from tensor product of two other irreps belonging to sg.
 *
 * \param [out] min_irrep The lowest label of resulting irrep.
 * \param [out] nr_irreps Number of resulting irreps.
 * \param [out] step Step with which the labels are separated.
 * \param [in] irrep1 The first irrep of the tensorproduct.
 * \param [in] irrep2 The second irrep of the tensorproduct.
 * \param [in] sign -1 if the inverse of irrep2 should be taken, +1 otherwise.
 */
void U1_tensprod_irrep( int *min_irrep, int *nr_irreps, int *step, int irrep1, int irrep2, int sign);

/**
 * \brief Returns the irrepstring, or INVALID if invalid.
 *
 * \param[out] buffer The string.
 * \param[in] irr The irrep.
 */
void U1_get_irrstring( char buffer[], int irr );

/**
 * \brief finds which irrep is given in the buffer.
 *
 * \param [in] buffer The buffer.
 * \param [out] irr The irrep.
 * \return 1 if successful, 0 otherwise.
 */
int U1_which_irrep( char buffer[], int *irr );
#endif
