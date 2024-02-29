/*
https://atcoder.jp/contests/abc241/submissions/50713437
*/

template <std::integral T, class F, class U = std::make_unsigned_t<T>>
    requires std::is_invocable_r_v<T, F, T>
std::tuple<U, U, T> Floyds_cycle_finding(const T init, const F f)
    noexcept(std::is_nothrow_invocable_v<T, F, T>)
{
    T tortoise = init, hare = init;
    for (
        tortoise = f(tortoise), hare = f(f(hare));
        tortoise != hare;
        tortoise = f(tortoise), hare = f(f(hare))
    ) {
    }
    U lambda = 0;
    for (
        hare = init;
        tortoise != hare;
        tortoise = f(tortoise), hare = f(hare)
    ) {
        ++lambda;
    }
    const T loop_start = hare;
    U mu = 1;
    for (
        tortoise = f(tortoise), hare = f(f(hare));
        tortoise != hare;
        tortoise = f(tortoise), hare = f(f(hare))
    ) {
        ++mu;
    }
    return {lambda, mu, loop_start};
}
