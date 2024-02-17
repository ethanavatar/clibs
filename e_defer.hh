#ifndef E_DEFER_MACRO_HH
#define E_DEFER_MACRO_HH

#ifdef __cplusplus

#define P_CONCATENATE_IMPL(x, y) x##y
#define P_CONCATENATE(x, y) P_CONCATENATE_IMPL(x, y)

template <typename F>
struct PDefer {
    F f;
    PDefer(F f) : f(f) {}
    ~PDefer() { f(); }
};

template <typename F>
PDefer<F> p_defer_func(F f) {
    return PDefer<F>(f);
}

#define P_DEFER_VAR(name) P_CONCATENATE(name, __COUNTER__)
#define DEFER(code) auto P_DEFER_VAR(_deferred_var) = p_defer_func([&](){code;})

#endif // !__cplusplus

#endif // !E_DEFER_MACRO_HH
