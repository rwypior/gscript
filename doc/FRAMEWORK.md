Framework
=========

The script provides some built-in functions. Unlike extensions, the functions
provided by default (known as framework functions) are always accessible in the
script.

Currently the script provides the following framework functions:

##### string tostring(char x)
##### string tostring(byte x)
##### string tostring(int x)
##### string tostring(unsigned int x)
##### string tostring(float x)
##### string tostring(double x)
##### string tostring(\<class\> x) - TODO

Converts given value `x` into string. Currently this functionality only works on POD
types. Future implementation will provide an ability to convert any given class value
into the string by calling specific method inside that class if provided.