#ifndef E_WITH_MACRO_H
#define E_WITH_MACRO_H

#define P_CONCATENATE_IMPL(x, y) x##y
#define P_CONCATENATE(x, y) P_CONCATENATE_IMPL(x, y)

#define WITH_VAR(name) P_CONCATENATE(name, __COUNTER__)
#define WITH(setup, teardown) for ( \
    int WITH_VAR(_with_var) = (setup, 0); \
    !WITH_VAR(_with_var); \
    (WITH_VAR(_with_var) += 1, teardown))

#endif // !E_WITH_MACRO_H
