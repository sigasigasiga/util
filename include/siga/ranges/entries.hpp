#pragma once

#include <algorithm>
#include <functional>
#include <ranges>
#include <utility>

#include <siga/ranges/subrange.hpp>

namespace siga::ranges {

// https://doc.rust-lang.org/std/collections/hash_map/enum.Entry.html
// TODO: is `siga::ranges` good place for this?
template<typename Map>
requires requires(Map m, Map::iterator i, Map::key_type k, Map::mapped_type v) {
    m.equal_range(k);
    m.emplace_hint(i, k, v);
}
class entries
{
public:
    using key_type = Map::key_type;
    using mapped_type = Map::mapped_type;
    using iterator = Map::iterator;

public:
    template<typename Key>
    entries(Map &map, Key &&key)
        : map_{&map}
        , key_{std::forward<Key>(key)}
        , equal_range_{make_subrange(map_->equal_range(key_))}
    {
    }

public:
    bool vacant() const { return equal_range_.empty(); }

    template<typename Self>
    auto &&key(this Self &&self)
    {
        return std::forward<Self>(self).key_;
    }

    template<typename Self, typename F>
    mapped_type &or_insert_with(this Self &&self, F &&fn)
    {
        auto it = self.equal_range_.begin();

        if(self.vacant()) {
            it = self.map_.emplace_hint(
                it,
                std::forward<Self>(self).key(),
                std::invoke(std::forward<F>(fn))
            );
        }

        return it->second;
    }

    template<typename Self, std::convertible_to<mapped_type> Value>
    mapped_type &or_insert(this Self &&self, Value &&value)
    {
        auto it = self.equal_range_.begin();

        if(self.vacant()) {
            it = self.map_->emplace_hint( //
                it,
                std::forward<Self>(self).key(),
                std::forward<Value>(value)
            );
        }

        return it->second;
    }

    template<typename F>
    entries &and_modify(F fn)
    {
        auto vals = equal_range_ | std::views::values;
        std::ranges::transform(vals, vals.begin(), std::move(fn));
        return *this;
    }

private:
    Map *map_;
    key_type key_;
    std::ranges::subrange<iterator, iterator> equal_range_;
};

} // namespace siga::ranges
