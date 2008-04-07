AC_INIT(mygame, 0.0.4, ken@ninkendo.org)
AM_INIT_AUTOMAKE($PACKAGE_TARNAME, $PACKAGE_VERSION)

AC_PROG_CXX
AC_PROG_INSTALL

dnl ======================
dnl Check for GL Headers
dnl ======================

AC_ARG_WITH(gl-inc,   [  --with-gl-inc=DIR       OpenGL header file location])
AC_ARG_WITH(glut-inc,   [  --with-glut-inc=DIR       GLUT toolkit header file location])

if test "x$with_gl_inc" = "x" ; then
    GL_CPPFLAGS=""
else
    GL_CPPFLAGS="-I$with_gl_inc"
fi

if test "x$with_glut_inc" = "x" ; then
    GLUT_CPPFLAGS=""
else
    GLUT_CPPFLAGS="-I$with_glut_inc"
fi

dnl check for GL/gl.h

saved_CPPFLAGS="$CPPFLAGS"

AC_DEFUN( CHECK_FOR_GL_H, [
    CPPFLAGS="$saved_CPPFLAGS $GL_CPPFLAGS"

    AC_MSG_CHECKING([for GL/gl.h])
    AC_TRY_CPP( [ #include <GL/gl.h> ], have_gl_h=yes, have_gl_h=no )
    AC_MSG_RESULT([$have_gl_h])
])

CHECK_FOR_GL_H

if test "x$have_gl_h" = "xno" ; then
    AC_MSG_ERROR([Cannot find GL/gl.h])
fi

CPPFLAGS="$saved_CPPFLAGS"


dnl check for GL/glut.h
saved_CPPFLAGS="$CPPFLAGS"

AC_DEFUN( CHECK_FOR_GLUT_H, [
    CPPFLAGS="$saved_CPPFLAGS $GLUT_CPPFLAGS $GL_CPPFLAGS"

    AC_MSG_CHECKING([for GL/glut.h])
    AC_TRY_CPP( [ #include <GL/glut.h> ], have_glut_h=yes, have_glut_h=no )
    AC_MSG_RESULT([$have_glut_h])
])

CHECK_FOR_GLUT_H

if test "x$have_glut_h" = "xno"; then
    AC_MSG_ERROR([Cannot find GL/glut.h])
fi

CPPFLAGS="$saved_CPPFLAGS"


dnl ===================
dnl Check for GL library
dnl ===================

AC_ARG_WITH(gl-libs,      [  --with-gl-libs=DIR      GL/MesaGL library location])
AC_ARG_WITH(gl-lib-name,  [  --with-gl-lib-name=NAME GL library name])

if test "x$with_gl_libs" = "x" ; then
    GL_LDOPTS=""
else
    GL_LDOPTS="-L$with_gl_libs"
fi

saved_LIBS="$LIBS"
GL_LIBS=""

AC_DEFUN( CHECK_FOR_GL_LIB, [
    AC_MSG_CHECKING([for $GL_LIB_NAME library])
    LIBS="$LIBS $GL_LDOPTS -l$GL_LIB_NAME"
    AC_TRY_LINK( , , have_GL=yes, have_GL=no)
    AC_MSG_RESULT([$have_GL])

    if test "x$have_GL" = "xyes" ; then
    GL_LIBS="$GL_LDOPTS -l$GL_LIB_NAME"

    else
    dnl Try with -lpthread

    AC_MSG_CHECKING([for $GL_LIB_NAME library (with pthreads)])
    LIBS="$LIBS $GL_LDOPTS -l$GL_LIB_NAME -lpthread"
    AC_TRY_LINK( , , have_GL=yes, have_GL=no)
    AC_MSG_RESULT([$have_GL])

    if test "x$have_GL" = "xyes" ; then
        GL_LIBS="$saved_LIBS $GL_LDOPTS -l$GL_LIB_NAME -lpthread"
    fi
    fi
])

AC_DEFUN( FIND_GL_LIB, [
    if test "x$with_gl_lib_name" = "x" ; then
        GL_LIB_NAME="GL"
    else
        GL_LIB_NAME="$with_gl_lib_name"
    fi

    CHECK_FOR_GL_LIB

    if test "x$have_GL" = "xno" -a "x$with_gl_lib_name" = "x" ; then
        GL_LIB_LIST=`grep -v -E "^$GL_LIB_NAME\$" <<EOF
GL
MesaGL
opengl32
EOF
`

        for GL_LIB_NAME in $GL_LIB_LIST ; do
            CHECK_FOR_GL_LIB

            if test "x$have_GL" = "xyes" ; then
                break;
            fi
        done
    fi
])

FIND_GL_LIB
if test "x$have_GL" = "xno" -a "x$GL_LDOPTS" = "x" ; then
    echo "*** Hmm, you don't seem to have OpenGL libraries installed in the standard"
    echo "*** location (/usr/lib).  I'll check in /usr/X11R6/lib, since"
    echo "*** many distributions (incorrectly) put OpenGL libs there."
    GL_LDOPTS="-L/usr/X11R6/lib"
    FIND_GL_LIB
fi

if test "x$have_GL" = "xno" ; then
    AC_MSG_ERROR([Cannot find GL library])
fi

LIBS="$saved_LIBS"

dnl ===================
dnl Check for GLUT toolkit
dnl ===================
AC_ARG_WITH(glut-libs,	[	--with-glut-libs=DIR	GLUT toolkit location])

if test "x$with_glut_libs" = "x" ; then
    GLUT_LDOPTS=""
else
    GLUT_LDOPTS="-L$with_glut_libs"
fi

saved_LIBS="$LIBS"
GLUT_LIBS=""

AC_DEFUN( CHECK_FOR_GLUT_LIB, [
    AC_MSG_CHECKING([for GLUT library])
    LIBS="$LIBS $GL_LDOPTS -l$GL_LIB_NAME -lglut"
    AC_TRY_LINK( , , have_GLUT=yes, have_GLUT=no)
    AC_MSG_RESULT([$have_GLUT])

    if test "x$have_GLUT" = "xyes" ; then
        GLUT_LIBS="-lglut"
    fi
])

CHECK_FOR_GLUT_LIB

if test "x$have_GLUT" = "xno" ; then
    AC_MSG_ERROR([Cannot find GLUT library])
fi

LIBS="$saved_LIBS"

LIBS="$LIBS $GL_LIBS $GLUT_LIBS"

AC_OUTPUT([Makefile src/Makefile])
