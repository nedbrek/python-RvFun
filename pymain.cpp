#include <boost/python.hpp>

const char* hello()
{
	return "Hello world!";
}

BOOST_PYTHON_MODULE(hello_ext)
{
	namespace bp = boost::python;
	bp::def("hello", hello);
}

