#pragma once
#include <vector>
#include <memory>

// クラス
template <typename T>
class ObjectPool
{
public:
	ObjectPool(size_t initialSize_ = 10);

	std::unique_ptr<T> Get();
	void Return(std::unique_ptr<T> object);
	std::vector<std::unique_ptr<T>>& GetPool(); 

private:
	std::vector<std::unique_ptr<T>> pool;
	size_t size;
};

// 数と生成
template <typename T>
ObjectPool<T>::ObjectPool(size_t initialSize_) {
	// 要素のクリアとサイズの変更
	size=initialSize_;
	pool.clear();
	pool.reserve(initialSize_); 

	// 生成
	for ( size_t i = 0; i < initialSize_; ++i )
	{
		pool.push_back(std::make_unique<T>());
	}
}

template <typename T>
void ObjectPool<T>::Return(std::unique_ptr<T> object) {
	pool.push_back(std::move(object));
}

template <typename T>
std::vector<std::unique_ptr<T>>& ObjectPool<T>::GetPool() {
	return pool;
}
