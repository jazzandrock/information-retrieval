## Information retrieval project

A pile of C++ code with lots of templates, algorithms, low-level bit manipulation and raw pointer memory management.

It indexes your files and helps you find some most useful of them.

We build index: a word gets mapped to a list of IDs of documents that contain it. When index reaches some critical size, we compress it: we use less bytes for small numbers. Further, for sorted lists of IDs we only save differences between numbers, reaching even more efficient compression. Compressed indexes can also be merged together.

For searching, we represent documents as vectors in _number-of-unique-words_-dimensional space. We treat a query as another document, and find angle between all our docs and the query, returning the most similar ones.

![screen of search results](http://telegra.ph/file/972b5dfac62be8d29a6be.png)