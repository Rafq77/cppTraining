Exercise "Error Handling"
base: staticVector

 - Add error checking to index operator.

 - Exception (out_of_range)

 - terminate

 - Write your own class 'Expected<Val, Err'.
   Provide constructors for value and error.
   Provide hasError(), hasValue(), op* and op->.
   Throw if value is accessed but error is stored.
   Throw if destructed and error was stored but not accessed.
