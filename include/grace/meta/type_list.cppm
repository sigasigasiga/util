export module grace.meta.type_list;

export namespace grace::meta {

template<typename... Ts>
class [[nodiscard]] type_list
{
public:
    template<typename T>
    using push_back = type_list<Ts..., T>;
};

} // namespace grace::meta
