#ifndef NEEDFUL_MACROS_H
#define NEEDFUL_MACROS_H

/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define NF_STR_CAT_IMPL(A,B) A##B

/*
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 */
#define NF_STR_CAT(A,B) NF_STR_CAT_IMPL(A,B)

/*
 * Turn A into a string literal without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define NF_CONV_IMPL(A) #A

/*
 * Turn A into a string literal after macro-expanding it.
 */
#define NF_STR_CONV(A) NF_CONV_IMPL(A)

//----- Compile Time Messages

#define $Line NF_STR_CONV(__LINE__ )
#define MSG_INFORM __FILE__ "(" $Line ") : INFO: "
#define MSG_TODO   __FILE__ "(" $Line ") : TODO: "
#define MSG_WARN   __FILE__ "(" $Line ") : WARN: "

//-----

#endif // NEEDFUL_MACROS_H
