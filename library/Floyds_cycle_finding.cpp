/*
https://atcoder.jp/contests/abc241/submissions/50954650
https://atcoder.jp/contests/abc357/submissions/54376567
https://atcoder.jp/contests/abc377/submissions/59203292
*/

template <class T, class F>
    requires std::is_invocable_r_v<T, F, T>
std::tuple<std::size_t, std::size_t, T> Floyds_cycle_finding(const T init, const F f)
    noexcept(std::is_nothrow_invocable_v<T, F, T>)
{
    auto tortoise = init, hare = init;
    for (
        tortoise = f(tortoise), hare = f(f(hare));
        tortoise != hare;
        tortoise = f(tortoise), hare = f(f(hare))
    ) {
    }
    auto lambda = 0uz;
    for (
        hare = init;
        tortoise != hare;
        tortoise = f(tortoise), hare = f(hare)
    ) {
        ++lambda;
    }
    const auto loop_start = hare;
    auto mu = 1uz;
    for (
        tortoise = f(tortoise), hare = f(f(hare));
        tortoise != hare;
        tortoise = f(tortoise), hare = f(f(hare))
    ) {
        ++mu;
    }
    return {lambda, mu, loop_start};
}
