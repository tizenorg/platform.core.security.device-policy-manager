/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#ifndef __CALLBACK_HOLDER__
#define __CALLBACK_HOLDER__

#include <utility>
#include <functional>

#include "message.hxx"

namespace Ipc {

template<typename Type, typename... Args>
struct MethodHandler {
    typedef std::function<Type(Args&...)> type;
};
// using MethodHandler = std::function<Type(Args&...)>;

template<typename Type, typename... Args>
struct CallbackHolder {
    template<int...>
    struct Sequence {};

    template<int N, int... S>
    struct SequenceExpansion : SequenceExpansion<N-1, N-1, S...> {};

    template<int... S>
    struct SequenceExpansion<0, S...> {
        typedef Sequence<S...> type;
    };

    std::tuple<Args...> parameters;
    const typename MethodHandler<Type, Args...>::type& callback;

    CallbackHolder(const typename MethodHandler<Type, Args...>::type& m)
        : callback(m)
    {
    }

    Type dispatch(const Message& message)
    {
        return callCallback(message, typename SequenceExpansion<sizeof...(Args)>::type());
    }

    void buildParameters(const Message& message)
    {
    }

    template<typename F>
    void buildParameters(const Message& message, F& arg)
    {
        message.disclose<F>(arg);
    }

    template<typename F, typename... R>
    void buildParameters(const Message& message, F& first, R&... rest)
    {
        buildParameters(message, first);
        buildParameters(message, rest...);
    }

    template<int... S>
    Type callCallback(const Message& message, Sequence<S...>)
    {
        buildParameters(message, std::get<S>(parameters)...);
        return callback(std::get<S>(parameters)...);
    }
};

} // namespace Ipc
#endif //!__CALLBACK_HOLDER__
