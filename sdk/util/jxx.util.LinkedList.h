#pragma once
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.AbstractSequentialList.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.Deque.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.SubList.h"
namespace jxx::util {
template <typename E>
class LinkedList : public jxx::lang::ClassBase<LinkedList<E>, AbstractSequentialList<E>, Deque<E>, jxx::lang::Cloneable, jxx::io::SerializableI> {
    struct Node { jxx::Ptr<E> item; Node* prev=nullptr; Node* next=nullptr; explicit Node(const jxx::Ptr<E>& e):item(e){} };
    Node* first_=nullptr; Node* last_=nullptr; jxx::lang::jint size_=0;
    Node* node_(jxx::lang::jint i) const { if(i<0||i>=size_) throw jxx::lang::IndexOutOfBoundsException(); Node* x; if(i<(size_>>1)){x=first_;while(i--)x=x->next;}else{x=last_;for(auto n=size_-1;n>i;--n)x=x->prev;} return x; }
    void linkFirst_(const jxx::Ptr<E>& e){auto n=new Node(e);n->next=first_;if(first_)first_->prev=n;else last_=n;first_=n;++size_;++this->modCount;}
    void linkLast_(const jxx::Ptr<E>& e){auto n=new Node(e);n->prev=last_;if(last_)last_->next=n;else first_=n;last_=n;++size_;++this->modCount;}
    void linkBefore_(const jxx::Ptr<E>& e,Node* s){auto n=new Node(e);n->next=s;n->prev=s->prev;if(s->prev)s->prev->next=n;else first_=n;s->prev=n;++size_;++this->modCount;}
    jxx::Ptr<E> unlink_(Node* x){auto v=x->item;if(x->prev)x->prev->next=x->next;else first_=x->next;if(x->next)x->next->prev=x->prev;else last_=x->prev;delete x;--size_;++this->modCount;return v;}
    bool eq_(const jxx::Ptr<jxx::lang::Object>& a,const jxx::Ptr<jxx::lang::Object>& b) const{return a==nullptr?b==nullptr:a->equals(b);}
    class ListItr final : public jxx::lang::ClassBase<ListItr,jxx::lang::Object,ListIterator<E>> {
        jxx::Ptr<LinkedList<E>> owner_; Node* next_; Node* last_=nullptr; jxx::lang::jint index_; jxx::lang::jint expected_;
        void check_(){if(expected_!=owner_->modCount)throw ConcurrentModificationException();}
    public:
        ListItr(const jxx::Ptr<LinkedList<E>>& o,jxx::lang::jint i):owner_(o),next_(i==o->size_?nullptr:o->node_(i)),index_(i),expected_(o->modCount){if(i<0||i>o->size_)throw jxx::lang::IndexOutOfBoundsException();}
        jxx::lang::jbool hasNext() override{return index_<owner_->size_;}
        jxx::Ptr<E> next() override{check_();if(!hasNext())throw NoSuchElementException();last_=next_;next_=next_->next;++index_;return last_->item;}
        jxx::lang::jbool hasPrevious() override{return index_>0;}
        jxx::Ptr<E> previous() override{check_();if(!hasPrevious())throw NoSuchElementException();next_=next_?next_->prev:owner_->last_;last_=next_;--index_;return last_->item;}
        jxx::lang::jint nextIndex() override{return index_;} jxx::lang::jint previousIndex() override{return index_-1;}
        void remove() override{check_();if(!last_)throw jxx::lang::IllegalStateException();auto n=last_->next;if(next_==last_)next_=n;else --index_;owner_->unlink_(last_);last_=nullptr;expected_=owner_->modCount;}
        void set(const jxx::Ptr<E>& e) override{check_();if(!last_)throw jxx::lang::IllegalStateException();last_->item=e;}
        void add(const jxx::Ptr<E>& e) override{check_();if(next_)owner_->linkBefore_(e,next_);else owner_->linkLast_(e);++index_;last_=nullptr;expected_=owner_->modCount;}
    };
    class DescItr final : public jxx::lang::ClassBase<DescItr,jxx::lang::Object,Iterator<E>> { jxx::Ptr<ListIterator<E>> it_; public: explicit DescItr(const jxx::Ptr<ListIterator<E>>& i):it_(i){} jxx::lang::jbool hasNext() override{return it_->hasPrevious();} jxx::Ptr<E> next() override{return it_->previous();} void remove() override{it_->remove();}};
public:
    LinkedList()=default;
    explicit LinkedList(const jxx::Ptr<wildcard::CollectionExtends<E>>& c){if(!c)throw jxx::lang::NullPointerException();auto it=c->iteratorExtends();while(it->hasNext())linkLast_(it->next());}
    ~LinkedList() override{Node*x=first_;while(x){auto n=x->next;delete x;x=n;}}
    jxx::lang::jint size() override{return size_;} jxx::lang::jbool isEmpty() override{return size_==0;}
    jxx::Ptr<Iterator<E>> iterator() override {
        return jxx::CAST<Iterator<E>>(this->listIterator(0));
    }

    jxx::Ptr<ListIterator<E>> listIterator() override {
        return this->listIterator(0);
    }

    jxx::Ptr<ListIterator<E>> listIterator(
        jxx::lang::jint index) override {

        if (index < 0 || index > size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }

        auto self =
            jxx::CAST<LinkedList<E>>(this->thisPtr());

        if (self == nullptr) {
            throw jxx::lang::IllegalStateException();
        }

        auto iteratorValue =
            jxx::NEW<ListItr>(self, index);

        return jxx::CAST<ListIterator<E>>(iteratorValue);
    }
    jxx::Ptr<E> get(jxx::lang::jint i) const override{return node_(i)->item;}
    jxx::Ptr<E> set(jxx::lang::jint i,const jxx::Ptr<E>& e) override{auto n=node_(i);auto old=n->item;n->item=e;return old;}
    void add(jxx::lang::jint i,const jxx::Ptr<E>& e) override{if(i<0||i>size_)throw jxx::lang::IndexOutOfBoundsException();if(i==size_)linkLast_(e);else linkBefore_(e,node_(i));}
    jxx::Ptr<E> remove(jxx::lang::jint i) override{return unlink_(node_(i));}
    jxx::lang::jbool add(const jxx::Ptr<E>& e) override{linkLast_(e);return true;}
    jxx::lang::jbool contains(const jxx::Ptr<jxx::lang::Object>& o) override{return indexOf(o)>=0;}
    jxx::lang::jbool remove(const jxx::Ptr<jxx::lang::Object>& o) override{for(auto n=first_;n;n=n->next)if(eq_(o,jxx::CAST<jxx::lang::Object>(n->item))){unlink_(n);return true;}return false;}
    jxx::lang::jint indexOf(const jxx::Ptr<jxx::lang::Object>& o) override{jxx::lang::jint i=0;for(auto n=first_;n;n=n->next,++i)if(eq_(o,jxx::CAST<jxx::lang::Object>(n->item)))return i;return -1;}
    jxx::lang::jint lastIndexOf(const jxx::Ptr<jxx::lang::Object>& o) override{jxx::lang::jint i=size_-1;for(auto n=last_;n;n=n->prev,--i)if(eq_(o,jxx::CAST<jxx::lang::Object>(n->item)))return i;return -1;}
    jxx::lang::jbool addAll(const jxx::Ptr<wildcard::CollectionExtends<E>>& c) override{return addAll(size_,c);}
    jxx::lang::jbool addAll(jxx::lang::jint i,const jxx::Ptr<wildcard::CollectionExtends<E>>& c) override{if(!c)throw jxx::lang::NullPointerException();if(i<0||i>size_)throw jxx::lang::IndexOutOfBoundsException();bool changed=false;auto it=c->iteratorExtends();while(it->hasNext()){add(i++,it->next());changed=true;}return changed;}
    void clear() override{while(first_)unlink_(first_);}
    void addFirst(const jxx::Ptr<E>& e) override{linkFirst_(e);} void addLast(const jxx::Ptr<E>& e) override{linkLast_(e);}
    jxx::lang::jbool offerFirst(const jxx::Ptr<E>& e) override{addFirst(e);return true;} jxx::lang::jbool offerLast(const jxx::Ptr<E>& e) override{addLast(e);return true;} jxx::lang::jbool offer(const jxx::Ptr<E>& e) override{return offerLast(e);}
    jxx::Ptr<E> removeFirst() override{if(!first_)throw NoSuchElementException();return unlink_(first_);} jxx::Ptr<E> removeLast() override{if(!last_)throw NoSuchElementException();return unlink_(last_);} jxx::Ptr<E> pollFirst() override{return first_?unlink_(first_):nullptr;} jxx::Ptr<E> pollLast() override{return last_?unlink_(last_):nullptr;}
    jxx::Ptr<E> getFirst() override{if(!first_)throw NoSuchElementException();return first_->item;} jxx::Ptr<E> getLast() override{if(!last_)throw NoSuchElementException();return last_->item;} jxx::Ptr<E> peekFirst() override{return first_?first_->item:nullptr;} jxx::Ptr<E> peekLast() override{return last_?last_->item:nullptr;}
    jxx::Ptr<E> remove() override{return removeFirst();} jxx::Ptr<E> poll() override{return pollFirst();} jxx::Ptr<E> element() override{return getFirst();} jxx::Ptr<E> peek() override{return peekFirst();}
    jxx::lang::jbool removeFirstOccurrence(const jxx::Ptr<jxx::lang::Object>& o) override{return remove(o);} jxx::lang::jbool removeLastOccurrence(const jxx::Ptr<jxx::lang::Object>& o) override{for(auto n=last_;n;n=n->prev)if(eq_(o,jxx::CAST<jxx::lang::Object>(n->item))){unlink_(n);return true;}return false;}
    void push(const jxx::Ptr<E>& e) override{addFirst(e);} jxx::Ptr<E> pop() override{return removeFirst();}
    jxx::Ptr<Iterator<E>> descendingIterator() override{return jxx::CAST<Iterator<E>>(jxx::NEW<DescItr>(this->listIterator(size_)));}
    jxx::Ptr<List<E>> subList(
        jxx::lang::jint fromIndex,
        jxx::lang::jint toIndex) override {

        if (fromIndex < 0 || toIndex < fromIndex || toIndex > size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }

        auto self = jxx::CAST<LinkedList<E>>(this->thisPtr());
        if (self == nullptr) {
            throw jxx::lang::IllegalStateException();
        }

        auto root = jxx::CAST<List<E>>(self);
        auto view = jxx::NEW<SubList<E>>(root, fromIndex, toIndex);
        return jxx::CAST<List<E>>(view);
    }
    jxx::lang::ObjectArray toArray() override{auto a=jxx::NEW<jxx::lang::ObjectArrayType>(static_cast<std::uint32_t>(size_));jxx::lang::jint i=0;for(auto n=first_;n;n=n->next)(*a)[i++]=jxx::CAST<jxx::lang::Object>(n->item);return a;}
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override{auto r=jxx::NEW<LinkedList<E>>();for(auto n=first_;n;n=n->next)r->add(n->item);return jxx::CAST<jxx::lang::Object>(r);}
    void writeObject(const jxx::Ptr<jxx::io::ObjectOutputStream>& out) override{if(!out)throw jxx::lang::NullPointerException();throw jxx::lang::UnsupportedOperationException();}
    void readObject(const jxx::Ptr<jxx::io::ObjectInputStream>& in) override{if(!in)throw jxx::lang::NullPointerException();throw jxx::lang::UnsupportedOperationException();}
    void readObjectNoData() override{throw jxx::lang::UnsupportedOperationException();}
};
} // namespace jxx::util
