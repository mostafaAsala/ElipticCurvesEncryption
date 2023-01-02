/* --- This file is distributed under the MIT Open Source License, as detailed
   in "LICENSE.TXT" in the root of this repository --- */

#ifndef HURCHALLA_PROGRAMMING_BY_CONTRACT_H_INCLUDED
#define HURCHALLA_PROGRAMMING_BY_CONTRACT_H_INCLUDED

/*
These are the main contract assertion macros:
    HPBC_PRECONDITION(x), HPBC_PRECONDITION2(x), HPBC_PRECONDITION3(x)
    HPBC_POSTCONDITION(x), HPBC_POSTCONDITION2(x), HPBC_POSTCONDITION3(x)
    HPBC_INVARIANT(x), HPBC_INVARIANT2(x), HPBC_INVARIANT3(x)
    HPBC_ASSERT(x), HPBC_ASSERT2(x), HPBC_ASSERT3(x)
    Additionally for C++ only, HPBC_CONSTEXPR_PRECONDITION(x),
      HPBC_CONSTEXPR_POSTCONDITION(x), HPBC_CONSTEXPR_INVARIANT(x),
      HPBC_CONSTEXPR_ASSERT(x)
They are purposely implemented as macros instead of inline functions, as
described at the end of this section.

Precondition asserts are intended to check that a precondition is satisfied.
Postcondition asserts are intended to check that a postcondition is satisfied.
Invariant asserts are intended to check that invariants are true.
Plain asserts are intended to check that logical assumptions are true.

The number at the end of an assertion macro name specifies the assertion level.
The assertion macros without a number at the end (precondition, postcondition,
invariant, assumption) can be viewed as having an implicit level of 1. For
more detail:
  An unnumbered (implicit level 1) assert can be viewed as a "normal" assert
    and is the go-to assert level, useful for when you either don't care about
    levels at all, or for when an assert is unremarkable in the time it will
    take to check while the program is running. [Note that if you are able to
    perform a check at compile time, you should always prefer static_assert()
    in C++ or _Static_assert() in C, since it is always free of cost and fails
    during compilation.]
  A level 2 assert is intended for checks that you know will be unusually
    expensive to perform.
  A level 3 assert is for checks that are incredibly expensive, such as a check
    with a larger order of computational complexity than the function that
    contains it. It could be considered a "safe mode" assert.

When NDEBUG is defined in a translation unit, all of these asserts will be
  replaced with a no-op, regardless of assertion level.
When NDEBUG is undefined, each non-CONSTEXPR assert will be replaced with first
  a call to a trivial getter function [hpbcGetHandlerAssertLevel()] to determine
  the program-wide assert level. Then, this will be followed by a conditional to
  skip the assert check if your particular assert's level is greater than the
  program-wide assert level. If this occurs and you are using link-time-
  optimization, the linker will typically see that the code will always skip the
  assert, and it will remove the getter call and the conditional and the assert,
  resulting in a no-op. However, the effects of link-time-optimization depend
  upon your linker implementation.

For a given compiler, by default NDEBUG is typically defined in release builds
  and undefined in debug builds. It's a standard macro (see the C or C++
  documentation of assert()) and you can predefine it (or not) for any
  compilation project - for example by using -D in gcc or clang, or in MSVC++
  via either command line option /D or by UI setting "preprocessor definitions".

The C++ CONSTEXPR assertion macros are designed for use in constexpr functions.
  If one of these assertions is evaluated at compile-time, it will cause a
  compilation error when the asserted condition is false.  If it is evaluated
  at run-time it essentially just invokes the standard assert() macro defined in
  <cassert>.  It specifically does not check the current assert level [via
  hpbcGetHandlerAssertLevel()], because that would be impossible to do within
  a constexpr function given that hpbcGetHandlerAssertLevel is not (and is not
  meant to be) constexpr.

Note: the assertions are all purposely implemented as macros instead of inline
  functions. Implementing them as functions involves at least two unavoidable
  problems, which is very likely the reason the standard assert() from assert.h
  is also implemented as a macro. First, the assserts use the C/C++ standard
  predefined macros __FILE__ and __LINE_ to show the location of an assertion
  failure. If assertions were implemented as functions, this could not work,
  since __FILE__ and __LINE_ would provide locations in the called assert
  functions rather than the location of the assert in the caller. Second, the
  contract assertions are designed to guarantee zero overhead when NDEBUG is
  defined in a translation unit. If assertions were implemented as functions,
  the argument given to an assert could break that guarantee. For example
  consider: assert(isGood()). If isGood() has side effects, the compiler would
  always need to call it, even if a hypothetical assert function (instead of
  macro) had an empty body. Even if isGood() has no side effects, it's possible
  the compiler and linker would miss an optimization we ordinarily would expect
  to remove the isGood() call. Missed optimizations might be unlikely, but the
  lack of guarantee and the side-effects issue would make the overhead (zero?)
  of these contract asserts unpredictable in general. This behavior would also
  be inconsistent with the standard library assert() macro defined in assert.h.
Assertions implemented as macros do not have these problems. However, macros
  have the problem that they 'pollute' the namespace. If your souce code uses
  any names identitical to one of the macro names in this file, then you may
  either get errors while compiling, or bugs from the compiler silently
  replacing your name with a macro expansion. Consequently, you *must* treat all
  the assertion macro names in this file as reserved keywords throughout your
  projects. If the given assertion names here are impossible or undesirable for
  you to use, you can edit this file to change the macro names. To minimize the
  pollution aspect, the current macro names are all-caps and prefixed with
  "HPBC". Nevertheless, potential macro name collision is a danger.
*/
/*
Some ideas in this file were inspired by
 https://www.youtube.com/watch?v=1QhtXRMp3Hg
 http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/
*/

/*
Ordinarily, you shouldn't change anything in this file.
*/

#if defined(NDEBUG)
#  if 0
     /* HPBC_DO_NOTHING() is written to avoid 'unused variable' warnings during
        compilation. Note: for C++ prior to C++20, the line below will not
        compile if the arg contains a lambda. Perhaps more importantly, this
        version below doesn't seem to achieve the goal of avoiding 'unused
        variable' warnings for the Intel C/C++ compiler, or for all the recent
        MSVC compilers. The alternative version in the #else seems to be
        preferable to this version. */
#    define HPBC_DO_NOTHING(...) ((void)sizeof(__VA_ARGS__))
#  else
     /* Alternative version that will work C++ pre-20 with lambdas in contract
        args, and that will work with Intel C/C++ compiler and with MSVC */
#    ifdef _MSC_VER
#      define HPBC_DO_NOTHING(...) (false ? (void)(__VA_ARGS__) : (void)0)
#    else
#      define HPBC_DO_NOTHING(...) ((void)(true || (__VA_ARGS__)))
#    endif
//#    define HPBC_DO_NOTHING(...) (while (false) (void)(__VA_ARGS__))
//#    define HPBC_DO_NOTHING(...) do { (void)(__VA_ARGS__); } while (0, 0)
#  endif

#  define HPBC_PRECONDITION(...) HPBC_DO_NOTHING(__VA_ARGS__)
#  define HPBC_PRECONDITION2(...) HPBC_DO_NOTHING(__VA_ARGS__)
#  define HPBC_PRECONDITION3(...) HPBC_DO_NOTHING(__VA_ARGS__)
#  define HPBC_POSTCONDITION(...) HPBC_DO_NOTHING(__VA_ARGS__)
#  define HPBC_POSTCONDITION2(...) HPBC_DO_NOTHING(__VA_ARGS__)
#  define HPBC_POSTCONDITION3(...) HPBC_DO_NOTHING(__VA_ARGS__)
#  define HPBC_INVARIANT(...) HPBC_DO_NOTHING(__VA_ARGS__)
#  define HPBC_INVARIANT2(...) HPBC_DO_NOTHING(__VA_ARGS__)
#  define HPBC_INVARIANT3(...) HPBC_DO_NOTHING(__VA_ARGS__)
#  define HPBC_ASSERT(...) HPBC_DO_NOTHING(__VA_ARGS__)
#  define HPBC_ASSERT2(...) HPBC_DO_NOTHING(__VA_ARGS__)
#  define HPBC_ASSERT3(...) HPBC_DO_NOTHING(__VA_ARGS__)

#  if defined(__cplusplus)
#     define HPBC_FALSE_VALUE (false)
#  else
#     define HPBC_FALSE_VALUE (0)
#  endif
#  define HPBC_PRECONDITION_MACRO_IS_ACTIVE HPBC_FALSE_VALUE
#  define HPBC_PRECONDITION2_MACRO_IS_ACTIVE HPBC_FALSE_VALUE
#  define HPBC_PRECONDITION3_MACRO_IS_ACTIVE HPBC_FALSE_VALUE
#  define HPBC_POSTCONDITION_MACRO_IS_ACTIVE HPBC_FALSE_VALUE
#  define HPBC_POSTCONDITION2_MACRO_IS_ACTIVE HPBC_FALSE_VALUE
#  define HPBC_POSTCONDITION3_MACRO_IS_ACTIVE HPBC_FALSE_VALUE
#  define HPBC_INVARIANT_MACRO_IS_ACTIVE HPBC_FALSE_VALUE
#  define HPBC_INVARIANT2_MACRO_IS_ACTIVE HPBC_FALSE_VALUE
#  define HPBC_INVARIANT3_MACRO_IS_ACTIVE HPBC_FALSE_VALUE
#  define HPBC_ASSERT_MACRO_IS_ACTIVE HPBC_FALSE_VALUE
#  define HPBC_ASSERT2_MACRO_IS_ACTIVE HPBC_FALSE_VALUE
#  define HPBC_ASSERT3_MACRO_IS_ACTIVE HPBC_FALSE_VALUE

#else
#  if !defined(HPBC_ENABLE_FULL_FEATURES)
      /* wrap the standard library assert */
#     if defined(__cplusplus)
#        include <cassert>
#     else
#        include <assert.h>
#     endif
#     define HPBC_LEVEL_ASSERT(LEVEL, ...) assert(__VA_ARGS__)
#     define HPBC_LEVEL_ASSERT_PRE(LEVEL, ...) assert(__VA_ARGS__)
#     define HPBC_HANDLER_LEVEL 3
#     define HPBC_HANDLER_PRECOND_LEVEL 3
#  else   /* the normal Programming by Contract features */
#     include "hurchalla/util/assert_handler.h"

      /* If this is C++, we can (probably) detect if exceptions are enabled by
         checking the gcc/clang macro __EXCEPTIONS, msvc's macro _CPPUNWIND, and
         the official but not always supported C++98 macro __cpp_exceptions. */
#     if defined(__cplusplus) && (defined(__EXCEPTIONS) || defined(_CPPUNWIND) \
                        || (defined(__cpp_exceptions) && __cpp_exceptions != 0))
         // C++ with exceptions: treat an exception as a failure during assert
#        define HPBC_BASIC_ASSERT(...) do { \
                       bool assertPassed = false; \
                       try { if (__VA_ARGS__) assertPassed = true; } \
                       catch (...) {} \
                       if (!assertPassed) \
                           hpbcAssertHandler(#__VA_ARGS__, __FILE__, __LINE__);\
                   } while(0)
#     else   /* C, or C++ without exceptions */
#        define HPBC_BASIC_ASSERT(...) do { if (__VA_ARGS__) {} else  { \
               hpbcAssertHandler(#__VA_ARGS__, __FILE__, __LINE__); } } while(0)
#     endif

#     define HPBC_LEVEL_ASSERT(LEVEL, ...) do { \
                       if (hpbcGetHandlerAssertLevel() >= LEVEL) { \
                            HPBC_BASIC_ASSERT(__VA_ARGS__); } } while(0)
#     define HPBC_LEVEL_ASSERT_PRE(LEVEL, ...) do { \
                       if (hpbcGetHandlerPreconditionAssertLevel() >= LEVEL) { \
                            HPBC_BASIC_ASSERT(__VA_ARGS__); } } while(0)
#     define HPBC_HANDLER_LEVEL hpbcGetHandlerAssertLevel()
#     define HPBC_HANDLER_PRECOND_LEVEL hpbcGetHandlerPreconditionAssertLevel()
#  endif
#  define HPBC_PRECONDITION(...) HPBC_LEVEL_ASSERT_PRE(1, __VA_ARGS__)
#  define HPBC_PRECONDITION2(...) HPBC_LEVEL_ASSERT_PRE(2, __VA_ARGS__)
#  define HPBC_PRECONDITION3(...) HPBC_LEVEL_ASSERT_PRE(3, __VA_ARGS__)
#  define HPBC_POSTCONDITION(...) HPBC_LEVEL_ASSERT(1, __VA_ARGS__)
#  define HPBC_POSTCONDITION2(...) HPBC_LEVEL_ASSERT(2, __VA_ARGS__)
#  define HPBC_POSTCONDITION3(...) HPBC_LEVEL_ASSERT(3, __VA_ARGS__)
#  define HPBC_INVARIANT(...) HPBC_LEVEL_ASSERT(1, __VA_ARGS__)
#  define HPBC_INVARIANT2(...) HPBC_LEVEL_ASSERT(2, __VA_ARGS__)
#  define HPBC_INVARIANT3(...) HPBC_LEVEL_ASSERT(3, __VA_ARGS__)
#  define HPBC_ASSERT(...) HPBC_LEVEL_ASSERT(1, __VA_ARGS__)
#  define HPBC_ASSERT2(...) HPBC_LEVEL_ASSERT(2, __VA_ARGS__)
#  define HPBC_ASSERT3(...) HPBC_LEVEL_ASSERT(3, __VA_ARGS__)

#  define HPBC_PRECONDITION_MACRO_IS_ACTIVE (HPBC_HANDLER_PRECOND_LEVEL >= 1)
#  define HPBC_PRECONDITION2_MACRO_IS_ACTIVE (HPBC_HANDLER_PRECOND_LEVEL >= 2)
#  define HPBC_PRECONDITION3_MACRO_IS_ACTIVE (HPBC_HANDLER_PRECOND_LEVEL >= 3)
#  define HPBC_POSTCONDITION_MACRO_IS_ACTIVE (HPBC_HANDLER_LEVEL >= 1)
#  define HPBC_POSTCONDITION2_MACRO_IS_ACTIVE (HPBC_HANDLER_LEVEL >= 2)
#  define HPBC_POSTCONDITION3_MACRO_IS_ACTIVE (HPBC_HANDLER_LEVEL >= 3)
#  define HPBC_INVARIANT_MACRO_IS_ACTIVE (HPBC_HANDLER_LEVEL >= 1)
#  define HPBC_INVARIANT2_MACRO_IS_ACTIVE (HPBC_HANDLER_LEVEL >= 2)
#  define HPBC_INVARIANT3_MACRO_IS_ACTIVE (HPBC_HANDLER_LEVEL >= 3)
#  define HPBC_ASSERT_MACRO_IS_ACTIVE (HPBC_HANDLER_LEVEL >= 1)
#  define HPBC_ASSERT2_MACRO_IS_ACTIVE (HPBC_HANDLER_LEVEL >= 2)
#  define HPBC_ASSERT3_MACRO_IS_ACTIVE (HPBC_HANDLER_LEVEL >= 3)
#endif  /* NDEBUG */


#if defined(__cplusplus)
   // this section was adapted from the ideas in
   // https://akrzemi1.wordpress.com/2017/05/18/asserts-in-constexpr-functions/
   // https://gist.github.com/oliora/928424f7675d58fadf49c70fdba70d2f
#  include "hurchalla/util/compiler_macros.h"
#  include <cassert>
#  include <utility>
#  if defined(__GNUC__) && !defined(__clang__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  endif
   template <class L>
   void hurchalla_hpbc_forward_lambda(L&& lambda) noexcept 
   {
      std::forward<L>(lambda)();
   }
#  if defined(__GNUC__) && !defined(__clang__)
#    pragma GCC diagnostic pop
#  endif
#  ifdef _MSC_VER
#    define HPBC_CONSTEXPR_ASSERT(...) ((void)(HURCHALLA_LIKELY(__VA_ARGS__) ? \
                             (void)0 : hurchalla_hpbc_forward_lambda( \
                             [](){ assert(!#__VA_ARGS__);}), (void)0))
#  else
#    define HPBC_CONSTEXPR_ASSERT(...) ((void)(HURCHALLA_LIKELY(__VA_ARGS__) ? \
                             (void)0 : hurchalla_hpbc_forward_lambda( \
                             [](){ assert(#__VA_ARGS__ == nullptr);}), (void)0))
#  endif
#  define HPBC_CONSTEXPR_PRECONDITION(...) HPBC_CONSTEXPR_ASSERT(__VA_ARGS__)
#  define HPBC_CONSTEXPR_POSTCONDITION(...) HPBC_CONSTEXPR_ASSERT(__VA_ARGS__)
#  define HPBC_CONSTEXPR_INVARIANT(...) HPBC_CONSTEXPR_ASSERT(__VA_ARGS__)
#endif


/* For postconditions where we need to save a value at the function's start: */
/* C example:
void foo(Widget* p)
{
   int origVal;
   _Bool origHasValue = 0;
   if (HPBC_POSTCONDITION_MACRO_IS_ACTIVE) {
      origVal = p->a;
      origHasValue = 1;
   }
   ... preconditions ...

   ... function body ...   
   changeWidget(p);
   ... more function body ...

   HPBC_POSTCONDITION(origHasValue && origVal < p->a);
}
*/

/* C++ example:
   We'll normally want to use std::optional (requires C++17)
   or boost::optional (pre C++17):
void bar()
{
   std::optional<int> origVal;
   if (HPBC_POSTCONDITION_MACRO_IS_ACTIVE)
      origVal = getValue();
   ... preconditions ...
   ... function body ...
   HPBC_POSTCONDITION(origVal.has_value() && origVal < getValue());
}
*/


#if defined(NDEBUG)
#  define HPBC_INVARIANTS_CHECK(METHOD_NAME) ((void)0)
#else
#  define HPBC_INVARIANTS_CHECK(METHOD_NAME) do { \
                                    if (hpbcGetHandlerAssertLevel() > 0) { \
                                        METHOD_NAME(); \
                                    } } while(0)
#endif


#if defined(__cplusplus)
#  if defined(NDEBUG)
#     define HPBC_INVARIANTS_GUARD(INVARIANTS_METHOD_NAME) ((void)0)
#  else
#     define HPBC_INVARIANTS_GUARD(INVARIANTS_METHOD_NAME) \
         using OuterClassPtrHpbcInvariantsGuard = decltype(this); \
         class HpbcClassInvariantsGuard { \
         public: \
            explicit \
            HpbcClassInvariantsGuard(OuterClassPtrHpbcInvariantsGuard ptr) \
               : pOuter(ptr) \
            { \
               if (hpbcGetHandlerAssertLevel() > 0) \
                  pOuter->INVARIANTS_METHOD_NAME(); \
            } \
            ~HpbcClassInvariantsGuard() \
            { \
               if (hpbcGetHandlerAssertLevel() > 0) \
                  pOuter->INVARIANTS_METHOD_NAME(); \
            } \
            HpbcClassInvariantsGuard(const HpbcClassInvariantsGuard&) = delete;\
            HpbcClassInvariantsGuard& operator=(const HpbcClassInvariantsGuard&) = delete; \
         private: \
            OuterClassPtrHpbcInvariantsGuard pOuter; \
         } hpbcClassInvariantsGuardVar(this)
#  endif
#endif  /* defined(__cplusplus) */


#endif
