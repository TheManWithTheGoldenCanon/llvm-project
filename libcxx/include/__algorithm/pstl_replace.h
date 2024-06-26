//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___ALGORITHM_PSTL_REPLACE_H
#define _LIBCPP___ALGORITHM_PSTL_REPLACE_H

#include <__algorithm/pstl_for_each.h>
#include <__algorithm/pstl_frontend_dispatch.h>
#include <__algorithm/pstl_transform.h>
#include <__config>
#include <__iterator/cpp17_iterator_concepts.h>
#include <__iterator/iterator_traits.h>
#include <__pstl/configuration.h>
#include <__type_traits/enable_if.h>
#include <__type_traits/remove_cvref.h>
#include <__utility/move.h>
#include <optional>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

#if !defined(_LIBCPP_HAS_NO_INCOMPLETE_PSTL) && _LIBCPP_STD_VER >= 17

_LIBCPP_BEGIN_NAMESPACE_STD

template <class>
void __pstl_replace_if();

template <class _ExecutionPolicy,
          class _ForwardIterator,
          class _Pred,
          class _Tp,
          class _RawPolicy                                    = __remove_cvref_t<_ExecutionPolicy>,
          enable_if_t<is_execution_policy_v<_RawPolicy>, int> = 0>
[[nodiscard]] _LIBCPP_HIDE_FROM_ABI optional<__empty>
__replace_if(_ExecutionPolicy&& __policy,
             _ForwardIterator&& __first,
             _ForwardIterator&& __last,
             _Pred&& __pred,
             const _Tp& __new_value) noexcept {
  return std::__pstl_frontend_dispatch(
      _LIBCPP_PSTL_CUSTOMIZATION_POINT(__pstl_replace_if, _RawPolicy),
      [&__policy](
          _ForwardIterator&& __g_first, _ForwardIterator&& __g_last, _Pred&& __g_pred, const _Tp& __g_new_value) {
        std::for_each(__policy, __g_first, __g_last, [&](__iter_reference<_ForwardIterator> __element) {
          if (__g_pred(__element))
            __element = __g_new_value;
        });
        return optional<__empty>{__empty{}};
      },
      std::move(__first),
      std::move(__last),
      std::move(__pred),
      __new_value);
}

template <class _ExecutionPolicy,
          class _ForwardIterator,
          class _Pred,
          class _Tp,
          class _RawPolicy                                    = __remove_cvref_t<_ExecutionPolicy>,
          enable_if_t<is_execution_policy_v<_RawPolicy>, int> = 0>
_LIBCPP_HIDE_FROM_ABI void
replace_if(_ExecutionPolicy&& __policy,
           _ForwardIterator __first,
           _ForwardIterator __last,
           _Pred __pred,
           const _Tp& __new_value) {
  _LIBCPP_REQUIRE_CPP17_FORWARD_ITERATOR(_ForwardIterator, "replace_if requires ForwardIterators");
  auto __res = std::__replace_if(__policy, std::move(__first), std::move(__last), std::move(__pred), __new_value);
  if (!__res)
    std::__throw_bad_alloc();
}

template <class>
void __pstl_replace();

template <class _ExecutionPolicy,
          class _ForwardIterator,
          class _Tp,
          class _RawPolicy                                    = __remove_cvref_t<_ExecutionPolicy>,
          enable_if_t<is_execution_policy_v<_RawPolicy>, int> = 0>
[[nodiscard]] _LIBCPP_HIDE_FROM_ABI optional<__empty>
__replace(_ExecutionPolicy&& __policy,
          _ForwardIterator&& __first,
          _ForwardIterator&& __last,
          const _Tp& __old_value,
          const _Tp& __new_value) noexcept {
  return std::__pstl_frontend_dispatch(
      _LIBCPP_PSTL_CUSTOMIZATION_POINT(__pstl_replace, _RawPolicy),
      [&__policy](
          _ForwardIterator __g_first, _ForwardIterator __g_last, const _Tp& __g_old_value, const _Tp& __g_new_value) {
        return std::__replace_if(
            __policy,
            std::move(__g_first),
            std::move(__g_last),
            [&](__iter_reference<_ForwardIterator> __element) { return __element == __g_old_value; },
            __g_new_value);
      },
      std::forward<_ForwardIterator>(__first),
      std::forward<_ForwardIterator>(__last),
      __old_value,
      __new_value);
}

template <class _ExecutionPolicy,
          class _ForwardIterator,
          class _Tp,
          class _RawPolicy                                    = __remove_cvref_t<_ExecutionPolicy>,
          enable_if_t<is_execution_policy_v<_RawPolicy>, int> = 0>
_LIBCPP_HIDE_FROM_ABI void
replace(_ExecutionPolicy&& __policy,
        _ForwardIterator __first,
        _ForwardIterator __last,
        const _Tp& __old_value,
        const _Tp& __new_value) {
  _LIBCPP_REQUIRE_CPP17_FORWARD_ITERATOR(_ForwardIterator, "replace requires ForwardIterators");
  if (!std::__replace(__policy, std::move(__first), std::move(__last), __old_value, __new_value))
    std::__throw_bad_alloc();
}

template <class>
void __pstl_replace_copy_if();

template <class _ExecutionPolicy,
          class _ForwardIterator,
          class _ForwardOutIterator,
          class _Pred,
          class _Tp,
          class _RawPolicy                                    = __remove_cvref_t<_ExecutionPolicy>,
          enable_if_t<is_execution_policy_v<_RawPolicy>, int> = 0>
[[nodiscard]] _LIBCPP_HIDE_FROM_ABI optional<__empty> __replace_copy_if(
    _ExecutionPolicy&& __policy,
    _ForwardIterator&& __first,
    _ForwardIterator&& __last,
    _ForwardOutIterator&& __result,
    _Pred&& __pred,
    const _Tp& __new_value) noexcept {
  return std::__pstl_frontend_dispatch(
      _LIBCPP_PSTL_CUSTOMIZATION_POINT(__pstl_replace_copy_if, _RawPolicy),
      [&__policy](_ForwardIterator __g_first,
                  _ForwardIterator __g_last,
                  _ForwardOutIterator __g_result,
                  _Pred __g_pred,
                  const _Tp& __g_new_value) -> optional<__empty> {
        if (!std::__transform(
                __policy, __g_first, __g_last, __g_result, [&](__iter_reference<_ForwardIterator> __element) {
                  return __g_pred(__element) ? __g_new_value : __element;
                }))
          return nullopt;
        return __empty{};
      },
      std::move(__first),
      std::move(__last),
      std::move(__result),
      std::move(__pred),
      __new_value);
}

template <class _ExecutionPolicy,
          class _ForwardIterator,
          class _ForwardOutIterator,
          class _Pred,
          class _Tp,
          class _RawPolicy                                    = __remove_cvref_t<_ExecutionPolicy>,
          enable_if_t<is_execution_policy_v<_RawPolicy>, int> = 0>
_LIBCPP_HIDE_FROM_ABI void replace_copy_if(
    _ExecutionPolicy&& __policy,
    _ForwardIterator __first,
    _ForwardIterator __last,
    _ForwardOutIterator __result,
    _Pred __pred,
    const _Tp& __new_value) {
  _LIBCPP_REQUIRE_CPP17_FORWARD_ITERATOR(_ForwardIterator, "replace_copy_if requires ForwardIterators");
  _LIBCPP_REQUIRE_CPP17_FORWARD_ITERATOR(_ForwardOutIterator, "replace_copy_if requires ForwardIterators");
  _LIBCPP_REQUIRE_CPP17_OUTPUT_ITERATOR(
      _ForwardOutIterator, decltype(*__first), "replace_copy_if requires an OutputIterator");
  _LIBCPP_REQUIRE_CPP17_OUTPUT_ITERATOR(_ForwardOutIterator, const _Tp&, "replace_copy requires an OutputIterator");
  if (!std::__replace_copy_if(
          __policy, std::move(__first), std::move(__last), std::move(__result), std::move(__pred), __new_value))
    std::__throw_bad_alloc();
}

template <class>
void __pstl_replace_copy();

template <class _ExecutionPolicy,
          class _ForwardIterator,
          class _ForwardOutIterator,
          class _Tp,
          class _RawPolicy                                    = __remove_cvref_t<_ExecutionPolicy>,
          enable_if_t<is_execution_policy_v<_RawPolicy>, int> = 0>
[[nodiscard]] _LIBCPP_HIDE_FROM_ABI optional<__empty> __replace_copy(
    _ExecutionPolicy&& __policy,
    _ForwardIterator&& __first,
    _ForwardIterator&& __last,
    _ForwardOutIterator&& __result,
    const _Tp& __old_value,
    const _Tp& __new_value) noexcept {
  return std::__pstl_frontend_dispatch(
      _LIBCPP_PSTL_CUSTOMIZATION_POINT(__pstl_replace_copy, _RawPolicy),
      [&__policy](_ForwardIterator __g_first,
                  _ForwardIterator __g_last,
                  _ForwardOutIterator __g_result,
                  const _Tp& __g_old_value,
                  const _Tp& __g_new_value) {
        return std::__replace_copy_if(
            __policy,
            std::move(__g_first),
            std::move(__g_last),
            std::move(__g_result),
            [&](__iter_reference<_ForwardIterator> __element) { return __element == __g_old_value; },
            __g_new_value);
      },
      std::move(__first),
      std::move(__last),
      std::move(__result),
      __old_value,
      __new_value);
}

template <class _ExecutionPolicy,
          class _ForwardIterator,
          class _ForwardOutIterator,
          class _Tp,
          class _RawPolicy                                    = __remove_cvref_t<_ExecutionPolicy>,
          enable_if_t<is_execution_policy_v<_RawPolicy>, int> = 0>
_LIBCPP_HIDE_FROM_ABI void replace_copy(
    _ExecutionPolicy&& __policy,
    _ForwardIterator __first,
    _ForwardIterator __last,
    _ForwardOutIterator __result,
    const _Tp& __old_value,
    const _Tp& __new_value) {
  _LIBCPP_REQUIRE_CPP17_FORWARD_ITERATOR(_ForwardIterator, "replace_copy requires ForwardIterators");
  _LIBCPP_REQUIRE_CPP17_FORWARD_ITERATOR(_ForwardOutIterator, "replace_copy requires ForwardIterators");
  _LIBCPP_REQUIRE_CPP17_OUTPUT_ITERATOR(
      _ForwardOutIterator, decltype(*__first), "replace_copy requires an OutputIterator");
  _LIBCPP_REQUIRE_CPP17_OUTPUT_ITERATOR(_ForwardOutIterator, const _Tp&, "replace_copy requires an OutputIterator");
  if (!std::__replace_copy(
          __policy, std::move(__first), std::move(__last), std::move(__result), __old_value, __new_value))
    std::__throw_bad_alloc();
}

_LIBCPP_END_NAMESPACE_STD

#endif // !defined(_LIBCPP_HAS_NO_INCOMPLETE_PSTL) && _LIBCPP_STD_VER >= 17

_LIBCPP_POP_MACROS

#endif // _LIBCPP___ALGORITHM_PSTL_REPLACE_H
