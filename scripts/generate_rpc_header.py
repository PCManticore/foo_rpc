"""Parses a given header in order to generate the RPC interface of it."""

import argparse
import datetime
import os
import re
import sys

import CppHeaderParser as header_parser


CLASS_HEADER = """
class Rpc{name} {{
  private:
    foobar::{name} api;
  public:
{methods}
}};"""

HEADER_HEADER = """// Generated on {date}
#pragma once
#include <string>
#include <map>
#include <tuple>

#include "stdafx.h"
#include "event.h"
#include "percolate.h"
#include "api/{header}"
#include "factory.h"

using namespace std;
using namespace serialization;

{cls}"""

EVENT_ONLY = """
    Payload {name}(vector<char> & _unused) {{
      Event event;
      fb2k::inMainThread([&] {{
        api.{name}(event);
      }});
      event.wait();
      return serialization::serializer.packed_bool(true);
    }}"""

PARAM_ONLY = """
    Payload {name}(vector<char> & buffer) {{
      {param_type} param(serialization::serializer.unpack<{param_param_type}>(buffer));
      Event event;
      fb2k::inMainThread([&] {{
        api.{name}(param, event);
      }});
      event.wait();
      return serialization::serializer.packed_bool(true);
    }}"""

RESULT_ONLY = """
    Payload {name}(vector<char> & buffer) {{
      {type} result;
      fb2k::inMainThread([&] {{
        api.{name}(result);
      }});
      result.wait();            
      return serialization::serializer.packed_result(result);
    }}"""

RESULT_AND_PARAM = """
    Payload {name}(vector<char> & buffer) {{
      {param_type} param(serialization::serializer.unpack<{param_param_type}>(buffer));
      {result_type} result;
      fb2k::inMainThread([&] {{
        api.{name}(param, result);
      }});
      result.wait();
      return serialization::serializer.packed_result(result);
    }}"""

DISPATCH_ENTRY = '''
      registry["{cls_name}.{method}"] = [&](vector<char> & param) {{
        return {rpc_member}.{method}(param);
      }};
'''


def _generate_method_from_parameters(name, parameters):
    if len(parameters) == 1:
        parameter = parameters[0]
        parameter_type = parameter['raw_type']
        if parameter_type.find('ApiResult') > -1:
            return RESULT_ONLY.format(name=name, type=parameter_type)

        elif parameter_type.find('Event') > -1:
            return EVENT_ONLY.format(name=name)

    if len(parameters) == 2:
        first, second = parameters
        first_parameter_type = first['raw_type']
        second_parameter_type = second['raw_type']
        param_type = re.sub("^ApiParam<(.*?)>$", r"\1", first_parameter_type)
        param_type = param_type.strip()

        if second_parameter_type.find('Event') > -1:
            return PARAM_ONLY.format(name=name,
                                     param_type=first_parameter_type,
                                     param_param_type=param_type)

        elif second_parameter_type.find('ApiResult') > -1:
            return RESULT_AND_PARAM.format(name=name,
                                           param_type=first_parameter_type,
                                           param_param_type=param_type,
                                           result_type=second_parameter_type)
    else:
        raise ValueError('Cannot interpreter the given method.')


class RpcClass(object):

    def __init__(self, name, body):
        self.name = name
        self.body = body

    def public_methods(self):
        for method in self.body['methods']['public']:
            name = method['name']
            parameters = method['parameters']
            if name == self.name:
                # It is a constructor
                continue

            yield _generate_method_from_parameters(name, parameters)

    # Generation of data

    def generate(self):
        methods = "\n".join(self.public_methods())
        return CLASS_HEADER.format(name=self.name, methods=methods)


def parse_header(header_file):
    return header_parser.CppHeader(header_file)

def generate_header(args):
    header = parse_header(args.header)
    for cls, cls_body in header.classes.items():
        class_header = RpcClass(cls, cls_body).generate()

        header_header = HEADER_HEADER.format(
            header=os.path.basename(args.header),
            cls=class_header,
            date=datetime.datetime.now().strftime("%Y-%M-%d %H:%m:%S"))

        with args.output as stream:
            stream.write(header_header)


def _to_snake_case(name):
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()


def generate_dispatch(args):
    header = parse_header(args.header)
    for cls, cls_body in header.classes.items():
        cls_snake_case = _to_snake_case(cls)
        rpc_member = "rpc_" + cls_snake_case

        for method in cls_body['methods']['public']:
            name = method['name']
            if name == cls:
                # Constructor.
                continue
            print(DISPATCH_ENTRY.format(rpc_member=rpc_member, method=name, cls_name=cls), end="")


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="cmd")
    generate_header_parser = subparsers.add_parser(
        "generate_header",
        description="Generates a RPC header file from an API header file.")
    generate_dispatch_parser = subparsers.add_parser(
        "generate_dispatch",
        description="Generates a partial dispatch table with the methods "
                    "from the given header.")

    generate_header_parser.add_argument("--header", type=str)
    generate_header_parser.add_argument("--output",
                                        required=True,
                                        type=argparse.FileType('w+'))

    generate_dispatch_parser.add_argument("--header", type=str)

    args = parser.parse_args()

    if args.cmd is None:
        parser.print_help()
        exit()

    globals()[args.cmd](args)
