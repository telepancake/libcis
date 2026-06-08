// Tests for <type_traits>, ported from libc++ test suite.
// Each test_* function exercises one or more traits via static_assert (compile-time)
// and CHECK() (runtime). No main(); it's auto-generated.
#include <type_traits>
#include "test.h"

//===----------------------------------------------------------------------===//
// Helpers
//===----------------------------------------------------------------------===//
struct Empty {};
struct NotEmpty { int x; };
struct VirtDestr { virtual ~VirtDestr() {} };
struct Abstract { virtual void f() = 0; };
struct ConcreteFromAbstract : Abstract { void f() override {} };
struct Final final {};
struct Polymorphic { virtual void f(); };
struct Base {};
struct Derived : Base {};
union Union1 { int x; float y; };
enum Enum1 { A, B };
enum class ScopedEnum { X, Y };
enum class ScopedEnum2 : short { P, Q };
struct Aggregate { int x; float y; };
struct WithVirtualDtor { virtual ~WithVirtualDtor() = default; };
struct NonDestructible { ~NonDestructible() = delete; };
struct Trivial { int x; };

//===----------------------------------------------------------------------===//
// test_integral_constant
//===----------------------------------------------------------------------===//
void test_integral_constant() {
    static_assert(std::true_type::value == true);
    static_assert(std::false_type::value == false);
    static_assert(std::integral_constant<int, 5>::value == 5);
    static_assert(std::bool_constant<true>::value == true);
    static_assert(std::bool_constant<false>::value == false);

    std::true_type t;
    CHECK(t.operator bool() == true);
    CHECK(t() == true);
}

//===----------------------------------------------------------------------===//
// test_is_same
//===----------------------------------------------------------------------===//
void test_is_same() {
    static_assert(std::is_same_v<int, int>);
    static_assert(!std::is_same_v<int, unsigned int>);
    static_assert(!std::is_same_v<int, const int>);
    static_assert(!std::is_same_v<int, float>);
    static_assert(std::is_same_v<void, void>);
    static_assert(std::is_same_v<int*, int*>);
    static_assert(!std::is_same_v<int*, const int*>);

    static_assert(std::is_same<int, int>::value);
    static_assert(!std::is_same<int, float>::value);
}

//===----------------------------------------------------------------------===//
// test_remove_cv
//===----------------------------------------------------------------------===//
void test_remove_cv() {
    static_assert(std::is_same_v<std::remove_const_t<const int>, int>);
    static_assert(std::is_same_v<std::remove_const_t<int>, int>);
    static_assert(std::is_same_v<std::remove_volatile_t<volatile int>, int>);
    static_assert(std::is_same_v<std::remove_volatile_t<int>, int>);
    static_assert(std::is_same_v<std::remove_cv_t<const volatile int>, int>);
    static_assert(std::is_same_v<std::remove_cv_t<const int>, int>);
    static_assert(std::is_same_v<std::remove_cv_t<volatile int>, int>);
    static_assert(std::is_same_v<std::remove_cv_t<int>, int>);
    // Pointer constness is not removed by remove_cv
    static_assert(std::is_same_v<std::remove_cv_t<const int*>, const int*>);
}

//===----------------------------------------------------------------------===//
// test_add_cv
//===----------------------------------------------------------------------===//
void test_add_cv() {
    static_assert(std::is_same_v<std::add_const_t<int>, const int>);
    static_assert(std::is_same_v<std::add_volatile_t<int>, volatile int>);
    static_assert(std::is_same_v<std::add_cv_t<int>, const volatile int>);
    // Already const stays const
    static_assert(std::is_same_v<std::add_const_t<const int>, const int>);
}

//===----------------------------------------------------------------------===//
// test_remove_reference
//===----------------------------------------------------------------------===//
void test_remove_reference() {
    static_assert(std::is_same_v<std::remove_reference_t<int&>, int>);
    static_assert(std::is_same_v<std::remove_reference_t<int&&>, int>);
    static_assert(std::is_same_v<std::remove_reference_t<int>, int>);
    static_assert(std::is_same_v<std::remove_reference_t<const int&>, const int>);
}

//===----------------------------------------------------------------------===//
// test_remove_cvref
//===----------------------------------------------------------------------===//
void test_remove_cvref() {
    static_assert(std::is_same_v<std::remove_cvref_t<const int&>, int>);
    static_assert(std::is_same_v<std::remove_cvref_t<volatile int&&>, int>);
    static_assert(std::is_same_v<std::remove_cvref_t<int>, int>);
    static_assert(std::is_same_v<std::remove_cvref_t<const volatile int&>, int>);
    // Array reference
    static_assert(std::is_same_v<std::remove_cvref_t<int(&)[3]>, int[3]>);
}

//===----------------------------------------------------------------------===//
// test_add_reference
//===----------------------------------------------------------------------===//
void test_add_reference() {
    static_assert(std::is_same_v<std::add_lvalue_reference_t<int>, int&>);
    static_assert(std::is_same_v<std::add_rvalue_reference_t<int>, int&&>);
    // References collapse
    static_assert(std::is_same_v<std::add_lvalue_reference_t<int&>, int&>);
    static_assert(std::is_same_v<std::add_rvalue_reference_t<int&>, int&>);
    // void
    static_assert(std::is_same_v<std::add_lvalue_reference_t<void>, void>);
    static_assert(std::is_same_v<std::add_rvalue_reference_t<void>, void>);
}

//===----------------------------------------------------------------------===//
// test_remove_pointer
//===----------------------------------------------------------------------===//
void test_remove_pointer() {
    static_assert(std::is_same_v<std::remove_pointer_t<int*>, int>);
    static_assert(std::is_same_v<std::remove_pointer_t<int>, int>);
    static_assert(std::is_same_v<std::remove_pointer_t<const int*>, const int>);
    static_assert(std::is_same_v<std::remove_pointer_t<int* const>, int>);
    static_assert(std::is_same_v<std::remove_pointer_t<int**>, int*>);
}

//===----------------------------------------------------------------------===//
// test_add_pointer
//===----------------------------------------------------------------------===//
void test_add_pointer() {
    static_assert(std::is_same_v<std::add_pointer_t<int>, int*>);
    static_assert(std::is_same_v<std::add_pointer_t<const int>, const int*>);
    static_assert(std::is_same_v<std::add_pointer_t<int&>, int*>);  // lvalue ref -> T*
    static_assert(std::is_same_v<std::add_pointer_t<int&&>, int*>); // rval ref -> T*
    static_assert(std::is_same_v<std::add_pointer_t<void>, void*>);
}

//===----------------------------------------------------------------------===//
// test_remove_extent
//===----------------------------------------------------------------------===//
void test_remove_extent() {
    static_assert(std::is_same_v<std::remove_extent_t<int[3]>, int>);
    static_assert(std::is_same_v<std::remove_extent_t<int[]>, int>);
    static_assert(std::is_same_v<std::remove_extent_t<int[3][4]>, int[4]>);
    static_assert(std::is_same_v<std::remove_extent_t<int>, int>);
}

//===----------------------------------------------------------------------===//
// test_remove_all_extents
//===----------------------------------------------------------------------===//
void test_remove_all_extents() {
    static_assert(std::is_same_v<std::remove_all_extents_t<int[3]>, int>);
    static_assert(std::is_same_v<std::remove_all_extents_t<int[][3]>, int>);
    static_assert(std::is_same_v<std::remove_all_extents_t<int[3][4][5]>, int>);
    static_assert(std::is_same_v<std::remove_all_extents_t<int>, int>);
}

//===----------------------------------------------------------------------===//
// test_is_void
//===----------------------------------------------------------------------===//
void test_is_void() {
    static_assert(std::is_void_v<void>);
    static_assert(std::is_void_v<const void>);
    static_assert(std::is_void_v<volatile void>);
    static_assert(!std::is_void_v<int>);
    static_assert(!std::is_void_v<void*>);
}

//===----------------------------------------------------------------------===//
// test_is_null_pointer
//===----------------------------------------------------------------------===//
void test_is_null_pointer() {
    static_assert(std::is_null_pointer_v<decltype(nullptr)>);
    static_assert(std::is_null_pointer_v<std::nullptr_t>);
    static_assert(!std::is_null_pointer_v<int>);
    static_assert(!std::is_null_pointer_v<int*>);
}

//===----------------------------------------------------------------------===//
// test_is_integral
//===----------------------------------------------------------------------===//
void test_is_integral() {
    static_assert(std::is_integral_v<bool>);
    static_assert(std::is_integral_v<char>);
    static_assert(std::is_integral_v<int>);
    static_assert(std::is_integral_v<long>);
    static_assert(std::is_integral_v<long long>);
    static_assert(std::is_integral_v<unsigned int>);
    static_assert(std::is_integral_v<const int>);
    static_assert(!std::is_integral_v<float>);
    static_assert(!std::is_integral_v<double>);
    static_assert(!std::is_integral_v<void>);
    static_assert(!std::is_integral_v<int*>);
    static_assert(!std::is_integral_v<Enum1>);
}

//===----------------------------------------------------------------------===//
// test_is_floating_point
//===----------------------------------------------------------------------===//
void test_is_floating_point() {
    static_assert(std::is_floating_point_v<float>);
    static_assert(std::is_floating_point_v<double>);
    static_assert(std::is_floating_point_v<long double>);
    static_assert(std::is_floating_point_v<const float>);
    static_assert(!std::is_floating_point_v<int>);
    static_assert(!std::is_floating_point_v<bool>);
}

//===----------------------------------------------------------------------===//
// test_is_array
//===----------------------------------------------------------------------===//
void test_is_array() {
    static_assert(std::is_array_v<int[3]>);
    static_assert(std::is_array_v<int[]>);
    static_assert(!std::is_array_v<int>);
    static_assert(!std::is_array_v<int*>);

    static_assert(std::is_bounded_array_v<int[3]>);
    static_assert(!std::is_bounded_array_v<int[]>);
    static_assert(!std::is_bounded_array_v<int>);

    static_assert(std::is_unbounded_array_v<int[]>);
    static_assert(!std::is_unbounded_array_v<int[3]>);
    static_assert(!std::is_unbounded_array_v<int>);
}

//===----------------------------------------------------------------------===//
// test_is_pointer
//===----------------------------------------------------------------------===//
void test_is_pointer() {
    static_assert(std::is_pointer_v<int*>);
    static_assert(std::is_pointer_v<const int*>);
    static_assert(std::is_pointer_v<void*>);
    static_assert(!std::is_pointer_v<int>);
    static_assert(!std::is_pointer_v<int&>);
    static_assert(!std::is_pointer_v<int[]>);
    // member pointer is NOT a pointer
    static_assert(!std::is_pointer_v<int Base::*>);
}

//===----------------------------------------------------------------------===//
// test_is_reference
//===----------------------------------------------------------------------===//
void test_is_reference() {
    static_assert(std::is_lvalue_reference_v<int&>);
    static_assert(!std::is_lvalue_reference_v<int>);
    static_assert(!std::is_lvalue_reference_v<int&&>);

    static_assert(std::is_rvalue_reference_v<int&&>);
    static_assert(!std::is_rvalue_reference_v<int>);
    static_assert(!std::is_rvalue_reference_v<int&>);

    static_assert(std::is_reference_v<int&>);
    static_assert(std::is_reference_v<int&&>);
    static_assert(!std::is_reference_v<int>);
}

//===----------------------------------------------------------------------===//
// test_is_function
//===----------------------------------------------------------------------===//
void test_is_function() {
    static_assert(std::is_function_v<void()>);
    static_assert(std::is_function_v<int(int, float)>);
    static_assert(!std::is_function_v<int>);
    static_assert(!std::is_function_v<void(*)()>);  // pointer to function
    static_assert(!std::is_function_v<int&>);
}

//===----------------------------------------------------------------------===//
// test_is_member_pointer
//===----------------------------------------------------------------------===//
void test_is_member_pointer() {
    static_assert(std::is_member_pointer_v<int Base::*>);
    static_assert(std::is_member_pointer_v<void (Base::*)()>);
    static_assert(!std::is_member_pointer_v<int>);
    static_assert(!std::is_member_pointer_v<int*>);

    static_assert(std::is_member_object_pointer_v<int Base::*>);
    static_assert(!std::is_member_object_pointer_v<void (Base::*)()>);

    static_assert(std::is_member_function_pointer_v<void (Base::*)()>);
    static_assert(!std::is_member_function_pointer_v<int Base::*>);
}

//===----------------------------------------------------------------------===//
// test_is_class_union_enum
//===----------------------------------------------------------------------===//
void test_is_class_union_enum() {
    static_assert(std::is_class_v<Base>);
    static_assert(std::is_class_v<Derived>);
    static_assert(!std::is_class_v<int>);
    static_assert(!std::is_class_v<Union1>);

    static_assert(std::is_union_v<Union1>);
    static_assert(!std::is_union_v<Base>);
    static_assert(!std::is_union_v<int>);

    static_assert(std::is_enum_v<Enum1>);
    static_assert(std::is_enum_v<ScopedEnum>);
    static_assert(!std::is_enum_v<int>);
    static_assert(!std::is_enum_v<Base>);
}

//===----------------------------------------------------------------------===//
// test_is_arithmetic
//===----------------------------------------------------------------------===//
void test_is_arithmetic() {
    static_assert(std::is_arithmetic_v<int>);
    static_assert(std::is_arithmetic_v<float>);
    static_assert(std::is_arithmetic_v<bool>);
    static_assert(!std::is_arithmetic_v<void>);
    static_assert(!std::is_arithmetic_v<int*>);
    static_assert(!std::is_arithmetic_v<Enum1>);
}

//===----------------------------------------------------------------------===//
// test_is_fundamental
//===----------------------------------------------------------------------===//
void test_is_fundamental() {
    static_assert(std::is_fundamental_v<int>);
    static_assert(std::is_fundamental_v<void>);
    static_assert(std::is_fundamental_v<float>);
    static_assert(std::is_fundamental_v<std::nullptr_t>);
    static_assert(!std::is_fundamental_v<Base>);
    static_assert(!std::is_fundamental_v<int*>);
    static_assert(!std::is_fundamental_v<Enum1>);
}

//===----------------------------------------------------------------------===//
// test_is_compound_object_scalar
//===----------------------------------------------------------------------===//
void test_is_compound_object_scalar() {
    static_assert(!std::is_compound_v<int>);
    static_assert(std::is_compound_v<Base>);
    static_assert(std::is_compound_v<int*>);
    static_assert(std::is_compound_v<Enum1>);

    static_assert(std::is_object_v<int>);
    static_assert(std::is_object_v<Base>);
    static_assert(!std::is_object_v<void>);
    static_assert(!std::is_object_v<int&>);
    static_assert(!std::is_object_v<void()>);

    static_assert(std::is_scalar_v<int>);
    static_assert(std::is_scalar_v<int*>);
    static_assert(std::is_scalar_v<Enum1>);
    static_assert(std::is_scalar_v<std::nullptr_t>);
    static_assert(!std::is_scalar_v<Base>);
    static_assert(!std::is_scalar_v<void>);
}

//===----------------------------------------------------------------------===//
// test_is_const_volatile
//===----------------------------------------------------------------------===//
void test_is_const_volatile() {
    static_assert(std::is_const_v<const int>);
    static_assert(!std::is_const_v<int>);
    static_assert(std::is_const_v<const volatile int>);

    static_assert(std::is_volatile_v<volatile int>);
    static_assert(!std::is_volatile_v<int>);
    static_assert(std::is_volatile_v<const volatile int>);
}

//===----------------------------------------------------------------------===//
// test_is_signed_unsigned
//===----------------------------------------------------------------------===//
void test_is_signed_unsigned() {
    static_assert(std::is_signed_v<int>);
    static_assert(std::is_signed_v<float>);
    static_assert(!std::is_signed_v<unsigned int>);
    static_assert(!std::is_signed_v<bool>);

    static_assert(std::is_unsigned_v<unsigned int>);
    static_assert(!std::is_unsigned_v<int>);
    // bool is unsigned in terms of is_unsigned
    static_assert(std::is_unsigned_v<bool>);
}

//===----------------------------------------------------------------------===//
// test_class_props
//===----------------------------------------------------------------------===//
void test_class_props() {
    static_assert(std::is_abstract_v<Abstract>);
    static_assert(!std::is_abstract_v<ConcreteFromAbstract>);
    static_assert(!std::is_abstract_v<int>);

    static_assert(std::is_polymorphic_v<Polymorphic>);
    static_assert(std::is_polymorphic_v<Abstract>);
    static_assert(!std::is_polymorphic_v<Base>);
    static_assert(!std::is_polymorphic_v<int>);

    static_assert(std::is_final_v<Final>);
    static_assert(!std::is_final_v<Base>);

    static_assert(std::is_empty_v<Empty>);
    static_assert(!std::is_empty_v<NotEmpty>);

    static_assert(std::is_aggregate_v<Aggregate>);
    static_assert(!std::is_aggregate_v<int>);

    static_assert(std::is_standard_layout_v<int>);
    static_assert(std::is_standard_layout_v<Trivial>);

    static_assert(std::is_trivial_v<int>);
    static_assert(std::is_trivial_v<Trivial>);
    static_assert(!std::is_trivial_v<Polymorphic>);

    static_assert(std::is_trivially_copyable_v<int>);
    static_assert(!std::is_trivially_copyable_v<Polymorphic>);

    static_assert(std::is_pod_v<int>);
    static_assert(std::is_pod_v<Trivial>);

    static_assert(std::has_virtual_destructor_v<WithVirtualDtor>);
    static_assert(!std::has_virtual_destructor_v<int>);
    static_assert(!std::has_virtual_destructor_v<Base>);
}

//===----------------------------------------------------------------------===//
// test_is_base_of
//===----------------------------------------------------------------------===//
void test_is_base_of() {
    static_assert(std::is_base_of_v<Base, Derived>);
    static_assert(std::is_base_of_v<Base, Base>);
    static_assert(!std::is_base_of_v<Derived, Base>);
    static_assert(!std::is_base_of_v<int, int>);
}

//===----------------------------------------------------------------------===//
// test_constructibility
//===----------------------------------------------------------------------===//
void test_constructibility() {
    static_assert(std::is_constructible_v<int, int>);
    static_assert(std::is_constructible_v<int>);
    static_assert(!std::is_constructible_v<NonDestructible>);

    static_assert(std::is_default_constructible_v<int>);
    static_assert(std::is_default_constructible_v<Base>);

    static_assert(std::is_copy_constructible_v<int>);
    static_assert(!std::is_copy_constructible_v<Abstract>);

    static_assert(std::is_move_constructible_v<int>);
    static_assert(!std::is_move_constructible_v<Abstract>);
}

//===----------------------------------------------------------------------===//
// test_assignability
//===----------------------------------------------------------------------===//
void test_assignability() {
    static_assert(std::is_assignable_v<int&, int>);
    static_assert(!std::is_assignable_v<int, int>);

    static_assert(std::is_copy_assignable_v<int>);
    static_assert(std::is_move_assignable_v<int>);
}

//===----------------------------------------------------------------------===//
// test_destructibility
//===----------------------------------------------------------------------===//
void test_destructibility() {
    static_assert(std::is_destructible_v<int>);
    static_assert(std::is_destructible_v<Base>);
    static_assert(!std::is_destructible_v<void>);
    static_assert(!std::is_destructible_v<int[]>);
    static_assert(!std::is_destructible_v<void()>);
    static_assert(!std::is_destructible_v<NonDestructible>);
}

//===----------------------------------------------------------------------===//
// test_trivially_constructible
//===----------------------------------------------------------------------===//
void test_trivially_constructible() {
    static_assert(std::is_trivially_constructible_v<int>);
    static_assert(std::is_trivially_default_constructible_v<int>);
    static_assert(std::is_trivially_copy_constructible_v<int>);
    static_assert(std::is_trivially_move_constructible_v<int>);
}

//===----------------------------------------------------------------------===//
// test_trivially_assignable
//===----------------------------------------------------------------------===//
void test_trivially_assignable() {
    static_assert(std::is_trivially_assignable_v<int&, int>);
    static_assert(std::is_trivially_copy_assignable_v<int>);
    static_assert(std::is_trivially_move_assignable_v<int>);
}

//===----------------------------------------------------------------------===//
// test_trivially_destructible
//===----------------------------------------------------------------------===//
void test_trivially_destructible() {
    static_assert(std::is_trivially_destructible_v<int>);
    static_assert(!std::is_trivially_destructible_v<VirtDestr>);
}

//===----------------------------------------------------------------------===//
// test_nothrow_constructible
//===----------------------------------------------------------------------===//
void test_nothrow_constructible() {
    static_assert(std::is_nothrow_constructible_v<int, int>);
    static_assert(std::is_nothrow_default_constructible_v<int>);
    static_assert(std::is_nothrow_copy_constructible_v<int>);
    static_assert(std::is_nothrow_move_constructible_v<int>);
}

//===----------------------------------------------------------------------===//
// test_nothrow_assignable
//===----------------------------------------------------------------------===//
void test_nothrow_assignable() {
    static_assert(std::is_nothrow_assignable_v<int&, int>);
    static_assert(std::is_nothrow_copy_assignable_v<int>);
    static_assert(std::is_nothrow_move_assignable_v<int>);
}

//===----------------------------------------------------------------------===//
// test_nothrow_destructible
//===----------------------------------------------------------------------===//
void test_nothrow_destructible() {
    static_assert(std::is_nothrow_destructible_v<int>);
    static_assert(!std::is_nothrow_destructible_v<void>);
    static_assert(std::is_nothrow_destructible_v<int&>);
    static_assert(std::is_nothrow_destructible_v<int&&>);
    static_assert(std::is_nothrow_destructible_v<int[3]>);
}

//===----------------------------------------------------------------------===//
// test_is_convertible
//===----------------------------------------------------------------------===//
void test_is_convertible() {
    static_assert(std::is_convertible_v<int, int>);
    static_assert(std::is_convertible_v<int, long>);
    static_assert(std::is_convertible_v<int, float>);
    static_assert(!std::is_convertible_v<int*, float*>);
    static_assert(std::is_convertible_v<void, void>);
    static_assert(!std::is_convertible_v<void, int>);
    static_assert(std::is_convertible_v<Derived*, Base*>);
    static_assert(!std::is_convertible_v<Base*, Derived*>);
}

//===----------------------------------------------------------------------===//
// test_is_nothrow_convertible
//===----------------------------------------------------------------------===//
void test_is_nothrow_convertible() {
    static_assert(std::is_nothrow_convertible_v<int, int>);
    static_assert(std::is_nothrow_convertible_v<int, long>);
    static_assert(!std::is_nothrow_convertible_v<int*, float*>);
}

//===----------------------------------------------------------------------===//
// test_is_scoped_enum
//===----------------------------------------------------------------------===//
void test_is_scoped_enum() {
    static_assert(std::is_scoped_enum_v<ScopedEnum>);
    static_assert(std::is_scoped_enum_v<ScopedEnum2>);
    static_assert(!std::is_scoped_enum_v<Enum1>);
    static_assert(!std::is_scoped_enum_v<int>);
}

//===----------------------------------------------------------------------===//
// test_underlying_type
//===----------------------------------------------------------------------===//
void test_underlying_type() {
    static_assert(std::is_same_v<std::underlying_type_t<ScopedEnum2>, short>);
    static_assert(std::is_same_v<std::underlying_type_t<Enum1>, unsigned int>);
}

//===----------------------------------------------------------------------===//
// test_swappable
//===----------------------------------------------------------------------===//
void test_swappable() {
    static_assert(std::is_swappable_v<int>);
    static_assert(std::is_swappable_v<Base>);
    static_assert(!std::is_swappable_v<void>);

    static_assert(std::is_nothrow_swappable_v<int>);
}

//===----------------------------------------------------------------------===//
// test_rank_extent
//===----------------------------------------------------------------------===//
void test_rank_extent() {
    static_assert(std::rank_v<int> == 0);
    static_assert(std::rank_v<int[]> == 1);
    static_assert(std::rank_v<int[3]> == 1);
    static_assert(std::rank_v<int[3][4]> == 2);

    static_assert(std::extent_v<int> == 0);
    static_assert(std::extent_v<int[]> == 0);
    static_assert(std::extent_v<int[3]> == 3);
    static_assert(std::extent_v<int[3][4]> == 3);
    static_assert(std::extent_v<int[3][4], 1> == 4);
}

//===----------------------------------------------------------------------===//
// test_alignment_of
//===----------------------------------------------------------------------===//
void test_alignment_of() {
    static_assert(std::alignment_of_v<int> == alignof(int));
    static_assert(std::alignment_of_v<double> == alignof(double));
    static_assert(std::alignment_of<char>::value == 1);
}

//===----------------------------------------------------------------------===//
// test_make_signed_unsigned
//===----------------------------------------------------------------------===//
void test_make_signed_unsigned() {
    static_assert(std::is_same_v<std::make_signed_t<unsigned int>, int>);
    static_assert(std::is_same_v<std::make_signed_t<int>, int>);
    static_assert(std::is_same_v<std::make_signed_t<unsigned long>, long>);
    static_assert(std::is_same_v<std::make_signed_t<unsigned long long>, long long>);
    static_assert(std::is_same_v<std::make_signed_t<unsigned short>, short>);
    static_assert(std::is_same_v<std::make_signed_t<unsigned char>, signed char>);

    static_assert(std::is_same_v<std::make_unsigned_t<int>, unsigned int>);
    static_assert(std::is_same_v<std::make_unsigned_t<unsigned int>, unsigned int>);
    static_assert(std::is_same_v<std::make_unsigned_t<long>, unsigned long>);
    static_assert(std::is_same_v<std::make_unsigned_t<signed char>, unsigned char>);

    // CV qualifiers are preserved
    static_assert(std::is_same_v<std::make_signed_t<const unsigned int>, const int>);
    static_assert(std::is_same_v<std::make_unsigned_t<const int>, const unsigned int>);
}

//===----------------------------------------------------------------------===//
// test_decay
//===----------------------------------------------------------------------===//
void test_decay() {
    static_assert(std::is_same_v<std::decay_t<int>, int>);
    static_assert(std::is_same_v<std::decay_t<const int>, int>);
    static_assert(std::is_same_v<std::decay_t<int&>, int>);
    static_assert(std::is_same_v<std::decay_t<int&&>, int>);
    static_assert(std::is_same_v<std::decay_t<const int&>, int>);
    static_assert(std::is_same_v<std::decay_t<int[3]>, int*>);
    static_assert(std::is_same_v<std::decay_t<int()>, int(*)()>);
    static_assert(std::is_same_v<std::decay_t<int(int)>, int(*)(int)>);
}

//===----------------------------------------------------------------------===//
// test_conditional
//===----------------------------------------------------------------------===//
void test_conditional() {
    static_assert(std::is_same_v<std::conditional_t<true, int, float>, int>);
    static_assert(std::is_same_v<std::conditional_t<false, int, float>, float>);
}

//===----------------------------------------------------------------------===//
// test_enable_if
//===----------------------------------------------------------------------===//
void test_enable_if() {
    static_assert(std::is_same_v<std::enable_if_t<true, int>, int>);
    static_assert(std::is_same_v<std::enable_if_t<true>, void>);
    // enable_if<false> has no ::type — tested via SFINAE in code, not here.
}

//===----------------------------------------------------------------------===//
// test_common_type
//===----------------------------------------------------------------------===//
void test_common_type() {
    static_assert(std::is_same_v<std::common_type_t<int, int>, int>);
    static_assert(std::is_same_v<std::common_type_t<int, long>, long>);
    static_assert(std::is_same_v<std::common_type_t<int>, int>);
    static_assert(std::is_same_v<std::common_type_t<int, int, long>, long>);
}

//===----------------------------------------------------------------------===//
// test_type_identity
//===----------------------------------------------------------------------===//
void test_type_identity() {
    static_assert(std::is_same_v<std::type_identity_t<int>, int>);
    static_assert(std::is_same_v<std::type_identity_t<const int>, const int>);
    static_assert(std::is_same_v<std::type_identity_t<int&>, int&>);
}

//===----------------------------------------------------------------------===//
// test_conjunction_disjunction_negation
//===----------------------------------------------------------------------===//
void test_conjunction_disjunction_negation() {
    static_assert(std::conjunction_v<std::true_type, std::true_type>);
    static_assert(!std::conjunction_v<std::true_type, std::false_type>);
    static_assert(!std::conjunction_v<std::false_type, std::true_type>);
    static_assert(std::conjunction_v<>);

    static_assert(std::disjunction_v<std::true_type, std::false_type>);
    static_assert(!std::disjunction_v<std::false_type, std::false_type>);
    static_assert(!std::disjunction_v<>);

    static_assert(std::negation_v<std::false_type>);
    static_assert(!std::negation_v<std::true_type>);
}

//===----------------------------------------------------------------------===//
// test_void_t
//===----------------------------------------------------------------------===//
void test_void_t() {
    static_assert(std::is_same_v<std::void_t<int, float, void>, void>);
    static_assert(std::is_same_v<std::void_t<>, void>);
}

//===----------------------------------------------------------------------===//
// test_type_traits_common_reference_basic
// Ported from libc++ test/std/utilities/meta/meta.trans/meta.trans.other/
//   common_reference.compile.pass.cpp
//===----------------------------------------------------------------------===//
struct CRBase {};
struct CRDerived : CRBase {};

// g++-10: "requires { typename T::type; }" only works as a named template,
// not inline inside static_assert.  Use this helper for "no type" checks.
template<class T>
constexpr bool cr_has_type = requires { typename T::type; };

void test_type_traits_common_reference_basic() {
    // (6.1) zero types: no ::type
    static_assert(!cr_has_type<std::common_reference<>>);

    // (6.2) one type: identity
    static_assert(std::is_same_v<std::common_reference_t<void>, void>);
    static_assert(std::is_same_v<std::common_reference_t<int>, int>);
    static_assert(std::is_same_v<std::common_reference_t<int&>, int&>);
    static_assert(std::is_same_v<std::common_reference_t<int&&>, int&&>);
    static_assert(std::is_same_v<std::common_reference_t<int const>, int const>);
    static_assert(std::is_same_v<std::common_reference_t<int const&>, int const&>);
    static_assert(std::is_same_v<std::common_reference_t<int const&&>, int const&&>);
    static_assert(std::is_same_v<std::common_reference_t<void (&)()>, void (&)()>);
    static_assert(std::is_same_v<std::common_reference_t<void (&&)()>, void (&&)()>);
}

void test_type_traits_common_reference_refs() {
    // (6.3.1) two lvalue refs with COMMON-REF existing (pointer convertibility)
    static_assert(std::is_same_v<std::common_reference_t<CRBase&, CRDerived&>, CRBase&>);
    static_assert(std::is_same_v<std::common_reference_t<CRBase const&, CRDerived&>, CRBase const&>);
    static_assert(std::is_same_v<std::common_reference_t<CRBase&, CRDerived const&>, CRBase const&>);

    // two rvalue refs
    static_assert(std::is_same_v<std::common_reference_t<CRBase&&, CRDerived&&>, CRBase&&>);
    static_assert(std::is_same_v<std::common_reference_t<CRBase const&&, CRDerived&&>, CRBase const&&>);
    static_assert(std::is_same_v<std::common_reference_t<CRBase&&, CRDerived const&&>, CRBase const&&>);

    // mixed ref categories (rvalue + lvalue → const lvalue)
    static_assert(std::is_same_v<std::common_reference_t<CRBase&, CRDerived&&>, CRBase const&>);
    static_assert(std::is_same_v<std::common_reference_t<CRBase&&, CRDerived&>, CRBase const&>);
    static_assert(std::is_same_v<std::common_reference_t<CRBase const&, CRDerived&&>, CRBase const&>);

    // cv-qualifier merging
    static_assert(std::is_same_v<std::common_reference_t<int const&, int volatile&>, int const volatile&>);
    static_assert(std::is_same_v<std::common_reference_t<int const volatile&&, int volatile&&>, int const volatile&&>);

    // array refs
    static_assert(std::is_same_v<std::common_reference_t<int const (&)[10], int volatile (&)[10]>,
                                  int const volatile (&)[10]>);
}

void test_type_traits_common_reference_non_ref() {
    // (6.3.3) COND-RES fallback for non-reference types
    static_assert(std::is_same_v<std::common_reference_t<void, void>, void>);
    static_assert(std::is_same_v<std::common_reference_t<int, short>, int>);
    static_assert(std::is_same_v<std::common_reference_t<int, short&>, int>);
    static_assert(std::is_same_v<std::common_reference_t<int&, short&>, int>);

    // tricky volatile reference case → COND-RES falls back to prvalue int
    static_assert(std::is_same_v<std::common_reference_t<int&&, int volatile&>, int>);
    static_assert(std::is_same_v<std::common_reference_t<int volatile&, int&&>, int>);

    // array decay
    static_assert(std::is_same_v<std::common_reference_t<int (&)[10], int (&)[11]>, int*>);
}

void test_type_traits_common_reference_multi() {
    // (6.4) 3+ types
    static_assert(std::is_same_v<std::common_reference_t<int, int, int>, int>);
    static_assert(std::is_same_v<std::common_reference_t<int&&, int const&, int volatile&>,
                                  int const volatile&>);
    static_assert(std::is_same_v<std::common_reference_t<CRBase&, CRDerived const&, CRDerived&>, CRBase const&>);

    // (6.4.2) no common_reference → no type
    static_assert(!cr_has_type<std::common_reference<int, short, int, char*>>);
}

//===----------------------------------------------------------------------===//
// test_type_traits_invoke_result
// Ported from libc++ meta.trans.other / invoke_result tests.
//===----------------------------------------------------------------------===//
struct InvokeTag {};
struct InvokeDerived : InvokeTag {};

struct InvokeFnObj {
    int operator()(InvokeTag, int) const { return 0; }
    double operator()(double) const { return 0.0; }
};

int invoke_free_fn(InvokeTag, int) { return 0; }
int invoke_nothrow_fn(int) noexcept { return 0; }

void test_type_traits_invoke_result_basic() {
    // Free function pointer
    using Fp = int(*)(InvokeTag, int);
    static_assert(std::is_same_v<std::invoke_result_t<Fp, InvokeTag, int>, int>);
    static_assert(std::is_same_v<std::invoke_result_t<Fp, InvokeDerived, int>, int>);

    // Function object
    static_assert(std::is_same_v<std::invoke_result_t<InvokeFnObj, InvokeTag, int>, int>);
    static_assert(std::is_same_v<std::invoke_result_t<InvokeFnObj, double>, double>);

    // Member function pointer — bullet 1
    using Mfp = int(InvokeTag::*)(int);
    static_assert(std::is_same_v<std::invoke_result_t<Mfp, InvokeTag&, int>, int>);
    static_assert(std::is_same_v<std::invoke_result_t<Mfp, InvokeDerived&, int>, int>);

    // Member object pointer — bullet 4
    using Mop = int InvokeTag::*;
    static_assert(std::is_same_v<std::invoke_result_t<Mop, InvokeTag&>, int&>);

    // invoke_result: no ::type when not invocable
    static_assert(!cr_has_type<std::invoke_result<int, int>>);
    static_assert(!cr_has_type<std::invoke_result<void>>);
}

//===----------------------------------------------------------------------===//
// test_type_traits_invoke_is_invocable
// Ported from libc++ meta.rel/is_invocable.pass.cpp
//===----------------------------------------------------------------------===//

struct InvokeNotCallableWithInt {
    int operator()(int) = delete;
    int operator()(InvokeTag) { return 42; }
};

struct InvokeImplicit { InvokeImplicit(int) {} };
struct InvokeExplicit { explicit InvokeExplicit(int) {} };

void test_type_traits_invoke_is_invocable_basic() {
    using AbominableFunc = void(...) const;

    // Non-callable things
    static_assert(!std::is_invocable_v<void>);
    static_assert(!std::is_invocable_v<const void>);
    static_assert(!std::is_invocable_v<std::nullptr_t>);
    static_assert(!std::is_invocable_v<int>);
    static_assert(!std::is_invocable_v<double>);
    static_assert(!std::is_invocable_v<int[]>);
    static_assert(!std::is_invocable_v<int[3]>);
    static_assert(!std::is_invocable_v<int*>);
    static_assert(!std::is_invocable_v<int&>);
    static_assert(!std::is_invocable_v<int&&>);
    static_assert(!std::is_invocable_v<AbominableFunc>);

    // With parameters
    static_assert(!std::is_invocable_v<int, int>);
    static_assert(!std::is_invocable_v<int, double, float>);
}

void test_type_traits_invoke_is_invocable_memfn() {
    // Bullet 1: member function pointer, object/reference
    using Fn = int (InvokeTag::*)(int);
    using RFn = int (InvokeTag::*)(int)&&;
    static_assert(std::is_invocable_v<Fn, InvokeTag&, int>);
    static_assert(std::is_invocable_v<Fn, InvokeDerived&, int>);
    static_assert(std::is_invocable_v<RFn, InvokeTag&&, int>);
    static_assert(!std::is_invocable_v<RFn, InvokeTag&, int>);
    static_assert(!std::is_invocable_v<Fn, InvokeTag&>);           // too few args
    static_assert(!std::is_invocable_v<Fn, InvokeTag const&, int>);// wrong cv

    // Bullet 3: member function pointer via pointer
    using T = InvokeTag*;
    using DT = InvokeDerived*;
    static_assert(std::is_invocable_v<Fn, T&, int>);
    static_assert(std::is_invocable_v<Fn, DT&, int>);
    static_assert(std::is_invocable_v<Fn, T&&, int>);
    static_assert(!std::is_invocable_v<Fn, const InvokeTag*, int>);
}

void test_type_traits_invoke_is_invocable_memobj() {
    // Bullets 4 and 6: member object pointer
    using Fn = int(InvokeTag::*);
    static_assert(!std::is_invocable_v<Fn>);                        // no object arg

    // Bullet 4: direct object
    static_assert(std::is_invocable_v<Fn, InvokeTag&>);
    static_assert(std::is_invocable_v<Fn, InvokeDerived&>);
    static_assert(std::is_invocable_v<Fn, InvokeTag&&>);
    static_assert(std::is_invocable_v<Fn, InvokeTag const&>);

    // Bullet 6: via pointer
    static_assert(std::is_invocable_v<Fn, InvokeTag*>);
    static_assert(std::is_invocable_v<Fn, InvokeDerived*>);
    static_assert(std::is_invocable_v<Fn, InvokeTag* const>);
    static_assert(std::is_invocable_v<Fn, const InvokeTag*>);
}

void test_type_traits_invoke_is_invocable_callable() {
    // Bullet 7: plain callable
    using Fp = void(*)(InvokeTag&, int);
    static_assert(std::is_invocable_v<Fp, InvokeTag&, int>);
    static_assert(std::is_invocable_v<Fp, InvokeDerived&, int>);
    static_assert(!std::is_invocable_v<Fp, InvokeTag const&, int>);
    static_assert(!std::is_invocable_v<Fp>);
    static_assert(!std::is_invocable_v<Fp, InvokeTag&>);

    // Function object
    static_assert(std::is_invocable_v<InvokeNotCallableWithInt, InvokeTag>);
    static_assert(!std::is_invocable_v<InvokeNotCallableWithInt, int>);
}

void test_type_traits_invoke_is_invocable_r() {
    // is_invocable_r: return type convertibility check
    using Fn = int(*)();
    static_assert(std::is_invocable_r_v<int, Fn>);
    static_assert(std::is_invocable_r_v<InvokeImplicit, Fn>);
    static_assert(std::is_invocable_r_v<double, Fn>);
    static_assert(std::is_invocable_r_v<const volatile void, Fn>);
    static_assert(!std::is_invocable_r_v<InvokeExplicit, Fn>);  // explicit ctor

    // _v variants
    using Fv = void(*)();
    static_assert(std::is_invocable_v<Fv>);
    static_assert(!std::is_invocable_v<Fv, int>);
    static_assert(std::is_invocable_r_v<void, Fv>);
    static_assert(!std::is_invocable_r_v<int, Fv>);
}

void test_type_traits_invoke_is_nothrow_invocable() {
    // is_nothrow_invocable
    using FnNoexcept = int(*)(int) noexcept;
    using FnThrows   = int(*)(int);
    static_assert(std::is_nothrow_invocable_v<FnNoexcept, int>);
    static_assert(!std::is_nothrow_invocable_v<FnThrows, int>);
    static_assert(!std::is_nothrow_invocable_v<int>);  // not callable

    // is_nothrow_invocable_r
    static_assert(std::is_nothrow_invocable_r_v<int, FnNoexcept, int>);
    static_assert(!std::is_nothrow_invocable_r_v<int, FnThrows, int>);
    static_assert(std::is_nothrow_invocable_r_v<void, FnNoexcept, int>);
    static_assert(std::is_nothrow_invocable_r_v<long, FnNoexcept, int>); // int -> long noexcept
}
