// Copyright (c) 2015 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
#include <string>
#include <stdexcept>

#include "exception.hxx"

#include "xml/parser.hxx"
#include "xml/document.hxx"
#include "audit/logger.hxx"

#include "testbench/testbench.hxx"

const std::string testXmlFilePath = "/usr/share/dpm/sample-policy.xml";

TESTCASE(XPath)
{
    try {
        Xml::Document* document = Xml::Parser::parseFile(testXmlFilePath);
        Xml::Node::NodeList nodes = document->evaluate("//policy-group[@name='APPLICATION']/policy[@name='SET_APP_INSTALLATION_MODE']");
        Xml::Node::NodeList::iterator iter = nodes.begin();
        while (iter != nodes.end()) {
            std::cout << "Node Name: " << iter->getName() << std::endl;
            ++iter;
        }
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(XmlWriter)
{
    try {
        Xml::Document* document = Xml::Parser::parseFile(testXmlFilePath);
        document->write("/opt/usr/tx.xml", "UTF-8", true);
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(XmlDomTree)
{
    try {
        Xml::Document* document = Xml::Parser::parseFile(testXmlFilePath);

        Xml::Node& root = document->getRootNode();

        Xml::Node::NodeList list = root.getChildren();
        Xml::Node::NodeList::iterator iter = list.begin();
        while (iter != list.end()) {
            std::cout << "Node Name: " << iter->getName() << std::endl;
            ++iter;
        }
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}
