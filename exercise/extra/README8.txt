Exercise "RAII"

 - Get rid of Group's (non-default) destructor.
 - Think about which kind of (smart) pointers you want to use where.

 - Apply "Rule of Zero" to Group.

 - Possibly rename your existing Canvas
 - Use extra/canvas-pimpl.* as canvas.*
 - Apply "Rule of Zero" (with move, no copy) to the new Canvas

 - Get rid of CanvasImpl's (non-default) destructor.
   Note: UnregisterCallback() must be called first in ~CanvasImpl()
   to avoid any problems should we ever have multithreading!

 - Get rid of the (non-default) destructors for Win (hard).

 - Think about "Rule of Zero" for your original Canvas class
   (extremly hard, requires a helper class that implements
   the "magical" functions).

 - If you have move for Win, think about "Rule of Zero" for it
   (extremly hard, again requires helper function).
