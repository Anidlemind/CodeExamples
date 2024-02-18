#pragma once
#include <iostream>
#include <memory>

template <size_t N>
class StackStorage {
 public:
  char data[N];
  size_t end = 0;

  StackStorage() = default;
  StackStorage(StackStorage& obj) = delete;
  ~StackStorage() = default;
  void operator=(const StackStorage& obj) = delete;
};

template <typename T, size_t N>
class StackAllocator {
 public:
  StackStorage<N>* storage_ = nullptr;

  using value_type = T;
  using pointer = T*;

  StackAllocator() = default;
  StackAllocator(StackStorage<N>& obj)
      : storage_(&(obj)) {
  }
  StackAllocator(const StackAllocator<T, N>& obj)
      : storage_(obj.storage_) {
  }
  template <typename TN>
  StackAllocator(const StackAllocator<TN, N>& obj)
      : storage_(obj.storage_){};
  StackAllocator(StackAllocator<T, N>&& obj)
      : storage_(std::move(obj.storage_)){};

  StackAllocator& operator=(const StackAllocator<T, N>& obj) {
    storage_ = obj.storage_;
    return *this;
  }

  ~StackAllocator() = default;

  T* allocate(size_t n) {
    void* ptr = reinterpret_cast<void*>(storage_->data + storage_->end);
    size_t space = N - storage_->end;
    std::align(alignof(T), n * sizeof(T), ptr, space);
    storage_->end = reinterpret_cast<char*>(ptr) - storage_->data;
    auto old_shift = storage_->end;
    storage_->end += sizeof(T) * n;
    return reinterpret_cast<T*>(storage_->data + old_shift);
  }

  void deallocate(T*, size_t) {
  }

  template <typename... args>
  void construct(T* ptr, const args&... arguments) {
    new (ptr) T(arguments...);
  }

  void destroy(T* ptr) {
    ptr->~T();
  }

  template <class TN>
  struct rebind {
    using other = StackAllocator<TN, N>;
  };
};

template <typename T, typename Alloc = std::allocator<T>>
class List {
 private:
  struct BNode {
    BNode* next_;
    BNode* prev_;

    BNode() = default;
    BNode(BNode* next, BNode* previous)
        : next_(next),
          prev_(previous) {
    }

    ~BNode() = default;
  };
  struct Node : BNode {
   public:
    T val_;

    Node() = default;
    Node(const T& value)
        : val_(value) {
    }
    Node(T&& value)
        : val_(std::move(value)) {
    }
  };
  mutable BNode* fake_;

  size_t size_;

  using AllocTraits = std::allocator_traits<Alloc>;
  using bnode_alloc_type = typename AllocTraits::template rebind_alloc<BNode>;
  typename AllocTraits::template rebind_alloc<BNode> _bnode_alloc = Alloc();
  using node_alloc_type = typename AllocTraits::template rebind_alloc<Node>;
  typename AllocTraits::template rebind_alloc<Node> _node_alloc = Alloc();

  void clear() {
    size_t save_sz = size_;
    for (size_t i = 0; i < save_sz; ++i) {
      pop_back();
    }
  }

  void exchange(Node* new_node) {
    BNode* last_node = fake_->prev_;
    new_node->next_ = fake_;
    new_node->prev_ = last_node;
    fake_->prev_ = new_node;
    last_node->next_ = new_node;
    ++size_;
  };

 public:
  List();
  List(size_t size);
  List(size_t size, const T& obj);
  explicit List(const Alloc& alloc);
  List(size_t size, const Alloc& alloc);
  List(size_t size, const T& obj, const Alloc& alloc);
  List(const List& other);

  template <bool Const>
  class CommonIterator {
   public:
    BNode* pos_ = nullptr;

    using iterator_category = std::bidirectional_iterator_tag;
    using pointer = typename std::conditional<Const, const T*, T*>::type;
    using value_type = typename std::conditional<Const, const T, T>::type;
    using difference_type = std::ptrdiff_t;
    using reference = typename std::conditional<Const, const T&, T&>::type;

    CommonIterator() = default;

    CommonIterator(BNode* pos)
        : pos_(pos) {
    }
    ~CommonIterator() = default;

    CommonIterator& operator=(const CommonIterator& obj) {
      pos_ = obj.pos_;
      return *this;
    }

    CommonIterator& operator++() {
      pos_ = pos_->next_;
      return *this;
    }
    CommonIterator operator++(int) {
      CommonIterator copy(*this);
      pos_ = pos_->next_;
      return copy;
    }
    CommonIterator& operator--() {
      pos_ = pos_->prev_;
      return *this;
    }
    CommonIterator& operator--(int) {
      CommonIterator copy(*this);
      pos_ = pos_->prev_;
      return copy;
    }
    CommonIterator& operator+=(size_t x) {
      for (size_t i = 0; i < x; ++i) {
        pos_ = pos_->next_;
      }
      return *this;
    }
    CommonIterator& operator-=(size_t x) {
      for (size_t i = 0; i < x; ++i) {
        pos_ = pos_->prev_;
      }
      return *this;
    }
    CommonIterator operator+(size_t x) {
      CommonIterator copy = *this;
      copy += x;
      return copy;
    }
    CommonIterator operator-(size_t x) {
      CommonIterator copy = *this;
      copy -= x;
      return copy;
    }
    bool operator==(const CommonIterator& obj) const {
      return pos_ == obj.pos_;
    }
    bool operator!=(const CommonIterator& obj) const {
      return !(*this == obj);
    }
    reference operator*() {
      return static_cast<Node*>(pos_)->val_;
    }
    pointer operator->() {
      return &(static_cast<Node*>(pos_)->val_);
    }
    operator CommonIterator<true>() const {
      return CommonIterator<true>(pos_);
    }
  };
  using iterator = CommonIterator<false>;

  using reverse_iterator = std::reverse_iterator<iterator>;

  using const_iterator = CommonIterator<true>;

  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

 private:
  const_iterator insert_node(const_iterator it, Node* new_node) {
    BNode* before_node = it.pos_->prev_;
    new_node->next_ = it.pos_;
    new_node->prev_ = before_node;
    before_node->next_ = new_node;
    it.pos_->prev_ = new_node;
    ++size_;
    return (it - 1);
  }

 public:
  ~List() noexcept;

  List& operator=(const List& obj);

  size_t size() const {
    return size_;
  };
  void push_back(const T& value);
  void push_front(const T& value);
  void pop_back() noexcept;
  void pop_front() noexcept;
  node_alloc_type get_allocator() const {
    return _node_alloc;
  }
  void swap(List& other) {
    std::swap(_node_alloc, other._node_alloc);
    std::swap(size_, other.size_);
    std::swap(fake_, other.fake_);
    std::swap(_bnode_alloc, other._bnode_alloc);
  }

  iterator begin() {
    return iterator(fake_->next_);
  }
  const_iterator begin() const {
    return const_iterator(fake_->next_);
  }
  iterator end() {
    return iterator(fake_);
  }
  const_iterator end() const {
    return const_iterator(fake_);
  }
  const_iterator cbegin() const {
    return const_iterator(fake_->next_);
  }
  const_iterator cend() const {
    return const_iterator(fake_);
  }
  reverse_iterator rbegin() {
    return std::make_reverse_iterator(end());
  }
  const_reverse_iterator rbegin() const {
    return crbegin();
  }
  reverse_iterator rend() {
    return std::make_reverse_iterator(begin());
  }
  const_reverse_iterator rend() const {
    return crend();
  }
  const_reverse_iterator crbegin() const {
    return std::make_reverse_iterator(cend());
  }
  const_reverse_iterator crend() const {
    return std::make_reverse_iterator(cbegin());
  }

  template <bool Const>
  void insert(CommonIterator<Const> it, const T& value) {
    Node* add = _node_alloc.allocate(1);
    try {
      std::allocator_traits<node_alloc_type>::construct(_node_alloc, add,
                                                        value);
    } catch (...) {
      _node_alloc.deallocate(add, 1);
      throw;
    }
    insert_node(it, add);
  }

  template <bool Const>
  void erase(CommonIterator<Const> pos) noexcept {
    pos.pos_->prev_->next_ = pos.pos_->next_;
    pos.pos_->next_->prev_ = pos.pos_->prev_;
    std::allocator_traits<node_alloc_type>::destroy(
        _node_alloc, static_cast<Node*>(pos.pos_));
    std::allocator_traits<node_alloc_type>::deallocate(
        _node_alloc, static_cast<Node*>(pos.pos_), 1);
    --size_;
  }

  void push_back();
};
template <typename T, typename Alloc>
List<T, Alloc>::List()
    : size_(0) {
  fake_ = _bnode_alloc.allocate(1);
  std::allocator_traits<bnode_alloc_type>::construct(_bnode_alloc, fake_, fake_,
                                                     fake_);
}
template <typename T, typename Alloc>
List<T, Alloc>::List(size_t size)
    : size_(0) {
  fake_ = _bnode_alloc.allocate(1);
  std::allocator_traits<bnode_alloc_type>::construct(_bnode_alloc, fake_, fake_,
                                                     fake_);
  try {
    for (size_t i = 0; i < size; ++i) {
      push_back();
    }
  } catch (...) {
    clear();
    throw;
  }
}
template <typename T, typename Alloc>
List<T, Alloc>::List(const Alloc& alloc)
    : size_(0),
      _bnode_alloc(alloc),
      _node_alloc(alloc) {
  fake_ = _bnode_alloc.allocate(1);
  std::allocator_traits<bnode_alloc_type>::construct(_bnode_alloc, fake_, fake_,
                                                     fake_);
}
template <typename T, typename Alloc>
List<T, Alloc>::List(size_t size, const Alloc& alloc)
    : size_(0),
      _bnode_alloc(alloc),
      _node_alloc(alloc) {
  fake_ = _bnode_alloc.allocate(1);
  std::allocator_traits<bnode_alloc_type>::construct(_bnode_alloc, fake_, fake_,
                                                     fake_);
  try {
    for (size_t i = 0; i < size; ++i) {
      push_back();
    }
  } catch (...) {
    clear();
    throw;
  }
}
template <typename T, typename Alloc>
List<T, Alloc>::List(size_t size, const T& obj, const Alloc& alloc)
    : size_(0),
      _bnode_alloc(alloc),
      _node_alloc(alloc) {
  fake_ = _bnode_alloc.allocate(1);
  std::allocator_traits<bnode_alloc_type>::construct(_bnode_alloc, fake_, fake_,
                                                     fake_);
  try {
    for (size_t i = 0; i < size; ++i) {
      push_back(obj);
    }
  } catch (...) {
    clear();
    throw;
  }
}
template <typename T, typename Alloc>
List<T, Alloc>::List(const List& other)
    : size_(0),
      _bnode_alloc(
          std::allocator_traits<Alloc>::select_on_container_copy_construction(
              other._bnode_alloc)),
      _node_alloc(
          std::allocator_traits<Alloc>::select_on_container_copy_construction(
              other._node_alloc)) {
  fake_ = _bnode_alloc.allocate(1);
  std::allocator_traits<bnode_alloc_type>::construct(_bnode_alloc, fake_, fake_,
                                                     fake_);
  try {
    auto to_copy = other.begin();
    for (size_t i = 0; i < other.size_; ++i) {
      push_back(*to_copy);
      to_copy++;
    }
  } catch (...) {
    clear();
    throw;
  }
}
template <typename T, typename Alloc>
List<T, Alloc>::~List() noexcept {
  clear();
  std::allocator_traits<bnode_alloc_type>::destroy(_bnode_alloc, fake_);
}
template <typename T, typename Alloc>
List<T, Alloc>& List<T, Alloc>::operator=(const List& other) {
  List copy(std::allocator_traits<
                Alloc>::propagate_on_container_copy_assignment::value
                ? other._node_alloc
                : _node_alloc);
  try {
    auto it = other.cbegin();
    while (it != other.cend()) {
      copy.push_back(*it);
      ++it;
    }
  } catch (...) {
    size_t save = copy.size_;
    for (size_t cnt = 0; cnt < save; ++cnt) {
      copy.pop_back();
    }
    throw;
  }
  swap(copy);
  return *this;
}
template <typename T, typename Alloc>
void List<T, Alloc>::push_back(const T& value) {
  insert(end(), value);
}

// this is not insert-acceptable, just creating memory
template <typename T, typename Alloc>
void List<T, Alloc>::push_back() {
  Node* new_node = _node_alloc.allocate(1);
  try {
    std::allocator_traits<node_alloc_type>::construct(_node_alloc, new_node);
  } catch (...) {
    _node_alloc.deallocate(new_node, 1);
    throw;
  }
  exchange(new_node);
}

template <typename T, typename Alloc>
void List<T, Alloc>::push_front(const T& value) {
  insert(begin(), value);
}
template <typename T, typename Alloc>
void List<T, Alloc>::pop_back() noexcept {
  erase(--(this->end()));
}
template <typename T, typename Alloc>
void List<T, Alloc>::pop_front() noexcept {
  erase(this->begin());
}
