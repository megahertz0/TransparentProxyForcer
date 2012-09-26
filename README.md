# Usage
## On Linux
  * export LD_PRELOAD=/path/to/forcedproxy.so
  * /path/to/your/application
  
## On Mac OS X
  * export DYLD_INSERT_LIBRARIES=/path/to/forcedproxy.dylib
  * export DYLD_FORCE_FLAT_NAMESPACE=1 
  * /path/to/your/application

## On Windows
  * Run the ForcedProxy.exe, click Install
  * Start your Application (it should appear on the listbox)
  * Close your Application
  * Check the tickbox beside Appplication name on the listbox
  * Start TransparentProxy on your computer
  * Start your Application  