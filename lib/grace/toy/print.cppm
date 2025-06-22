export module grace.toy:print;

export namespace grace::toy {

template<typename...>
struct type_print;

template<auto...>
struct value_print;

template<typename... Ts>
type_print<Ts...> print();

template<typename... Ts>
type_print<Ts &&...> print(Ts &&...);

template<auto... Vs>
value_print<Vs...> print();

} // namespace grace::toy
