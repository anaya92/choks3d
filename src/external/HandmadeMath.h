/*
  HandmadeMath.h v1.13.0

  This is a single header file with a bunch of useful functions for game and
  graphics math operations.

  =============================================================================

  To disable SSE intrinsics, you MUST

  #define HANDMADE_MATH_NO_SSE

  in EXACTLY one C or C++ file that includes this header, BEFORE the
  include, like this:

     #define HANDMADE_MATH_NO_SSE
     #include "HandmadeMath.h"

  =============================================================================

  If you would prefer not to use the HMM_ prefix on function names, you can

  #define HMM_PREFIX

  To use a custom prefix instead, you can

  #define HMM_PREFIX(name) YOUR_PREFIX_##name

  =============================================================================

  To use HandmadeMath without the CRT, you MUST

     #define HMM_SINF MySinF
     #define HMM_COSF MyCosF
     #define HMM_TANF MyTanF
     #define HMM_SQRTF MySqrtF
     #define HMM_EXPF MyExpF
     #define HMM_LOGF MyLogF
     #define HMM_ACOSF MyACosF
     #define HMM_ATANF MyATanF
     #define HMM_ATAN2F MYATan2F

  Provide your own implementations of SinF, CosF, TanF, ACosF, ATanF, ATan2F,
  ExpF, and LogF in EXACTLY one C or C++ file that includes this header,
  BEFORE the include, like this:

     #define HMM_SINF MySinF
     #define HMM_COSF MyCosF
     #define HMM_TANF MyTanF
     #define HMM_SQRTF MySqrtF
     #define HMM_EXPF MyExpF
     #define HMM_LOGF MyLogF
     #define HMM_ACOSF MyACosF
     #define HMM_ATANF MyATanF
     #define HMM_ATAN2F MyATan2F
     #include "HandmadeMath.h"

  If you do not define all of these, HandmadeMath.h will use the
  versions of these functions that are provided by the CRT.

  =============================================================================

  LICENSE

  This software is in the public domain. Where that dedication is not
  recognized, you are granted a perpetual, irrevocable license to copy,
  distribute, and modify this file as you see fit.

  CREDITS

  Written by Zakary Strange (strangezak@protonmail.com && @strangezak)

  Functionality:
   Matt Mascarenhas (@miblo_)
   Aleph
   FieryDrake (@fierydrake)
   Gingerbill (@TheGingerBill)
   Ben Visness (@bvisness)
   Trinton Bullard (@Peliex_Dev)
   @AntonDan

  Fixes:
   Jeroen van Rijn (@J_vanRijn)
   Kiljacken (@Kiljacken)
   Insofaras (@insofaras)
   Daniel Gibson (@DanielGibson)
*/

// Dummy macros for when test framework is not present.
#ifndef COVERAGE
#define COVERAGE(a, b)
#endif

#ifndef ASSERT_COVERED
#define ASSERT_COVERED(a)
#endif

/* let's figure out if SSE is really available (unless disabled anyway)
   (it isn't on non-x86/x86_64 platforms or even x86 without explicit SSE support)
   => only use "#ifdef HANDMADE_MATH__USE_SSE" to check for SSE support below this block! */
#ifndef HANDMADE_MATH_NO_SSE

# ifdef _MSC_VER
   /* MSVC supports SSE in amd64 mode or _M_IX86_FP >= 1 (2 means SSE2) */
#  if defined(_M_AMD64) || ( defined(_M_IX86_FP) && _M_IX86_FP >= 1 )
#   define HANDMADE_MATH__USE_SSE 1
#  endif
# else /* not MSVC, probably GCC, clang, icc or something that doesn't support SSE anyway */
#  ifdef __SSE__ /* they #define __SSE__ if it's supported */
#   define HANDMADE_MATH__USE_SSE 1
#  endif /*  __SSE__ */
# endif /* not _MSC_VER */

#endif /* #ifndef HANDMADE_MATH_NO_SSE */

#ifdef HANDMADE_MATH__USE_SSE
#include <xmmintrin.h>
#endif

#ifndef HANDMADE_MATH_H
#define HANDMADE_MATH_H

#ifdef _MSC_VER
#pragma warning(disable:4201)
#endif

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#if (defined(__GNUC__) && (__GNUC__ == 4 && __GNUC_MINOR__ < 8)) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wmissing-braces"
#endif
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#endif

#if defined(__GNUC__) || defined(__clang__)
#define HMM_DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define HMM_DEPRECATED(msg) __declspec(deprecated(msg))
#else
#define HMM_DEPRECATED(msg)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define HMM_INLINE static inline

#if !defined(HMM_SINF) || !defined(HMM_COSF) || !defined(HMM_TANF) || \
    !defined(HMM_SQRTF) || !defined(HMM_EXPF) || !defined(HMM_LOGF) || \
    !defined(HMM_ACOSF) || !defined(HMM_ATANF)|| !defined(HMM_ATAN2F)
#include <math.h>
#endif

#ifndef HMM_SINF
#define HMM_SINF sinf
#endif

#ifndef HMM_COSF
#define HMM_COSF cosf
#endif

#ifndef HMM_TANF
#define HMM_TANF tanf
#endif

#ifndef HMM_SQRTF
#define HMM_SQRTF sqrtf
#endif

#ifndef HMM_EXPF
#define HMM_EXPF expf
#endif

#ifndef HMM_LOGF
#define HMM_LOGF logf
#endif

#ifndef HMM_ACOSF
#define HMM_ACOSF acosf
#endif

#ifndef HMM_ATANF
#define HMM_ATANF atanf
#endif

#ifndef HMM_ATAN2F
#define HMM_ATAN2F atan2f
#endif

#define HMM_PI32 3.14159265359f
#define HMM_PI 3.14159265358979323846

#define HMM_MIN(a, b) ((a) > (b) ? (b) : (a))
#define HMM_MAX(a, b) ((a) < (b) ? (b) : (a))
#define HMM_ABS(a) ((a) > 0 ? (a) : -(a))
#define HMM_MOD(a, m) (((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m)))
#define HMM_SQUARE(x) ((x) * (x))

#ifndef HMM_PREFIX
#define HMM_PREFIX(name) HMM_##name
#endif

typedef union vec2_t
{
    struct
    {
        float x, y;
    };

    struct
    {
        float u, v;
    };

    struct
    {
        float left, right;
    };

    struct
    {
        float width, height;
    };

    float elements[2];

#ifdef __cplusplus
    inline float &operator[](const int &Index)
    {
        return elements[Index];
    }
#endif
} vec2_t;

typedef union vec3_t
{
    struct
    {
        float x, y, z;
    };

    struct
    {
        float u, v, w;
    };

    struct
    {
        float r, g, b;
    };

    struct
    {
        vec2_t xy;
        float ignored0_;
    };

    struct
    {
        float ignored1_;
        vec2_t yz;
    };

    struct
    {
        vec2_t uv;
        float ignored2_;
    };

    struct
    {
        float ignored3_;
        vec2_t vw;
    };

    float elements[3];

#ifdef __cplusplus
    inline float &operator[](const int &Index)
    {
        return elements[Index];
    }
#endif
} vec3_t;

typedef union vec4_t
{
    struct
    {
        union
        {
            vec3_t xyz;
            struct
            {
                float x, y, z;
            };
        };

        float w;
    };
    struct
    {
        union
        {
            vec3_t rgb;
            struct
            {
                float r, g, b;
            };
        };

        float a;
    };

    struct
    {
        vec2_t xy;
        float ignored0_;
        float ignored1_;
    };

    struct
    {
        float ignored2_;
        vec2_t yz;
        float ignored3_;
    };

    struct
    {
        float Ignored4_;
        float Ignored5_;
        vec2_t zw;
    };

    float elements[4];

#ifdef HANDMADE_MATH__USE_SSE
    __m128 _internal_sse;
#endif

#ifdef __cplusplus
    inline float &operator[](const int &Index)
    {
        return elements[Index];
    }
#endif
} vec4_t;

typedef union mat4_t
{
    float elements[4][4];

#ifdef HANDMADE_MATH__USE_SSE
    __m128 columns[4];

    HMM_DEPRECATED("Our matrices are column-major, so this was named incorrectly. Use Columns instead.")
    __m128 rows[4];
#endif

#ifdef __cplusplus
    inline vec4_t operator[](const int &Index)
    {
        vec4_t Result;
        float* Column = elements[Index];
        

        Result.elements[0] = Column[0];
        Result.elements[1] = Column[1];
        Result.elements[2] = Column[2];
        Result.elements[3] = Column[3];

        return Result;
    }
#endif
} mat4_t;

typedef union quaternion_t
{
    struct
    {
        union
        {
            vec3_t xyz;
            struct
            {
                float x, y, z;
            };
        };

        float w;
    };

    float elements[4];

#ifdef HANDMADE_MATH__USE_SSE
    __m128 _internal_sse;
#endif
} quaternion_t;

typedef signed int hmm_bool;

/*
 * Floating-point math functions
 */

COVERAGE(HMM_SinF, 1)
HMM_INLINE float HMM_PREFIX(SinF)(float Radians)
{
    ASSERT_COVERED(HMM_SinF);

    float Result = HMM_SINF(Radians);

    return (Result);
}

COVERAGE(HMM_CosF, 1)
HMM_INLINE float HMM_PREFIX(CosF)(float Radians)
{
    ASSERT_COVERED(HMM_CosF);

    float Result = HMM_COSF(Radians);

    return (Result);
}

COVERAGE(HMM_TanF, 1)
HMM_INLINE float HMM_PREFIX(TanF)(float Radians)
{
    ASSERT_COVERED(HMM_TanF);

    float Result = HMM_TANF(Radians);

    return (Result);
}

COVERAGE(HMM_ACosF, 1)
HMM_INLINE float HMM_PREFIX(ACosF)(float Radians)
{
    ASSERT_COVERED(HMM_ACosF);

    float Result = HMM_ACOSF(Radians);

    return (Result);
}

COVERAGE(HMM_ATanF, 1)
HMM_INLINE float HMM_PREFIX(ATanF)(float Radians)
{
    ASSERT_COVERED(HMM_ATanF);

    float Result = HMM_ATANF(Radians);

    return (Result);
}

COVERAGE(HMM_ATan2F, 1)
HMM_INLINE float HMM_PREFIX(ATan2F)(float left, float right)
{
    ASSERT_COVERED(HMM_ATan2F);

    float Result = HMM_ATAN2F(left, right);

    return (Result);
}

COVERAGE(HMM_ExpF, 1)
HMM_INLINE float HMM_PREFIX(ExpF)(float Float)
{
    ASSERT_COVERED(HMM_ExpF);

    float Result = HMM_EXPF(Float);

    return (Result);
}

COVERAGE(HMM_LogF, 1)
HMM_INLINE float HMM_PREFIX(LogF)(float Float)
{
    ASSERT_COVERED(HMM_LogF);

    float Result = HMM_LOGF(Float);

    return (Result);
}

COVERAGE(HMM_SquareRootF, 1)
HMM_INLINE float HMM_PREFIX(SquareRootF)(float Float)
{
    ASSERT_COVERED(HMM_SquareRootF);

    float Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 In = _mm_set_ss(Float);
    __m128 Out = _mm_sqrt_ss(In);
    Result = _mm_cvtss_f32(Out);
#else
    Result = HMM_SQRTF(Float);
#endif

    return(Result);
}

COVERAGE(HMM_RSquareRootF, 1)
HMM_INLINE float HMM_PREFIX(RSquareRootF)(float Float)
{
    ASSERT_COVERED(HMM_RSquareRootF);

    float Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 In = _mm_set_ss(Float);
    __m128 Out = _mm_rsqrt_ss(In);
    Result = _mm_cvtss_f32(Out);
#else
    Result = 1.0f/HMM_PREFIX(SquareRootF)(Float);
#endif

    return(Result);
}

COVERAGE(HMM_Power, 2)
HMM_INLINE float HMM_PREFIX(Power)(float Base, int Exponent)
{
    ASSERT_COVERED(HMM_Power);

    float Result = 1.0f;
    float Mul = Exponent < 0 ? 1.f / Base : Base;
    int x = Exponent < 0 ? -Exponent : Exponent;
    while (x)
    {
        if (x & 1)
        {
            ASSERT_COVERED(HMM_Power);

            Result *= Mul;
        }

        Mul *= Mul;
        x >>= 1;
    }

    return (Result);
}

COVERAGE(HMM_PowerF, 1)
HMM_INLINE float HMM_PREFIX(PowerF)(float Base, float Exponent)
{
    ASSERT_COVERED(HMM_PowerF);

    float Result = HMM_EXPF(Exponent * HMM_LOGF(Base));

    return (Result);
}


/*
 * Utility functions
 */

COVERAGE(HMM_ToRadians, 1)
HMM_INLINE float HMM_PREFIX(ToRadians)(float Degrees)
{
    ASSERT_COVERED(HMM_ToRadians);

    float Result = Degrees * (HMM_PI32 / 180.0f);

    return (Result);
}

COVERAGE(HMM_Lerp, 1)
HMM_INLINE float HMM_PREFIX(Lerp)(float a, float Time, float b)
{
    ASSERT_COVERED(HMM_Lerp);

    float Result = (1.0f - Time) * a + Time * b;

    return (Result);
}

COVERAGE(HMM_Clamp, 1)
HMM_INLINE float HMM_PREFIX(Clamp)(float Min, float Value, float Max)
{
    ASSERT_COVERED(HMM_Clamp);

    float Result = Value;

    if(Result < Min)
    {
        Result = Min;
    }

    if(Result > Max)
    {
        Result = Max;
    }

    return (Result);
}


/*
 * Vector initialization
 */

COVERAGE(HMM_Vec2, 1)
HMM_INLINE vec2_t HMM_PREFIX(Vec2)(float x, float y)
{
    ASSERT_COVERED(HMM_Vec2);

    vec2_t Result;

    Result.x = x;
    Result.y = y;

    return (Result);
}

COVERAGE(HMM_Vec2i, 1)
HMM_INLINE vec2_t HMM_PREFIX(Vec2i)(int x, int y)
{
    ASSERT_COVERED(HMM_Vec2i);

    vec2_t Result;

    Result.x = (float)x;
    Result.y = (float)y;

    return (Result);
}

COVERAGE(HMM_Vec3, 1)
HMM_INLINE vec3_t HMM_PREFIX(Vec3)(float x, float y, float z)
{
    ASSERT_COVERED(HMM_Vec3);

    vec3_t Result;

    Result.x = x;
    Result.y = y;
    Result.z = z;

    return (Result);
}

COVERAGE(HMM_Vec3i, 1)
HMM_INLINE vec3_t HMM_PREFIX(Vec3i)(int x, int y, int z)
{
    ASSERT_COVERED(HMM_Vec3i);

    vec3_t Result;

    Result.x = (float)x;
    Result.y = (float)y;
    Result.z = (float)z;

    return (Result);
}

COVERAGE(HMM_Vec4, 1)
HMM_INLINE vec4_t HMM_PREFIX(Vec4)(float x, float y, float z, float w)
{
    ASSERT_COVERED(HMM_Vec4);

    vec4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result._internal_sse = _mm_setr_ps(x, y, z, w);
#else
    Result.x = x;
    Result.y = y;
    Result.z = z;
    Result.w = w;
#endif

    return (Result);
}

COVERAGE(HMM_Vec4i, 1)
HMM_INLINE vec4_t HMM_PREFIX(Vec4i)(int x, int y, int z, int w)
{
    ASSERT_COVERED(HMM_Vec4i);

    vec4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result._internal_sse = _mm_setr_ps((float)x, (float)y, (float)z, (float)w);
#else
    Result.x = (float)x;
    Result.y = (float)y;
    Result.z = (float)z;
    Result.w = (float)w;
#endif

    return (Result);
}

COVERAGE(HMM_Vec4v, 1)
HMM_INLINE vec4_t HMM_PREFIX(Vec4v)(vec3_t Vector, float w)
{
    ASSERT_COVERED(HMM_Vec4v);

    vec4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result._internal_sse = _mm_setr_ps(Vector.x, Vector.y, Vector.z, w);
#else
    Result.xyz = Vector;
    Result.w = w;
#endif

    return (Result);
}


/*
 * Binary vector operations
 */

COVERAGE(HMM_AddVec2, 1)
HMM_INLINE vec2_t HMM_PREFIX(AddVec2)(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_AddVec2);

    vec2_t Result;

    Result.x = left.x + right.x;
    Result.y = left.y + right.y;

    return (Result);
}

COVERAGE(HMM_AddVec3, 1)
HMM_INLINE vec3_t HMM_PREFIX(AddVec3)(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_AddVec3);

    vec3_t Result;

    Result.x = left.x + right.x;
    Result.y = left.y + right.y;
    Result.z = left.z + right.z;

    return (Result);
}

COVERAGE(HMM_AddVec4, 1)
HMM_INLINE vec4_t HMM_PREFIX(AddVec4)(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_AddVec4);

    vec4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result._internal_sse = _mm_add_ps(left._internal_sse, right._internal_sse);
#else
    Result.x = left.x + right.x;
    Result.y = left.y + right.y;
    Result.z = left.z + right.z;
    Result.w = left.w + right.w;
#endif

    return (Result);
}

COVERAGE(HMM_SubtractVec2, 1)
HMM_INLINE vec2_t HMM_PREFIX(SubtractVec2)(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_SubtractVec2);

    vec2_t Result;

    Result.x = left.x - right.x;
    Result.y = left.y - right.y;

    return (Result);
}

COVERAGE(HMM_SubtractVec3, 1)
HMM_INLINE vec3_t HMM_PREFIX(SubtractVec3)(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_SubtractVec3);

    vec3_t Result;

    Result.x = left.x - right.x;
    Result.y = left.y - right.y;
    Result.z = left.z - right.z;

    return (Result);
}

COVERAGE(HMM_SubtractVec4, 1)
HMM_INLINE vec4_t HMM_PREFIX(SubtractVec4)(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_SubtractVec4);

    vec4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result._internal_sse = _mm_sub_ps(left._internal_sse, right._internal_sse);
#else
    Result.x = left.x - right.x;
    Result.y = left.y - right.y;
    Result.z = left.z - right.z;
    Result.w = left.w - right.w;
#endif

    return (Result);
}

COVERAGE(HMM_MultiplyVec2, 1)
HMM_INLINE vec2_t HMM_PREFIX(MultiplyVec2)(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec2);

    vec2_t Result;

    Result.x = left.x * right.x;
    Result.y = left.y * right.y;

    return (Result);
}

COVERAGE(HMM_MultiplyVec2f, 1)
HMM_INLINE vec2_t HMM_PREFIX(MultiplyVec2f)(vec2_t left, float right)
{
    ASSERT_COVERED(HMM_MultiplyVec2f);

    vec2_t Result;

    Result.x = left.x * right;
    Result.y = left.y * right;

    return (Result);
}

COVERAGE(HMM_MultiplyVec3, 1)
HMM_INLINE vec3_t HMM_PREFIX(MultiplyVec3)(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec3);

    vec3_t Result;

    Result.x = left.x * right.x;
    Result.y = left.y * right.y;
    Result.z = left.z * right.z;

    return (Result);
}

COVERAGE(HMM_MultiplyVec3f, 1)
HMM_INLINE vec3_t HMM_PREFIX(MultiplyVec3f)(vec3_t left, float right)
{
    ASSERT_COVERED(HMM_MultiplyVec3f);

    vec3_t Result;

    Result.x = left.x * right;
    Result.y = left.y * right;
    Result.z = left.z * right;

    return (Result);
}

COVERAGE(HMM_MultiplyVec4, 1)
HMM_INLINE vec4_t HMM_PREFIX(MultiplyVec4)(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec4);

    vec4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result._internal_sse = _mm_mul_ps(left._internal_sse, right._internal_sse);
#else
    Result.x = left.x * right.x;
    Result.y = left.y * right.y;
    Result.z = left.z * right.z;
    Result.w = left.w * right.w;
#endif

    return (Result);
}

COVERAGE(HMM_MultiplyVec4f, 1)
HMM_INLINE vec4_t HMM_PREFIX(MultiplyVec4f)(vec4_t left, float right)
{
    ASSERT_COVERED(HMM_MultiplyVec4f);

    vec4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Scalar = _mm_set1_ps(right);
    Result._internal_sse = _mm_mul_ps(left._internal_sse, Scalar);
#else
    Result.x = left.x * right;
    Result.y = left.y * right;
    Result.z = left.z * right;
    Result.w = left.w * right;
#endif

    return (Result);
}

COVERAGE(HMM_DivideVec2, 1)
HMM_INLINE vec2_t HMM_PREFIX(DivideVec2)(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_DivideVec2);

    vec2_t Result;

    Result.x = left.x / right.x;
    Result.y = left.y / right.y;

    return (Result);
}

COVERAGE(HMM_DivideVec2f, 1)
HMM_INLINE vec2_t HMM_PREFIX(DivideVec2f)(vec2_t left, float right)
{
    ASSERT_COVERED(HMM_DivideVec2f);

    vec2_t Result;

    Result.x = left.x / right;
    Result.y = left.y / right;

    return (Result);
}

COVERAGE(HMM_DivideVec3, 1)
HMM_INLINE vec3_t HMM_PREFIX(DivideVec3)(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_DivideVec3);

    vec3_t Result;

    Result.x = left.x / right.x;
    Result.y = left.y / right.y;
    Result.z = left.z / right.z;

    return (Result);
}

COVERAGE(HMM_DivideVec3f, 1)
HMM_INLINE vec3_t HMM_PREFIX(DivideVec3f)(vec3_t left, float right)
{
    ASSERT_COVERED(HMM_DivideVec3f);

    vec3_t Result;

    Result.x = left.x / right;
    Result.y = left.y / right;
    Result.z = left.z / right;

    return (Result);
}

COVERAGE(HMM_DivideVec4, 1)
HMM_INLINE vec4_t HMM_PREFIX(DivideVec4)(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_DivideVec4);

    vec4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result._internal_sse = _mm_div_ps(left._internal_sse, right._internal_sse);
#else
    Result.x = left.x / right.x;
    Result.y = left.y / right.y;
    Result.z = left.z / right.z;
    Result.w = left.w / right.w;
#endif

    return (Result);
}

COVERAGE(HMM_DivideVec4f, 1)
HMM_INLINE vec4_t HMM_PREFIX(DivideVec4f)(vec4_t left, float right)
{
    ASSERT_COVERED(HMM_DivideVec4f);

    vec4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Scalar = _mm_set1_ps(right);
    Result._internal_sse = _mm_div_ps(left._internal_sse, Scalar);
#else
    Result.x = left.x / right;
    Result.y = left.y / right;
    Result.z = left.z / right;
    Result.w = left.w / right;
#endif

    return (Result);
}

COVERAGE(HMM_EqualsVec2, 1)
HMM_INLINE hmm_bool HMM_PREFIX(EqualsVec2)(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_EqualsVec2);

    hmm_bool Result = (left.x == right.x && left.y == right.y);

    return (Result);
}

COVERAGE(HMM_EqualsVec3, 1)
HMM_INLINE hmm_bool HMM_PREFIX(EqualsVec3)(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_EqualsVec3);

    hmm_bool Result = (left.x == right.x && left.y == right.y && left.z == right.z);

    return (Result);
}

COVERAGE(HMM_EqualsVec4, 1)
HMM_INLINE hmm_bool HMM_PREFIX(EqualsVec4)(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_EqualsVec4);

    hmm_bool Result = (left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w);

    return (Result);
}

COVERAGE(HMM_DotVec2, 1)
HMM_INLINE float HMM_PREFIX(DotVec2)(vec2_t VecOne, vec2_t VecTwo)
{
    ASSERT_COVERED(HMM_DotVec2);

    float Result = (VecOne.x * VecTwo.x) + (VecOne.y * VecTwo.y);

    return (Result);
}

COVERAGE(HMM_DotVec3, 1)
HMM_INLINE float HMM_PREFIX(DotVec3)(vec3_t VecOne, vec3_t VecTwo)
{
    ASSERT_COVERED(HMM_DotVec3);

    float Result = (VecOne.x * VecTwo.x) + (VecOne.y * VecTwo.y) + (VecOne.z * VecTwo.z);

    return (Result);
}

COVERAGE(HMM_DotVec4, 1)
HMM_INLINE float HMM_PREFIX(DotVec4)(vec4_t VecOne, vec4_t VecTwo)
{
    ASSERT_COVERED(HMM_DotVec4);

    float Result;

    // NOTE(zak): IN the future if we wanna check what version SSE is support
    // we can use _mm_dp_ps (4.3) but for now we will use the old way.
    // Or a r = _mm_mul_ps(v1, v2), r = _mm_hadd_ps(r, r), r = _mm_hadd_ps(r, r) for SSE3
#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEResultOne = _mm_mul_ps(VecOne._internal_sse, VecTwo._internal_sse);
    __m128 SSEResultTwo = _mm_shuffle_ps(SSEResultOne, SSEResultOne, _MM_SHUFFLE(2, 3, 0, 1));
    SSEResultOne = _mm_add_ps(SSEResultOne, SSEResultTwo);
    SSEResultTwo = _mm_shuffle_ps(SSEResultOne, SSEResultOne, _MM_SHUFFLE(0, 1, 2, 3));
    SSEResultOne = _mm_add_ps(SSEResultOne, SSEResultTwo);
    _mm_store_ss(&Result, SSEResultOne);
#else
    Result = (VecOne.x * VecTwo.x) + (VecOne.y * VecTwo.y) + (VecOne.z * VecTwo.z) + (VecOne.w * VecTwo.w);
#endif

    return (Result);
}

COVERAGE(HMM_Cross, 1)
HMM_INLINE vec3_t HMM_PREFIX(Cross)(vec3_t VecOne, vec3_t VecTwo)
{
    ASSERT_COVERED(HMM_Cross);

    vec3_t Result;

    Result.x = (VecOne.y * VecTwo.z) - (VecOne.z * VecTwo.y);
    Result.y = (VecOne.z * VecTwo.x) - (VecOne.x * VecTwo.z);
    Result.z = (VecOne.x * VecTwo.y) - (VecOne.y * VecTwo.x);

    return (Result);
}


/*
 * Unary vector operations
 */

COVERAGE(HMM_LengthSquaredVec2, 1)
HMM_INLINE float HMM_PREFIX(LengthSquaredVec2)(vec2_t a)
{
    ASSERT_COVERED(HMM_LengthSquaredVec2);

    float Result = HMM_PREFIX(DotVec2)(a, a);

    return (Result);
}

COVERAGE(HMM_LengthSquaredVec3, 1)
HMM_INLINE float HMM_PREFIX(LengthSquaredVec3)(vec3_t a)
{
    ASSERT_COVERED(HMM_LengthSquaredVec3);

    float Result = HMM_PREFIX(DotVec3)(a, a);

    return (Result);
}

COVERAGE(HMM_LengthSquaredVec4, 1)
HMM_INLINE float HMM_PREFIX(LengthSquaredVec4)(vec4_t a)
{
    ASSERT_COVERED(HMM_LengthSquaredVec4);

    float Result = HMM_PREFIX(DotVec4)(a, a);

    return (Result);
}

COVERAGE(HMM_LengthVec2, 1)
HMM_INLINE float HMM_PREFIX(LengthVec2)(vec2_t a)
{
    ASSERT_COVERED(HMM_LengthVec2);

    float Result = HMM_PREFIX(SquareRootF)(HMM_PREFIX(LengthSquaredVec2)(a));

    return (Result);
}

COVERAGE(HMM_LengthVec3, 1)
HMM_INLINE float HMM_PREFIX(LengthVec3)(vec3_t a)
{
    ASSERT_COVERED(HMM_LengthVec3);

    float Result = HMM_PREFIX(SquareRootF)(HMM_PREFIX(LengthSquaredVec3)(a));

    return (Result);
}

COVERAGE(HMM_LengthVec4, 1)
HMM_INLINE float HMM_PREFIX(LengthVec4)(vec4_t a)
{
    ASSERT_COVERED(HMM_LengthVec4);

    float Result = HMM_PREFIX(SquareRootF)(HMM_PREFIX(LengthSquaredVec4)(a));

    return(Result);
}

COVERAGE(HMM_NormalizeVec2, 2)
HMM_INLINE vec2_t HMM_PREFIX(NormalizeVec2)(vec2_t a)
{
    ASSERT_COVERED(HMM_NormalizeVec2);

    vec2_t Result = {0};

    float VectorLength = HMM_PREFIX(LengthVec2)(a);

    /* NOTE(kiljacken): We need a zero check to not divide-by-zero */
    if (VectorLength != 0.0f)
    {
        ASSERT_COVERED(HMM_NormalizeVec2);

        Result.x = a.x * (1.0f / VectorLength);
        Result.y = a.y * (1.0f / VectorLength);
    }

    return (Result);
}

COVERAGE(HMM_NormalizeVec3, 2)
HMM_INLINE vec3_t HMM_PREFIX(NormalizeVec3)(vec3_t a)
{
    ASSERT_COVERED(HMM_NormalizeVec3);

    vec3_t Result = {0};

    float VectorLength = HMM_PREFIX(LengthVec3)(a);

    /* NOTE(kiljacken): We need a zero check to not divide-by-zero */
    if (VectorLength != 0.0f)
    {
        ASSERT_COVERED(HMM_NormalizeVec3);

        Result.x = a.x * (1.0f / VectorLength);
        Result.y = a.y * (1.0f / VectorLength);
        Result.z = a.z * (1.0f / VectorLength);
    }

    return (Result);
}

COVERAGE(HMM_NormalizeVec4, 2)
HMM_INLINE vec4_t HMM_PREFIX(NormalizeVec4)(vec4_t a)
{
    ASSERT_COVERED(HMM_NormalizeVec4);

    vec4_t Result = {0};

    float VectorLength = HMM_PREFIX(LengthVec4)(a);

    /* NOTE(kiljacken): We need a zero check to not divide-by-zero */
    if (VectorLength != 0.0f)
    {
        ASSERT_COVERED(HMM_NormalizeVec4);

        float Multiplier = 1.0f / VectorLength;

#ifdef HANDMADE_MATH__USE_SSE
        __m128 SSEMultiplier = _mm_set1_ps(Multiplier);
        Result._internal_sse = _mm_mul_ps(a._internal_sse, SSEMultiplier);
#else
        Result.x = a.x * Multiplier;
        Result.y = a.y * Multiplier;
        Result.z = a.z * Multiplier;
        Result.w = a.w * Multiplier;
#endif
    }

    return (Result);
}

COVERAGE(HMM_FastNormalizeVec2, 1)
HMM_INLINE vec2_t HMM_PREFIX(FastNormalizeVec2)(vec2_t a)
{
    ASSERT_COVERED(HMM_FastNormalizeVec2);

    return HMM_PREFIX(MultiplyVec2f)(a, HMM_PREFIX(RSquareRootF)(HMM_PREFIX(DotVec2)(a, a)));
}

COVERAGE(HMM_FastNormalizeVec3, 1)
HMM_INLINE vec3_t HMM_PREFIX(FastNormalizeVec3)(vec3_t a)
{
    ASSERT_COVERED(HMM_FastNormalizeVec3);

    return HMM_PREFIX(MultiplyVec3f)(a, HMM_PREFIX(RSquareRootF)(HMM_PREFIX(DotVec3)(a, a)));
}

COVERAGE(HMM_FastNormalizeVec4, 1)
HMM_INLINE vec4_t HMM_PREFIX(FastNormalizeVec4)(vec4_t a)
{
    ASSERT_COVERED(HMM_FastNormalizeVec4);

    return HMM_PREFIX(MultiplyVec4f)(a, HMM_PREFIX(RSquareRootF)(HMM_PREFIX(DotVec4)(a, a)));
}


/*
 * SSE stuff
 */

#ifdef HANDMADE_MATH__USE_SSE
COVERAGE(HMM_LinearCombineSSE, 1)
HMM_INLINE __m128 HMM_PREFIX(LinearCombineSSE)(__m128 left, mat4_t right)
{
    ASSERT_COVERED(HMM_LinearCombineSSE);

    __m128 Result;
    Result = _mm_mul_ps(_mm_shuffle_ps(left, left, 0x00), right.columns[0]);
    Result = _mm_add_ps(Result, _mm_mul_ps(_mm_shuffle_ps(left, left, 0x55), right.columns[1]));
    Result = _mm_add_ps(Result, _mm_mul_ps(_mm_shuffle_ps(left, left, 0xaa), right.columns[2]));
    Result = _mm_add_ps(Result, _mm_mul_ps(_mm_shuffle_ps(left, left, 0xff), right.columns[3]));

    return (Result);
}
#endif


/*
 * Matrix functions
 */

COVERAGE(HMM_Mat4, 1)
HMM_INLINE mat4_t HMM_PREFIX(Mat4)(void)
{
    ASSERT_COVERED(HMM_Mat4);

    mat4_t Result = {0};

    return (Result);
}

COVERAGE(HMM_Mat4d, 1)
HMM_INLINE mat4_t HMM_PREFIX(Mat4d)(float Diagonal)
{
    ASSERT_COVERED(HMM_Mat4d);

    mat4_t Result = HMM_PREFIX(Mat4)();

    Result.elements[0][0] = Diagonal;
    Result.elements[1][1] = Diagonal;
    Result.elements[2][2] = Diagonal;
    Result.elements[3][3] = Diagonal;

    return (Result);
}

COVERAGE(HMM_Transpose, 1)
HMM_INLINE mat4_t HMM_PREFIX(Transpose)(mat4_t Matrix)
{
    ASSERT_COVERED(HMM_Transpose);

    mat4_t Result = Matrix;

#ifdef HANDMADE_MATH__USE_SSE
    _MM_TRANSPOSE4_PS(Result.columns[0], Result.columns[1], Result.columns[2], Result.columns[3]);
#else
    int columns;
    for(columns = 0; columns < 4; ++columns)
    {
        int rows;
        for(rows = 0; rows < 4; ++rows)
        {
            Result.elements[rows][columns] = Matrix.elements[columns][rows];
        }
    }
#endif


    return (Result);
}

COVERAGE(HMM_AddMat4, 1)
HMM_INLINE mat4_t HMM_PREFIX(AddMat4)(mat4_t left, mat4_t right)
{
    ASSERT_COVERED(HMM_AddMat4);

    mat4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.columns[0] = _mm_add_ps(left.columns[0], right.columns[0]);
    Result.columns[1] = _mm_add_ps(left.columns[1], right.columns[1]);
    Result.columns[2] = _mm_add_ps(left.columns[2], right.columns[2]);
    Result.columns[3] = _mm_add_ps(left.columns[3], right.columns[3]);
#else
     int columns;
    for(columns = 0; columns < 4; ++columns)
    {
        int rows;
        for(rows = 0; rows < 4; ++rows)
        {
            Result.elements[columns][rows] = left.elements[columns][rows] + right.elements[columns][rows];
        }
    }
#endif

   
    return (Result);
}

COVERAGE(HMM_SubtractMat4, 1)
HMM_INLINE mat4_t HMM_PREFIX(SubtractMat4)(mat4_t left, mat4_t right)
{
    ASSERT_COVERED(HMM_SubtractMat4);

    mat4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.columns[0] = _mm_sub_ps(left.columns[0], right.columns[0]);
    Result.columns[1] = _mm_sub_ps(left.columns[1], right.columns[1]);
    Result.columns[2] = _mm_sub_ps(left.columns[2], right.columns[2]);
    Result.columns[3] = _mm_sub_ps(left.columns[3], right.columns[3]);
#else
    int columns;
    for(columns = 0; columns < 4; ++columns)
    {
        int rows;
        for(rows = 0; rows < 4; ++rows)
        {
            Result.elements[columns][rows] = left.elements[columns][rows] - right.elements[columns][rows];
        }
    }
#endif
 
    return (Result);
}

COVERAGE(HMM_MultiplyMat4, 1)
HMM_INLINE mat4_t HMM_PREFIX(MultiplyMat4)(mat4_t left, mat4_t right)
{
    ASSERT_COVERED(HMM_MultiplyMat4);

    mat4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.columns[0] = HMM_PREFIX(LinearCombineSSE)(right.columns[0], left);
    Result.columns[1] = HMM_PREFIX(LinearCombineSSE)(right.columns[1], left);
    Result.columns[2] = HMM_PREFIX(LinearCombineSSE)(right.columns[2], left);
    Result.columns[3] = HMM_PREFIX(LinearCombineSSE)(right.columns[3], left);
#else
    int columns;
    for(columns = 0; columns < 4; ++columns)
    {
        int rows;
        for(rows = 0; rows < 4; ++rows)
        {
            float Sum = 0;
            int CurrentMatrice;
            for(CurrentMatrice = 0; CurrentMatrice < 4; ++CurrentMatrice)
            {
                Sum += left.elements[CurrentMatrice][rows] * right.elements[columns][CurrentMatrice];
            }

            Result.elements[columns][rows] = Sum;
        }
    }
#endif

    return (Result);
}


COVERAGE(HMM_MultiplyMat4f, 1)
HMM_INLINE mat4_t HMM_PREFIX(MultiplyMat4f)(mat4_t Matrix, float Scalar)
{
    ASSERT_COVERED(HMM_MultiplyMat4f);

    mat4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEScalar = _mm_set1_ps(Scalar);
    Result.columns[0] = _mm_mul_ps(Matrix.columns[0], SSEScalar);
    Result.columns[1] = _mm_mul_ps(Matrix.columns[1], SSEScalar);
    Result.columns[2] = _mm_mul_ps(Matrix.columns[2], SSEScalar);
    Result.columns[3] = _mm_mul_ps(Matrix.columns[3], SSEScalar);
#else
    int columns;
    for(columns = 0; columns < 4; ++columns)
    {
        int rows;
        for(rows = 0; rows < 4; ++rows)
        {
            Result.elements[columns][rows] = Matrix.elements[columns][rows] * Scalar;
        }
    }
#endif

    return (Result);
}

COVERAGE(HMM_MultiplyMat4ByVec4, 1)
HMM_INLINE vec4_t HMM_PREFIX(MultiplyMat4ByVec4)(mat4_t Matrix, vec4_t Vector)
{
    ASSERT_COVERED(HMM_MultiplyMat4ByVec4);

    vec4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result._internal_sse = HMM_PREFIX(LinearCombineSSE)(Vector._internal_sse, Matrix);
#else
    int columns, rows;
    for(rows = 0; rows < 4; ++rows)
    {
        float Sum = 0;
        for(columns = 0; columns < 4; ++columns)
        {
            Sum += Matrix.elements[columns][rows] * Vector.elements[columns];
        }

        Result.elements[rows] = Sum;
    }
#endif

    return (Result);
}


COVERAGE(HMM_DivideMat4f, 1)
HMM_INLINE mat4_t HMM_PREFIX(DivideMat4f)(mat4_t Matrix, float Scalar)
{
    ASSERT_COVERED(HMM_DivideMat4f);

    mat4_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEScalar = _mm_set1_ps(Scalar);
    Result.columns[0] = _mm_div_ps(Matrix.columns[0], SSEScalar);
    Result.columns[1] = _mm_div_ps(Matrix.columns[1], SSEScalar);
    Result.columns[2] = _mm_div_ps(Matrix.columns[2], SSEScalar);
    Result.columns[3] = _mm_div_ps(Matrix.columns[3], SSEScalar);
#else
    int columns;
    for(columns = 0; columns < 4; ++columns)
    {
        int rows;
        for(rows = 0; rows < 4; ++rows)
        {
            Result.elements[columns][rows] = Matrix.elements[columns][rows] / Scalar;
        }
    }
#endif

    return (Result);
}

/*
 * Common graphics transformations
 */

COVERAGE(HMM_Orthographic, 1)
HMM_INLINE mat4_t HMM_PREFIX(Orthographic)(float left, float right, float Bottom, float Top, float Near, float Far)
{
    ASSERT_COVERED(HMM_Orthographic);

    mat4_t Result = HMM_PREFIX(Mat4)();

    Result.elements[0][0] = 2.0f / (right - left);
    Result.elements[1][1] = 2.0f / (Top - Bottom);
    Result.elements[2][2] = 2.0f / (Near - Far);
    Result.elements[3][3] = 1.0f;

    Result.elements[3][0] = (left + right) / (left - right);
    Result.elements[3][1] = (Bottom + Top) / (Bottom - Top);
    Result.elements[3][2] = (Far + Near) / (Near - Far);

    return (Result);
}

COVERAGE(HMM_Perspective, 1)
HMM_INLINE mat4_t HMM_PREFIX(Perspective)(float FOV, float AspectRatio, float Near, float Far)
{
    ASSERT_COVERED(HMM_Perspective);

    mat4_t Result = HMM_PREFIX(Mat4)();

    // See https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml

    float Cotangent = 1.0f / HMM_PREFIX(TanF)(FOV * (HMM_PI32 / 360.0f));

    Result.elements[0][0] = Cotangent / AspectRatio;
    Result.elements[1][1] = Cotangent;
    Result.elements[2][3] = -1.0f;
    Result.elements[2][2] = (Near + Far) / (Near - Far);
    Result.elements[3][2] = (2.0f * Near * Far) / (Near - Far);
    Result.elements[3][3] = 0.0f;

    return (Result);
}

COVERAGE(HMM_Translate, 1)
HMM_INLINE mat4_t HMM_PREFIX(Translate)(vec3_t Translation)
{
    ASSERT_COVERED(HMM_Translate);

    mat4_t Result = HMM_PREFIX(Mat4d)(1.0f);

    Result.elements[3][0] = Translation.x;
    Result.elements[3][1] = Translation.y;
    Result.elements[3][2] = Translation.z;

    return (Result);
}

COVERAGE(HMM_Rotate, 1)
HMM_INLINE mat4_t HMM_PREFIX(Rotate)(float Angle, vec3_t Axis)
{
    ASSERT_COVERED(HMM_Rotate);

    mat4_t Result = HMM_PREFIX(Mat4d)(1.0f);

    Axis = HMM_PREFIX(NormalizeVec3)(Axis);

    float SinTheta = HMM_PREFIX(SinF)(HMM_PREFIX(ToRadians)(Angle));
    float CosTheta = HMM_PREFIX(CosF)(HMM_PREFIX(ToRadians)(Angle));
    float CosValue = 1.0f - CosTheta;

    Result.elements[0][0] = (Axis.x * Axis.x * CosValue) + CosTheta;
    Result.elements[0][1] = (Axis.x * Axis.y * CosValue) + (Axis.z * SinTheta);
    Result.elements[0][2] = (Axis.x * Axis.z * CosValue) - (Axis.y * SinTheta);

    Result.elements[1][0] = (Axis.y * Axis.x * CosValue) - (Axis.z * SinTheta);
    Result.elements[1][1] = (Axis.y * Axis.y * CosValue) + CosTheta;
    Result.elements[1][2] = (Axis.y * Axis.z * CosValue) + (Axis.x * SinTheta);

    Result.elements[2][0] = (Axis.z * Axis.x * CosValue) + (Axis.y * SinTheta);
    Result.elements[2][1] = (Axis.z * Axis.y * CosValue) - (Axis.x * SinTheta);
    Result.elements[2][2] = (Axis.z * Axis.z * CosValue) + CosTheta;

    return (Result);
}



COVERAGE(HMM_Scale, 1)
HMM_INLINE mat4_t HMM_PREFIX(Scale)(vec3_t Scale)
{
    ASSERT_COVERED(HMM_Scale);

    mat4_t Result = HMM_PREFIX(Mat4d)(1.0f);

    Result.elements[0][0] = Scale.x;
    Result.elements[1][1] = Scale.y;
    Result.elements[2][2] = Scale.z;

    return (Result);
}

COVERAGE(HMM_LookAt, 1)
HMM_INLINE mat4_t HMM_PREFIX(LookAt)(vec3_t Eye, vec3_t Center, vec3_t Up)
{
    ASSERT_COVERED(HMM_LookAt);

    mat4_t Result;

    vec3_t F = HMM_PREFIX(NormalizeVec3)(HMM_PREFIX(SubtractVec3)(Center, Eye));
    vec3_t S = HMM_PREFIX(NormalizeVec3)(HMM_PREFIX(Cross)(F, Up));
    vec3_t u = HMM_PREFIX(Cross)(S, F);

    Result.elements[0][0] = S.x;
    Result.elements[0][1] = u.x;
    Result.elements[0][2] = -F.x;
    Result.elements[0][3] = 0.0f;

    Result.elements[1][0] = S.y;
    Result.elements[1][1] = u.y;
    Result.elements[1][2] = -F.y;
    Result.elements[1][3] = 0.0f;

    Result.elements[2][0] = S.z;
    Result.elements[2][1] = u.z;
    Result.elements[2][2] = -F.z;
    Result.elements[2][3] = 0.0f;

    Result.elements[3][0] = -HMM_PREFIX(DotVec3)(S, Eye);
    Result.elements[3][1] = -HMM_PREFIX(DotVec3)(u, Eye);
    Result.elements[3][2] = HMM_PREFIX(DotVec3)(F, Eye);
    Result.elements[3][3] = 1.0f;

    return (Result);
}

/*
 * Quaternion operations
 */

COVERAGE(HMM_Quaternion, 1)
HMM_INLINE quaternion_t HMM_PREFIX(Quaternion)(float x, float y, float z, float w)
{
    ASSERT_COVERED(HMM_Quaternion);

    quaternion_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result._internal_sse = _mm_setr_ps(x, y, z, w);
#else
    Result.x = x;
    Result.y = y;
    Result.z = z;
    Result.w = w;
#endif

    return (Result);
}

COVERAGE(HMM_QuaternionV4, 1)
HMM_INLINE quaternion_t HMM_PREFIX(QuaternionV4)(vec4_t Vector)
{
    ASSERT_COVERED(HMM_QuaternionV4);

    quaternion_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result._internal_sse = Vector._internal_sse;
#else
    Result.x = Vector.x;
    Result.y = Vector.y;
    Result.z = Vector.z;
    Result.w = Vector.w;
#endif

    return (Result);
}

COVERAGE(HMM_AddQuaternion, 1)
HMM_INLINE quaternion_t HMM_PREFIX(AddQuaternion)(quaternion_t left, quaternion_t right)
{
    ASSERT_COVERED(HMM_AddQuaternion);

    quaternion_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result._internal_sse = _mm_add_ps(left._internal_sse, right._internal_sse);
#else

    Result.x = left.x + right.x;
    Result.y = left.y + right.y;
    Result.z = left.z + right.z;
    Result.w = left.w + right.w;
#endif

    return (Result);
}

COVERAGE(HMM_SubtractQuaternion, 1)
HMM_INLINE quaternion_t HMM_PREFIX(SubtractQuaternion)(quaternion_t left, quaternion_t right)
{
    ASSERT_COVERED(HMM_SubtractQuaternion);

    quaternion_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result._internal_sse = _mm_sub_ps(left._internal_sse, right._internal_sse);
#else

    Result.x = left.x - right.x;
    Result.y = left.y - right.y;
    Result.z = left.z - right.z;
    Result.w = left.w - right.w;
#endif

    return (Result);
}

COVERAGE(HMM_MultiplyQuaternion, 1)
HMM_INLINE quaternion_t HMM_PREFIX(MultiplyQuaternion)(quaternion_t left, quaternion_t right)
{
    ASSERT_COVERED(HMM_MultiplyQuaternion);

    quaternion_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEResultOne = _mm_xor_ps(_mm_shuffle_ps(left._internal_sse, left._internal_sse, _MM_SHUFFLE(0, 0, 0, 0)), _mm_setr_ps(0.f, -0.f, 0.f, -0.f));
    __m128 SSEResultTwo = _mm_shuffle_ps(right._internal_sse, right._internal_sse, _MM_SHUFFLE(0, 1, 2, 3));
    __m128 SSEResultThree = _mm_mul_ps(SSEResultTwo, SSEResultOne);

    SSEResultOne = _mm_xor_ps(_mm_shuffle_ps(left._internal_sse, left._internal_sse, _MM_SHUFFLE(1, 1, 1, 1)) , _mm_setr_ps(0.f, 0.f, -0.f, -0.f));
    SSEResultTwo = _mm_shuffle_ps(right._internal_sse, right._internal_sse, _MM_SHUFFLE(1, 0, 3, 2));
    SSEResultThree = _mm_add_ps(SSEResultThree, _mm_mul_ps(SSEResultTwo, SSEResultOne));

    SSEResultOne = _mm_xor_ps(_mm_shuffle_ps(left._internal_sse, left._internal_sse, _MM_SHUFFLE(2, 2, 2, 2)), _mm_setr_ps(-0.f, 0.f, 0.f, -0.f));
    SSEResultTwo = _mm_shuffle_ps(right._internal_sse, right._internal_sse, _MM_SHUFFLE(2, 3, 0, 1));
    SSEResultThree = _mm_add_ps(SSEResultThree, _mm_mul_ps(SSEResultTwo, SSEResultOne));

    SSEResultOne = _mm_shuffle_ps(left._internal_sse, left._internal_sse, _MM_SHUFFLE(3, 3, 3, 3));
    SSEResultTwo = _mm_shuffle_ps(right._internal_sse, right._internal_sse, _MM_SHUFFLE(3, 2, 1, 0));
    Result._internal_sse = _mm_add_ps(SSEResultThree, _mm_mul_ps(SSEResultTwo, SSEResultOne));
#else
    Result.x = (left.x * right.w) + (left.y * right.z) - (left.z * right.y) + (left.w * right.x);
    Result.y = (-left.x * right.z) + (left.y * right.w) + (left.z * right.x) + (left.w * right.y);
    Result.z = (left.x * right.y) - (left.y * right.x) + (left.z * right.w) + (left.w * right.z);
    Result.w = (-left.x * right.x) - (left.y * right.y) - (left.z * right.z) + (left.w * right.w);
#endif

    return (Result);
}

COVERAGE(HMM_MultiplyQuaternionF, 1)
HMM_INLINE quaternion_t HMM_PREFIX(MultiplyQuaternionF)(quaternion_t left, float Multiplicative)
{
    ASSERT_COVERED(HMM_MultiplyQuaternionF);

    quaternion_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Scalar = _mm_set1_ps(Multiplicative);
    Result._internal_sse = _mm_mul_ps(left._internal_sse, Scalar);
#else
    Result.x = left.x * Multiplicative;
    Result.y = left.y * Multiplicative;
    Result.z = left.z * Multiplicative;
    Result.w = left.w * Multiplicative;
#endif

    return (Result);
}

COVERAGE(HMM_DivideQuaternionF, 1)
HMM_INLINE quaternion_t HMM_PREFIX(DivideQuaternionF)(quaternion_t left, float Dividend)
{
    ASSERT_COVERED(HMM_DivideQuaternionF);

    quaternion_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Scalar = _mm_set1_ps(Dividend);
    Result._internal_sse = _mm_div_ps(left._internal_sse, Scalar);
#else
    Result.x = left.x / Dividend;
    Result.y = left.y / Dividend;
    Result.z = left.z / Dividend;
    Result.w = left.w / Dividend;
#endif

    return (Result);
}

COVERAGE(HMM_DotQuaternion, 1)
HMM_INLINE float HMM_PREFIX(DotQuaternion)(quaternion_t left, quaternion_t right)
{
    ASSERT_COVERED(HMM_DotQuaternion);

    float Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEResultOne = _mm_mul_ps(left._internal_sse, right._internal_sse);
    __m128 SSEResultTwo = _mm_shuffle_ps(SSEResultOne, SSEResultOne, _MM_SHUFFLE(2, 3, 0, 1));
    SSEResultOne = _mm_add_ps(SSEResultOne, SSEResultTwo);
    SSEResultTwo = _mm_shuffle_ps(SSEResultOne, SSEResultOne, _MM_SHUFFLE(0, 1, 2, 3));
    SSEResultOne = _mm_add_ps(SSEResultOne, SSEResultTwo);
    _mm_store_ss(&Result, SSEResultOne);
#else
    Result = (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w);
#endif

    return (Result);
}


COVERAGE(HMM_InverseQuaternion, 1)
HMM_INLINE quaternion_t HMM_PREFIX(InverseQuaternion)(quaternion_t left)
{
    ASSERT_COVERED(HMM_InverseQuaternion);
    
    quaternion_t Result;

    Result.x = -left.x;
    Result.y = -left.y;
    Result.z = -left.z;
    Result.w = left.w;

    Result = HMM_PREFIX(DivideQuaternionF)(Result, (HMM_PREFIX(DotQuaternion)(left, left)));

    return (Result);
}


COVERAGE(HMM_NormalizeQuaternion, 1)
HMM_INLINE quaternion_t HMM_PREFIX(NormalizeQuaternion)(quaternion_t left)
{
    ASSERT_COVERED(HMM_NormalizeQuaternion);

    quaternion_t Result;

    float Length = HMM_PREFIX(SquareRootF)(HMM_PREFIX(DotQuaternion)(left, left));
    Result = HMM_PREFIX(DivideQuaternionF)(left, Length);

    return (Result);
}

COVERAGE(HMM_NLerp, 1)
HMM_INLINE quaternion_t HMM_PREFIX(NLerp)(quaternion_t left, float Time, quaternion_t right)
{
    ASSERT_COVERED(HMM_NLerp);

    quaternion_t Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 ScalarLeft = _mm_set1_ps(1.0f - Time);
    __m128 ScalarRight = _mm_set1_ps(Time);
    __m128 SSEResultOne = _mm_mul_ps(left._internal_sse, ScalarLeft);
    __m128 SSEResultTwo = _mm_mul_ps(right._internal_sse, ScalarRight);
    Result._internal_sse = _mm_add_ps(SSEResultOne, SSEResultTwo);
#else
    Result.x = HMM_PREFIX(Lerp)(left.x, Time, right.x);
    Result.y = HMM_PREFIX(Lerp)(left.y, Time, right.y);
    Result.z = HMM_PREFIX(Lerp)(left.z, Time, right.z);
    Result.w = HMM_PREFIX(Lerp)(left.w, Time, right.w);
#endif
    Result = HMM_PREFIX(NormalizeQuaternion)(Result);

    return (Result);
}

COVERAGE(HMM_Slerp, 1)
HMM_INLINE quaternion_t HMM_PREFIX(Slerp)(quaternion_t left, float Time, quaternion_t right)
{
    ASSERT_COVERED(HMM_Slerp);

    quaternion_t Result;
    quaternion_t QuaternionLeft;
    quaternion_t QuaternionRight;

    float Cos_Theta = HMM_PREFIX(DotQuaternion)(left, right);
    float Angle = HMM_PREFIX(ACosF)(Cos_Theta);

    float S1 = HMM_PREFIX(SinF)((1.0f - Time) * Angle);
    float S2 = HMM_PREFIX(SinF)(Time * Angle);
    float Is = 1.0f / HMM_PREFIX(SinF)(Angle);

    QuaternionLeft = HMM_PREFIX(MultiplyQuaternionF)(left, S1);
    QuaternionRight = HMM_PREFIX(MultiplyQuaternionF)(right, S2);

    Result = HMM_PREFIX(AddQuaternion)(QuaternionLeft, QuaternionRight);
    Result = HMM_PREFIX(MultiplyQuaternionF)(Result, Is);

    return (Result);
}

COVERAGE(HMM_QuaternionToMat4, 1)
HMM_INLINE mat4_t HMM_PREFIX(QuaternionToMat4)(quaternion_t left)
{
    ASSERT_COVERED(HMM_QuaternionToMat4);

    mat4_t Result;

    quaternion_t NormalizedQuaternion = HMM_PREFIX(NormalizeQuaternion)(left);

    float XX, YY, ZZ,
          xy, XZ, yz,
          WX, WY, WZ;

    XX = NormalizedQuaternion.x * NormalizedQuaternion.x;
    YY = NormalizedQuaternion.y * NormalizedQuaternion.y;
    ZZ = NormalizedQuaternion.z * NormalizedQuaternion.z;
    xy = NormalizedQuaternion.x * NormalizedQuaternion.y;
    XZ = NormalizedQuaternion.x * NormalizedQuaternion.z;
    yz = NormalizedQuaternion.y * NormalizedQuaternion.z;
    WX = NormalizedQuaternion.w * NormalizedQuaternion.x;
    WY = NormalizedQuaternion.w * NormalizedQuaternion.y;
    WZ = NormalizedQuaternion.w * NormalizedQuaternion.z;

    Result.elements[0][0] = 1.0f - 2.0f * (YY + ZZ);
    Result.elements[0][1] = 2.0f * (xy + WZ);
    Result.elements[0][2] = 2.0f * (XZ - WY);
    Result.elements[0][3] = 0.0f;

    Result.elements[1][0] = 2.0f * (xy - WZ);
    Result.elements[1][1] = 1.0f - 2.0f * (XX + ZZ);
    Result.elements[1][2] = 2.0f * (yz + WX);
    Result.elements[1][3] = 0.0f;

    Result.elements[2][0] = 2.0f * (XZ + WY);
    Result.elements[2][1] = 2.0f * (yz - WX);
    Result.elements[2][2] = 1.0f - 2.0f * (XX + YY);
    Result.elements[2][3] = 0.0f;

    Result.elements[3][0] = 0.0f;
    Result.elements[3][1] = 0.0f;
    Result.elements[3][2] = 0.0f;
    Result.elements[3][3] = 1.0f;

    return (Result);
}

// This method taken from Mike Day at Insomniac Games.
// https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf
//
// Note that as mentioned at the top of the paper, the paper assumes the matrix
// would be *post*-multiplied to a vector to rotate it, meaning the matrix is
// the transpose of what we're dealing with. But, because our matrices are
// stored in column-major order, the indices *appear* to match the paper.
//
// For example, m12 in the paper is row 1, column 2. We need to transpose it to
// row 2, column 1. But, because the column comes first when referencing
// elements, it looks like M.elements[1][2].
//
// Don't be confused! Or if you must be confused, at least trust this
// comment. :)
COVERAGE(HMM_Mat4ToQuaternion, 4)
HMM_INLINE quaternion_t HMM_PREFIX(Mat4ToQuaternion)(mat4_t M)
{
    float T;
    quaternion_t Q;

    if (M.elements[2][2] < 0.0f) {
        if (M.elements[0][0] > M.elements[1][1]) {
            ASSERT_COVERED(HMM_Mat4ToQuaternion);

            T = 1 + M.elements[0][0] - M.elements[1][1] - M.elements[2][2];
            Q = HMM_PREFIX(Quaternion)(
                T,
                M.elements[0][1] + M.elements[1][0],
                M.elements[2][0] + M.elements[0][2],
                M.elements[1][2] - M.elements[2][1]
            );
        } else {
            ASSERT_COVERED(HMM_Mat4ToQuaternion);

            T = 1 - M.elements[0][0] + M.elements[1][1] - M.elements[2][2];
            Q = HMM_PREFIX(Quaternion)(
                M.elements[0][1] + M.elements[1][0],
                T,
                M.elements[1][2] + M.elements[2][1],
                M.elements[2][0] - M.elements[0][2]
            );
        }
    } else {
        if (M.elements[0][0] < -M.elements[1][1]) {
            ASSERT_COVERED(HMM_Mat4ToQuaternion);

            T = 1 - M.elements[0][0] - M.elements[1][1] + M.elements[2][2];
            Q = HMM_PREFIX(Quaternion)(
                M.elements[2][0] + M.elements[0][2],
                M.elements[1][2] + M.elements[2][1],
                T,
                M.elements[0][1] - M.elements[1][0]
            );
        } else {
            ASSERT_COVERED(HMM_Mat4ToQuaternion);

            T = 1 + M.elements[0][0] + M.elements[1][1] + M.elements[2][2];
            Q = HMM_PREFIX(Quaternion)(
                M.elements[1][2] - M.elements[2][1],
                M.elements[2][0] - M.elements[0][2],
                M.elements[0][1] - M.elements[1][0],
                T
            );
        }
    }

    Q = HMM_PREFIX(MultiplyQuaternionF)(Q, 0.5f / HMM_PREFIX(SquareRootF)(T));

    return Q;
}

COVERAGE(HMM_QuaternionFromAxisAngle, 1)
HMM_INLINE quaternion_t HMM_PREFIX(QuaternionFromAxisAngle)(vec3_t Axis, float AngleOfRotation)
{
    ASSERT_COVERED(HMM_QuaternionFromAxisAngle);

    quaternion_t Result;

    vec3_t AxisNormalized = HMM_PREFIX(NormalizeVec3)(Axis);
    float SineOfRotation = HMM_PREFIX(SinF)(AngleOfRotation / 2.0f);

    Result.xyz = HMM_PREFIX(MultiplyVec3f)(AxisNormalized, SineOfRotation);
    Result.w = HMM_PREFIX(CosF)(AngleOfRotation / 2.0f);

    return (Result);
}

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

COVERAGE(HMM_LengthVec2CPP, 1)
HMM_INLINE float HMM_PREFIX(Length)(vec2_t a)
{
    ASSERT_COVERED(HMM_LengthVec2CPP);

    float Result = HMM_PREFIX(LengthVec2)(a);
    return (Result);
}

COVERAGE(HMM_LengthVec3CPP, 1)
HMM_INLINE float HMM_PREFIX(Length)(vec3_t a)
{
    ASSERT_COVERED(HMM_LengthVec3CPP);

    float Result = HMM_PREFIX(LengthVec3)(a);

    return (Result);
}

COVERAGE(HMM_LengthVec4CPP, 1)
HMM_INLINE float HMM_PREFIX(Length)(vec4_t a)
{
    ASSERT_COVERED(HMM_LengthVec4CPP);

    float Result = HMM_PREFIX(LengthVec4)(a);

    return (Result);
}

COVERAGE(HMM_LengthSquaredVec2CPP, 1)
HMM_INLINE float HMM_PREFIX(LengthSquared)(vec2_t a)
{
    ASSERT_COVERED(HMM_LengthSquaredVec2CPP);

    float Result = HMM_PREFIX(LengthSquaredVec2)(a);

    return (Result);
}

COVERAGE(HMM_LengthSquaredVec3CPP, 1)
HMM_INLINE float HMM_PREFIX(LengthSquared)(vec3_t a)
{
    ASSERT_COVERED(HMM_LengthSquaredVec3CPP);

    float Result = HMM_PREFIX(LengthSquaredVec3)(a);

    return (Result);
}

COVERAGE(HMM_LengthSquaredVec4CPP, 1)
HMM_INLINE float HMM_PREFIX(LengthSquared)(vec4_t a)
{
    ASSERT_COVERED(HMM_LengthSquaredVec4CPP);

    float Result = HMM_PREFIX(LengthSquaredVec4)(a);

    return (Result);
}

COVERAGE(HMM_NormalizeVec2CPP, 1)
HMM_INLINE vec2_t HMM_PREFIX(Normalize)(vec2_t a)
{
    ASSERT_COVERED(HMM_NormalizeVec2CPP);

    vec2_t Result = HMM_PREFIX(NormalizeVec2)(a);

    return (Result);
}

COVERAGE(HMM_NormalizeVec3CPP, 1)
HMM_INLINE vec3_t HMM_PREFIX(Normalize)(vec3_t a)
{
    ASSERT_COVERED(HMM_NormalizeVec3CPP);

    vec3_t Result = HMM_PREFIX(NormalizeVec3)(a);

    return (Result);
}

COVERAGE(HMM_NormalizeVec4CPP, 1)
HMM_INLINE vec4_t HMM_PREFIX(Normalize)(vec4_t a)
{
    ASSERT_COVERED(HMM_NormalizeVec4CPP);

    vec4_t Result = HMM_PREFIX(NormalizeVec4)(a);

    return (Result);
}

COVERAGE(HMM_FastNormalizeVec2CPP, 1)
HMM_INLINE vec2_t HMM_PREFIX(FastNormalize)(vec2_t a)
{
    ASSERT_COVERED(HMM_FastNormalizeVec2CPP);

    vec2_t Result = HMM_PREFIX(FastNormalizeVec2)(a);

    return (Result);
}

COVERAGE(HMM_FastNormalizeVec3CPP, 1)
HMM_INLINE vec3_t HMM_PREFIX(FastNormalize)(vec3_t a)
{
    ASSERT_COVERED(HMM_FastNormalizeVec3CPP);

    vec3_t Result = HMM_PREFIX(FastNormalizeVec3)(a);

    return (Result);
}

COVERAGE(HMM_FastNormalizeVec4CPP, 1)
HMM_INLINE vec4_t HMM_PREFIX(FastNormalize)(vec4_t a)
{
    ASSERT_COVERED(HMM_FastNormalizeVec4CPP);

    vec4_t Result = HMM_PREFIX(FastNormalizeVec4)(a);

    return (Result);
}

COVERAGE(HMM_NormalizeQuaternionCPP, 1)
HMM_INLINE hmm_quaternion HMM_PREFIX(Normalize)(hmm_quaternion a)
{
    ASSERT_COVERED(HMM_NormalizeQuaternionCPP);

    hmm_quaternion Result = HMM_PREFIX(NormalizeQuaternion)(a);

    return (Result);
}

COVERAGE(HMM_DotVec2CPP, 1)
HMM_INLINE float HMM_PREFIX(Dot)(vec2_t VecOne, vec2_t VecTwo)
{
    ASSERT_COVERED(HMM_DotVec2CPP);

    float Result = HMM_PREFIX(DotVec2)(VecOne, VecTwo);

    return (Result);
}

COVERAGE(HMM_DotVec3CPP, 1)
HMM_INLINE float HMM_PREFIX(Dot)(vec3_t VecOne, vec3_t VecTwo)
{
    ASSERT_COVERED(HMM_DotVec3CPP);

    float Result = HMM_PREFIX(DotVec3)(VecOne, VecTwo);

    return (Result);
}

COVERAGE(HMM_DotVec4CPP, 1)
HMM_INLINE float HMM_PREFIX(Dot)(vec4_t VecOne, vec4_t VecTwo)
{
    ASSERT_COVERED(HMM_DotVec4CPP);

    float Result = HMM_PREFIX(DotVec4)(VecOne, VecTwo);

    return (Result);
}

COVERAGE(HMM_DotQuaternionCPP, 1)
HMM_INLINE float HMM_PREFIX(Dot)(hmm_quaternion QuatOne, hmm_quaternion QuatTwo)
{
    ASSERT_COVERED(HMM_DotQuaternionCPP);

    float Result = HMM_PREFIX(DotQuaternion)(QuatOne, QuatTwo);

    return (Result);
}

COVERAGE(HMM_AddVec2CPP, 1)
HMM_INLINE vec2_t HMM_PREFIX(Add)(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_AddVec2CPP);

    vec2_t Result = HMM_PREFIX(AddVec2)(left, right);

    return (Result);
}

COVERAGE(HMM_AddVec3CPP, 1)
HMM_INLINE vec3_t HMM_PREFIX(Add)(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_AddVec3CPP);

    vec3_t Result = HMM_PREFIX(AddVec3)(left, right);

    return (Result);
}

COVERAGE(HMM_AddVec4CPP, 1)
HMM_INLINE vec4_t HMM_PREFIX(Add)(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_AddVec4CPP);

    vec4_t Result = HMM_PREFIX(AddVec4)(left, right);

    return (Result);
}

COVERAGE(HMM_AddMat4CPP, 1)
HMM_INLINE mat4_t HMM_PREFIX(Add)(mat4_t left, mat4_t right)
{
    ASSERT_COVERED(HMM_AddMat4CPP);

    mat4_t Result = HMM_PREFIX(AddMat4)(left, right);

    return (Result);
}

COVERAGE(HMM_AddQuaternionCPP, 1)
HMM_INLINE hmm_quaternion HMM_PREFIX(Add)(hmm_quaternion left, hmm_quaternion right)
{
    ASSERT_COVERED(HMM_AddQuaternionCPP);

    hmm_quaternion Result = HMM_PREFIX(AddQuaternion)(left, right);

    return (Result);
}

COVERAGE(HMM_SubtractVec2CPP, 1)
HMM_INLINE vec2_t HMM_PREFIX(Subtract)(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_SubtractVec2CPP);

    vec2_t Result = HMM_PREFIX(SubtractVec2)(left, right);

    return (Result);
}

COVERAGE(HMM_SubtractVec3CPP, 1)
HMM_INLINE vec3_t HMM_PREFIX(Subtract)(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_SubtractVec3CPP);

    vec3_t Result = HMM_PREFIX(SubtractVec3)(left, right);

    return (Result);
}

COVERAGE(HMM_SubtractVec4CPP, 1)
HMM_INLINE vec4_t HMM_PREFIX(Subtract)(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_SubtractVec4CPP);

    vec4_t Result = HMM_PREFIX(SubtractVec4)(left, right);

    return (Result);
}

COVERAGE(HMM_SubtractMat4CPP, 1)
HMM_INLINE mat4_t HMM_PREFIX(Subtract)(mat4_t left, mat4_t right)
{
    ASSERT_COVERED(HMM_SubtractMat4CPP);

    mat4_t Result = HMM_PREFIX(SubtractMat4)(left, right);

    return (Result);
}

COVERAGE(HMM_SubtractQuaternionCPP, 1)
HMM_INLINE hmm_quaternion HMM_PREFIX(Subtract)(hmm_quaternion left, hmm_quaternion right)
{
    ASSERT_COVERED(HMM_SubtractQuaternionCPP);

    hmm_quaternion Result = HMM_PREFIX(SubtractQuaternion)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec2CPP, 1)
HMM_INLINE vec2_t HMM_PREFIX(Multiply)(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec2CPP);

    vec2_t Result = HMM_PREFIX(MultiplyVec2)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec2fCPP, 1)
HMM_INLINE vec2_t HMM_PREFIX(Multiply)(vec2_t left, float right)
{
    ASSERT_COVERED(HMM_MultiplyVec2fCPP);

    vec2_t Result = HMM_PREFIX(MultiplyVec2f)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec3CPP, 1)
HMM_INLINE vec3_t HMM_PREFIX(Multiply)(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec3CPP);

    vec3_t Result = HMM_PREFIX(MultiplyVec3)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec3fCPP, 1)
HMM_INLINE vec3_t HMM_PREFIX(Multiply)(vec3_t left, float right)
{
    ASSERT_COVERED(HMM_MultiplyVec3fCPP);

    vec3_t Result = HMM_PREFIX(MultiplyVec3f)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec4CPP, 1)
HMM_INLINE vec4_t HMM_PREFIX(Multiply)(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec4CPP);

    vec4_t Result = HMM_PREFIX(MultiplyVec4)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec4fCPP, 1)
HMM_INLINE vec4_t HMM_PREFIX(Multiply)(vec4_t left, float right)
{
    ASSERT_COVERED(HMM_MultiplyVec4fCPP);

    vec4_t Result = HMM_PREFIX(MultiplyVec4f)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyMat4CPP, 1)
HMM_INLINE mat4_t HMM_PREFIX(Multiply)(mat4_t left, mat4_t right)
{
    ASSERT_COVERED(HMM_MultiplyMat4CPP);

    mat4_t Result = HMM_PREFIX(MultiplyMat4)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyMat4fCPP, 1)
HMM_INLINE mat4_t HMM_PREFIX(Multiply)(mat4_t left, float right)
{
    ASSERT_COVERED(HMM_MultiplyMat4fCPP);

    mat4_t Result = HMM_PREFIX(MultiplyMat4f)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyMat4ByVec4CPP, 1)
HMM_INLINE vec4_t HMM_PREFIX(Multiply)(mat4_t Matrix, vec4_t Vector)
{
    ASSERT_COVERED(HMM_MultiplyMat4ByVec4CPP);

    vec4_t Result = HMM_PREFIX(MultiplyMat4ByVec4)(Matrix, Vector);

    return (Result);
}

COVERAGE(HMM_MultiplyQuaternionCPP, 1)
HMM_INLINE hmm_quaternion HMM_PREFIX(Multiply)(hmm_quaternion left, hmm_quaternion right)
{
    ASSERT_COVERED(HMM_MultiplyQuaternionCPP);

    hmm_quaternion Result = HMM_PREFIX(MultiplyQuaternion)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyQuaternionFCPP, 1)
HMM_INLINE hmm_quaternion HMM_PREFIX(Multiply)(hmm_quaternion left, float right)
{
    ASSERT_COVERED(HMM_MultiplyQuaternionFCPP);

    hmm_quaternion Result = HMM_PREFIX(MultiplyQuaternionF)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideVec2CPP, 1)
HMM_INLINE vec2_t HMM_PREFIX(Divide)(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_DivideVec2CPP);

    vec2_t Result = HMM_PREFIX(DivideVec2)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideVec2fCPP, 1)
HMM_INLINE vec2_t HMM_PREFIX(Divide)(vec2_t left, float right)
{
    ASSERT_COVERED(HMM_DivideVec2fCPP);

    vec2_t Result = HMM_PREFIX(DivideVec2f)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideVec3CPP, 1)
HMM_INLINE vec3_t HMM_PREFIX(Divide)(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_DivideVec3CPP);

    vec3_t Result = HMM_PREFIX(DivideVec3)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideVec3fCPP, 1)
HMM_INLINE vec3_t HMM_PREFIX(Divide)(vec3_t left, float right)
{
    ASSERT_COVERED(HMM_DivideVec3fCPP);

    vec3_t Result = HMM_PREFIX(DivideVec3f)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideVec4CPP, 1)
HMM_INLINE vec4_t HMM_PREFIX(Divide)(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_DivideVec4CPP);

    vec4_t Result = HMM_PREFIX(DivideVec4)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideVec4fCPP, 1)
HMM_INLINE vec4_t HMM_PREFIX(Divide)(vec4_t left, float right)
{
    ASSERT_COVERED(HMM_DivideVec4fCPP);

    vec4_t Result = HMM_PREFIX(DivideVec4f)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideMat4fCPP, 1)
HMM_INLINE mat4_t HMM_PREFIX(Divide)(mat4_t left, float right)
{
    ASSERT_COVERED(HMM_DivideMat4fCPP);

    mat4_t Result = HMM_PREFIX(DivideMat4f)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideQuaternionFCPP, 1)
HMM_INLINE hmm_quaternion HMM_PREFIX(Divide)(hmm_quaternion left, float right)
{
    ASSERT_COVERED(HMM_DivideQuaternionFCPP);

    hmm_quaternion Result = HMM_PREFIX(DivideQuaternionF)(left, right);

    return (Result);
}

COVERAGE(HMM_EqualsVec2CPP, 1)
HMM_INLINE hmm_bool HMM_PREFIX(Equals)(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_EqualsVec2CPP);

    hmm_bool Result = HMM_PREFIX(EqualsVec2)(left, right);

    return (Result);
}

COVERAGE(HMM_EqualsVec3CPP, 1)
HMM_INLINE hmm_bool HMM_PREFIX(Equals)(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_EqualsVec3CPP);

    hmm_bool Result = HMM_PREFIX(EqualsVec3)(left, right);

    return (Result);
}

COVERAGE(HMM_EqualsVec4CPP, 1)
HMM_INLINE hmm_bool HMM_PREFIX(Equals)(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_EqualsVec4CPP);

    hmm_bool Result = HMM_PREFIX(EqualsVec4)(left, right);

    return (Result);
}

COVERAGE(HMM_AddVec2Op, 1)
HMM_INLINE vec2_t operator+(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_AddVec2Op);

    vec2_t Result = HMM_PREFIX(AddVec2)(left, right);

    return (Result);
}

COVERAGE(HMM_AddVec3Op, 1)
HMM_INLINE vec3_t operator+(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_AddVec3Op);

    vec3_t Result = HMM_PREFIX(AddVec3)(left, right);

    return (Result);
}

COVERAGE(HMM_AddVec4Op, 1)
HMM_INLINE vec4_t operator+(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_AddVec4Op);

    vec4_t Result = HMM_PREFIX(AddVec4)(left, right);

    return (Result);
}

COVERAGE(HMM_AddMat4Op, 1)
HMM_INLINE mat4_t operator+(mat4_t left, mat4_t right)
{
    ASSERT_COVERED(HMM_AddMat4Op);

    mat4_t Result = HMM_PREFIX(AddMat4)(left, right);

    return (Result);
}

COVERAGE(HMM_AddQuaternionOp, 1)
HMM_INLINE hmm_quaternion operator+(hmm_quaternion left, hmm_quaternion right)
{
    ASSERT_COVERED(HMM_AddQuaternionOp);

    hmm_quaternion Result = HMM_PREFIX(AddQuaternion)(left, right);

    return (Result);
}

COVERAGE(HMM_SubtractVec2Op, 1)
HMM_INLINE vec2_t operator-(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_SubtractVec2Op);

    vec2_t Result = HMM_PREFIX(SubtractVec2)(left, right);

    return (Result);
}

COVERAGE(HMM_SubtractVec3Op, 1)
HMM_INLINE vec3_t operator-(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_SubtractVec3Op);

    vec3_t Result = HMM_PREFIX(SubtractVec3)(left, right);

    return (Result);
}

COVERAGE(HMM_SubtractVec4Op, 1)
HMM_INLINE vec4_t operator-(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_SubtractVec4Op);

    vec4_t Result = HMM_PREFIX(SubtractVec4)(left, right);

    return (Result);
}

COVERAGE(HMM_SubtractMat4Op, 1)
HMM_INLINE mat4_t operator-(mat4_t left, mat4_t right)
{
    ASSERT_COVERED(HMM_SubtractMat4Op);

    mat4_t Result = HMM_PREFIX(SubtractMat4)(left, right);

    return (Result);
}

COVERAGE(HMM_SubtractQuaternionOp, 1)
HMM_INLINE hmm_quaternion operator-(hmm_quaternion left, hmm_quaternion right)
{
    ASSERT_COVERED(HMM_SubtractQuaternionOp);

    hmm_quaternion Result = HMM_PREFIX(SubtractQuaternion)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec2Op, 1)
HMM_INLINE vec2_t operator*(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec2Op);

    vec2_t Result = HMM_PREFIX(MultiplyVec2)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec3Op, 1)
HMM_INLINE vec3_t operator*(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec3Op);

    vec3_t Result = HMM_PREFIX(MultiplyVec3)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec4Op, 1)
HMM_INLINE vec4_t operator*(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec4Op);

    vec4_t Result = HMM_PREFIX(MultiplyVec4)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyMat4Op, 1)
HMM_INLINE mat4_t operator*(mat4_t left, mat4_t right)
{
    ASSERT_COVERED(HMM_MultiplyMat4Op);

    mat4_t Result = HMM_PREFIX(MultiplyMat4)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyQuaternionOp, 1)
HMM_INLINE hmm_quaternion operator*(hmm_quaternion left, hmm_quaternion right)
{
    ASSERT_COVERED(HMM_MultiplyQuaternionOp);

    hmm_quaternion Result = HMM_PREFIX(MultiplyQuaternion)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec2fOp, 1)
HMM_INLINE vec2_t operator*(vec2_t left, float right)
{
    ASSERT_COVERED(HMM_MultiplyVec2fOp);

    vec2_t Result = HMM_PREFIX(MultiplyVec2f)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec3fOp, 1)
HMM_INLINE vec3_t operator*(vec3_t left, float right)
{
    ASSERT_COVERED(HMM_MultiplyVec3fOp);

    vec3_t Result = HMM_PREFIX(MultiplyVec3f)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec4fOp, 1)
HMM_INLINE vec4_t operator*(vec4_t left, float right)
{
    ASSERT_COVERED(HMM_MultiplyVec4fOp);

    vec4_t Result = HMM_PREFIX(MultiplyVec4f)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyMat4fOp, 1)
HMM_INLINE mat4_t operator*(mat4_t left, float right)
{
    ASSERT_COVERED(HMM_MultiplyMat4fOp);

    mat4_t Result = HMM_PREFIX(MultiplyMat4f)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyQuaternionFOp, 1)
HMM_INLINE hmm_quaternion operator*(hmm_quaternion left, float right)
{
    ASSERT_COVERED(HMM_MultiplyQuaternionFOp);

    hmm_quaternion Result = HMM_PREFIX(MultiplyQuaternionF)(left, right);

    return (Result);
}

COVERAGE(HMM_MultiplyVec2fOpLeft, 1)
HMM_INLINE vec2_t operator*(float left, vec2_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec2fOpLeft);

    vec2_t Result = HMM_PREFIX(MultiplyVec2f)(right, left);

    return (Result);
}

COVERAGE(HMM_MultiplyVec3fOpLeft, 1)
HMM_INLINE vec3_t operator*(float left, vec3_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec3fOpLeft);

    vec3_t Result = HMM_PREFIX(MultiplyVec3f)(right, left);

    return (Result);
}

COVERAGE(HMM_MultiplyVec4fOpLeft, 1)
HMM_INLINE vec4_t operator*(float left, vec4_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec4fOpLeft);

    vec4_t Result = HMM_PREFIX(MultiplyVec4f)(right, left);

    return (Result);
}

COVERAGE(HMM_MultiplyMat4fOpLeft, 1)
HMM_INLINE mat4_t operator*(float left, mat4_t right)
{
    ASSERT_COVERED(HMM_MultiplyMat4fOpLeft);

    mat4_t Result = HMM_PREFIX(MultiplyMat4f)(right, left);

    return (Result);
}

COVERAGE(HMM_MultiplyQuaternionFOpLeft, 1)
HMM_INLINE hmm_quaternion operator*(float left, hmm_quaternion right)
{
    ASSERT_COVERED(HMM_MultiplyQuaternionFOpLeft);

    hmm_quaternion Result = HMM_PREFIX(MultiplyQuaternionF)(right, left);

    return (Result);
}

COVERAGE(HMM_MultiplyMat4ByVec4Op, 1)
HMM_INLINE vec4_t operator*(mat4_t Matrix, vec4_t Vector)
{
    ASSERT_COVERED(HMM_MultiplyMat4ByVec4Op);

    vec4_t Result = HMM_PREFIX(MultiplyMat4ByVec4)(Matrix, Vector);

    return (Result);
}

COVERAGE(HMM_DivideVec2Op, 1)
HMM_INLINE vec2_t operator/(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_DivideVec2Op);

    vec2_t Result = HMM_PREFIX(DivideVec2)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideVec3Op, 1)
HMM_INLINE vec3_t operator/(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_DivideVec3Op);

    vec3_t Result = HMM_PREFIX(DivideVec3)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideVec4Op, 1)
HMM_INLINE vec4_t operator/(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_DivideVec4Op);

    vec4_t Result = HMM_PREFIX(DivideVec4)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideVec2fOp, 1)
HMM_INLINE vec2_t operator/(vec2_t left, float right)
{
    ASSERT_COVERED(HMM_DivideVec2fOp);

    vec2_t Result = HMM_PREFIX(DivideVec2f)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideVec3fOp, 1)
HMM_INLINE vec3_t operator/(vec3_t left, float right)
{
    ASSERT_COVERED(HMM_DivideVec3fOp);

    vec3_t Result = HMM_PREFIX(DivideVec3f)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideVec4fOp, 1)
HMM_INLINE vec4_t operator/(vec4_t left, float right)
{
    ASSERT_COVERED(HMM_DivideVec4fOp);

    vec4_t Result = HMM_PREFIX(DivideVec4f)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideMat4fOp, 1)
HMM_INLINE mat4_t operator/(mat4_t left, float right)
{
    ASSERT_COVERED(HMM_DivideMat4fOp);

    mat4_t Result = HMM_PREFIX(DivideMat4f)(left, right);

    return (Result);
}

COVERAGE(HMM_DivideQuaternionFOp, 1)
HMM_INLINE hmm_quaternion operator/(hmm_quaternion left, float right)
{
    ASSERT_COVERED(HMM_DivideQuaternionFOp);

    hmm_quaternion Result = HMM_PREFIX(DivideQuaternionF)(left, right);

    return (Result);
}

COVERAGE(HMM_AddVec2Assign, 1)
HMM_INLINE vec2_t &operator+=(vec2_t &left, vec2_t right)
{
    ASSERT_COVERED(HMM_AddVec2Assign);

    return (left = left + right);
}

COVERAGE(HMM_AddVec3Assign, 1)
HMM_INLINE vec3_t &operator+=(vec3_t &left, vec3_t right)
{
    ASSERT_COVERED(HMM_AddVec3Assign);

    return (left = left + right);
}

COVERAGE(HMM_AddVec4Assign, 1)
HMM_INLINE vec4_t &operator+=(vec4_t &left, vec4_t right)
{
    ASSERT_COVERED(HMM_AddVec4Assign);

    return (left = left + right);
}

COVERAGE(HMM_AddMat4Assign, 1)
HMM_INLINE mat4_t &operator+=(mat4_t &left, mat4_t right)
{
    ASSERT_COVERED(HMM_AddMat4Assign);

    return (left = left + right);
}

COVERAGE(HMM_AddQuaternionAssign, 1)
HMM_INLINE hmm_quaternion &operator+=(hmm_quaternion &left, hmm_quaternion right)
{
    ASSERT_COVERED(HMM_AddQuaternionAssign);

    return (left = left + right);
}

COVERAGE(HMM_SubtractVec2Assign, 1)
HMM_INLINE vec2_t &operator-=(vec2_t &left, vec2_t right)
{
    ASSERT_COVERED(HMM_SubtractVec2Assign);

    return (left = left - right);
}

COVERAGE(HMM_SubtractVec3Assign, 1)
HMM_INLINE vec3_t &operator-=(vec3_t &left, vec3_t right)
{
    ASSERT_COVERED(HMM_SubtractVec3Assign);

    return (left = left - right);
}

COVERAGE(HMM_SubtractVec4Assign, 1)
HMM_INLINE vec4_t &operator-=(vec4_t &left, vec4_t right)
{
    ASSERT_COVERED(HMM_SubtractVec4Assign);

    return (left = left - right);
}

COVERAGE(HMM_SubtractMat4Assign, 1)
HMM_INLINE mat4_t &operator-=(mat4_t &left, mat4_t right)
{
    ASSERT_COVERED(HMM_SubtractMat4Assign);

    return (left = left - right);
}

COVERAGE(HMM_SubtractQuaternionAssign, 1)
HMM_INLINE hmm_quaternion &operator-=(hmm_quaternion &left, hmm_quaternion right)
{
    ASSERT_COVERED(HMM_SubtractQuaternionAssign);

    return (left = left - right);
}

COVERAGE(HMM_MultiplyVec2Assign, 1)
HMM_INLINE vec2_t &operator*=(vec2_t &left, vec2_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec2Assign);

    return (left = left * right);
}

COVERAGE(HMM_MultiplyVec3Assign, 1)
HMM_INLINE vec3_t &operator*=(vec3_t &left, vec3_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec3Assign);

    return (left = left * right);
}

COVERAGE(HMM_MultiplyVec4Assign, 1)
HMM_INLINE vec4_t &operator*=(vec4_t &left, vec4_t right)
{
    ASSERT_COVERED(HMM_MultiplyVec4Assign);

    return (left = left * right);
}

COVERAGE(HMM_MultiplyVec2fAssign, 1)
HMM_INLINE vec2_t &operator*=(vec2_t &left, float right)
{
    ASSERT_COVERED(HMM_MultiplyVec2fAssign);

    return (left = left * right);
}

COVERAGE(HMM_MultiplyVec3fAssign, 1)
HMM_INLINE vec3_t &operator*=(vec3_t &left, float right)
{
    ASSERT_COVERED(HMM_MultiplyVec3fAssign);

    return (left = left * right);
}

COVERAGE(HMM_MultiplyVec4fAssign, 1)
HMM_INLINE vec4_t &operator*=(vec4_t &left, float right)
{
    ASSERT_COVERED(HMM_MultiplyVec4fAssign);

    return (left = left * right);
}

COVERAGE(HMM_MultiplyMat4fAssign, 1)
HMM_INLINE mat4_t &operator*=(mat4_t &left, float right)
{
    ASSERT_COVERED(HMM_MultiplyMat4fAssign);

    return (left = left * right);
}

COVERAGE(HMM_MultiplyQuaternionFAssign, 1)
HMM_INLINE hmm_quaternion &operator*=(hmm_quaternion &left, float right)
{
    ASSERT_COVERED(HMM_MultiplyQuaternionFAssign);

    return (left = left * right);
}

COVERAGE(HMM_DivideVec2Assign, 1)
HMM_INLINE vec2_t &operator/=(vec2_t &left, vec2_t right)
{
    ASSERT_COVERED(HMM_DivideVec2Assign);

    return (left = left / right);
}

COVERAGE(HMM_DivideVec3Assign, 1)
HMM_INLINE vec3_t &operator/=(vec3_t &left, vec3_t right)
{
    ASSERT_COVERED(HMM_DivideVec3Assign);

    return (left = left / right);
}

COVERAGE(HMM_DivideVec4Assign, 1)
HMM_INLINE vec4_t &operator/=(vec4_t &left, vec4_t right)
{
    ASSERT_COVERED(HMM_DivideVec4Assign);

    return (left = left / right);
}

COVERAGE(HMM_DivideVec2fAssign, 1)
HMM_INLINE vec2_t &operator/=(vec2_t &left, float right)
{
    ASSERT_COVERED(HMM_DivideVec2fAssign);

    return (left = left / right);
}

COVERAGE(HMM_DivideVec3fAssign, 1)
HMM_INLINE vec3_t &operator/=(vec3_t &left, float right)
{
    ASSERT_COVERED(HMM_DivideVec3fAssign);

    return (left = left / right);
}

COVERAGE(HMM_DivideVec4fAssign, 1)
HMM_INLINE vec4_t &operator/=(vec4_t &left, float right)
{
    ASSERT_COVERED(HMM_DivideVec4fAssign);

    return (left = left / right);
}

COVERAGE(HMM_DivideMat4fAssign, 1)
HMM_INLINE mat4_t &operator/=(mat4_t &left, float right)
{
    ASSERT_COVERED(HMM_DivideMat4fAssign);

    return (left = left / right);
}

COVERAGE(HMM_DivideQuaternionFAssign, 1)
HMM_INLINE hmm_quaternion &operator/=(hmm_quaternion &left, float right)
{
    ASSERT_COVERED(HMM_DivideQuaternionFAssign);

    return (left = left / right);
}

COVERAGE(HMM_EqualsVec2Op, 1)
HMM_INLINE hmm_bool operator==(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_EqualsVec2Op);

    return HMM_PREFIX(EqualsVec2)(left, right);
}

COVERAGE(HMM_EqualsVec3Op, 1)
HMM_INLINE hmm_bool operator==(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_EqualsVec3Op);

    return HMM_PREFIX(EqualsVec3)(left, right);
}

COVERAGE(HMM_EqualsVec4Op, 1)
HMM_INLINE hmm_bool operator==(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_EqualsVec4Op);

    return HMM_PREFIX(EqualsVec4)(left, right);
}

COVERAGE(HMM_EqualsVec2OpNot, 1)
HMM_INLINE hmm_bool operator!=(vec2_t left, vec2_t right)
{
    ASSERT_COVERED(HMM_EqualsVec2OpNot);

    return !HMM_PREFIX(EqualsVec2)(left, right);
}

COVERAGE(HMM_EqualsVec3OpNot, 1)
HMM_INLINE hmm_bool operator!=(vec3_t left, vec3_t right)
{
    ASSERT_COVERED(HMM_EqualsVec3OpNot);

    return !HMM_PREFIX(EqualsVec3)(left, right);
}

COVERAGE(HMM_EqualsVec4OpNot, 1)
HMM_INLINE hmm_bool operator!=(vec4_t left, vec4_t right)
{
    ASSERT_COVERED(HMM_EqualsVec4OpNot);

    return !HMM_PREFIX(EqualsVec4)(left, right);
}

COVERAGE(HMM_UnaryMinusVec2, 1)
HMM_INLINE vec2_t operator-(vec2_t In)
{
    ASSERT_COVERED(HMM_UnaryMinusVec2);

    vec2_t Result;
    Result.x = -In.x;
    Result.y = -In.y;
    return(Result);
}

COVERAGE(HMM_UnaryMinusVec3, 1)
HMM_INLINE vec3_t operator-(vec3_t In)
{
    ASSERT_COVERED(HMM_UnaryMinusVec3);

    vec3_t Result;
    Result.x = -In.x;
    Result.y = -In.y;
    Result.z = -In.z;
    return(Result);
}

COVERAGE(HMM_UnaryMinusVec4, 1)
HMM_INLINE vec4_t operator-(vec4_t In)
{
    ASSERT_COVERED(HMM_UnaryMinusVec4);

    vec4_t Result;
#if HANDMADE_MATH__USE_SSE
    Result._internal_sse = _mm_xor_ps(In._internal_sse, _mm_set1_ps(-0.0f));
#else
    Result.x = -In.x;
    Result.y = -In.y;
    Result.z = -In.z;
    Result.w = -In.w;
#endif
    return(Result);
}

#endif /* __cplusplus */

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#endif /* HANDMADE_MATH_H */



