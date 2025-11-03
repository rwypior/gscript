Extensions
==========

The language provides simple mechanism of extending the it's functionality by
extensions. Extensions are simple classes registered and executed **before** the
compilation stage. The host application may introduce any amount of extensions
performing _any_ action. Typically the extensions are used to register _namespaces_,
_classes_ and _functions_, but their behavior is _not_ limited to just that.

Default extensions
------------------

The script provides a set of default extensions, loaded by `gscript::Script::loadDefaultExtensions`
function, which loads all extensions provided in _(src;include)/extension_ directory.