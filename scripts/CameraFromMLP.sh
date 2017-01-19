#!/bin/bash

# Extract matrix attributes from an xml file.
# ./ExtractFromXML /path/to/file TranslationVector


cat  ${1} | grep VCGCamera | sed 's/.*'${2}'="\([0-9e \.-]*\)".*/\1/'
