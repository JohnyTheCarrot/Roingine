#include <duk_module_duktape.h>
#include <duktape.h>
#include <roingine/duk_wrappers.h>

namespace roingine {
	static void FatalHandler(void *, char const *msg) {
		throw FatalScriptError{msg};
	}

	DukContext::DukContext()
	    : m_DukContext{duk_create_heap(nullptr, nullptr, nullptr, nullptr, FatalHandler)} {
		duk_module_duktape_init(m_DukContext.get());
	}

	DukContext::~DukContext() = default;

	DukObject DukContext::PushGlobalObject() const {
		return {m_DukContext.get()};
	}

	void DukContext::Eval(std::string_view code) const {
		if (duk_peval_string(m_DukContext.get(), code.data()) != 0) {
			std::string const errMessage{duk_safe_to_string(m_DukContext.get(), -1)};
			duk_pop(m_DukContext.get());
			throw ScriptCompilationFailedException{errMessage};
		}

		duk_pop(m_DukContext.get());
	}

	char const *DukContext::GetGlobalString(std::string const &key) const {
		duk_get_global_string(m_DukContext.get(), key.c_str());
		auto const scriptName{duk_get_string(m_DukContext.get(), -1)};
		duk_pop(m_DukContext.get());
		return scriptName;
	}

	duk_context *DukContext::GetRawContext() {
		return m_DukContext.get();
	}

	DukObject::DukObject(duk_context *ctx, std::optional<DukAddObjToParentInfo> addInfo)
	    : m_DukContext{ctx}
	    , m_AddToParentInfo{addInfo} {
		if (!addInfo.has_value()) {
			duk_push_global_object(ctx);
		} else {
			duk_push_object(ctx);
		}

		m_StackBottomOffset = duk_require_top_index(ctx);
	}

	DukObject::~DukObject() {
		auto const currentTop{duk_require_top_index(m_DukContext)};
		if (m_AddToParentInfo.has_value()) {
			auto const index{m_AddToParentInfo->parentIndex - currentTop + 1};

			duk_put_prop_string(m_DukContext, index, m_AddToParentInfo->key.c_str());
		} else if (m_ShouldPop)
			duk_pop(m_DukContext);
	}

	int DukObject::GetInt(std::string_view key) const {
		duk_get_prop_string(m_DukContext, m_StackBottomOffset, key.data());
		int const result{duk_to_int32(m_DukContext, -1)};
		duk_pop(m_DukContext);

		return result;
	}

	void DukObject::PutFunctionList(duk_function_list_entry const *functionList) {
		auto const currentTop{duk_require_top_index(m_DukContext)};
		auto const index{currentTop - m_StackBottomOffset + 1};

		duk_put_function_list(m_DukContext, index, functionList);
	}

	void DukObject::PutNumberList(duk_number_list_entry const *numberList) {
		auto const currentTop{duk_require_top_index(m_DukContext)};
		auto const index{currentTop - m_StackBottomOffset + 1};

		duk_put_number_list(m_DukContext, index, numberList);
	}

	void DukObject::MarkForReturn() {
		m_ShouldPop = false;
	}

	DukObject DukObject::PushObject(std::string const &key) const {
		return {m_DukContext, DukAddObjToParentInfo{key, m_StackBottomOffset}};
	}

	void DukObject::PushPointer(std::string const &key, void *ptr) const {
		auto const currentTop{duk_require_top_index(m_DukContext)};
		auto const index{currentTop - m_StackBottomOffset};

		duk_push_pointer(m_DukContext, ptr);
		duk_put_prop_string(m_DukContext, index, key.c_str());
	}

	DukObject::DukObject(DukObject &&other)
	    : m_DukContext{other.m_DukContext}
	    , m_StackBottomOffset{other.m_StackBottomOffset}
	    , m_ShouldPop{other.m_ShouldPop} {
		other.m_ShouldPop = false;
	}

	DukObject &DukObject::operator=(DukObject &&other) {
		m_DukContext        = other.m_DukContext;
		m_StackBottomOffset = other.m_StackBottomOffset;
		m_ShouldPop         = other.m_ShouldPop;
		other.m_ShouldPop   = false;

		return *this;
	}
}// namespace roingine
