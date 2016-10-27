#include <cstdlib>
#include <fstream>
#include <sstream>
#include "jsmn.h"
#include "jsmn.hpp"

namespace jsmn {

// maximum token size
const int MAX_TOKEN_SIZE = 1000;

Object::Object() {
}
Object::Object(const Object& other) {
  object_ = other.object_;
}
Object& Object::operator=(const Object& other) {
  if (this == &other) return *this;
  object_ = other.object_;
  return *this;
}
Object::~Object() {
}

Value& Object::operator[](const string& key) {
  return object_[key];
}
const Value& Object::operator[](const string& key) const {
  type::const_iterator it = object_.find(key);
  if (it == object_.end()) throw Error("Object key not exists!");
  return it->second;
}

Object::type::iterator Object::begin() {
  return object_.begin();
}
Object::type::iterator Object::end() {
  return object_.end();
}
Object::type::const_iterator Object::begin() const {
  return object_.begin();
}
Object::type::const_iterator Object::end() const {
  return object_.end();
}

Array::Array() {
}
Array::Array(const Array& other) {
  array_ = other.array_;
}
Array& Array::operator=(const Array& other) {
  if (this == &other) return *this;
  array_ = other.array_;
  return *this;
}
Array::~Array() {
}

Value& Array::operator[](int index) {
  if (index >= array_.size()) throw Error("Index out of range!");
  return array_[index];
}
const Value& Array::operator[](int index) const {
  if (index >= array_.size()) throw Error("Index out of range!");
  return array_[index];
}

Array::type::iterator Array::begin() {
  return array_.begin();
}
Array::type::iterator Array::end() {
  return array_.end();
}
Array::type::const_iterator Array::begin() const {
  return array_.begin();
}
Array::type::const_iterator Array::end() const {
  return array_.end();
}

void Array::Push(const Value& value) {
  array_.push_back(value);
}

Value::Value()
  : type_(NIL) {
}
Value::Value(const Object& o)
  : type_(OBJECT), o_(o) {
}
Value::Value(const Array& a)
  : type_(ARRAY), a_(a) {
}
Value::Value(const String& s)
  : type_(STRING), s_(s) {
}
Value::Value(const char* s)
  : type_(STRING), s_(s) {
}
Value::Value(const Number& num)
  : type_(NUMBER), num_(num) {
}
Value::Value(const int num)
  : type_(NUMBER), num_(num) {
}
Value::Value(const Boolean& b)
  : type_(BOOLEAN), b_(b) {
}
Value::Value(const Value& other)
  : type_(other.type_) {
  switch (type_) {
  case jsmn::Value::BOOLEAN:
    b_ = other.b_; break;
  case jsmn::Value::NUMBER:
    num_ = other.num_; break;
  case jsmn::Value::STRING:
    s_ = other.s_;  break;
  case jsmn::Value::ARRAY:
    a_ = other.a_;  break;
  case jsmn::Value::OBJECT:
    o_ = other.o_; break;
  case jsmn::Value::NIL:
  default:
    break;
  }
}
Value& Value::operator=(const Value& other) {
  if (this == &other) return *this;
  type_ = other.type_;
  switch (type_) {
  case jsmn::Value::BOOLEAN:
    b_ = other.b_; break;
  case jsmn::Value::NUMBER:
    num_ = other.num_; break;
  case jsmn::Value::STRING:
    s_ = other.s_;  break;
  case jsmn::Value::ARRAY:
    a_ = other.a_;  break;
  case jsmn::Value::OBJECT:
    o_ = other.o_; break;
  case jsmn::Value::NIL:
  default:
    break;
  }
  return *this;
}
Value::~Value() {
}

// unwrappers

template<>
Number& Value::unwrap<Number>() {
  if (type_ != NUMBER) throw Error("Type is not Number!");
  return num_;
}
template<>
const Number& Value::unwrap<Number>() const {
  if (type_ != NUMBER) throw Error("Type is not Number!");
  return num_;
}

template<>
Boolean& Value::unwrap<Boolean>() {
  if (type_ != BOOLEAN) throw Error("Type is not Boolean!");
  return b_;
}
template<>
const Boolean& Value::unwrap<Boolean>() const {
  if (type_ != BOOLEAN) throw Error("Type is not Boolean!");
  return b_;
}

template<>
String& Value::unwrap<String>() {
  if (type_ != STRING) throw Error("Type is not String!");
  return s_;
}
template<>
const String& Value::unwrap<String>() const {
  if (type_ != STRING) throw Error("Type is not String!");
  return s_;
}

template<>
Object& Value::unwrap<Object>() {
  if (type_ != OBJECT) throw Error("Type is not Object!");
  return o_;
}
template<>
const Object& Value::unwrap<Object>() const {
  if (type_ != OBJECT) throw Error("Type is not Object!");
  return o_;
}

template<>
Array& Value::unwrap<Array>() {
  if (type_ != ARRAY) throw Error("Type is not Array!");
  return a_;
}
template<>
const Array& Value::unwrap<Array>() const {
  if (type_ != ARRAY) throw Error("Type is not Array!");
  return a_;
}

Value& Value::operator[](const string& key) {
  if (type_ != OBJECT) throw Error("Value type is not Object");
  return o_[key];
}
const Value& Value::operator[](const string& key) const {
  if (type_ != OBJECT) throw Error("Value type is not Object");
  return o_[key];
}

Value& Value::operator[](int index) {
  if (type_ != ARRAY) throw Error("Value type is not Array");
  return a_[index];
}
const Value& Value::operator[](int index) const {
  if (type_ != ARRAY) throw Error("Value type is not Array");
  return a_[index];
}

Object parse(const char* file) {
  std::ifstream f(file);
  if (!f.is_open()) throw Error("File not exists!");

  std::stringstream buff;
  buff << f.rdbuf();
  Object o = parse(buff.str());
  return o;
}

static Value parse(const char* js, const jsmntok_t* token, int& count) {
  if (token->type == JSMN_STRING) {
    string s(js, token->start, token->end - token->start);
    count = 1;
    return Value(s);
  }

  if (token->type == JSMN_PRIMITIVE) {
    count = 1;
    if (js[token->start] == 't') {
      return Value(true);
    }
    else if (js[token->start] == 'f') {
      return Value(false);
    }
    else if (js[token->start] == 'n') {
      return Value();
    }
    else {
      string s(js, token->start, token->end - token->start);
      std::stringstream ss(s);
      double num;
      ss >> num;
      return Value(num);
    }
  }

  if (token->type == JSMN_ARRAY) {
    int size = token->size;
    int offset = 1;
    int count_ = 0;
    Array _;
    Value value(_);
    Array& a = value.unwrap<Array>();
    for (int i = 0; i < size; i++) {
      a.Push(parse(js, token + offset, count_));
      offset += count_;
    }
    count = offset;
    return value;
  }

  if (token->type == JSMN_OBJECT) {
    int size = token->size;
    int offset = 1;
    int count_ = 0;
    Object _;
    Value value(_);
    Object& o = value.unwrap<Object>();
    for (int i = 0; i < size; i++) {
      Value key = parse(js, token + offset, count_);
      offset += count_;
      o[key.unwrap<String>()] = parse(js, token + offset, count_);
      offset += count_;
    }
    count = offset;
    return value;
  }

  // shouldn't be here
  throw Error("Unsupported Token type!");
}

Object parse(const string& json_string) {
  const char* buff = json_string.c_str();
  const int buff_size = json_string.length();

  jsmn_parser parser;
  jsmn_init(&parser);
  int token_size = MAX_TOKEN_SIZE;
  jsmntok_t* tokens = (jsmntok_t*)malloc(token_size*sizeof(jsmntok_t));
  if (tokens == NULL) throw Error("Insufficient memory!");

  int r = jsmn_parse(&parser, buff, buff_size, tokens, token_size);
  if (r < 0) throw Error("Parse json string failed!");
  if (r < 1 || tokens[0].type != JSMN_OBJECT) throw Error("Object expected!");

  int count;
  Value value = parse(buff, tokens, count);

  free(tokens);
  return value.unwrap<Object>();
}

static string serialize(const Value& value) {
  const Value::Type type = value.type();
  if (type == Value::BOOLEAN) {
    if (value.unwrap<Boolean>() == true) return "true";
    else return "false";
  }

  if (type == Value::STRING) {
    string s = value.unwrap<String>();
    return string("\"") + s + "\"";
  }

  if (type == Value::NIL) {
    return "null";
  }

  if (type == Value::NUMBER) {
    std::stringstream ss;
    ss << static_cast<double>(value.unwrap<Number>());
    return ss.str();
  }
  if (type == Value::ARRAY) {
    const Array& a = value.unwrap<Array>();
    string s = "[";
    for (int i = 0; i < a.size() - 1; i++) {
      s += serialize(a[i]) + ",";
    }
    if (a.size() >= 1) {
      s += serialize(a[a.size() - 1]);
    }
    s += "]";
    return s;
  }

  if (type == Value::OBJECT) {
    const Object& o = value.unwrap<Object>();
    Object::type::const_iterator it = o.begin();
    string s = "{";
    while (it != o.end()) {
      string key = it->first;
      s += "\"" + key + "\":";
      s += serialize(it->second);
      it++;
      if (it != o.end()) s += ",";
    }
    s += "}";
    return s;
  }

  // shouldn't be here
  throw Error("Unsupported Value type!");
}

string serialize(const Object& o) {
  Object::type::const_iterator it = o.begin();
  string s = "{";
  while (it != o.end()) {
    string key = it->first;
    s += "\"" + key + "\":";
    s += serialize(it->second);
    it++;
    if (it != o.end()) s += ",";
  }
  s += "}";
  return s;
}

} // namespace jsmn
