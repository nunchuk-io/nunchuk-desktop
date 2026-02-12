#!/usr/bin/env python3
"""
Script to generate AppStrings.h from STR_QML.js file.
This parses the JavaScript string definitions and creates C++ property definitions.
"""

import re
import argparse
from pathlib import Path


def parse_js_strings(js_file_path: str) -> list[tuple[str, str]]:
    """
    Parse the JavaScript file and extract string definitions.
    Returns a list of tuples (variable_name, string_value).
    """
    strings = []
    
    with open(js_file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Pattern to match: var STR_QML_XXX = qsTr("...")
    # Handles both single and double quotes, and multiline strings
    pattern = r'var\s+(STR_QML_\w+)\s*=\s*qsTr\s*\(\s*["\'](.+?)["\']\s*\)'
    
    # Also handle template strings with .arg() calls - we'll just get the base string
    pattern_with_arg = r'var\s+(STR_QML_\w+)\s*=\s*qsTr\s*\(\s*["\'](.+?)["\']\s*\)(?:\.arg\([^)]+\))*'
    
    for match in re.finditer(pattern_with_arg, content, re.DOTALL):
        var_name = match.group(1)
        string_value = match.group(2)
        
        # Clean up the string value
        # Replace escaped quotes
        string_value = string_value.replace("\\'", "'")
        string_value = string_value.replace('\\"', '"')
        
        # Handle multiline strings (remove \n and extra whitespace)
        string_value = re.sub(r'\s*\\\s*\n\s*', ' ', string_value)
        string_value = re.sub(r'\s+', ' ', string_value).strip()
        
        strings.append((var_name, string_value))
    
    return strings


def convert_to_camel_case(var_name: str) -> str:
    """
    Convert STR_QML_XXX to a camelCase property name.
    Examples:
        STR_QML_000 -> strQml000
        STR_QML_1234 -> strQml1234
        STR_QML_123_abc -> strQml123Abc
    """
    # Remove STR_QML_ prefix and convert to lowercase
    name = var_name.replace('STR_QML_', '').lower()
    
    # Handle underscores by capitalizing the next letter
    parts = name.split('_')
    if len(parts) > 1:
        result = parts[0] + ''.join(p.capitalize() for p in parts[1:])
    else:
        result = name
    
    return f"strQml{result.capitalize()}" if result[0].isdigit() else f"strQml_{result}"


def generate_property_name(var_name: str) -> str:
    """
    Generate a property name from the variable name.
    Keep the original name from JS file (e.g., STR_QML_000)
    """
    return var_name


def escape_cpp_string(s: str) -> str:
    """
    Escape special characters for C++ string literal.
    """
    # Escape backslashes first
    s = s.replace('\\', '\\\\')
    # Escape double quotes
    s = s.replace('"', '\\"')
    return s


def generate_app_strings_header(strings: list[tuple[str, str]], output_path: str):
    """
    Generate the AppStrings.h file with all string properties.
    """
    header = '''#pragma once
#include <QObject>
#include <QQmlEngine>

#include "StringProperty.h"

namespace core::common::resources {
class AppStrings : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

'''
    
    # Add all string property definitions
    for var_name, string_value in strings:
        prop_name = generate_property_name(var_name)
        escaped_value = escape_cpp_string(string_value)
        header += f'    DEFINE_STRING_PROPERTY({prop_name}, "{escaped_value}")\n'
    
    header += '''
public:
    static AppStrings *instance();
    explicit AppStrings(QObject *parent = nullptr);

private:
    static AppStrings *s_instance;
};
}  // namespace core::common::resources
#define Strings core::common::resources::AppStrings::instance()
'''
    
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(header)
    
    print(f"Generated {output_path} with {len(strings)} string properties.")


def generate_qml_mapping(strings: list[tuple[str, str]], output_path: str):
    """
    Generate a QML helper file that maps old variable names to new AppStrings properties.
    """
    content = '''// Auto-generated mapping from STR_QML.js to AppStrings
// Usage: import this file and use Strings.str_qml_xxx instead of STR_QML_XXX

pragma Singleton
import QtQuick 2.15

QtObject {
'''
    
    for var_name, _ in strings:
        prop_name = generate_property_name(var_name)
        content += f'    readonly property string {var_name}: Strings.{prop_name}\n'
    
    content += '}\n'
    
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(content)
    
    print(f"Generated QML mapping file: {output_path}")


def main():
    parser = argparse.ArgumentParser(
        description='Generate AppStrings.h from STR_QML.js'
    )
    parser.add_argument(
        '-i', '--input',
        default='localization/STR_QML.js',
        help='Path to the input STR_QML.js file'
    )
    parser.add_argument(
        '-o', '--output',
        default='core/common/resources/AppStrings.h',
        help='Path to the output AppStrings.h file'
    )
    parser.add_argument(
        '--qml-mapping',
        default=None,
        help='Optional: Generate QML mapping file'
    )
    parser.add_argument(
        '--dry-run',
        action='store_true',
        help='Print the output without writing to file'
    )
    
    args = parser.parse_args()
    
    # Parse the JavaScript file
    print(f"Parsing {args.input}...")
    strings = parse_js_strings(args.input)
    print(f"Found {len(strings)} string definitions.")
    
    if args.dry_run:
        print("\n--- Preview of first 10 properties ---")
        for var_name, string_value in strings[:10]:
            prop_name = generate_property_name(var_name)
            escaped_value = escape_cpp_string(string_value)
            print(f'DEFINE_STRING_PROPERTY({prop_name}, "{escaped_value[:50]}...")')
        print("...")
        return
    
    # Generate the header file
    generate_app_strings_header(strings, args.output)
    
    # Optionally generate QML mapping
    if args.qml_mapping:
        generate_qml_mapping(strings, args.qml_mapping)


if __name__ == '__main__':
    main()
