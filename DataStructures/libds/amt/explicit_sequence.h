#pragma once

#include <libds/amt/abstract_memory_type.h>
#include <libds/amt/sequence.h>
#include <functional>
#include <iterator>

namespace ds::amt {

	template<typename BlockType>
	class ExplicitSequence :
		public Sequence<BlockType>,
		public ExplicitAMS<BlockType>
	{
	public:
		ExplicitSequence();
		ExplicitSequence(const ExplicitSequence& other);
		~ExplicitSequence() override;

		AMT& assign(const AMT& other) override;
		void clear() override;
		bool equals(const AMT& other) override;

		size_t calculateIndex(BlockType& data) override;

		BlockType* accessFirst() const override;
		BlockType* accessLast() const override;
		BlockType* access(size_t index) const override;
		BlockType* accessNext(const BlockType& block) const override;
		BlockType* accessPrevious(const BlockType& block) const override;

		BlockType& insertFirst() override;
		BlockType& insertLast() override;
		BlockType& insert(size_t index) override;
		BlockType& insertAfter(BlockType& block) override;
		BlockType& insertBefore(BlockType& block) override;

		void removeFirst() override;
		void removeLast() override;
		void remove(size_t index) override;
		void removeNext(const BlockType& block) override;
		void removePrevious(const BlockType& block) override;

	protected:
		virtual void connectBlocks(BlockType* previous, BlockType* next);
		virtual void disconnectBlock(BlockType* block);

		BlockType* first_;
		BlockType* last_;

	public:
		using DataType = decltype(BlockType().data_);

		class ExplicitSequenceIterator {
		public:
            explicit ExplicitSequenceIterator(BlockType* position);
			ExplicitSequenceIterator(const ExplicitSequenceIterator& other);
			ExplicitSequenceIterator& operator++();
			ExplicitSequenceIterator operator++(int);
			bool operator==(const ExplicitSequenceIterator& other) const;
			bool operator!=(const ExplicitSequenceIterator& other) const;
			DataType& operator*();

		private:
			BlockType* position_;
		};

		ExplicitSequenceIterator begin();
		ExplicitSequenceIterator end();

		using IteratorType = ExplicitSequenceIterator;
	};

	template<typename BlockType>
	using ES = ExplicitSequence<BlockType>;

	//----------

	template<typename DataType>
	struct SinglyLinkedSequenceBlock :
		public MemoryBlock<DataType>
	{

		SinglyLinkedSequenceBlock() : next_(nullptr) {}
		~SinglyLinkedSequenceBlock() { next_ = nullptr; }

		SinglyLinkedSequenceBlock<DataType>* next_;
	};
	template<typename DataType>
	using SLSBlock = SinglyLinkedSequenceBlock<DataType>;

	template<typename DataType>
	class SinglyLinkedSequence :
		public ES<SLSBlock<DataType>>
	{
	public:
		using BlockType = SinglyLinkedSequenceBlock<DataType>;
	};

	template<typename DataType>
	using SinglyLS = SinglyLinkedSequence<DataType>;

	template<typename DataType>
	class SinglyCyclicLinkedSequence :
		public SinglyLS<DataType>
	{
	};

	template<typename DataType>
	using SinglyCLS = SinglyCyclicLinkedSequence<DataType>;

	//----------

	template<typename DataType>
	struct DoublyLinkedSequenceBlock :
		public SLSBlock<DataType>
	{

		DoublyLinkedSequenceBlock() : previous_(nullptr) {}
		~DoublyLinkedSequenceBlock() { previous_ = nullptr; }

		DoublyLinkedSequenceBlock<DataType>* previous_;
	};

	template<typename DataType>
	using DLSBlock = DoublyLinkedSequenceBlock<DataType>;

	template<typename DataType>
	class DoublyLinkedSequence :
		public ES<DLSBlock<DataType>>
	{
	public:
		using BlockType = DLSBlock<DataType>;

		BlockType* access(size_t index) const override;
		BlockType* accessPrevious(const BlockType& block) const override;

		void removeFirst() override;

	protected:
		void connectBlocks(BlockType* previous, BlockType* next) override;
		void disconnectBlock(BlockType* block) override;
	};

    template<typename DataType>
	using DoublyLS = DoublyLinkedSequence<DataType>;

	template<typename DataType>
	class DoublyCyclicLinkedSequence :
		public DoublyLS<DataType>
	{
	};

    template<typename DataType>
	using DoublyCLS = DoublyCyclicLinkedSequence<DataType>;


	template<typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequence() :
		first_(nullptr),
		last_(nullptr)
	{
	}

	template<typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequence(const ExplicitSequence& other) :
	    ExplicitSequence()
	{
		assign(other);
	}

	template<typename BlockType>
    ExplicitSequence<BlockType>::~ExplicitSequence()
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		this->clear();
	}

	template<typename BlockType>
    AMT& ExplicitSequence<BlockType>::assign(const AMT& other)
	{
		if (this != &other)
		{
			clear();

			const ExplicitSequence<BlockType>& otherExplicitSequence = dynamic_cast<const ExplicitSequence<BlockType>&>(other);
			otherExplicitSequence.processAllBlocksForward([&](const BlockType* b)
			{
			    this->insertLast().data_ = b->data_;
			});
		}

		return *this;
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::clear()
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		BlockType* current = this->first_;
		while (current != nullptr)
		{
			this->first_ = this->accessNext(*this->first_);
			this->memoryManager_->releaseMemory(current);
			current = this->first_;
		}
		this->last_ = nullptr;
	}

	template<typename BlockType>
    bool ExplicitSequence<BlockType>::equals(const AMT& other)
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		const ExplicitSequence<BlockType>* otherES = dynamic_cast<const ExplicitSequence<BlockType>*>(&other); // <> naco to chcem pretypovat, () co chcem pretypovat
		// vznikne nam iba alias otheru
		// const ExplicitSequence<BlockType> otherES = dynamic_cast<const ExplicitSequence<BlockType>&>(other); --> vznikne nam novy objekt
		if (otherES == nullptr || this->size() != otherES->size()) // tu sa kontroluje ci je to implicitna sekvencia alebo explicitna, ak nie je rovny nullptr tak je to explicitna
		{
			return false;
		}
		else if (this == &other) {
			return true;
		}

		BlockType* currentThis = this->first_;
		BlockType* currentOther = otherES->first_;

		while (currentThis != nullptr && currentThis->data_ == currentOther->data_)
		{
			currentThis = this->accessNext(*currentThis);
			currentOther = otherES->accessNext(*currentOther);
		}

		return currentThis == nullptr; // ak sa currentThis == nullptr tak je to identicka sekevencia, ak sa != tak nie je
	}

	template<typename BlockType>
    size_t ExplicitSequence<BlockType>::calculateIndex(BlockType& data)
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");

		// predicate vo findBlockProperty() ... podobnu vec treba pouzit v semestralke
		
		// std::function<bool(BlockType*)> predicate
		size_t i = 0;
		BlockType* foundBlock = this->findBlockWithProperty([&](BlockType* block)  -> bool // lambda funkcia [&] definuje, ze vsetko je pristupne pre funkciu
			{
				++i;
				return &data == block;
			}
		);
		return foundBlock != nullptr ? i - 1 : INVALID_INDEX;
	}

	template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessFirst() const
	{
		return first_;
	}

	template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessLast() const
	{
		return last_;
	}

	template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::access(size_t index) const
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		BlockType* result = nullptr;

		if (index + 1 < this->size())
		{
			result = this->first_;
			for (size_t i = 1; i <= index; i++)
			{
				result = this->accessNext(*result);
			}
		}
		else if (index + 1 == this->size())
		{
			result = this->accessLast();
		}
		return result;
	}

	template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessNext(const BlockType& block) const
	{
		return static_cast<BlockType*>(block.next_);
	}

	template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessPrevious(const BlockType& block) const
	{
		return this->findBlockWithProperty([&](BlockType* b) {return b->next_ == &block; });
	}

	template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertFirst()
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		if (this->size() == 0)
		{
			first_ = last_ = AMS<BlockType>::memoryManager_->allocateMemory();
			return *first_;
		}
		else
		{
			return this->insertBefore(*first_);
		}
	}

	template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertLast()
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		if (this->size() == 0)
		{
			first_ = last_ = AbstractMemoryStructure<BlockType>::memoryManager_->allocateMemory();
			return *last_;
		}
		else
		{
			return this->insertAfter(*last_);
		}
	}

	template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insert(size_t index)
	{
		return index == 0 ? insertFirst() : index == this->size() ? insertLast() : insertAfter(*access(index - 1));
	}

	template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertAfter(BlockType& block)
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		BlockType* nextBlock = this->accessNext(block);
		BlockType* newBlock = AbstractMemoryStructure<BlockType>::memoryManager_->allocateMemory();

		this->connectBlocks(&block, newBlock);
		this->connectBlocks(newBlock, nextBlock);

		if (last_ == &block)
		{
			last_ = newBlock;
		}
		return *newBlock;
	}

	template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertBefore(BlockType& block)
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		BlockType* prevBlock = this->accessPrevious(block);
		BlockType* newBlock = AbstractMemoryStructure<BlockType>::memoryManager_->allocateMemory();

		this->connectBlocks(prevBlock, newBlock);
		this->connectBlocks(newBlock, &block);

		if (first_ == &block)
		{
			first_ = newBlock;
		}
		return *newBlock;
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::removeFirst()
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		if (first_ == last_)
		{
			AbstractMemoryStructure<BlockType>::memoryManager_->releaseMemory(first_);
			first_ = last_ = nullptr;
		}
		else
		{
			BlockType* newFirst = this->accessNext(*first_);
			AbstractMemoryStructure<BlockType>::memoryManager_->releaseMemory(first_);
			first_ = newFirst;
		}
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::removeLast()
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		if (first_ == last_)
		{
			AbstractMemoryStructure<BlockType>::memoryManager_->releaseMemory(last_);
			first_ = last_ = nullptr;
		}
		else
		{
			BlockType* newLast = this->accessPrevious(*last_);
			AbstractMemoryStructure<BlockType>::memoryManager_->releaseMemory(last_);
			last_ = newLast;
			last_->next_ = nullptr;
		}
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::remove(size_t index)
	{
		if (index == 0)
		{
			removeFirst();
		}
		else
		{
			removeNext(*access(index - 1));
		}
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::removeNext(const BlockType& block)
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		BlockType* deletedBlock = this->accessNext(block);
		if (deletedBlock == last_)
		{
			this->removeLast();
		}
		else
		{
			this->disconnectBlock(deletedBlock);
			AbstractMemoryStructure<BlockType>::memoryManager_->releaseMemory(deletedBlock);
		}
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::removePrevious(const BlockType& block)
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		BlockType* deletedBlock = this->accessPrevious(block);
		if (deletedBlock == first_)
		{
			this->removeFirst();
		}
		else
		{
			this->disconnectBlock(deletedBlock);
			AbstractMemoryStructure<BlockType>::memoryManager_->releaseMemory(deletedBlock);
		}
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::connectBlocks(BlockType* previous, BlockType* next)
	{
		if (previous != nullptr)
		{
			previous->next_ = next;
		}
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::disconnectBlock(BlockType* block)
	{
		connectBlocks(accessPrevious(*block), accessNext(*block));
		block->next_ = nullptr;
	}

    template <typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequenceIterator::ExplicitSequenceIterator(BlockType* position) :
		position_(position)
    {
    }

    template <typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequenceIterator::ExplicitSequenceIterator(
        const ExplicitSequenceIterator& other
	) :
		position_(other.position_)
    {
    }

    template <typename BlockType>
    auto ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator++() -> ExplicitSequenceIterator&
    {
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		this->position_ = static_cast<BlockType*>(this->position_->next_); // použili sme static_cast, lebo tie pointre su odlišného typu
		return *this;
    }

    template <typename BlockType>
    auto ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator++(int) -> ExplicitSequenceIterator
    {
		ExplicitSequenceIterator tmp(*this); operator++();
	    return tmp;
    }

    template <typename BlockType>
    bool ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator==(const ExplicitSequenceIterator& other) const
    {
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		return this->position_ == other.position_;
    }

    template <typename BlockType>
    bool ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator!=(const ExplicitSequenceIterator& other) const
    {
		return !(*this == other);
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::DataType& ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator*()
    {
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		return this->position_->data_;
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::ExplicitSequenceIterator ExplicitSequence<BlockType>::begin()
    {
		return ExplicitSequenceIterator(first_);
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::ExplicitSequenceIterator ExplicitSequence<BlockType>::end()
    {
		return ExplicitSequenceIterator(nullptr);
    }

    template<typename DataType>
    DLSBlock<DataType>* DoublyLinkedSequence<DataType>::access(size_t index) const
	{
		// TODO 04
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
		BlockType* result = nullptr;

		if (index < this->size())
		{
			if (index < this->size() / 2)
			{
				result = this->first_;

				for (size_t i = 1; i <= index; i++)
				{
					result = static_cast<BlockType*>(result->next_);
				}
			}
			else
			{
				result = this->last_;

				for (size_t i = this->size() - index; i > 1; i--)
				{
					result = result->previous_;
				}
			}
		}

		return result;
	}

	template<typename DataType>
    DLSBlock<DataType>* DoublyLinkedSequence<DataType>::accessPrevious(const BlockType& block) const
	{
		return block.previous_;
	}

	template<typename DataType>
    void DoublyLinkedSequence<DataType>::removeFirst()
	{
		ExplicitSequence<BlockType>::removeFirst();

		if (ExplicitSequence<BlockType>::first_ != nullptr)
		{
			ExplicitSequence<BlockType>::first_->previous_ = nullptr;
		}
	}

	template<typename DataType>
    void DoublyLinkedSequence<DataType>::connectBlocks(BlockType* previous, BlockType* next)
	{
		ExplicitSequence<BlockType>::connectBlocks(previous, next);

		if (next != nullptr)
		{
			next->previous_ = const_cast<BlockType*>(previous);
		}
	}

	template<typename DataType>
    void DoublyLinkedSequence<DataType>::disconnectBlock(BlockType* block)
	{
		ExplicitSequence<BlockType>::disconnectBlock(block);
		block->previous_ = nullptr;
	}

}