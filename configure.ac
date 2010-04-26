AC_INIT(mygame, 0.0.5, ken@ninkendo.org)
AM_INIT_AUTOMAKE($PACKAGE_TARNAME, $PACKAGE_VERSION)

AC_PROG_CXX
AC_PROG_INSTALL
AC_LANG([C++])

dnl ======================
dnl Check for GL Headers
dnl ======================

AC_ARG_WITH(gl-inc,   [  --with-gl-inc=DIR       OpenGL header file location])
AC_ARG_WITH(glut-inc,   [  --with-glut-inc=DIR       GLUT toolkit header file location])
AC_ARG_WITH(boost-inc,   [  --with-boost-inc=DIR       Boost libraries header file location])

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

if test "x$with_boost_inc" = "x" ; then
    BOOST_CPPFLAGS=""
else
    BOOST_CPPFLAGS="-I$with_boost_inc"
fi

dnl ===================
dnl Check for GL/gl.h
dnl ===================

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


dnl ===================
dnl Check for GL/glut.h
dnl ===================

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
dnl Check for boost/thread.hpp
dnl ===================

saved_CPPFLAGS="$CPPFLAGS"

AC_DEFUN( CHECK_FOR_BOOST_THREAD_HPP, [
    CPPFLAGS="$saved_CPPFLAGS $BOOST_CPPFLAGS"

    AC_MSG_CHECKING([for boost/thread.hpp])
    AC_TRY_CPP( [ #include <boost/thread.hpp> ], have_boost_thread_hpp=yes, have_boost_thread_hpp=no )
    AC_MSG_RESULT([$have_boost_thread_hpp])
])

CHECK_FOR_BOOST_THREAD_HPP

if test "x$have_boost_thread_hpp" = "xno"; then
    AC_MSG_ERROR([Cannot find boost/thread.hpp])
fi

CPPFLAGS="$saved_CPPFLAGS $GL_CPPFLAGS $GLUT_CPPFLAGS $BOOST_CPPFLAGS"

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
OpenGL
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
AC_ARG_WITH(glut-libs,	[  --with-glut-libs=DIR	GLUT toolkit location])

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

dnl ===================
dnl Check for GLU
dnl ===================
AC_ARG_WITH(glu-libs,	[  --with-glu-libs=DIR	GLU library location])

if test "x$with_glu_libs" = "x" ; then
    GLU_LDOPTS=""
else
    GLU_LDOPTS="-L$with_glu_libs"
fi

saved_LIBS="$LIBS"
GLU_LIBS=""

AC_DEFUN( CHECK_FOR_GLU_LIB, [
    AC_MSG_CHECKING([for GLU library])
    LIBS="$LIBS $GL_LDOPTS -l$GL_LIB_NAME -lGLU"
    AC_TRY_LINK( , , have_GLU=yes, have_GLU=no)
    AC_MSG_RESULT([$have_GLU])

    if test "x$have_GLU" = "xyes" ; then
        GLU_LIBS="-lGLU"
    fi
])

CHECK_FOR_GLU_LIB

if test "x$have_GLU" = "xno" ; then
    AC_MSG_ERROR([Cannot find GLU library])
fi

LIBS="$saved_LIBS"

dnl ===================
dnl Check for pthread
dnl ===================

AC_ARG_WITH(pthread-libs,	[  --with-pthread-libs=DIR	pthread library location])

if test "x$with_pthread_libs" = "x" ; then
    PTHREAD_LDOPTS=""
else
    PTHREAD_LDOPTS="-L$with_pthread_libs"
fi

saved_LIBS="$LIBS"
PTHREAD_LIBS=""

AC_DEFUN( CHECK_FOR_PTHREAD_LIB, [
    AC_MSG_CHECKING([for libpthread])
    LIBS="$LIBS $PTHREAD_LDOPTS -lpthread"
    AC_TRY_LINK( , , have_PTHREAD=yes, have_PTHREAD=no)
    AC_MSG_RESULT([$have_PTHREAD])

    if test "x$have_PTHREAD" = "xyes" ; then
        PTHREAD_LIBS="$PTHREAD_LDOPTS -lpthread"
    fi
])

CHECK_FOR_PTHREAD_LIB

if test "x$have_PTHREAD" = "xno" ; then
    AC_MSG_ERROR([Cannot find libpthread])
fi

dnl ===================
dnl Check for Boost
dnl ===================

AC_ARG_WITH(boost-libs,	[  --with-boost-libs=DIR	Boost libraries location])

if test "x$with_boost_libs" = "x" ; then
    BOOST_LDOPTS=""
else
    BOOST_LDOPTS="-L$with_boost_libs"
fi

saved_LIBS="$LIBS"
BOOST_LIBS=""

AC_DEFUN( CHECK_FOR_BOOST_LIB, [
    AC_MSG_CHECKING([for Boost library (with -mt)])
    LIBS="$LIBS $BOOST_LDOPTS -lboost_thread-mt"
    AC_TRY_LINK( , , have_BOOSTmt=yes, have_BOOSTmt=no)
    AC_MSG_RESULT([$have_BOOSTmt])

    if test "x$have_BOOSTmt" = "xyes" ; then
        BOOST_LIBS="$BOOST_LDOPTS -lboost_thread-mt"
    fi

	dnl in case -mt isn't available
	if test "x$have_BOOSTmt" = "xno" ; then

		LIBS="$saved_LIBS"
		AC_MSG_CHECKING([for Boost library (without -mt)])
		LIBS="$LIBS $BOOST_LDOPTS -lboost_thread"
		AC_TRY_LINK( , , have_BOOST=yes, have_BOOST=no)
		AC_MSG_RESULT([$have_BOOST])

		if test "x$have_BOOST" = "xyes" ; then
			BOOST_LIBS="$BOOST_LDOPTS -lboost_thread"
		fi
	fi
])

CHECK_FOR_BOOST_LIB

if test "x$have_BOOST" = "xno" ; then
    AC_MSG_ERROR([Cannot find Boost library])
fi

LIBS="$saved_LIBS"

LIBS="$LIBS $GL_LIBS $GLU_LIBS $GLUT_LIBS $BOOST_LIBS $PTHREAD_LIBS"

AC_OUTPUT([Makefile src/Makefile])
