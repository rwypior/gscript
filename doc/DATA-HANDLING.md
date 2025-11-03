Data handling
=============

The data handling between the host application, and the script is handled by a set of
classes based on `ScriptValue` class. In general, all major script calls will return
a pointer to `ScriptValue` base object, which provides some basic utilities to access
the actual data. Supplying data **to** the script, involves a creation of concrete data
type.

The script supports a range of common POD types, and few special types. Following is
the list of supported data types:

- ScriptBoolValue
- ScriptCharValue
- ScriptByteValue
- ScriptIntValue
- ScriptUnsignedIntValue
- ScriptFloatValue
- ScriptDoubleValue
- ScriptStringValue
- ScriptClassValue
- ScriptArrayValue
- ScriptReferenceValue
- ScriptNullValue
- ScriptReturnValue

### Subtypes

Certain types provided by the script are working in conjunction with other types refered
later as subtypes. Subtypes are used to reference underlying datatype, and distinguish
calls to functions accepting such types, for example, classes, references and arrays.

ScriptStringValue
-----------------

The `ScriptStringValue` encapsulates a basic string type and acts as a regular POD type.
This functionality must be extended to provide more string operations like concatenation,
metrics, formatting etc.

StringArrayValue
----------------

The `StringArrayValue` is a aggregation type providing functionality of basic array containing
a number of subtype objects.

ScriptReferenceValue
--------------------

The `ScriptReferenceValue` encapsulates any arbitrary type and is used to switch the default
pass-by-value (copy) behavior to pass-by-reference.

ScriptNullValue
---------------

The `ScriptNullValue` represents an empty data.

ScriptReturnValue
-----------------

The `ScriptReturnValue` is an internally used datatype, which encapsulates any arbitrary data 
type and is supposed to only be used internally by functions to detect whether `return` statement
was ran and the function execution should be stopped.