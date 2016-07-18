#ifndef __LYNX_PLATFORM_LIB_CONTAINTER_H__
#define __LYNX_PLATFORM_LIB_CONTAINTER_H__

#include "DataStruct.h"
#include "Logger.h"
#include "MemoryManager.h"

namespace Lynx
{
    template <typename ValueType >
    class List
    {
    public:
        typedef ValueType value_type;

        List() : mIterCount(0)
        {
            LYNX_DLIST_INIT(&mList);
        }

        List(const List<ValueType >& rhs)
        {
            LYNX_DLIST_INIT(&mList);
            mIterCount = 0;
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                insertTail(iter->mValue);
            }
        }

        ~List()
        {
            this->clear();
        }

        struct Iter
        {
            Iter() {};
            Iter(const ValueType& value) : mValue(value) {}

            //struct Entries
            //{
            //    struct Iter* mNext;
            //    struct Iter** mPrev;
            //};

            ValueType mValue;
            //Entries mEntries;
            struct Iter* mNext;
            struct Iter** mPrev;
        };

        typedef Iter iter_type;
        typedef const Iter ConstIter;

        struct _List
        {
            struct Iter* mFirst;
            struct Iter** mLast;
        };

        _List mList;
        unsigned int mIterCount;

        inline bool empty() const
        {
            return (mIterCount == 0);
        }

        inline unsigned int size() const
        {
            return mIterCount;
        }

        inline void clear()
        {
            Iter* iter;
            while (iter = LYNX_DLIST_FIRST(&mList))
            {
                LYNX_DLIST_REMOVE(&mList, iter, mEntries);
                XDELETE(iter);
            }
            mIterCount = 0;
        }

        inline void insertHead(Iter* iter)
        {
            ASSERT(iter);
            LYNX_DLIST_INSERT_HEAD(&mList, iter, mEntries);
            mIterCount++;
        }

        inline Iter* insertHead(const ValueType& value)
        {
            Iter* iter = XNEW(Iter) (value);
            insertHead(iter);
            return iter;
        }

        inline void insertTail(Iter* iter)
        {
            ASSERT(iter);
            LYNX_DLIST_INSERT_TAIL(&mList, iter, mEntries);
            mIterCount++;
        }

        inline Iter* insertTail(const ValueType& value)
        {
            Iter* iter = XNEW(Iter) (value);
            insertTail(iter);
            return iter;
        }

        inline void insertAfter(Iter* listNode, Iter* iter)
        {
            ASSERT(iter && listNode);
            LYNX_DLIST_INSERT_AFTER(&mList, listNode, iter, mEntries);
            mIterCount++;
        }

        inline Iter* insertAfter(const ValueType& value, Iter* listNode)
        {
            Iter* iter = XNEW(Iter)(value);
            insertAfter(listNode, iter);
            return iter;
        }

        inline void insertBefore(Iter* listNode, Iter* iter)
        {
            ASSERT(listNode && iter);
            LYNX_DLIST_INSERT_BEFORE(listNode, iter, mEntries);
            mIterCount++;
        }

        inline Iter* insertBefore(const ValueType& value, Iter* listNode)
        {
            Iter* iter = XNEW(Iter) (value);
            insertBefore(listNode, iter);
            return iter;
        }

        inline Iter* remove(Iter* iter) // 移除节点， 但不销毁内存
        {
            if (iter == NULL) return NULL;
            Iter* nextIter = next(iter);
            LYNX_DLIST_REMOVE(&mList, iter, mEntries);
            mIterCount--;
            return nextIter;
        }

        // 删除迭代器， 返回下一个
        inline Iter* erase(Iter* iter) // 销毁内存
        {
            Iter* nextIter = remove(iter);
            XDELETE(iter);
            return nextIter;
        }

        // 删除迭代器， 返回上一个
        inline Iter* r_erase(Iter* iter) // 销毁内存
        {
            Iter* prevIter = LYNX_DLIST_PREV(iter, _List, mEntries);
            remove(iter);
            return prevIter;
        }

        inline ConstIter* find(const ValueType& value, ConstIter* beginIter, ConstIter* endIter) const
        {
            for (ConstIter* iter = beginIter; iter != endIter; iter = next(iter))
            {
                if (iter->mValue == value)
                {
                    return iter;
                }
            }
            return NULL;
        }

        inline Iter* find(const ValueType& value, Iter* beginIter, Iter* endIter) const
        {
            for (Iter* iter = beginIter; iter != endIter; iter = next(iter))
            {
                if (iter->mValue == value)
                {
                    return iter;
                }
            }
            return NULL;
        }

        inline ConstIter* r_find(const ValueType& value, ConstIter* beginIter, ConstIter* endIter) const
        {
            for (ConstIter* iter = beginIter; iter != endIter; iter = prev(iter))
            {
                if (iter->mValue == value)
                {
                    return iter;
                }
            }
            return NULL;
        }

        inline Iter* r_find(const ValueType& value, Iter* beginIter, Iter* endIter) const
        {
            for (Iter* iter = beginIter; iter != endIter; iter = prev(iter))
            {
                if (iter->mValue == value)
                {
                    return iter;
                }
            }
            return NULL;
        }

        inline Iter* first() const
        {
            return begin();
        }

        inline Iter* last() const
        {
            return LYNX_DLIST_LAST(&mList, _List);
        }

        inline Iter* begin() const
        {
            return LYNX_DLIST_FIRST(&mList);
        }

        inline Iter* r_begin() const
        {
            return LYNX_DLIST_LAST(&mList, _List);
        }

        inline Iter* end() const
        {
            return NULL;
        }

        inline Iter* r_end() const
        {
            return NULL;
        }

        inline Iter* next(Iter* iter) const
        {
            if (iter == NULL) return NULL;
            return LYNX_DLIST_NEXT(iter, mEntries);
        }

        inline ConstIter* next(ConstIter* iter) const
        {
            if (iter == NULL) return NULL;
            return (ConstIter*)LYNX_DLIST_NEXT((Iter*)iter, mEntries);
        }

        inline Iter* prev(Iter* iter) const
        {
            if (iter == NULL) return NULL;
            return LYNX_DLIST_PREV((Iter*)iter, _List, mEntries);
        }

        inline ConstIter* prev(ConstIter* iter) const
        {
            if (iter == NULL) return NULL;
            return (ConstIter*)LYNX_DLIST_PREV(iter, _List, mEntries);
        }

        inline Iter* getn(UInt32 index) const
        {
            UInt32 i = 0;
            for (Iter* iter = begin(); iter != end(); iter = next(iter))
            {
                if (i++ == index)
                {
                    return iter;
                }
            }
            return NULL;
        }

        inline List<ValueType >& operator = (const List<ValueType >& rhs)
        {
            clear();
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                insertTail(iter->mValue);
            }
            return *this;
        }

        inline List<ValueType >& operator + (const List<ValueType >& rhs)
        {
            List<ValueType> ret = *this;
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                ret.insertTail(iter->mValue);
            }
            return ret;
        }

        inline List<ValueType >& operator += (const List<ValueType >& rhs)
        {
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                insertTail(iter->mValue);
            }
            return *this;
        }
    };

    template <class Type> struct IsLynxList { static const bool sValue = false; };
    template <class ValueType >
    struct IsLynxList<Lynx::List<ValueType > > { static const bool sValue = true; };

    template <typename KeyType, typename ValueType, bool UpSort = true>
    class Map
    {
    public:
        typedef KeyType key_type;
        typedef ValueType mapped_type;
        
        Map() : mIterCount(0)
        {
            LYNX_RB_INIT(&mTree);
        }

        Map(const Map<KeyType, ValueType >& rhs)
        {
            LYNX_RB_INIT(&mTree);
            mIterCount = 0;
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                insert(iter->mKey, iter->mValue);
            }
        }

        ~Map()
        {
            this->clear();
        }

        struct Iter
        {
            Iter() {}
            Iter(const KeyType& key, const ValueType& value) : mKey(key), mValue(value) {}
            Iter(const KeyType& key) : mKey(key) {}

            //struct Entries {
            //    struct Iter* rbe_left;
            //    struct Iter* rbe_right;
            //    struct Iter* rbe_parent;
            //    Int32 rbe_color;
            //};

            KeyType mKey;
            ValueType mValue;
            // Entries mEntries;
            struct Iter* rbe_left;
            struct Iter* rbe_right;
            struct Iter* rbe_parent;
            int rbe_color;
        };

        typedef Iter iter_type;
        typedef const Iter ConstIter;

        struct _Map
        {
            struct Iter* rbh_root;
        };

        _Map mTree;
        unsigned int mIterCount;

        inline int compare(Iter* a, Iter* b) const
        {
            if (UpSort == true)
            {
                if (a->mKey > b->mKey) return 1;
                if (a->mKey < b->mKey) return -1;
            }
            else
            {
                if (a->mKey < b->mKey) return 1;
                if (a->mKey > b->mKey) return -1;
            }

            return 0;
        }

        LYNX_RB_GENERATE(_Map, Iter, Dummy, compare);
        //LYNX_RB_GENERATE(_Map, Iter, mEntries, compare);

        inline bool empty() const
        {
            return (mIterCount == 0);
        }

        inline unsigned int size() const
        {
            return mIterCount;
        }

        inline void clear()
        {
            Iter* iter = begin();
            if (iter != NULL)
            {
                while (iter = erase(iter));
            }
        }

        inline Iter* insert(const KeyType& key, const ValueType& value)
        {
            Iter* iter = XNEW(Iter) (key, value);
            ASSERT(iter);
            Iter* tmpIter = LYNX_RB_INSERT(_Map, &mTree, iter);
            if (tmpIter != NULL)
            {
                ASSERT(0);
                XDELETE(iter);
                tmpIter->mValue = value;
                return tmpIter;
            }

            mIterCount++;
            return iter;
        }

        inline Iter* insertAndReplace(const KeyType& key, const ValueType& value)
        {
            Iter* iter = XNEW(Iter) (key, value);
            ASSERT(iter);
            Iter* tmpIter = LYNX_RB_INSERT(_Map, &mTree, iter);
            if (tmpIter != NULL)
            {
                XDELETE(iter);
                tmpIter->mValue = value;
                return tmpIter;
            }

            mIterCount++;
            return iter;
        }

        //inline void pushIter(Iter* iter)
        //{
        //    ASSERT(iter);
        //    Iter* tmpIter = LYNX_RB_INSERT(_Map, &mTree, iter);
        //    if (tmpIter != NULL)
        //    {
        //        LYNX_RB_REMOVE(_Map, &mTree, tmpIter);
        //        XDELETE(tmpIter);
        //        LYNX_RB_INSERT(_Map, &mTree, iter);
        //    }
        //    else
        //    {
        //        mIterCount++;
        //    }
        //}

        inline void erase(const KeyType& key)
        {
            Iter tmpIter(key);
            Iter* iter = LYNX_RB_FIND(_Map, &mTree, &tmpIter);
            if (iter != NULL)
            {
                LYNX_RB_REMOVE(_Map, &mTree, iter);
                XDELETE(iter);
                mIterCount--;
            }
        }

        //inline Iter* popIter(const KeyType& key) // 移除节点， 但不销毁内存
        //{
        //    mTempIter.mKey = key;
        //    Iter* iter = LYNX_RB_FIND(_Map, &mTree, &mTempIter);
        //    if (iter != NULL)
        //    {
        //        LYNX_RB_REMOVE(_Map, &mTree, iter);
        //        //XDELETE(iter);
        //        mIterCount--;
        //        return iter;
        //    }
        //    return NULL;
        //}

        // 删除迭代器，返回下一个迭代器
        inline Iter* erase(Iter* iter)
        {
            Iter* nextIter = LYNX_RB_NEXT(_Map, &mTree, iter);
            LYNX_RB_REMOVE(_Map, &mTree, iter);
            XDELETE(iter);
            mIterCount--;
            return nextIter;
        }

        // 反erase 删除迭代器同时返回前一个迭代器
        inline Iter* r_erase(Iter* iter)
        {
            Iter* prevIter = LYNX_RB_PREV(_Map, &mTree, iter);
            LYNX_RB_REMOVE(_Map, &mTree, iter);
            XDELETE(iter);
            mIterCount--;
            return prevIter;
        }

        inline Iter* find(const KeyType& key) const
        {
            Iter findIter(key);
            //return LYNX_RB_FIND(_Map, &mTree, &findIter);

            struct Iter* tmp = mTree.rbh_root;
            int comp;
            while (tmp)
            {
                comp = compare(&findIter, tmp);
                if (comp < 0)
                {
                    tmp = tmp->rbe_left;
                    //tmp = tmp->mEntries.rbe_left;
                }
                else if (comp > 0)
                {
                    tmp = tmp->rbe_right;
                    //tmp = tmp->mEntries.rbe_right;
                }
                else
                {
                    return tmp;
                }
            }
            return NULL;
        }

		inline ValueType findValue(const KeyType& key) const 
		{
			Iter findIter(key);
			//return LYNX_RB_FIND(_Map, &mTree, &findIter);

			struct Iter* tmp = mTree.rbh_root;
			int comp;
			while (tmp)
			{
				comp = compare(&findIter, tmp);
				if (comp < 0)
				{
					tmp = tmp->rbe_left;
					//tmp = tmp->mEntries.rbe_left;
				}
				else if (comp > 0)
				{
					tmp = tmp->rbe_right;
					//tmp = tmp->mEntries.rbe_right;
				}
				else
				{
					return tmp->mValue;
				}
			}
			return NULL;
		}

		inline ValueType* findRealValue(const KeyType& key) const 
		{
			Iter findIter(key);
			//return LYNX_RB_FIND(_Map, &mTree, &findIter);

			struct Iter* tmp = mTree.rbh_root;
			int comp;
			while (tmp)
			{
				comp = compare(&findIter, tmp);
				if (comp < 0)
				{
					tmp = tmp->rbe_left;
					//tmp = tmp->mEntries.rbe_left;
				}
				else if (comp > 0)
				{
					tmp = tmp->rbe_right;
					//tmp = tmp->mEntries.rbe_right;
				}
				else
				{
					return &tmp->mValue;
				}
			}
			return NULL;
// 			ValueType *val;
// 			return val;
		}

        inline Iter* begin() const
        {
            return LYNX_RB_MIN(_Map, &mTree);
        }

        inline Iter* r_begin() const
        {
            return LYNX_RB_MAX(_Map, &mTree);
        }

        inline Iter* end() const
        {
            return NULL;
        }

        inline Iter* r_end() const
        {
            return NULL;
        }

        inline Iter* prev(Iter* iter) const
        {
            if (iter == NULL) return NULL;
            return LYNX_RB_PREV(_Map, &mTree, iter);
        }

        inline ConstIter* prev(ConstIter* iter) const
        {
            if (iter == NULL) return NULL;
            return (ConstIter*)LYNX_RB_PREV(_Map, &mTree, (Iter*)iter);
        }

        inline Iter* next(Iter* iter) const
        {
            if (iter == NULL) return NULL;
            return LYNX_RB_NEXT(_Map, &mTree, iter);
        }

        inline ConstIter* next(ConstIter* iter) const
        {
            if (iter == NULL) return NULL;
            return LYNX_RB_NEXT(_Map, &mTree, (Iter*)iter);
        }

        inline Iter* getMin() const
        {
            return LYNX_RB_MIN(_Map, &mTree);
        }

        inline Iter* getMax() const
        {
             return LYNX_RB_MAX(_Map, &mTree);
        }

        inline Iter* getn(UInt32 index) const
        {
            UInt32 i = 0;
            for (Iter* iter = begin(); iter != end(); iter = next(iter))
            {
                if (i++ == index)
                {
                    return iter;
                }
            }
            return NULL;
        }

        //inline ValueType& operator [] (const KeyType& key)
        //{
        //    Iter findIter(key);
        //    Iter* iter = LYNX_RB_FIND(_Map, &mTree, &findIter);
        //    if (iter == NULL)
        //    {
        //        iter = XNEW(Iter)(key);
        //        ASSERT(iter);
        //        LYNX_RB_INSERT(_Map, &mTree, iter);
        //        mIterCount++;
        //    }
        //    return iter->mValue;
        //}

        inline Map<KeyType, ValueType >& operator = (const Map<KeyType, ValueType >& rhs)
        {
            clear();
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                insert(iter->mKey, iter->mValue);
            }
            return *this;
        }

        inline Map<KeyType, ValueType > operator + (const Map<KeyType, ValueType >& rhs)
        {
            Map<KeyType, ValueType > ret = *this;
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                ret.insert(iter->mKey, iter->mValue);
            }
            return ret;
        }

        inline Map<KeyType, ValueType >& operator += (const Map<KeyType, ValueType >& rhs)
        {
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                insert(iter->mKey, iter->mValue);
            }
            return *this;
        }

        inline Map<KeyType, ValueType > operator - (const Map<KeyType, ValueType >& rhs)
        {
            Map<KeyType, ValueType > ret = *this;
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                Iter* tmpIter = ret.find(iter->mKey);
                if (tmpIter)
                {
                    ret.erase(tmpIter);
                }
            }
            return ret;
        }

        inline Map<KeyType, ValueType >& operator -= (const Map<KeyType, ValueType >& rhs)
        {
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                Iter* tmpIter = find(iter->mKey);
                if (tmpIter)
                {
                    erase(tmpIter);
                }
            }
            return *this;
        }
    };

    template <class Type> struct IsLynxMap { static const bool sValue = false; };
    template <class KeyType, class ValueType >
    struct IsLynxMap<Lynx::Map<KeyType, ValueType > > { static const bool sValue = true; };

    template <typename ValueType >
    class StringMap
    {
    public:
        typedef ValueType mapped_type;
        
        StringMap() : mIterCount(0)
        {
            LYNX_RB_INIT(&mTree);
        }

        StringMap(const StringMap<ValueType >& rhs)
        {
            LYNX_RB_INIT(&mTree);
            mIterCount = 0;
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                insert(iter->mKey, iter->mValue);
            }
        }

        ~StringMap()
        {
            this->clear();
        }

        struct Iter
        {
            Iter() {}
            Iter(const String& key, const ValueType& value) : mKey(key), mValue(value) {}
            Iter(const String& key) : mKey(key) {}

            //struct Entries {
            //    struct Iter* rbe_left;
            //    struct Iter* rbe_right;
            //    struct Iter* rbe_parent;
            //    Int32 rbe_color;
            //};

            String mKey;
            ValueType mValue;
            //Entries mEntries;
            struct Iter* rbe_left;
            struct Iter* rbe_right;
            struct Iter* rbe_parent;
            int rbe_color;
        };

        typedef Iter iter_type;
        typedef const Iter ConstIter;

        struct _Map
        {
            struct Iter* rbh_root;
        };

        _Map mTree;
        unsigned int mIterCount;

        inline int compare(Iter* a, Iter* b) const
        {
            return strcmp(a->mKey.c_str(), b->mKey.c_str());
        }

        LYNX_RB_GENERATE(_Map, Iter, Dummy, compare);
        //LYNX_RB_GENERATE(_Map, Iter, mEntries, compare);

        inline bool empty() const
        {
            return (mIterCount == 0);
        }

        inline unsigned int size() const
        {
            return mIterCount;
        }

        inline void clear()
        {
            Iter* iter = begin();
            if (iter != NULL)
            {
                while (iter = erase(iter));
            }
        }

        inline Iter* insert(const String& key, const ValueType& value)
        {
            Iter* iter = XNEW(Iter) (key, value);
            ASSERT(iter);
            Iter* tmpIter = LYNX_RB_INSERT(_Map, &mTree, iter);
            if (tmpIter != NULL)
            {
                ASSERT(0);
                XDELETE(iter);
                tmpIter->mValue = value;
                return tmpIter;
            }

            mIterCount++;
            return iter;
        }

        inline Iter* insertAndReplace(const String& key, const ValueType& value)
        {
            Iter* iter = XNEW(Iter) (key, value);
            ASSERT(iter);
            Iter* tmpIter = LYNX_RB_INSERT(_Map, &mTree, iter);
            if (tmpIter != NULL)
            {
                XDELETE(iter);
                tmpIter->mValue = value;
                return tmpIter;
            }

            mIterCount++;
            return iter;
        }

        //inline void pushIter(Iter* iter)
        //{
        //    ASSERT(iter);
        //    Iter* tmpIter = LYNX_RB_INSERT(_Map, &mTree, iter);
        //    if (tmpIter != NULL)
        //    {
        //        LYNX_RB_REMOVE(_Map, &mTree, tmpIter);
        //        XDELETE(tmpIter);
        //        LYNX_RB_INSERT(_Map, &mTree, iter);
        //    }
        //    else
        //    {
        //        mIterCount++;
        //    }
        //}

        inline void erase(const String& key)
        {
            Iter tmpIter(key);
            Iter* iter = LYNX_RB_FIND(_Map, &mTree, &tmpIter);
            if (iter != NULL)
            {
                LYNX_RB_REMOVE(_Map, &mTree, iter);
                XDELETE(iter);
                mIterCount--;
            }
        }

        //inline Iter* popIter(const String& key) // 移除节点， 但不销毁内存
        //{
        //    mTempIter.mKey = key;
        //    Iter* iter = LYNX_RB_FIND(_Map, &mTree, &mTempIter);
        //    if (iter != NULL)
        //    {
        //        LYNX_RB_REMOVE(_Map, &mTree, iter);
        //        //XDELETE(iter);
        //        mIterCount--;
        //        return iter;
        //    }
        //    return NULL;
        //}

        inline Iter* erase(Iter* iter)
        {
            Iter* nextIter = LYNX_RB_NEXT(_Map, &mTree, iter);
            LYNX_RB_REMOVE(_Map, &mTree, iter);
            XDELETE(iter);
            mIterCount--;
            return nextIter;
        }

        inline Iter* r_erase(Iter* iter)
        {
            Iter* prevIter = LYNX_RB_PREV(_Map, &mTree, iter);
            LYNX_RB_REMOVE(_Map, &mTree, iter);
            XDELETE(iter);
            mIterCount--;
            return prevIter;
        }

        inline Iter* find(const String& key) const
        {
            Iter findIter(key);
            //return LYNX_RB_FIND(_Map, &mTree, &findIter);

            struct Iter* tmp = mTree.rbh_root;
            int comp;
            while (tmp)
            {
                comp = compare(&findIter, tmp);
                if (comp < 0)
                {
                    tmp = tmp->rbe_left;
                    //tmp = tmp->mEntries.rbe_left;
                }
                else if (comp > 0)
                {
                    tmp = tmp->rbe_right;
                    //tmp = tmp->mEntries.rbe_right;
                }
                else
                {
                    return tmp;
                }
            }
            return NULL;
        }

        inline Iter* begin() const
        {
            return LYNX_RB_MIN(_Map, &mTree);
        }

        inline Iter* r_begin() const
        {
            return LYNX_RB_MAX(_Map, &mTree);
        }

        inline Iter* end() const
        {
            return NULL;
        }

        inline Iter* r_end() const
        {
            return NULL;
        }

        inline Iter* prev(Iter* iter) const
        {
            if (iter == NULL) return NULL;
            return LYNX_RB_PREV(_Map, &mTree, iter);
        }

        inline ConstIter* prev(ConstIter* iter) const
        {
            if (iter == NULL) return NULL;
            return (ConstIter*)LYNX_RB_PREV(_Map, &mTree, (Iter*)iter);
        }

        inline Iter* next(Iter* iter) const
        {
            if (iter == NULL) return NULL;
            return LYNX_RB_NEXT(_Map, &mTree, iter);
        }

        inline ConstIter* next(ConstIter* iter) const
        {
            if (iter == NULL) return NULL;
            return (ConstIter*)LYNX_RB_NEXT(_Map, &mTree, (Iter*)iter);
        }

        inline Iter* min() const
        {
            return LYNX_RB_MIN(_Map, &mTree);
        }

        inline Iter* max() const
        {
             return LYNX_RB_MAX(_Map, &mTree);
        }

        inline Iter* getn(UInt32 index) const
        {
            UInt32 i = 0;
            for (Iter* iter = begin(); iter != end(); iter = next(iter))
            {
                if (i++ == index)
                {
                    return iter;
                }
            }
            return NULL;
        }

        //inline ValueType& operator [] (const String& key)
        //{
        //    Iter findIter(key);
        //    mTempIter.mKey = key;
        //    Iter* iter = LYNX_RB_FIND(_Map, &mTree, &findIter);
        //    if (iter == NULL)
        //    {
        //        iter = XNEW(Iter)(key);
        //        ASSERT(iter);
        //        LYNX_RB_INSERT(_Map, &mTree, iter);
        //        mIterCount++;
        //    }
        //    return iter->mValue;
        //}

        inline StringMap<ValueType >& operator = (const StringMap<ValueType >& rhs)
        {
            clear();
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                insert(iter->mKey, iter->mValue);
            }
            return *this;
        }

        inline StringMap<ValueType > operator + (const StringMap<ValueType >& rhs)
        {
            StringMap<ValueType > ret = *this;
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                ret.insert(iter->mKey, iter->mValue);
            }
            return ret;
        }

        inline StringMap<ValueType >& operator += (const StringMap<ValueType >& rhs)
        {
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                insert(iter->mKey, iter->mValue);
            }
            return *this;
        }

        inline StringMap<ValueType > operator - (const StringMap<ValueType >& rhs)
        {
            StringMap<ValueType > ret = *this;
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                Iter* tmpIter = ret.find(iter->mKey);
                if (tmpIter)
                {
                    ret.erase(tmpIter);
                }
            }
            return ret;
        }

        inline StringMap<ValueType >& operator -= (const StringMap<ValueType >& rhs)
        {
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                Iter* tmpIter = find(iter->mKey);
                if (tmpIter)
                {
                    erase(tmpIter);
                }
            }
            return *this;
        }
    };

    template <class Type> struct IsLynxStringMap { static const bool sValue = false; };
    template <class ValueType >
    struct IsLynxStringMap<Lynx::StringMap<ValueType > > { static const bool sValue = true; };

    template <typename ValueType>
    class Set
    {
    public:
        typedef ValueType value_type;
        
        Set() : mIterCount(0)
        {
            LYNX_RB_INIT(&mTree);
        }

        Set(const Set<ValueType >& rhs)
        {
            LYNX_RB_INIT(&mTree);
            mIterCount = 0;
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                insert(iter->mValue);
            }
        }

        ~Set()
        {
            this->clear();
        }

        struct Iter
        {
            Iter() {}
            Iter(const ValueType& value) : mValue(value) {}

            //struct Entries {
            //    struct Iter* rbe_left;
            //    struct Iter* rbe_right;
            //    struct Iter* rbe_parent;
            //    Int32 rbe_color;
            //};

            ValueType mValue;
            //Entries mEntries;
            struct Iter* rbe_left;
            struct Iter* rbe_right;
            struct Iter* rbe_parent;
            int rbe_color;
        };

        typedef Iter iter_type;
        typedef const Iter ConstIter;

        struct _Set
        {
            struct Iter* rbh_root;
        };

        _Set mTree;
        unsigned int mIterCount;

        inline int compare(Iter* a, Iter* b) const
        {
            if (b->mValue < a->mValue) return 1;
            if (a->mValue < b->mValue) return -1;
            return 0;
        }

        LYNX_RB_GENERATE(_Set, Iter, Dummy, compare);
        //LYNX_RB_GENERATE(_Set, Iter, mEntries, compare);

        inline bool empty() const
        {
            return (mIterCount == 0);
        }

        inline unsigned int size() const
        {
            return mIterCount;
        }

        inline void clear()
        {
            Iter* iter = begin();
            if (iter != NULL)
            {
                while (iter = erase(iter));
            }
        }

        inline void insert(const ValueType& value)
        {
            Iter* iter = XNEW(Iter) (value);
            ASSERT(iter);
            Iter* tmpIter = LYNX_RB_INSERT(_Set, &mTree, iter);
            if (tmpIter != NULL)
            {
                XDELETE(iter);
                tmpIter->mValue = value;
            }
            else
            {
                mIterCount++;
            }
        }

        inline void erase(const ValueType& value)
        {
            Iter tmpIter(value);
            Iter* iter = LYNX_RB_FIND(_Set, &mTree, &tmpIter);
            if (iter != NULL)
            {
                LYNX_RB_REMOVE(_Set, &mTree, iter);
                XDELETE(iter);
                mIterCount--;
            }
        }

        inline Iter* erase(Iter* iter)
        {
            Iter* nextIter = LYNX_RB_NEXT(_Set, &mTree, iter);
            LYNX_RB_REMOVE(_Set, &mTree, iter);
            XDELETE(iter);
            mIterCount--;
            return nextIter;
        }

        inline Iter* find(const ValueType& value) const
        {
            Iter findIter(value);
            //return LYNX_RB_FIND(_Set, &mTree, &findIter);

            struct Iter* tmp = mTree.rbh_root;
            int comp;
            while (tmp)
            {
                comp = compare(&findIter, tmp);
                if (comp < 0)
                {
                    tmp = tmp->rbe_left;
                    //tmp = tmp->mEntries.rbe_left;
                }
                else if (comp > 0)
                {
                    tmp = tmp->rbe_right;
                    //tmp = tmp->mEntries.rbe_right;
                }
                else
                {
                    return tmp;
                }
            }
            return NULL;
        }

        inline Iter* begin() const
        {
            return LYNX_RB_MIN(_Set, &mTree);
        }

         inline Iter* r_begin() const
        {
            return LYNX_RB_MAX(_Set, &mTree);
        }

        inline Iter* end() const
        {
            return NULL;
        }

        inline Iter* r_end() const
        {
            return NULL;
        }

        inline Iter* prev(Iter* iter) const
        {
            if (iter == NULL) return NULL;
            return LYNX_RB_PREV(_Set, &mTree, iter);
        }

        inline ConstIter* prev(ConstIter* iter) const
        {
            if (iter == NULL) return NULL;
            return (ConstIter*)LYNX_RB_PREV(_Set, &mTree, (Iter*)iter);
        }

        inline Iter* next(Iter* iter) const
        {
            if (iter == NULL) return NULL;
            return LYNX_RB_NEXT(_Set, &mTree, iter);
        }

        inline ConstIter* next(ConstIter* iter) const
        {
            if (iter == NULL) return NULL;
            return (ConstIter*)LYNX_RB_NEXT(_Set, &mTree, (Iter*)iter);
        }

        inline Iter* getn(UInt32 index) const
        {
            UInt32 i = 0;
            for (Iter* iter = begin(); iter != end(); iter = next(iter))
            {
                if (i++ == index)
                {
                    return iter;
                }
            }
            return NULL;
        }

        //inline ValueType& operator [] (const ValueType& value)
        //{
        //    Iter findIter(value);
        //    Iter* iter = LYNX_RB_FIND(_Set, &mTree, &findIter);
        //    if (iter == NULL)
        //    {
        //        iter = XNEW(Iter)(value);
        //        ASSERT(iter);
        //        LYNX_RB_INSERT(_Set, &mTree, iter);
        //        mIterCount++;
        //    }
        //    return iter->mValue;
        //}

        inline Set<ValueType >& operator = (const Set<ValueType >& rhs)
        {
            clear();
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                insert(iter->mValue);
            }
            return *this;
        }

        inline Set<ValueType > operator + (const Set<ValueType >& rhs)
        {
            Set<ValueType > ret = *this;
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                ret.insert(iter->mValue);
            }
            return ret;
        }

        inline Set<ValueType >& operator += (const Set<ValueType >& rhs)
        {
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                insert(iter->mValue);
            }
            return *this;
        }

        inline Set<ValueType > operator - (const Set<ValueType >& rhs)
        {
            Set<ValueType > ret = *this;
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                Iter* tmpIter = ret.find(iter->mValue);
                if (tmpIter)
                {
                    ret.erase(tmpIter);
                }
            }
            return ret;
        }

        inline Set<ValueType >& operator -= (const Set<ValueType >& rhs)
        {
            for (Iter* iter = rhs.begin(); iter != rhs.end(); iter = rhs.next(iter))
            {
                Iter* tmpIter = find(iter->mValue);
                if (tmpIter)
                {
                    erase(tmpIter);
                }
            }
            return *this;
        }
    };

    template <class Type> struct IsLynxSet { static const bool sValue = false; };
    template <class ValueType>
    struct IsLynxSet<Lynx::Set<ValueType> > { static const bool sValue = true; };

    template <typename PointerType, UInt32 size>
    class PointerArray
    {
    public:
        PointerArray()
        {
            memset(mArray, 0, sizeof(PointerType) * (size + 1));
            mMaxIndex = 0;
            mValidSize = 0;
        }

        ~PointerArray()
        {
        }

        inline UInt32 getMaxIndex()
        {
            return mMaxIndex;
        }

        inline UInt32 getValidSize()
        {
            return mValidSize;
        }

        inline UInt32 insert(PointerType value)
        {
            for (UInt32 i = 1; i <= size; i++)
            {
                if (mArray[i] == NULL)
                {
                    mArray[i] = value;
                    if (i > mMaxIndex)
                    {
                        mMaxIndex = i;
                    }
                    mValidSize++;
                    return i;
                }
            }
            return 0;
        }

        inline bool insert(UInt32 key, PointerType value)
        {
            if (key > size)
            {
                LOG_WARN("PointerArray overflow key: %d, size: %d", key, size);
                return false;
            }
            ASSERT(!mArray[key]);
            mArray[key] = value;
            mValidSize++;
            if (key > mMaxIndex) mMaxIndex = key;
            return true;
        }

        inline PointerType erase(UInt32 key)
        {
            if (key > size)
            {
                LOG_WARN("PointerArray overflow key: %d, size: %d", key, size);
                return NULL;
            }
  
            PointerType ret = mArray[key];
            mArray[key] = NULL;
            mValidSize--;
            if (key == mMaxIndex)
            {
                UInt32 i = mMaxIndex;
                for (; i > 1 && i <= size; i--)
                {
                    if (mArray[i] != NULL) 
                    {
                        mMaxIndex = i;
                        return ret;
                    }
                }
                mMaxIndex = i;
            }
            return ret;
        }

        inline PointerType find(UInt32 key)
        {
            if (key > size)
            {
                LOG_WARN("PointerArray overflow key: %d, size: %d", key, size);
                return NULL;
            }
            return mArray[key];
        }

        inline Vector<PointerType> toVector()
        {
            Vector<PointerType> ret;
            for (UInt32 i = 1; i <= mMaxIndex; i++)
            {
                if (mArray[i] != NULL)
                {
                    ret.push_back(mArray[i]);
                }
            }
            return ret;
        }

        inline PointerType operator [] (UInt32 key)
        {
            if (key > size)
            {
                LOG_WARN("PointerArray overflow key: %d, size: %d", key, size);
                return NULL;
            }
            return mArray[key];
        }
    private:
        PointerType mArray[size+1];
        UInt32 mMaxIndex;
        UInt32 mValidSize;
    };

    class TireTree
    {
    public:
        TireTree(){}

        ~TireTree() 
        {
            clean(&mTree);
        }

        enum Flag
        {
            BYTE_IS_NULL          = 0x00,
            BYTE_HAS_NEXT         = 0x01,
            BYTE_IS_END           = 0x10,
        };

        struct Byte
        {
            UInt8 mFlag;
            UInt32 mKey;
            void* mNext;
            Byte() : mFlag(BYTE_IS_NULL), mKey(0), mNext(NULL) {}
        };

        typedef Map<UInt8, Byte> TireTreeMap;
        TireTreeMap mTree;

        // Construct dictionary, key == 0 indicate null
        inline void addWord(const char* word, UInt32 key = 0)
        {
            if (word == NULL)
            {
                return;
            }

            TireTreeMap* tree = &mTree;
            Int32 len = (Int32)strlen(word);

            for (Int32 i = 0; i < len; i++)
            {
                unsigned char c = word[i];
                TireTreeMap::Iter* iter = tree->find(c);
                if (iter == NULL)
                {
                    Byte byte;
                    iter = tree->insert(c, byte);
                }

                if (i == len - 1)
                {
                    iter->mValue.mFlag |= BYTE_IS_END;
                    iter->mValue.mKey = key;
                    break;
                }
                else
                {
                    iter->mValue.mFlag |= BYTE_HAS_NEXT;
                }

                if (iter->mValue.mNext == NULL)
                {
                    TireTreeMap* subTree = XNEW(TireTreeMap)();
                    iter->mValue.mNext = subTree;
                }

                tree = (TireTreeMap*)iter->mValue.mNext;
            }
        }

        inline void delWord(const char* word)
        {
            if (word == NULL)
            {
                return;
            }

            TireTreeMap* tree = &mTree;
            Int32 len = (Int32)strlen(word);

            for (Int32 i = 0; i < len; i++)
            {
                if (tree == NULL) return;

                unsigned char c = word[i];
                TireTreeMap::Iter* iter = tree->find(c);
                if (iter == NULL)
                {
                    return;
                }

                if (i == len - 1 && (iter->mValue.mFlag & BYTE_IS_END))
                {
                    iter->mValue.mFlag &= ~BYTE_IS_END;
                    iter->mValue.mKey = 0;
                    return;
                }
                tree = (TireTreeMap*)iter->mValue.mNext;
            }
        }

        // 快速字符串查找
        inline UInt32 getWord(const char* word)
        {
            if (word == NULL)
            {
                return 0;
            }

            TireTreeMap* tree = &mTree;
            Int32 len = (Int32)strlen(word);

            for (Int32 i = 0; i < len; i++)
            {
                if (tree == NULL) return 0;

                unsigned char c = word[i];
                TireTreeMap::Iter* iter = tree->find(c);
                if (iter == NULL)
                {
                    return 0;
                }

                if (i == len - 1 && (iter->mValue.mFlag & BYTE_IS_END))
                {
                    return iter->mValue.mKey;
                }

                tree = (TireTreeMap*)iter->mValue.mNext;
            }
            return 0;
        }

        inline void clean(TireTreeMap* tree)
        {
            if (tree == NULL) return;
            for (TireTreeMap::Iter* iter = tree->begin(); iter != tree->end();)
            {
                clean((TireTreeMap*)iter->mValue.mNext);
                XDELETE((TireTreeMap*)iter->mValue.mNext);
                iter = tree->erase(iter);
            }
        }
    };

    template <UInt32 size>
    class CharArray  
    { 
    public:
        CharArray() : mLength(0)
        {
            memset(mData, 0, size + 4);
        }

        CharArray(const char* str) 
        {
            if (str == NULL)
            {
                memset(mData, 0, size + 4);
                return;
            }

            UInt32 len = strlen(str);
            if (len > size)
            {
                snprintf(mData, size + 1, "%s", str);
                mData[size] = 0;
                mLength = size;
            }
            else
            {
                snprintf(mData, len + 1, "%s", str);
                mData[len] = 0;
                mLength = len;
            }
        }

        CharArray(const CharArray<size>& rhs)
        {
            mLength = rhs.mLength;
            memcpy(mData, rhs.mData, size + 1);
        }

        virtual ~CharArray() {}

        inline UInt32 length() const { return mLength; }

        inline const char * c_str() const   
        {
            return mData; 
        }

        inline bool find(const char c)
        {
            for (UInt32 i = 0; i < mLength; i++)
            {
                if (mData[i] == c)
                {
                    return true;
                }
            }
            return false;
        }

        bool operator == (const CharArray<size>& rhs)
        {
            if (mLength != rhs.mLength)
            {
                return false;
            }

            if (strncmp(mData, rhs.c_str(), mLength))
            {
                return false;
            }
            return true;
        }

        friend bool operator == (const CharArray<size>& a, const char* b) 
        {
            if (b == NULL)
            {
                if (a.mLength == 0)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            if (a.mLength != strlen(b))
            {
                return false;
            }

            if (strncmp(a.mData, b, a.mLength))
            {
                return false;
            }
            return true;
        }

        friend bool operator == (const char* a, const CharArray<size>& b) 
        {
            if (a == NULL)
            {
                if (b.mLength == 0)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            if (b.mLength != strlen(a))
            {
                return false;
            }

            if (strncmp(b.mData, a, b.mLength))
            {
                return false;
            }
            return true;
        }

        friend bool operator == (const CharArray<size>& a, const String& b)
        {
            if (a.mLength != b.size())
            {
                return false;
            }

            if (strncmp(a.mData, b.c_str(), a.mLength))
            {
                return false;
            }
            return true;
        }

        friend bool operator == (const String& a, const CharArray<size>& b)
        {
            if (b.mLength != a.size())
            {
                return false;
            }

            if (strncmp(b.mData, a.c_str(), b.mLength))
            {
                return false;
            }
            return true;
        }

        inline CharArray<size>& operator = (const CharArray<size>& rhs)
        {
            mLength = rhs.mLength;
            memcpy(mData, rhs.mData, size + 1);
            return *this;
        }

        inline CharArray<size>& operator = (const char* rhs)
        {
            if (rhs == NULL)
            {
                return *this;
            }

            UInt32 len = strlen(rhs);
            if (len > size)
            {
                snprintf(mData, size + 1, "%s", rhs);
                mData[size] = 0;
                mLength = size;
            }
            else
            {
                snprintf(mData, len + 1, "%s", rhs);
                mData[len] = 0;
                mLength = len;
            }
            return *this;
        }

        inline CharArray<size> operator + (const CharArray<size>& rhs)
        {
            CharArray<size> ret;
            UInt32 len = mLength + rhs.mLength;
            if (len > size)
            {
                snprintf(ret.mData, mLength + 1, "%s", mData);
                snprintf(ret.mData + mLength, size - mLength + 1, "%s", rhs.mData);
                ret.mLength = size;
            }
            else
            {
                snprintf(ret.mData, mLength + 1, "%s", mData);
                snprintf(ret.mData + mLength, rhs.mLength + 1, "%s", rhs.mData);
                ret.mLength = len;
            }
            return ret;
        }

        inline CharArray<size> operator + (const char* rhs)
        {
            if (rhs == NULL)
            {
                CharArray<size> ret;
                UInt32 len = mLength;
                snprintf(ret.mData, mLength + 1, "%s", mData);
                ret.mLength = len;
                return ret;
            }

            CharArray<size> ret;
            UInt32 len = mLength + strlen(rhs);
            if (len > size)
            {
                snprintf(ret.mData, mLength + 1, "%s", mData);
                snprintf(ret.mData + mLength, size - mLength + 1, "%s", rhs);
                ret.mLength = size;
            }
            else
            {
                snprintf(ret.mData, mLength + 1, "%s", mData);
                snprintf(ret.mData + mLength, strlen(rhs) + 1, "%s", rhs);
                ret.mLength = len;
            }
            return ret;
        }

        inline CharArray<size> operator + (const char c)
        {
            CharArray<size> ret;
            UInt32 len = mLength + 1;
            if (len > size)
            {
                return *this;
            }
            else
            {
                ret = *this;
                ret.mData[ret.mLength] = c;
                ret.mLength++;
                ret.mData[ret.mLength] = 0;
                return ret;
            }
        }

        inline CharArray<size>& operator += (const CharArray<size>& rhs)
        {
            UInt32 len = mLength + rhs.length();
            if (len > size)
            {
                snprintf(mData + mLength, size - mLength + 1, "%s", rhs.mData);
                mLength = size;
            }
            else
            {
                snprintf(mData + mLength, rhs.length() + 1, "%s", rhs.mData);
                mLength += rhs.mLength;
            }
            mData[mLength] = 0;
            return *this;
        }

        inline CharArray<size>& operator += (const char* rhs)
        {
            if (rhs == NULL)
            {
                return *this;
            }

            UInt32 len = mLength + strlen(rhs);
            if (len > size)
            {
                snprintf(mData + mLength, size - mLength + 1, "%s", rhs);
                mLength = size;
            }
            else
            {
                snprintf(mData + mLength, strlen(rhs) + 1, "%s", rhs);
                mLength = len;
            }
            mData[mLength] = 0;
            return *this;
        }

        inline CharArray<size>& operator += (const char c)
        {
            UInt32 len = mLength + 1;
            if (len > size)
            {
                return *this;
            }
            else
            {
                mData[mLength] = c;
                mLength++;
                mData[mLength] = 0;
                return *this;
            }
        }

        inline char operator [] (UInt32 index)
        {
            return mData[index];
        }

    private:
        UInt32         mLength;
        char           mData[size + 4];
    };

    template <class Type> struct IsLynxCharArray { static const bool sValue = false; };
    template <UInt32 size >
    struct IsLynxCharArray<Lynx::CharArray<size > > { static const bool sValue = true; };

    typedef CharArray<40> NameString;
    typedef CharArray<6> IconString;
	typedef CharArray<50> AttributeString;
} // namespace Lynx

#endif // __LYNX_PLATFORM_LIB_CONTAINTER_H__





