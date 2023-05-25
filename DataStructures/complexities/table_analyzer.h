#pragma once
#include <complexities/complexity_analyzer.h>
#include <libds/adt/table.h>
#include <libds/adt/list.h>
#include <random>

namespace ds::utils
{
    /**
     * @brief Common base for list analyzers.
     */
    template<class Table>
    class TableAnalyzer : public ComplexityAnalyzer<Table>
    {
    public:
        explicit TableAnalyzer(const std::string& name);

    protected:
        void beforeOperation(Table& table) override;
        void executeOperation(Table& table) override;
        void beforeReplication(Table& table) override;

    private:
        void insertNElements(Table& table, size_t n);

    private:
        std::default_random_engine rngIndex_;
        std::default_random_engine rngKey_;
        adt::ImplicitList<int> keyList_;
        int key_;
    };

    class TablesAnalyzer : public CompositeAnalyzer
    {
    public:
        TablesAnalyzer() :
            CompositeAnalyzer("Tables")
        {
            this->addAnalyzer(std::make_unique<TableAnalyzer<ds::adt::UnsortedExplicitSequenceTable<int, int>>>("uest-contains"));
            this->addAnalyzer(std::make_unique<TableAnalyzer<ds::adt::UnsortedImplicitSequenceTable<int, int>>>("uist-contains"));
            //this->addAnalyzer(std::make_unique<TableAnalyzer<ds::adt::SortedSequenceTable<int, int>>>("sst-contains"));
        }
    };

    template<class Table>
    inline void TableAnalyzer<Table>::beforeOperation(Table& table)
    {
        std::uniform_int_distribution<int> indexDist(0, keyList_.size() - 1);
        size_t index = indexDist(this->rngIndex_);
        this->key_ = this->keyList_.access(index);
    }

    template<class Table>
    inline void TableAnalyzer<Table>::executeOperation(Table& table)
    {
        table.contains(this->key_);
    }

    template<class Table>
    inline void TableAnalyzer<Table>::beforeReplication(Table& table)
    {
        this->keyList_.clear();
    }

    template<class Table>
    inline void TableAnalyzer<Table>::insertNElements(Table& table, size_t n)
    {
        std::uniform_int_distribution<int> keyDist(INT_MIN, INT_MAX);

       
        for (size_t i = 0; i < n; i++)
        {
            int key = keyDist(this->rngKey_);
            int data = key;
            
            try {
                table.insert(key, data);
                this->keyList_.insertLast(key);
            }
            catch (...)
            {
                --i;
            }
        }
    }

    template<class Table>
    inline TableAnalyzer<Table>::TableAnalyzer(const std::string& name) :
        ComplexityAnalyzer<Table>(name, [this](Table& table, size_t size)
            {
                this->insertNElements(table, size);
            }),
        rngIndex_(std::random_device()()),
        keyList_(),
        key_(0)
    {
    }

}
