#ifndef __MAIN_H__
#define __MAIN_H__

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <vector>
#include "utils.h"

using namespace std;


// =============================================================================
// GRAPH EXCEPTIONS
// =============================================================================
class VertexNotFoundException : public std::logic_error {
public:
    VertexNotFoundException() : std::logic_error("Vertex not found!") {}
    explicit VertexNotFoundException(const std::string& what_arg) : std::logic_error(what_arg) {}
};

class EdgeNotFoundException : public std::logic_error {
public:
    EdgeNotFoundException() : std::logic_error("Edge not found!") {}
    explicit EdgeNotFoundException(const std::string& what_arg) : std::logic_error(what_arg) {}
};

// =============================================================================
// KNOWLEDGE GRAPH EXCEPTIONS
// =============================================================================

class EntityExistsException : public std::logic_error {
public:
    EntityExistsException() : std::logic_error("Entity already exists!") {}
    explicit EntityExistsException(const std::string& what_arg) : std::logic_error(what_arg) {}
};

class EntityNotFoundException : public std::logic_error {
public:
    EntityNotFoundException() : std::logic_error("Entity not found!") {}
    explicit EntityNotFoundException(const std::string& what_arg) : std::logic_error(what_arg) {}
};


#endif // __MAIN_H__
