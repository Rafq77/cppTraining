Exercise "Exception Safety"
base: staticVector

 - Uncomment '-fsanitize=address' in common.mak

 - Introduce non-exception safe constructs to trigger the sanitizer
   with memory leaks.
