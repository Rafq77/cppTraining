Exercise "Lambda"

 - Replace the predicate for find_if in XDisplayHolder::handleEvent by a lambda.
   It looks in WinList for a Window with matches the id in e.xexpose.window
   and uses the compare() member function of XWin for this.

 - Replace the Redraw class by an argument-less lambda.

 - Replace the implementation of the recursive functions in Group
   by an apply function taking a lambda.
