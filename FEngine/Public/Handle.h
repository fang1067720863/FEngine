#pragma once

#include<string>

struct DefaultNamespaceType {

};

template <typename HandleType, typename NamespaceType>
const Handle<HandleType, NamespaceType> Handle<HandleType, NamespaceType>::Null(
    Handle<HandleType, NamespaceType>::NullIndex);

// 使用模板 一个装饰器让很对类的对象具有了索引
template <typename T = uint32_t, typename NamespaceType = DefaultNamespaceType >
struct Handle
{
    using IndexType = T;

    static const T NullIndex = T(-1);
    static const Handle Null;

    Handle() = default;
    explicit Handle(T index) : m_index{ index } {}

    template <typename U>
    explicit Handle(U index) : m_index{ aznumeric_caster(index) } {}

    //bool operator == (const Handle& rhs) const;
    //bool operator != (const Handle& rhs) const;
    //bool operator < (const Handle& rhs) const;
    //bool operator > (const Handle& rhs) const;
    //bool operator <= (const Handle& rhs) const;

    /// Resets the handle to NullIndex.
    void Reset() { m_index = NullIndex; }

    /// Returns the index currently stored in the handle.
    T GetIndex() const { return m_index; }

    /// Returns whether the handle is equal to NullIndex.
    bool IsNull() const { return m_index == Null.m_index; }

    /// Returns whether the handle is NOT equal to NullIndex.
    //bool IsValid() const { return m_index == Null.m_index; }

    T m_index = NullIndex;
};