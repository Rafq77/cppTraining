Exercise "Dynamic Memory w/ Allocator"
base staticVector

 - define StaticVector as (template) typedef of std::pmr::vector

 - provide an allocator as derivation of std::pmr::memory_resource
   that only allows one allocation and throws on a second allocation

 - create an allocator object and a static vector with it
   and do an initial capacity()

 - provide a function makeStaticVector(size) that already
   does the capacity() call and returns a StaticVector
   (in a pair together with a unique_ptr to the allocator)
