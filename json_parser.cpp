#include "json_parser.hpp"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include <optional>
#include <memory>
#include <utility>

namespace {

using BinaryTree::Node;
using NodePtr = std::unique_ptr<Node>;
using NamedValue = std::pair<QString, QJsonValue>;

std::optional<NamedValue> findFirstValue(
    const QJsonObject& object,
    std::initializer_list<const char*> keys)
{
    for (const char* rawKey : keys) {
        const QString key = QString::fromLatin1(rawKey);
        const auto it = object.find(key);
        if (it != object.end())
            return NamedValue{key, it.value()};
    }

    return std::nullopt;
}

OperationResult<Node> parseNodeValue(const QJsonValue& jsonValue, const QString& path);

OperationResult<NodePtr> parseChildNode(const QJsonValue& jsonValue, const QString& path)
{
    if (jsonValue.isNull())
        return OperationResult<NodePtr>::success(nullptr);

    auto nodeResult = parseNodeValue(jsonValue, path);
    if (!nodeResult.ok)
        return OperationResult<NodePtr>::failed(nodeResult.message);

    return OperationResult<NodePtr>::success(std::make_unique<Node>(std::move(nodeResult.value)));
}

OperationResult<NodePtr> parseObjectChild(
    const QJsonObject& object,
    std::initializer_list<const char*> keys,
    const QString& path)
{
    const auto entry = findFirstValue(object, keys);
    if (!entry.has_value())
        return OperationResult<NodePtr>::success(nullptr);

    return parseChildNode(entry->second, path + QLatin1Char('.') + entry->first);
}

OperationResult<NodePtr> parseArrayNode(const QJsonArray& array, qsizetype index, const QString& path)
{
    if (index >= array.size())
        return OperationResult<NodePtr>::success(nullptr);

    const QJsonValue currentValue = array.at(index);
    if (currentValue.isNull())
        return OperationResult<NodePtr>::success(nullptr);

    if (currentValue.isArray()) {
        return OperationResult<NodePtr>::failed(
            QStringLiteral("%1 must not contain nested arrays").arg(path));
    }

    auto nodeResult = parseChildNode(currentValue, path);
    if (!nodeResult.ok)
        return OperationResult<NodePtr>::failed(nodeResult.message);

    auto leftResult = parseArrayNode(array, (index * 2) + 1, path + QStringLiteral(".left"));
    if (!leftResult.ok)
        return OperationResult<NodePtr>::failed(leftResult.message);

    auto rightResult = parseArrayNode(array, (index * 2) + 2, path + QStringLiteral(".right"));
    if (!rightResult.ok)
        return OperationResult<NodePtr>::failed(rightResult.message);

    nodeResult.value->left = std::move(leftResult.value);
    nodeResult.value->right = std::move(rightResult.value);
    return OperationResult<NodePtr>::success(std::move(nodeResult.value));
}

OperationResult<Node> parseObjectNode(const QJsonObject& object, const QString& path)
{
    const auto valueEntry = findFirstValue(object, {"value", "data", "key", "label", "id"});
    if (!valueEntry.has_value()) {
        return OperationResult<Node>::failed(
            QStringLiteral("%1 has no supported value field").arg(path));
    }

    Node node(valueEntry->second.toVariant());

    auto leftResult = parseObjectChild(object, {"left"}, path);
    if (!leftResult.ok)
        return OperationResult<Node>::failed(leftResult.message);

    auto rightResult = parseObjectChild(object, {"right"}, path);
    if (!rightResult.ok)
        return OperationResult<Node>::failed(rightResult.message);

    node.left = std::move(leftResult.value);
    node.right = std::move(rightResult.value);

    return OperationResult<Node>::success(std::move(node));
}

OperationResult<Node> parseNodeValue(const QJsonValue& jsonValue, const QString& path)
{
    if (jsonValue.isNull()) {
        return OperationResult<Node>::failed(QStringLiteral("%1 is null").arg(path));
    }

    if (jsonValue.isObject()) {
        return parseObjectNode(jsonValue.toObject(), path);
    }

    if (jsonValue.isArray()) {
        return OperationResult<Node>::failed(
            QStringLiteral("%1 must be an object or scalar value").arg(path));
    }

    return OperationResult<Node>::success(Node(jsonValue.toVariant()));
}

} // namespace

OperationResult<BinaryTree::Node> JsonParser::parseJson(QJsonDocument jsonDoc) const
{
    if (jsonDoc.isNull()) {
        return OperationResult<Node>::failed(QStringLiteral("JSON document is null"));
    }

    if (jsonDoc.isArray()) {
        auto rootResult = parseArrayNode(jsonDoc.array(), 0, QStringLiteral("root"));
        if (!rootResult.ok) {
            return OperationResult<Node>::failed(rootResult.message);
        }

        if (!rootResult.value) {
            return OperationResult<Node>::failed(QStringLiteral("Root node is missing"));
        }

        return OperationResult<Node>::success(std::move(*rootResult.value));
    }

    const QJsonObject rootObject = jsonDoc.object();
    if (rootObject.contains(QStringLiteral("root"))) {
        return parseNodeValue(rootObject.value(QStringLiteral("root")), QStringLiteral("root"));
    }

    return parseObjectNode(rootObject, QStringLiteral("root"));
}
