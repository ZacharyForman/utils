#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

// Struct that represents an argument
struct Arg {
  std::string arg_name;
  std::string help;
  std::string error;
  bool singular;
  bool required;
  bool used;
  // Parses the argument given.
  virtual void parse_value(const char *arg) = 0;
  // Returns the value of the variable in string form.
  virtual std::string get_var() = 0;
};

// struct that represents an argument with specific type.
template<typename T>
struct TypedArg : public Arg {
  T *var;
  void parse_value(const char *arg);
  std::string get_var();
};

template<typename T>
void TypedArg<T>::parse_value(const char *arg)
{
  std::stringstream ss;
  ss << arg;
  if (!(ss >> *var)) {
    std::cerr << error << std::endl;
    exit(1);
  }
}

template<>
void TypedArg<bool>::parse_value(const char *arg)
{
  if (singular) {
    *var = true;
    return;
  }
  std::stringstream ss;
  ss << arg;
  if (!(ss >> *var)) {
    std::cerr << error << std::endl;
    exit(1);
  }
  used = true;
}

template<typename T>
std::string TypedArg<T>::get_var()
{
  std::stringstream ss;
  ss << *var;
  return ss.str();
}

// Class that allows manipulation of command line args.
class ArgList {
public:
  // Adds an argument to be parsed.
  template<typename T>
  void add_arg(T *var, const std::string &flag,
               const T &default_val, const std::string &help,
               const std::string &error, bool required);

  // Parses the arguments given
  void parse_args(int argc, char **argv);

  // Cleans up the arguments added.
  ~ArgList();
private:
  std::map<std::string, Arg*> arguments;
};

template<typename T>
void ArgList::add_arg(T *var, const std::string &flag,
                      const T &default_val, const std::string &help,
                      const std::string &error, bool required)
{
  TypedArg<T> *a = new TypedArg<T>();
  *var = default_val;
  a->var = var;
  a->arg_name = flag;
  a->help = help;
  a->error = error;
  a->singular = false;
  a->required = required;
  a->used = false;
  arguments[flag] = a;
}

template<>
void ArgList::add_arg<bool>(bool *var, const std::string &flag,
                            const bool &default_val, const std::string &help,
                            const std::string &error, bool required)
{
  TypedArg<bool> *a = new TypedArg<bool>();
  *var = default_val;
  a->var = var;
  a->arg_name = flag;
  a->help = help;
  a->error = error;
  a->singular = true;
  a->required = required;
  a->used = false;
  arguments[flag] = a;
}

ArgList::~ArgList()
{
  std::map<std::string, Arg*>::iterator it;
  for (it = arguments.begin(); it != arguments.end(); it++) {
    delete it->second;
  }
}

// Parse arguments from argv.
void ArgList::parse_args(int argc, char **argv)
{
  std::map<std::string, Arg*>::iterator it;

  for (int i = 1; i < argc; i++) {
    // Check for help flag
    if (std::string(argv[i]) == "--help") {
      for (it = arguments.begin(); it != arguments.end(); it++) {
        std::stringstream ss;
        ss << it->second->arg_name;
        ss << std::string(18-ss.str().length(), ' ');
        ss << "= " << it->second->get_var();
        ss << std::string(35-ss.str().length(), ' ');
        ss << it->second->help << std::endl;
        std::cerr << ss.str();
      }
      exit(0);
    }
    // get arguments
    if (arguments.find(argv[i]) == arguments.end()) {
      std::cerr << "Bad argument " << argv[i] << std::endl;
      std::cerr << argv[0] << " --help for assistance." << std::endl;
      exit(1);
    } else {
      if (arguments[argv[i]]->singular) {
        arguments[argv[i]]->parse_value(argv[i]);
      } else {
        i++;
        arguments[argv[i-1]]->parse_value(argv[i]);
      }
    }
  }
  for (it = arguments.begin(); it != arguments.end(); it++) {
    if (it->second->required && !it->second->used) {
      std::cerr << "Error: " << it->second->arg_name
                << " required" << std::endl;
      exit(1);
    }
  }
}

#endif // ARGUMENTS_H
