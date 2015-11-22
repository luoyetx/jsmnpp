#ifndef JSMN_HPP_
#define JSMN_HPP_

#include <map>
#include <vector>
#include <string>
#include <exception>

namespace jsmn {

using std::map;
using std::vector;
using std::string;

class Error : public std::exception {
public:
  explicit Error(const string& msg = "Error")
    : msg_(msg) {
  }
  virtual ~Error() throw() {
  }
  virtual const char* what() const throw() {
    return msg_.c_str();
  }
private:
  string msg_;
};

// forward declaration
class Value;

/*!
 * \breif json object
 */
class Object {
public:
  /*! \breif default constructor */
  Object();
  /*! breif copy constructor */
  Object(const Object& other);
  /*! \breif assignment operator */
  Object& operator=(const Object& other);
  /*! \breif destructor */
  ~Object();

public:
  typedef map<string, Value> type;
  /*!
   * \breif subscript operator, access element in object
   * \param key   key
   * \return      value
   * \throws      jsmn::Error if key not exists in const mode
   *
   * \note in none const mode, you can use operator[] to insert a new value
   */
  Value& operator[](const string& key);
  const Value& operator[](const string& key) const;
  /*!
   * \breif iterators
   */
  type::iterator begin();
  type::iterator end();
  type::const_iterator begin() const;
  type::const_iterator end() const;
  /*!
   * \breif size of object
   * \return    size
   */
  inline size_t size() const {
    return object_.size();
  }

private:
  /*! \breif internal object */
  type object_;
};

/*!
 * \breif json array
 */
class Array {
public:
  /*! \breif default constructor */
  Array();
  /*! \breif copy constructor */
  Array(const Array& other);
  /*! \breif assignment operator */
  Array& operator=(const Array& other);
  /*! \breif destructor */
  ~Array();

public:
  typedef vector<Value> type;
  /*!
   * \breif subscripy operator, access element in array
   * \param index   index
   * \return        value of index in array
   * \throws        jsmn::Error if index out of range
   */
  Value& operator[](int index);
  const Value& operator[](int index) const;
  /*!
   * \breif iterators
   */
  type::iterator begin();
  type::iterator end();
  type::const_iterator begin() const;
  type::const_iterator end() const;
  /*!
   * \breif insert a value in the end of array
   * \param value   value
   */
  void Push(const Value& value);
  /*!
   * \breif size of array
   * \return    size
   */
  inline size_t size() const {
    return array_.size();
  }

private:
  /*! \breif internal array */
  type array_;
};

typedef double Number;
typedef bool Boolean;
typedef string String;

/*! \breif jsmn value */
class Value {
public:
  /*! \breif default constructor */
  Value();
  /*! \breif constructors with vary json type */
  Value(const Object& o);
  Value(const Array& a);
  Value(const String& s);
  Value(const char* s);
  Value(const Number& num);
  Value(const int num);
  Value(const Boolean& b);
  /*! \breif copy constructor */
  Value(const Value& other);
  /*! \breif assignment operator */
  Value& operator=(const Value& other);
  /*! \breif destructor */
  ~Value();

public:
  enum Type {
    BOOLEAN,
    NUMBER,
    STRING,
    ARRAY,
    OBJECT,
    NIL,
  };
  /*! \breif return data type */
  inline Type type() const {
    return type_;
  }
  /*!
   * \breif data transform
   * \tparam T    must be one of supported type
   */
  template<typename T>
  T& unwrap();
  template<typename T>
  const T& unwrap() const;
  /*!
   * \breif like a object
   * \param key   key
   * \return      value
   * \throws      jsmn::Error if type is not Object
   */
  Value& operator[](const string& key);
  const Value& operator[](const string& key) const;
  /*!
   * \breif like a array
   * \param index   index
   * \return        value
   * \throws        jsmn::Error if type is not Array
   */
  Value& operator[](int index);
  const Value& operator[](int index) const;

private:
  /*! \breif internal values */
  Object o_;
  Array a_;
  String s_;
  Number num_;
  Boolean b_;
  /*! \breif data type in this value */
  Type type_;
};

/*!
 * \breif parse from a json string
 * \param json_string   json string
 * \return              top level json object
 * \throws              jsmn::Error if memory insufficient or json string is wrong
 */
Object parse(const string& json_string);
/*!
 * \breif parse from a json file
 * \param file    path of json file
 * \return        top level json object
 * \throws        jsmn::Error if file not exists or throw the Error from parse(json_string)
 */
Object parse(const char* file);
/*!
 * \breif serialize a json object
 * \param o   top level json object
 * \return    json string of the object
 */
string serialize(const Object& o);

} // namespace jsmn

#endif // JSMN_HPP_
