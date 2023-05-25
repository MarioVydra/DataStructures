#pragma once

#include <libds/mm/memory_manager.h>
#include <libds/mm/memory_omanip.h>
#include <libds/constants.h>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <algorithm>
#include <iostream>

namespace ds::mm {

	template<typename BlockType>
	class CompactMemoryManager : public MemoryManager<BlockType> {
	public:
		CompactMemoryManager();
		CompactMemoryManager(size_t size);
		CompactMemoryManager(const CompactMemoryManager<BlockType>& other);
		~CompactMemoryManager() override;

		BlockType* allocateMemory() override;
		BlockType* allocateMemoryAt(size_t index);
		void releaseMemory(BlockType* pointer) override;
		void releaseMemoryAt(size_t index);
		void releaseMemory();

		size_t getCapacity() const;

		CompactMemoryManager<BlockType>& assign(const CompactMemoryManager<BlockType>& other);
		void changeCapacity(size_t newCapacity);
		void shrinkMemory();
		void clear();
		bool equals(const CompactMemoryManager<BlockType>& other) const;
		void* calculateAddress(const BlockType& data);
		size_t calculateIndex(const BlockType& data);
		BlockType& getBlockAt(size_t index);
		void swap(size_t index1, size_t index2);

		void print(std::ostream& os);

	private:
		size_t getAllocatedBlocksSize() const;
		size_t getAllocatedCapacitySize() const;

	private:
		BlockType* base_;
		BlockType* end_;
		BlockType* limit_;

		static const size_t INIT_SIZE = 4;
	};

	template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager() :
		CompactMemoryManager(INIT_SIZE)
	{
	}

	template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(size_t size) :
		base_(static_cast<BlockType*>(std::calloc(size, sizeof(BlockType)))),
		end_(base_),
		limit_(base_ + size)
	{
	}

	template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(const CompactMemoryManager<BlockType>& other):
		CompactMemoryManager(other.getAllocatedBlockCount())
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");

		this->assign(other);
	}

	template<typename BlockType>
    CompactMemoryManager<BlockType>::~CompactMemoryManager()
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");

		// Ensures that destrucotr for each object is called.
		CompactMemoryManager<BlockType>::releaseMemory(base_);
		
		std::free(base_);
		
		base_ = nullptr;
		end_ = nullptr;
		limit_ = nullptr;
	}

	template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemory()
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		return this->allocateMemoryAt(end_ - base_);

	}

	template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemoryAt(size_t index)
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		if (end_ == limit_)
		{
			this->changeCapacity(2 * this->getAllocatedBlockCount());
		}

		if (end_ - base_ > static_cast<std::ptrdiff_t>(index))
		{
			std::memmove(
				base_ + index + 1,
				base_ + index,
				(end_ - base_ - index) * sizeof(BlockType)
			);
		}
		++MemoryManager<BlockType>::allocatedBlockCount_;
		++end_;

		return placement_new(base_ + index);
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory(BlockType* pointer)
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		BlockType* p = pointer;
		while (p != end_)
		{
			p->~BlockType();
			++p;
		}
		end_ = pointer;
		this->allocatedBlockCount_ = end_ - base_;
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemoryAt(size_t index)
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		this->getBlockAt(index).~BlockType();
		std::memmove(
			base_ + index,
			base_ + index + 1,
			(end_ - base_ - index - 1) * sizeof(BlockType)
		);
		// memmove pouzijeme ak sa prekryvaju bloky, ktore presuvam
		// memcpy pouzijeme ak sa neprekryvaju bloky, 
		--end_;
		--MemoryManager<BlockType>::allocatedBlockCount_;
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory()
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");

		this->releaseMemory(end_ - 1);
	}

	template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getCapacity() const
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		return (limit_ - base_);

	}

	template<typename BlockType>
    CompactMemoryManager<BlockType>& CompactMemoryManager<BlockType>::assign(const CompactMemoryManager<BlockType>& other) {
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");

		if (this != &other)
		{
			this->releaseMemory(base_);
			MemoryManager<BlockType>::allocatedBlockCount_ = other.MemoryManager<BlockType>::allocatedBlockCount_;
			void* newBase = std::realloc(base_, other.getAllocatedCapacitySize());
		
			if (newBase == nullptr)
			{
				throw std::bad_alloc();
			}
			base_ = static_cast<BlockType*>(newBase);
			end_ = base_ + MemoryManager<BlockType>::allocatedBlockCount_;
			// MemoryManager<BlockType>::allocatedBlockCount_
			// mozme to pisat aj ako this->allocatedBLockCount_ alebo iba allocatedBlockCount
			// problem by nastal pri multidedicnosti, ak by sme dedili z 2 tried ten isty atribut s rovnakym nazvom
			limit_ = base_ + (other.limit_ - other.base_);
			for (size_t i = 0; i < other.getAllocatedBlockCount(); i++)
			{
				// vytvor TypBLoku((in˝->b·za + i)*) na (b·za + i)
				placement_copy(base_ + i, *(other.base_ + i)); // *(other.base_ + i) dereferencia adresy na objekt
			}
		}
		return *this;
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::shrinkMemory()
	{
		size_t newCapacity = end_ - base_;

		if (newCapacity < CompactMemoryManager<BlockType>::INIT_SIZE)
		{
			newCapacity = CompactMemoryManager<BlockType>::INIT_SIZE;
		}

		changeCapacity(newCapacity);
	}

	template<typename BlockType>
	void CompactMemoryManager<BlockType>::changeCapacity(size_t newCapacity)
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");

		if (newCapacity == this->getCapacity())
		{
			return;
		}
		if (newCapacity < this->getAllocatedBlockCount())
		{
			this->releaseMemory(base_ + newCapacity);
		}

		void* newBase = std::realloc(base_, newCapacity * sizeof(BlockType)); // realloc - skopiruje vsetky alokovane bajty na inu adresu

		if (newBase == nullptr)
		{
			throw std::bad_alloc();
		}
		base_ = static_cast<BlockType*>(newBase);
		end_ = base_ + this->getAllocatedBlockCount();
		limit_ = base_ + newCapacity;
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::clear()
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		this->releaseMemory(base_);
	}

	template<typename BlockType>
    bool CompactMemoryManager<BlockType>::equals(const CompactMemoryManager<BlockType>& other) const
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		return this == &other ||
			this->getAllocatedBlockCount() == other.getAllocatedBlockCount() &&
			std::memcmp(base_, other.base_, this->getAllocatedBlocksSize()) == 0;
	}

	template<typename BlockType>
    void* CompactMemoryManager<BlockType>::calculateAddress(const BlockType& data)
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		BlockType* p = base_;
		while (p != end_ && p != &data)
		{
			++p;
		}
		/*
		if (p == end_)
			return nullptr;
		else
			return p
		*/
		return p == end_ ? nullptr : p;
	}

	template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::calculateIndex(const BlockType& data)
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");
		/*KeÔ platÌ (dajAdresu(d·ta)<=koniec && dajAdresu(d·ta)>=b·za)tak vr·ù dajAdresu(d·ta)-b·za inak vr·ù -1*/
		return &data < end_ && &data >= base_
			? &data - base_
			: INVALID_INDEX;
	}

	template<typename BlockType>
    BlockType& CompactMemoryManager<BlockType>::getBlockAt(size_t index)
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		return *(base_ + index);
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::swap(size_t index1, size_t index2)
	{
		std::swap(getBlockAt(index1), getBlockAt(index2));
	}

	template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedBlocksSize() const
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		return (end_ - base_) * sizeof(BlockType);
	}

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedCapacitySize() const
	{
		// TODO 02
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		return (limit_ - base_) * sizeof(BlockType);
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::print(std::ostream& os)
	{
		os << "first = " << base_ << std::endl;
		os << "last = " << end_ << std::endl;
		os << "limit = " << limit_ << std::endl;
		os << "block size = " << sizeof(BlockType) << "B" << std::endl;

		BlockType* ptr = base_;
		while (ptr != limit_) {
			std::cout << ptr;
			os << PtrPrintBin<BlockType>(ptr);

			if (ptr == base_) {
				os << "<- first";
			}
			else if (ptr == end_) {
				os << "<- last";
			}
			os << std::endl;
			ptr++;
		}

		os << limit_ << "|<- limit" << std::endl;
	}

}