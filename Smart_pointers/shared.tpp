#ifndef SHARED_PTR_TPP
#define SHARED_PTR_TPP

#include "shared.hpp"

template<typename T>
shared_ptr<T>::shared_ptr(T value)
    : _controlBlock{new ControlBlock<T>{new T{value}}}
{}
template<typename T>
shared_ptr<T>::shared_ptr(T* ptr)
    : _controlBlock{new ControlBlock<T>{ptr}}
{}
template<typename T>
shared_ptr<T>::shared_ptr(ControlBlock<T>* controlBlock)
    : _controlBlock{controlBlock} {
    if (_controlBlock) {
        _controlBlock->incrementSharedRefCount();
    }
}
template<typename T>
shared_ptr<T>::shared_ptr(const shared_ptr& src)
    : _controlBlock{src._controlBlock}
{
    _controlBlock->incrementSharedRefCount();
}
template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& rhs)
{
    if (this != &rhs) {
        if (_controlBlock) {
            _controlBlock->decrementSharedRefCount();
            if (_controlBlock->getSharedCount() == 0) {
                _controlBlock->free();
                delete _controlBlock;
            }
        }
        _controlBlock = rhs._controlBlock;
        if (_controlBlock) {
            _controlBlock->incrementSharedRefCount();
        }
    }
    return *this;
}
template<typename T>
shared_ptr<T>::shared_ptr(shared_ptr&& src) noexcept
    : _controlBlock{src._controlBlock}
{
       
}
template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr&& rhs) noexcept
{
    if (this != &rhs) {
        _controlBlock->free();
        _controlBlock = rhs._controlBlock;
        rhs._controlBlock = nullptr;
    }
    return *this;
}
template<typename T>
shared_ptr<T>::~shared_ptr()
{
    if (_controlBlock) {
        _controlBlock->free();
    }
}
template<typename T>
void shared_ptr<T>::reset(T* ptr)
{
    if (_controlBlock) {
        _controlBlock->free();
        _controlBlock = new ControlBlock{ptr};
    }
}
template<typename T>
const T* shared_ptr<T>::get() const
{
    return _controlBlock->get();
}
template<typename T>
T* shared_ptr<T>::get() 
{
    return const_cast<T*>(std::as_const(*this).get());
}
template<typename T>
int shared_ptr<T>::use_count() const
{
    return _controlBlock->getSharedCount();
}
template<typename T>
T& shared_ptr<T>::operator*()
{
    return const_cast<T&>(*std::as_const(*this));
}
template<typename T>
const T& shared_ptr<T>::operator*() const
{
    return *(_controlBlock->get());
}
template<typename T>
template<typename U, typename>
const T* shared_ptr<T>::operator->() const
{
    return _controlBlock->get();
}
template<typename T>
template<typename U, typename>
T* shared_ptr<T>::operator->()
{
    return _controlBlock->get();
}
template<typename T>
shared_ptr<T>::operator bool() const
{
    return _controlBlock && _controlBlock->getSharedCount() > 0 && _controlBlock->get() != nullptr;
}

#endif //SHARED_PTR_TPP
