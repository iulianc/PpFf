
#ifndef KEYVALUE_HPP_
#define KEYVALUE_HPP_

#include <iostream>

template<typename K, typename V>
class KeyValue {
public:

    typedef K keytype;
    typedef V valuetype;

    KeyValue(){};
    KeyValue(K key_, V val_): key(key_), val(val_){};
    KeyValue(const KeyValue &kv): key(kv.key), val(kv.val){};
    KeyValue(KeyValue &&kv): key(std::move(kv.key)), val(std::move(kv.val)){};


    KeyValue& operator=(const KeyValue& kv){
        key = kv.key;
        val = kv.val;
        return *this;
    }

    KeyValue& operator=(KeyValue&& kv){
        key = std::move(kv.key);
        val = std::move(kv.val);
        return *this;
    }


    const K& Key() const{
        return key;
    }

    const V& Value() const{
        return val;
    }

    void Key(K key_){
        key = key_;
    }
    void Value(V val_){
        val = val_;
    }

    bool operator==(KeyValue &kv){
        return key == kv.Key() && val == kv.Value();
    }

    friend bool operator<(const KeyValue& l, const KeyValue& r){
        return l.Value() < r.Value();
    }

    friend std::ostream& operator<<(std::ostream& os, const KeyValue& kv){
        os << kv.to_string();
        return os;
    }

    KeyValue& operator+=(const KeyValue& rhs){
        val += rhs.val;
        return *this;
    }

    friend KeyValue operator+(KeyValue lhs, const KeyValue& rhs){
        lhs.val += rhs.val;
        return lhs;
    }

    void sumValue(const KeyValue &kv){
        val += kv.val;
    }

    bool sameKey(const KeyValue &kv) const{
        return key == kv.key;
    }

    std::string to_string() const{
        std::string value = "<";
        value.append(key).append(", ").append(std::to_string(val));
        value.append(">");
        return value;
    }

private:
    K key;
    V val;
};

#endif
