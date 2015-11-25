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

#include <string>

#include <libxml/parserInternals.h>

#include "exception.hxx"

#include "parser.hxx"
#include "keepblanks.hxx"

namespace Xml {

Document* Parser::parseFile(const std::string& filename, bool validate)
{
    KeepBlanks(false);

    xmlParserCtxt *context = xmlCreateFileParserCtxt(filename.c_str());
    if (context == nullptr) {
        throw Runtime::Exception("Could not create parser context");
    }

    if (context->directory == nullptr) {
        context->directory = xmlParserGetDirectory(filename.c_str());
    }

    int options = 0;

    if (validate) {
        options |= XML_PARSE_DTDVALID;
    } else {
        options &= ~XML_PARSE_DTDVALID;
    }

    xmlCtxtUseOptions(context, options);

    if (xmlParseDocument(context) < 0) {
        xmlFreeParserCtxt(context);
        throw Runtime::Exception("Parsing failed");
    }

    xmlDoc *document = context->myDoc;

    // We took the ownership on the doc
    context->myDoc = nullptr;

    xmlFreeParserCtxt(context);

    return new Document(document);
}

} // namespace Xml
