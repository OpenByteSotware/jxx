#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <pugixml.hpp>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.PugiDom.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Attr.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementation.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NamedNodeMap.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Text.h"

namespace jxx::org::w3c::dom::internal
{
	using jxx::lang::String;

	struct Store
	{
		pugi::xml_document document;
	};
	class DomNode;
	jxx::Ptr<Node> wrap(const std::shared_ptr<Store>& store, pugi::xml_node node);
	class NodeListImpl final : public jxx::lang::ClassBase<NodeListImpl, jxx::lang::Object, NodeList>
	{
	public: NodeListImpl(std::shared_ptr<Store> s, std::vector<pugi::xml_node> n) :store_(std::move(s)), nodes_(std::move(n))
	{
	} jxx::Ptr<Node> item(jxx::lang::jint i)const override
	{
		return i < 0 || static_cast<std::size_t>(i) >= nodes_.size() ? nullptr : wrap(store_, nodes_[static_cast<std::size_t>(i)]);
	} jxx::lang::jint getLength()const override
	{
		return static_cast<jxx::lang::jint>(nodes_.size());
	} private: std::shared_ptr<Store> store_; std::vector<pugi::xml_node> nodes_;
	};
	class DomNode final : public jxx::lang::ClassBase<DomNode, jxx::lang::Object, Document, Element, Attr, Text>
	{
	public: DomNode(std::shared_ptr<Store> s, pugi::xml_node n) :store_(std::move(s)), node_(n)
	{
	} jxx::Ptr<String> getNodeName()const override
	{
		return jxx::NEW<String>(node_.name());
	} jxx::Ptr<String> getNodeValue()const override
	{
		return jxx::NEW<String>(node_.value());
	} void setNodeValue(const jxx::Ptr<String>& v)override
	{
		node_.set_value(v ? v->utf8().c_str() : "");
	} jxx::lang::jshort getNodeType()const override
	{
		switch (node_.type()) {
			case pugi::node_document:return Node::DOCUMENT_NODE; case pugi::node_element:return Node::ELEMENT_NODE; case pugi::node_pcdata:return Node::TEXT_NODE; case pugi::node_cdata:return Node::CDATA_SECTION_NODE; case pugi::node_comment:return Node::COMMENT_NODE; case pugi::node_pi:return Node::PROCESSING_INSTRUCTION_NODE; default:return 0;
		}
	} jxx::Ptr<Node> getParentNode()const override
	{
		return wrap(store_, node_.parent());
	} jxx::Ptr<NodeList> getChildNodes()const override
	{
		std::vector<pugi::xml_node> v; for (auto n : node_.children())v.push_back(n); return jxx::NEW<NodeListImpl>(store_, std::move(v));
	} jxx::Ptr<Node> getFirstChild()const override
	{
		return wrap(store_, node_.first_child());
	} jxx::Ptr<Node> getLastChild()const override
	{
		return wrap(store_, node_.last_child());
	} jxx::Ptr<Node> getPreviousSibling()const override
	{
		return wrap(store_, node_.previous_sibling());
	} jxx::Ptr<Node> getNextSibling()const override
	{
		return wrap(store_, node_.next_sibling());
	} jxx::Ptr<NamedNodeMap> getAttributes()const override; jxx::Ptr<Document> getOwnerDocument()const override
	{
		return jxx::CAST<Document>(wrap(store_, store_->document));
	} jxx::Ptr<Node> insertBefore(const jxx::Ptr<Node>& c, const jxx::Ptr<Node>& r)override; jxx::Ptr<Node> replaceChild(const jxx::Ptr<Node>& c, const jxx::Ptr<Node>& o)override; jxx::Ptr<Node> removeChild(const jxx::Ptr<Node>& c)override; jxx::Ptr<Node> appendChild(const jxx::Ptr<Node>& c)override; jxx::lang::jbool hasChildNodes()const override
	{
		return node_.first_child();
	} jxx::Ptr<Node> cloneNode(jxx::lang::jbool deep)const override
	{
		auto d = newDocument(); auto x = jxx::CAST<DomNode>(d); auto copied = x->node_.append_copy(node_); if (!deep)while (copied.first_child())copied.remove_child(copied.first_child()); return wrap(x->store_, copied);
	} void normalize()override
	{
	} jxx::lang::jbool hasAttributes()const override
	{
		return node_.first_attribute();
	} jxx::Ptr<String> getNamespaceURI()const override
	{
		return nullptr;
	} jxx::Ptr<String> getPrefix()const override
	{
		std::string n = node_.name(); auto p = n.find(':'); return p == std::string::npos ? nullptr : jxx::NEW<String>(n.substr(0, p));
	} void setPrefix(const jxx::Ptr<String>&)override
	{
		throw DOMException(DOMException::NOT_SUPPORTED_ERR, jxx::NEW<String>("Prefix mutation is not supported"));
	} jxx::Ptr<String> getLocalName()const override
	{
		std::string n = node_.name(); auto p = n.find(':'); return jxx::NEW<String>(p == std::string::npos ? n : n.substr(p + 1));
	} jxx::Ptr<String> getTextContent()const override
	{
		return jxx::NEW<String>(node_.text().get());
	} void setTextContent(const jxx::Ptr<String>& t)override
	{
		node_.text().set(t ? t->utf8().c_str() : "");
	} jxx::lang::jbool isSameNode(const jxx::Ptr<Node>& o)const override
	{
		auto x = jxx::CAST<DomNode>(o); return x && x->store_ == store_ && x->node_ == node_;
	}
	jxx::Ptr<Element> getDocumentElement()const override
	{
		return jxx::CAST<Element>(wrap(store_, node_.document_element()));
	} jxx::Ptr<Element> createElement(const jxx::Ptr<String>& n)override
	{
		auto x = store_->document.append_child(n->utf8().c_str()); return jxx::CAST<Element>(wrap(store_, x));
	} jxx::Ptr<Text> createTextNode(const jxx::Ptr<String>& d)override
	{
		auto x = store_->document.append_child(pugi::node_pcdata); x.set_value(d->utf8().c_str()); return jxx::CAST<Text>(wrap(store_, x));
	} jxx::Ptr<Attr> createAttribute(const jxx::Ptr<String>&)override
	{
		return nullptr;
	} jxx::Ptr<NodeList> getElementsByTagName(const jxx::Ptr<String>& n)const override
	{
		std::vector<pugi::xml_node> v; std::string q = n->utf8(); std::function<void(pugi::xml_node)> scan = [&](pugi::xml_node x)
			{
				for (auto c : x.children()) {
					if (q == "*" || q == c.name())v.push_back(c); scan(c);
				}
			}; scan(node_); return jxx::NEW<NodeListImpl>(store_, std::move(v));
	} jxx::Ptr<DOMImplementation> getImplementation()const override
	{
		return implementation();
	}
	jxx::Ptr<String> getTagName()const override
	{
		return getNodeName();
	} jxx::Ptr<String> getAttribute(const jxx::Ptr<String>& n)const override
	{
		return jxx::NEW<String>(node_.attribute(n->utf8().c_str()).value());
	} void setAttribute(const jxx::Ptr<String>& n, const jxx::Ptr<String>& v)override
	{
		auto a = node_.attribute(n->utf8().c_str()); if (!a)a = node_.append_attribute(n->utf8().c_str()); a.set_value(v->utf8().c_str());
	} void removeAttribute(const jxx::Ptr<String>& n)override
	{
		node_.remove_attribute(n->utf8().c_str());
	} jxx::lang::jbool hasAttribute(const jxx::Ptr<String>& n)const override
	{
		return node_.attribute(n->utf8().c_str());
	}
	jxx::Ptr<String> getName()const override
	{
		return getNodeName();
	} jxx::Ptr<String> getValue()const override
	{
		return getNodeValue();
	} void setValue(const jxx::Ptr<String>& v)override
	{
		setNodeValue(v);
	} jxx::Ptr<String> getData()const override
	{
		return getNodeValue();
	} void setData(const jxx::Ptr<String>& v)override
	{
		setNodeValue(v);
	} std::shared_ptr<Store> store_; pugi::xml_node node_;
	};
	class NamedNodeMapImpl final : public jxx::lang::ClassBase<NamedNodeMapImpl, jxx::lang::Object, NamedNodeMap>
	{
	public: NamedNodeMapImpl(std::shared_ptr<Store>s, pugi::xml_node n) :store_(std::move(s)), node_(n)
	{
	} jxx::Ptr<Node> getNamedItem(const jxx::Ptr<String>&)const override
	{
		return nullptr;
	} jxx::Ptr<Node> setNamedItem(const jxx::Ptr<Node>&)override
	{
		throw DOMException(DOMException::NOT_SUPPORTED_ERR, jxx::NEW<String>("Attribute nodes are not mutable in Release A"));
	} jxx::Ptr<Node> removeNamedItem(const jxx::Ptr<String>& n)override
	{
		node_.remove_attribute(n->utf8().c_str()); return nullptr;
	} jxx::Ptr<Node> item(jxx::lang::jint)const override
	{
		return nullptr;
	} jxx::lang::jint getLength()const override
	{
		jxx::lang::jint n = 0; for (auto a : node_.attributes()) {
			(void)a; ++n;
		}return n;
	} private:std::shared_ptr<Store>store_; pugi::xml_node node_;
	};
	jxx::Ptr<NamedNodeMap> DomNode::getAttributes()const
	{
		return jxx::NEW<NamedNodeMapImpl>(store_, node_);
	} jxx::Ptr<Node> DomNode::appendChild(const jxx::Ptr<Node>& c)
	{
		auto x = jxx::CAST<DomNode>(c); if (!x)throw DOMException(DOMException::WRONG_DOCUMENT_ERR, jxx::NEW<String>("Unsupported node")); auto n = node_.append_copy(x->node_); return wrap(store_, n);
	} jxx::Ptr<Node> DomNode::insertBefore(const jxx::Ptr<Node>& c, const jxx::Ptr<Node>& r)
	{
		auto x = jxx::CAST<DomNode>(c), y = jxx::CAST<DomNode>(r); auto n = node_.insert_copy_before(x->node_, y->node_); return wrap(store_, n);
	} jxx::Ptr<Node> DomNode::replaceChild(const jxx::Ptr<Node>& c, const jxx::Ptr<Node>& o)
	{
		auto x = insertBefore(c, o); removeChild(o); return x;
	} jxx::Ptr<Node> DomNode::removeChild(const jxx::Ptr<Node>& c)
	{
		auto x = jxx::CAST<DomNode>(c); if (!node_.remove_child(x->node_))throw DOMException(DOMException::NOT_FOUND_ERR, jxx::NEW<String>("Child not found")); return c;
	}
	jxx::Ptr<Node> wrap(const std::shared_ptr<Store>& s, pugi::xml_node n)
	{
		return n ? jxx::CAST<Node>(jxx::NEW<DomNode>(s, n)) : nullptr;
	}
	class DomImplementationImpl final : public jxx::lang::ClassBase<DomImplementationImpl, jxx::lang::Object, DOMImplementation>
	{
	public:jxx::lang::jbool hasFeature(const jxx::Ptr<String>& f, const jxx::Ptr<String>&)const override
	{
		if (!f)return false; auto x = f->utf8(); return x == "Core" || x == "XML";
	} jxx::Ptr<Document> createDocument(const jxx::Ptr<String>&, const jxx::Ptr<String>& q)override
	{
		auto d = newDocument(); if (q && !q->utf8().empty())d->createElement(q); return d;
	}
	};
}
jxx::Ptr<Document> parseDocument(const std::string& xml, jxx::lang::jbool ignoreComments, jxx::lang::jbool coalescing, std::string& error, jxx::lang::jint& offset)
{
	auto s = std::make_shared<Store>(); unsigned int flags = pugi::parse_default; if (!ignoreComments)flags |= pugi::parse_comments; if (!coalescing)flags |= pugi::parse_cdata; auto r = s->document.load_buffer(xml.data(), xml.size(), flags, pugi::encoding_auto); if (!r) {
		error = r.description(); offset = static_cast<jxx::lang::jint>(r.offset); return nullptr;
	}return jxx::CAST<Document>(wrap(s, s->document));
}
jxx::Ptr<Document> loadDocument(const std::string& path, jxx::lang::jbool ignoreComments, jxx::lang::jbool coalescing, std::string& error, jxx::lang::jint& offset)
{
	auto s = std::make_shared<Store>(); unsigned int flags = pugi::parse_default; if (!ignoreComments)flags |= pugi::parse_comments; if (!coalescing)flags |= pugi::parse_cdata; auto r = s->document.load_file(path.c_str(), flags, pugi::encoding_auto); if (!r) {
		error = r.description(); offset = static_cast<jxx::lang::jint>(r.offset); return nullptr;
	}return jxx::CAST<Document>(wrap(s, s->document));
}
jxx::Ptr<Document> newDocument()
{
	auto s = std::make_shared<Store>(); return jxx::CAST<Document>(wrap(s, s->document));
}
jxx::Ptr<DOMImplementation> implementation()
{
	static auto i = jxx::CAST<DOMImplementation>(jxx::NEW<DomImplementationImpl>()); return i;
}
