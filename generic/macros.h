#ifndef FLEX_ARRAY
/*
 * See if our compiler is known to support flexible array members.
 */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && (!defined(__SUNPRO_C) || (__SUNPRO_C > 0x580))
# define FLEX_ARRAY             /* empty */
#elif defined(__GNUC__)
# if (__GNUC__ >= 3)
#  define FLEX_ARRAY            /* empty */
# else
#  define FLEX_ARRAY 0          /* older GNU extension */
# endif
#endif

/*
 * Otherwise, default to safer but a bit wasteful traditional style
 */
#ifndef FLEX_ARRAY
# define FLEX_ARRAY 1
#endif
#endif

#define FLEX_ARRAY_SIZE (FLEX_ARRAY + 0)

#define Init(INTERP, NS_PTR) \
extern int JOIN (PACKAGE, _Init) (Tcl_Interp *); \
static int JOIN (PACKAGE, _Init_) (Tcl_Interp *, Tcl_Namespace *); \
extern int \
JOIN (PACKAGE, _Init) (Tcl_Interp * INTERP) \
{ \
  Tcl_Namespace *NS_PTR; \
  if (Tcl_InitStubs ((INTERP), TCL_VERSION, 0) == NULL) \
    { \
      return TCL_ERROR; \
    } \
  NS_PTR = Tcl_CreateNamespace ((INTERP), "::" PACKAGE_NAME, NULL, NULL); \
  if (JOIN(PACKAGE, _Init_) (INTERP, NS_PTR) != TCL_OK) \
    { \
      return TCL_ERROR; \
    }; \
  Tcl_CreateEnsemble(INTERP, "::" PACKAGE_NAME, NS_PTR, TCL_ENSEMBLE_PREFIX); \
  if (Tcl_PkgProvide (INTERP, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) \
    { \
      return TCL_ERROR; \
    }; \
  return TCL_OK; \
} \
static int \
JOIN (PACKAGE, _Init_) (Tcl_Interp *INTERP, Tcl_Namespace *NS_PTR)

#ifdef Tcl_NRCallObjProc

#define CommandDecl(NAME) \
static int JOIN (PACKAGE, NAME) (ClientData, Tcl_Interp *, int, Tcl_Obj * const *); \
static int JOIN (PACKAGE, JOIN (NR, NAME)) (ClientData, Tcl_Interp *, int, Tcl_Obj * const *)

#define CommandDef(NAME, CLIENT_DATA, INTERP, OBJC, OBJV) \
static int \
JOIN (PACKAGE, NAME) (ClientData clientData, Tcl_Interp * interp, int objc, \
         Tcl_Obj * const * objv) \
{ \
  return Tcl_NRCallObjProc (interp, JOIN (PACKAGE, JOIN (NR, NAME)), clientData, objc, objv); \
} \
static int \
JOIN (PACKAGE, JOIN (NR, NAME)) (ClientData CLIENT_DATA, Tcl_Interp * INTERP, int OBJC, \
           Tcl_Obj * const OBJV[])

#define CreateCommand(INTERP, nsPtr, NAME) do { \
  Tcl_NRCreateCommand((INTERP), "::" PACKAGE_NAME "::" #NAME, JOIN (PACKAGE, NAME), JOIN (PACKAGE, JOIN (NR, NAME)), NULL, NULL); \
  if (Tcl_Export (INTERP, nsPtr, #NAME, 0) != TCL_OK) { \
    return TCL_ERROR; \
  } \
} while (0)

#else

#define CommandDecl(NAME) \
static int JOIN (PACKAGE, NAME) (ClientData, Tcl_Interp *, int, Tcl_Obj * const *)

#define CommandDef(NAME, CLIENT_DATA, INTERP, OBJC, OBJV) \
static int \
JOIN (PACKAGE, NAME) (ClientData clientData, Tcl_Interp * interp, int objc, \
         Tcl_Obj * const * objv)

#define CreateCommand(INTERP, nsPtr, NAME) do { \
  Tcl_CreateObjCommand((INTERP), "::" PACKAGE_NAME "::" #NAME, JOIN (PACKAGE, NAME), NULL, NULL); \
  if (Tcl_Export (INTERP, nsPtr, #NAME, 0) != TCL_OK) { \
    return TCL_ERROR; \
  } \
} while (0)

#endif
