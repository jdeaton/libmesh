dnl -------------------------------------------------------------
dnl Parmetis
dnl -------------------------------------------------------------
AC_DEFUN([CONFIGURE_PARMETIS],
[
  AC_ARG_ENABLE(parmetis,
                AS_HELP_STRING([--disable-parmetis],
                               [build without Parmetis parallel graph partitioning support]),
                [case "${enableval}" in
                  yes)  enableparmetis=yes ;;
                  no)  enableparmetis=no ;;
                  *)  AC_MSG_ERROR(bad value ${enableval} for --enable-parmetis) ;;
                esac],
                [enableparmetis=$enableoptional])

  dnl Trump --enable-parmetis with --disable-mpi
  if (test "x$enablempi" = xno); then
    enableparmetis=no
  fi

  dnl The PARMETIS API is distributed with libmesh, so we don't have to guess
  dnl where it might be installed...
  if (test $enableparmetis = yes); then
    dnl We always configure for Metis before ParMetis, and we will respect the value of $build_metis
    dnl determined there -- if we are using PETSc's Metis, we'll assume we are also using PETSc's ParMetis.
    if (test $build_metis = yes); then
      PARMETIS_INCLUDE="-I\$(top_srcdir)/contrib/parmetis/include"
      PARMETIS_LIB="\$(EXTERNAL_LIBDIR)/libparmetis\$(libext)"
    fi
    AC_DEFINE(HAVE_PARMETIS, 1, [Flag indicating whether the library will be compiled with Parmetis support])
    AC_MSG_RESULT(<<< Configuring library with Parmetis support >>>)
  else
    PARMETIS_INCLUDE=""
    PARMETIS_LIB=""
    enableparmetis=no
  fi

          dnl Case A: PETSc is built with both METIS and ParMETIS support. In this case we use both.
          AS_IF([test "x$build_metis" = "xpetsc" && test $petsc_have_parmetis -gt 0],
                [
                  AC_DEFINE(HAVE_PARMETIS, 1,
                            [Flag indicating whether the library will be compiled with Parmetis support])
                  build_parmetis=no
                  AC_MSG_RESULT([<<< Configuring library with PETSc Parmetis support >>>])
                ])

          dnl Case B: PETSc is built with Metis support but no
          dnl ParMETIS. We use their METIS and disable ParMETIS support to
          dnl avoid mixing.
          AS_IF([test "x$build_metis" = "xpetsc" && test $petsc_have_parmetis -eq 0],
                [
                  PARMETIS_INCLUDE=""
                  PARMETIS_LIB=""
                  build_parmetis=no
                  enableparmetis=no
                  AC_MSG_RESULT([<<< PETSc has METIS but no ParMETIS, configuring library without Parmetis support >>>])
                ])

          dnl Case C: PETSc was not built with METIS, but it was built
          dnl with ParMETIS. I'm not sure this is actually possible, but
          dnl if it is we will just build our own METIS and disable
          dnl ParMETIS support to avoid mixing.
          AS_IF([test "x$build_metis" = "xyes" && test $petsc_have_parmetis -gt 0],
                [
                  PARMETIS_INCLUDE=""
                  PARMETIS_LIB=""
                  build_parmetis=no
                  enableparmetis=no
                  AC_MSG_RESULT([<<< PETSc has no METIS but it does have ParMETIS, configuring library without Parmetis support >>>])
                ])

          dnl Case D: We're using internal or non-PETSc external
          dnl METIS. We build ParMETIS on our own.
          AS_IF([test "x$build_metis" != "xpetsc" && test $petsc_have_parmetis -eq 0],
                [
                  PARMETIS_INCLUDE="-I\$(top_srcdir)/contrib/parmetis/include"
                  PARMETIS_LIB="\$(EXTERNAL_LIBDIR)/libparmetis\$(libext)"
                  build_parmetis=yes
                  AC_DEFINE(HAVE_PARMETIS, 1,
                            [Flag indicating whether the library will be compiled with Parmetis support])
                  AC_MSG_RESULT([<<< Configuring library with internal Parmetis support >>>])
                ])
        ],
        [
          PARMETIS_INCLUDE=""
          PARMETIS_LIB=""
          enableparmetis=no
          build_parmetis=no
        ])
  AM_CONDITIONAL(BUILD_PARMETIS, test x$build_parmetis = xyes)

  AC_SUBST(PARMETIS_INCLUDE)
  AC_SUBST(PARMETIS_LIB)
])
