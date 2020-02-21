#include "inst.hpp"
#include "simple_arch_state.hpp"
#include "host_system.hpp"
#include <boost/python.hpp>
#include <iomanip>

rvfun::HostSystem host_;
rvfun::SimpleArchState state_;

// map files
// toggle debug

class Driver
{
public:
	Driver()
	{
		state_.setSys(&host_);
		state_.setMem(host_.getMem());
	}

	bool setElf(const char *elf_name)
	{
		return host_.loadElf(elf_name, state_);
	}

	void addArg(const std::string &s)
	{
		host_.addArg(s);
	}

	void finalize()
	{
		if (finalized_)
			return;
		finalized_ = true;

		host_.completeEnv(state_);
	}

	bool hadExit() const { return host_.hadExit(); }

	bool run(uint64_t num_i)
	{
		if (!finalized_)
			return true;

		bool debug = true;
		for (uint64_t icount = 0; icount < num_i; ++icount)
		{
			uint32_t opc_sz = 4;
			uint32_t full_inst = 0;
			if (debug)
				std::cout << std::setw(12) << icount << ' ';
			std::unique_ptr<rvfun::Inst> inst(decode(state_, opc_sz, full_inst, debug));
			if (!inst)
			{
				state_.incPc(opc_sz);
			}
			else
			{
				inst->execute(state_);
			}
			if (debug)
				std::cout << '\n';
		}
		return false;
	}

private:
	bool finalized_ = false;
};

BOOST_PYTHON_MODULE(rvfun)
{
	namespace bp = boost::python;
	bp::class_<Driver>("rvfun")
	    .def("set_elf", &Driver::setElf)
	    .def("add_arg", &Driver::addArg)
	    .def("finalize", &Driver::finalize)
	    .def("had_exit", &Driver::hadExit)
	    .def("run", &Driver::run)
	;
}

