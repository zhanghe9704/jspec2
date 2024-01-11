# generate_bindings.py



import glob
import os
import CppHeaderParser

def get_class_names_from_header(header_file):
    try:
        cpp_header = CppHeaderParser.CppHeader(header_file)
        return list(cpp_header.classes.keys())
    except CppHeaderParser.CppParseError as e:
        print(f"Error parsing header file {header_file}: {e}")
        return []
    
def get_root_class_names_from_header(header_file):
    try:
        cpp_header = CppHeaderParser.CppHeader(header_file)
        root_classes = [class_name for class_name, class_info in cpp_header.classes.items() 
                        if not class_info['inherits']]
        root_classes = [class_name for class_name in root_classes 
                        if '::' not in class_name and len(cpp_header.classes[class_name]['methods']['public'])>0]
        return root_classes, cpp_header
    except CppHeaderParser.CppParseError as e:
        print(f"Error parsing header file {header_file}: {e}")
        return [], cpp_header
    
def find_overloaded_functions(cpp_class):
    overloaded_functions = []

    try:
        # # Parse the C++ header file
        # cpp_header = CppHeaderParser.CppHeader(header_file)

        # # Iterate through the parsed classes
        # for class_name in cpp_header.classes:
        #     cpp_class = cpp_header.classes[class_name]

            # Dictionary to store methods and their signatures
            method_signatures = {}

            # Collect information about methods
            for access in ["public", "private", "protected"]:
                for method in cpp_class["methods"][access]:
                    method_signature = (method['name'], len(method['parameters']))
                    method_signatures.setdefault(method_signature, 0)
                    method_signatures[method_signature] += 1

            # Add overloaded methods to the result
            for method_signature, count in method_signatures.items():
                if count > 1:
                    overloaded_functions.append((method_signature[0], method_signature[1], count))

    except CppHeaderParser.CppParseError as e:
        print(f"Error parsing header file: {e}")

    return overloaded_functions

# from gen_jspec import jspec2_pb, file_pb

headers_to_bind = ['beam.h', 'ring.h', 'cooler.h', 'ions.h', 'ibs.h', 
                   'ecooling.h', 'force.h', 'simulator.h']

jspec_path = '../' 
header_path = jspec_path+'/include'
headers_fullpath = glob.glob(header_path + '/*.h')
# print(headers_fullpath)

headers_found = []
for file_path in headers_fullpath:
   headers_found.append(os.path.basename(file_path))
   
print(headers_found)
   
headers = list(set(headers_found) & set(headers_to_bind))
headers = [header[:-2] for header in headers]
print(headers)

# jspec2_pb(headers)

# h = 'ring'

# file_pb(h, header_path)

root_classes = {}
header_parsers = {}
for header in headers:
    header_file = header_path + '/' + header +'.h'
    root_class_names, h = get_root_class_names_from_header(header_file)
    header_parsers[header] = h
    for c in root_class_names:
        root_classes[c] = header
    # overloads = find_overloaded_functions(header_file)

    # if overloads:
    #     for name, param_count, overload_count in overloads:
    #         print(f"Overloaded Method: {name}, Parameters: {param_count}, Overloaded Count: {overload_count}")
    # else:
    #     print("No overloaded functions found.")
   
print(root_classes)

file_1 = 'fortran_interface.h'
file_2 = 'fortran_interface.cc'
file_tmp_1 = 'type.tmp'
file_tmp_2 = 'interface.tmp'
file_tmp_3 = 'genetic_interface.tmp'
file_tmp_4 = 'contains.tmp'

fortran_interface_includes = '\
#ifndef FORTRAN_INTERFACE_H\n\
#define FORTRAN_INTERFACE_H\n\
'

for h in headers_to_bind:
    fortran_interface_includes += '#include "' + h+'"\n'


fortran_interface_includes += '\n#include <string>\n\n'

fortran_interface_includes += 'enum class JSPEC_Class {'
for c in root_classes.keys():
    fortran_interface_includes += c.upper()+', '
    
fortran_interface_includes += '};\n\n'

with open(file_1, 'w') as file:
    file.write(fortran_interface_includes)
