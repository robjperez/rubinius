#ifndef RBX_CAPI_TAG_HPP
#define RBX_CAPI_TAG_HPP

    /* VALUE and ID are defined here rather than including ruby.h to allow
     * for sharing C-API function prototypes between the VM proper and the
     * C-API code. @see vm/capi/include/ruby.h for explanations of VALUE and ID.
     */
#define VALUE intptr_t
#define ID    intptr_t

    /**
     * These values must be the same as the values given to Qfalse, etc in
     * vm/capi/include/ruby.h
     */
#define cCApiHandleQfalse         ((VALUE)(0x00))
#define cCApiHandleQtrue          ((VALUE)(0x22))
#define cCApiHandleQnil           ((VALUE)(0x42))
#define cCApiHandleQundef         ((VALUE)(0x62))

    /**
     * The C-API uses handles to refer to all Ruby objects rather than raw
     * pointers to objects. There are four categories of objects:
     *
     *   1. References (e.g. instances of Array, String, Hash, etc.)
     *   2. Fixnums
     *   3. Symbols
     *   4. "Booleans" (i.e. Qfalse, Qtrue, Qnil, Qundef)
     *
     * The handles are tagged to distinguish the categories of objects.
     *
     * The tagging scheme allows passing Symbols and Fixnums straight through
     * without changing their values. This ensures that if a C extension
     * assigns a global ID with the value of a Symbol, any subsequent handle
     * requested for that symbol (e.g. from rb_intern()) will have the same
     * value. Passing Fixnums through means that all the Fixnum conversions
     * do not have to be reimplemented for the C-API.
     *
     * The tags break down as follows (@see vm/oop.hpp for more details):
     *
     *   00 0 0000   Qfalse
     *   xx x xxx1   Fixnum
     *   xx x x110   Symbol
     *
     *   01 0 0010   Qtrue
     *   10 0 0010   Qnil
     *   11 0 0010   Qundef
     *
     * In other words, any bit pattern ending in 1 is a Fixnum, ending in 110
     * is a Symbol, ending in 0010 is a C-API "boolean" and Qfalse is 0.
     */

#define CAPI_FALSE_P(v)           ((VALUE)(v) == cCApiHandleQfalse)
#define CAPI_TRUE_P(v)            ((VALUE)(v) == cCApiHandleQtrue)
#define CAPI_NIL_P(v)             ((VALUE)(v) == cCApiHandleQnil)
#define CAPI_UNDEF_P(v)           ((VALUE)(v) == cCApiHandleQundef)

#define CAPI_REFERENCE_TAG        0x00
#define CAPI_REFERENCE_MASK       0x03

#define CAPI_REFERENCE_P(v)       ({ VALUE __v = (VALUE)(v); __v && (__v & CAPI_REFERENCE_MASK) == CAPI_REFERENCE_TAG; })

#endif
