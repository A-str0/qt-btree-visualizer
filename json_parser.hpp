#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include "binary_tree.hpp"
#include "operation_result.hpp"

class JsonParser final
{
public:
    OperationResult<BinaryTree::Node> parseJson(QJsonDocument jsonDoc) const;
};

#endif // JSON_PARSER_HPP
