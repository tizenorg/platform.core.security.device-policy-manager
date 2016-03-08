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

#include "exception.hxx"

#include "keepblanks.hxx"
#include "document.hxx"

#include "audit/logger.hxx"

namespace Xml {

Document::Document(const std::string& version)
    : rootNode(nullptr),
      implementation(xmlNewDoc((const xmlChar*)version.c_str()))
{
    if (implementation == nullptr) {
        throw Runtime::Exception("Failed to create document");
    }

    implementation->_private = this;
}

Document::Document(xmlDoc* doc)
    : implementation(doc)
{
    implementation->_private = this;

    rootNode = new Node(xmlDocGetRootElement(implementation));
}

Document::~Document()
{
    if (rootNode != nullptr) {
        delete rootNode;
    }

    xmlFreeDoc(implementation);
}

Node& Document::getRootNode()
{
    if (rootNode == nullptr) {
        throw Runtime::Exception("Empty document");
    }

    return *rootNode;
}

std::string Document::getEncoding() const
{
    std::string encoding;

    if (implementation->encoding) {
        encoding = (const char*)implementation->encoding;
    }

    return encoding;
}

Node::NodeList Document::evaluate(const std::string& xpath)
{
    auto ctxt = xmlXPathNewContext(implementation);
    if (ctxt == nullptr) {
        throw Runtime::Exception("Failed to create XPath context for " + xpath);
    }

    auto result = xmlXPathEval((const xmlChar*)xpath.c_str(), ctxt);
    if (result == nullptr) {
        xmlXPathFreeContext(ctxt);
        throw Runtime::Exception("Invalid XPath: " + xpath);
    }

    if (result ->type != XPATH_NODESET) {
        xmlXPathFreeObject(result);
        xmlXPathFreeContext(ctxt);

        throw Runtime::Exception("Only nodeset result types are supported");
    }

    auto nodeset = result->nodesetval;

    Node::NodeList nodes;
    if ((nodeset == nullptr) || (xmlXPathNodeSetIsEmpty(nodeset))) {
        xmlXPathFreeContext(ctxt);
        return nodes;
    }

    const int count = xmlXPathNodeSetGetLength(nodeset);

    nodes.reserve(count);
    for (int i = 0; i != count; i++) {
        auto cnode = xmlXPathNodeSetItem(nodeset, i);
        if (!cnode) {
            ERROR("Node::find() : xmlNode was null");
            continue;
        }

        if (cnode->type == XML_NAMESPACE_DECL) {
            ERROR("Node::find() : Ignore an xmlNS object");
            continue;
        }

        nodes.push_back(Node(cnode));
    }

    xmlXPathFreeObject(result);
    xmlXPathFreeContext(ctxt);

    return nodes;
}

void Document::write(const std::string& filename, const std::string& encoding, bool formatted)
{
    KeepBlanks keepBlanks(KeepBlanks::Default);
    xmlIndentTreeOutput = formatted;

    xmlResetLastError();

    const int result = xmlSaveFormatFileEnc(filename.c_str(),
                                            implementation,
                                            encoding.c_str(),
                                            formatted);
    if (result == 0) {
        throw Runtime::Exception("Failed to write XML document");
    }
}

} // namespace Xml
