/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#include <iostream>

#include "node.h"

#include "exception.h"

namespace xml {

Node::Node(xmlNode* node) :
    implementation(node)
{
    implementation->_private = this;
}

Node::Node(Node&& node) :
    implementation(node.implementation)
{
    implementation->_private = this;
    node.implementation = nullptr;
}

Node::~Node()
{
    if (implementation != nullptr) {
        implementation->_private = nullptr;
    }
}

Node::NodeList Node::getChildren()
{
    NodeList nodeList;

    auto child = implementation->xmlChildrenNode;
    while (child != nullptr) {
        nodeList.push_back(Node(child));
        child = child->next;
    }

    return nodeList;
}

Node Node::addNewChild(const std::string& name)
{
   xmlNode* nodePtr = xmlNewNode(NULL, xmlStrdup((const xmlChar*)name.c_str()));
   xmlAddChild(implementation, nodePtr);

   return Node(nodePtr);
}

std::string Node::getName() const
{
    return implementation->name ? (const char*)implementation->name : "";
}

void Node::setName(const std::string& name)
{
    xmlNodeSetName(implementation, (const xmlChar*)name.c_str());
}

std::string Node::getContent() const
{
    if (implementation->type != XML_ELEMENT_NODE) {
        throw runtime::Exception("This node type does not have content");
    }
    xmlNode* child = implementation->xmlChildrenNode;
    if (child == NULL || xmlIsBlankNode(child)) {
        return "";
    }
    return child->content ? (char*)child->content : "";
}

void Node::setContent(const std::string& content)
{
    if (implementation->type != XML_ELEMENT_NODE) {
        throw runtime::Exception("Can not set content for this node type");
    }

    auto child = implementation->xmlChildrenNode;
    xmlNodeSetContent(child, (xmlChar*)content.c_str());
}

std::string Node::getProp(const std::string& name) const
{
    xmlChar* result;

    if (implementation->type != XML_ELEMENT_NODE) {
        throw runtime::Exception("This node type does not have properties");
    }

    result = xmlGetProp(implementation, (xmlChar*)name.c_str());
    if (result) {
        std::string retStr((const char*)result);
        xmlFree(result);
        return retStr;
    }

    return "";
}

void Node::setProp(const std::string& name, const std::string& val)
{
    if (implementation->type != XML_ELEMENT_NODE) {
        throw runtime::Exception("Can not set properties for this node type");
    }

    xmlSetProp(implementation, (xmlChar*)name.c_str(), (xmlChar*)val.c_str());
}

bool Node::isBlank() const
{
    return xmlIsBlankNode(const_cast<xmlNode*>(implementation));
}

} // namespace xml
