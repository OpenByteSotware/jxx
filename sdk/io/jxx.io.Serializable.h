#pragma once

namespace jxx::io {
	class ObjectOutputStream;
	class ObjectInputStream;
    
    class Serializable {
    public:
	virtual ~Serializable() = default;

    virtual void writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out) = 0;
    virtual void readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) = 0;        
    virtual void readObjectNoData() = 0;
};
}