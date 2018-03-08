Variable byte encoding list
=============

In-memory usage
-------------
Sometimes you may want to store a list of unsigned integers in a space-efficient way. For that purpose you can use VBList, which uses many bytes for large numbers and less for little ones. It supports push_back operation and can be iterated from the start. If your integers are sorted, you may further save your space by storing only gaps between your numbers: for example, instead of storing 44, 48, 54, you may store 44, 4, 6. 
usage:

	VBList<unsigned long long> l;
	l.push_back(12);
	l.push_back(1223233);
	for (auto i: l)
		cout << i << endl;


Writing to file
-------------
If you want to save a lot of unsigned integers to a file, you may use VBOutputIterator.
The usage is:

	ofstream fout("file");
	ostreambuf_iterator<char> ofstr_iter (fout.rdbuf());
	VBOutputIterator<ostreambuf_iterator<char>, unsigned, false> out(ofstr_iter);
	out = 1;
	out = 333;
	out = 18;
	out.terminate();
	fout.flush();

That's it!

To read back from the file:

	ifstream fin("file");
	VBInputIterator<istreambuf_iterator<char>, unsigned> in(fin.rdbuf());
	VBInputIterator<istreambuf_iterator<char>, unsigned> end;
	while (in != end)
		{
		cout << *in << endl;
		++in;
		}

Enjoy!