/*
   Copyright 2013 David Malcolm <dmalcolm@redhat.com>
   Copyright 2013 Red Hat, Inc.

   This is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see
   <http://www.gnu.org/licenses/>.
*/

/*
   Private header for the API, for handling compatibility between GCC
   versions
*/

#ifndef INCLUDED__GCC_C_API_PRIVATE_COMPAT_H
#define INCLUDED__GCC_C_API_PRIVATE_COMPAT_H

/*************************************************************************
 Vectors
 *************************************************************************/

/* Getting the length of a vector, returning 0 if it is NULL: */
#if (GCC_VERSION >= 4008)
  #define GCC_COMPAT_VEC_LENGTH(KIND, V) \
    ( (V) ? ( (V)->length() ) : 0 )
#else
  #define GCC_COMPAT_VEC_LENGTH(KIND, V) \
    ( VEC_length(KIND, (V)) )
#endif

/* Looking up an element by index: */
#if (GCC_VERSION >= 4008)
  #define GCC_COMPAT_VEC_INDEX(KIND, V, IDX) \
    ( (*(V))[IDX] )
#else
  #define GCC_COMPAT_VEC_INDEX(KIND, V, IDX) \
    ( VEC_index(KIND, (V), (IDX) ) )
#endif

/* Iterating over every element in a vector, or not at all if it is
   NULL: */
#if (GCC_VERSION >= 4008)
  #define GCC_COMPAT_FOR_EACH_VEC_ELT(KIND, V, IDX_VAR, ITEM_VAR) \
    if ( (V) != NULL ) \
      FOR_EACH_VEC_ELT ( (*V), (IDX_VAR), (ITEM_VAR) )
#else
  #define GCC_COMPAT_FOR_EACH_VEC_ELT(KIND, V, IDX_VAR, ITEM_VAR) \
    FOR_EACH_VEC_ELT(KIND, (V), (IDX_VAR), (ITEM_VAR) )
#endif



/*************************************************************************
 Declarations
 *************************************************************************/

/* GCC 11 renamed DECL_IS_BUILTIN to DECL_IS_UNDECLARED_BUILTIN.  */
#if (GCC_VERSION >= 11000)
  #define GCC_COMPAT_DECL_IS_BUILTIN(DECL) \
    DECL_IS_UNDECLARED_BUILTIN (DECL)
#else
  #define GCC_COMPAT_DECL_IS_BUILTIN(DECL) \
    DECL_IS_BUILTIN (DECL)
#endif


/*************************************************************************
 GIMPLE
 *************************************************************************/

/* GCC 12 removed gimple_expr_type.  The replacement lives in
   gcc-gimple.c rather than here, because this header is also included
   by files that do not include gimple.h.  */
#if (GCC_VERSION >= 12000)
  #define GCC_COMPAT_GIMPLE_EXPR_TYPE(STMT) \
    gcc_compat_gimple_expr_type (STMT)
#else
  #define GCC_COMPAT_GIMPLE_EXPR_TYPE(STMT) \
    gimple_expr_type (STMT)
#endif


/*************************************************************************
 Garbage collection
 *************************************************************************/

/* cgraph_node and varpool_node are marked through their base class, and
   which class that is has changed over time: in GCC 4.8 varpool_node
   became part of union symtab_node_def, in GCC 4.9 that union became
   class symtab_node, and GCC 16 added toplevel_node below it.  */
#if (GCC_VERSION >= 16000)
  #define GCC_COMPAT_GT_GGC_MX_CGRAPH_NODE(NODE)  gt_ggc_mx_toplevel_node (NODE)
  #define GCC_COMPAT_GT_GGC_MX_VARPOOL_NODE(NODE) gt_ggc_mx_toplevel_node (NODE)
#elif (GCC_VERSION >= 4009)
  #define GCC_COMPAT_GT_GGC_MX_CGRAPH_NODE(NODE)  gt_ggc_mx_symtab_node (NODE)
  #define GCC_COMPAT_GT_GGC_MX_VARPOOL_NODE(NODE) gt_ggc_mx_symtab_node (NODE)
#elif (GCC_VERSION >= 4008)
  #define GCC_COMPAT_GT_GGC_MX_CGRAPH_NODE(NODE)  gt_ggc_mx_cgraph_node (NODE)
  #define GCC_COMPAT_GT_GGC_MX_VARPOOL_NODE(NODE) gt_ggc_mx_symtab_node_def (NODE)
#else
  #define GCC_COMPAT_GT_GGC_MX_CGRAPH_NODE(NODE)  gt_ggc_mx_cgraph_node (NODE)
  #define GCC_COMPAT_GT_GGC_MX_VARPOOL_NODE(NODE) gt_ggc_mx_varpool_node (NODE)
#endif


/*
Local variables:
c-basic-offset: 2
indent-tabs-mode: nil
End:
*/

#endif /* INCLUDED__GCC_C_API_PRIVATE_COMPAT_H */

