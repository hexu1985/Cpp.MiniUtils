/**
 * @file RCIPtr.hpp
 * @brief 引用计数指针, 类似于std::shared_ptr
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-10-10
 *
 * @see Efficient C++: Performance Programming Techniques, chapter 12
 */
#ifndef MINI_UTILS_RCI_PTR_INC
#define MINI_UTILS_RCI_PTR_INC

#include "RCObject.hpp"

namespace mini_utils {

/**
 * @brief 引用计数指针
 *
 * @tparam T 被引用计数对象的类型
 */
template <class T>
class RCIPtr {
public:
    /**
     * @brief 构造函数
     *
     * @param realPtr 被引用计数对象的指针
     */
    RCIPtr(T *realPtr = 0);

    /**
     * @brief 复制构造
     *
     * @param rhs 增加rhs的引用计数(如果rhs.counter_不为空)
     */
    RCIPtr(const RCIPtr &rhs);

    /**
     * @brief 复制赋值
     *
     * @param rhs 首先减少*this的引用计数(如果counter_不为空), 
     *            然后this->counter_ = rhs.counter_,
     *            然后增加rhs的引用计数(如果rhs不为空)
     *
     * @return *this
     */
    RCIPtr& operator=(const RCIPtr &rhs);

    /**
     * @brief 析构: 减少*this的引用计数
     */
    ~RCIPtr();

    /**
     * @brief 重载->运算符, 获取引用计数对象的地址
     *
     * @return 引用计数对象的地址
     */
    T *operator ->() const;

    /**
     * @brief 重载*运算符, 获取引用计数对象的引用
     *
     * @return 引用计数对象的引用
     */
    T &operator *() const;

private:
    struct CountHolder: public RCObject {
        ~CountHolder() { delete pointee; }
        T *pointee;
    };

    CountHolder *counter_;
    void init();
};

template <class T>
void RCIPtr<T>::init()
{
    if (counter_ == 0) { 
        return;
    }
    counter_->addReference();
}

template <class T>
RCIPtr<T>::RCIPtr(T *realPtr): counter_(new CountHolder)
{
    counter_->pointee = realPtr;
    init();
}

template <class T>
RCIPtr<T>::RCIPtr(const RCIPtr &rhs): counter_(rhs.counter_)
{
    init();
}

template<class T>
RCIPtr<T> &RCIPtr<T>::operator =(const RCIPtr &rhs)
{
    if (counter_ == rhs.counter_) {
        return *this;
    }

    if (counter_) {
        counter_->removeReference();
    }

    counter_ = rhs.counter_;
    init();

    return *this;
}

template<class T>
RCIPtr<T>::~RCIPtr()
{
    if (counter_) counter_->removeReference();
}

template<class T>
T *RCIPtr<T>::operator ->() const { return counter_->pointee; }

template<class T>
T &RCIPtr<T>::operator *() const { return *(counter_->pointee); }

}   // namespace mini_utils

#endif
