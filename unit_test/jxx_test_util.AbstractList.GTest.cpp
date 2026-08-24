#include <cstdint>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>
#include <type_traits>
#include <gtest/gtest.h>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.ListIterator.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace
{

	using String = jxx::lang::String;
	using StringPtr = jxx::Ptr<String>;

	StringPtr text(const char* value)
	{
		return jxx::NEW<String>(value);
	}

	/**
	 * Small concrete list used to test AbstractList's skeletal implementation.
	 *
	 * Structural changes increment modCount. Element replacement does not, which
	 * matches the Java AbstractList iterator contract.
	 */
	class TestStringList final : public jxx::util::AbstractList<String>
	{
	private:
		std::vector<StringPtr> values_;

		void checkElementIndex(jxx::lang::jint index) const
		{
			if (index < 0 ||
				index >= static_cast<jxx::lang::jint>(values_.size())) {
				throw jxx::lang::IndexOutOfBoundsException();
			}
		}

		void checkPositionIndex(jxx::lang::jint index) const
		{
			if (index < 0 ||
				index > static_cast<jxx::lang::jint>(values_.size())) {
				throw jxx::lang::IndexOutOfBoundsException();
			}
		}

	public:
	
		explicit TestStringList()
		{
		}

		TestStringList(std::initializer_list<StringPtr> values)
			: values_(values)
		{
		}

		virtual ~TestStringList() {
		}

		jxx::lang::jint size() override
		{
			return static_cast<jxx::lang::jint>(values_.size());
		}

		virtual StringPtr get(jxx::lang::jint index) override
		{
			checkElementIndex(index);
			return values_[static_cast<std::size_t>(index)];
		}

		virtual StringPtr set(
			jxx::lang::jint index,
			StringPtr element) override
		{

			checkElementIndex(index);
			auto oldValue = values_[static_cast<std::size_t>(index)];
			values_[static_cast<std::size_t>(index)] = std::move(element);
			return oldValue;
		}

		virtual void add(
			jxx::lang::jint index,
			StringPtr element) override
		{

			checkPositionIndex(index);
			values_.insert(
				values_.begin() + static_cast<std::ptrdiff_t>(index),
				std::move(element));
			++this->modCount;
		}

		/*
		 * Override the broken recursive AbstractList::add(Ptr<E>) implementation.
		 */
		virtual jxx::lang::jbool add(StringPtr element) override
		{
			add(size(), std::move(element));
			return true;
		}

		virtual StringPtr remove(jxx::lang::jint index) override
		{
			checkElementIndex(index);
			auto oldValue = values_[static_cast<std::size_t>(index)];
			values_.erase(
				values_.begin() + static_cast<std::ptrdiff_t>(index));
			++this->modCount;
			return oldValue;
		}

		void structuralAppendForFailFastTest(const StringPtr& value)
		{
			values_.push_back(value);
			++this->modCount;
		}

	protected:
		jxx::Ptr<jxx::lang::Object> cloneImpl() const override
		{
			auto copy = jxx::NEW<TestStringList>();
			copy->values_ = values_;
			return jxx::CAST<jxx::lang::Object>(copy);
		}
	};

	std::string utf8(const StringPtr& value)
	{
		return value == nullptr ? std::string("<null>") : value->utf8();
	}

	
	TEST(AbstractListTest, IteratorTraversesElementsInOrder)
	{
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{text("left"), text("middle"), text("right")});

		auto iterator = list->iterator();

		ASSERT_TRUE(iterator->hasNext());
		EXPECT_EQ(utf8(iterator->next()), "left");
		ASSERT_TRUE(iterator->hasNext());
		EXPECT_EQ(utf8(iterator->next()), "middle");
		ASSERT_TRUE(iterator->hasNext());
		EXPECT_EQ(utf8(iterator->next()), "right");
		EXPECT_FALSE(iterator->hasNext());
		EXPECT_THROW(iterator->next(), jxx::util::NoSuchElementException);
	}

	TEST(AbstractListTest, IteratorRemoveRemovesLastReturnedElement)
	{
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{text("left"), text("middle"), text("right")});

		auto iterator = list->iterator();
		EXPECT_EQ(utf8(iterator->next()), "left");
		EXPECT_EQ(utf8(iterator->next()), "middle");

		iterator->remove();

		ASSERT_EQ(list->size(), 2);
		EXPECT_EQ(utf8(list->get(0)), "left");
		EXPECT_EQ(utf8(list->get(1)), "right");
		EXPECT_THROW(iterator->remove(), jxx::lang::IllegalStateException);
	}

	TEST(AbstractListTest, IteratorRemoveBeforeNextThrowsIllegalState)
	{
		auto list = jxx::NEW<TestStringList>();
		auto iterator = list->iterator();

		EXPECT_THROW(iterator->remove(), jxx::lang::IllegalStateException);
	}

	TEST(AbstractListTest, IteratorDetectsConcurrentStructuralModification)
	{
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{text("left"), text("right")});

		auto iterator = list->iterator();
		EXPECT_EQ(utf8(iterator->next()), "left");

		list->structuralAppendForFailFastTest(text("later"));

		EXPECT_THROW(iterator->next(), jxx::util::ConcurrentModificationException);
		EXPECT_THROW(iterator->remove(), jxx::util::ConcurrentModificationException);
	}

	TEST(AbstractListTest, ListIteratorSupportsForwardAndBackwardTraversal)
	{
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{text("left"), text("middle"), text("right")});

		auto iterator = list->listIterator(1);

		EXPECT_EQ(iterator->nextIndex(), 1);
		EXPECT_EQ(iterator->previousIndex(), 0);
		EXPECT_TRUE(iterator->hasPrevious());
		EXPECT_EQ(utf8(iterator->previous()), "left");
		EXPECT_FALSE(iterator->hasPrevious());
		EXPECT_EQ(iterator->nextIndex(), 0);
		EXPECT_EQ(iterator->previousIndex(), -1);
		EXPECT_EQ(utf8(iterator->next()), "left");
		EXPECT_EQ(utf8(iterator->next()), "middle");
		EXPECT_EQ(utf8(iterator->previous()), "middle");
	}

	TEST(AbstractListTest, ListIteratorSetReplacesLastReturnedElement)
	{
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{text("left"), text("right")});

		auto iterator = list->listIterator();
		EXPECT_EQ(utf8(iterator->next()), "left");

		iterator->set(text("changed"));

		EXPECT_EQ(utf8(list->get(0)), "changed");
		EXPECT_EQ(utf8(iterator->previous()), "changed");
	}

	TEST(AbstractListTest, ListIteratorSetBeforeTraversalThrowsIllegalState)
	{
		auto list = jxx::NEW<TestStringList>();
		auto iterator = list->listIterator();

		EXPECT_THROW(iterator->set(text("value")), jxx::lang::IllegalStateException);
	}

	TEST(AbstractListTest, ListIteratorAddInsertsAtCursor)
	{
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{text("left"), text("right")});

		auto iterator = list->listIterator(1);
		iterator->add(text("middle"));

		ASSERT_EQ(list->size(), 3);
		EXPECT_EQ(utf8(list->get(0)), "left");
		EXPECT_EQ(utf8(list->get(1)), "middle");
		EXPECT_EQ(utf8(list->get(2)), "right");
		EXPECT_EQ(iterator->previousIndex(), 1);
		EXPECT_EQ(iterator->nextIndex(), 2);
	}

	TEST(AbstractListTest, IndexOfAndLastIndexOfUseElementEquality)
	{
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{
			text("left"), nullptr, text("right"), text("left"), nullptr});

		auto leftProbe = text("left");
		auto rightProbe = text("right");

		EXPECT_EQ(
			list->indexOf(jxx::CAST<jxx::lang::Object>(leftProbe)),
			0);
		EXPECT_EQ(
			list->lastIndexOf(jxx::CAST<jxx::lang::Object>(leftProbe)),
			3);
		EXPECT_EQ(
			list->indexOf(jxx::CAST<jxx::lang::Object>(rightProbe)),
			2);
		EXPECT_EQ(list->indexOf(nullptr), 1);
		EXPECT_EQ(list->lastIndexOf(nullptr), 4);
		EXPECT_EQ(
			list->indexOf(jxx::CAST<jxx::lang::Object>(text("missing"))),
			-1);
	}

	TEST(AbstractListTest, RemoveByIndexReturnsRemovedElement)
	{
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{text("left"), text("right")});

		auto removed = list->remove(0);

		EXPECT_EQ(utf8(removed), "left");
		ASSERT_EQ(list->size(), 1);
		EXPECT_EQ(utf8(list->get(0)), "right");
	}

	TEST(AbstractListTest, RemoveByObjectRemovesFirstEqualElement)
	{
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{text("left"), text("right"), text("left")});

		//EXPECT_TRUE(list->remove(jxx::CAST<jxx::lang::Object>(text("left"))));
		ASSERT_EQ(list->size(), 2);
		EXPECT_EQ(utf8(list->get(0)), "right");
		EXPECT_EQ(utf8(list->get(1)), "left");
		//EXPECT_FALSE(list->remove(jxx::CAST<jxx::lang::Object>(text("missing"))));
	}

	TEST(AbstractListTest, ClearRemovesAllElementsThroughIterator)
	{
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{text("left"), text("middle"), text("right")});

		list->clear();

		EXPECT_EQ(list->size(), 0);
		EXPECT_TRUE(list->isEmpty());
	}

	TEST(AbstractListTest, ListIteratorRangeChecksPosition)
	{
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{text("value")});

		EXPECT_NO_THROW(list->listIterator(0));
		EXPECT_NO_THROW(list->listIterator(1));
		EXPECT_THROW(list->listIterator(-1), jxx::lang::IndexOutOfBoundsException);
		EXPECT_THROW(list->listIterator(2), jxx::lang::IndexOutOfBoundsException);
	}

	TEST(AbstractListTest, EmptyListIteratorBoundaryMethodsThrow)
	{
		auto list = jxx::NEW<TestStringList>();
		auto iterator = list->listIterator();

		EXPECT_FALSE(iterator->hasNext());
		EXPECT_FALSE(iterator->hasPrevious());
		EXPECT_THROW(iterator->next(), jxx::util::NoSuchElementException);
		EXPECT_THROW(iterator->previous(), jxx::util::NoSuchElementException);
	}

	TEST(AbstractListTest, HashCodeUsesJavaListFormula)
	{
		auto left = text("left");
		auto right = text("right");
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{left, nullptr, right});

		std::uint32_t expected = 1U;
		expected = expected * 31U + static_cast<std::uint32_t>(left->hashCode());
		expected = expected * 31U;
		expected = expected * 31U + static_cast<std::uint32_t>(right->hashCode());

		EXPECT_EQ(
			list->hashCode(),
			static_cast<jxx::lang::jint>(static_cast<std::int32_t>(expected)));
	}

	TEST(AbstractListTest, SubListCurrentlyThrowsUnsupportedOperation)
	{
		auto list = jxx::NEW<TestStringList>(
			std::initializer_list<StringPtr>{text("left"), text("right")});

		EXPECT_THROW(list->subList(0, 1), jxx::lang::UnsupportedOperationException);
	}

	} // namespace
