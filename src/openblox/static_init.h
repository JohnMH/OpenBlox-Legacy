#include "OpenBlox.h"
#include <vector>


#define DECLARE_STATIC_INIT(ClassName) \
   static void static_init_func(); \
   static OpenBlox::static_init_helper ClassName##_static_init_helper

#define STATIC_INIT(ClassName) \
	OpenBlox::static_init_helper ClassName::ClassName##_static_init_helper(&ClassName::static_init_func); \
	void ClassName::static_init_func()

typedef void (*init_func_type)();

class static_init{
	public:
		static static_init& instance(){
			static static_init inst;
			return inst;
		}

		void add_init_func(init_func_type f){
			funcs_.push_back(f);
		}

		static void execute(){
			auto& inst = instance();
			for(auto c : inst.funcs_)
				c();
		}

	private:
		static_init(){
		}

		std::vector<init_func_type> funcs_;
};

namespace OpenBlox{
	class static_init_helper{
		public:
			static_init_helper(init_func_type f){
				static_init::instance().add_init_func(f);
			}
	};
}
