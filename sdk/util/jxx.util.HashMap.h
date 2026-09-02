#pragma once

#include <cmath>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.AbstractMap.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.Set.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx
{
	namespace util
	{

		template <typename K, typename V>
		class HashMap : public jxx::lang::ClassBase<HashMap<K, V>,
			AbstractMap<K, V>, 
			jxx::lang::Cloneable, 
			jxx::io::SerializableI>
		{
		private:
			struct KeyHash
			{
				std::size_t operator()(const jxx::Ptr<K>& key) const
				{
					if (key == nullptr) {
						return 0u;
					}

					auto object = jxx::CAST<jxx::lang::Object>(key);
					if (object == nullptr) {
						return reinterpret_cast<std::size_t>(key.get());
					}

					return static_cast<std::size_t>(object->hashCode());
				}
			};

			struct KeyEq
			{
				bool operator()(
					const jxx::Ptr<K>& left,
					const jxx::Ptr<K>& right) const
				{

					if (left == nullptr || right == nullptr) {
						return left == right;
					}

					auto leftObject =
						jxx::CAST<jxx::lang::Object>(left);

					if (leftObject == nullptr) {
						return left.get() == right.get();
					}

					return leftObject->equals(
						jxx::CAST<jxx::lang::Object>(right));
				}
			};

			using InternalMap = std::unordered_map<
				jxx::Ptr<K>,
				jxx::Ptr<V>,
				KeyHash,
				KeyEq>;

			static constexpr jxx::lang::jint DEFAULT_INITIAL_CAPACITY = 16;
			static constexpr jxx::lang::jfloat DEFAULT_LOAD_FACTOR = 0.75f;

			InternalMap map_;
			jxx::lang::jfloat loadFactor_;
			jxx::lang::jint modCount_;

			jxx::Ptr<Set<MapEntry<K, V>>> entrySetView_;

			static jxx::Ptr<K> castObjectToKey(
                const jxx::Ptr<jxx::lang::Object>& object)
			{

				return jxx::CAST<K>(object);
			}

			static jxx::lang::jbool objectEquals(
                const jxx::Ptr<jxx::lang::Object>& left,
                const jxx::Ptr<jxx::lang::Object>& right)
			{

				if (left == nullptr || right == nullptr) {
					return static_cast<jxx::lang::jbool>(left == right);
				}

				return left->equals(right);
			}

		protected:
			virtual void afterNodeAccess(const jxx::Ptr<K>& /*key*/)
			{
			}

			virtual void afterNodeInsertion(const jxx::Ptr<K>& /*key*/,
				jxx::lang::jbool /*isNewKey*/)
			{
			}

			virtual void afterNodeRemoval(const jxx::Ptr<K>& /*key*/)
			{
			}

			virtual void afterClear()
			{
			}

            jxx::lang::jint modificationCount_() const noexcept
            {
                return modCount_;
            }

            void incrementModificationCount_() noexcept
            {
                ++modCount_;
            }

            jxx::lang::jfloat loadFactorValue_() const noexcept
            {
                return loadFactor_;
            }

            jxx::Ptr<V> getWithoutAccess_(
                const jxx::Ptr<K>& key) const
            {
                const auto found = map_.find(key);
                return found == map_.end() ? nullptr : found->second;
            }

		public:
			HashMap()
				: map_()
				, loadFactor_(DEFAULT_LOAD_FACTOR)
				, modCount_(0)
				, entrySetView_(nullptr)
			{

				map_.max_load_factor(
					static_cast<float>(loadFactor_));

				map_.reserve(
					static_cast<std::size_t>(
						DEFAULT_INITIAL_CAPACITY));
			}

			explicit HashMap(jxx::lang::jint initialCapacity)
				: HashMap(
					initialCapacity,
					DEFAULT_LOAD_FACTOR)
			{
			}

			HashMap(
				jxx::lang::jint initialCapacity,
				jxx::lang::jfloat loadFactor)
				: map_()
				, loadFactor_(loadFactor)
				, modCount_(0)
				, entrySetView_(nullptr)
			{

				if (initialCapacity < 0) {
					throw jxx::lang::IllegalArgumentException();
				}

				if (!(loadFactor > 0.0f) ||
					std::isnan(static_cast<double>(loadFactor))) {

					throw jxx::lang::IllegalArgumentException();
				}

				map_.max_load_factor(
					static_cast<float>(loadFactor_));

				map_.reserve(
					static_cast<std::size_t>(
						initialCapacity));
			}

			explicit HashMap(const jxx::Ptr<Map<K, V>>& source)
				: HashMap()
			{

				if (source == nullptr) {
					throw jxx::lang::NullPointerException();
				}

				putAll(source);
			}

			virtual ~HashMap() = default;

			/*
			 * Your Serializable interface is pure virtual rather than
			 * Java's marker-only interface. These methods make HashMap
			 * concrete without inventing ObjectStream APIs.
			 */
			virtual void writeObject(
				const jxx::Ptr<jxx::io::ObjectOutputStream>& out) override
			{

				if (out == nullptr) {
					throw jxx::lang::NullPointerException();
				}

				throw jxx::lang::UnsupportedOperationException();
			}

			virtual void readObject(
				const jxx::Ptr<jxx::io::ObjectInputStream>& in) override
			{

				if (in == nullptr) {
					throw jxx::lang::NullPointerException();
				}

				throw jxx::lang::UnsupportedOperationException();
			}

			virtual void readObjectNoData() override
			{
				throw jxx::lang::UnsupportedOperationException();
			}

			virtual jxx::lang::jint size() override
			{
				return static_cast<jxx::lang::jint>(
					map_.size());
			}

			virtual jxx::lang::jbool isEmpty() override
			{
				return static_cast<jxx::lang::jbool>(
					map_.empty());
			}

			virtual jxx::lang::jbool containsKey(
				const jxx::Ptr<jxx::lang::Object>& key) override
			{

				if (key == nullptr) {
					return static_cast<jxx::lang::jbool>(
						map_.find(nullptr) != map_.end());
				}

				auto castKey = castObjectToKey(key);
				if (castKey == nullptr) {
					return static_cast<jxx::lang::jbool>(false);
				}

				return static_cast<jxx::lang::jbool>(
					map_.find(castKey) != map_.end());
			}

			virtual jxx::lang::jbool containsValue(
				const jxx::Ptr<jxx::lang::Object>& value) override
			{

				for (const auto& pair : map_) {
					auto mappedValue =
						jxx::CAST<jxx::lang::Object>(
							pair.second);

					if (objectEquals(mappedValue, value)) {
						return static_cast<jxx::lang::jbool>(true);
					}
				}

				return static_cast<jxx::lang::jbool>(false);
			}

			virtual jxx::Ptr<V> get(
				const jxx::Ptr<jxx::lang::Object>& key) override
			{

				jxx::Ptr<K> castKey = nullptr;

				if (key != nullptr) {
					castKey = castObjectToKey(key);

					if (castKey == nullptr) {
						return nullptr;
					}
				}

				auto found = map_.find(castKey);
				if (found == map_.end()) {
					return nullptr;
				}

				afterNodeAccess(castKey);
				return found->second;
			}

			virtual jxx::Ptr<V> put(
				const jxx::Ptr<K>& key,
				const jxx::Ptr<V>& value) override
			{

				auto found = map_.find(key);

				if (found == map_.end()) {
					map_.emplace(key, value);
					++modCount_;

					afterNodeInsertion(
						key,
						static_cast<jxx::lang::jbool>(true));

					return nullptr;
				}

				auto previous = found->second;
				found->second = value;

				afterNodeAccess(key);
				return previous;
			}

			virtual jxx::Ptr<V> remove(
				const jxx::Ptr<jxx::lang::Object>& key) override
			{

				jxx::Ptr<K> castKey = nullptr;

				if (key != nullptr) {
					castKey = castObjectToKey(key);

					if (castKey == nullptr) {
						return nullptr;
					}
				}

				auto found = map_.find(castKey);
				if (found == map_.end()) {
					return nullptr;
				}

				auto previous = found->second;

				map_.erase(found);
				++modCount_;

				afterNodeRemoval(castKey);
				return previous;
			}

			virtual void putAll(
				const jxx::Ptr<Map<K, V>>& source) override
			{

				if (source == nullptr) {
					throw jxx::lang::NullPointerException();
				}

				auto entries = source->entrySet();
				if (entries == nullptr) {
					return;
				}

				auto iterator = entries->iterator();

				while (iterator->hasNext()) {
					auto entry = iterator->next();

					if (entry != nullptr) {
						put(
							entry->getKey(),
							entry->getValue());
					}
				}
			}

			virtual void clear() override
			{
				if (map_.empty()) {
					return;
				}

				map_.clear();
				++modCount_;

				afterClear();
			}

		protected:
			class EntryView
                : public jxx::lang::ClassBase<
                      EntryView,
                      jxx::lang::Object,
                      MapEntry<K, V>>
			{
			private:
				jxx::Ptr<HashMap<K, V>> owner_;
				jxx::Ptr<K> key_;

			public:
				EntryView(
					HashMap<K, V>* owner,
					const jxx::Ptr<K>& key)
					: owner_(owner)
					, key_(key)
				{
				}

				virtual ~EntryView() = default;

				virtual jxx::Ptr<K> getKey() override
				{
					return key_;
				}

				virtual jxx::Ptr<V> getValue() override
				{
					return owner_->get(
						jxx::CAST<jxx::lang::Object>(key_));
				}

				virtual jxx::Ptr<V> setValue(
                    const jxx::Ptr<V>& value) override
				{

					return owner_->put(key_, value);
				}

				virtual jxx::lang::jbool equals(
                    const jxx::Ptr<jxx::lang::Object>& object) const override
				{

					auto other =
						jxx::CAST<MapEntry<K, V>>(object);

					if (other == nullptr) {
						return static_cast<jxx::lang::jbool>(false);
					}

					auto thisKey =
						jxx::CAST<jxx::lang::Object>(key_);

					auto otherKey =
						jxx::CAST<jxx::lang::Object>(
							other->getKey());

					auto thisValue =
						jxx::CAST<jxx::lang::Object>(
							const_cast<EntryView*>(this)->getValue());

					auto otherValue =
						jxx::CAST<jxx::lang::Object>(
							other->getValue());

					return static_cast<jxx::lang::jbool>(
						objectEquals(thisKey, otherKey) &&
						objectEquals(thisValue, otherValue));
				}

				virtual jxx::lang::jint hashCode() const override
				{
					auto keyObject =
						jxx::CAST<jxx::lang::Object>(key_);

					auto valueObject =
						jxx::CAST<jxx::lang::Object>(
							const_cast<EntryView*>(this)->getValue());

					const jxx::lang::jint keyHash =
						keyObject == nullptr
						? 0
						: keyObject->hashCode();

					const jxx::lang::jint valueHash =
						valueObject == nullptr
						? 0
						: valueObject->hashCode();

					return keyHash ^ valueHash;
				}

				virtual jxx::Ptr<jxx::lang::String> toString() const override
				{
					auto keyObject =
						jxx::CAST<jxx::lang::Object>(key_);

					auto valueObject =
						jxx::CAST<jxx::lang::Object>(
							const_cast<EntryView*>(this)->getValue());

                    const std::string keyText =
						keyObject == nullptr
						? "null"
						: keyObject->toString()->utf8();

					const std::string valueText =
						valueObject == nullptr
						? "null"
						: valueObject->toString()->utf8();

					return jxx::NEW<jxx::lang::String>(
                        keyText + "=" + valueText);
				}
			};

			virtual jxx::Ptr<MapEntry<K, V>> makeEntryView(
                const jxx::Ptr<K>& key)
            {
                auto owner =
                    jxx::CAST<HashMap<K, V>>(this->thisPtr);

                if (owner == nullptr) {
                    throw jxx::lang::IllegalStateException();
                }

                auto entry = jxx::NEW<EntryView>(owner, key);
                return jxx::CAST<MapEntry<K, V>>(entry);
            }

			class EntryIterator
                : public jxx::lang::ClassBase<
                      EntryIterator,
                      jxx::lang::Object,
                      Iterator<MapEntry<K, V>>>
			{
			private:
				jxx::Ptr<HashMap<K, V>> owner_;

				/*
				 * Snapshot only the keys. This avoids exposing STL and avoids
				 * storing unordered_map iterators across map mutations.
				 */
				std::vector<jxx::Ptr<K>> keys_;
				std::size_t cursor_;

				jxx::Ptr<K> lastReturnedKey_;
				jxx::lang::jbool canRemove_;
				jxx::lang::jint expectedModCount_;

				void checkForComodification() const
				{
					if (owner_->modCount_ != expectedModCount_) {
						throw jxx::util::ConcurrentModificationException();
					}
				}

			public:
				explicit EntryIterator(const jxx::Ptr<HashMap<K, V>>& owner)
					: owner_(owner)
					, keys_()
					, cursor_(0)
					, lastReturnedKey_(nullptr)
					, canRemove_(
						static_cast<jxx::lang::jbool>(false))
					, expectedModCount_(owner->modCount_)
				{

					keys_.reserve(owner_->map_.size());

					for (const auto& pair : owner_->map_) {
						keys_.push_back(pair.first);
					}
				}

				virtual ~EntryIterator() = default;

				virtual jxx::lang::jbool hasNext() override
				{
					return static_cast<jxx::lang::jbool>(
						cursor_ < keys_.size());
				}

				virtual jxx::Ptr<MapEntry<K, V>> next() override
				{
					checkForComodification();

					if (cursor_ >= keys_.size()) {
						throw jxx::util::NoSuchElementException();
					}

					lastReturnedKey_ = keys_[cursor_++];
					canRemove_ =
						static_cast<jxx::lang::jbool>(true);

					return owner_->makeEntryView(
						lastReturnedKey_);
				}

				virtual void remove() override
				{
					if (!canRemove_) {
						throw jxx::lang::IllegalStateException();
					}

					checkForComodification();

					owner_->remove(
						jxx::CAST<jxx::lang::Object>(
							lastReturnedKey_));

					expectedModCount_ = owner_->modCount_;

					lastReturnedKey_ = nullptr;
					canRemove_ =
						static_cast<jxx::lang::jbool>(false);
				}
			};

			class EntrySet
                : public AbstractSet<MapEntry<K, V>>
			{
			private:
				jxx::Ptr<HashMap<K, V>> owner_;

			public:
				explicit EntrySet(const jxx::Ptr<HashMap<K, V>>& owner)
					: owner_(owner)
				{
				}

				virtual ~EntrySet() = default;

				virtual jxx::lang::jint size() override
				{
					return owner_->size();
				}

				virtual jxx::lang::jbool isEmpty() override
				{
					return owner_->isEmpty();
				}

				virtual jxx::lang::jbool contains(
                    const jxx::Ptr<jxx::lang::Object>& object) override
				{

					auto entry =
						jxx::CAST<MapEntry<K, V>>(object);

					if (entry == nullptr) {
						return static_cast<jxx::lang::jbool>(false);
					}

					auto keyObject =
						jxx::CAST<jxx::lang::Object>(
							entry->getKey());

					if (!owner_->containsKey(keyObject)) {
						return static_cast<jxx::lang::jbool>(false);
					}

					auto currentValue =
						jxx::CAST<jxx::lang::Object>(
							owner_->get(keyObject));

					auto expectedValue =
						jxx::CAST<jxx::lang::Object>(
							entry->getValue());

					return objectEquals(
						currentValue,
						expectedValue);
				}

				virtual jxx::Ptr<Iterator<MapEntry<K, V>>>
                    iterator() override
                {
                    auto iteratorValue =
                        jxx::NEW<EntryIterator>(owner_);

                    return jxx::CAST<Iterator<MapEntry<K, V>>>(
                        iteratorValue);
                }

				virtual jxx::lang::jbool add(
                    const jxx::Ptr<MapEntry<K, V>>& /*entry*/) override
				{

					throw jxx::lang::UnsupportedOperationException();
				}

				virtual jxx::lang::jbool remove(
                    const jxx::Ptr<jxx::lang::Object>& object) override
				{

					auto entry =
						jxx::CAST<MapEntry<K, V>>(object);

					if (entry == nullptr) {
						return static_cast<jxx::lang::jbool>(false);
					}

					auto keyObject =
						jxx::CAST<jxx::lang::Object>(
							entry->getKey());

					if (!contains(object)) {
						return static_cast<jxx::lang::jbool>(false);
					}

					owner_->remove(keyObject);
					return static_cast<jxx::lang::jbool>(true);
				}

				virtual void clear() override
				{
					owner_->clear();
				}

				/*
				 * containsAll/removeAll/retainAll/toArray are inherited from
				 * AbstractSet/AbstractCollection. Do not re-declare them here
				 * unless your exact base class leaves them pure virtual.
				 */
			};

			virtual jxx::Ptr<Set<MapEntry<K, V>>>
                createEntrySetView()
            {
                auto owner =
                    jxx::CAST<HashMap<K, V>>(this->thisPtr);

                if (owner == nullptr) {
                    throw jxx::lang::IllegalStateException();
                }

                auto view = jxx::NEW<EntrySet>(owner);
                return jxx::CAST<Set<MapEntry<K, V>>>(view);
            }

		public:
			virtual jxx::Ptr<Set<MapEntry<K, V>>>
				entrySet() override
			{

				if (entrySetView_ == nullptr) {
					entrySetView_ = createEntrySetView();
				}

				return entrySetView_;
			}

			virtual jxx::Ptr<jxx::lang::Object> clone() const override
            {
				auto cloned =
                    jxx::NEW<HashMap<K, V>>(
                        static_cast<jxx::lang::jint>(map_.size()),
                        loadFactor_);

				for (const auto& pair : map_) {
					cloned->map_.emplace(
						pair.first,
						pair.second);
				}

				cloned->modCount_ = 0;
				cloned->entrySetView_ = nullptr;

				return jxx::CAST<jxx::lang::Object>(cloned);
			}
		};

	} // namespace util
} // namespace jxx